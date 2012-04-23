# -------------------------------------------------
# Project created by QtCreator 2012-04-18T20:12:26
# -------------------------------------------------
QT += opengl \
    script \
    xml
TARGET = OptimalLunarLandingAnalysis
TEMPLATE = app
SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/ElevationDataMap.cpp \
    src/DataMap.cpp \
    src/MapTile.cpp \
    src/ResponsiveGraphicsScene.cpp \
    src/SlopeAnalysisMap.cpp \
    src/SlopeAnalysisMapO2.cpp \
    src/AnalysisMap.cpp \
    src/ImageDataMap.cpp \
    src/ExponentialSlopeAnalysisMap.cpp \
    src/CombinedAnalysisMap.cpp \
    src/ElevationAnalysisMap.cpp \
    src/SpatialAnalysisMap.cpp \
    src/ColorReliefDataMap.cpp
HEADERS += src/MainWindow.h \
    src/SleeperThread.h \
    src/PixmapLoader.h \
    src/ElevationDataMap.h \
    src/DataMap.h \
    src/MapTile.h \
    src/ResponsiveGraphicsScene.h \
    src/SlopeAnalysisMap.h \
    src/SlopeAnalysisMapO2.h \
    src/AnalysisMap.h \
    src/DataMapListWidgetItem.h \
    src/ImageDataMap.h \
    src/MapTileLoader.h \
    src/ExponentialSlopeAnalysisMap.h \
    src/CombinedAnalysisMap.h \
    src/ElevationAnalysisMap.h \
    src/SpatialAnalysisMap.h \
    src/ColorReliefDataMap.h
FORMS += forms/MainWindow.ui
DESTDIR = bin
OBJECTS_DIR = bin/.obj
MOC_DIR = bin/.moc
RCC_DIR = bin/.rcc

