#ifndef _TECHNOLOGY_H
#define _TECHNOLOGY_H

#include <QObject>
#include <QDBusVariant>

class QDBusPendingCallWatcher;
class ConnmanTechnology;

class Technology: public QObject
{
    Q_OBJECT

public:
    Technology(const QString &path, QObject *parent);

    QString state();

signals:
    void propertiesChanged();
    void stateChanged();

private slots:
    void processProperties(QDBusPendingCallWatcher *watcher);
    void propertyChanged(const QString &name, const QDBusVariant &value);

private:
    ConnmanTechnology *m_technology;
    QVariantMap m_properties;
};

#endif
