#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>
#include <QStringList>

class Application: public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char *argv[], const QString &uniqueKey)
        : QApplication(argc, argv), m_uniqueKey(uniqueKey)
    {
        m_sharedMemory.setKey(m_uniqueKey);
        if (m_sharedMemory.attach())
            m_isRunning = true;
        else
        {
            m_isRunning = false;
            if (!m_sharedMemory.create(1))
                qFatal("Unable to create single instance");

            m_localServer = new QLocalServer(this);
            connect(m_localServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));
            m_localServer->listen(m_uniqueKey);
        }
    }

    bool isRunning()
    {
        return m_isRunning;
    }

    bool sendMessage(const QString &message)
    {
        if (!m_isRunning)
            return false;

        QLocalSocket localSocket(this);
        localSocket.connectToServer(m_uniqueKey, QIODevice::WriteOnly);
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

signals:
	void messageAvailable(QString message);

private:
    bool m_isRunning;
	QString m_uniqueKey;
	QSharedMemory m_sharedMemory;
	QLocalServer *m_localServer;
};

int main(int argc, char *argv[])
{
    Application app(argc, argv, "qconnman");

    QStringList args = app.arguments();
    args.removeFirst();

    if (!args.isEmpty() && args.at(0).contains(QRegExp("--?applet")))
    {
        qDebug("applet");
    }
    else
    {
        qDebug("manager");
    }

    return app.exec();
}

#include "main.moc"
