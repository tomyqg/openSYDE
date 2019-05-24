//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Header view for tables with both headers (implementation)

   Header view for tables with both headers.

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "C_SdNdeDataPoolListDataSetHeaderView.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_opensyde_gui;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in]     ore_Orientation Orientation
   \param[in,out] opc_Parent      Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeDataPoolListDataSetHeaderView::C_SdNdeDataPoolListDataSetHeaderView(const Qt::Orientation & ore_Orientation,
                                                                           QWidget * const opc_Parent) :
   C_SdNdeDataPoolListTableHeaderView(ore_Orientation, opc_Parent)
{
}
