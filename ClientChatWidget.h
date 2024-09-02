#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

// Защита от повторного включения заголовочного файла (include guard)

#include "ClientManager.h"

// Подключение заголовочного файла ClientManager

#include <QDir>
#include <QTcpSocket>
#include <QWidget>

// Подключение необходимых классов из библиотеки Qt:
// QDir используется для работы с файловыми системами и каталогами
// QTcpSocket используется для сетевого взаимодействия через TCP
// QWidget является базовым классом для всех объектов пользовательского интерфейса

namespace Ui {
class ClientChatWidget;
}

// Объявляем пространство имен Ui, содержащее класс ClientChatWidget, который будет сгенерирован из UI-файла

class ClientChatWidget : public QWidget
{
    Q_OBJECT

    // Макрос Q_OBJECT необходим для всех классов, которые определяют сигналы и слоты
public:
    // Конструктор класса ClientChatWidget
    explicit ClientChatWidget(QTcpSocket *client, QWidget *parent = nullptr);

    // Метод для отключения клиента
    void disconnect();

    // Деструктор класса
    ~ClientChatWidget();

private slots:
    // Слоты - это функции, которые могут быть подключены к сигналам и вызываются в ответ на сигналы

    // Слот вызывается при отключении клиента
    void clientDisconnected();

    // Слот вызывается при нажатии кнопки отправки сообщения
    void on_btnSend_clicked();

    // Слот вызывается при получении текстового сообщения
    void textMessageReceived(QString message, QString receiver);

    // Слот вызывается, когда клиент начинает набирать текст
    void onTyping();

    // Слот вызывается при инициализации получения файла
    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);

    // Слот вызывается после сохранения файла
    void onFileSaved(QString path);

    // Слот вызывается при активации ссылки на открытие папки
    void on_lblOpenFolder_linkActivated(const QString &link);

    // Слот вызывается при изменении имени клиента
    void onClientNameChanged(QString prevName, QString name);

signals:
    // Сигналы - это функции, которые могут быть испущены и подключены к слотам

    // Сигнал об изменении имени клиента
    void clientNameChanged(QString prevName, QString name);

    // Сигнал о том, что клиент набирает текст
    void isTyping(QString message);

    // Сигнал об изменении статуса клиента
    void statusChanged(ChatProtocol::Status status);

    // Сигнал для передачи текстового сообщения другим клиентам
    void textForOtherClients(QString message, QString receiver, QString sender);

private:
    Ui::ClientChatWidget *ui;  // Указатель на объект интерфейса, созданный из UI-файла
    ClientManager *_client;    // Указатель на объект менеджера клиента
    QDir dir;                  // Объект для работы с каталогами
};

#endif // CLIENTCHATWIDGET_H

// Завершение защиты от повторного включения заголовочного файла
// Это закрывает условие #ifndef, начатое в начале файла
