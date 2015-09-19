#include "mainwindow.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicsworker.h"
#include "layoutmodel.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFile>
#include <QToolBar>

#include <QFileDialog>
#include <QDir>

#include <QApplication>
#include <QDebug>

#include <QDockWidget>
#include <QListView>
#include <QTableView>
namespace bean {
mainwindow::mainwindow() : m_scene(new GraphicsScene), m_view(new GraphicsView(m_scene)), m_worker(new GraphicsWorker)
{
    // The central view
    setCentralWidget(m_view);
    centralWidget()->resize( {800, 600});
    m_worker->setScene(m_scene);

    // Menubar
    auto menu = menuBar()->addMenu("File");

    QAction *action = new QAction(this);
    action->setText("Open");
#if QT_VERSION >= 0x050000
    connect(action, &QAction::triggered, [this]() {
        QFileDialog fileDialog;
        if (fileDialog.exec()) {
            this->m_scene->clear();
            this->m_view->resetTransform();
            loadGdsFile(fileDialog.selectedFiles().first());
        }
    });
#else
    connect(action, SIGNAL(triggered()), this, SLOT(onOpenFile()));
#endif
    menu->addAction(action);

    action = new QAction(this);
    action->setText("Quit");
    connect(action, SIGNAL(triggered()), SLOT(close()));
    menu->addAction(action);
    // Toolbar
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    auto toolbar = new QToolBar("Edit", this);
    addToolBar(Qt::TopToolBarArea, toolbar);
    // QGraphicsItem::ItemIsSelectable
    auto selectableAction = toolbar->addAction("Selectable");
    selectableAction->setIcon(QIcon(":/images/edit-select.svg"));
    selectableAction->setCheckable(true);
#if QT_VERSION >= 0x050000
    connect(selectableAction, &QAction::triggered, [selectableAction, this]() {
        bool checked = selectableAction->isChecked();
        this->m_scene->setItemFlag(QGraphicsItem::ItemIsSelectable, checked);
    });
#endif
    auto movableAction = toolbar->addAction("Movable"); // FIXME wording
    movableAction->setIcon(QIcon(":/images/transform-move.svg"));
    movableAction->setCheckable(true);
#if QT_VERSION >= 0x050000
    connect(movableAction, &QAction::triggered, [movableAction, this]() {
        bool checked = movableAction->isChecked();
        for (auto item : this->m_scene->items()) {
            item->setFlag(QGraphicsItem::ItemIsMovable, checked);
        }
    });
#endif
//@{
    auto noDragAction = toolbar->addAction("No drag");
//     noDragAction->setIcon(QIcon("/usr/share/icons/gnome/32x32/actions/edit-copy.png"));
    noDragAction->setCheckable(true);
    auto rubberBandDragAction = toolbar->addAction("Rubber band drag");
    rubberBandDragAction->setIcon(QIcon(":/images/zoom-draw.svg"));
    rubberBandDragAction->setCheckable(true);
    auto scrollHandDragAction = toolbar->addAction("Scroll hand drag");
    scrollHandDragAction->setIcon(QIcon(":/images/zoom-draw.svg"));
    scrollHandDragAction->setCheckable(true);

    auto dragGroup = new QActionGroup(this);
    dragGroup->addAction(noDragAction);
    dragGroup->addAction(rubberBandDragAction);
    dragGroup->addAction(scrollHandDragAction);
    noDragAction->setChecked(true);
#if QT_VERSION >= 0x050000
    connect(dragGroup, &QActionGroup::triggered, [noDragAction, rubberBandDragAction, scrollHandDragAction, this](QAction * action) {
        if (action == noDragAction) {
            this->m_view->setDragMode(QGraphicsView::NoDrag);
        } else if (action == rubberBandDragAction) {
            this->m_view->setDragMode(QGraphicsView::RubberBandDrag);
        } else if (action == scrollHandDragAction) {
            this->m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        }
    });
#endif
//@}
    auto centerAction =  toolbar->addAction("Center on"); // FIXME wording
    centerAction->setIcon(QIcon(":/images/zoom-fit-best.svg"));
#if QT_VERSION >= 0x050000
    connect(centerAction, &QAction::triggered, [this]() {
        QRectF rect = this->m_scene->itemsBoundingRect();
        qDebug() << rect;
        this->m_view->setSceneRect(rect.adjusted(-1, -1, 1, 1));
        this->m_view->fitInView(rect, Qt::KeepAspectRatio);
    });
#else
    connect(centerAction, SIGNAL(triggered()), this, SLOT(onCenterOn()));
#endif
    auto dock = new QDockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    auto model = new LayoutModel(m_scene, this);
    auto v = new QTableView(this);
    v->setModel(model);
    dock->setWidget(v);
    resize( {800, 700});
}

mainwindow::~mainwindow()
{}

// TODO Move to other's

void mainwindow::loadGdsFile(const QString &fileName)
{
    qDebug() << fileName;
    QMetaObject::invokeMethod(this->m_worker, "loadGdsFile",  Qt::QueuedConnection, Q_ARG(QString, fileName));
}

#if QT_VERSION < 0x050000
void ::onOpenFile()
{
    QFileDialog fileDialog;
    if (fileDialog.exec()) {
        //FIXME
        this->m_scene->clear();
        loadGdsFile(fileDialog.selectedFiles().first());
        this->m_view->fitInView(this->m_view->sceneRect(), Qt::KeepAspectRatio);
    }
}
void ::onCenterOn()
{
    this->m_view->fitInView(this->m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}
#endif
}