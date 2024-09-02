#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

// Защита от повторного включения заголовочного файла (include guard)

#include "ChatProtocol.h"

// Подключение заголовочного файла ChatProtocol

#include <QObject>
#include <QTcpSocket>

// Подключение необходимых классов из библиотеки Qt:
// QObject - базовый класс для всех объектов Qt с поддержкой сигналов и слотов
// QTcpSocket - класс для работы с TCP-соединениями

class ClientManager : public QObject
{
    Q_OBJECT

    // Макрос Q_OBJECT необходим для всех классов, которые определяют сигналы и слоты
public:
    // Первый конструктор класса ClientManager, принимает IP-адрес и порт для подключения к серверу
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);

    // Второй конструктор класса ClientManager, принимает указатель на QTcpSocket для управления соединением клиента
    explicit ClientManager(QTcpSocket *client, QObject *parent = nullptr);

    // Метод для подключения к серверу
    void connectToServer();

    // Метод для отключения от сервера
    void disconnectFromHost();

    // Метод для отправки текстового сообщения
    void sendMessage(QString message);

    // Метод для отправки имени пользователя
    void sendName(QString name);

    // Метод для отправки статуса пользователя
    void sendStatus(ChatProtocol::Status status);

    // Метод для получения имени пользователя
    QString name() const;

    // Метод для отправки сообщения о том, что пользователь набирает текст
    void sendIsTyping();

    // Метод для инициализации отправки файла
    void sendInitSendingFile(QString fileName);

    // Метод для принятия файла
    void sendAcceptFile();

    // Метод для отклонения файла
    void sendRejectFile();

signals:
    // Сигнал о подключении к серверу
    void connected();

    // Сигнал об отключении от сервера
    void disconnected();

    // Сигнал о получении текстового сообщения
    void textMessageReceived(const QString message, QString receiver);

    // Сигнал о том, что пользователь набирает текст
    void isTyping();

    // Сигнал об изменении имени пользователя
    void nameChanged(QString prevName, QString name);

    // Сигнал об изменении статуса пользователя
    void statusChanged(ChatProtocol::Status status);

    // Сигнал об отклонении получения файла
    void rejectReceivingFile();

    // Сигнал о инициализации получения файла
    void initReceivingFile(QString clientName, QString fileName, qint64 fileSize);

    // Сигнал о сохранении файла
    void fileSaved(QString path);

private slots:
    // Слот, вызываемый при готовности сокета к чтению данных
    void readyRead();

private: // fields (поля)
    QTcpSocket *_socket;      // Указатель на объект QTcpSocket для управления TCP-соединением
    QHostAddress _ip;         // IP-адрес сервера
    ushort _port;             // Порт сервера
    ChatProtocol _protocol;   // Объект ChatProtocol для управления протоколом сообщений
    QString _tmpFileName;     // Временное имя файла

private: // methods (методы)
    // Вспомогательный метод для настройки клиента
    void setupClient();

    // Вспомогательный метод для отправки файла
    void sendFile();

    // Вспомогательный метод для сохранения файла
    void saveFile();
};

#endif // CLIENTMANAGER_H

// Завершение защиты от повторного включения заголовочного файла
// Это закрывает условие #ifndef, начатое в начале файла
