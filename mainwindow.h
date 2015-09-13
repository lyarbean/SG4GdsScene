#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace bean {
class GraphicsScene;
class GraphicsView;
class GraphicsWorker;

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow();
    virtual ~mainwindow();
    void loadGdsFile(const QString& fileName);
// signals:
//     void hasPolygon(quint64 index, const QPolygonF& polygon);
//     void hasPolygons(quint64 index, const QVector<QPolygonF>& polygons);
protected:

    
// virtual void wheelEvent(QWheelEvent * event);
private slots:
#if QT_VERSION < 0x050000
    void onOpenFile();
    void onCenterOn();
//     void onModeChanged();
#endif
private:
    GraphicsScene* m_scene;
    GraphicsView* m_view;
    GraphicsWorker* m_worker;
};
}
#endif // MAINWINDOW_H
