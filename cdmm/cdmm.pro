TEMPLATE = app
INCLUDEPATH += . ../common
DEPENDPATH = . ../common
OBJECTS_DIR = tmp
TARGET    = cdmm
VERSION   = 0.4
DESTDIR   = ../bin
#CONFIG -= release
#CONFIG += debug

CONFIG -= qt
CONFIG += console

unix {
  LIBS += -lpthread
}

# Input
HEADERS += util.h \
           port.h \
           serialport.h \
           configuration.h \
           mutex.h \
           thread.h \
           dmmclass.h \
           metexascii.h \
           escort97.h \
           logger.h \
           vc820.h \
           vc870.h \
           fluke45.h \
           m9803r.h \
           iocbuvmonitor.h \
           gdm703.h \
           peaktech10ascii.h \
           metex14bytespolling.h \
           metex14bytescontinuous.h \
           metex15bytescontinuous.h \
           qm1537.h \
           vc940.h \
           idm73.h \
           pt506.h \
           telnetconnection.h \
           telnetserver.h \
           socket.h \
           serversocket.h \
           socketexception.h \
	   udpserver.h \
	   vichy12bytepolling.h \
	   vichyascii.h

SOURCES += main.cpp \
           util.cpp \
           port.cpp \
           serialport.cpp \
           ../common/configuration.cpp \
           mutex.cpp \
           thread.cpp \
           dmmclass.cpp \
           metexascii.cpp \
           logger.cpp \
           vc820.cpp \
           vc870.cpp \
           fluke45.cpp \
           m9803r.cpp \
           escort97.cpp \
           iocbuvmonitor.cpp \
           gdm703.cpp \
           peaktech10ascii.cpp \
           metex14bytespolling.cpp \
           metex14bytescontinuous.cpp \
           metex15bytescontinuous.cpp \
           qm1537.cpp \
           vc940.cpp \
           idm73.cpp \
           pt506.cpp \
           telnetconnection.cpp \
           telnetserver.cpp \
           socket.cpp \
           serversocket.cpp \
	   udpserver.cpp \
	   vichy12bytepolling.cpp \
	   vichyascii.cpp
