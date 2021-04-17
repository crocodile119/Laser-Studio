QT += widgets
QT += charts
QT += printsupport
CONFIG += thread

qtHaveModule(printsupport): QT += printsupport
RC_ICONS = LaserStudioIcon.ico
CONFIG += c++14

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
    sliderscrolllabel.cpp \
    tablescontroller.cpp \
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
    sliderscrolllabel.h \
    tablescontroller.h \
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
    sliderscrolllabel.ui \
    wetchartdialog.ui
