#include "ClientManager.h"  // Включение заголовочного файла класса ClientManager.

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject(parent),
    _ip(ip),
    _port(port)
{
    setupClient();  // Вызов метода для настройки клиентского соединения.
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);  // Подключение к серверу по заданному IP и порту.
}

void ClientManager::sendMessage(QString message, QString receiver)
{
    _socket->write(_protocol.textMessage(message, receiver));  // Отправка текстового сообщения на сервер.
}

void ClientManager::sendName(QString name)
{
    _socket->write(_protocol.setNameMessage(name));  // Отправка имени пользователя на сервер.
}

void ClientManager::sendStatus(ChatProtocol::Status status)
{
    _socket->write(_protocol.setStatusMessage(status));  // Отправка статуса пользователя на сервер.
}

void ClientManager::sendIsTyping()
{
    _socket->write(_protocol.isTypingMessage());  // Отправка сообщения о том, что пользователь печатает.
}

void ClientManager::sendInitSendingFile(QString fileName)
{
    _tmpFileName = fileName;  // Сохранение имени файла для отправки.
    _socket->write(_protocol.setInitSendingFileMessage(fileName));  // Отправка сообщения о начале отправки файла на сервер.
}

void ClientManager::sendAcceptFile()
{
    _socket->write(_protocol.setAcceptFileMessage());  // Отправка сообщения о принятии файла на сервер.
}

void ClientManager::sendRejectFile()
{
    _socket->write(_protocol.setRejectFileMessage());  // Отправка сообщения о отказе в приеме файла на сервер.
}

void ClientManager::readyRead()
{
    auto data = _socket->readAll();  // Чтение всех данных, полученных от сервера.
    _protocol.loadData(data);  // Загрузка данных из полученного массива байтов в протокол.
    switch (_protocol.type()) {  // Начало переключения по типу сообщения.
    case ChatProtocol::Text:  // В случае получения текстового сообщения:
        emit textMessageReceived(_protocol.message());  // Генерация сигнала о получении текстового сообщения.
        break;  // Выход из переключения.
    case ChatProtocol::SetName:  // В случае установки имени пользователя:
        emit nameChanged(_protocol.name());  // Генерация сигнала об изменении имени пользователя.
        break;  // Выход из переключения.
    case ChatProtocol::SetStatus:  // В случае установки статуса пользователя:
        emit statusChanged(_protocol.status());  // Генерация сигнала об изменении статуса пользователя.
        break;  // Выход из переключения.
    case ChatProtocol::IsTyping:  // В случае сообщения о том, что пользователь печатает:
        emit isTyping();  // Генерация сигнала о начале ввода текста.
        break;  // Выход из переключения.
    case ChatProtocol::InitSendingFile:  // В случае начала отправки файла:
        emit initReceivingFile(_protocol.name(), _protocol.fileName(), _protocol.fileSize());  // Генерация сигнала о начале получения файла.
        break;  // Выход из переключения.
    case ChatProtocol::AcceptSendingFile:  // В случае принятия файла сервером:
        sendFile();  // Вызов метода для отправки файла.
        break;  // Выход из переключения.
    case ChatProtocol::RejectSendingFile:  // В случае отказа в приеме файла сервером:
        emit rejectReceivingFile();  // Генерация сигнала об отказе в приеме файла.
        break;  // Выход из переключения.
    case ChatProtocol::ConnectionACK:  // В случае подтверждения соединения с сервером:
        emit connectionACK(_protocol.myName(), _protocol.clientsName());  // Генерация сигнала о подтверждении соединения.
        break;  // Выход из переключения.
    case ChatProtocol::NewClient:  // В случае подключения нового клиента к серверу:
        emit newClientConnectedToServer(_protocol.clientName());  // Генерация сигнала о подключении нового клиента.
        break;  // Выход из переключения.
    case ChatProtocol::ClientDisconnected:  // В случае отключения клиента от сервера:
        emit clientDisconnected(_protocol.clientName());  // Генерация сигнала об отключении клиента.
        break;  // Выход из переключения.
    case ChatProtocol::ClientName:  // В случае изменения имени клиента:
        emit clientNameChanged(_protocol.prevName(), _protocol.clientName());  // Генерация сигнала об изменении имени клиента.
        break;  // Выход из переключения.
    default:  // В случае неизвестного типа сообщения:
        break;  // Выход из переключения.
    }
}

void ClientManager::setupClient()
{
    _socket = new QTcpSocket(this);  // Создание нового сокета для работы с TCP соединением.
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);  // Установка соединения сигнала подключения к серверу с соответствующим слотом.
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);  // Установка соединения сигнала отключения от сервера с соответствующим слотом.
    connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);  // Установка соединения сигнала готовности к чтению данных от сервера с соответствующим слотом.
}

void ClientManager::sendFile()
{
    _socket->write(_protocol.setFileMessage(_tmpFileName));  // Отправка файла на сервер.
}
