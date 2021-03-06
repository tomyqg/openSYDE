TARGET = osy_syde_coder_c
TEMPLATE = app
CONFIG += console

#no Qt stuff at all (pure ANSI-C++)
QT -= gui
QT -= core

#add windows API libraries
LIBS += -lversion

#link all libraries statically
QMAKE_LFLAGS += -static-libstdc++
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static

#version info:
RC_FILE = ../src/resources.rc

DESTDIR = ../result

SOURCES += \
    ../libs/osy_core/C_OSCChecksummedIniFile.cpp \
    ../libs/osy_core/C_OSCUtils.cpp \
    ../libs/osy_core/exports/C_OSCExportCommunicationStack.cpp \
    ../libs/osy_core/exports/C_OSCExportDataPool.cpp \
    ../libs/osy_core/exports/C_OSCExportHalc.cpp \
    ../libs/osy_core/exports/C_OSCExportNode.cpp \
    ../libs/osy_core/exports/C_OSCExportOsyInit.cpp \
    ../libs/osy_core/exports/C_OSCExportUti.cpp \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfig.cpp \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigChannel.cpp \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigDomain.cpp \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigFiler.cpp \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigParameter.cpp \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigParameterStruct.cpp \
    ../libs/osy_core/halc/definition/base/C_OSCHalcDefBase.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDef.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefChannelAvailability.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefChannelUseCase.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefContent.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefContentBitmaskItem.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefDomain.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefElement.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefFiler.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefStruct.cpp \
    ../libs/osy_core/halc/definition/C_OSCHalcDefStructFiler.cpp \
    ../libs/osy_core/halc/magician/C_OSCHALCMagicianUtil.cpp \
    ../libs/osy_core/kefex_diaglib/CSCLResourceStrings.cpp \
    ../libs/osy_core/kefex_diaglib/DLStrings.cpp \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLFile.cpp \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLTasks.cpp \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLTime.cpp \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLUtils.cpp \
    ../libs/osy_core/logging/C_OSCLoggingHandler.cpp \
    ../libs/osy_core/project/C_OSCProject.cpp \
    ../libs/osy_core/project/C_OSCProjectFiler.cpp \
    ../libs/osy_core/project/system/node/can/C_OSCCanMessage.cpp \
    ../libs/osy_core/project/system/node/can/C_OSCCanMessageContainer.cpp \
    ../libs/osy_core/project/system/node/can/C_OSCCanMessageIdentificationIndices.cpp \
    ../libs/osy_core/project/system/node/can/C_OSCCanProtocol.cpp \
    ../libs/osy_core/project/system/node/can/C_OSCCanSignal.cpp \
    ../libs/osy_core/project/system/node/C_OSCNode.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeApplication.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeComInterfaceSettings.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPool.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolContent.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolDataSet.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolFiler.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolList.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolListElement.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolListElementId.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolListId.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeFiler.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeOpenSydeServerSettings.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeProperties.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeStwFlashloaderSettings.cpp \
    ../libs/osy_core/project/system/C_OSCDeviceDefinitionFiler.cpp \
    ../libs/osy_core/project/system/C_OSCDeviceDefinition.cpp \
    ../libs/osy_core/project/system/C_OSCDeviceGroup.cpp \
    ../libs/osy_core/project/system/C_OSCDeviceManager.cpp \
    ../libs/osy_core/project/system/C_OSCSystemBus.cpp \
    ../libs/osy_core/project/system/C_OSCSystemBusFiler.cpp \
    ../libs/osy_core/project/system/C_OSCSystemDefinition.cpp \
    ../libs/osy_core/project/system/C_OSCSystemDefinitionFiler.cpp \
    ../libs/osy_core/project/system/C_OSCSystemFilerUtil.cpp \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCNodeDataPoolFilerV2.cpp \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCNodeFilerV2.cpp \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCSystemBusFilerV2.cpp \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCSystemDefinitionFilerV2.cpp \
    ../libs/osy_core/project/system/node/can/C_OSCCanUtil.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeCommFiler.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolId.cpp \
    ../libs/osy_core/project/system/C_OSCTargetSupportPackage.cpp \
    ../libs/osy_core/project/system/C_OSCTargetSupportPackageFiler.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolContentUtil.cpp \
    ../libs/osy_core/scl/CSCLChecksums.cpp \
    ../libs/osy_core/scl/CSCLDateTime.cpp \
    ../libs/osy_core/scl/CSCLIniFile.cpp \
    ../libs/osy_core/scl/CSCLString.cpp \
    ../libs/osy_core/scl/CSCLStringList.cpp \
    ../libs/osy_core/xml_parser/tinyxml2/tinyxml2.cpp \
    ../libs/osy_core/xml_parser/C_OSCChecksummedXML.cpp \
    ../libs/osy_core/xml_parser/C_OSCXMLParser.cpp \
    ../src/C_CExport.cpp \
    ../src/osy_code_exporter/C_OsyCodeExportBase.cpp \
    ../libs/osy_core/project/system/node/C_OSCNodeCodeExportSettings.cpp

PRECOMPILED_HEADER = ../src/precomp_headers.h

