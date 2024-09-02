#ifndef MAINWINDOW_H
#define MAINWINDOW_H  // Препроцессорная директива для предотвращения множественного включения этого заголовочного файла.

#include <QMainWindow>  // Включение класса QMainWindow из библиотеки Qt для создания главного окна приложения.
#include "ClientManager.h"  // Включение заголовочного файла ClientManager для работы с менеджером клиента.

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }  // Объявление класса Ui::MainWindow, который будет сгенерирован из файла формы (UI файл).
QT_END_NAMESPACE

class MainWindow : public QMainWindow  // Объявление класса MainWindow, наследующего QMainWindow.
{
    Q_OBJECT  // Макрос Q_OBJECT необходим для использования сигналов и слотов в Qt.

public slots:
    void openWindow(QString);  // Слот для открытия окна.

public:
    MainWindow(QWidget *parent = nullptr);  // Конструктор класса MainWindow, принимающий указатель на родительский объект (по умолчанию nullptr).
    ~MainWindow();  // Деструктор класса MainWindow.

private slots:
    void on_actionConnect_triggered();  // Слот для обработки события подключения.
    void on_btnSend_clicked();  // Слот для обработки события нажатия кнопки отправки сообщения.
    void dataReceived(QString message);  // Слот для обработки получения данных.
    void on_lnClientName_editingFinished();  // Слот для обработки завершения редактирования имени клиента.
    void on_cmbStatus_currentIndexChanged(int index);  // Слот для обработки изменения индекса статуса.
    void onTyping();  // Слот для обработки события печати сообщения.
    void on_btnSendFile_clicked();  // Слот для обработки нажатия кнопки отправки файла.
    void onRejectReceivingFile();  // Слот для обработки события отклонения получения файла.
    void onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize);  // Слот для обработки инициализации получения файла.
    void onConnectionACK(QString myName, QStringList clientsName);  // Слот для обработки подтверждения подключения.
    void onNewClientConnectedToServer(QString clienName);  // Слот для обработки события подключения нового клиента.
    void onClientNameChanged(QString prevName, QString clientName);  // Слот для обработки изменения имени клиента.
    void onClientDisconnected(QString clientName);  // Слот для обработки отключения клиента.

private:
    Ui::MainWindow *ui;  // Указатель на объект Ui::MainWindow для доступа к элементам пользовательского интерфейса.
    ClientManager *_client;  // Указатель на объект ClientManager для работы с менеджером клиента.
    void setupClient();  // Приватный метод для настройки клиента.
};

#endif // MAINWINDOW_H  // Завершение директивы препроцессора для предотвращения множественного включения.
