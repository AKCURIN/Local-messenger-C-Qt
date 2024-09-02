# Подключаем необходимые модули Qt: core, gui и network
QT       += core gui network

# Если версия Qt больше 4, добавляем модуль widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Настраиваем проект для использования стандарта C++17
CONFIG += c++17

# Вы можете сделать так, чтобы ваш код не компилировался, если он использует устаревшие API.
# Чтобы это сделать, раскомментируйте следующую строку.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # отключает все API, устаревшие до Qt 6.0.0

# Указываем файлы исходного кода, которые будут включены в проект
SOURCES += \
    ChatProtocol.cpp \
    ChatProtocol.cpp \
    ClientChatWidget.cpp \
    ClientChatWidget.cpp \
    ClientManager.cpp \
    ClientManager.cpp \
    ServerManager.cpp \
    ServerManager.cpp \
    main.cpp \
    mainwindow.cpp

# Указываем заголовочные файлы, которые будут включены в проект
HEADERS += \
    ChatProtocol.h \
    ChatProtocol.h \
    ClientChatWidget.h \
    ClientChatWidget.h \
    ClientManager.h \
    ClientManager.h \
    ServerManager.h \
    ServerManager.h \
    mainwindow.h

# Указываем файлы форм, созданных в Qt Designer, которые будут включены в проект
FORMS += \
    ClientChatWidget.ui \
    ClientChatWidget.ui \
    mainwindow.ui

# Правила по умолчанию для развертывания.
# Для QNX (операционная система реального времени) указываем путь для развертывания
qnx: target.path = /tmp/$${TARGET}/bin

# Для Unix систем, кроме Android, указываем путь для развертывания
else: unix:!android: target.path = /opt/$${TARGET}/bin

# Если путь развертывания не пустой, добавляем его в INSTALLS
!isEmpty(target.path): INSTALLS += target

# Указываем ресурсные файлы, которые будут включены в проект
RESOURCES += \
    icons.qrc
