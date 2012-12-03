/*
 * qconnman - Connman Applet
 * Copyright (C) 2011 O.S. Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "ui_mainwindow.h"

class Manager;
class Agent;

class MainWindow: public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void changePage(const QModelIndex &technology);
    void reportError();

private:
    Ui::MainWindow ui;
    Manager *m_manager;
    Agent *m_agent;
    QMap<QModelIndex, QWidget *> m_pages;
};

#endif
