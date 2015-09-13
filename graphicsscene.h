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

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMap>

namespace bean {
class GraphicsItem;
class GraphicsScenePrivate;


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
    // Use the following typedef's to work around Q_PROPERTY, please refer to The Property System
    typedef QMap<quint64, QPen> Pens;
    typedef QMap<quint64, QBrush> Brushes;
    Q_PROPERTY(Pens m_pens READ pens WRITE setPens RESET resetPens NOTIFY pensChanged)
    Q_PROPERTY(Brushes m_brushes READ brushes WRITE setBrushes RESET resetBrushes NOTIFY brushesChanged)
    Q_PROPERTY(int m_compositionMode READ compositionMode WRITE setCompositionMode) // TODO NOTIFY
public:

    GraphicsScene ( QObject *parent =0 );
    virtual ~GraphicsScene();

    // Pens
    QMap<quint64, QPen> pens() const;
    void setPens(QMap<quint64, QPen> pens);
    void resetPens();
    // Brushes
    QMap<quint64, QBrush> brushes() const;
    void setBrushes(QMap<quint64, QBrush> brushes);
    void resetBrushes();
    // Pen
    QPen pen(quint64 index) const;
    void setPen(quint64 index, QPen pen);
    void resetPen(quint64);
    // Brush
    QBrush brush(quint64 index) const;
    void setBrush(quint64 index, QBrush brush);
    void resetBrush(quint64 index);
    // CompositionMode
    int compositionMode() const;
    void setCompositionMode(int compositionMode);
    // ItemFlag
    // Use this flag as a global flag to decide whether item's flag
    // To work with move/focus, we need to override mousePressEvent/mousePressEvent
    QGraphicsItem::GraphicsItemFlag itemFlags() const;
    Q_INVOKABLE void setItemFlag(QGraphicsItem::GraphicsItemFlag flag, bool enabled = true);

    // INVOKABLEs
    Q_INVOKABLE QRectF itemsBoundingRect();

signals:
    void pensChanged();
    void brushesChanged();
    void penChanged(quint64 index);
    void brushChanged(quint64 index);
public slots:
    void addItems (const QVariantList items);
// protected:
//     virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
//     virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
//     virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
//     void render(QPainter * painter, const QRectF & target = QRectF(), const QRectF & source = QRectF(), Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio);
private:
    GraphicsScene ( const GraphicsScene &other ) = delete;
    GraphicsScene &operator= ( const GraphicsScene &other ) = delete;
    bool operator== ( const GraphicsScene &other ) const = delete;

private:
    Q_DECLARE_PRIVATE(GraphicsScene)
    const QScopedPointer<GraphicsScenePrivate>  d_ptr;
};
}
#endif // GRAPHICSSCENE_H
