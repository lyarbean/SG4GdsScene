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

#include "graphicspolygonitem_p.h"
#include "graphicsscene_p.h"

#include <QStyleOption>
#include <QPainter>
#include <QDebug>


namespace bean
{

/*!
 \brief This class is for all gds polygons. Note that polygons have the same shape share the same
 \c GraphicsPolygonItemPrivateData. To edit the shape, detach it tp make a deep copy first.
 */
GraphicsPolygonItem::GraphicsPolygonItem(const QPointF &offset, const QExplicitlySharedDataPointer< GraphicsPolygonItemPrivateData > &data, QGraphicsItem *parent)
    : QGraphicsItem(parent), d_ptr(new GraphicsPolygonItemPrivate)
{
    setCacheMode(NoCache);
    Q_D(GraphicsPolygonItem);
    setPos(offset);
    d->m_data = data;
}

void GraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//     if (!option->exposedRect.intersects(boundingRect())) {
//         return;
//     }
    qreal level = option->levelOfDetailFromTransform(painter->worldTransform());
    qreal thresholdMin = qMin(boundingRect().width(), boundingRect().height()) * level;
    qreal thresholdMax = qMax(boundingRect().width(), boundingRect().height()) * level;
    // The scene must be a QsvGraphicsScene
    auto s = static_cast<GraphicsScene *>(scene());
    quint64 stack = quint64(zValue());
    // The pen for this item
    auto p = s->pen(quint64(stack));
    // Adjust screen size to scene size
    p.setWidthF(1 / level);

    const QRectF &rect =  d_ptr->m_data.constData()->m_rect;
    painter->setCompositionMode(QPainter::CompositionMode(s->compositionMode()));
    if (thresholdMin >= 5) {     // Detail mode
        // The brush for this item
        auto b = s->brush(quint64(stack));
        // Adjust screen size to scene size
        b.setTransform(QTransform(painter->worldTransform().inverted()));
        painter->setBrush(b);
        painter->setPen(p);
        // Apply composition mode

        painter->drawPolygon(d_ptr->m_data.constData()->m_polygon); // TODO, fillRule());
        if (s->itemFlags() & QGraphicsItem::ItemIsSelectable && option->state & QStyle::State_Selected) {
            // Draw a rectangle with dash line style on border, the margin is to keep that rectangle onto boundingRect
            const qreal margin = 1 / level;
            painter->setBrush(Qt::NoBrush);
            p.setWidthF(margin * 3); // In bold
            p.setStyle(Qt::DashLine);
            p.setColor(Qt::black);
            painter->setPen(p);
            painter->drawPath(d_ptr->m_data.constData()->m_painterPath);
            qDebug() << mapToScene(rect).boundingRect();
            // painter->drawPath(shape()); // TODO if m_data is shared many times, save or fetch its shape in image
//             if (thresholdMin > 2) {
//                 painter->setMatrixEnabled(false);
//                 p.setColor(Qt::red);
//                 painter->setPen(p);
//                 QFont f;
//                 f.setPixelSize(12);
//                 painter->setFont(f);
//                 for (auto pp : d_ptr->m_data->m_polygon) {
//                     painter->drawText(pp+pos(),  QString("(%1,%2)").arg(pp.x()).arg(pp.y()));
//                 }
//                 painter->setMatrixEnabled(true);
//             }
        }
    } else if (thresholdMin > 2) { // Fuzzy mode 1
        painter->setPen(p);
        painter->drawRect(rect);
    } else if (thresholdMax > 1) { // Fuzzy mode 2
        painter->setPen(p);
        if (rect.height() > rect.width()) {
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
        } else {
            painter->drawLine(rect.topLeft(), rect.topRight());
        }
    } else { // TODO Indication mode
//         painter->setPen(Qt::DotLine);
//         painter->drawEllipse(boundingRect().adjusted(-1, -1, 1, 1));
    }
}

QRectF GraphicsPolygonItem::boundingRect() const
{
    // Override the default implementation to avoid padding
    return d_ptr->m_data.constData()->m_rect; //.translated(x(), y());
}

QPainterPath GraphicsPolygonItem::shape() const
{
    return d_ptr->m_data.constData()->m_painterPath;//.translated(x(), y());
}

QPainterPath GraphicsPolygonItem::opaqueArea() const
{
    auto s = static_cast<GraphicsScene *>(scene());
    auto b = s->brush(quint64(zValue()));
    if (b.isOpaque()) {
        return shape();
    }
    return QPainterPath();
}
}
