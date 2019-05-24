//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Transparent (except scroll bars) scroll area (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_OGETRANSPARENTSCROLLAREA_H
#define C_OGETRANSPARENTSCROLLAREA_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <QScrollArea>

#include "stwtypes.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_OgeTransparentScrollArea :
   public QScrollArea
{
   Q_OBJECT

public:
   C_OgeTransparentScrollArea(QWidget * const opc_Parent = NULL);

   void DeactivateScrollbarResize(void);

private:
   void m_ScrollBarRangeChanged(const stw_types::sintn osn_Min, const stw_types::sintn osn_Max) const;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
