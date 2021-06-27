QT += core gui widgets charts printsupport
CONFIG += c++14 thread
TEMPLATE = app
TARGET = LaserStudio

qtHaveModule(printsupport): QT += printsupport
RC_ICONS = LaserStudioIcon.ico 

SOURCES +=\
    atmosphericeffectsdialog.cpp \
    binocular.cpp \
    binocularlink.cpp \
    binocularpropertiesdialog.cpp \
    binocularslistmodel.cpp \
    centralwidget.cpp \
    computeemp.cpp \
    computelea.cpp \
    description.cpp \
    dialcontrol.cpp \
    diffusionhazard.cpp \
    displayscene.cpp \
    dockcontrols.cpp \
    dockeffects.cpp \
    dockgoggle.cpp \
    dockhistory.cpp \
    docklea.cpp \
    dockresults.cpp \
    dockskin.cpp \
    empleatables.cpp \
    environmentlistmodel.cpp \
    footprintdialog.cpp \
    footprintobject.cpp \
    fresnelchartdialog.cpp \
    fresnelchartview.cpp \
    fresnelreflector.cpp \
    gotopointdialog.cpp \
    graphicsscene.cpp \
    gridlines.cpp \
    labeditdialog.cpp \
    labroom.cpp \
    lambertianchartdialog.cpp \
    lambertianchartview.cpp \
    lambertianreflector.cpp \
    laserapplication.cpp \
    laserclasscw.cpp \
    laserclassmp.cpp \
    laserclasssp.cpp \
    lasergoggle.cpp \
    laserlistmodel.cpp \
    laserpoint.cpp \
    laserpropertiesdialog.cpp \
    laserreport.cpp \
    lasersafety.cpp \
    lasersafetycw.cpp \
    lasersafetymp.cpp \
    laserskinsafety.cpp \
    laserskinsafetymp.cpp \
    link.cpp \
    main.cpp \
    mainwindow.cpp \
    mychartview.cpp \
    objectlink.cpp \
    reflector.cpp \
    reflectorpropertiesdialog.cpp \
    reflectorslistmodel.cpp \
    reflectorsqlist.cpp \
    scalenumbersmodelview.cpp \
    scientificnotationcontrol.cpp \
    scrollbarcontrol.cpp \
    sliderscrolllabel.cpp \
    tablescontroller.cpp \
    undo_commands/addbinocularcommand.cpp \
    undo_commands/addcontrolvaluecommand.cpp \
    undo_commands/adddialvaluecommand.cpp \
    undo_commands/addfootprintcommand.cpp \
    undo_commands/addreflectorcommand.cpp \
    undo_commands/addscrollbarvaluecommand.cpp \
    undo_commands/addscrollslidervaluecommand.cpp \
    undo_commands/deletebinocularcommand.cpp \
    undo_commands/deletefootprintcommand.cpp \
    undo_commands/deletereflectorcommand.cpp \
    undo_commands/movecommand.cpp \
    wetchartdialog.cpp \
    wetchartview.cpp \
    wetreflector.cpp

HEADERS  += mainwindow.h \
    atmosphericeffectsdialog.h \
    binocular.h \
    binocularlink.h \
    binocularpropertiesdialog.h \
    binocularslistmodel.h \
    centralwidget.h \
    computeemp.h \
    computelea.h \
    description.h \
    dialcontrol.h \
    diffusionhazard.h \
    displayscene.h \
    dockcontrols.h \
    dockeffects.h \
    dockgoggle.h \
    dockhistory.h \
    docklea.h \
    dockresults.h \
    dockskin.h \
    empdata.h \
    empleatables.h \
    environmentlistmodel.h \
    footprintdialog.h \
    footprintobject.h \
    fresnelchartdialog.h \
    fresnelchartview.h \
    fresnelreflector.h \
    gotopointdialog.h \
    graphicsscene.h \
    gridlines.h \
    labeditdialog.h \
    labroom.h \
    lambertianchartdialog.h \
    lambertianchartview.h \
    lambertianreflector.h \
    laserapplication.h \
    laserclasscw.h \
    laserclassmp.h \
    laserclasssp.h \
    lasergoggle.h \
    laserlistmodel.h \
    laserpoint.h \
    laserpropertiesdialog.h \
    laserreport.h \
    lasersafety.h \
    lasersafetycw.h \
    lasersafetymp.h \
    laserskinsafety.h \
    laserskinsafetymp.h \
    leadata.h \
    link.h \
    mychartview.h \
    objectlink.h \
    reflector.h \
    reflectorpropertiesdialog.h \
    reflectorslistmodel.h \
    reflectorsqlist.h \
    scalenumbersmodelview.h \
    scientificnotationcontrol.h \
    scrollbarcontrol.h \
    sliderscrolllabel.h \
    tablescontroller.h \
    undo_commands/addbinocularcommand.h \
    undo_commands/addcontrolvaluecommand.h \
    undo_commands/adddialvaluecommand.h \
    undo_commands/addfootprintcommand.h \
    undo_commands/addreflectorcommand.h \
    undo_commands/addscrollbarvaluecommand.h \
    undo_commands/addscrollslidervaluecommand.h \
    undo_commands/deletebinocularcommand.h \
    undo_commands/deletefootprintcommand.h \
    undo_commands/deletereflectorcommand.h \
    undo_commands/movecommand.h \
    wetchartdialog.h \
    wetchartview.h \
    wetreflector.h

RESOURCES       = LaserStudio.qrc

FORMS    += \
    atmosphericeffectsdialog.ui \
    binocularpropertiesdialog.ui \
    description.ui \
    dockcontrols.ui \
    dockeffects.ui \
    dockgoggle.ui \
    dockhistory.ui \
    docklea.ui \
    dockresults.ui \
    dockskin.ui \
    footprintdialog.ui \
    fresnelchartdialog.ui \
    gotopointdialog.ui \
    labeditdialog.ui \
    lambertianchartdialog.ui \
    laserpropertiesdialog.ui \
    reflectorpropertiesdialog.ui \
    reflectorsqlist.ui \
    wetchartdialog.ui

DISTFILES += \
    ls_model.qmodel

STATECHARTS += \
    LS_State_Chart.scxml
