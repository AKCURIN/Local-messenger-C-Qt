#include "ServerManager.h"

ServerManager::ServerManager(ushort port, QObject *parent)
    : QObject{parent}  // Инициализация базового класса QObject с указанием родительского объекта
{
    setupServer(port);  // Вызов функции настройки сервера с указанием порта
}

void ServerManager::notifyOtherClients(QString prevName, QString name)
{
    auto message = _protocol.setClientNameMessage(prevName, name);  // Создание сообщения об изменении имени клиента
    foreach (auto cl, _clients) {  // Перебор всех клиентов
        auto clientName = cl->property("clientName").toString();  // Получение имени клиента
        if (clientName != name) {  // Если имя клиента не совпадает с новым именем
            cl->write(message);  // Отправка сообщения клиенту
        }
    }
}

void ServerManager::onTextForOtherClients(QString message, QString receiver, QString sender)
{
    auto msg = _protocol.textMessage(message, receiver);  // Создание текстового сообщения по протоколу
    if (receiver == "All") {  // Если сообщение предназначено для всех клиентов
        foreach (auto cl, _clients) {  // Перебор всех клиентов
            auto clientName = cl->property("clientName").toString();  // Получение имени клиента
            if (clientName != sender) {  // Если имя клиента не совпадает с именем отправителя
                cl->write(msg);  // Отправка сообщения клиенту
            }
        }
    } else {  // Если сообщение предназначено для конкретного клиента
        foreach (auto cl, _clients) {  // Перебор всех клиентов
            auto clientName = cl->property("clientName").toString();  // Получение имени клиента
            if (clientName == receiver) {  // Если имя клиента совпадает с именем получателя
                cl->write(msg);  // Отправка сообщения клиенту
                return;  // Выход из функции после отправки сообщения
            }
        }
    }
}

void ServerManager::newClientConnectionReceived()
{
    auto client = _server->nextPendingConnection();  // Получение следующего ожидающего подключения

    auto id = _clients.count() + 1;  // Определение идентификатора нового клиента
    auto clientName = QString("Client (%1)").arg(id);  // Создание имени клиента
    client->setProperty("id", id);  // Установка свойства "id" для клиента
    client->setProperty("clientName", clientName);  // Установка свойства "clientName" для клиента

    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onClientDisconnected);  // Соединение сигнала отключения клиента с соответствующим слотом
    emit newClientConnected(client);  // Эмитирование сигнала о новом подключении клиента

    if (id > 1) {  // Если это не первый клиент
        auto message = _protocol.setConnectionACKMessage(clientName, _clients.keys());  // Создание сообщения подтверждения подключения
        client->write(message);  // Отправка сообщения новому клиенту

        auto newClientMessage = _protocol.setNewClientMessage(clientName);  // Создание сообщения о новом клиенте
        foreach (auto cl, _clients) {  // Перебор всех клиентов
            cl->write(newClientMessage);  // Отправка сообщения клиентам
        }
    }
    _clients[clientName] = client;  // Добавление нового клиента в список клиентов
}

void ServerManager::onClientDisconnected()
{
    auto client = qobject_cast<QTcpSocket *>(sender());  // Получение указателя на отключившийся клиент
    auto clientName = client->property("clientName").toString();  // Получение имени отключившегося клиента
    _clients.remove(clientName);  // Удаление клиента из списка клиентов
    auto message = _protocol.setClinetDisconnectedMessage(clientName);  // Создание сообщения об отключении клиента
    foreach (auto cl, _clients) {  // Перебор всех клиентов
        cl->write(message);  // Отправка сообщения клиентам
    }

    emit clientDisconnected(client);  // Эмитирование сигнала об отключении клиента
}

void ServerManager::setupServer(ushort port)
{
    _server = new QTcpServer(this);  // Создание объекта QTcpServer и установка его родителя
    connect(_server, &QTcpServer::newConnection, this, &ServerManager::newClientConnectionReceived);  // Соединение сигнала нового подключения с соответствующим слотом
    _server->listen(QHostAddress::Any, port);  // Прослушивание входящих подключений на указанном порту
}
