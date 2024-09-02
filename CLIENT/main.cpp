//#include "mainwindow.h"  // Эта строка закомментирована, потому что main.cpp не использует MainWindow.
#include "authorizationwindow.h"  // Включение заголовочного файла AuthorizationWindow для создания объекта окна авторизации.
#include <QApplication>  // Включение заголовочного файла QApplication для управления приложением.

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // Создание объекта приложения QApplication, который управляет всеми аспектами приложения.
    AuthorizationWindow w;  // Создание объекта окна авторизации.
    w.show();  // Отображение окна авторизации на экране.
    return a.exec();  // Запуск главного цикла обработки событий приложения, возвращение управления операционной системе при выходе из приложения.
}
