#include "ChatItemWidget.h"  // Включение заголовочного файла класса ChatItemWidget.
#include "ui_ChatItemWidget.h"  // Включение заголовочного файла для доступа к элементам UI.

#include <QTime>  // Включение заголовочного файла для работы с классом QTime.

ChatItemWidget::ChatItemWidget(QWidget *parent) :
    QWidget(parent),  // Вызов конструктора базового класса QWidget с передачей указателя на родительский объект.
    ui(new Ui::ChatItemWidget)  // Инициализация указателя ui новым объектом Ui::ChatItemWidget.
{
    ui->setupUi(this);  // Установка UI из файла формы.
}

ChatItemWidget::~ChatItemWidget()  // Деструктор класса ChatItemWidget.
{
    delete ui;  // Освобождение памяти, занятой объектом ui.
}

void ChatItemWidget::setMessage(QString message, bool isMyMessage)  // Определение метода setMessage().
{
    if(isMyMessage)  // Проверка, если сообщение отправлено текущим пользователем.
        ui->lblMessage->setAlignment(Qt::AlignRight);  // Выравнивание текста сообщения по правому краю.
    ui->lblMessage->setText(message);  // Установка текста сообщения в метку lblMessage.
    ui->lblTime->setText(QTime::currentTime().toString("HH:mm"));  // Установка текущего времени в метку lblTime в формате "ЧЧ:мм".
}
