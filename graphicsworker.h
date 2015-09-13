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

#ifndef GRAPHICSWORKER_H
#define GRAPHICSWORKER_H

#include "graphicspolygonitem_p.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QExplicitlySharedDataPointer>

namespace bean {
class GraphicsScene;

/*!
  \class QsvGraphicsWorker
   \brief QsvGraphicsWorker is a class that used to load gds file and create QsvGraphicsPolygonItems.
   QsvGraphicsWorker runs at its own thread, and  populate items to QsvGraphicsScene using BlockingQueuedConnection,
   so that the main thread that QsvGraphicsScene lives receives items in sequence.

*/
 
class GraphicsWorker : public QObject
{
    Q_OBJECT

public:
    GraphicsWorker ( QObject *parent = 0);
    void setScene(GraphicsScene* scene);
    // TODO Slot
    Q_INVOKABLE void loadGdsFile(const QString &fileName);
// signals:
//     void hasPolygon(quint64 index, const QPolygonF& polygon);
//     void hasPolygons(quint64 index, const QVector<QPolygonF>& polygons);
// public slots:
//     void onHasPolygon ( quint64 index, const QPolygonF &polygon );
//     void onHasPolygons ( quint64 index, const QVector<QPolygonF> &polygons );
private:
    void addPolygons ( quint64 index, const QVector<QPolygonF> &polygons );
private:
    GraphicsScene* m_scene;
    QThread m_thread;
    QMutex m_mutex;
    QVariantList m_items;
    QList<QExplicitlySharedDataPointer<GraphicsPolygonItemPrivateData> > m_itemData;
};
}
#endif // GRAPHICSWORKER_H
