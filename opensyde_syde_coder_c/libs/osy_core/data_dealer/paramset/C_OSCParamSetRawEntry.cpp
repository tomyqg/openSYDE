//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data class for one raw entry in a parameter set file (implementation)

   Data class for one raw entry in a parameter set file

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "C_OSCParamSetRawEntry.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_opensyde_core;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*! \brief   Default constructor
*/
//-----------------------------------------------------------------------------
C_OSCParamSetRawEntry::C_OSCParamSetRawEntry(void) :
   u32_StartAddress(0)
{
}
