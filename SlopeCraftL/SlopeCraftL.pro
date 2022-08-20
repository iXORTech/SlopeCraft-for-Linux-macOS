CONFIG -= qt

TEMPLATE = lib
CONFIG += shared
CONFIG += c++17

include('../common.pri')

#INCLUDEPATH += \
#                               ../AiCvterInterface \
#                               ../GAConverter \
#                               D:\CppLibs\HeuristicFlow-1.5.0

CONFIG += sharedlib

unix {
    INCLUDEPATH += \
                    ../AiCvterInterface \
                    ../GAConverter \
                    $$HEURISTIC_FLOW_INCLUDE
    CONFIG += link_pkgconfig create_pc
    PKGCONFIG += zlib eigen3
}

win32 {
    INCLUDEPATH += \
                ../AiCvterInterface \
                ../GAConverter \
                $$ZLIB_INCLUDE \
                $$EIGEN_INCLUDE \
                $$HEURISTIC_FLOW_INCLUDE
}

DESTDIR = $$COMMON_LIB_DIR

RC_LANG = 0x0004

VERSION += 3.9.0.0

QMAKE_TARGET_DESCRIPTION = SlopeCraft Library# (with AiConverter)
#描述信息

QMAKE_TARGET_COPYRIGHT = TokiNoBug
#版权信息

QMAKE_TARGET_PRODUCT = SlopeCraftL
#产品名称

QMAKE_TARGET_INTERNALNAME = SlopeCraftL

QMAKE_CXXFLAGS += -fopenmp

LIBS += -fopenmp -lz

DEFINES += SLOPECRAFTL_LIBRARY


LIBS += -lz

#LIBS += D:\CppLibs\zlib\lib\libzlibstatic.a
#LIBS += -lz


SOURCES += \
    AiCvterOpt.cpp \
    CIEDE00.cpp \
    ColorSource.cpp \
    HeightLine.cpp \
    OptiChain.cpp \
    PrimGlassBuilder.cpp \
    SlopeCraftL_CAPI.cpp \
    TokiSlopeCraft.cpp \
    TokiSlopeCraft_build.cpp \
    TokiSlopeCraft_convert.cpp \
    WaterItem.cpp \
    colorset.cpp \
    imagePreprocess.cpp \
    lossyCompressor.cpp \
    simpleBlock.cpp \
    tokicolor.cpp \
    SlopeCraftL.cpp

HEADERS += \
    AiCvterOpt.h \
    ColorSet.h \
    HeightLine.h \
    newNBTWriter.hpp \
    OptiChain.h \
    PrimGlassBuilder.h \
    SCLDefines.h \
    TokiColor.h \
    TokiSlopeCraft.h \
    WaterItem.h \
    lossyCompressor.h \
    object_pool.hpp \
    simpleBlock.h \
    SlopeCraftL.h \
    SlopeCraftL_global.h \
    defines_of_extern_classes.h

# Default rules for deployment.
#unix {
#    target.path = /usr/lib
#}
#!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GAConverter/release/ -lGAConverter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GAConverter/debug/ -lGAConverter
else:unix: LIBS += -L$$OUT_PWD/../GAConverter/ -lGAConverter

INCLUDEPATH += $$PWD/../GAConverter
DEPENDPATH += $$PWD/../GAConverter

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GAConverter/release/libGAConverter.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GAConverter/debug/libGAConverter.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GAConverter/release/GAConverter.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GAConverter/debug/GAConverter.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../GAConverter/libGAConverter.a
