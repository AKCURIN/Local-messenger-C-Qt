#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H  // Препроцессорная директива для предотвращения множественного включения этого заголовочного файла.

#include "ChatProtocol.h"  // Включение заголовочного файла с описанием протокола чата.

#include <QObject>  // Включение класса QObject из библиотеки Qt для использования базовых возможностей Qt.
#include <QTcpSocket>  // Включение класса QTcpSocket из библиотеки Qt для работы с TCP-соединениями.

class ClientManager : public QObject  // Объявление класса ClientManager, наследующего QObject.
{
    Q_OBJECT  // Макрос Q_OBJECT необходим для использования сигналов и слотов в Qt.

public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);  // Конструктор класса ClientManager с параметрами IP-адреса и порта (по умолчанию localhost и порт 4500).

    void connectToServer();  // Метод для подключения к серверу.

    void sendMessage(QString message, QString receiver);  // Метод для отправки текстового сообщения.
    void sendName(QString name);  // Метод для отправки имени.
    void sendStatus(ChatProtocol::Status status);  // Метод для отправки статуса.
    void sendIsTyping();  // Метод для отправки сообщения о том, что пользователь печатает.

    void sendInitSendingFile(QString fileName);  // Метод для отправки инициализации отправки файла.
    void sendAcceptFile();  // Метод для отправки принятия файла.
    void sendRejectFile();  // Метод для отправки отклонения файла.

signals:
    void connected();  // Сигнал о подключении.
    void disconnected();  // Сигнал об отключении.
    // void dataReceived(QByteArray data);  // Сигнал о получении данных (закомментировано).
    void textMessageReceived(QString message);  // Сигнал о получении текстового сообщения.
    void isTyping();  // Сигнал о том, что пользователь печатает.
    void nameChanged(QString name);  // Сигнал об изменении имени.
    void statusChanged(ChatProtocol::Status status);  // Сигнал об изменении статуса.
    void rejectReceivingFile();  // Сигнал об отклонении получения файла.
    void initReceivingFile(QString clientName, QString fileName, qint64 fileSize);  // Сигнал об инициализации получения файла.

    void connectionACK(QString myName, QStringList clientsName);  // Сигнал о подтверждении подключения.
    void newClientConnectedToServer(QString clienName);  // Сигнал о новом подключенном клиенте.
    void clientNameChanged(QString prevName, QString clientName);  // Сигнал об изменении имени клиента.
    void clientDisconnected(QString clientName);  // Сигнал об отключении клиента.

private slots:
    void readyRead();  // Слот для обработки готовности к чтению данных.

private:  // Поля
    QTcpSocket *_socket;  // Указатель на объект QTcpSocket для работы с TCP-соединением.
    QHostAddress _ip;  // IP-адрес сервера.
    ushort _port;  // Порт сервера.
    ChatProtocol _protocol;  // Объект для работы с протоколом чата.
    QString _tmpFileName;  // Временное имя файла.

private:  // Методы
    void setupClient();  // Приватный метод для настройки клиента.
    void sendFile();  // Приватный метод для отправки файла.
};

#endif // CLIENTMANAGER_H  // Завершение директивы препроцессора для предотвращения множественного включения.
