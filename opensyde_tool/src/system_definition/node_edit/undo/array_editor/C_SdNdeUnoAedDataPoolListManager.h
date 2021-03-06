//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool list array edit command stack (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEUNOAEDDATAPOOLLISTMANAGER_H
#define C_SDNDEUNOAEDDATAPOOLLISTMANAGER_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <QUndoCommand>
#include "stwtypes.h"
#include "C_SdNdeDpUtil.h"
#include "C_SdNdeDpListModelViewManager.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeUnoAedDataPoolListManager
{
public:
   C_SdNdeUnoAedDataPoolListManager(void);

   void DoDataChangeElements(const stw_types::uint32 & oru32_NodeIndex, const stw_types::uint32 & oru32_DataPoolIndex,
                             const stw_types::uint32 & oru32_ListIndex, const stw_types::uint32 & oru32_ElementIndex,
                             const C_SdNdeDpUtil::E_ArrayEditType & ore_ArrayEditType,
                             const stw_types::uint32 & oru32_DataSetIndex,
                             const stw_types::uint32 & oru32_ArrayElementIndex, const QVariant & orc_NewData,
                             C_SdNdeDpListModelViewManager * const opc_DataPoolListModelViewManager);
   QUndoCommand * TakeUndoCommand(void);

private:
   QUndoCommand * mpc_UndoCommand;

   void m_InitUndoCommand(void);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
