//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Delete undo command (implementation)

   Delete undo command

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "C_SdManUnoTopologyDeleteCommand.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   \param[in,out] opc_Scene            Pointer to currently active scene
   \param[in]     orc_IDs              Affected unique IDs
   \param[in,out] opc_Parent           Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdManUnoTopologyDeleteCommand::C_SdManUnoTopologyDeleteCommand(QGraphicsScene * const opc_Scene,
                                                                 const std::vector<stw_types::uint64> & orc_IDs,
                                                                 QUndoCommand * const opc_Parent) :
   C_SdManUnoTopologyAddDeleteBaseCommand(opc_Scene, orc_IDs, "Delete drawing element(s)",
                                          opc_Parent)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdManUnoTopologyDeleteCommand::~C_SdManUnoTopologyDeleteCommand()
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Redo delete
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdManUnoTopologyDeleteCommand::redo(void)
{
   this->m_DeleteSave();
   QUndoCommand::redo();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Undo delete
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdManUnoTopologyDeleteCommand::undo(void)
{
   this->m_Restore();
   QUndoCommand::undo();
}
