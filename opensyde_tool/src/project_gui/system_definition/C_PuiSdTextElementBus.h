//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       UI text element bus data: stores UI information (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_PUISDTEXTELEMENTBUS_H
#define C_PUISDTEXTELEMENTBUS_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "stwtypes.h"

#include "C_PuiBsTextElement.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_PuiSdTextElementBus :
   public C_PuiBsTextElement
{
public:
   C_PuiSdTextElementBus();
   virtual ~C_PuiSdTextElementBus();

   virtual void CalcHash(stw_types::uint32 & oru32_HashValue) const;

   stw_types::uint32 u32_BusIndex;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
