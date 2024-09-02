#ifndef CHATITEMWIDGET_H
#define CHATITEMWIDGET_H  // Препроцессорная директива для предотвращения множественного включения этого заголовочного файла.

#include <QWidget>  // Включение класса QWidget из библиотеки Qt для создания виджета.

namespace Ui {
class ChatItemWidget;  // Объявление класса Ui::ChatItemWidget, который будет сгенерирован из файла формы (UI файл).
}

class ChatItemWidget : public QWidget  // Объявление класса ChatItemWidget, наследующего QWidget.
{
    Q_OBJECT  // Макрос Q_OBJECT необходим для использования сигналов и слотов в Qt.

public:
    explicit ChatItemWidget(QWidget *parent = nullptr);  // Конструктор класса ChatItemWidget, принимающий указатель на родительский объект (по умолчанию nullptr).
    ~ChatItemWidget();  // Деструктор класса ChatItemWidget.
    void setMessage(QString message, bool isMyMessage = false);  // Метод для установки сообщения в виджет. Принимает строку сообщения и флаг, указывающий, является ли это сообщение от текущего пользователя.

private:
    Ui::ChatItemWidget *ui;  // Указатель на объект Ui::ChatItemWidget для доступа к элементам пользовательского интерфейса.
};

#endif // CHATITEMWIDGET_H  // Завершение директивы препроцессора для предотвращения множественного включения.
