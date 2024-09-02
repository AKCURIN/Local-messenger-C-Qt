#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)  // Инициализация базового класса QMainWindow с указанием родительского объекта
    , ui(new Ui::MainWindow)  // Инициализация указателя на объект интерфейса пользователя
{
    ui->setupUi(this);  // Настройка интерфейса пользователя из сгенерированного файла ui
    seupServer();  // Вызов функции настройки сервера
}

MainWindow::~MainWindow()
{
    delete ui;  // Освобождение памяти, занятой объектом интерфейса пользователя
}

void MainWindow::newClientConnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();  // Получение идентификатора нового клиента
    ui->lstClients->addItem(QString("New Client added: %1").arg(id));  // Добавление информации о новом клиенте в список
    auto chatWidget = new ClientChatWidget(client, ui->tbClientsChat);  // Создание нового виджета чата для клиента
    ui->tbClientsChat->addTab(chatWidget, QString("Client (%1)").arg(id));  // Добавление вкладки чата для клиента

    connect(chatWidget, &ClientChatWidget::clientNameChanged, this, &MainWindow::setClientName);  // Соединение сигнала изменения имени клиента с соответствующим слотом
    connect(chatWidget, &ClientChatWidget::statusChanged, this, &MainWindow::setClientStatus);  // Соединение сигнала изменения статуса клиента с соответствующим слотом
    connect(chatWidget, &ClientChatWidget::isTyping, [this](QString name){  // Соединение сигнала печати с анонимной функцией для обновления строки состояния
        this->statusBar()->showMessage(name, 750);
    });

    connect(chatWidget, &ClientChatWidget::textForOtherClients, _server, &ServerManager::onTextForOtherClients);  // Соединение сигнала текста для других клиентов с соответствующим слотом на сервере
}

void MainWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();  // Получение идентификатора отключившегося клиента
    ui->lstClients->addItem(QString("Client disconnected: %1").arg(id));  // Добавление информации об отключении клиента в список
}

void MainWindow::setClientName(QString prevName, QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());  // Получение указателя на виджет, отправивший сигнал
    auto index = ui->tbClientsChat->indexOf(widget);  // Получение индекса вкладки, соответствующей виджету
    ui->tbClientsChat->setTabText(index, name);  // Установка нового имени вкладки

    _server->notifyOtherClients(prevName, name);  // Уведомление других клиентов о смене имени
}

void MainWindow::setClientStatus(ChatProtocol::Status status)
{
    auto widget = qobject_cast<QWidget *>(sender());  // Получение указателя на виджет, отправивший сигнал
    auto index = ui->tbClientsChat->indexOf(widget);  // Получение индекса вкладки, соответствующей виджету
    QString iconName = ":/icons/";  // Путь к папке с иконками
    switch (status) {
    case ChatProtocol::Available:
        iconName.append("available.png");  // Установка иконки "Доступен"
        break;
    case ChatProtocol::Away:
        iconName.append("away.png");  // Установка иконки "Отсутствует"
        break;
    case ChatProtocol::Busy:
        iconName.append("busy.png");  // Установка иконки "Занят"
        break;
    default:
        iconName = "";  // Если статус не определен, иконка не устанавливается
        break;
    }

    auto icon = QIcon(iconName);  // Создание объекта иконки
    ui->tbClientsChat->setTabIcon(index, icon);  // Установка иконки на вкладку
}

void MainWindow::seupServer()
{
    _server = new ServerManager();  // Создание объекта сервера
    connect(_server, &ServerManager::newClientConnected, this, &MainWindow::newClientConnected);  // Соединение сигнала нового подключения с соответствующим слотом
    connect(_server, &ServerManager::clientDisconnected, this, &MainWindow::clientDisconnected);  // Соединение сигнала отключения клиента с соответствующим слотом
}

void MainWindow::on_tbClientsChat_tabCloseRequested(int index)
{
    auto chatWidget = qobject_cast<ClientChatWidget *>(ui->tbClientsChat->widget(index));  // Получение указателя на виджет чата по индексу вкладки
    chatWidget->disconnect();  // Отключение виджета чата
    ui->tbClientsChat->removeTab(index);  // Удаление вкладки чата
}
