#ifndef GRAPHICSSCENE_P_H
#define GRAPHICSSCENE_P_H

#include "graphicsscene.h"

#include <QMap>
#include <QSet>
#include <QMutex>
namespace bean {
class GraphicsScenePrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(GraphicsScene)
public:
    explicit GraphicsScenePrivate(GraphicsScene* parent)
        : q_ptr(parent), m_compositionMode(26)/**, m_itemFlags(0)*/ {}
    virtual ~GraphicsScenePrivate() {}
private:
    GraphicsScene* const q_ptr;
    QMap<quint64, QBrush> m_brushes;
    QMap<quint64, QPen> m_pens;
    QMap<quint64, bool> m_stacks; // stack - visility
    QMap<quint64, QGraphicsItem*> m_blocks; // stack - visility
    int m_compositionMode;
    QGraphicsItem::GraphicsItemFlag m_itemFlags;
    QMutex m_mutex;
    QList <QGraphicsView *> m_views;
};
}
#endif // GRAPHICSSCENE_P_H

