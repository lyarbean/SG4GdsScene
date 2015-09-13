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

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

namespace bean {
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
//     QsvGraphicsView ( QWidget *parent =0 );
    GraphicsView ( QGraphicsScene *scene, QWidget *parent = 0 );

    virtual ~GraphicsView();
protected:
    /**
     * @brief Handle Zoom in/out when wheeling
     *
     * @param event
     * @return void
     */
    virtual void wheelEvent ( QWheelEvent *event ) override;
    virtual void paintEvent ( QPaintEvent *event ) override;;
//     virtual void resizeEvent ( QResizeEvent *event ) override;;
private:
};
}
#endif // GRAPHICSVIEW_H
