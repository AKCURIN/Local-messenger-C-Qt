#include "ClientManager.h"

#include <QDir>

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},  // Инициализация базового класса QObject с указанием родительского объекта
    _ip(ip),             // Инициализация адреса сервера
    _port(port)           // Инициализация порта сервера
{
    _socket = new QTcpSocket(this);  // Создание нового объекта QTcpSocket и установка его родителя
    setupClient();                   // Вызов функции настройки клиента
}

ClientManager::ClientManager(QTcpSocket *client, QObject *parent)
    : QObject{parent},  // Инициализация базового класса QObject с указанием родительского объекта
    _socket(client)    // Инициализация существующего сокета
{
    setupClient();     // Вызов функции настройки клиента
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);  // Подключение к серверу по указанному адресу и порту
}

void ClientManager::disconnectFromHost()
{
    _socket->disconnectFromHost();  // Отключение от сервера
}

void ClientManager::sendMessage(QString message)
{
    _socket->write(_protocol.textMessage(message, name()));  // Отправка текстового сообщения через сокет
}

void ClientManager::sendName(QString name)
{
    _socket->write(_protocol.setNameMessage(name));  // Отправка сообщения с установкой имени пользователя
}

void ClientManager::sendStatus(ChatProtocol::Status status)
{
    _socket->write(_protocol.setStatusMessage(status));  // Отправка сообщения с установкой статуса
}

QString ClientManager::name() const
{
    auto id = _socket->property("id").toInt();  // Получение идентификатора клиента
    auto name = _protocol.name().length() > 0 ? _protocol.name() : QString("Client (%1)").arg(id);  // Получение имени клиента

    return name;  // Возврат имени клиента
}

void ClientManager::sendIsTyping()
{
    _socket->write(_protocol.isTypingMessage());  // Отправка сообщения о том, что пользователь печатает
}

void ClientManager::sendInitSendingFile(QString fileName)
{
    _tmpFileName = fileName;  // Установка имени временного файла
    _socket->write(_protocol.setInitSendingFileMessage(fileName));  // Отправка сообщения инициации отправки файла
}

void ClientManager::sendAcceptFile()
{
    _socket->write(_protocol.setAcceptFileMessage());  // Отправка сообщения подтверждения принятия файла
}

void ClientManager::sendRejectFile()
{
    _socket->write(_protocol.setRejectFileMessage());  // Отправка сообщения отказа в принятии файла
}

void ClientManager::readyRead()
{
    auto data = _socket->readAll();  // Чтение всех доступных данных из сокета
    _protocol.loadData(data);  // Загрузка данных в протокол

    switch (_protocol.type()) {  // Проверка типа полученного сообщения по протоколу
    case ChatProtocol::Text:
        emit textMessageReceived(_protocol.message(), _protocol.receiver());  // Эмитирование сигнала о получении текстового сообщения
        break;
    case ChatProtocol::SetName:{
        auto prevName = _socket->property("clientName").toString();  // Получение предыдущего имени клиента
        _socket->setProperty("clientName", name());  // Установка нового имени клиента в свойство сокета
        emit nameChanged(prevName, name());  // Эмитирование сигнала о смене имени клиента
        break;
    }
    case ChatProtocol::SetStatus:
        emit statusChanged(_protocol.status());  // Эмитирование сигнала о изменении статуса клиента
        break;
    case ChatProtocol::IsTyping:
        emit isTyping();  // Эмитирование сигнала о том, что клиент печатает
        break;
    case ChatProtocol::InitSendingFile:
        emit initReceivingFile(_protocol.name(), _protocol.fileName(), _protocol.fileSize());  // Эмитирование сигнала о начале получения файла
        break;
    case ChatProtocol::AcceptSendingFile:
        sendFile();  // Вызов функции отправки файла
        break;
    case ChatProtocol::RejectSendingFile:
        emit rejectReceivingFile();  // Эмитирование сигнала об отказе в получении файла
        break;
    case ChatProtocol::SendFile:
        saveFile();  // Вызов функции сохранения файла
        break;
    default:
        break;
    }
}

void ClientManager::setupClient()
{
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);  // Установка соединения сигнала о подключении
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);  // Установка соединения сигнала об отключении
    connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);  // Установка соединения сигнала о готовности к чтению данных
}

void ClientManager::sendFile()
{
    _socket->write(_protocol.setFileMessage(_tmpFileName));  // Отправка файла через сокет
}

void ClientManager::saveFile()
{
    QDir dir;  // Создание объекта для работы с директориями
    dir.mkdir(name());  // Создание директории с именем клиента
    auto path = QString("%1/%2/%3_%4")
                    .arg(dir.canonicalPath(), name(), QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"), _protocol.fileName());  // Генерация пути для сохранения файла
    QFile file(path);  // Создание объекта файла
    if (file.open(QIODevice::WriteOnly)) {  // Попытка открыть файл для записи
        file.write(_protocol.fileData());  // Запись данных файла
        file.close();  // Закрытие файла
        emit fileSaved(path);  // Эмитирование сигнала о сохранении файла с указанием пути
    }
}
