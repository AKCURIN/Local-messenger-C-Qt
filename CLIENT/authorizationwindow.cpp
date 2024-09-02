#include "authorizationwindow.h"  // Включение заголовочного файла класса AuthorizationWindow.
#include "ui_authorizationwindow.h"  // Включение заголовочного файла для доступа к элементам UI.
#include <QDebug>  // Включение заголовочного файла для отладки с использованием qDebug().
#include <QMessageBox>  // Включение заголовочного файла для работы с QMessageBox.

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QMainWindow(parent),  // Вызов конструктора базового класса QMainWindow.
    ui(new Ui::AuthorizationWindow)  // Инициализация указателя ui новым объектом Ui::AuthorizationWindow.
{
    ui->setupUi(this);  // Установка UI из файла формы.

    file = new QFile("myfile.txt");  // Инициализация указателя file объектом QFile, представляющим файл "myfile.txt".

    connect(ui->connectBut, SIGNAL(clicked()), SLOT(connectServer()));  // Подключение сигнала clicked() кнопки connectBut к слоту connectServer().
    connect(ui->registrBut, SIGNAL(clicked()), SLOT(registration()));  // Подключение сигнала clicked() кнопки registrBut к слоту registration().

    ui->passEdit->setEchoMode(QLineEdit::Password);  // Установка режима отображения пароля в поле passEdit.
}

AuthorizationWindow::~AuthorizationWindow()  // Деструктор класса AuthorizationWindow.
{
    delete ui;  // Освобождение памяти, занятой объектом ui.
}

void AuthorizationWindow::connectServer()  // Определение метода connectServer().
{
    if(ui->loginEdit->text().isEmpty())  // Проверка, если поле loginEdit пусто.
    {
        QMessageBox::warning(this, "Attention!", "The username is not specified...");  // Отображение предупреждающего сообщения.
    }
    if(ui->passEdit->text().isEmpty())  // Проверка, если поле passEdit пусто.
    {
        QMessageBox::warning(this, "Attention!", "The password is not specified...");  // Отображение предупреждающего сообщения.
    }
    else if(file->open(QIODevice::ReadOnly))  // Попытка открытия файла в режиме только для чтения.
    {
        clientWindow = new MainWindow;  // Создание нового объекта MainWindow.
        connect(this, SIGNAL(openWindowsSignals(QString)), clientWindow, SLOT(openWindow(QString)));  // Подключение сигнала openWindowsSignals(QString) к слоту openWindow(QString) объекта clientWindow.

        QTextStream in(file);  // Создание текстового потока для чтения из файла.
        while (!in.atEnd())  // Цикл до конца файла.
        {
            QString line = in.readLine();  // Чтение строки из файла.

            // Проверка, если введенный логин и пароль соответствуют данным в файле.
            if(ui->loginEdit->text() == line.left(line.indexOf('/')) &&
                line.left(line.indexOf('/')) + "/" + ui->passEdit->text() ==
                    line.right(line.indexOf('/') - line.count() + 1))
            {
                openWindowsSignals(ui->loginEdit->text());  // Отправка сигнала с логином.
                this->hide();  // Скрытие окна авторизации.
                file->close();  // Закрытие файла.
                break;  // Прерывание цикла.
            }
        }

        if(file->isOpen())  // Проверка, если файл все еще открыт.
        {
            QMessageBox::warning(this, "Attention!", "The wrong username or password..");  // Отображение предупреждающего сообщения.
            file->close();  // Закрытие файла.
        }
    }
}

void AuthorizationWindow::registration()  // Определение метода registration().
{
    if(ui->loginEdit->text().isEmpty())  // Проверка, если поле loginEdit пусто.
    {
        QMessageBox::warning(this, "Attention!", "The username is not specified...");  // Отображение предупреждающего сообщения.
    }
    if(ui->passEdit->text().isEmpty())  // Проверка, если поле passEdit пусто.
    {
        QMessageBox::warning(this, "Attention!", "The password is not specified...");  // Отображение предупреждающего сообщения.
    }
    else if(file->open(QIODevice::Append | QIODevice::Text))  // Попытка открытия файла в режиме добавления и текстового режима.
    {
        QTextStream writeStream(file);  // Создание текстового потока для записи в файл.
        writeStream << ui->loginEdit->text();  // Запись логина в файл.
        writeStream << "/" + ui->passEdit->text() + "\n";  // Запись пароля в файл.
        file->close();  // Закрытие файла.
    }
}
