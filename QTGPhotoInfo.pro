HEADERS += src/infodialog.h \
    src/imagedisplaydialog.h \
    src/settingsdialog.h 
SOURCES += src/infodialog.cpp \
    src/main.cpp \
    src/imagedisplaydialog.cpp \
    src/settingsdialog.cpp 
FORMS += forms/infodialog.ui \
    forms/imagedisplaydialog.ui \
    forms/settingsdialog.ui

LIBS += -lgphoto2 \
    -lm \
    -lgphoto2_port \
    -ldl



# All objects should go into tmp
MOC_DIR = tmp/
UI_DIR = tmp/
UI_HEADER_DIR = tmp/
UI_SOURCE_DIR = tmp/
OBJECTS_DIR = tmp/
RCC_DIR = tmp/

include("config/local.pro")
