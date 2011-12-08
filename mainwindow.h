#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "ui_mainwindow.h"

class MainWindow: public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void changePage(const QString &technology);

private:
    Ui::MainWindow ui;
    QMap<QString, QWidget *> m_pages;
};

#endif
