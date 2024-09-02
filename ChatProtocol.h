#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

// Защита от повторного включения заголовочного файла (include guard)
// Если CHATPROTOCOL_H не определен, определяем его и включаем содержимое файла
// Это предотвращает множественное включение одного и того же заголовочного файла

#include <QByteArray>
#include <QString>

// Подключаем необходимые классы из библиотеки Qt:
// QByteArray используется для хранения и передачи последовательностей байтов
// QString используется для хранения и работы со строками

class ChatProtocol
{
public:
    // Определение перечисления MessageType для различных типов сообщений, которые могут быть переданы в чате
    enum MessageType{
        Text,                // Текстовое сообщение
        IsTyping,            // Сообщение о том, что пользователь набирает текст
        SetName,             // Сообщение об установке имени пользователя
        SetStatus,           // Сообщение об установке статуса пользователя
        InitSendingFile,     // Инициализация отправки файла
        AcceptSendingFile,   // Подтверждение принятия файла
        RejectSendingFile,   // Отклонение принятия файла
        SendFile,            // Отправка файла
        ClientName,          // Имя клиента
        ConnectionACK,       // Подтверждение подключения
        NewClient,           // Уведомление о новом клиенте
        ClientDisconnected   // Уведомление о отключении клиента
    };

    // Определение перечисления Status для различных статусов пользователя
    enum Status{
        None,        // Нет статуса
        Available,   // Доступен
        Away,        // Отошел
        Busy         // Занят
    };

    // Конструктор класса ChatProtocol
    ChatProtocol();

    // Методы для создания различных типов сообщений, возвращающие QByteArray
    // Они принимают нужные параметры и возвращают готовое сообщение в виде массива байтов
    QByteArray textMessage(QString message, QString receiver);        // Создает текстовое сообщение
    QByteArray isTypingMessage();                                      // Создает сообщение о том, что пользователь набирает текст
    QByteArray setNameMessage(QString name);                           // Создает сообщение об установке имени пользователя
    QByteArray setStatusMessage(Status status);                        // Создает сообщение об установке статуса пользователя
    QByteArray setInitSendingFileMessage(QString fileName);            // Создает сообщение об инициализации отправки файла
    QByteArray setAcceptFileMessage();                                 // Создает сообщение о принятии файла
    QByteArray setRejectFileMessage();                                 // Создает сообщение об отклонении файла
    QByteArray setFileMessage(QString fileName);                       // Создает сообщение об отправке файла

    QByteArray setClientNameMessage(QString prevName, QString name);   // Создает сообщение об изменении имени клиента
    QByteArray setConnectionACKMessage(QString clientName, QStringList otherClients);  // Создает сообщение подтверждения подключения
    QByteArray setNewClientMessage(QString clientName);                // Создает сообщение о новом клиенте
    QByteArray setClinetDisconnectedMessage(QString clientName);       // Создает сообщение о отключении клиента

    // Метод для загрузки данных сообщения из QByteArray
    void loadData(QByteArray data);

    // Геттеры для получения различных атрибутов сообщения
    const QString &message() const;          // Возвращает текст сообщения
    const QString &name() const;             // Возвращает имя пользователя
    Status status() const;                   // Возвращает статус пользователя
    MessageType type() const;                // Возвращает тип сообщения
    const QString &fileName() const;         // Возвращает имя файла
    qint64 fileSize() const;                 // Возвращает размер файла
    const QByteArray &fileData() const;      // Возвращает данные файла
    const QString &receiver() const;         // Возвращает имя получателя сообщения

private:
    // Вспомогательный метод для создания данных сообщения на основе типа и строки данных
    QByteArray getData(MessageType type, QString data);

    // Поля для хранения атрибутов сообщения
    MessageType _type;      // Тип сообщения
    QString _message;       // Текст сообщения
    QString _name;          // Имя пользователя
    Status _status;         // Статус пользователя
    QString _fileName;      // Имя файла
    qint64 _fileSize;       // Размер файла
    QByteArray _fileData;   // Данные файла
    QString _receiver;      // Имя получателя сообщения
};

#endif // CHATPROTOCOL_H

// Завершение защиты от повторного включения заголовочного файла
// Это закрывает условие #ifndef, начатое в начале файла
