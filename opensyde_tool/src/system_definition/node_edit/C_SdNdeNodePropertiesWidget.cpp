//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Properties dialog for node properties

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QCheckBox>
#include <QSpinBox>
#include <QFileInfo>

#include "C_Uti.h"
#include "C_PuiSdUtil.h"
#include "stwerrors.h"
#include "C_SdNdeStwFlashloaderOptions.h"
#include "C_SdNdeNodePropertiesWidget.h"
#include "ui_C_SdNdeNodePropertiesWidget.h"
#include "C_GtGetText.h"
#include "C_OSCUtils.h"
#include "C_SdNdeComIfSettingsTableDelegate.h"
#include "C_PuiSdHandler.h"
#include "TGLUtils.h"
#include "C_SdUtil.h"
#include "C_OSCNodeProperties.h"
#include "C_OgeChxTristateTransparentError.h"
#include "constants.h"
#include "C_OgeWiUtil.h"
#include "C_OgeChxTristate.h"
#include "C_OSCNodeComInterfaceSettings.h"
#include "C_SdNdeIpAddressConfigurationWidget.h"
#include "C_OgeLabNodePropComIfTable.h"
#include "C_OgeWiCustomMessage.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */

using namespace stw_errors;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;
using namespace stw_types;
using namespace stw_scl;
using namespace stw_tgl;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */
const uint16 mu16_NODE_IMG_WIDTH = 300;

const uint8 mu8_FL_INDEX_OS = 0;
const uint8 mu8_FL_INDEX_STW = 1;
const uint8 mu8_FL_INDEX_NOSUPPORT = 2;

