//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Screen for parameter set file information (implementation)

   Screen for parameter set file information

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QFileInfo>

#include "stwtypes.h"
#include "stwerrors.h"
#include "C_GtGetText.h"
#include "C_SyvUpParamSetFileAddPopUp.h"
#include "ui_C_SyvUpParamSetFileAddPopUp.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] orc_Parent      Reference to parent
   \param[in]     orc_Path        Path for parameter set (usage: absolute path to load)
   \param[in]     orc_StoragePath Path to return for param info structure (usage: relative path for saving)
   \param[in]     ou32_NodeIndex  Node index for comparison
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpParamSetFileAddPopUp::C_SyvUpParamSetFileAddPopUp(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent,
                                                         const QString & orc_Path, const QString & orc_StoragePath,
                                                         const uint32 ou32_NodeIndex) :
   QWidget(&orc_Parent),
   mpc_Ui(new Ui::C_SyvUpParamSetFileAddPopUp),
   mrc_ParentDialog(orc_Parent),
   mc_FileInfo(orc_Path, orc_StoragePath, ou32_NodeIndex)
{
   this->mpc_Ui->setupUi(this);

   InitStaticNames();

   // register the widget for showing
   this->mrc_ParentDialog.SetWidget(this);

   connect(this->mpc_Ui->pc_PushButtonOk, &QPushButton::clicked, this, &C_SyvUpParamSetFileAddPopUp::m_OkClicked);
   connect(this->mpc_Ui->pc_PushButtonCancel, &QPushButton::clicked, this,
           &C_SyvUpParamSetFileAddPopUp::m_CancelClicked);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpParamSetFileAddPopUp::~C_SyvUpParamSetFileAddPopUp(void)
{
   delete this->mpc_Ui;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle file read step

   \return
   C_NO_ERR   data read
   C_RD_WR    specified file does not exist
              specified file is present but structure is invalid (e.g. invalid XML file; not checksum found)
   C_CHECKSUM specified file is present but checksum is invalid
   C_CONFIG   file does not contain essential information
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_SyvUpParamSetFileAddPopUp::ReadFile(void)
{
   const sint32 s32_Result = this->mc_FileInfo.ReadFile();
   const QString & rc_Text = this->mc_FileInfo.GetComparisonResultsHtml();
   QString c_Html;

   c_Html += "<html>";
   c_Html += "<body>";
   c_Html += rc_Text;
   c_Html += "</body>";
   c_Html += "</html>";

   this->mpc_Ui->pc_TextEditCompare->setHtml(c_Html);
   return s32_Result;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get read file info (check file read function return value for validity)

   \return
   Read file info (check file read function return value for validity)
*/
//----------------------------------------------------------------------------------------------------------------------
const C_PuiSvNodeUpdateParamInfo & C_SyvUpParamSetFileAddPopUp::GetParamInfo(void) const
{
   return this->mc_FileInfo.GetParamInfo();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize all displayed static names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpParamSetFileAddPopUp::InitStaticNames(void) const
{
   const QFileInfo c_Info(this->mc_FileInfo.GetPath());

   this->mrc_ParentDialog.SetTitle(C_GtGetText::h_GetText("Parameter Set Image File"));
   this->mrc_ParentDialog.SetSubTitle(c_Info.fileName());
   this->mpc_Ui->pc_LabelHeadingPreview->setText(C_GtGetText::h_GetText("File Information"));
   this->mpc_Ui->pc_PushButtonOk->setText(C_GtGetText::h_GetText("Accept"));
   this->mpc_Ui->pc_PushButtonCancel->setText(C_GtGetText::h_GetText("Discard"));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten key press event slot

   Here: Handle specific enter key cases

   \param[in,out] opc_KeyEvent Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpParamSetFileAddPopUp::keyPressEvent(QKeyEvent * const opc_KeyEvent)
{
   bool q_CallOrg = true;

   //Handle all enter key cases manually
   if ((opc_KeyEvent->key() == static_cast<sintn>(Qt::Key_Enter)) ||
       (opc_KeyEvent->key() == static_cast<sintn>(Qt::Key_Return)))
   {
      if (((opc_KeyEvent->modifiers().testFlag(Qt::ControlModifier) == true) &&
           (opc_KeyEvent->modifiers().testFlag(Qt::AltModifier) == false)) &&
          (opc_KeyEvent->modifiers().testFlag(Qt::ShiftModifier) == false))
      {
         this->mrc_ParentDialog.accept();
      }
      else
      {
         q_CallOrg = false;
      }
   }
   if (q_CallOrg == true)
   {
      QWidget::keyPressEvent(opc_KeyEvent);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of Ok button click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpParamSetFileAddPopUp::m_OkClicked(void)
{
   this->mrc_ParentDialog.accept();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of Cancel button click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpParamSetFileAddPopUp::m_CancelClicked(void)
{
   this->mrc_ParentDialog.reject();
}
