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

#ifndef LAYOUTMODEL_H
#define LAYOUTMODEL_H

#include "graphicsscene.h"
#include <QAbstractItemModel>


namespace bean
{

class LayoutModel : public QAbstractListModel
{
    Q_OBJECT

public:
    LayoutModel (GraphicsScene * scene, QObject *parent = 0);
    virtual QVariant data ( const QModelIndex &index, int role ) const override;
    virtual int columnCount ( const QModelIndex &parent ) const override;
    virtual int rowCount ( const QModelIndex &parent ) const override;
    virtual QModelIndex parent ( const QModelIndex &child ) const override;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const override;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
private slots:
    void sceneUpdated();
private:
    // pen, brush
    GraphicsScene* const m_scene;
    QList<quint64> m_stacks;
};
}

#endif // LAYOUTMODEL_H
