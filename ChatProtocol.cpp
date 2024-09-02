#include "ChatProtocol.h"

// Подключаем заголовочный файл ChatProtocol

#include <QFileInfo>
#include <QIODevice>

// Подключаем необходимые классы для работы с файлами и устройствами ввода-вывода

ChatProtocol::ChatProtocol()
{
    // Конструктор по умолчанию, не выполняет никаких действий
}

QByteArray ChatProtocol::textMessage(QString message, QString receiver)
{
    // Метод для создания текстового сообщения
    QByteArray ba;
    // Создаем QByteArray для хранения данных
    QDataStream out(&ba, QIODevice::WriteOnly);
    // Создаем QDataStream для записи в QByteArray
    out.setVersion(QDataStream::Qt_6_0);
    // Устанавливаем версию формата данных
    out << Text << receiver << message;
    // Записываем тип сообщения, получателя и само сообщение в поток
    return ba;
    // Возвращаем сформированный QByteArray
}

QByteArray ChatProtocol::isTypingMessage()
{
    // Метод для создания сообщения о наборе текста
    return getData(IsTyping, "");
    // Используем вспомогательный метод getData для формирования сообщения
}

QByteArray ChatProtocol::setNameMessage(QString name)
{
    // Метод для создания сообщения об установке имени
    return getData(SetName, name);
    // Используем вспомогательный метод getData для формирования сообщения
}

QByteArray ChatProtocol::setStatusMessage(Status status)
{
    // Метод для создания сообщения об установке статуса
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    // Создаем QDataStream для записи в QByteArray
    out.setVersion(QDataStream::Qt_6_0);
    // Устанавливаем версию формата данных
    out << SetStatus << status;
    // Записываем тип сообщения и статус в поток
    return ba;
    // Возвращаем сформированный QByteArray
}

QByteArray ChatProtocol::setInitSendingFileMessage(QString fileName)
{
    // Метод для создания сообщения об инициализации отправки файла
    QByteArray ba;
    QFileInfo info(fileName);
    // Получаем информацию о файле
    QDataStream out(&ba, QIODevice::WriteOnly);
    // Создаем QDataStream для записи в QByteArray
    out.setVersion(QDataStream::Qt_6_0);
    // Устанавливаем версию формата данных
    out << InitSendingFile << info.fileName() << info.size();
    // Записываем тип сообщения, имя файла и его размер в поток
    return ba;
    // Возвращаем сформированный QByteArray
}

QByteArray ChatProtocol::setAcceptFileMessage()
{
    // Метод для создания сообщения о принятии файла
    return getData(AcceptSendingFile, "");
    // Используем вспомогательный метод getData для формирования сообщения
}

QByteArray ChatProtocol::setRejectFileMessage()
{
    // Метод для создания сообщения об отклонении файла
    return getData(RejectSendingFile, "");
    // Используем вспомогательный метод getData для формирования сообщения
}

QByteArray ChatProtocol::setFileMessage(QString fileName)
{
    // Метод для создания сообщения с файлом
    QByteArray ba;
    QFile file(fileName);
    // Открываем файл для чтения
    if (file.open(QIODevice::ReadOnly)) {
        QFileInfo info(fileName);
        // Получаем информацию о файле
        QDataStream out(&ba, QIODevice::WriteOnly);
        // Создаем QDataStream для записи в QByteArray
        out.setVersion(QDataStream::Qt_6_0);
        // Устанавливаем версию формата данных
        out << SendFile << info.fileName() << info.size() << file.readAll();
        // Записываем тип сообщения, имя файла, размер и содержимое файла в поток
        file.close();
        // Закрываем файл
    }
    return ba;
    // Возвращаем сформированный QByteArray
}

QByteArray ChatProtocol::setClientNameMessage(QString prevName, QString name)
{
    // Метод для создания сообщения об изменении имени клиента
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    // Создаем QDataStream для записи в QByteArray
    out.setVersion(QDataStream::Qt_6_0);
    // Устанавливаем версию формата данных
    out << ClientName << prevName << name;
    // Записываем тип сообщения, предыдущее и новое имя в поток
    return ba;
    // Возвращаем сформированный QByteArray
}

QByteArray ChatProtocol::setConnectionACKMessage(QString clientName, QStringList otherClients)
{
    // Метод для создания сообщения подтверждения подключения
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    // Создаем QDataStream для записи в QByteArray
    out.setVersion(QDataStream::Qt_6_0);
    // Устанавливаем версию формата данных
    out << ConnectionACK << clientName << otherClients;
    // Записываем тип сообщения, имя клиента и список других клиентов в поток
    return ba;
    // Возвращаем сформированный QByteArray
}

QByteArray ChatProtocol::setNewClientMessage(QString clientName)
{
    // Метод для создания сообщения о новом клиенте
    return getData(NewClient, clientName);
    // Используем вспомогательный метод getData для формирования сообщения
}

QByteArray ChatProtocol::setClinetDisconnectedMessage(QString clientName)
{
    // Метод для создания сообщения об отключении клиента
    return getData(ClientDisconnected, clientName);
    // Используем вспомогательный метод getData для формирования сообщения
}

void ChatProtocol::loadData(QByteArray data)
{
    // Метод для загрузки данных из QByteArray
    QDataStream in(&data, QIODevice::ReadOnly);
    // Создаем QDataStream для чтения из QByteArray
    in.setVersion(QDataStream::Qt_6_0);
    // Устанавливаем версию формата данных
    in >> _type;
    // Читаем тип сообщения из потока
    switch (_type) {
    case Text:
        in >> _receiver >> _message;
        // Читаем получателя и сообщение из потока
        break;
    case SetName:
        in >> _name;
        // Читаем имя из потока
        break;
    case SetStatus:
        in >> _status;
        // Читаем статус из потока
        break;
    case InitSendingFile:
        in >> _fileName >> _fileSize;
        // Читаем имя файла и его размер из потока
        break;
    case SendFile:
        in >> _fileName >> _fileSize >> _fileData;
        // Читаем имя файла, его размер и содержимое из потока
        break;
    default:
        break;
    }
}

QByteArray ChatProtocol::getData(MessageType type, QString data)
{
    // Вспомогательный метод для создания сообщения
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    // Создаем QDataStream для записи в QByteArray
    out.setVersion(QDataStream::Qt_6_0);
    // Устанавливаем версию формата данных
    out << type << data;
    // Записываем тип сообщения и данные в поток
    return ba;
    // Возвращаем сформированный QByteArray
}

const QString &ChatProtocol::receiver() const
{
    // Метод для получения получателя сообщения
    return _receiver;
}

const QByteArray &ChatProtocol::fileData() const
{
    // Метод для получения данных файла
    return _fileData;
}

qint64 ChatProtocol::fileSize() const
{
    // Метод для получения размера файла
    return _fileSize;
}

const QString &ChatProtocol::fileName() const
{
    // Метод для получения имени файла
    return _fileName;
}

ChatProtocol::MessageType ChatProtocol::type() const
{
    // Метод для получения типа сообщения
    return _type;
}

ChatProtocol::Status ChatProtocol::status() const
{
    // Метод для получения статуса
    return _status;
}

const QString &ChatProtocol::name() const
{
    // Метод для получения имени
    return _name;
}

const QString &ChatProtocol::message() const
{
    // Метод для получения текста сообщения
    return _message;
}
