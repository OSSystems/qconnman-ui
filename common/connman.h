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

#ifndef _CONNMAN_H
#define _CONNMAN_H

#include <QObject>
#include <QVariantMap>
#include <QDBusVariant>

class QDBusPendingCallWatcher;
class ConnmanManager;
class Technology;

class Connman: public QObject
{
    Q_OBJECT

public:
    void init();

    void registerAgent(const QString &path);

    QStringList availableTechnologies() const;
    QStringList connectedTechnologies() const;
    QStringList services() const;

    QString ethernetService();

    void enableTechnology(const QString &type);
    void disableTechnology(const QString &type);
    bool isTechnologyEnabled(const QString &type);
    bool isTechnologyConnected(const QString &type);
    QString technologyPath(const QString &type);

    void requestScan(const QString &type);

    QString serviceType(const QString &service);

    Technology *technology(const QString &path);

    static Connman *instance();

signals:
    void technologiesChanged();
    void enabledTechnologiesChanged();
    void connectedTechnologiesChanged();
    void servicesChanged();

private:
    Connman();

private slots:
    void processConnmanProperties(QDBusPendingCallWatcher *watcher);
    void propertyChanged(const QString &name, const QDBusVariant &value);
    void processNewTechnology(const QDBusObjectPath &obj, const QVariantMap &map);

private:
    ConnmanManager *m_manager;
    QVariantMap m_properties;
    QMap<QString, Technology *> m_technologyList;
};

#endif
