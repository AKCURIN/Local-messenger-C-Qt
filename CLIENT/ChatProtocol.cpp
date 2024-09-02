#include "ChatProtocol.h"  // Включение заголовочного файла класса ChatProtocol.

#include <QFileInfo>  // Включение заголовочного файла для работы с информацией о файле.
#include <QIODevice>  // Включение заголовочного файла для работы с устройствами ввода-вывода.

ChatProtocol::ChatProtocol()  // Определение конструктора класса ChatProtocol.
{

}

QByteArray ChatProtocol::textMessage(QString message, QString receiver)  // Определение метода textMessage для отправки текстового сообщения.
{
    QByteArray ba;  // Создание массива байтов для хранения данных.
    QDataStream out(&ba, QIODevice::WriteOnly);  // Создание потока данных для записи в массив байтов.
    out.setVersion(QDataStream::Qt_6_0);  // Установка версии потока данных.
    out << Text << receiver << message;  // Запись типа сообщения, получателя и сообщения в поток данных.
    return ba;  // Возврат массива байтов.
}

QByteArray ChatProtocol::isTypingMessage()  // Определение метода isTypingMessage для отправки сообщения о том, что пользователь печатает.
{
    return getData(IsTyping, "");  // Возврат данных для сообщения о том, что пользователь печатает.
}

QByteArray ChatProtocol::setNameMessage(QString name)  // Определение метода setNameMessage для установки имени пользователя.
{
    return getData(SetName, name);  // Возврат данных для установки имени пользователя.
}

QByteArray ChatProtocol::setStatusMessage(Status status)  // Определение метода setStatusMessage для установки статуса пользователя.
{
    QByteArray ba;  // Создание массива байтов для хранения данных.
    QDataStream out(&ba, QIODevice::WriteOnly);  // Создание потока данных для записи в массив байтов.
    out.setVersion(QDataStream::Qt_6_0);  // Установка версии потока данных.
    out << SetStatus << status;  // Запись типа сообщения и статуса в поток данных.
    return ba;  // Возврат массива байтов.
}

QByteArray ChatProtocol::setInitSendingFileMessage(QString fileName)  // Определение метода setInitSendingFileMessage для отправки сообщения о начале отправки файла.
{
    QByteArray ba;  // Создание массива байтов для хранения данных.
    QDataStream out(&ba, QIODevice::WriteOnly);  // Создание потока данных для записи в массив байтов.
    out.setVersion(QDataStream::Qt_6_0);  // Установка версии потока данных.
    QFileInfo info(fileName);  // Получение информации о файле.
    out << InitSendingFile << info.fileName() << info.size();  // Запись типа сообщения, имени файла и его размера в поток данных.
    return ba;  // Возврат массива байтов.
}

QByteArray ChatProtocol::setAcceptFileMessage()  // Определение метода setAcceptFileMessage для отправки сообщения о принятии файла.
{
    return getData(AcceptSendingFile, "");  // Возврат данных для сообщения о принятии файла.
}

QByteArray ChatProtocol::setRejectFileMessage()  // Определение метода setRejectFileMessage для отправки сообщения о отказе в приеме файла.
{
    return getData(RejectSendingFile, "");  // Возврат данных для сообщения о отказе в приеме файла.
}

QByteArray ChatProtocol::setFileMessage(QString fileName)  // Определение метода setFileMessage для отправки файла.
{
    QByteArray ba;  // Создание массива байтов для хранения данных.
    QFile file(fileName);  // Создание объекта файла.
    if (file.open(QIODevice::ReadOnly)) {  // Попытка открытия файла в режиме только для чтения.
        QDataStream out(&ba, QIODevice::WriteOnly);  // Создание потока данных для записи в массив байтов.
        out.setVersion(QDataStream::Qt_6_0);  // Установка версии потока данных.
        QFileInfo info(fileName);  // Получение информации о файле.
        out << SendFile << info.fileName() << info.size() << file.readAll();  // Запись типа сообщения, имени файла, его размера и содержимого в поток данных.
        file.close();  // Закрытие файла.
    }
    return ba;  // Возврат массива байтов.
}

void ChatProtocol::loadData(QByteArray data)  // Определение метода loadData для загрузки данных из массива байтов.
{
    QDataStream in(&data, QIODevice::ReadOnly);  // Создание потока данных для чтения из массива байтов.
    in.setVersion(QDataStream::Qt_6_0);  // Установка версии потока данных.
    in >> _type;  // Чтение типа сообщения из потока данных.
    switch (_type) {  // Начало переключения по типу сообщения.
    case Text:  // В случае текстового сообщения:
        in >> _receiver >> _message;  // Чтение получателя и сообщения из потока данных.
        break;  // Выход из переключения.
    case SetName:  // В случае установки имени пользователя:
        in >> _name;  // Чтение имени пользователя из потока данных.
        break;  // Выход из переключения.
    case SetStatus:  // В случае установки статуса пользователя:
        in >> _status;  // Чтение статуса пользователя из потока данных.
        break;  // Выход из переключения.
    case InitSendingFile:  // В случае начала отправки файла:
        in >> _fileName >> _fileSize;  // Чтение имени файла и его размера из потока данных.
        break;  // Выход из переключения.
    case SendFile:  // В случае отправки файла:
        in >> _fileName >> _fileSize >> _fileData;  // Чтение имени файла, его размера и содержимого из потока данных.
        break;  // Выход из переключения.
    case ClientName:  // В случае имени клиента:
        in >> _prevName >> _clientName;  // Чтение предыдущего и текущего имени клиента из потока данных.
        break;  // Выход из переключения.
    case NewClient:  // В случае подключения нового клиента:
    case ClientDisconnected:  // В случае отключения клиента:
        in >> _clientName;  // Чтение имени клиента из потока данных.
        break;  // Выход из переключения.
    case ConnectionACK:  // В случае подтверждения подключения:
        in >> _myName >> _clientsName;  // Чтение имени текущего пользователя и списка клиентов из потока данных.
        break;  // Выход из переключения.
    default:  // В случае неизвестного типа сообщения:
        break;  // Выход из переключения.
    }
}

QByteArray ChatProtocol::getData(MessageType type, QString data)  // Определение метода getData для получения данных в формате массива байтов.
{
    QByteArray ba;  // Создание массива байтов для хранения данных.
    QDataStream out(&ba, QIODevice::WriteOnly);  // Создание потока данных для записи в массив байтов.
    out.setVersion(QDataStream::Qt_6_0);  // Установка версии потока данных.
    out << type << data;  // Запись типа сообщения и данных в поток данных.
    return ba;  // Возврат массива байтов.
}

// Методы получения значений членов класса:

const QString &ChatProtocol::myName() const  // Метод получения текущего имени пользователя.
{
    return _myName;  // Возврат текущего имени пользователя.
}

const QStringList &ChatProtocol::clientsName() const  // Метод получения списка имен клиентов.
{
    return _clientsName;  // Возврат списка имен клиентов.
}

const QString &ChatProtocol::prevName() const  // Метод получения предыдущего имени клиента.
{
    return _prevName;  // В
