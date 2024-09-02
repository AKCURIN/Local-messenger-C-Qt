#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Защита от повторного включения заголовочного файла (include guard)

#include <QMainWindow>
#include "ServerManager.h"
#include "ClientChatWidget.h"

// Подключение необходимых классов и заголовочных файлов
// QMainWindow - класс для создания основного окна приложения
// ServerManager - заголовочный файл менеджера сервера
// ClientChatWidget - заголовочный файл виджета чата клиента

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Объявление пространства имен Ui и класса MainWindow, который будет сгенерирован из UI-файла

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Макрос Q_OBJECT необходим для всех классов, которые определяют сигналы и слоты
public:
    // Конструктор класса MainWindow, принимает указатель на родительский виджет
    MainWindow(QWidget *parent = nullptr);

    // Деструктор класса
    ~MainWindow();

private slots:
    // Слоты - это функции, которые могут быть подключены к сигналам и вызываются в ответ на сигналы

    // Слот, вызываемый при подключении нового клиента
    void newClientConnected(QTcpSocket *client);

    // Слот, вызываемый при отключении клиента
    void clientDisconnected(QTcpSocket *client);

    // Слот, вызываемый при изменении имени клиента
    void setClientName(QString prevName, QString name);

    // Слот, вызываемый при изменении статуса клиента
    void setClientStatus(ChatProtocol::Status status);

    // Слот, вызываемый при закрытии вкладки чата клиента
    void on_tbClientsChat_tabCloseRequested(int index);

private: // fields (поля)
    Ui::MainWindow *ui;   // Указатель на объект интерфейса, созданный из UI-файла
    ServerManager *_server;   // Указатель на объект менеджера сервера

private: // methods (методы)
    // Вспомогательный метод для настройки сервера
    void seupServer();
};

#endif // MAINWINDOW_H

// Завершение защиты от повторного включения заголовочного файла
// Это закрывает условие #ifndef, начатое в начале файла