HEADERS += \
    ../src/precomp_headers.h \
    ../libs/osy_core/stwtypes/stwtypes.h \
    ../libs/osy_core/stwerrors.h \
    ../libs/osy_core/C_OSCChecksummedIniFile.h \
    ../libs/osy_core/C_OSCUtils.h \
    ../libs/osy_core/exports/C_OSCExportCommunicationStack.h \
    ../libs/osy_core/exports/C_OSCExportDataPool.h \
    ../libs/osy_core/exports/C_OSCExportHalc.h \
    ../libs/osy_core/exports/C_OSCExportNode.h \
    ../libs/osy_core/exports/C_OSCExportOsyInit.h \
    ../libs/osy_core/exports/C_OSCExportUti.h \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfig.h \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigChannel.h \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigDomain.h \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigFiler.h \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigParameter.h \
    ../libs/osy_core/halc/configuration/C_OSCHalcConfigParameterStruct.h \
    ../libs/osy_core/halc/definition/base/C_OSCHalcDefBase.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDef.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefChannelAvailability.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefChannelUseCase.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefContent.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefContentBitmaskItem.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefDomain.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefElement.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefFiler.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefStruct.h \
    ../libs/osy_core/halc/definition/C_OSCHalcDefStructFiler.h \
    ../libs/osy_core/halc/magician/C_OSCHALCMagicianUtil.h \
    ../libs/osy_core/kefex_diaglib/CSCLResourceStrings.h \
    ../libs/osy_core/kefex_diaglib/DLStrings.h \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLFile.h \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLTasks.h \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLTime.h \
    ../libs/osy_core/kefex_diaglib/tgl_windows/TGLUtils.h \
    ../libs/osy_core/logging/C_OSCLoggingHandler.h \
    ../libs/osy_core/project/C_OSCProject.h \
    ../libs/osy_core/project/C_OSCProjectFiler.h \
    ../libs/osy_core/project/system/node/can/C_OSCCanMessage.h \
    ../libs/osy_core/project/system/node/can/C_OSCCanMessageContainer.h \
    ../libs/osy_core/project/system/node/can/C_OSCCanMessageIdentificationIndices.h \
    ../libs/osy_core/project/system/node/can/C_OSCCanProtocol.h \
    ../libs/osy_core/project/system/node/can/C_OSCCanSignal.h \
    ../libs/osy_core/project/system/node/C_OSCNode.h \
    ../libs/osy_core/project/system/node/C_OSCNodeApplication.h \
    ../libs/osy_core/project/system/node/C_OSCNodeComInterfaceSettings.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPool.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolContent.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolDataSet.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolFiler.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolList.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolListElement.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolListElementId.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolListId.h \
    ../libs/osy_core/project/system/node/C_OSCNodeFiler.h \
    ../libs/osy_core/project/system/node/C_OSCNodeOpenSydeServerSettings.h \
    ../libs/osy_core/project/system/node/C_OSCNodeProperties.h \
    ../libs/osy_core/project/system/node/C_OSCNodeStwFlashloaderSettings.h \
    ../libs/osy_core/project/system/C_OSCDeviceDefinition.h \
    ../libs/osy_core/project/system/C_OSCDeviceDefinitionFiler.h \
    ../libs/osy_core/project/system/C_OSCDeviceGroup.h \
    ../libs/osy_core/project/system/C_OSCDeviceManager.h \
    ../libs/osy_core/project/system/C_OSCSystemBus.h \
    ../libs/osy_core/project/system/C_OSCSystemBusFiler.h \
    ../libs/osy_core/project/system/C_OSCSystemDefinition.h \
    ../libs/osy_core/project/system/C_OSCSystemDefinitionFiler.h \
    ../libs/osy_core/project/system/C_OSCSystemFilerUtil.h \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCNodeDataPoolFilerV2.h \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCNodeFilerV2.h \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCSystemBusFilerV2.h \
    ../libs/osy_core/project/system/FileLoadersV2/C_OSCSystemDefinitionFilerV2.h \
    ../libs/osy_core/project/system/node/can/C_OSCCanUtil.h \
    ../libs/osy_core/project/system/node/C_OSCNodeCommFiler.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolId.h \
    ../libs/osy_core/project/system/C_OSCTargetSupportPackage.h \
    ../libs/osy_core/project/system/C_OSCTargetSupportPackageFiler.h \
    ../libs/osy_core/project/system/node/C_OSCNodeDataPoolContentUtil.h \
    ../libs/osy_core/scl/CSCLChecksums.h \
    ../libs/osy_core/scl/CSCLDateTime.h \
    ../libs/osy_core/scl/CSCLIniFile.h \
    ../libs/osy_core/scl/CSCLString.h \
    ../libs/osy_core/scl/CSCLStringList.h \
    ../libs/osy_core/xml_parser/tinyxml2/tinyxml2.h \
    ../libs/osy_core/xml_parser/C_OSCChecksummedXML.h \
    ../libs/osy_core/xml_parser/C_OSCXMLParser.h \
    ../src/osy_code_exporter/C_OsyCodeExportBase.h \
    ../libs/osy_core/project/system/node/C_OSCNodeCodeExportSettings.h

#regular (non-library) header paths (also lists the system ones so QtCreator parser sees them)
INCLUDEPATH += ../src \
               ../src/osy_code_exporter \
               ../libs/osy_core \
               ../libs/osy_core/data_dealer \
               ../libs/osy_core/data_dealer/paramset \
               ../libs/osy_core/exports \
               ../libs/osy_core/halc \
               ../libs/osy_core/halc/configuration \
               ../libs/osy_core/halc/definition \
               ../libs/osy_core/halc/definition/base \
               ../libs/osy_core/halc/magician \
               ../libs/osy_core/logging \
               ../libs/osy_core/project \
               ../libs/osy_core/project/system \
               ../libs/osy_core/project/system/node \
               ../libs/osy_core/project/system/node/can \
               ../libs/osy_core/project/system/FileLoadersV2 \
               ../libs/osy_core/scl \
               ../libs/osy_core/stwtypes\
               ../libs/osy_core/kefex_diaglib \
               ../libs/osy_core/kefex_diaglib/tgl_windows \
               ../libs/osy_core/protocol_drivers \
               ../libs/osy_core/xml_parser \
               ../libs/osy_core/xml_parser/tinyxml2
