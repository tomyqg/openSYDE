//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for system definition update

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef C_SDNDEDATAPOOLEDITWIDGET_H
#define C_SDNDEDATAPOOLEDITWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>

#include "stwtypes.h"

#include "C_NagUseCaseWidget.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */

namespace Ui
{
class C_SdNdeDataPoolEditWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeDataPoolEditWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeDataPoolEditWidget(QWidget * const opc_Parent = NULL);
   virtual ~C_SdNdeDataPoolEditWidget();
   void SetNode(const stw_types::uint32 & oru32_NodeIndex);
   void CheckDataPoolInteraction(void) const;
   void OpenDetail(const stw_types::sint32 os32_DataPoolIndex, const stw_types::sint32 os32_ListIndex,
                   const stw_types::sint32 os32_DataElementIndex) const;
   void UpdateComLists(void) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigChanged(void);
   void SigErrorChange(void) const;
   void SigSwitchToBus(const stw_types::uint32 & oru32_BusIndex, const QString & orc_BusName);
   void SigEdit(const stw_types::uint32 ou32_DataPoolIndex, const stw_types::uint32 ou32_ListIndex);
   void SigSave(void);
   void SigSaveAs(void);

private:
   //Avoid call
   C_SdNdeDataPoolEditWidget(const C_SdNdeDataPoolEditWidget &);
   C_SdNdeDataPoolEditWidget & operator =(const C_SdNdeDataPoolEditWidget &);

   Ui::C_SdNdeDataPoolEditWidget * mpc_Ui;
   stw_types::uint32 mu32_NodeIndex;

   void m_DataPoolSelected(const stw_types::uint32 ou32_DataPoolIndex) const;
   void m_OnErrorChange(const bool & orq_Error) const;
   void m_OnSizeChange(void) const;
   void m_OnSwitchToBus(const stw_types::uint32 & oru32_BusIndex, const QString & orc_BusName);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
