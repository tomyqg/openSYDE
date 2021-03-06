//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       List with all datapools of a specific type of one node (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEDPSELECTORLISTWIDGET_H
#define C_SDNDEDPSELECTORLISTWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <QTimer>
#include <QAction>

#include "stwtypes.h"

#include "C_SdNdeDpSelectorListDelegate.h"

#include "C_OgeHorizontalListWidget.h"
#include "C_OgeContextMenu.h"
#include "C_OSCNodeDataPool.h"
#include "C_OSCNodeDataPoolId.h"
#include "C_OSCCanProtocol.h"
#include "C_PuiSdNodeDataPool.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeDpSelectorListWidget :
   public stw_opensyde_gui_elements::C_OgeHorizontalListWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeDpSelectorListWidget(QWidget * const opc_Parent = NULL);
   virtual ~C_SdNdeDpSelectorListWidget();

   bool SetTypeAndNode(const stw_opensyde_core::C_OSCNodeDataPool::E_Type oe_Type,
                       const stw_types::uint32 ou32_NodeIndex, const bool oq_UsageViewActive);
   bool SetActualDataPoolConflict(const bool oq_Active) const;
   void UpdateActualDataPool(void);
   void SetActive(const bool oq_Active);
   void SetMaximized(const bool oq_Maximized);
   void AddNewDatapool(void);
   void Paste(void);
   bool CheckDataPoolsForConflict(std::vector<stw_types::uint32> * const opc_InvalidDatapoolIndices = NULL) const;
   bool SetDataPoolActive(const stw_types::uint32 ou32_DataPoolIndex);

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigWidgetFocused(const stw_types::sintn osn_DataPoolWidgetIndex, const bool oq_ForceChange = false);
   void SigDataPoolChanged(const bool oq_InitChange = false);
   void SigErrorCheck(void);
   void SigUpdateLists(const stw_types::uint32 ou32_NodeIndex, const stw_types::uint32 ou32_DataPoolIndex);
   void SigNoDataPoolSelected(void);
   void SigHideOtherToolTips(void);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void paintEvent(QPaintEvent * const opc_Event) override;
   virtual void resizeEvent(QResizeEvent * const opc_Event) override;
   virtual void keyPressEvent(QKeyEvent * const opc_Event) override;
   virtual void focusInEvent(QFocusEvent * const opc_Event) override;
   virtual void mouseDoubleClickEvent(QMouseEvent * const opc_Event) override;
   //lint -restore

   virtual void m_MoveItem(const stw_types::sintn osn_SourceIndex, const stw_types::sintn osn_TargetIndex) override;
   virtual void m_UpdateNumbers(void) const override;
   virtual void m_DelegateStartPaint(void) override;
   virtual void m_DelegateStopPaint(void) override;

private:
   //Avoid call
   C_SdNdeDpSelectorListWidget(const C_SdNdeDpSelectorListWidget &);
   C_SdNdeDpSelectorListWidget & operator =(const C_SdNdeDpSelectorListWidget &);

   bool m_OpenShareDataPoolDialog(stw_opensyde_core::C_OSCNodeDataPool & orc_OSCDataPool,
                                  bool & orq_SharedDatapoolSelected,
                                  stw_opensyde_core::C_OSCNodeDataPoolId & orc_SharedDatapoolId);
   bool m_OpenDataPoolDialog(stw_opensyde_core::C_OSCNodeDataPool & orc_OSCDataPool,
                             stw_opensyde_gui_logic::C_PuiSdNodeDataPool & orc_UiDataPool,
                             const stw_opensyde_core::C_OSCNodeDataPoolId * const opc_SharedDatapoolId,
                             const bool oq_ShowApplicationSection, const stw_types::sint32 os32_DataPoolIndex = -1,
                             const bool oq_SelectName = false);
   void m_AddNewDataPool(const stw_opensyde_core::C_OSCNodeDataPool & orc_OSCDataPool,
                         const stw_opensyde_gui_logic::C_PuiSdNodeDataPool & orc_UIDataPool,
                         const stw_types::sint32 os32_DataPoolIndex, const bool oq_AllowNameAdaptation,
                         const bool oq_AllowDataAdaptation, const bool oq_SharedDatapoolSelected = false,
                         const stw_opensyde_core::C_OSCNodeDataPoolId * const opc_SharedDatapoolId = NULL);
   void m_AddDataPoolWidget(const stw_types::uint32 ou32_DataPoolIndex);
   void m_UpdateDataPoolWidget(const stw_types::uint32 ou32_DataPoolIndex,
                               const stw_types::sintn osn_DataPoolWidgetIndex) const;
   void m_SynchronizeDatapoolProperties(const stw_opensyde_core::C_OSCNodeDataPoolId & orc_AdaptedDatapool) const;

   void m_OnCustomContextMenuRequested(const QPoint & orc_Pos);
   void m_SetupContextMenu(void);
   void m_Edit(const bool oq_SelectName = false);
   void m_Copy(void) const;
   void m_Cut(void);
   void m_DeleteSlot(void);
   void m_Delete(const bool oq_AskUser);
   void m_MoveLeft(void);
   void m_MoveRight(void);
   void m_ItemDoubleClicked(QListWidgetItem * const opc_Item);

   void m_MoveDatapool(const stw_types::sintn osn_SourceIndex, const stw_types::sintn osn_TargetIndex);
   void m_InitFromData(const bool oq_Update = false);
   void m_SelectionChanged(void);
   void m_AdaptSize(const QSize & orc_WidgetSize);
   void m_UpdateCounters(void);
   void m_UpdateItemErrorToolTip(void) const;

   stw_opensyde_gui_logic::C_SdNdeDpSelectorListDelegate mc_Delegate;
   stw_opensyde_gui_elements::C_OgeContextMenu * mpc_ContextMenu;
   QAction * mpc_AddAction;
   QAction * mpc_EditAction;
   QAction * mpc_EditActionSeparator;
   QAction * mpc_DeleteAction;
   QAction * mpc_DeleteActionSeparator;
   QAction * mpc_CopyAction;
   QAction * mpc_CutAction;
   QAction * mpc_PasteAction;
   QAction * mpc_MoveActionSeparator;
   QAction * mpc_MoveLeftAction;
   QAction * mpc_MoveRightAction;

   stw_types::uint32 mu32_NodeIndex;
   stw_types::sint32 ms32_LastKnownDpCount;
   stw_opensyde_core::C_OSCNodeDataPool::E_Type me_DataPoolType;
   stw_opensyde_core::C_OSCCanProtocol::E_Type me_ProtocolType;
   bool mq_ItemActive;
   bool mq_Maximized;
   bool mq_UsageViewActive;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
