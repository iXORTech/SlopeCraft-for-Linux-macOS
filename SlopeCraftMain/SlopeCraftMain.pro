include("../common.pri")

QT       += core gui concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


TARGET = SlopeCraft

ICON = others/SlopeCraftIconNew.icns
RC_ICONS = others/SlopeCraftIconNew.ico

RC_LANG = 0x0004

VERSION += 3.9.0.0

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
    INCLUDEPATH += \
                $$EIGEN_INCLUDE
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

# LIBS += D:\Git\build-SlopeCraft-Desktop_Qt_6_1_0_MinGW_64_bit-Release\SlopeCraftL\release\SlopeCraftL3.dll

unix {
    LIBS += $$COMMON_LIB_DIR/libSlopeCraftL.a -lz -fopenmp
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BlockListManager/release/ -lBlockListManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BlockListManager/debug/ -lBlockListManager
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lBlockListManager

INCLUDEPATH += $$PWD/../BlockListManager
DEPENDPATH += $$PWD/../BlockListManager

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../BlockListManager/release/libBlockListManager.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../BlockListManager/debug/libBlockListManager.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../BlockListManager/release/BlockListManager.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../BlockListManager/debug/BlockListManager.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libBlockListManager.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SlopeCraftL/release/ -lSlopeCraftL3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SlopeCraftL/debug/ -lSlopeCraftL3
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lSlopeCraftL

INCLUDEPATH += $$PWD/../SlopeCraftL
DEPENDPATH += $$PWD/../SlopeCraftL


win32: {
    SlopeCraft_blocks_dir=$$PWD/../blocks
    CONFIG(release,debug|release): {

    SlopeCraftMain_exe_dir=$$OUT_PWD/../SlopeCraftMain/release
    SlopeCraftL_shared_dir=$$OUT_PWD/../SlopeCraftL/release

        #QMAKE_PRE_LINK += del .\SlopeCraftMain\release\SlopeCraftL3.dll
        #QMAKE_PRE_LINK+=copy /Y $$SlopeCraftMain_exe_dir\SlopeCraftL3.dll $$SlopeCraftMain_exe_dir
        #QMAKE_PRE_LINK+=Xcopy $$PWD\..\Blocks
    }
    else: {
    SlopeCraftMain_exe_dir=$$OUT_PWD/../SlopeCraftMain/debug
    SlopeCraftL_shared_dir=$$OUT_PWD/../SlopeCraftL/debug
        #QMAKE_PRE_LINK += del .\SlopeCraftMain\debug\SlopeCraftL3.dll
        #QMAKE_PRE_LINK+=copy /Y .\SlopeCraftL\debug\SlopeCraftL3.dll .\SlopeCraftMain\debug
    }
    QMAKE_PRE_LINK+=copy /Y \"$$SlopeCraftL_shared_dir\SlopeCraftL3.dll\" \"$$SlopeCraftMain_exe_dir\"
    QMAKE_PRE_LINK+= & Xcopy \"$$SlopeCraft_blocks_dir\" \"$$SlopeCraftMain_exe_dir/blocks\" /Y /E /K /I
#message("SlopeCraftMain_exe_dir = "$$SlopeCraftMain_exe_dir)
#message("SlopeCraftL_shared_dir = "$$SlopeCraftL_shared_dir)
message("command = "$$QMAKE_PRE_LINK)
}
else: {
    message(Copying and pasting SlopeCraftL3.dll on other platforms is not implemented yet. \
        Remind to copy this dynamic linked library manually)
}

message("SlopeCraft_blocks_dir = "$$SlopeCraft_blocks_dir)

#message("PWD = ")
#message($$PWD)
