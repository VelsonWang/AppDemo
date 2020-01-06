DEFINES += CORE_LIBRARY
QT += \
    network \
    printsupport \
    qml \
    sql

isEmpty(QTC_SHOW_BUILD_DATE): QTC_SHOW_BUILD_DATE = $$(QTC_SHOW_BUILD_DATE)
!isEmpty(QTC_SHOW_BUILD_DATE): DEFINES += QTC_SHOW_BUILD_DATE

include(../../AppDemoplugin.pri)
msvc: QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250
SOURCES += \
    coreplugin.cpp \
    mainwindow.cpp

HEADERS += \
    core_global.h \
    coreplugin.h \
    mainwindow.h

FORMS += \

RESOURCES += core.qrc

win32 {
#    SOURCES += progressmanager/progressmanager_win.cpp
    QT += gui-private # Uses QPlatformNativeInterface.
    LIBS += -lole32 -luser32
}
else:macx {
    OBJECTIVE_SOURCES += \
        progressmanager/progressmanager_mac.mm
    LIBS += -framework AppKit
}
else:unix {
    SOURCES += progressmanager/progressmanager_x11.cpp

    IMAGE_SIZE_LIST = 16 24 32 48 64 128 256 512

    for(imagesize, IMAGE_SIZE_LIST) {
        eval(image$${imagesize}.files = images/logo/$${imagesize}/QtProject-qtcreator.png)
        eval(image$${imagesize}.path = $$QTC_PREFIX/share/icons/hicolor/$${imagesize}x$${imagesize}/apps)
        INSTALLS += image$${imagesize}
    }
}

