#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

// Защита от повторного включения заголовочного файла (include guard)

#include "ChatProtocol.h"

// Подключение заголовочного файла ChatProtocol

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

// Подключение необходимых классов из библиотеки Qt:
// QObject - базовый класс для всех объектов Qt с поддержкой сигналов и слотов
// QTcpServer - класс для работы с TCP-сервером
// QTcpSocket - класс для работы с TCP-соединениями

class ServerManager : public QObject
{
    Q_OBJECT

    // Макрос Q_OBJECT необходим для всех классов, которые определяют сигналы и слоты
public:
    // Конструктор класса ServerManager, принимает порт для прослушивания соединений
    explicit ServerManager(ushort port = 4500, QObject *parent = nullptr);

    // Метод для уведомления других клиентов об изменении имени
    void notifyOtherClients(QString prevName, QString name);

public slots:
    // Слот для обработки текстовых сообщений для других клиентов
    void onTextForOtherClients(QString message, QString receiver, QString sender);

signals:
    // Сигнал о подключении нового клиента
    void newClientConnected(QTcpSocket *client);

    // Сигнал об отключении клиента
    void clientDisconnected(QTcpSocket *client);

private slots:
    // Слот, вызываемый при получении нового клиентского соединения
    void newClientConnectionReceived();

    // Слот, вызываемый при отключении клиента
    void onClientDisconnected();

private: // fields (поля)
    QTcpServer *_server;                  // Указатель на объект QTcpServer для управления сервером
    QMap<QString, QTcpSocket *> _clients; // Карта для хранения клиентов и их соответствующих сокетов
    ChatProtocol _protocol;               // Объект ChatProtocol для управления протоколом сообщений

private: // methods (методы)
    // Вспомогательный метод для настройки сервера
    void setupServer(ushort port);
};

#endif // SERVERMANAGER_H

// Завершение защиты от повторного включения заголовочного файла
// Это закрывает условие #ifndef, начатое в начале файла
