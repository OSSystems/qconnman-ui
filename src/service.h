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

#ifndef _SERVICE_H
#define _SERVICE_H

#include <QObject>
#include <QDBusVariant>

class QDBusPendingCallWatcher;
class ConnmanService;

class Service: public QObject
{
    Q_OBJECT

public:
    Service(const QString &path, QObject *parent);

    QString path() const;
    QString name() const;
    bool isHidden() const;
    QString state() const;
    QString type() const;
    QStringList security() const;
    QString passphrase() const;
    void setPassphrase(const QString &passphrase);
    quint8 strength() const;

    void setAutoConnect(bool value);

    QVariantMap ipv4Settings();
    void setIpv4Settings(const QVariantMap &map);

    void connect();
    void disconnect();

signals:
    void propertiesChanged();
    void stateChanged();

private slots:
    void processProperties(QDBusPendingCallWatcher *watcher);
    void propertyChanged(const QString &name, const QDBusVariant &value);

private:
    ConnmanService *m_service;
    QVariantMap m_properties;
};

#endif
