include("../common.pri")

QT       += core gui concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


TARGET = SlopeCraft

ICON = others/SlopeCraft.icns
RC_ICONS = others/SlopeCraft.ico

RC_LANG = 0x0004

VERSION += 3.8.1.0

#QMAKE_TARGET_COMPANY = TokiNoBug
#公司名称

QMAKE_TARGET_DESCRIPTION = Map Pixel Art Tool
#描述信息

QMAKE_TARGET_COPYRIGHT = TokiNoBug
#版权信息

QMAKE_TARGET_PRODUCT = SlopeCraft
#产品名称

INCLUDEPATH += \
                ../SlopeCraftL \
                ../BlockListManager

win32 {
    INCLUDEPATH += \
                $$EIGEN_INCLUDE
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += eigen3
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    AiCvterParameterDialog.cpp \
    BatchUi.cpp \
    TaskBox.cpp \
    VersionDialog.cpp \
    main.cpp \
    MainWindow.cpp \
    previewwind.cpp \
    tpstrategywind.cpp

HEADERS += \
    AiCvterParameterDialog.h \
    BatchUi.h \
    TaskBox.h \
    VersionDialog.h \
    MainWindow.h \
    previewwind.h \
    tpstrategywind.h

FORMS += \
    others/AiCvterParameterDialog.ui \
    others/BatchUi.ui \
    others/TaskBox.ui \
    others/VersionDialog.ui \
    others/mainwindow.ui \
    others/previewwind.ui \
    others/tpstrategywind.ui

TRANSLATIONS += \
    ./others/SlopeCraft_zh_CN.ts \
    ./others/SlopeCraft_en_US.ts

CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += \
    others/BlockTextures.qrc \
    others/Pics.qrc

DISTFILES += \
    others/SlopeCraft.ico


#LIBS += D:\Git\build-SlopeCraft-Desktop_Qt_6_1_0_MinGW_64_bit-Release\SlopeCraftL\release\SlopeCraftL3.dll


# LIBS += D:\Git\build-SlopeCraft-Desktop_Qt_6_1_0_MinGW_64_bit-Release\SlopeCraftL\release\SlopeCraftL3.dll
unix {
    LIBS += $$COMMON_LIB_DIR/libSlopeCraftL.a -lz -fopenmp
}
