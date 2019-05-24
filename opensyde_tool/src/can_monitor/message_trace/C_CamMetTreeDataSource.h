//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Artificial data generator (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_CAMMETTREEDATASOURCE_H
#define C_CAMMETTREEDATASOURCE_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QTimer>
#include <QObject>
#include "C_CamMetTreeLoggerData.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_CamMetTreeDataSource :
   public QObject
{
   Q_OBJECT

public:
   explicit C_CamMetTreeDataSource(QObject * const opc_Parent = NULL);

   void Start(void);

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigInternalTrigger(void);
   void SigNewData(const C_CamMetTreeLoggerData & orc_Data);

private:
   QTimer mc_Timer;

   void m_GenerateNewData(void);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
