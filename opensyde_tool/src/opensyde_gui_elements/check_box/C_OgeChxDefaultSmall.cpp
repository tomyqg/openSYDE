//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Custom small default check box (implementation)

   Custom default check box.
   This class offers an optional index for more easy identification of the object

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "C_OgeChxDefaultSmall.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
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

   \param[in,out]  opc_Parent    Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_OgeChxDefaultSmall::C_OgeChxDefaultSmall(QWidget * const opc_Parent) :
   C_OgeChxToolTipBase(opc_Parent),
   mu32_Index(0U),
   mu32_SubIndex(0U)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in]      ou32_Index       Index for easy identification of the object
   \param[in]      ou32_SubIndex    Sub index for easy identification of the object
   \param[in,out]  opc_Parent       Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_OgeChxDefaultSmall::C_OgeChxDefaultSmall(const stw_types::uint32 ou32_Index, const stw_types::uint32 ou32_SubIndex,
                                           QWidget * const opc_Parent) :
   C_OgeChxToolTipBase(opc_Parent),
   mu32_Index(ou32_Index),
   mu32_SubIndex(ou32_SubIndex)
{
   connect(this, &C_OgeChxDefaultSmall::toggled, this, &C_OgeChxDefaultSmall::m_CheckBoxToggled);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns the configured indexes

   \param[out]  oru32_Index      First Index
   \param[out]  oru32_SubIndex   Second index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_OgeChxDefaultSmall::GetIndexes(stw_types::uint32 & oru32_Index, stw_types::uint32 & oru32_SubIndex) const
{
   oru32_Index = this->mu32_Index;
   oru32_SubIndex = this->mu32_SubIndex;
}

//----------------------------------------------------------------------------------------------------------------------
void C_OgeChxDefaultSmall::m_CheckBoxToggled(const bool oq_Checked)
{
   Q_EMIT this->SigCheckBoxToggled(this->mu32_Index, this->mu32_SubIndex, oq_Checked);
}
