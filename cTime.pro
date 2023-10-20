TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/LibOb/CommonCpp/LibOb_strptime.c \
    src/main.cpp \
    src/LibCpp/Time/cTimeStd.cpp \

HEADERS += \
    src/LibCpp/Time/cTime.h \
    src/LibOb/CommonCpp/LibOb_strptime.h
