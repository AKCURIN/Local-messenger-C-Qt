#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H  // Препроцессорная директива для предотвращения множественного включения этого заголовочного файла.

#include <QByteArray>  // Включение класса QByteArray из библиотеки Qt для работы с массивами байтов.
#include <QString>  // Включение класса QString из библиотеки Qt для работы со строками.
#include <QStringList>  // Включение класса QStringList из библиотеки Qt для работы со списками строк.

class ChatProtocol
{
public:
    // Перечисление типов сообщений.
    enum MessageType {
        Text,  // Текстовое сообщение.
        IsTyping,  // Сообщение о том, что пользователь печатает.
        SetName,  // Установка имени пользователя.
        SetStatus,  // Установка статуса пользователя.
        InitSendingFile,  // Инициализация отправки файла.
        AcceptSendingFile,  // Принятие отправки файла.
        RejectSendingFile,  // Отклонение отправки файла.
        SendFile,  // Отправка файла.
        ClientName,  // Имя клиента.
        ConnectionACK,  // Подтверждение подключения.
        NewClient,  // Новый клиент.
        ClientDisconnected  // Клиент отключился.
    };

    // Перечисление статусов пользователей.
    enum Status {
        None,  // Нет статуса.
        Available,  // Доступен.
        Away,  // Отсутствует.
        Busy  // Занят.
    };

    ChatProtocol();  // Конструктор класса ChatProtocol.

    QByteArray textMessage(QString message, QString receiver);  // Метод для создания текстового сообщения.
    QByteArray isTypingMessage();  // Метод для создания сообщения о том, что пользователь печатает.
    QByteArray setNameMessage(QString name);  // Метод для создания сообщения об установке имени.
    QByteArray setStatusMessage(Status status);  // Метод для создания сообщения об установке статуса.
    QByteArray setInitSendingFileMessage(QString fileName);  // Метод для создания сообщения об инициализации отправки файла.
    QByteArray setAcceptFileMessage();  // Метод для создания сообщения о принятии файла.
    QByteArray setRejectFileMessage();  // Метод для создания сообщения об отклонении файла.
    QByteArray setFileMessage(QString fileName);  // Метод для создания сообщения об отправке файла.

    void loadData(QByteArray data);  // Метод для загрузки данных из массива байтов.

    const QString &message() const;  // Метод для получения сообщения.
    const QString &name() const;  // Метод для получения имени.
    Status status() const;  // Метод для получения статуса.
    MessageType type() const;  // Метод для получения типа сообщения.
    const QString &fileName() const;  // Метод для получения имени файла.
    qint64 fileSize() const;  // Метод для получения размера файла.
    const QByteArray &fileData() const;  // Метод для получения данных файла.
    QString receiver() const;  // Метод для получения получателя.
    const QString &clientName() const;  // Метод для получения имени клиента.
    const QString &prevName() const;  // Метод для получения предыдущего имени.
    const QStringList &clientsName() const;  // Метод для получения списка имен клиентов.
    const QString &myName() const;  // Метод для получения моего имени.

private:
    QByteArray getData(MessageType type, QString data);  // Приватный метод для получения данных сообщения определенного типа.

    MessageType _type;  // Переменная для хранения типа сообщения.
    QString _message;  // Переменная для хранения сообщения.
    QString _name;  // Переменная для хранения имени.
    Status _status;  // Переменная для хранения статуса.
    QString _fileName;  // Переменная для хранения имени файла.
    qint64 _fileSize;  // Переменная для хранения размера файла.
    QByteArray _fileData;  // Переменная для хранения данных файла.
    QString _receiver;  // Переменная для хранения получателя.
    QString _clientName;  // Переменная для хранения имени клиента.
    QString _prevName;  // Переменная для хранения предыдущего имени.
    QStringList _clientsName;  // Переменная для хранения списка имен клиентов.
    QString _myName;  // Переменная для хранения моего имени.
};

#endif // CHATPROTOCOL_H  // Завершение директивы препроцессора для предотвращения множественного включения.
