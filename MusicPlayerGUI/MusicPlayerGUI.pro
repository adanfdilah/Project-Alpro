QT       += core gui widgets multimedia network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG -= console
CONFIG += windows


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clickabletextedit.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    clickabletextedit.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    MusicPlayerGUI_id_ID.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    appicon.rc \
    musicplayer_multisize.ico

RC_FILE = appicon.rc
