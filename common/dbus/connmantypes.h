#ifndef _CONNMANTYPES_H
#define _CONNMANTYPES_H

#include <QObject>
#include <QList>

struct TechnologyStruct
{
	QDBusObjectPath obj;
	QVariantMap map;
};

Q_DECLARE_METATYPE(TechnologyStruct);
Q_DECLARE_METATYPE(QList<TechnologyStruct>);

typedef QList<TechnologyStruct> TechnologyList;

#endif

