#ifndef GRAPHICSSCENE_P_H
#define GRAPHICSSCENE_P_H

#include "graphicsscene.h"

#include <QMap>

namespace bean {
class GraphicsScenePrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(GraphicsScene)
public:
    explicit GraphicsScenePrivate(GraphicsScene* parent): q_ptr(parent), m_compositionMode(0)/**, m_itemFlags(0)*/ {}
    virtual ~GraphicsScenePrivate() {}
private:
    GraphicsScene* const q_ptr;
    QMap<quint64, QBrush> m_brushes;
    QMap<quint64, QPen> m_pens;
    int m_compositionMode;
    QGraphicsItem::GraphicsItemFlag m_itemFlags;
};
}
#endif // GRAPHICSSCENE_P_H