const stw_types::sint32 C_SdNdeNodePropertiesWidget::mhs32_PR_INDEX_DISABLED = 0;
const stw_types::sint32 C_SdNdeNodePropertiesWidget::mhs32_PR_INDEX_ENABLED = 1;

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent  Reference to parent
   \param[in]     orc_Name    Name of the bus for the title
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeNodePropertiesWidget::C_SdNdeNodePropertiesWidget(QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SdNdeNodePropertiesWidget),
   mu32_NodeIndex(0)
{
   QPixmap c_ImgLogo;

   // init UI
   mpc_Ui->setupUi(this);

   //SSI:
   //hide technical spec group
   // - Communication IF section is more important (4cs-gw: 11 IFs)
   // Where to move the info?
   // NVM Size: Datapool properties (done)
   // Flash access? Is a flashloader thing, but not important to show in a extra line (TBD)
   mpc_Ui->pc_GroupBoxTechnicalSpecHeader->setVisible(false);
   mpc_Ui->pc_FramSperator1_5->setVisible(false);
   mpc_Ui->pc_GroupTechnicalSpec->setVisible(false);

   //load STW logo
   c_ImgLogo.load("://images/STW_Logo_Dark.png");
   c_ImgLogo = c_ImgLogo.scaled((c_ImgLogo.width() / 22), (c_ImgLogo.height() / 22),
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);
   this->mpc_Ui->pc_StwLogo->setPixmap(c_ImgLogo);

   //Options buttons
   this->mpc_Ui->pc_PushButtonFlashloaderOptions->SetCustomIcon("://images/SettingsIcon.svg",
                                                                "://images/SettingsIconDisabled.svg");
   this->mpc_Ui->pc_PushButtonFlashloaderOptions->setIconSize(mc_ICON_SIZE_24);

   //set explicit min width to allow the layout to cut off parts of the button (necessary to free space for smaller
   // screen resolutions)
   this->mpc_Ui->pc_PushButtonFlashloaderOptions->setMinimumWidth(52);

   InitStaticNames();

   //table setups
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(static_cast<sintn> (
                                                                      C_SdNdeComIfSettingsTableDelegate::eINTERFACE))->
   setTextAlignment(static_cast<sintn> (Qt::AlignLeft));
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(static_cast<sintn> (
                                                                      C_SdNdeComIfSettingsTableDelegate::eCONNECTION))->
   setTextAlignment(static_cast<sintn> (Qt::AlignLeft));

   //set min column width (necessary for "Linked to..." strech column
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setMinimumSectionSize(75);

   //setup column size mode and size
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setSectionResizeMode(static_cast<sintn> (
                                                                                          C_SdNdeComIfSettingsTableDelegate
                                                                                          ::eINTERFACE),
                                                                                       QHeaderView::Fixed);

   this->mpc_Ui->pc_TableWidgetComIfSettings->setColumnWidth(static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::
                                                                                 eINTERFACE), 130);

   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setSectionResizeMode(static_cast<sintn> (
                                                                                          C_SdNdeComIfSettingsTableDelegate
                                                                                          ::eCONNECTION),
                                                                                       QHeaderView::Stretch);

   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setSectionResizeMode(static_cast<sintn> (
                                                                                          C_SdNdeComIfSettingsTableDelegate
                                                                                          ::eNODEID),
                                                                                       QHeaderView::Fixed);
   this->mpc_Ui->pc_TableWidgetComIfSettings->setColumnWidth(static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::
                                                                                 eNODEID), 130);

   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setSectionResizeMode(static_cast<sintn> (
                                                                                          C_SdNdeComIfSettingsTableDelegate
                                                                                          ::eIPADDRESS),
                                                                                       QHeaderView::Fixed);
   this->mpc_Ui->pc_TableWidgetComIfSettings->setColumnWidth(static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::
                                                                                 eIPADDRESS), 130);

   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setSectionResizeMode(static_cast<sintn> (
                                                                                          C_SdNdeComIfSettingsTableDelegate
                                                                                          ::eUPDATE),
                                                                                       QHeaderView::Fixed);
   this->mpc_Ui->pc_TableWidgetComIfSettings->setColumnWidth(static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::
                                                                                 eUPDATE), 130);

   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setSectionResizeMode(static_cast<sintn> (
                                                                                          C_SdNdeComIfSettingsTableDelegate
                                                                                          ::eROUTING),
                                                                                       QHeaderView::Fixed);
   this->mpc_Ui->pc_TableWidgetComIfSettings->setColumnWidth(static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::
                                                                                 eROUTING), 130);

   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->setSectionResizeMode(static_cast<sintn> (
                                                                                          C_SdNdeComIfSettingsTableDelegate
                                                                                          ::eDIAGNOSTIC),
                                                                                       QHeaderView::Fixed);
   this->mpc_Ui->pc_TableWidgetComIfSettings->setColumnWidth(static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::
                                                                                 eDIAGNOSTIC), 130);

   //Name restriction
   this->mpc_Ui->pc_LineEditNodeName->setMaxLength(msn_C_ITEM_MAX_CHAR_COUNT);

   // connects
   connect(this->mpc_Ui->pc_LineEditNodeName, &QLineEdit::textChanged, this,
           &C_SdNdeNodePropertiesWidget::m_CheckNodeName);
   connect(this->mpc_Ui->pc_LineEditNodeName, &QLineEdit::editingFinished, this,
           &C_SdNdeNodePropertiesWidget::m_TrimNodeName);
   connect(this->mpc_Ui->pc_TableWidgetComIfSettings, &QTableWidget::cellChanged, this,
           &C_SdNdeNodePropertiesWidget::m_CheckComIfId);
   connect(this->mpc_Ui->pc_TableWidgetComIfSettings, &QTableWidget::cellClicked, this,
           &C_SdNdeNodePropertiesWidget::m_IpAddressClick);
   connect(this->mpc_Ui->pc_PushButtonFlashloaderOptions, &C_OgePubOptions::clicked, this,
           &C_SdNdeNodePropertiesWidget::m_FlashloaderOptions);
   //lint -e{929} Cast required to avoid ambiguous signal of qt interface
   connect(this->mpc_Ui->pc_ComboBoxProtocol,
           static_cast<void (QComboBox::*)(sintn)>(&QComboBox::currentIndexChanged), this,
           &C_SdNdeNodePropertiesWidget::m_SupportedProtocolChange);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeNodePropertiesWidget::~C_SdNdeNodePropertiesWidget(void)
{
   delete mpc_Ui;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize all displayed static names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::InitStaticNames(void) const
{
   QString c_InterfaceString;
   QString c_ConnectString;

   const sintn sn_ColInterface = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eINTERFACE);
   const sintn sn_ColConnection = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eCONNECTION);
   const sintn sn_ColNodeId = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eNODEID);
   const sintn sn_ColIpAddress = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eIPADDRESS);
   const sintn sn_ColUpdate = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eUPDATE);
   const sintn sn_ColRouting = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eROUTING);
   const sintn sn_ColDiagnostic = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eDIAGNOSTIC);

   this->mpc_Ui->pc_LabelName->setText(C_GtGetText::h_GetText("Name"));
   this->mpc_Ui->pc_LabelComment->setText(C_GtGetText::h_GetText("Comment"));
   this->mpc_Ui->pc_LabelConfiguration->setText(C_GtGetText::h_GetText("Configuration"));
   this->mpc_Ui->pc_LabelProtocol->setText(C_GtGetText::h_GetText("Protocol Support"));
   this->mpc_Ui->pc_LabelProgramming->setText(C_GtGetText::h_GetText("Programming Support"));
   this->mpc_Ui->pc_LabelComIfSettings->setText(C_GtGetText::h_GetText("Communication Interfaces Settings"));

   this->mpc_Ui->pc_ComboBoxProgramming->addItem(C_GtGetText::h_GetText("Disabled"));
   this->mpc_Ui->pc_ComboBoxProgramming->addItem(C_GtGetText::h_GetText("Enabled"));

   //Add space because of spacing issue between icon and text
   this->mpc_Ui->pc_PushButtonFlashloaderOptions->setText(QString(' ') +
                                                          C_GtGetText::h_GetText("STW Flashloader Settings"));

   this->mpc_Ui->pc_LabelFlashAccess->setText(C_GtGetText::h_GetText("Flash Access"));

   //table column text
   //fake padding with Spaces. No other solution known so far
   c_InterfaceString = "          ";
   c_InterfaceString.append(C_GtGetText::h_GetText("Interface"));
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(sn_ColInterface)->setText(c_InterfaceString);

   //fake padding with Spaces. No other solution known so far
   c_ConnectString = "  ";
   c_ConnectString.append(C_GtGetText::h_GetText("Linked to..."));
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(sn_ColConnection)->setText(c_ConnectString);
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(sn_ColNodeId)->setText(C_GtGetText::h_GetText(
                                                                                             "Node ID"));
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(sn_ColIpAddress)->setText(C_GtGetText::h_GetText(
                                                                                                "IP Address"));
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(sn_ColUpdate)->setText(C_GtGetText::h_GetText(
                                                                                             "Update"));
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(sn_ColRouting)->setText(C_GtGetText::h_GetText(
                                                                                              "Routing"));
   this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeaderItem(sn_ColDiagnostic)->setText(C_GtGetText::h_GetText(
                                                                                                 "Diagnostic"));

   this->mpc_Ui->pc_TextEditComment->setPlaceholderText(C_GtGetText::h_GetText("Add your comment here ..."));

   //Tool tips
   this->mpc_Ui->pc_LabelName->SetToolTipInformation(C_GtGetText::h_GetText("Name"),
                                                     C_GtGetText::h_GetText(
                                                        "Symbolic node name. Unique within Network Topology.\n"
                                                        "\nFollowing C naming conventions are required:"
                                                        "\n - must not be empty"
                                                        "\n - only alphanumeric characters + \"_\""
                                                        "\n - should not be longer than 31 characters"));

   this->mpc_Ui->pc_LabelComment->SetToolTipInformation(C_GtGetText::h_GetText("Comment"),
                                                        C_GtGetText::h_GetText("Comment for this node."));

   this->mpc_Ui->pc_LabelProgramming->SetToolTipInformation(C_GtGetText::h_GetText("Programming Support"),
                                                            C_GtGetText::h_GetText(
                                                               "This property shows if the device is user programmable."
                                                               "\nDefined in read only *.syde_devdef file."
                                                               "\n\nIf enabled, Data Blocks of type \"Programmable Application\" can be created."));

   this->mpc_Ui->pc_LabelProtocol->SetToolTipInformation(C_GtGetText::h_GetText("Protocol Support"),
                                                         C_GtGetText::h_GetText(
                                                            "Type of flashloader and diagnostic server.\n"
                                                            "Options:\n"
                                                            "   - openSYDE: openSYDE Server and openSYDE Flashloader support\n"
                                                            "   - KEFEX: KEFEX server and STW Flashloader support\n"
                                                            "   - none: no STW protocol support (e.g.: 3rd party node)\n"
                                                            "\nSupported protocols defined in read only "
                                                            "*.syde_devdef file."));

   this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipHeadingAt(sn_ColInterface, Qt::Horizontal,
                                                                  C_GtGetText::h_GetText("Interface"),
                                                                  C_GtGetText::h_GetText(
                                                                     "Name of communication interface (CAN/ETHERNET)."));

   this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipHeadingAt(sn_ColConnection, Qt::Horizontal,
                                                                  C_GtGetText::h_GetText("Linked to..."),
                                                                  C_GtGetText::h_GetText(
                                                                     "Name of bus to which the interface is linked to."));

   this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipHeadingAt(sn_ColNodeId, Qt::Horizontal,
                                                                  C_GtGetText::h_GetText("Node ID"),
                                                                  C_GtGetText::h_GetText(
                                                                     "Node ID is unique on connected bus. The ID is "
                                                                     "used for addressing in the communication "
                                                                     "protocol. \nThis property is configured for all "
                                                                     "connected interfaces on device while \"Device "
                                                                     "configuration\" (SYSTEM COMMISSIONING/Setup)."));

   this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipHeadingAt(sn_ColIpAddress, Qt::Horizontal,
                                                                  C_GtGetText::h_GetText("IP Address"),
                                                                  C_GtGetText::h_GetText(
                                                                     "IP address settings: IP address and subnet mask"
                                                                     "\nThese properties are configured for all "
                                                                     "connected interfaces on device while \"Device "
                                                                     "configuration\" (SYSTEM COMMISSIONING/Setup)"));

   this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipHeadingAt(sn_ColUpdate, Qt::Horizontal,
                                                                  C_GtGetText::h_GetText("Update"),
                                                                  C_GtGetText::h_GetText(
                                                                     "Is the openSYDE tool legitimated to use this "
                                                                     "interface for System Update?"
                                                                     "\nUse case: System Update. This property is just "
                                                                     "a configuration for openSYDE tool, "
                                                                     "\nit is NOT configured on device."));

   this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipHeadingAt(sn_ColDiagnostic, Qt::Horizontal,
                                                                  C_GtGetText::h_GetText("Diagnostic"),
                                                                  C_GtGetText::h_GetText(
                                                                     "Is the openSYDE tool legitimated to use this "
                                                                     "interface for diagnostic requests?"
                                                                     "\nUse case: Dashboards. This property is just a "
                                                                     "configuration for openSYDE tool, "
                                                                     "\nit is NOT configured on device."));

   this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipHeadingAt(sn_ColRouting, Qt::Horizontal,
                                                                  C_GtGetText::h_GetText("Routing"),
                                                                  C_GtGetText::h_GetText(
                                                                     "Is the openSYDE tool legitimated to use this "
                                                                     "interface for diagnostic protocol routing? "
                                                                     "\nAttention: This property is intended as additive "
                                                                     "feature in addition to \"Update\" resp. \"Diagnostic\" "
                                                                     "\nUse cases: System Update and Dashboards. This "
                                                                     "property is just a configuration for openSYDE tool, "
                                                                     "\nit is NOT configured on device."));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Selects the node name in the text edit for fast editing
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::SelectName(void) const
{
   this->mpc_Ui->pc_LineEditNodeName->setFocus();
   this->mpc_Ui->pc_LineEditNodeName->selectAll();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Node ID setter

   Sets the private node id of widget

   \param[in]     ou32_NodeIndex         new node id
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::SetNodeId(const uint32 ou32_NodeIndex)
{
   this->mu32_NodeIndex = ou32_NodeIndex;

   //load node data
   this->m_LoadFromData();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Load node information

   Load node information from core data using node index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_LoadFromData(void)
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   //Disconnects for RegisterChange
   disconnect(this->mpc_Ui->pc_LineEditNodeName, &QLineEdit::editingFinished, this,
              &C_SdNdeNodePropertiesWidget::m_RegisterNameChange);
   disconnect(this->mpc_Ui->pc_TextEditComment, &QTextEdit::textChanged, this,
              &C_SdNdeNodePropertiesWidget::m_RegisterChange);
   disconnect(this->mpc_Ui->pc_TableWidgetComIfSettings, &QTableWidget::cellChanged, this,
              &C_SdNdeNodePropertiesWidget::m_RegisterChange);
   disconnect(this->mpc_Ui->pc_TableWidgetComIfSettings, &QTableWidget::cellChanged, this,
              &C_SdNdeNodePropertiesWidget::m_CheckComIfId);
   //lint -e{929} Cast required to avoid ambiguous signal of qt interface
   disconnect(this->mpc_Ui->pc_ComboBoxProtocol,
              static_cast<void (QComboBox::*)(sintn)>(&QComboBox::currentIndexChanged), this,
              &C_SdNdeNodePropertiesWidget::m_SupportedProtocolChange);

   tgl_assert(pc_Node != NULL);
   if (pc_Node != NULL)
   {
      QPixmap c_ImgNode;
      QString c_ComIfName;
      QString c_BusName;
      const C_OSCDeviceDefinition * const pc_DevDef = pc_Node->pc_DeviceDefinition;
      tgl_assert(pc_DevDef != NULL);

      //create delegate
      this->mpc_Ui->pc_TableWidgetComIfSettings->setItemDelegate(new C_SdNdeComIfSettingsTableDelegate(this,
                                                                                                       this->
                                                                                                       mu32_NodeIndex));

      if (pc_DevDef != NULL)
      {
         QFileInfo c_FileInfo;
         bool q_FileExists;
         bool q_StwFlashloaderActive = false;

         //name
         this->mpc_Ui->pc_LineEditNodeName->setText(pc_Node->c_Properties.c_Name.c_str());

         //comment
         this->mpc_Ui->pc_TextEditComment->setText(pc_Node->c_Properties.c_Comment.c_str());

         //protocol
         if ((pc_DevDef->q_FlashloaderOpenSydeCan == true) ||
             (pc_DevDef->q_FlashloaderStwCan == true))
         {
            if (pc_Node->c_Properties.e_FlashLoader == C_OSCNodeProperties::eFL_OPEN_SYDE)
            {
               //open SYDE
               this->mpc_Ui->pc_ComboBoxProtocol->setCurrentIndex(mu8_FL_INDEX_OS);
            }
            else if (pc_Node->c_Properties.e_FlashLoader == C_OSCNodeProperties::eFL_STW)
            {
               //STW
               this->mpc_Ui->pc_ComboBoxProtocol->setCurrentIndex(mu8_FL_INDEX_STW);
               q_StwFlashloaderActive = true;
            }
            else
            {
               //having "none" as flashloader is supported by the device definition structures;
               // but not by the UI (yet)
               tgl_assert(false);
            }

            if ((pc_DevDef->q_FlashloaderOpenSydeCan == true) &&
                (pc_DevDef->q_FlashloaderStwCan == true))
            {
               // Hybrid node. Both variants are possible.
               this->mpc_Ui->pc_ComboBoxProtocol->setEnabled(true);
               // No support is not supported on node with protocol support
               this->mpc_Ui->pc_ComboBoxProtocol->removeItem(mu8_FL_INDEX_NOSUPPORT);
            }
         }
         else
         {
            //not supported
            this->mpc_Ui->pc_ComboBoxProtocol->setCurrentIndex(mu8_FL_INDEX_NOSUPPORT);
         }

         //activate STW flashloader (options)
         this->mpc_Ui->pc_PushButtonFlashloaderOptions->setVisible(q_StwFlashloaderActive);

         //programming
         if (pc_DevDef->q_ProgrammingSupport == true)
         {
            this->mpc_Ui->pc_ComboBoxProgramming->setCurrentIndex(C_SdNdeNodePropertiesWidget::mhs32_PR_INDEX_ENABLED);
         }
         else
         {
            this->mpc_Ui->pc_ComboBoxProgramming->setCurrentIndex(C_SdNdeNodePropertiesWidget::mhs32_PR_INDEX_DISABLED);
         }

         //STW device? Assumption: If Flashloader support, its an STW device
         if (pc_Node->IsAnyUpdateAvailable() == true)
         {
            //show STW logo
            this->mpc_Ui->pc_StwLogo->setVisible(true);
         }
         else
         {
            //hide STW logo
            this->mpc_Ui->pc_StwLogo->setVisible(false);
         }

         //load device picture
         c_FileInfo.setFile(pc_DevDef->c_ImagePath.c_str());
         q_FileExists = (c_FileInfo.exists() && c_FileInfo.isFile());

         //check if file exists
         if (q_FileExists == true)
         {
            c_ImgNode.load(pc_DevDef->c_ImagePath.c_str());
            c_ImgNode = c_ImgNode.scaled(mu16_NODE_IMG_WIDTH, c_ImgNode.height(), //second parameter is not relevant
                                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
            this->mpc_Ui->pc_LabelNoImageAvailable->setVisible(false);
            this->mpc_Ui->pc_DatapoolTypeImage->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
         }
         else
         {
            //no image available
            QIcon c_Icon("://images/system_definition/Image_Grey.svg");
            c_ImgNode = c_Icon.pixmap(QSize(mu16_NODE_IMG_WIDTH / 2, mu16_NODE_IMG_WIDTH / 2));
            this->mpc_Ui->pc_LabelNoImageAvailable->setVisible(true);
            this->mpc_Ui->pc_DatapoolTypeImage->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
         }

         this->mpc_Ui->pc_DatapoolTypeImage->setPixmap(c_ImgNode);

         //load device type name
         this->mpc_Ui->pc_LabelNodeType->setText(pc_DevDef->GetDisplayName().c_str());

         //load nvm size
         this->mpc_Ui->pc_LabelNvmSizeValue->setText(QString::number(
                                                        pc_DevDef->u32_UserEepromSizeBytes) + " Bytes (EEPROM)");

         //flash access
         if (pc_Node->IsAnyUpdateAvailable() == true)
         {
            if (pc_DevDef->q_FlashloaderOpenSydeIsFileBased == true)
            {
               this->mpc_Ui->pc_LabelFlashAccessValue->setText(C_GtGetText::h_GetText("File based"));
            }
            else
            {
               this->mpc_Ui->pc_LabelFlashAccessValue->setText(C_GtGetText::h_GetText("Address based"));
            }
         }
         else
         {
            this->mpc_Ui->pc_LabelFlashAccessValue->setText(C_GtGetText::h_GetText(
                                                               "No Flashloader support."));
         }

         //clear table
         this->mpc_Ui->pc_TableWidgetComIfSettings->setRowCount(0);

         //insert can+ethernet count of rows
         this->mpc_Ui->pc_TableWidgetComIfSettings->setRowCount(static_cast<sintn> (pc_DevDef->u8_NumCanBusses) +
                                                                static_cast<sintn> (pc_DevDef->u8_NumEthernetBusses));

         {
            //set new table max size (Header = 38 + row*40)
            const sintn sn_MaxHeight = 38 + (this->mpc_Ui->pc_TableWidgetComIfSettings->rowCount() * 40);
            //Minimum is slightly off the optimal value (3 rows are visible)
            const sintn sn_MinHeight = 38 + (std::min(3, this->mpc_Ui->pc_TableWidgetComIfSettings->rowCount()) * 40);
            this->mpc_Ui->pc_TableWidgetComIfSettings->setMinimumHeight(sn_MinHeight);
            this->mpc_Ui->pc_TableWidgetComIfSettings->setMaximumHeight(sn_MaxHeight);
         }

         //insert com ifs
         for (uint16 u16_ComIfCnt = 0;
              u16_ComIfCnt <
              (static_cast<sintn> (pc_DevDef->u8_NumCanBusses) + static_cast<sintn> (pc_DevDef->u8_NumEthernetBusses));
              ++u16_ComIfCnt)
         {
            const sintn sn_ColInterface = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eINTERFACE);
            const sintn sn_ColConnection = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eCONNECTION);
            const sintn sn_ColNodeId = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eNODEID);
            const sintn sn_ColIpAddress = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eIPADDRESS);
            const sintn sn_ColUpdate = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eUPDATE);
            const sintn sn_ColRouting = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eROUTING);
            const sintn sn_ColDiagnostic = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eDIAGNOSTIC);
            bool q_IsUpdateAvailable;
            bool q_IsRoutingAvailable;
            bool q_IsDiagnosisAvailable;
            if (u16_ComIfCnt < pc_DevDef->u8_NumCanBusses)
            {
               q_IsUpdateAvailable = pc_DevDef->IsUpdateAvailable(C_OSCSystemBus::eCAN);
               q_IsRoutingAvailable = pc_Node->IsRoutingAvailable(C_OSCSystemBus::eCAN);
               q_IsDiagnosisAvailable = pc_DevDef->IsDiagnosisAvailable(C_OSCSystemBus::eCAN);
            }
            else
            {
               q_IsUpdateAvailable = pc_DevDef->IsUpdateAvailable(C_OSCSystemBus::eETHERNET);
               q_IsRoutingAvailable = pc_Node->IsRoutingAvailable(C_OSCSystemBus::eETHERNET);
               q_IsDiagnosisAvailable = pc_DevDef->IsDiagnosisAvailable(C_OSCSystemBus::eETHERNET);
            }

            //SSI: 17.04.18
            //Feature drop: Disable not connected interfaces
            //Reason: Node IFs properties has to be defined also if the interface is not connected to a bus
            //E.g.: No eds import permitted for node ids = 0.
            //
            //To avoid logic changes and to keep the possibility for quick feature revert
            //the q_Connected flag is set to true per default.
            bool q_Connected = true;
            QString c_IpAddressString;

            /**********************************************************************************************************/
            //INTERFACE
            this->mpc_Ui->pc_TableWidgetComIfSettings->setCellWidget(
               u16_ComIfCnt, sn_ColInterface, new C_OgeChxTristateTransparentError(this));
            //disable
            this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt, sn_ColInterface)->setEnabled(false);

            //set name
            if (u16_ComIfCnt < pc_DevDef->u8_NumCanBusses)
            {
               //its a CAN interface
               c_ComIfName = C_PuiSdUtil::h_GetInterfaceName(C_OSCSystemBus::eCAN, static_cast<uint8>(u16_ComIfCnt));
            }
            else
            {
               //its an Ethernet interface
               c_ComIfName =
                  C_PuiSdUtil::h_GetInterfaceName(C_OSCSystemBus::eETHERNET,
                                                  static_cast<uint8>(u16_ComIfCnt -
                                                                     static_cast<sintn> (pc_DevDef->u8_NumCanBusses)));
            }
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            dynamic_cast<C_OgeChxTristateTransparentError *> (this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(
                                                                 u16_ComIfCnt, sn_ColInterface))->setText(c_ComIfName);

            /**********************************************************************************************************/
            //CONNECTED TO
            this->mpc_Ui->pc_TableWidgetComIfSettings->setCellWidget(u16_ComIfCnt, sn_ColConnection,
                                                                     new C_OgeLabNodePropComIfTable(this));

            //set bus name
            if (pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].q_IsBusConnected == true)
            {
               const C_OSCSystemBus * const pc_Bus = C_PuiSdHandler::h_GetInstance()->GetOSCBus(
                  pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].u32_BusIndex);
               //get bus name
               if (pc_Bus != NULL)
               {
                  c_BusName = pc_Bus->c_Name.c_str();

                  if (pc_Bus->e_Type == C_OSCSystemBus::eCAN)
                  {
                     // add the bitrate
                     c_BusName += " (" + QString::number(pc_Bus->u64_BitRate / 1000ULL) + " kBit/s)";
                  }

                  q_Connected = true;
               }
            }
            else
            {
               c_BusName = "-"; // "not connected";
            }

            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            dynamic_cast<QLabel *> (this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(
                                       u16_ComIfCnt, sn_ColConnection))->setText(c_BusName);
            this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt, sn_ColConnection)->setEnabled(
               q_Connected);

            /**********************************************************************************************************/
            //INTERFACE style
            //lint -e944 Kept for possibility to revert to previous implementation
            C_OgeWiUtil::h_ApplyStylesheetProperty(this->mpc_Ui->pc_TableWidgetComIfSettings
                                                   ->cellWidget(u16_ComIfCnt, sn_ColInterface), "Disabled",
                                                   !q_Connected);

            /**********************************************************************************************************/
            //NODE ID
            this->mpc_Ui->pc_TableWidgetComIfSettings->setItem(u16_ComIfCnt, sn_ColNodeId, new QTableWidgetItem(""));
            //align center
            this->mpc_Ui->pc_TableWidgetComIfSettings->item(u16_ComIfCnt, sn_ColNodeId)
            ->setTextAlignment(static_cast<sintn> (Qt::AlignCenter));
            //set node value
            this->mpc_Ui->pc_TableWidgetComIfSettings->item(u16_ComIfCnt, sn_ColNodeId)->
            setText(QString::number(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].u8_NodeID));

            //lint -e948 Kept for possibility to revert to previous implementation
            if (q_Connected == true)
            {
               this->mpc_Ui->pc_TableWidgetComIfSettings->item(u16_ComIfCnt, sn_ColNodeId)->setFlags(
                  Qt::ItemIsEnabled | Qt::ItemIsEditable);
            }
            else
            {
               this->mpc_Ui->pc_TableWidgetComIfSettings->item(u16_ComIfCnt, sn_ColNodeId)->setFlags(Qt::NoItemFlags);
            }

            /**********************************************************************************************************/
            //IP Address
            this->mpc_Ui->pc_TableWidgetComIfSettings->setCellWidget(u16_ComIfCnt, sn_ColIpAddress,
                                                                     new C_OgeLabNodePropComIfTable(this));

            //set IP Address
            if (u16_ComIfCnt >= static_cast<sintn> (pc_DevDef->u8_NumCanBusses))
            {
               c_IpAddressString.append(QString::number(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].c_Ip.
                                                        au8_IpAddress[0]));
               c_IpAddressString.append(".");
               c_IpAddressString.append(QString::number(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].c_Ip.
                                                        au8_IpAddress[1]));
               c_IpAddressString.append(".");
               c_IpAddressString.append(QString::number(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].c_Ip.
                                                        au8_IpAddress[2]));
               c_IpAddressString.append(".");
               c_IpAddressString.append(QString::number(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].c_Ip.
                                                        au8_IpAddress[3]));
            }
            else
            {
               c_IpAddressString = "-";
            }

            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            dynamic_cast<QLabel *> (this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(
                                       u16_ComIfCnt, sn_ColIpAddress))->setText(c_IpAddressString);
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            dynamic_cast<QLabel *> (this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(
                                       u16_ComIfCnt, sn_ColIpAddress))->setAlignment(Qt::AlignCenter);

            //apply enabled/disabled style
            //if can, disable per default
            if (u16_ComIfCnt < pc_DevDef->u8_NumCanBusses)
            {
               this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt,
                                                                     sn_ColIpAddress)->setEnabled(false);
            }
            else
            {
               this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt,
                                                                     sn_ColIpAddress)->setEnabled(q_Connected);
               //apply special style
               C_OgeWiUtil::h_ApplyStylesheetProperty(this->mpc_Ui->pc_TableWidgetComIfSettings
                                                      ->cellWidget(u16_ComIfCnt,
                                                                   sn_ColIpAddress), "Enabled_IP_Address_Cell", true);
            }

            //hide the ip address column for devices without Ethernet interfaces
            if (pc_DevDef->u8_NumEthernetBusses == 0)
            {
               this->mpc_Ui->pc_TableWidgetComIfSettings->horizontalHeader()->hideSection(static_cast<sintn> (
                                                                                             C_SdNdeComIfSettingsTableDelegate
                                                                                             ::eIPADDRESS));
            }

            /**********************************************************************************************************/
            // UPDATE
            this->mpc_Ui->pc_TableWidgetComIfSettings->setCellWidget(u16_ComIfCnt, sn_ColUpdate, new C_OgeChxTristate(
                                                                        this));
            //set node value
            if ((q_IsUpdateAvailable == true) && (q_Connected == true))
            {
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                 ->cellWidget(u16_ComIfCnt, sn_ColUpdate))
               ->setChecked(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].q_IsUpdateEnabled);
            }
            else
            {
               //disable
               this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt, sn_ColUpdate)->setEnabled(false);
            }
            //connect to RegisterChange
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            connect(dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                      ->cellWidget(u16_ComIfCnt,
                                                                   sn_ColUpdate)), &QCheckBox::stateChanged, this,
                    &C_SdNdeNodePropertiesWidget::m_RegisterChange);

            /**********************************************************************************************************/
            // ROUTING
            this->mpc_Ui->pc_TableWidgetComIfSettings->setCellWidget(
               u16_ComIfCnt, sn_ColRouting, new C_OgeChxTristate(
                  this));

            if ((q_IsRoutingAvailable == true) && (q_Connected == true))
            {
               //set node value
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                 ->cellWidget(u16_ComIfCnt, sn_ColRouting))
               ->setChecked(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].q_IsRoutingEnabled);
            }
            else
            {
               //disable
               this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt, sn_ColRouting)->
               setEnabled(false);
            }
            //connect to RegisterChange
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            connect(dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                      ->cellWidget(u16_ComIfCnt,
                                                                   sn_ColRouting)), &QCheckBox::stateChanged, this,
                    &C_SdNdeNodePropertiesWidget::m_RegisterChange);

            /**********************************************************************************************************/
            // DIAGNOSTIC
            this->mpc_Ui->pc_TableWidgetComIfSettings->setCellWidget(u16_ComIfCnt, sn_ColDiagnostic, new C_OgeChxTristate(
                                                                        this));

            if ((q_IsDiagnosisAvailable == true) && (q_Connected == true))
            {
               //set node value
               //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
               dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                 ->cellWidget(u16_ComIfCnt, sn_ColDiagnostic))
               ->setChecked(pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt].q_IsDiagnosisEnabled);
            }
            else
            {
               //disable
               this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt, sn_ColDiagnostic)->
               setEnabled(false);
            }

            //connect to RegisterChange
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            connect(dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                      ->cellWidget(u16_ComIfCnt,
                                                                   sn_ColDiagnostic)), &QCheckBox::stateChanged, this,
                    &C_SdNdeNodePropertiesWidget::m_RegisterChange);
         }
      }
   }

   // Initial check of the data. Row is irrelevant because all rows will be checked of the column.
   this->m_CheckComIfId(0U, static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eNODEID));

   //connects for RegisterChange
   connect(this->mpc_Ui->pc_LineEditNodeName, &QLineEdit::editingFinished, this,
           &C_SdNdeNodePropertiesWidget::m_RegisterNameChange);
   connect(this->mpc_Ui->pc_TextEditComment, &QTextEdit::textChanged, this,
           &C_SdNdeNodePropertiesWidget::m_RegisterChange);
   connect(this->mpc_Ui->pc_TableWidgetComIfSettings, &QTableWidget::cellChanged, this,
           &C_SdNdeNodePropertiesWidget::m_RegisterChange);
   connect(this->mpc_Ui->pc_TableWidgetComIfSettings, &QTableWidget::cellChanged, this,
           &C_SdNdeNodePropertiesWidget::m_CheckComIfId);
   //lint -e{929} Cast required to avoid ambiguous signal of qt interface
   connect(this->mpc_Ui->pc_ComboBoxProtocol,
           static_cast<void (QComboBox::*)(sintn)>(&QComboBox::currentIndexChanged), this,
           &C_SdNdeNodePropertiesWidget::m_SupportedProtocolChange);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Save ui data to node

   Is called from outside
      - on system definition save
      - on page change

   \return   false:
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::SaveToData(void) const
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   if (pc_Node != NULL)
   {
      const C_OSCDeviceDefinition * const pc_DevDef = pc_Node->pc_DeviceDefinition;
      tgl_assert(pc_DevDef != NULL);

      if (pc_DevDef != NULL)
      {
         QString c_Name;
         QString c_Comment;
         C_OSCNodeProperties::E_DiagnosticServerProtocol e_DiagnosticServer;
         C_OSCNodeProperties::E_FlashLoaderProtocol e_FlashLoader;
         std::vector<uint8> c_NodeIds;
         std::vector<bool> c_UpdateFlags;
         std::vector<bool> c_RoutingFlags;
         std::vector<bool> c_DiagnosisFlags;
         const sintn sn_ColNodeId = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eNODEID);
         const sintn sn_ColUpdate = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eUPDATE);
         const sintn sn_ColRouting = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eROUTING);
         const sintn sn_ColDiagnostic = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eDIAGNOSTIC);

         //save data

         //name
         //Only accept new name if not in conflict
         if (C_PuiSdHandler::h_GetInstance()->CheckNodeNameAvailable(
                this->mpc_Ui->pc_LineEditNodeName->text().toStdString().c_str(), &this->mu32_NodeIndex, NULL))
         {
            c_Name = this->mpc_Ui->pc_LineEditNodeName->text();
         }
         else
         {
            //Restore previous name
            c_Name = pc_Node->c_Properties.c_Name.c_str();
         }

         //comment
         c_Comment = this->mpc_Ui->pc_TextEditComment->toPlainText();

         switch (this->mpc_Ui->pc_ComboBoxProtocol->currentIndex())
         {
         case mu8_FL_INDEX_OS:
            e_FlashLoader = C_OSCNodeProperties::eFL_OPEN_SYDE;
            e_DiagnosticServer = C_OSCNodeProperties::eDS_OPEN_SYDE;
            break;
         case mu8_FL_INDEX_STW:
            e_FlashLoader = C_OSCNodeProperties::eFL_STW;
            e_DiagnosticServer = C_OSCNodeProperties::eDS_KEFEX;
            break;
         default:
            //Not supported
            e_FlashLoader = C_OSCNodeProperties::eFL_NONE;
            e_DiagnosticServer = C_OSCNodeProperties::eDS_NONE;
            break;
         }

         //com interface settings
         for (uint16 u16_ComIfCnt = 0;
              (u16_ComIfCnt < pc_Node->c_Properties.c_ComInterfaces.size()) &&
              (u16_ComIfCnt < this->mpc_Ui->pc_TableWidgetComIfSettings->rowCount());
              ++u16_ComIfCnt)
         {
            bool q_NewValue;
            const C_OSCNodeComInterfaceSettings & rc_CurInterface = pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt];
            const bool q_IsUpdateAvailable = pc_DevDef->IsUpdateAvailable(rc_CurInterface.e_InterfaceType);
            const bool q_IsRoutingAvailable = pc_Node->IsRoutingAvailable(rc_CurInterface.e_InterfaceType);
            const bool q_IsDiagnosisAvailable = pc_DevDef->IsDiagnosisAvailable(rc_CurInterface.e_InterfaceType);
            //node id
            c_NodeIds.push_back(static_cast<uint8> ((this->mpc_Ui->pc_TableWidgetComIfSettings->item(u16_ComIfCnt,
                                                                                                     sn_ColNodeId)->text()
                                                     .toInt())));

            //update
            if (q_IsUpdateAvailable == true)
            {
               //set state only if the cell is enabled (connected + available)
               if (this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt,
                                                                         sn_ColUpdate)->isEnabled() == true)
               {
                  //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                  q_NewValue =
                     dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                       ->cellWidget(u16_ComIfCnt, sn_ColUpdate))->isChecked();
               }
               else
               {
                  q_NewValue = false;
               }
            }
            else
            {
               q_NewValue = false;
            }
            c_UpdateFlags.push_back(q_NewValue);

            //routing
            if (q_IsRoutingAvailable == true)
            {
               //set state only if the cell is enabled (connected + available)
               if (this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt,
                                                                         sn_ColRouting)->isEnabled() == true)
               {
                  //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                  q_NewValue =
                     dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                       ->cellWidget(u16_ComIfCnt, sn_ColRouting))->isChecked();
               }
               else
               {
                  q_NewValue = false;
               }
            }
            else
            {
               q_NewValue = false;
            }
            c_RoutingFlags.push_back(q_NewValue);

            //diagnosis
            if (q_IsDiagnosisAvailable == true)
            {
               //set state only if the cell is enabled (connected + available)
               if (this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt,
                                                                         sn_ColDiagnostic)->isEnabled() == true)
               {
                  //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
                  q_NewValue =
                     dynamic_cast<C_OgeChxTristate *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                                       ->cellWidget(u16_ComIfCnt, sn_ColDiagnostic))->isChecked();
               }
               else
               {
                  q_NewValue = false;
               }
            }
            else
            {
               q_NewValue = false;
            }
            c_DiagnosisFlags.push_back(q_NewValue);
         }

         //save new node
         C_PuiSdHandler::h_GetInstance()->SetOSCNodePropertiesDetailed(this->mu32_NodeIndex, c_Name, c_Comment,
                                                                       e_DiagnosticServer, e_FlashLoader, c_NodeIds,
                                                                       c_UpdateFlags, c_RoutingFlags, c_DiagnosisFlags);
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Reacts on changing protocol

   Adapts the visibility of the STW flashloader options button
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_SupportedProtocolChange(void)
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   // Update the visibility of the STW flashloader specific option button
   this->mpc_Ui->pc_PushButtonFlashloaderOptions->setVisible(
      this->mpc_Ui->pc_ComboBoxProtocol->currentIndex() == mu8_FL_INDEX_STW);

   // Save the data
   this->m_RegisterChange();

   // Update the com interface routing settings in the table
   if (pc_Node != NULL)
   {
      uint16 u16_ComIfCnt;
      const C_OSCDeviceDefinition * const pc_DevDef = pc_Node->pc_DeviceDefinition;

      tgl_assert(pc_DevDef != NULL);
      if (pc_DevDef != NULL)
      {
         for (u16_ComIfCnt = 0U;
              u16_ComIfCnt <
              (static_cast<uint16>(pc_DevDef->u8_NumCanBusses) + static_cast<uint16>(pc_DevDef->u8_NumEthernetBusses));
              ++u16_ComIfCnt)
         {
            const C_OSCNodeComInterfaceSettings & rc_CurInterface = pc_Node->c_Properties.c_ComInterfaces[u16_ComIfCnt];
            const sintn sn_ColRouting = static_cast<sintn>(C_SdNdeComIfSettingsTableDelegate::eROUTING);
            const bool q_IsRoutingAvailable = pc_Node->IsRoutingAvailable(rc_CurInterface.e_InterfaceType);
            //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
            C_OgeChxTristate * pc_Tristate = dynamic_cast<C_OgeChxTristate *>(this->mpc_Ui->pc_TableWidgetComIfSettings
                                                                              ->cellWidget(u16_ComIfCnt,
                                                                                           sn_ColRouting));

            if (pc_Tristate != NULL)
            {
               this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(u16_ComIfCnt, sn_ColRouting)->setEnabled(
                  q_IsRoutingAvailable);

               if (q_IsRoutingAvailable == true)
               {
                  //set node value
                  pc_Tristate->setChecked(rc_CurInterface.q_IsRoutingEnabled);
               }
               else
               {
                  // Setting is disabled, so it has to be unchecked too
                  pc_Tristate->setChecked(false);
               }
            }
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check node name
   - check input
   - show/hide invalid icon
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_CheckNodeName(void)
{
   //check
   const QString c_Text = this->mpc_Ui->pc_LineEditNodeName->text();
   const bool q_NameIsUnique = C_PuiSdHandler::h_GetInstance()->CheckNodeNameAvailable(
      c_Text.toStdString().c_str(), &this->mu32_NodeIndex, NULL);
   const bool q_NameIsValid = C_OSCUtils::h_CheckValidCName(
      this->mpc_Ui->pc_LineEditNodeName->text().toStdString().c_str());

   //set invalid text property
   C_OgeWiUtil::h_ApplyStylesheetProperty(this->mpc_Ui->pc_LineEditNodeName, "Valid", q_NameIsUnique && q_NameIsValid);

   if ((q_NameIsUnique == true) && (q_NameIsValid == true))
   {
      this->mpc_Ui->pc_LineEditNodeName->SetToolTipInformation("", "", C_NagToolTip::eDEFAULT);
   }
   else
   {
      const QString c_Heading = C_GtGetText::h_GetText("Node Name");
      QString c_Content;
      if (q_NameIsUnique == false)
      {
         c_Content += C_GtGetText::h_GetText("- is already in use\n");
      }
      if (q_NameIsValid == false)
      {
         c_Content += C_GtGetText::h_GetText("- is empty or contains invalid characters.\n");
      }
      this->mpc_Ui->pc_LineEditNodeName->SetToolTipInformation(c_Heading, c_Content, C_NagToolTip::eERROR);
   }

   Q_EMIT this->SigChanged();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Trim node name

   Remove whitespaces at the beginning and end of the string
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_TrimNodeName(void) const
{
   this->mpc_Ui->pc_LineEditNodeName->setText(this->mpc_Ui->pc_LineEditNodeName->text().trimmed());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Register Change

   Function where ui elements register a change. Change will be sent via a signal
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_RegisterChange(void)
{
   SaveToData();
   //signal
   Q_EMIT this->SigChanged();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Register name change
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_RegisterNameChange(void)
{
   //This function can somehow be called twice, so ... let's avoid that!
   static bool hq_InProgress = false;

   if (hq_InProgress == false)
   {
      std::vector<stw_scl::C_SCLString> c_ExistingNames;
      hq_InProgress = true;
      if (C_PuiSdHandler::h_GetInstance()->CheckNodeNameAvailable(this->mpc_Ui->pc_LineEditNodeName->text().toStdString()
                                                                  .
                                                                  c_str(), &this->mu32_NodeIndex,
                                                                  &c_ExistingNames) == false)
      {
         const QString c_Description = QString(C_GtGetText::h_GetText(
                                                  "A node with the name \"%1\" already exists. Choose another name.")).
                                       arg(this->mpc_Ui->pc_LineEditNodeName->text());
         QString c_Details;
         C_OgeWiCustomMessage c_Message(this, C_OgeWiCustomMessage::eERROR);
         c_Message.SetHeading(C_GtGetText::h_GetText("Node naming"));
         c_Message.SetDescription(c_Description);
         c_Details.append(C_GtGetText::h_GetText("Used node names:\n"));
         for (uint32 u32_ItExistingName = 0UL; u32_ItExistingName < c_ExistingNames.size(); ++u32_ItExistingName)
         {
            const C_SCLString & rc_Name = c_ExistingNames[u32_ItExistingName];
            c_Details.append(QString("\"%1\"\n").arg(rc_Name.c_str()));
         }
         c_Message.SetDetails(c_Details);
         c_Message.Execute();
         //Restore previous name
         {
            const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);
            if (pc_Node != NULL)
            {
               this->mpc_Ui->pc_LineEditNodeName->setText(pc_Node->c_Properties.c_Name.c_str());
            }
         }
      }
      else
      {
         m_TrimNodeName();
         m_RegisterChange();
         Q_EMIT this->SigNameChanged(this->mpc_Ui->pc_LineEditNodeName->text());
      }
      hq_InProgress = false;
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check com interface id
   - check input
   - valid/invalid text

   \param[in]     ou32_Row         Row
   \param[in]     ou32_Column      Column
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_CheckComIfId(const uint32 ou32_Row, const uint32 ou32_Column) const
{
   const sintn sn_ColNodeId = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eNODEID);

   Q_UNUSED(ou32_Row)

   //node id change?
   if (ou32_Column == sn_ColNodeId)
   {
      sintn sn_RowCounter; // Equals com interface number.
      // Check all node ids. It could be possible that a node is connected at least two times with the same bus
      // so the node ids of the same node shall not be identical

      for (sn_RowCounter = 0U; sn_RowCounter < this->mpc_Ui->pc_TableWidgetComIfSettings->model()->rowCount();
           ++sn_RowCounter)
      {
         const uint8 u8_NodeId =
            static_cast<uint8> (this->mpc_Ui->pc_TableWidgetComIfSettings->item(sn_RowCounter,
                                                                                sn_ColNodeId)->text().toInt());

         //check if node id is valid
         const bool q_IdIsValid =
            C_PuiSdHandler::h_GetInstance()->GetOSCSystemDefinitionConst().CheckInterfaceIsAvailable(
               this->mu32_NodeIndex,
               sn_RowCounter,
               u8_NodeId);

         if (q_IdIsValid == false)
         {
            //invalid
            this->mpc_Ui->pc_TableWidgetComIfSettings->model()->setData(
               this->mpc_Ui->pc_TableWidgetComIfSettings->model()->index(sn_RowCounter, sn_ColNodeId),
               mc_STYLE_GUIDE_COLOR_24, static_cast<sintn> (Qt::ForegroundRole));
         }
         else
         {
            //const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

            //SSI: 17.04.18
            //Feature drop: Disable not connected interfaces
            //Reason: Node IFs properties has to be defined also if the interface is not connected to a bus
            //E.g.: No eds import permitted for node ids = 0.
            //
            //To avoid logic changes and to keep the possibility for quick feature revert
            //the code snippet is still here
            //         if (((pc_Node != NULL) && (u32_RowCounter < pc_Node->c_Properties.c_ComInterfaces.size())) &&
            //             (pc_Node->c_Properties.c_ComInterfaces[u32_RowCounter].q_IsBusConnected == true))
            //         {
            //            //valid
            //            this->mpc_Ui->pc_TableWidgetComIfSettings->model()->setData(
            //               this->mpc_Ui->pc_TableWidgetComIfSettings->model()->index(u32_RowCounter, sn_ColNodeId),
            //               mc_STYLE_GUIDE_COLOR_6, static_cast<sintn> (Qt::ForegroundRole));
            //         }
            //         else
            //         {
            //            //valid
            //            this->mpc_Ui->pc_TableWidgetComIfSettings->model()->setData(
            //               this->mpc_Ui->pc_TableWidgetComIfSettings->model()->index(u32_RowCounter, sn_ColNodeId),
            //               mc_STYLE_GUIDE_COLOR_10, static_cast<sintn> (Qt::ForegroundRole));
            //         }

            //valid
            this->mpc_Ui->pc_TableWidgetComIfSettings->model()->setData(
               this->mpc_Ui->pc_TableWidgetComIfSettings->model()->index(sn_RowCounter, sn_ColNodeId),
               mc_STYLE_GUIDE_COLOR_6, static_cast<sintn> (Qt::ForegroundRole));
         }

         //handle invalid icon
         //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
         dynamic_cast<QCheckBox *> (this->mpc_Ui->pc_TableWidgetComIfSettings
                                    ->cellWidget(sn_RowCounter,
                                                 static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eINTERFACE)))
         ->setChecked(!q_IdIsValid);
         if (q_IdIsValid == true)
         {
            this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipAt(sn_RowCounter,
                                                                    static_cast<uint32> (
                                                                       C_SdNdeComIfSettingsTableDelegate
                                                                       ::eINTERFACE), "", "",
                                                                    C_NagToolTip::eDEFAULT);
         }
         else
         {
            const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);
            if (pc_Node != NULL)
            {
               if (sn_RowCounter < static_cast<sintn>(pc_Node->c_Properties.c_ComInterfaces.size()))
               {
                  const C_OSCNodeComInterfaceSettings & rc_Interface =
                     pc_Node->c_Properties.c_ComInterfaces[sn_RowCounter];
                  if (rc_Interface.q_IsBusConnected == true)
                  {
                     const C_OSCSystemBus * const pc_Bus = C_PuiSdHandler::h_GetInstance()->GetOSCBus(
                        rc_Interface.u32_BusIndex);
                     if (pc_Bus != NULL)
                     {
                        const std::vector<uint32> c_UsedIds = C_SdUtil::h_GetUsedNodeIdsForBusUniqueAndSortedAscending(
                           rc_Interface.u32_BusIndex, this->mu32_NodeIndex, static_cast<sint32>(sn_RowCounter));
                        const QString c_Heading = C_GtGetText::h_GetText("Interface: Node ID invalid");
                        const QString c_Content = C_SdUtil::h_InitUsedIdsString(c_UsedIds,
                                                                                pc_Bus->c_Name.c_str(),
                                                                                C_GtGetText::h_GetText("bus"));
                        this->mpc_Ui->pc_TableWidgetComIfSettings->SetToolTipAt(sn_RowCounter,
                                                                                static_cast<uint32> (
                                                                                   C_SdNdeComIfSettingsTableDelegate
                                                                                   ::eINTERFACE), c_Heading,
                                                                                c_Content, C_NagToolTip::eERROR);
                     }
                  }
               }
            }
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   m_IpAddressClick
   - check if ip cell is clicked
   - ...

   \param[in]     ou32_Row         Row
   \param[in]     ou32_Column      Column
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_IpAddressClick(const uint32 ou32_Row, const uint32 ou32_Column)
{
   const sintn sn_ColIpAddress = static_cast<sintn> (C_SdNdeComIfSettingsTableDelegate::eIPADDRESS);
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);
   bool q_EnabledCell = this->mpc_Ui->pc_TableWidgetComIfSettings->cellWidget(ou32_Row,
                                                                              sn_ColIpAddress)->isEnabled();

   //enabled ip address column clicked?
   if ((ou32_Column == sn_ColIpAddress) && (q_EnabledCell == true))
   {
      if (pc_Node != NULL)
      {
         //make copy for restoring
         //const C_OSCNodeComInterfaceSettings::C_IpAddress c_Copy =
         // pc_Node->c_Properties.c_ComInterfaces[ou32_Row].c_Ip;

         //Set parent for better hierarchy handling via window manager
         QPointer<C_OgePopUpDialog> c_New = new C_OgePopUpDialog(this->parentWidget(), this->parentWidget());
         C_SdNdeIpAddressConfigurationWidget * const pc_Dialog =
            new C_SdNdeIpAddressConfigurationWidget(*c_New, this->mu32_NodeIndex, ou32_Row);
         Q_UNUSED(pc_Dialog)

         //Resize
         c_New->SetSize(QSize(600, 300));

         if (c_New->exec() == static_cast<sintn>(QDialog::Accepted))
         {
            //refresh table
            this->m_LoadFromData();
         }
         //Hide overlay after dialog is not relevant anymore
         if (c_New.isNull() == false)
         {
            c_New->HideOverlay();
         }
         //lint -e{429}  no memory leak because of the parent of pc_New and pc_Dialog and the Qt memory management
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of flashloader options button
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeNodePropertiesWidget::m_FlashloaderOptions(void) const
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   if (pc_Node != NULL)
   {
      const C_OSCNodeStwFlashloaderSettings c_Copy = pc_Node->c_Properties.c_STWFlashloaderSettings;
      //Set parent for better hierarchy handling via window manager
      QPointer<C_OgePopUpDialog> c_New = new C_OgePopUpDialog(this->parentWidget(), this->parentWidget());
      C_SdNdeStwFlashloaderOptions * const pc_Dialog = new C_SdNdeStwFlashloaderOptions(*c_New, this->mu32_NodeIndex);

      Q_UNUSED(pc_Dialog)

      //Resize
      c_New->SetSize(QSize(768, 518));

      if (c_New->exec() != static_cast<sintn>(QDialog::Accepted))
      {
         //Revert
         C_PuiSdHandler::h_GetInstance()->SetStwFlashloaderSettings(this->mu32_NodeIndex, c_Copy);
      }
      //Hide overlay after dialog is not relevant anymore
      if (c_New.isNull() == false)
      {
         c_New->HideOverlay();
      }
      //lint -e{429}  no memory leak because of the parent of pc_New and pc_Dialog and the Qt memory management
   }
}
