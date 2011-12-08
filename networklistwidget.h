#ifndef _NETWORKLISTWIDGET_H
#define _NETWORKLISTWIDGET_H

#include <QListWidget>

class NetworkListWidget: public QListWidget
{
    Q_OBJECT

public:
    NetworkListWidget(QWidget *parent);

    QString selectedNetwork() const;

private slots:
    void updateItems();
    void scan();
};

#endif
