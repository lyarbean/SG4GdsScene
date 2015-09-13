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
#include "graphicsscene.h"

#include <QDebug>
#include <QFile>
#include <QVariant>

namespace bean {
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
    QMutexLocker locker(&m_mutex);
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
    // TODO use QMap<quint64, QPolygonF> map to cache as many as polygons and then emit at once
    QVector<QPolygonF> polygons;
    while (d < l) {
//         if (count > 500000) {
//             //qDebug() << "Too many";
//             // return;
//         }
        QByteArray type(d, 4);
        if (type.at(0) == '\0') {
            qDebug() << "Oops";
            break;
        }
        d += 4;
        if (type == "LAYE") {
            quint64 t = *((quint64 *)(d));
            if (layer_data_type != t) {
                if (!polygons.empty()) {
//                     emit hasPolygons(layer_data_type, polygons);
                    addPolygons(layer_data_type, polygons);
                    polygons.clear();
                }
                layer_data_type = t;
            }
            d += 8;
        } else if (type == "LINE") {
            int point_amount = *((int *)(d));
            d += 4;
            if (point_amount) {
                // QJsonArray line_json;
                QPolygonF p;
                for (int i = 0; i < point_amount; ++i) {
                    double x = *((double *)(d));
                    double y = *((double *)(d + 8));
                    d += 16;
                    count++;
                    p.append( {x, y});
                }
                //                 if (point_amount > 2) {
                //                     qDebug() << "L->P: " << point_amount << "points";
                polygons.append(p);
                // emit hasPolygon(layer_data_type, p);
                //                 } else {
                //                     qDebug() << "L: " << point_amount << "points";
                // lines_samples[layer_data_type].append(p);
                //emit hasPolygon(layer_data_type, p);
                //                 }
            } else {
                //                 qDebug() << "L: Empty";
            }
        } else if (type == "POLY") {
            int point_amount = *((int *)(d));
            d += 4;
            if (point_amount) {
                //                 qDebug() << "P: " << point_amount << "points";
                QPolygonF p;
                //  QJsonArray polygon_json;
                for (int i = 0; i < point_amount; ++i) {
                    double x = *((double *)(d));
                    double y = *((double *)(d + 8));
                    d += 16;
                    count++;
                    p.append( {x, y});
                }
                // polygons_samples[layer_data_type].append(p);
                polygons.append(p);
                //emit hasPolygon(layer_data_type, p);
            } else {
                //                 qDebug() << "P: Empty";
            }
        } else {
            //             qDebug() << "kk " << type;
        }
    }
    if (!polygons.empty()) {
        addPolygons(layer_data_type, polygons);
    }
    if (!m_items.empty()) {
        if (m_scene) {
            QMetaObject::invokeMethod(m_scene, "addItems", Qt::BlockingQueuedConnection, Q_ARG(QVariantList, m_items));
            m_items.clear();
        }
    }
    m_itemData.clear();
    qDebug() << count;
}

void GraphicsWorker::addPolygons(quint64 index, const QVector< QPolygonF > &polygons)
{
    static int newCounter = 0;
    static int newCounter2 = 0;
    for (auto polygon : polygons) {
        QExplicitlySharedDataPointer<GraphicsPolygonItemPrivateData> d;
        auto offset = polygon.boundingRect().topLeft();
        auto p = polygon.translated(-offset);
        for (auto data : m_itemData) {
            if (data->m_polygon == p) {
                d = data;
//                 newCounter2++;
                break;
            }
        }
        if (!d) {
//             newCounter++;
            d = new GraphicsPolygonItemPrivateData(p);
            m_itemData.append(d);
        }
        auto item = new GraphicsPolygonItem(offset, d);
        item->setData(1, index);
        item->setFlags(QGraphicsItem::ItemIsSelectable);
        m_items.append(QVariant::fromValue<QGraphicsItem *>(item));
    }
    if (m_items.size() > 1e5) {
        if (m_scene) {
            QMetaObject::invokeMethod(m_scene, "addItems", Qt::BlockingQueuedConnection, Q_ARG(QVariantList, m_items));
            m_items.clear();
        }
    }
//     qDebug() << newCounter << newCounter2;
}
}