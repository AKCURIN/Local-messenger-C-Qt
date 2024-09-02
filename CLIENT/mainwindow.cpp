#include "ChatItemWidget.h"  // Включение заголовочного файла для ChatItemWidget, виджета сообщений чата.
#include "mainwindow.h"  // Включение заголовочного файла для MainWindow, главного окна приложения.
#include "ui_mainwindow.h"  // Включение сгенерированного заголовочного файла для интерфейса MainWindow.
#include <QFileDialog>>  // Включение заголовочного файла для использования диалогового окна выбора файла.
#include <QMessageBox>  // Включение заголовочного файла для использования сообщений MessageBox.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)  // Инициализация объекта интерфейса Ui::MainWindow.
{
    ui->setupUi(this);  // Установка интерфейса для текущего окна MainWindow.
    setupClient();  // Вызов функции для настройки клиента (ClientManager).

}

MainWindow::~MainWindow()
{
    delete ui;  // Удаление объекта интерфейса Ui::MainWindow.
}


void MainWindow::setupClient()
{
    _client = new ClientManager();  // Создание объекта ClientManager для управления клиентским подключением.
    connect(_client , &ClientManager::connected, [this](){
        ui->centralwidget->setEnabled(true);  // Подключение сигнала connected ClientManager к активации центрального виджета.
    });
    connect(_client, &ClientManager::disconnected, [this](){
        ui->centralwidget->setEnabled(false);  // Подключение сигнала disconnected ClientManager к деактивации центрального виджета.
    });
    connect(_client, &ClientManager::textMessageReceived, this, &MainWindow::dataReceived);  // Подключение сигнала textMessageReceived ClientManager к слоту dataReceived MainWindow.
    connect(_client, &ClientManager::isTyping, this, &MainWindow::onTyping);  // Подключение сигнала isTyping ClientManager к слоту onTyping MainWindow.
    connect(_client, &ClientManager::initReceivingFile, this, &MainWindow::onInitReceivingFile);  // Подключение сигнала initReceivingFile ClientManager к слоту onInitReceivingFile MainWindow.
    connect(_client, &ClientManager::rejectReceivingFile, this, &MainWindow::onRejectReceivingFile);  // Подключение сигнала rejectReceivingFile ClientManager к слоту onRejectReceivingFile MainWindow.

    connect(ui->lnMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);  // Подключение изменения текста в QLineEdit к отправке сигнала isTyping ClientManager.
    connect(_client, &ClientManager::connectionACK, this, &MainWindow::onConnectionACK);  // Подключение сигнала connectionACK ClientManager к слоту onConnectionACK MainWindow.
    connect(_client, &ClientManager::newClientConnectedToServer, this, &MainWindow::onNewClientConnectedToServer);  // Подключение сигнала newClientConnectedToServer ClientManager к слоту onNewClientConnectedToServer MainWindow.
    connect(_client, &ClientManager::clientDisconnected, this, &MainWindow::onClientDisconnected);  // Подключение сигнала clientDisconnected ClientManager к слоту onClientDisconnected MainWindow.
    connect(_client, &ClientManager::clientNameChanged, this, &MainWindow::onClientNameChanged);  // Подключение сигнала clientNameChanged ClientManager к слоту onClientNameChanged MainWindow.
}

void MainWindow::on_actionConnect_triggered()
{
    _client->connectToServer();  // Вызов метода connectToServer() объекта ClientManager при нажатии на действие "Connect" в меню.
}


void MainWindow::on_btnSend_clicked()
{
    auto message = ui->lnMessage->text().trimmed();  // Получение текста из QLineEdit и удаление лишних пробелов.
    _client->sendMessage(message, ui->cmbDestination->currentText());  // Отправка сообщения через ClientManager.
    //    ui->lstMessages->addItem(message);  // Добавление сообщения в список сообщений (например, QListWidget).
    ui->lnMessage->setText("");  // Очистка поля ввода сообщения.
    ui->lnMessage->setFocus();  // Установка фокуса на поле ввода сообщения.

    auto chatWidget = new ChatItemWidget();  // Создание нового виджета сообщения чата.
    chatWidget->setMessage(message, true);  // Установка текста сообщения и выравнивания для собственных сообщений.
    auto listWidgetItem = new QListWidgetItem();  // Создание нового элемента списка для сообщений.
    listWidgetItem->setSizeHint(QSize(0, 65));  // Установка размера элемента списка.
    ui->lstMessages->addItem(listWidgetItem);  // Добавление элемента списка в QListWidget.
    ui->lstMessages->setItemWidget(listWidgetItem, chatWidget);  // Установка виджета сообщения для элемента списка.
}

void MainWindow::dataReceived(QString message)
{
    //    ui->lstMessages->addItem(data);  // Добавление данных в список сообщений (например, QListWidget).
    auto chatWidget = new ChatItemWidget();  // Создание нового виджета сообщения чата.
    chatWidget->setMessage(message);  // Установка текста сообщения.
    auto listWidgetItem = new QListWidgetItem();  // Создание нового элемента списка для сообщений.
    listWidgetItem->setSizeHint(QSize(0, 65));  // Установка размера элемента списка.
    ui->lstMessages->addItem(listWidgetItem);  // Добавление элемента списка в QListWidget.
    listWidgetItem->setBackground(QColor(167, 255, 237));  // Установка фона для элемента списка.
    ui->lstMessages->setItemWidget(listWidgetItem, chatWidget);  // Установка виджета сообщения для элемента списка.
}

void MainWindow::on_lnClientName_editingFinished()
{
    auto name = ui->lnClientName->text().trimmed();  // Получение текста имени и удаление лишних пробелов.
    _client->sendName(name);  // Отправка имени через ClientManager.
}

void MainWindow::on_cmbStatus_currentIndexChanged(int index)
{
    auto status = (ChatProtocol::Status)index;  // Получение выбранного статуса из QComboBox.
    _client->sendStatus(status);  // Отправка статуса через ClientManager.
}

void MainWindow::onTyping()
{
    statusBar()->showMessage("Server is typing...", 750);  // Отображение сообщения о том, что сервер набирает текст.
}

void MainWindow::on_btnSendFile_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Select a file", "/home");  // Открытие диалогового окна для выбора файла.
    _client->sendInitSendingFile(fileName);  // Отправка запроса на отправку файла через ClientManager.
}

void MainWindow::onRejectReceivingFile()
{
    QMessageBox::critical(this, "Sending File", "Operation rejected...");  // Вывод сообщения об ошибке при отклонении операции отправки файла.
}

void MainWindow::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    auto message = QString("Client (%1) wants to send a file. Do you want to accept it?\nFile Name:%2\nFile Size: %3 bytes")
                       .arg(clientName, fileName)
                       .arg(fileSize);  // Формирование сообщения с информацией о файле для запроса подтверждения.
    auto result = QMessageBox::question(this, "Receiving File", message);  // Отображение диалогового окна с вопросом о принятии файла.
    if (result == QMessageBox::Yes) {
        _client->sendAcceptFile();  // Отправка подтверждения о принятии файла через ClientManager.
    } else {
        _client->sendRejectFile();  // Отправка отказа в принятии файла через ClientManager.
    }
}

void MainWindow::onConnectionACK(QString myName, QStringList clientsName)
{
    ui->cmbDestination->clear();  // Очистка списка получателей сообщений.
    clientsName.prepend("All");  // Добавление "All" в начало списка клиентов.
    clientsName.prepend("Server");  // Добавление "Server" в начало списка клиентов.
    foreach (auto client, clientsName) {
        ui->cmbDestination->addItem(client);  // Добавление
