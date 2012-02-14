#ifndef _CONNMANTYPES_H
#define _CONNMANTYPES_H

#include <QObject>
#include <QList>

struct ConnmanObject
{
	QDBusObjectPath path;
	QVariantMap properties;
};

Q_DECLARE_METATYPE(ConnmanObject);
Q_DECLARE_METATYPE(QList<ConnmanObject>);

typedef QList<ConnmanObject> ConnmanTuple;

#endif

