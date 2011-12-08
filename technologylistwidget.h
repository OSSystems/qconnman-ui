#ifndef _TECHNOLOGYLISTWIDGET_H
#define _TECHNOLOGYLISTWIDGET_H

#include <QListWidget>

class TechnologyListWidget: public QListWidget
{
    Q_OBJECT

public:
    TechnologyListWidget(QWidget *parent = 0);

signals:
    void itemChanged(const QString &technology);

private slots:
    void updateItems();
    void emitItemChanged(int row);
};

#endif

