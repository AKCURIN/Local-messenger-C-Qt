QT       += core gui network  # Добавляет модули Qt, которые будут использоваться в проекте: core (основные функциональные возможности), gui (интерфейс пользователя) и network (работа с сетью).

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  # Добавляет модуль widgets, если версия Qt больше 4.

CONFIG += c++17  # Настраивает проект на использование стандарта C++17.

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # Отключает все API, устаревшие до версии Qt 6.0.0

SOURCES += \  # Указывает файлы с исходным кодом, которые будут скомпилированы.
    ChatItemWidget.cpp \  # Файл исходного кода для виджета элемента чата.
    ChatProtocol.cpp \
    ChatProtocol.cpp \  # Файл исходного кода для протокола чата.
    ChatltemWidget.cpp \
    ClientManager.cpp \
    ClientManager.cpp \  # Файл исходного кода для менеджера клиентов.
    authorizationwindow.cpp \
    authorizationwindow.cpp \  # Файл исходного кода для окна авторизации.
    main.cpp \  # Главный файл исходного кода, содержащий функцию main.
    mainwindow.cpp  # Файл исходного кода для главного окна приложения.

HEADERS += \  # Указывает заголовочные файлы, которые будут включены в проект.
    ChatItemWidget.h \  # Заголовочный файл для виджета элемента чата.
    ChatProtocol.h \
    ChatProtocol.h \  # Заголовочный файл для протокола чата.
    ChatltemWidget.h \
    ClientManager.h \
    ClientManager.h \  # Заголовочный файл для менеджера клиентов.
    authorizationwindow.h \
    authorizationwindow.h \  # Заголовочный файл для окна авторизации.
    mainwindow.h  # Заголовочный файл для главного окна приложения.

FORMS += \  # Указывает файлы форм (UI файлы), которые будут использоваться в проекте.
    ChatItemWidget.ui \  # UI файл для виджета элемента чата.
    ChatltemWidget.ui \
    authorizationwindow.ui \
    authorizationwindow.ui \  # UI файл для окна авторизации.
    mainwindow.ui  # UI файл для главного окна приложения.

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin  # Устанавливает путь для целевого файла в системе QNX.
else: unix:!android: target.path = /opt/$${TARGET}/bin  # Устанавливает путь для целевого файла в системах Unix, кроме Android.
!isEmpty(target.path): INSTALLS += target  # Добавляет целевой файл в список установочных файлов, если путь не пуст.
