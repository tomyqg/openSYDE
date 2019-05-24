//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       System view CAN bus (implementation)

   System view CAN bus

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "stwtypes.h"
#include "C_PuiSvHandler.h"
#include "C_GiSvCANBus.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in]     ou32_ViewIndex       Index of system view
   \param[in]     ors32_Index          Index of data element in system definition
   \param[in]     oru64_ID             Unique ID
   \param[in]     opc_TextElementName  Pointer to text element for showing bus name
   \param[in]     opc_Points           Points for line
   \param[in,out] opc_Parent           Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_GiSvCANBus::C_GiSvCANBus(const uint32 ou32_ViewIndex, const sint32 & ors32_Index, const uint64 & oru64_ID,
                           C_GiTextElementBus * const opc_TextElementName,
                           const std::vector<QPointF> * const opc_Points, QGraphicsItem * const opc_Parent) :
   C_GiLiCANBus(ors32_Index, oru64_ID, opc_TextElementName, false, opc_Points, opc_Parent),
   mu32_ViewIndex(ou32_ViewIndex)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Checking the bus data for errors and updates the conflict icon
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSvCANBus::CheckBusForChanges(void)
{
   if (this->mpc_TextElementName != NULL)
   {
      if (C_PuiSvHandler::h_GetInstance()->CheckBusDisabled(this->mu32_ViewIndex,
                                                            static_cast<uint32>(this->ms32_Index)) == false)
      {
         this->mpc_TextElementName->SetErrorState(C_PuiSvHandler::h_GetInstance()->GetErrorBus(this->ms32_Index));
      }
      else
      {
         this->mpc_TextElementName->SetErrorState(false);
      }

      //check for new name
      this->mpc_TextElementName->SetText(this->GetName());
      this->mpc_TextElementName->update();
   }
}
