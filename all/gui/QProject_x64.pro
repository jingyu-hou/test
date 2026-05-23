TEMPLATE = app
TARGET = WeICME
QT += opengl xml widgets
TMPDIR = tmp
OBJECTS_DIR = $$TMPDIR
MOC_DIR = $$TMPDIR
RCC_DIR = $$TMPDIR
UI_DIR = $$TMPDIR
VTK_DIR = $$PWD/../components/open_source/vtk
VISLIB_DIR = $$PWD/../components/commercial/vis/lib_linux
Chart_DIR = $$PWD/../components/commercial/chartdirector
FFMPEG_DIR = $$PWD/../components/open_source/ffmpeg

INCLUDEPATH += $$VTK_DIR/include/vtk-5.4 $$PWD/../components/commercial/vis/include $$Chart_DIR/include $$FFMPEG_DIR/include
LIBS += -L$$VTK_DIR/lib/vtk-5.4 -lQVTK -lvtkGraphics -lvtkQtChart -lvtkHybrid -lvtkRendering \
			-lvtkalglib -lvtkImaging -lvtksqlite -lvtkCommon -lvtkInfovis -lvtksys \
			-lvtkDICOMParser -lvtkIO -lvtktiff -lvtkexoIIc -lvtkjpeg -lvtkverdict -lvtkexpat \
			-lvtklibxml2 -lvtkViews -lvtkFiltering -lvtkmetaio -lvtkVolumeRendering \
			-lvtkfreetype -lvtkNetCDF -lvtkWidgets -lvtkftgl -lvtkzlib \
			-lvtkGenericFiltering -lvtkpng -lvtkGeovis -lvtkproj4 -lvtkParallel
LIBS += -L$$VISLIB_DIR -lvtkVISCommon -lvtkVISUnstructuredGrid -lvtkVISXYPlot
LIBS += -L$$Chart_DIR/libLinux -lchartdir
LIBS += -L$$FFMPEG_DIR/lib -lavcodec -lavdevice -lavfilter -lavformat\
		   -lavutil -lswresample -lswscale

DEFINES += SA_RIBBON_BAR_NO_EXPORT
SARibbonBar_DIR = $$PWD/../components/open_source/saribbon

INCLUDEPATH += $$PWD/src $$SARibbonBar_DIR/include/SARibbonBar $$PWD/src/Language $$PWD/src/HPprocess
HEADERS += $$PWD/src/*.h \
           $$PWD/src/PostProcess/*.h \
           $$PWD/src/Language/*.h \
           $$PWD/src/HPprocess/*.h \
           PrecompileHeader.src

SOURCES += $$PWD/src/*.cpp \
           $$PWD/src/PostProcess/*.cpp \
           $$PWD/src/HPprocess/*.cpp \
           $$PWD/src/Language/*.cpp

LIBS += -L$$SARibbonBar_DIR/lib -lSARibbonBar
QMAKE_LFLAGS += -Wl,-rpath,\$$ORIGIN/../components/open_source/saribbon/lib

RESOURCES += welcme.qrc
