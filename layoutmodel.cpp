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

#include "layoutmodel.h"
#include <QModelIndex>
#include <QDebug>
namespace bean
{

LayoutModel::LayoutModel(GraphicsScene *scene, QObject *parent) : QAbstractListModel(parent), m_scene(scene)
{
    if (m_scene) {
        auto stacks = m_scene->stacks();
        for (auto s : stacks) {
            m_stacks.append(s);
        }
        connect(m_scene, &GraphicsScene::stacksChanged, this, &LayoutModel::sceneUpdated);
    }
}

QVariant LayoutModel::data(const QModelIndex &index, int role) const
{
    if (!index.parent().isValid() && index.isValid()) {
        int row = index.row();
        int column = index.column();
        switch (role) {
        case Qt::DisplayRole: {
            if(column == 0) {
                quint64 k = m_stacks.at(row);
                quint32 k1 = k >> 32;
                quint32 k2 = k & 0xFFFF;
                return QString("%1:%2").arg(k1).arg(k2);
            }
        }
        case Qt::BackgroundRole:
            if(column == 1) {
                return  m_scene->brush(m_stacks.at(row));
            }
        case Qt::DecorationRole:
            if(column == 2) {
                return m_scene->pen(m_stacks.at(row)).color();
            }
        case Qt::CheckStateRole:
            if(column == 0) {
                return m_scene->isVisible(m_stacks.at(row));
            }
        }
    }

    return QVariant();
}

int LayoutModel::columnCount(const QModelIndex &parent) const
{
    return 4; // TODO
}

int LayoutModel::rowCount(const QModelIndex &parent) const
{
    return m_stacks.count();
}

QModelIndex LayoutModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

Qt::ItemFlags LayoutModel::flags(const QModelIndex &index) const
{
    auto f = QAbstractListModel::flags(index);
    if (!index.parent().isValid() && index.isValid()) {
        auto c = index.column();
        if (c == 0 ) {
            f |= Qt::ItemIsUserCheckable;
        }  else  if (c > 0 && c < 5) {
            f |= Qt::ItemIsEditable;
            f |= Qt::ItemIsSelectable;
        }
    }
    return f;
}

bool LayoutModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // TODO
    if (!index.parent().isValid() && index.isValid()) {
        int row = index.row();
        int column = index.column();
        if (column == 0 && role == Qt::CheckStateRole) {
            auto stack = m_stacks.at(row);
            bool v =  m_scene->isVisible(stack);
            m_scene->setVisible(stack, !v);
            emit dataChanged(index, index);
            return true;
        }
    }
    return QAbstractItemModel::setData(index, value, role);
}


void LayoutModel::sceneUpdated()
{
    beginResetModel();
    m_stacks = m_scene->stacks();
    endResetModel();

}

}
