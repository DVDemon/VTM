#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T16:35:23
#
#-------------------------------------------------

QT += core gui
QT += widgets
CONFIG += app_bundle

macx: QMAKE_INFO_PLIST = MyAppInfo.plist
macx: ICON = Resources\app.icns

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = VirtualTuringMachine
TEMPLATE = app

#macx:ICON = $${PWD}/vtm.icns

SOURCES += \
    configuration.cpp \
    formeditormachine.cpp \
    formeditorwidget.cpp \
    formmain.cpp \
    formnewproject.cpp \
    main.cpp \
    mainwindow.cpp \
    formdebugermachine.cpp \
    formcompilermachine.cpp \
    formlinewidget.cpp \
    formwarning.cpp \
    formalphabit.cpp \
    flowlayout.cpp \
    formnewmachine.cpp \
    machineslibrary.cpp \
    uicanvas.cpp \
    VMTAlphabit.cpp \
    vmtdebuger.cpp \
    VMTLine.cpp \
    vmtproject.cpp \
    VMTSerializer.cpp \
    VMTTransitionConditions.cpp \
    VMTTransitionImpl.cpp \
    formcalculator.cpp \
    vmterror.cpp \
    vmterrormachine.cpp \
    vmterrortransition.cpp \
    vmtundomanager.cpp \
    vmtundoelement.cpp \
    vmtundoelementmachine.cpp \
    vmtundoelementeditor.cpp \
    clickablelabel.cpp \
    screentools.cpp \
    formcomplexmachines.cpp \
    formactionhint.cpp \
    formexport4th.cpp \
    vmtexport4th/vmtexport4th.cpp \
    vmtmachines/vmtmachinerightword.cpp \
    vmtmachines/VMTMachineStub.cpp \
    vmtmachines/VMTMachineAlpha.cpp \
    vmtmachines/VMTComplexMachine.cpp \
    vmtmachines/vmtmachinecopy.cpp \
    vmtmachines/vmtmachineleft.cpp \
    vmtmachines/vmtmachineleftword.cpp \
    vmtmachines/vmtmachineright.cpp \
    vmtmachines/vmtmachinestart.cpp \
    vmtmachines/vmtmachinefinish.cpp \
    vmtmachines/VMTComplexMachineInner.cpp \
    vmtactions/vmtactionpointer.cpp \
    vmtactions/vmtactionmachine.cpp \
    vmtactions/vmtactionlink.cpp \
    vmtactions/vmtactiondelete.cpp \
    vmtactions/vmtactioncreateandconnect.cpp \
    vmtactions/vmtactiontranslate.cpp \
    formexercises.cpp \
    exercise.cpp

HEADERS  += \
    configuration.h \
    formbase.h \
    formeditormachine.h \
    formeditorwidget.h \
    formmain.h \
    formnewproject.h \
    mainwindow.h \
    statecontroller.h \
    uistate.h \
    uistateeditormachine.h \
    uistatemain.h \
    uistatenewproject.h \
    uistatestub.h \
    formdebugermachine.h \
    uistatedebugermachine.h \
    formcompilermachine.h \
    uistatecompilermachine.h \
    formlinewidget.h \
    formwarning.h \
    formalphabit.h \
    flowlayout.h \
    formnewmachine.h \
    uistatenewmachine.h \
    machineslibrary.h \
    machineslibrary_global.h \
    uicanvas.h \
    VMTAlphabit.h \
    vmtdebuger.h \
    VMTLine.h \
    vmtproject.h \
    VMTSerializer.h \
    VMTTransitionConditions.h \
    VMTTransitionImpl.h \
    formcalculator.h \
    vmterror.h \
    vmterrormachine.h \
    vmterrortransition.h \
    vmtundomanager.h \
    vmtundoelement.h \
    vmtundoelementmachine.h \
    vmtundoelementeditor.h \
    clickablelabel.h \
    screentools.h \
    formcomplexmachines.h \
    formactionhint.h \
    formexport4th.h \
    uistateexport4th.h \
    vmtexport4th/vmtexport4th.h \
    vmtmachines/VMTMachineStub.h \
    vmtmachines/VMTComplexMachine.h \
    vmtmachines/VMTMachineAlpha.h \
    vmtmachines/vmtmachinecopy.h \
    vmtmachines/vmtmachinefinish.h \
    vmtmachines/vmtmachineleft.h \
    vmtmachines/vmtmachineleftword.h \
    vmtmachines/vmtmachineright.h \
    vmtmachines/vmtmachinerightword.h \
    vmtmachines/vmtmachinestart.h \
    vmtmachines/VMTComplexMachineInner.h \
    interfaces/IVMTMachine.h \
    interfaces/IVMTEnvironment.h \
    interfaces/IVMTAlphabitSource.h \
    interfaces/ivmtundoobserver.h \
    interfaces/IVMTAction.h \
    interfaces/IVMTActionController.h \
    interfaces/IVMTTransition.h \
    interfaces/IVMDebugEnabler.h \
    interfaces/IVMTCalculatorCallback.h \
    interfaces/IVMTCompilerCallback.h \
    interfaces/IVMTComplexMachineSource.h \
    interfaces/ivmtdebuger.h \
    interfaces/IVMTDebugerCallback.h \
    interfaces/IVMTLineObserver.h \
    interfaces/IVMTProjectMachineReplacer.h \
    vmtactions/vmtactioncreateandconnect.h \
    vmtactions/vmtactionlink.h \
    vmtactions/vmtactionmachine.h \
    vmtactions/vmtactionpointer.h \
    vmtactions/vmtactiondelete.h \
    vmtactions/vmtactiontranslate.h \
    formexercises.h \
    uistateexercises.h \
    exercise.h

CONFIG += c++17

QMAKE_MAC_SDK = macosx

QMAKE_INFO_PLIST = MyAppInfo.plist

RESOURCES += \
    resources.qrc


CONFIG += appstore

MOBILITY =


FORMS += \
    formeditormachine.ui \
    formeditorwidget.ui \
    formnewproject.ui \
    mainwindow.ui \
    formmain.ui \
    formdebugermachine.ui \
    formcompilermachine.ui \
    formlinewidget.ui \
    formwarning.ui \
    formalphabit.ui \
    formnewmachine.ui \
    formcalculator.ui \
    formcomplexmachines.ui \
    formactionhint.ui \
    formexport4th.ui \
    formexercises.ui

DISTFILES += \
    android-sources/AndroidManifest.xml \
    images/app_low.png \
    images/app_medium.png \
    MyAppInfo.plist \
    Resources/app.icns

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources
ANDROID_PERMISSIONS=
