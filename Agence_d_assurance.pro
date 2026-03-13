QT       += core gui
QT       +=sql
QT += charts
INCLUDEPATH += C:/Qt/Qt5.9.9/Tools/mingw530_32/i686-w64-mingw32/include/QtCharts
QT += printsupport
QT       +=core gui multimedia multimediawidgets
QT += core multimedia
QT       += core gui  serialport


QT += quickwidgets
QT += network
QT += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adherent.cpp \
    arduino.cpp \
    connection.cpp \
    constat.cpp \
    contrat.cpp \
    dialogadherent.cpp \
    dialogconstat.cpp \
    dialogcontrat.cpp \
    dialogemployes.cpp \
    dialogforgotpassword.cpp \
    dialoglogin.cpp \
    dialogpartenaire.cpp \
    employe.cpp \
    main.cpp \
    mainwindow.cpp \
    partenaire.cpp \
    qrcodegen.cpp \
    sms.cpp \
    smtp.cpp

HEADERS += \
    adherent.h \
    arduino.h \
    connection.h \
    constat.h \
    contrat.h \
    dialogadherent.h \
    dialogconstat.h \
    dialogcontrat.h \
    dialogemployes.h \
    dialogforgotpassword.h \
    dialoglogin.h \
    dialogpartenaire.h \
    employe.h \
    mainwindow.h \
    partenaire.h \
    qrcodegen.hpp \
    sms.h \
    smtp.h

FORMS += \
    dialogadherent.ui \
    dialogconstat.ui \
    dialogcontrat.ui \
    dialogemployes.ui \
    dialogforgotpassword.ui \
    dialoglogin.ui \
    dialogpartenaire.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    camera.qrc \
    images.qrc \
    imgcam.qrc \
    profil_image.qrc \
    profile.qrc
