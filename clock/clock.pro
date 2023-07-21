TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        beijingtime.cpp \
        canadatime.cpp \
        dongjingtime.cpp \
        main.cpp \
        observer.cpp \
        subject.cpp

HEADERS += \
    beijingtime.h \
    canadatime.h \
    dongjingtime.h \
    observer.h \
    subject.h
