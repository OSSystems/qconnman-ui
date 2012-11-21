#include "widgetfilter.h"

#include <QEvent>
#include <QLabel>
#include <QDebug>

WidgetFilter::WidgetFilter(): QObject()
{
}

WidgetFilter *WidgetFilter::instance()
{
    static WidgetFilter *self = new WidgetFilter;
    return self;
}

bool WidgetFilter::eventFilter(QObject *obj, QEvent *e)
{
    return e->type() == QEvent::Paint;
}

void WidgetFilter::hide(QWidget *w)
{
    foreach (QWidget *child, w->findChildren<QWidget *>())
        hide(child);

    w->installEventFilter(this);
    w->update();
}

void WidgetFilter::unhide(QWidget *w)
{
    foreach (QWidget *child, w->findChildren<QWidget *>())
        unhide(child);

    w->removeEventFilter(this);
    w->update();
}

void WidgetFilter::hide()
{
    QObject *s = sender();
    if (s->isWidgetType())
        hide(qobject_cast<QWidget*>(s));
}

void WidgetFilter::unhide()
{
    QObject *s = sender();
    if (s->isWidgetType())
        unhide(qobject_cast<QWidget*>(s));
}
