#include <QApplication>
#include <QStringList>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDBusConnection>
#include <QFile>
#include <QDebug>

#include "connman.h"
#include "mainwindow.h"
#include "trayicon.h"

class Application: public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char *argv[])
        : QApplication(argc, argv)
    {
        m_isAlreadyRunning = !QDBusConnection::sessionBus().registerService("net.qconnman");
        if (!m_isAlreadyRunning)
        {
            QFile::remove("/tmp/qconnman");

            m_localServer = new QLocalServer(this);
            connect(m_localServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));
            m_localServer->listen("qconnman");

            connect(this, SIGNAL(messageAvailable(QString)), SLOT(processMessage(QString)));
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

    bool isAlreadyRunning()
    {
        return m_isAlreadyRunning;
    }

    void showApplet()
    {
        static TrayIcon *trayIcon = NULL;
        if (!trayIcon)
            trayIcon = new TrayIcon();
    }

    void showManager()
    {
        static MainWindow *window = NULL;
        if (!window)
            window = new MainWindow;
        window->show();
    }

private slots:
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

    void processMessage(const QString &msg)
    {
        if (msg == "show_manager")
            showManager();
        else if (msg == "show_applet")
            showApplet();
    }

private:
    QLocalServer *m_localServer;
    bool m_isAlreadyRunning;

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
        if (app.isAlreadyRunning())
        {
            app.sendMessage("show_manager");
            exit(0);
        }

        app.showManager();
    }
    else if (!args.isEmpty() && args.at(0).contains(QRegExp("--?applet$")))
    {
        if (app.isAlreadyRunning())
        {
            app.sendMessage("show_applet");
            exit(0);
        }

        app.showApplet();
    }
    else
        qFatal("Invalid option");

    return app.exec();
}

#include "main.moc"
