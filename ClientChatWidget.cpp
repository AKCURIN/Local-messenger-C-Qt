#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"

// Подключаем заголовочные файлы для ClientChatWidget и автоматически сгенерированный файл интерфейса

#include <QMessageBox>
#include <QDesktopServices>

// Подключаем классы для отображения сообщений и открытия папок

ClientChatWidget::ClientChatWidget(QTcpSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    // Устанавливаем интерфейс
    _client = new ClientManager(client, this);
    // Создаем новый объект ClientManager с переданным сокетом
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    // Подключаем сигнал отключения клиента к соответствующему слоту
    connect(_client, &ClientManager::textMessageReceived, this, &ClientChatWidget::textMessageReceived);
    // Подключаем сигнал получения текстового сообщения к соответствующему слоту
    connect(_client, &ClientManager::isTyping, this, &ClientChatWidget::onTyping);
    // Подключаем сигнал набора текста к соответствующему слоту
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::onClientNameChanged);
    // Подключаем сигнал изменения имени клиента к соответствующему слоту
    connect(_client, &ClientManager::statusChanged, this, &ClientChatWidget::statusChanged);
    // Подключаем сигнал изменения статуса клиента к соответствующему слоту
    connect(_client, &ClientManager::initReceivingFile, this, &ClientChatWidget::onInitReceivingFile);
    // Подключаем сигнал инициализации приема файла к соответствующему слоту
    connect(_client, &ClientManager::fileSaved, this, &ClientChatWidget::onFileSaved);
    // Подключаем сигнал сохранения файла к соответствующему слоту
    connect(ui->lnMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
    // Подключаем сигнал изменения текста в поле ввода сообщения к методу отправки сообщения о наборе текста

    dir.mkdir(_client->name());
    // Создаем директорию для клиента с его именем
    dir.setPath("./" + _client->name());
    // Устанавливаем путь к директории клиента
}

void ClientChatWidget::disconnect()
{
    // Метод для отключения клиента
    _client->disconnectFromHost();
}

ClientChatWidget::~ClientChatWidget()
{
    // Деструктор для очистки ресурсов
    delete ui;
}

void ClientChatWidget::clientDisconnected()
{
    // Слот, вызываемый при отключении клиента
    ui->wdgSendMessage->setEnabled(false);
    // Отключаем возможность отправки сообщений
}

void ClientChatWidget::on_btnSend_clicked()
{
    // Слот, вызываемый при нажатии кнопки отправки сообщения
    auto message = ui->lnMessage->text().trimmed();
    // Получаем и обрезаем пробелы у введенного сообщения
    _client->sendMessage(message);
    // Отправляем сообщение
    ui->lnMessage->setText("");
    // Очищаем поле ввода сообщения
    ui->lstMessages->addItem(message);
    // Добавляем сообщение в список сообщений
}

void ClientChatWidget::textMessageReceived(QString message, QString receiver)
{
    // Слот, вызываемый при получении текстового сообщения
    if (receiver == "Server" || receiver == "All") {
        ui->lstMessages->addItem(message);
        // Если получатель - сервер или все, добавляем сообщение в список сообщений
    }
    if (receiver != "Server") {
        emit textForOtherClients(message, receiver, _client->name());
        // Если получатель не сервер, отправляем сообщение другим клиентам
    }
}

void ClientChatWidget::onTyping()
{
    // Слот, вызываемый при наборе текста клиентом
    emit isTyping(QString("%1 is typing...").arg(_client->name()));
    // Отправляем сигнал о наборе текста с именем клиента
}

void ClientChatWidget::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    // Слот, вызываемый при инициализации приема файла
    auto message = QString("Client (%1) wants to send a file. Do you want to accept it?\nFile Name:%2\nFile Size: %3 bytes")
                       .arg(clientName, fileName)
                       .arg(fileSize);
    // Формируем сообщение с информацией о файле
    auto result = QMessageBox::question(this, "Receiving File", message);
    // Отображаем диалоговое окно с вопросом о приеме файла
    if (result == QMessageBox::Yes) {
        _client->sendAcceptFile();
        // Если пользователь согласился, отправляем сообщение о принятии файла
    } else {
        _client->sendRejectFile();
        // Если пользователь отказался, отправляем сообщение об отказе файла
    }
}

void ClientChatWidget::onFileSaved(QString path)
{
    // Слот, вызываемый при сохранении файла
    auto message = QString("File saved here:\n%1").arg(path);
    // Формируем сообщение с информацией о пути сохраненного файла
    QMessageBox::information(this, "File saved", message);
    // Отображаем диалоговое окно с информацией о сохранении файла
}

void ClientChatWidget::on_lblOpenFolder_linkActivated(const QString &link)
{
    // Слот, вызываемый при активации ссылки на открытие папки
    QDesktopServices::openUrl(QUrl::fromLocalFile(_client->name()));
    // Открываем директорию клиента
}

void ClientChatWidget::onClientNameChanged(QString prevName, QString name)
{
    // Слот, вызываемый при изменении имени клиента
    QFile::rename(dir.canonicalPath(), name);
    // Переименовываем директорию клиента
    emit clientNameChanged(prevName, name);
    // Отправляем сигнал об изменении имени клиента
}
