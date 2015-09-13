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

#include "graphicsscene_p.h"
#include "graphicspolygonitem.h"

#include <QCoreApplication>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QFile>  // loadGdsFile

namespace bean {
/*!
 \class QsvGraphicsScene
\fn resetBrushes
Clear all brushes for each index

\fn resetPens
Clear all pens for each index

*/

// Pens
QMap<quint64, QPen> GraphicsScene::pens() const
{
    return d_ptr->m_pens;
}
void GraphicsScene::setPens(QMap<quint64, QPen> pens)
{
    d_func()->m_pens = pens;
    emit pensChanged();
}
void GraphicsScene::resetPens()
{
    d_func()->m_pens.clear();
    emit pensChanged();
}

//Brushes
QMap<quint64, QBrush> GraphicsScene::brushes() const
{
    return d_ptr->m_brushes;
}
void GraphicsScene::setBrushes(QMap<quint64, QBrush> brushes)
{
    d_func()->m_brushes = brushes;
    emit brushesChanged();
}
void GraphicsScene::resetBrushes()
{
    d_func()->m_brushes.clear();
    emit brushesChanged();
}

// Pen
QPen GraphicsScene::pen(quint64 index) const
{
    if (d_ptr->m_pens.find(index) != d_ptr->m_pens.end()) {
        return d_ptr->m_pens[index];
    }
    QPen p(QColor(static_cast<Qt::GlobalColor>(index % 10 + 7)));
    p.setCapStyle(Qt::FlatCap);
    p.setJoinStyle(Qt::MiterJoin);
    return p;
}
void GraphicsScene::setPen(quint64 index, QPen pen)
{
    d_func()->m_pens.insert(index, pen);
    emit penChanged(index);
}
void GraphicsScene::resetPen(quint64 index)
{
    d_func()->m_pens.remove(index);
    emit penChanged(index);
}

// Brush
QBrush GraphicsScene::brush(quint64 index) const
{
    if (d_ptr->m_brushes.find(index) != d_ptr->m_brushes.end()) {
        return d_ptr->m_brushes [index];
    }
    return QBrush(QColor(Qt::GlobalColor(index % 10 + 7)),  Qt::BrushStyle(index % 15));
}
void GraphicsScene::setBrush(quint64 index, QBrush brush)
{
    d_func()->m_brushes.insert(index, brush);
    emit brushChanged(index);
}
void GraphicsScene::resetBrush(quint64 index)
{
    d_func()->m_brushes.remove(index);
    emit brushChanged(index);
}

int GraphicsScene::compositionMode() const
{
    return d_func()->m_compositionMode;
}

void GraphicsScene::setCompositionMode(int compositionMode)
{
    if (compositionMode >= QPainter::CompositionMode_SourceOver && compositionMode <= QPainter::RasterOp_SourceOrNotDestination) {
        d_ptr->m_compositionMode = compositionMode;
    }
}

QGraphicsItem::GraphicsItemFlag GraphicsScene::itemFlags() const
{
    return d_func()->m_itemFlags;
}

void GraphicsScene::setItemFlag(QGraphicsItem::GraphicsItemFlag flag, bool enabled)
{
    switch (flag) {
    case QGraphicsItem::ItemIsMovable:
    case QGraphicsItem::ItemIsSelectable:
    case QGraphicsItem::ItemIsFocusable:
        int f = d_ptr->m_itemFlags;
        if (enabled) {
            f |= flag;
            d_ptr->m_itemFlags = QGraphicsItem::GraphicsItemFlag(f);
        } else {
            f &= ~flag;
            d_ptr->m_itemFlags = QGraphicsItem::GraphicsItemFlag(f);
        }
        break;

    }
}


QRectF GraphicsScene::itemsBoundingRect()
{
    return QGraphicsScene::itemsBoundingRect();
}



GraphicsScene::GraphicsScene(QObject *parent): QGraphicsScene(parent), d_ptr(new GraphicsScenePrivate(this))
{
#if QT_VERSION >= 0x050000
    setMinimumRenderSize(.1);
#endif
    setBspTreeDepth(10); // Time preferred
}


GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::addItems(const QVariantList items)
{
    // Retain views
    auto v = views();
    for (auto view : v) {
        view->setScene(0);
    }
//     QCoreApplication::processEvents(); // Update on screen takes long time then do it now
    for (auto item : items) {
        addItem(item.value<QGraphicsItem *>());
    }
    for (auto view : v) {
        view->setScene(this);
    }
}

}



