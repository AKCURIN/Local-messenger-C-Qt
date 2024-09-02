#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H  // Препроцессорная директива для предотвращения множественного включения этого заголовочного файла.

#include <QMainWindow>  // Включение класса QMainWindow из библиотеки Qt для создания основного окна приложения.
#include <QFile>  // Включение класса QFile из библиотеки Qt для работы с файлами.
#include <mainwindow.h>  // Включение заголовочного файла для главного окна приложения.

namespace Ui {
class AuthorizationWindow;  // Объявление класса Ui::AuthorizationWindow, который будет сгенерирован из файла формы (UI файл).
}

class AuthorizationWindow : public QMainWindow  // Объявление класса AuthorizationWindow, наследующего QMainWindow.
{
    Q_OBJECT  // Макрос Q_OBJECT необходим для использования сигналов и слотов в Qt.
signals:
    void openWindowsSignals(QString);  // Объявление сигнала openWindowsSignals, который передает строку.

public:
    explicit AuthorizationWindow(QWidget *parent = nullptr);  // Конструктор класса AuthorizationWindow, принимающий указатель на родительский объект (по умолчанию nullptr).
    ~AuthorizationWindow();  // Деструктор класса AuthorizationWindow.

private slots:
    void connectServer();  // Объявление слота connectServer для подключения к серверу.
    void registration();  // Объявление слота registration для обработки регистрации.

private:
    MainWindow *clientWindow;  // Указатель на главное окно клиента.
    QFile *file;  // Указатель на объект QFile для работы с файлами.

    Ui::AuthorizationWindow *ui;  // Указатель на объект Ui::AuthorizationWindow для доступа к элементам пользовательского интерфейса.
};

#endif // AUTHORIZATIONWINDOW_H  // Завершение директивы препроцессора для предотвращения множественного включения.
