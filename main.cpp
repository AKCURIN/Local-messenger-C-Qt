#include "mainwindow.h"  // Подключение заголовочного файла MainWindow

#include <QApplication>  // Подключение заголовочного файла QApplication

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // Создание объекта QApplication, который управляет жизненным циклом приложения
    MainWindow w;  // Создание главного окна приложения
    w.show();  // Отображение главного окна на экране
    return a.exec();  // Запуск главного цикла обработки событий приложения
}
