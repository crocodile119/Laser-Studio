QT += core gui widgets charts printsupport
CONFIG += c++14 thread
TEMPLATE = app
TARGET = LaserStudio

qtHaveModule(printsupport): QT += printsupport
RC_ICONS = LaserStudioIcon.ico 

SOURCES +=\
    atmosphericeffectsdialog.cpp \
    beaminspector.cpp \
    beaminspectorchart.cpp \
    beaminspectordialog.cpp \
    binocular.cpp \
    binocularlink.cpp \
    binocularpropertiesdialog.cpp \
    centralwidget.cpp \
    chart.cpp \
    chartview.cpp \
    comboboxbands.cpp \
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
    exendedchartview.cpp \
    floatcomparison.cpp \
    footprintdialog.cpp \
    footprintobject.cpp \
    fresnelchartdialog.cpp \
    fresnelchartview.cpp \
    fresnelreflector.cpp \
    gotopointdialog.cpp \
    graphicsitemtree.cpp \
    graphicsscene.cpp \
    gridlines.cpp \
    htmldelegate.cpp \
    inspectorlink.cpp \
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
    laserpoint.cpp \
    laserpropertiesdialog.cpp \
    laserreport.cpp \
    lasersafety.cpp \
    lasersafetycw.cpp \
    lasersafetymp.cpp \
    laserskinsafety.cpp \
    laserskinsafetymp.cpp \
    main.cpp \
    mainwindow.cpp \
    mychartview.cpp \
    mycombobox.cpp \
    objectlink.cpp \
    reflector.cpp \
    reflectorlink.cpp \
    reflectorpropertiesdialog.cpp \
    scalenumbersmodelview.cpp \
    scientificnotationcontrol.cpp \
    scientificnumber.cpp \
    scrollbarcontrol.cpp \
    sliderscrolllabel.cpp \
    tablescontroller.cpp \
    treeitem.cpp \
    treemodel.cpp \
    undo_commands/addbeaminspectorcommand.cpp \
    undo_commands/addbinocularcommand.cpp \
    undo_commands/addbinocularpropertycommand.cpp \
    undo_commands/addcomboboxvaluecommand.cpp \
    undo_commands/addcontrolvaluecommand.cpp \
    undo_commands/adddialvaluecommand.cpp \
    undo_commands/addfootprintcommand.cpp \
    undo_commands/addfootprintpropertycommand.cpp \
    undo_commands/addinspectorpropertycommand.cpp \
    undo_commands/addlabpropertycommand.cpp \
    undo_commands/addlaserpropertycommand.cpp \
    undo_commands/addmeteocommand.cpp \
    undo_commands/addnindexcommand.cpp \
    undo_commands/addreflectorcommand.cpp \
    undo_commands/addreflectorpropertycommand.cpp \
    undo_commands/addscrollbarvaluecommand.cpp \
    undo_commands/addscrollslidervaluecommand.cpp \
    undo_commands/boxredimensioncommand.cpp \
    undo_commands/deletebeaminspectorcommand.cpp \
    undo_commands/deletebinocularcommand.cpp \
    undo_commands/deletefootprintcommand.cpp \
    undo_commands/deletereflectorcommand.cpp \
    undo_commands/movecommand.cpp \
    wetchartdialog.cpp \
    wetchartview.cpp \
    wetreflector.cpp

HEADERS  += mainwindow.h \
    atmosphericeffectsdialog.h \
    beaminspector.h \
    beaminspectorchart.h \
    beaminspectordialog.h \
    binocular.h \
    binocularlink.h \
    binocularpropertiesdialog.h \
    centralwidget.h \
    chart.h \
    chartview.h \
    comboboxbands.h \
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
    exendedchartview.h \
    floatcomparison.h \
    footprintdialog.h \
    footprintobject.h \
    fresnelchartdialog.h \
    fresnelchartview.h \
    fresnelreflector.h \
    gotopointdialog.h \
    graphicsitemtree.h \
    graphicsscene.h \
    gridlines.h \
    htmldelegate.h \
    inspectorlink.h \
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
    laserpoint.h \
    laserpropertiesdialog.h \
    laserreport.h \
    lasersafety.h \
    lasersafetycw.h \
    lasersafetymp.h \
    laserskinsafety.h \
    laserskinsafetymp.h \
    leadata.h \
    mychartview.h \
    mycombobox.h \
    objectlink.h \
    reflector.h \
    reflectorlink.h \
    reflectorpropertiesdialog.h \
    scalenumbersmodelview.h \
    scientificnotationcontrol.h \
    scientificnumber.h \
    scrollbarcontrol.h \
    sliderscrolllabel.h \
    tablescontroller.h \
    treeitem.h \
    treemodel.h \
    undo_commands/addbeaminspectorcommand.h \
    undo_commands/addbinocularcommand.h \
    undo_commands/addbinocularpropertycommand.h \
    undo_commands/addcomboboxvaluecommand.h \
    undo_commands/addcontrolvaluecommand.h \
    undo_commands/adddialvaluecommand.h \
    undo_commands/addfootprintcommand.h \
    undo_commands/addfootprintpropertycommand.h \
    undo_commands/addinspectorpropertycommand.h \
    undo_commands/addlabpropertycommand.h \
    undo_commands/addlaserpropertycommand.h \
    undo_commands/addmeteocommand.h \
    undo_commands/addnindexcommand.h \
    undo_commands/addreflectorcommand.h \
    undo_commands/addreflectorpropertycommand.h \
    undo_commands/addscrollbarvaluecommand.h \
    undo_commands/addscrollslidervaluecommand.h \
    undo_commands/boxredimensioncommand.h \
    undo_commands/deletebeaminspectorcommand.h \
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
    graphicsitemtree.ui \
    labeditdialog.ui \
    lambertianchartdialog.ui \
    laserpropertiesdialog.ui \
    reflectorpropertiesdialog.ui \
    wetchartdialog.ui

DISTFILES += \
    ls_model.qmodel

STATECHARTS += \
    LS_State_Chart.scxml
