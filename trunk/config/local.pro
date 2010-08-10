mac: {
    CONFIG += x86_64
    CONFIG -= x86
    INCLUDEPATH += /Users/andreas/X-Code/gphoto2/buildenv-mac/myprefix/include
    LIBS += -L/Users/andreas/X-Code/gphoto2/buildenv-mac/myprefix/lib
}

unix: {
    INCLUDEPATH += /home/exitas/myprefix/include/
    LIBS += -L/home/exitas/myprefix/lib/ -Wl,-rpath /home/exitas/myprefix/lib/
}

