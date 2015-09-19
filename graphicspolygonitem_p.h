#ifndef GRAPHICSPOLYGONITEM_P_H
#define GRAPHICSPOLYGONITEM_P_H
#include "graphicspolygonitem.h"


#include <QExplicitlySharedDataPointer>
namespace bean {
class GraphicsPolygonItemPrivateData : public QSharedData
{
   public:
    GraphicsPolygonItemPrivateData(const QPolygonF& polygon) {
            m_polygon = polygon;
            m_rect = polygon.boundingRect();
            m_painterPath.addPolygon(polygon);
    }
    GraphicsPolygonItemPrivateData(const GraphicsPolygonItemPrivateData &other)
        : QSharedData(other),m_polygon(other.m_polygon), m_rect(other.m_rect), m_painterPath(other.m_painterPath) { }
    ~GraphicsPolygonItemPrivateData() { }
    QPolygonF m_polygon;
    QRectF m_rect;
    QPainterPath m_painterPath;
};
/*!
 \class GraphicsPolygonItemPrivate
 holds data for QsvGraphicsPolygonItem
 
 \brief To save memory, QsvGraphicsPolygonItemPrivate doesn't retain QsvGraphicsPolygonItem's pointer,
 hence also make QsvGraphicsPolygonItemPrivate public accessible.
 \todo Complete setter and getter.
 */

class GraphicsPolygonItemPrivate
{
public:
    GraphicsPolygonItemPrivate(){}
    virtual ~GraphicsPolygonItemPrivate() {}
    QExplicitlySharedDataPointer<GraphicsPolygonItemPrivateData> m_data;
};
}
#endif //GRAPHICSPOLYGONITEM_H