//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Custom tristate check box (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_OGECHXTRISTATE_H
#define C_OGECHXTRISTATE_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "C_OgeChxTristateBase.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_OgeChxTristate :
   public C_OgeChxTristateBase
{
   Q_OBJECT

public:
   C_OgeChxTristate(QWidget * const opc_Parent = NULL);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
