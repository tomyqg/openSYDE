//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget to display data pool list com entries (implementation)

   Widget to display data pool list com entries

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "C_SdNdeDpComListWidget.h"
#include "ui_C_SdNdeDpComListWidget.h"
#include "C_PuiSdHandler.h"
#include "C_SdNdeDpListComHeaderWidget.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_core;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeDpComListWidget::C_SdNdeDpComListWidget(QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SdNdeDpComListWidget)
{
   mpc_Ui->setupUi(this);
   this->setAttribute(Qt::WA_TranslucentBackground);

   this->mpc_Ui->pc_ScrollArea->DeactivateScrollbarResize();
   this->mpc_Ui->pc_ScrollAreaWidgetContents->SetBackgroundColor(-1);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeDpComListWidget::~C_SdNdeDpComListWidget(void)
{
   delete mpc_Ui;
   //Clean up widgets
   for (uint32 u32_ItWidget = 0; u32_ItWidget < this->mc_LastWidgets.size(); ++u32_ItWidget)
   {
      delete (this->mc_LastWidgets[u32_ItWidget]);
      this->mc_LastWidgets[u32_ItWidget] = NULL;
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set new data pool

   \param[in] oru32_NodeIndex     Node index
   \param[in] oru32_DataPoolIndex Data pool index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpComListWidget::SetDataPool(const uint32 & oru32_NodeIndex, const uint32 & oru32_DataPoolIndex)
{
   const C_OSCNodeDataPool * const pc_DataPool = C_PuiSdHandler::h_GetInstance()->GetOSCDataPool(oru32_NodeIndex,
                                                                                                 oru32_DataPoolIndex);

   if (pc_DataPool != NULL)
   {
      bool q_Alternating = true;
      this->Clear();
      this->mc_LastWidgets.reserve(pc_DataPool->c_Lists.size() / 2);
      for (uint32 u32_ItList = 0; u32_ItList < pc_DataPool->c_Lists.size(); u32_ItList += 2)
      {
         C_SdNdeDpListComHeaderWidget * const pc_Widget = new C_SdNdeDpListComHeaderWidget(oru32_NodeIndex,
                                                                                           oru32_DataPoolIndex,
                                                                                           u32_ItList,
                                                                                           q_Alternating);
         //List
         this->mpc_Ui->pc_VerticalLayout->insertWidget(static_cast<sintn>(u32_ItList / 2UL), pc_Widget, 0);
         //Connects
         connect(pc_Widget, &C_SdNdeDpListComHeaderWidget::SigEdit, this,
                 &C_SdNdeDpComListWidget::m_OnEdit);
         connect(pc_Widget, &C_SdNdeDpListComHeaderWidget::SigSwitchToBus, this,
                 &C_SdNdeDpComListWidget::m_OnSwitchToBus);
         connect(pc_Widget, &C_SdNdeDpListComHeaderWidget::SigErrorChange, this,
                 &C_SdNdeDpComListWidget::m_OnErrorChange);

         //Alternate
         q_Alternating = !q_Alternating;
         //Save
         this->mc_LastWidgets.push_back(pc_Widget);
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Clear content
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpComListWidget::Clear(void)
{
   //Clean up last list
   for (uint32 u32_ItWidget = 0; u32_ItWidget < this->mc_LastWidgets.size(); ++u32_ItWidget)
   {
      this->mpc_Ui->pc_VerticalLayout->removeWidget(this->mc_LastWidgets[u32_ItWidget]);
      delete (this->mc_LastWidgets[u32_ItWidget]);
      this->mc_LastWidgets[u32_ItWidget] = NULL;
   }
   //Add new widgets
   this->mc_LastWidgets.clear();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Update messages & signals
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpComListWidget::Update(void)
{
   for (uint32 u32_ItWidget = 0; u32_ItWidget < this->mc_LastWidgets.size(); ++u32_ItWidget)
   {
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      C_SdNdeDpListComHeaderWidget * const pc_Widget =
         dynamic_cast<C_SdNdeDpListComHeaderWidget * const>(this->mc_LastWidgets[u32_ItWidget]);
      if (pc_Widget != NULL)
      {
         pc_Widget->UpdateMessagesAndSignals();
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle list error change
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpComListWidget::m_OnErrorChange(void)
{
   bool q_Error = false;

   for (uint32 u32_ItWidget = 0; u32_ItWidget < this->mc_LastWidgets.size(); ++u32_ItWidget)
   {
      //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
      C_SdNdeDpListComHeaderWidget * const pc_Widget =
         dynamic_cast<C_SdNdeDpListComHeaderWidget * const>(this->mc_LastWidgets[u32_ItWidget]);
      if (pc_Widget != NULL)
      {
         if (pc_Widget->GetErrorActive() == true)
         {
            q_Error = true;
         }
      }
   }
   Q_EMIT this->SigErrorChange(q_Error);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward signal

   \param[in] ou32_DataPoolIndex Data pool index
   \param[in] ou32_ListIndex     List index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpComListWidget::m_OnEdit(const uint32 ou32_DataPoolIndex, const uint32 ou32_ListIndex)
{
   Q_EMIT this->SigEdit(ou32_DataPoolIndex, ou32_ListIndex);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward signal

   \param[in] oru32_BusIndex Bus index
   \param[in] orc_BusName  Bus name
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpComListWidget::m_OnSwitchToBus(const uint32 & oru32_BusIndex, const QString & orc_BusName)
{
   Q_EMIT this->SigSwitchToBus(oru32_BusIndex, orc_BusName);
}
