#include <QApplication>
#include <QStringList>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDBusConnection>
#include <QDebug>

#include "connman.h"

class Application: public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char *argv[])
        : QApplication(argc, argv)
    {
        bool isAlreadyRunning = !QDBusConnection::sessionBus().registerService("net.qconnman");
        if (!isAlreadyRunning)
        {
            Connman::instance()->init();

            m_localServer = new QLocalServer(this);
            connect(m_localServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));
            m_localServer->listen("qconnman");
        }
    }

    bool sendMessage(const QString &message)
    {
        QLocalSocket localSocket(this);
        localSocket.connectToServer("qconnman", QIODevice::WriteOnly);
        if (!localSocket.waitForConnected())
            return false;

        localSocket.write(message.toUtf8());
        if (!localSocket.waitForBytesWritten())
            return false;

        localSocket.disconnectFromServer();
        return true;
    }

public slots:
    void receiveMessage()
    {
        QLocalSocket *localSocket = m_localServer->nextPendingConnection();
        if (!localSocket->waitForReadyRead())
            return;

        QByteArray byteArray = localSocket->readAll();
        QString message = QString::fromUtf8(byteArray.constData());
        emit messageAvailable(message);
        localSocket->disconnectFromServer();
    }

private:
    QLocalServer *m_localServer;

signals:
       void messageAvailable(QString message);
};

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator *appTranslator = new QTranslator;
    appTranslator->load(QString("%1/../share/qconnman/i18n/%2")
                        .arg(QCoreApplication::applicationDirPath())
                        .arg(QLocale::system().name()));

    QStringList args = app.arguments();
    args.removeFirst();

    if (!args.isEmpty() && args.at(0).contains(QRegExp("--?manager$")))
    {
        qDebug("manager");
    }
    else if (!args.isEmpty() && args.at(0).contains(QRegExp("--?applet$")))
    {
        qDebug("applet");
    }
    else
        qFatal("Invalid option");

    return app.exec();
}

#include "main.moc"
