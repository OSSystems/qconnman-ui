#ifndef _CONNMAN_H
#define _CONNMAN_H

#include <qconnman/manager.h>

class ConnMan: public Manager
{
public:
    ConnMan(QObject *parent);

protected:
    inline virtual QStringList ignoredTechnologies() const { return QStringList() << "bluetooth"; }
    inline virtual bool ignoreHiddenNetworks() const { return true; }

    virtual QIcon serviceIcon(Service *service) const;
};

#endif
