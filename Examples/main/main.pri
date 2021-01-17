SOURCES += \
    $$PWD/CatAbout.cpp \
    $$PWD/CatDrawingBoard.cpp \
    $$PWD/CatQuickWidget.cpp \
    $$PWD/CatSettings.cpp \
    $$PWD/CatWidget.cpp \
    $$PWD/WinWidget.cpp \
    $$PWD/main.cpp

HEADERS += \
    $$PWD/CatAbout.h \
    $$PWD/CatDrawingBoard.h \
    $$PWD/CatQuickWidget.h \
    $$PWD/CatSettings.h \
    $$PWD/CatWidget.h \
    $$PWD/WinWidget.h

FORMS += \
    $$PWD/CatAbout.ui \
    $$PWD/CatDrawingBoard.ui \
    $$PWD/CatQuickWidget.ui \
    $$PWD/CatSettings.ui \
    $$PWD/CatWidget.ui \
    $$PWD/WinWidget.ui

include(CatConfig/CatConfig.pri)
include(WidgetTools/WidgetTools.pri)
include(QuickWidgetTools/QuickWidgetTools.pri)
