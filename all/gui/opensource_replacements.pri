# Open source replacements for commercial components
# Commercial ChartDirector and VIS libraries have been removed.
# These open-source replacements are now the only option.

# ---- Open Source Chart (ChartDirector replacement) ----
INCLUDEPATH = $$PWD/src/opensource_chart $$INCLUDEPATH

SOURCES += $$PWD/src/opensource_chart/ChartWidget.cpp \
           $$PWD/src/opensource_chart/ChartDirectorCompat.cpp \
           $$PWD/src/opensource_chart/qchartviewer.cpp

HEADERS += $$PWD/src/opensource_chart/ChartWidget.h \
           $$PWD/src/opensource_chart/ChartDirectorCompat.h \
           $$PWD/src/opensource_chart/democharts.h \
           $$PWD/src/opensource_chart/qchartviewer.h

# ---- Open Source VIS (VIS replacement) ----
INCLUDEPATH += $$PWD/src/opensource_vis

SOURCES += $$files($$PWD/src/opensource_vis/*.cpp)

HEADERS += $$files($$PWD/src/opensource_vis/*.h)
