#-------------------------------------------------
#
# Project created by QtCreator 2021-01-11T17:20:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChickenFarm
TEMPLATE = app

INCLUDEPATH += Comm/FreeModbus/config \
            Comm/FreeModbus/driver \
            Comm/FreeModbus/utils \
            Comm/FreeModbus/master/functions \
            Comm/FreeModbus/master/port \
            Comm/FreeModbus/master/rtu \
            Comm/FreeModbus/slave/functions \
            Comm/FreeModbus/slave/port \
            Comm/FreeModbus/slave/rtu \
            Widget/Label \
            Widget/Button \
            Widget/Keyboard \
            Device \
            DataManage \
            Form/Module \
            Form \
            System

LIBS += -lrt \
        -lpthread \


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        Comm/FreeModbus/driver/ti_mbdriver.c \
        Comm/FreeModbus/master/functions/mb_m.c \
        Comm/FreeModbus/master/functions/mbbits_m.c \
        Comm/FreeModbus/master/functions/mbfunccoils_m.c \
        Comm/FreeModbus/master/functions/mbfuncdisc_m.c \
        Comm/FreeModbus/master/functions/mbfuncholding_m.c \
        Comm/FreeModbus/master/functions/mbfuncinput_m.c \
        Comm/FreeModbus/master/functions/mbmap_m.c \
        Comm/FreeModbus/master/functions/mbscan_m.c \
        Comm/FreeModbus/master/functions/mbtest_m.c \
        Comm/FreeModbus/master/port/mbportevent_m.c \
        Comm/FreeModbus/master/port/mbportserial_m.c \
        Comm/FreeModbus/master/port/mbporttimer_m.c \
        Comm/FreeModbus/master/port/tempCodeRunnerFile.c \
        Comm/FreeModbus/master/rtu/mbrtu_m.c \
        Comm/FreeModbus/slave/ascii/mbascii.c \
        Comm/FreeModbus/slave/functions/mb.c \
        Comm/FreeModbus/slave/functions/mbbits.c \
        Comm/FreeModbus/slave/functions/mbfunccoils.c \
        Comm/FreeModbus/slave/functions/mbfuncdiag.c \
        Comm/FreeModbus/slave/functions/mbfuncdisc.c \
        Comm/FreeModbus/slave/functions/mbfuncholding.c \
        Comm/FreeModbus/slave/functions/mbfuncinput.c \
        Comm/FreeModbus/slave/functions/mbfuncother.c \
        Comm/FreeModbus/slave/functions/mbmap.c \
        Comm/FreeModbus/slave/port/mbportevent.c \
        Comm/FreeModbus/slave/port/mbportserial.c \
        Comm/FreeModbus/slave/port/mbporttimer.c \
        Comm/FreeModbus/slave/rtu/mbrtu.c \
        Comm/FreeModbus/slave/tcp/mbtcp.c \
        Comm/FreeModbus/utils/mbutils.c \
        Comm/modbus.cpp \
        DataManage/datamonitor.cpp \
        Device/bump.cpp \
        Device/controller.cpp \
        Device/device.cpp \
        Device/fan.cpp \
        Device/sensor.cpp \
        Form/Module/axialfan.cpp \
        Form/Module/butterflyvalve.cpp \
        Form/Module/chilledbump.cpp \
        Form/Module/co2sensor.cpp \
        Form/Module/modular.cpp \
        Form/Module/modularair.cpp \
        Form/Module/modularchiller.cpp \
        Form/Module/pressuresensor.cpp \
        Form/Module/temphumisensor.cpp \
        Form/Module/tempsensor.cpp \
        Form/Page/systemmonitor.cpp \
        System/system.cpp \
        Widget/Button/analogvalbutton.cpp \
        Widget/Button/button.cpp \
        Widget/Button/modebutton.cpp \
        Widget/Button/statebutton.cpp \
        Widget/Keyboard/akeyboard.cpp \
        Widget/Keyboard/keybutton.cpp \
        Widget/Keyboard/pwdkeyboard.cpp \
        Widget/Keyboard/skeyboard.cpp \
        Widget/Label/datalabel.cpp \
        Widget/Label/textcontrol.cpp \
        Widget/Label/textlabel.cpp \
        main.cpp \
        Form/mainform.cpp

HEADERS += \
        Comm/FreeModbus/config/mbconfig.h \
        Comm/FreeModbus/config/mbframe.h \
        Comm/FreeModbus/config/mbproto.h \
        Comm/FreeModbus/config/port.h \
        Comm/FreeModbus/driver/mbdriver.h \
        Comm/FreeModbus/master/functions/mb_m.h \
        Comm/FreeModbus/master/functions/mbbits_m.h \
        Comm/FreeModbus/master/functions/mbdict_m.h \
        Comm/FreeModbus/master/functions/mbfunc_m.h \
        Comm/FreeModbus/master/functions/mbmap_m.h \
        Comm/FreeModbus/master/functions/mbscan_m.h \
        Comm/FreeModbus/master/functions/mbtest_m.h \
        Comm/FreeModbus/master/port/mbport_m.h \
        Comm/FreeModbus/master/rtu/mbrtu_m.h \
        Comm/FreeModbus/slave/ascii/mbascii.h \
        Comm/FreeModbus/slave/functions/mb.h \
        Comm/FreeModbus/slave/functions/mbbits.h \
        Comm/FreeModbus/slave/functions/mbdict.h \
        Comm/FreeModbus/slave/functions/mbfunc.h \
        Comm/FreeModbus/slave/functions/mbmap.h \
        Comm/FreeModbus/slave/port/mbport.h \
        Comm/FreeModbus/slave/rtu/mbrtu.h \
        Comm/FreeModbus/slave/tcp/mbtcp.h \
        Comm/FreeModbus/utils/mbutils.h \
        Comm/modbus.h \
        DataManage/datamonitor.h \
        Device/bump.h \
        Device/controller.h \
        Device/device.h \
        Device/fan.h \
        Device/sensor.h \
        Form/Module/axialfan.h \
        Form/Module/butterflyvalve.h \
        Form/Module/chilledbump.h \
        Form/Module/co2sensor.h \
        Form/Module/modular.h \
        Form/Module/modularair.h \
        Form/Module/modularchiller.h \
        Form/Module/pressuresensor.h \
        Form/Module/temphumisensor.h \
        Form/Module/tempsensor.h \
        Form/Page/systemmonitor.h \
        System/system.h \
        Form/mainform.h \
        Widget/Button/analogvalbutton.h \
        Widget/Button/button.h \
    Widget/Button/modebutton.h \
        Widget/Button/statebutton.h \
    Widget/Keyboard/akeyboard.h \
    Widget/Keyboard/keybutton.h \
    Widget/Keyboard/pwdkeyboard.h \
    Widget/Keyboard/skeyboard.h \
        Widget/Label/datalabel.h \
    Widget/Label/textcontrol.h \
        Widget/Label/textlabel.h \

FORMS += \
        Form/Module/axialfan.ui \
        Form/Module/butterflyvalve.ui \
        Form/Module/chilledbump.ui \
        Form/Module/co2sensor.ui \
        Form/Module/modular.ui \
        Form/Module/modularair.ui \
        Form/Module/modularchiller.ui \
        Form/Module/pressuresensor.ui \
        Form/Module/temphumisensor.ui \
        Form/Module/tempsensor.ui \
        Form/Page/systemmonitor.ui \
        Form/mainform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    uiresource.qrc

