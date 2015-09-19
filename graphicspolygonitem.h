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

#ifndef GRAPHICSPOLYGONITEM_H
#define GRAPHICSPOLYGONITEM_H

#include <QGraphicsItem>
namespace bean {
class GraphicsPolygonItemPrivate;
class GraphicsPolygonItemPrivateData;

/*!
 \class GraphicsPolygonItem
 */
class GraphicsPolygonItem : public QGraphicsItem
{
public:
//     QsvGraphicsPolygonItem ( QGraphicsItem *parent = 0 );
    GraphicsPolygonItem ( const QPointF &offset, const QExplicitlySharedDataPointer<GraphicsPolygonItemPrivateData>& data, QGraphicsItem *parent = 0 );
    virtual void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) override;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual QPainterPath opaqueArea() const override;
    enum {
        Type = UserType + 8
    };
    // TODO Mouse events
private:
    Q_DECLARE_PRIVATE(GraphicsPolygonItem)
    const QScopedPointer<GraphicsPolygonItemPrivate>  d_ptr;
};
}
#endif // GRAPHICSPOLYGONITEM_H
