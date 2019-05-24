//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Generic table combo box interaction field (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_CAMOGECBXTABLE_H
#define C_CAMOGECBXTABLE_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "C_OgeCbxResizingView.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_CamOgeCbxTable :
   public C_OgeCbxResizingView
{
   Q_OBJECT

public:
   C_CamOgeCbxTable(QWidget * const opc_Parent = NULL);
   ~C_CamOgeCbxTable(void);

private:
   //Avoid call
   C_CamOgeCbxTable(const C_CamOgeCbxTable &);
   C_CamOgeCbxTable & operator =(const C_CamOgeCbxTable &);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
