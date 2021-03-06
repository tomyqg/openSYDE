//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       short description (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDESFORESETMESSAGETABLEVIEW_H
#define C_SDNDESFORESETMESSAGETABLEVIEW_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QTableView>
#include "stwtypes.h"
#include "C_SdNdeSfoResetMessageTableModel.h"
#include "C_SdNdeSfoResetMessageTableDelegate.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeSfoResetMessageTableView :
   public QTableView
{
   Q_OBJECT

public:
   C_SdNdeSfoResetMessageTableView(QWidget * const opc_Parent = NULL);

   void SetNode(const stw_types::uint32 ou32_NodeIndex);

private:
   stw_opensyde_gui_logic::C_SdNdeSfoResetMessageTableDelegate mc_Delegate;
   stw_opensyde_gui_logic::C_SdNdeSfoResetMessageTableModel mc_Model;

   void m_InitColumns(void);
   void m_ScrollBarRangeChangedHor(const stw_types::sintn osn_Min, const stw_types::sintn osn_Max) const;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
