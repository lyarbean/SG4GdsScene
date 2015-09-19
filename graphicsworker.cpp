/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <beyan@briontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "graphicsworker.h"

#include <QDebug>
#include <QFile>
#include <QVariant>
#include <QTime>

namespace bean
{
GraphicsWorker::GraphicsWorker(QObject *parent) : m_scene(0)
{
    moveToThread(&m_thread);
    m_thread.start();
}

void GraphicsWorker::setScene(GraphicsScene *scene)
{
    m_scene = scene;
}

// TODO Reentrant
// Calling this function is not safe if the worker is busy.
void GraphicsWorker::loadGdsFile(const QString &fileName)
{
    QTime time;
    time.start();
    m_items.clear();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open " << fileName;
        return;
    }
    // TODO async
    QByteArray stream = file.readAll();
    // Parse

    if (stream.isEmpty()) {
        qDebug() << "Empty";
        return;
    }
    int current_layer = stream.indexOf("LAYE", 0);
    if (current_layer == -1) {
        qDebug() << "NO LAYER";
        return;
    }
    int next_layer = 0;
    const char *d = stream.data();
    const char *l = d + stream.size();
    // begin loop it
    quint64 layer_data_type = -1;
    qreal count = 0;
    QVector<QVector<QPointF>> polygons;
    polygons.reserve(1e7);
    while (d < l) {
        QByteArray type(d, 4);
        if (type.at(0) == '\0') {
            qDebug() << "Oops";
            break;
        }
        d += 4;
        if (type == "LAYE") {
            quint64 t = *((quint64 *)(d));
            d += 8;
            if (layer_data_type != t) {
                if (!polygons.empty()) {
                    addPolygons(layer_data_type, polygons);
                    polygons.clear();
                }
                layer_data_type = t;
            }
        } else if (type == "LINE") {
            int point_amount = *((int *)(d));
            d += 4;
            if (point_amount) {
                QVector<QPointF> p;
                for (int i = 0; i < point_amount; ++i) {
                    double x = *((double *)(d));
                    double y = *((double *)(d + 8));
                    d += 16;
                    p.append( {x, y});
                }
                polygons.append(p);
            }
        } else if (type == "POLY") {
            int point_amount = *((int *)(d));
            d += 4;
            if (point_amount) {
                QVector<QPointF> p;
                for (int i = 0; i < point_amount; ++i) {
                    double x = *((double *)(d));
                    double y = *((double *)(d + 8));
                    d += 16;
                    p.append( {x, y});
                }
                polygons.append(p);
            }
        }
    }

    if (!polygons.empty()) {
        addPolygons(layer_data_type, polygons);
    }
    quint64 c = 0;
    for (auto & i : m_itemData) {
        c += i.count();
    }
    quint64 c2 = 0;
    for (auto & i : m_items) {
        c2 += i.count();
    }
    qDebug() << c << m_itemData.count() << m_items.count() << c2;
    qDebug() << "Item creation: " << time.elapsed();
    time.restart();
    QMetaObject::invokeMethod(m_scene, "detachViews", Qt::BlockingQueuedConnection);
    qDebug() << "Detach views: " << time.elapsed();
    time.restart();
    for (auto k : m_items.keys()) {
        QMetaObject::invokeMethod(m_scene, "addItems", Qt::BlockingQueuedConnection, Q_ARG(QItemVector, m_items[k]), Q_ARG(quint64, k));
    }
    qDebug() << "Item insertion: " << time.elapsed();
    time.restart();
    QMetaObject::invokeMethod(m_scene, "attachViews", Qt::BlockingQueuedConnection);
    qDebug() << "Attach views: " << time.elapsed();
    

}

void GraphicsWorker::addPolygons(quint64 index, const QVector<QVector<QPointF> > &polygons)
{
    static int newCounter = 0;
    static int newCounter2 = 0;
//     QVariantList items;
    QItemVector &items = m_items[index];
//     items.reserve(polygons.size());
//     qDebug() << index << polygons.size();
    // FIXME It's too slow to create items
    for (auto & polygon : polygons) {
        QExplicitlySharedDataPointer<GraphicsPolygonItemPrivateData> d;
        QPolygonF p(polygon);
        auto offset = p.boundingRect().topLeft();
        p = p.translated(-offset);
//         auto r = p.boundingRect();
//         auto &itemData = m_itemData[quint64(r.width()*r.height())];
//         for (auto & data : itemData) {// TODO When m_itemData is large, performance gets impacted
//             if (data->m_polygon == p) {
//                 d = data;
//                 break;
//             }
//         }
//         if (!d) {
        d = new GraphicsPolygonItemPrivateData(p);
//             itemData.append(d);
//         }
        auto item = new GraphicsPolygonItem(offset, d);
        item->setFlags(QGraphicsItem::ItemIsSelectable);
        item->setZValue(index);
        items.append(item);
    }
//     qDebug() << index << items.count();
    // TODO add to temparory list, group into 9 areas
//     QMetaObject::invokeMethod(m_scene, "addItems", Qt::BlockingQueuedConnection, Q_ARG(QItemVector, items), Q_ARG(quint64, index));
//     if (items.size() > 1e4) {
//         QThread::usleep(10);
//     }
//     QThread::yieldCurrentThread();
}

}
