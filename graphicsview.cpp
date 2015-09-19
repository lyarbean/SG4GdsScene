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

#include "graphicsview.h"

#include <QWheelEvent>
#include <QDebug>
#include <QTime>
#include <QtMath>
namespace bean {
GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent): QGraphicsView(scene, parent)
{
//     setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

GraphicsView::~GraphicsView()
{
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
//     qDebug() << QDateTime::currentDateTime();
    //TODO better expression
    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
#if QT_VERSION >= 0x050000
            && event->angleDelta().x() == 0
#endif
    ) {
        double by = 1.f;
#if QT_VERSION >= 0x050000
        double angle = event->angleDelta().y(); // +- 120
        //TODO
        if (angle !=  0) {
            by = qPow(1.5f, angle/120);
        }
#else
        int angle = event->delta(); //
        if (angle >  0) {
            by = 2.f;
        } else if (angle < 0) {
            by = .5f;
        }
#endif
        if (!((transform().m11() <= 1 && by < 1.) || (transform().m11() >= 2048 && by > 1.))) {
            setTransform(QTransform::fromScale(by, by), true);
        }
        event->accept();
//         if (scene())
//         scene()->setSceneRect(mapToScene(viewport()->rect()).boundingRect());
        return;
    }

//     if ((event->modifiers()&Qt::ControlModifier) != Qt::ControlModifier) {
    /* no scrolling while control is held */
    QGraphicsView::wheelEvent(event);
//     }
}


void GraphicsView::paintEvent(QPaintEvent *event)
{
    static QTime tickTime;
    tickTime.restart();
    QGraphicsView::paintEvent(event);
    qDebug() << "Paint view:" << tickTime.elapsed() << "milliseconds";
}

// void QsvGraphicsView::resizeEvent(QResizeEvent *event)
// {
//     QGraphicsView::resizeEvent(event);
// //     auto r = mapToScene(QRect(QPoint(),event->size())).boundingRect();
// }

}