//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for displaying and editing hardware configuration.
   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEHALCWIDGET_H
#define C_SDNDEHALCWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>

#include "stwtypes.h"

#include "C_OSCHalcDef.h"
#include "C_OSCHalcConfig.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace Ui
{
class C_SdNdeHalcWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeHalcWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeHalcWidget(QWidget * const opc_Parent = NULL);
   ~C_SdNdeHalcWidget(void);

   void InitStaticNames(void) const;
   void SetNode(const stw_types::uint32 ou32_NodeIndex);

private:
   Ui::C_SdNdeHalcWidget * mpc_Ui;
   stw_types::uint32 mu32_NodeIndex;

   static const QString mhc_CONFIG_FILE_TYPE;

   //Avoid call
   C_SdNdeHalcWidget(const C_SdNdeHalcWidget &);
   C_SdNdeHalcWidget & operator =(const C_SdNdeHalcWidget &);

   void m_DisableButtons(const bool oq_Active) const;
   void m_OnImportConfigClicked(void);
   void m_OnExportConfigClicked (void);
   void m_OnSettingsClicked(void);
   void m_OnCleanUpClicked(void) const;
   void m_OnLoadClicked(void);
   void m_OnViewDetailsClicked(void) const;
   void m_OnMagicRequested(void); // TODO remove - development only
   void m_OnOverviewToggled(const bool oq_Checked) const;
   void m_OnChannelSelected(const QModelIndex & orc_Index) const;
   void m_OnChannelUpdate(const stw_types::uint32 ou32_DomainIndex, const stw_types::uint32 ou32_ChannelIndex,
                          const bool oq_UseChannelIndex) const;

   void m_UpdateNodeData(void) const;

   static QString mh_GetDialogPath(void);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
