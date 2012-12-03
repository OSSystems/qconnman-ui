#ifndef _WIDGETFILTER_H
#define _WIDGETFILTER_H

#include <QWidget>

class WidgetFilter: public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    WidgetFilter();

public:
    static WidgetFilter *instance();

    void hide(QWidget *w);
    void unhide(QWidget *w);

public slots:
    void hide();
    void unhide();
};

#endif
