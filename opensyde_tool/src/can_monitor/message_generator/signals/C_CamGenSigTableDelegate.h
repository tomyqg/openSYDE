//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Delegate for message generator signals table (header)

   See cpp file for detailed description

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_CAMGENSIGTABLEDELEGATE_H
#define C_CAMGENSIGTABLEDELEGATE_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "C_TblDelegate.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_CamGenSigTableDelegate :
   public C_TblDelegate
{
   Q_OBJECT

public:
   C_CamGenSigTableDelegate(QObject * const opc_Parent = NULL);

   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void setEditorData(QWidget * const opc_Editor, const QModelIndex & orc_Index) const override;
   //lint -restore

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736
Q_SIGNALS:
   //lint -restore
   void SigAfterSetEditorData(QLineEdit * const opc_Widget) const;

protected:
   virtual QComboBox * m_CreateComboBox(QWidget * const opc_Parent) const override;

private:
   void m_AfterSetEditorData(QLineEdit * const opc_Widget) const;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
