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


namespace bean {
GraphicsPolygonItem::GraphicsPolygonItem(const QPointF &offset, const QExplicitlySharedDataPointer< GraphicsPolygonItemPrivateData >& data, QGraphicsItem *parent)
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
    // The pen for this item
    auto p = s->pen(data(1).toULongLong());
    // Adjust screen size to scene size
    p.setWidthF(1 / level);
    if (thresholdMin >= 3) {     // Detail mode
        // The brush for this item
        auto b = s->brush(data(1).toULongLong());
        // Adjust screen size to scene size
        b.setTransform(QTransform(painter->worldTransform().inverted()));
        painter->setBrush(b);
        painter->setPen(p);
        // Apply composition mode
        painter->setCompositionMode(QPainter::CompositionMode(s->compositionMode()));
        painter->drawPolygon(d_ptr->m_data->m_polygon); // TODO, fillRule());
        if (s->itemFlags() & QGraphicsItem::ItemIsSelectable && option->state & QStyle::State_Selected) {
            // Draw a rectangle with dash line style on border, the margin is to keep that rectangle onto boundingRect
            const qreal margin = 1 / level;
            painter->setBrush(Qt::NoBrush);
            p.setWidthF(margin * 3); // In bold
            p.setStyle(Qt::DashLine);
            painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter->setPen(p);
            painter->drawPath(shape());
        }
    } else if (thresholdMin > 1) { // Fuzzy mode, just paint item's boundingRect
        painter->setPen(p);
        painter->drawRect(boundingRect());
    } else if (thresholdMax > 1) { // TODO Indication mode. However, currently draw one line instead
        painter->setPen(p);
        if (boundingRect().height() > boundingRect().width()) {
            painter->drawLine(boundingRect().topLeft(), boundingRect().bottomLeft());
        } else {
            painter->drawLine(boundingRect().topLeft(), boundingRect().topRight());
        }
    }
}

QRectF GraphicsPolygonItem::boundingRect() const
{
    // Override the default implementation to avoid padding
    return d_ptr->m_data->m_rect; //.translated(x(), y());
}

QPainterPath GraphicsPolygonItem::shape() const
{
    return d_ptr->m_data->m_painterPath;//.translated(x(), y());
}

QPainterPath GraphicsPolygonItem::opaqueArea() const
{
    auto s = static_cast<GraphicsScene *>(scene());
    auto b = s->brush(data(1).toULongLong());
    if (b.isOpaque()) {
        return shape();
    }
    return QPainterPath();
}
}