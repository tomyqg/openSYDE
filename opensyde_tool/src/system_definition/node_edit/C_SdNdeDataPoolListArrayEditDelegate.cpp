//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Node data pool list array edit drawing delegate (implementation)

   Node data pool list array edit drawing delegate

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <limits>
#include <QPainter>
#include "C_SdNdeDataPoolContentUtil.h"
#include "C_SdNdeDataPoolListArrayEditDelegate.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeDataPoolListArrayEditDelegate::C_SdNdeDataPoolListArrayEditDelegate(QObject * const opc_Parent) :
   QStyledItemDelegate(opc_Parent),
   mpc_Model(NULL)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten create editor event slot

   Here: Create appropriate editor widget

   \param[in,out] opc_Parent Parent widget
   \param[in]     orc_Option Style options
   \param[in]     orc_Index  Correlating index

   \return
   Editor widget
*/
//----------------------------------------------------------------------------------------------------------------------
QWidget * C_SdNdeDataPoolListArrayEditDelegate::createEditor(QWidget * const opc_Parent,
                                                             const QStyleOptionViewItem & orc_Option,
                                                             const QModelIndex & orc_Index) const
{
   QWidget * pc_Retval = NULL;

   Q_UNUSED(orc_Option)
   if (orc_Index.isValid() == true)
   {
      pc_Retval = m_CreateEditor(opc_Parent, orc_Index);
   }
   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten set editor data event slot

   Here: Pass relevant data

   \param[in,out] opc_Editor Editor widget
   \param[in]     orc_Index  Correlating index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDataPoolListArrayEditDelegate::setEditorData(QWidget * const opc_Editor,
                                                         const QModelIndex & orc_Index) const
{
   if ((opc_Editor != NULL) && (orc_Index.isValid() == true))
   {
      C_SdNdeDataPoolUtil::h_SetGenericEditorDataVariable(opc_Editor, orc_Index);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten set model data event slot

   Here: Pass relevant data

   \param[in,out] opc_Editor Editor widget
   \param[in,out] opc_Model  Model object
   \param[in]     orc_Index  Correlating index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDataPoolListArrayEditDelegate::setModelData(QWidget * const opc_Editor,
                                                        QAbstractItemModel * const opc_Model,
                                                        const QModelIndex & orc_Index) const
{
   if (((opc_Editor != NULL) && (opc_Model != NULL)) && (orc_Index.isValid() == true))
   {
      C_SdNdeDataPoolUtil::h_SetModelGenericDataVariable(opc_Editor, opc_Model, orc_Index);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set model for column look up

   \param[in] opc_Model  Model for column look up
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDataPoolListArrayEditDelegate::SetModel(const C_SdNdeDataPoolListArrayEditModel * const opc_Value)
{
   this->mpc_Model = opc_Value;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Create widget to use for editing this value

   \param[in,out] opc_Parent    Parent widget
   \param[in]     orc_Index     Correlating index
   \param[in]     orq_UseMinMax Optional indicator if min max should be read of model

   \return
   Editor widget
*/
//----------------------------------------------------------------------------------------------------------------------
QWidget * C_SdNdeDataPoolListArrayEditDelegate::m_CreateEditor(QWidget * const opc_Parent,
                                                               const QModelIndex & orc_Index) const
{
   QWidget * pc_Retval = NULL;

   if (orc_Index.isValid() == true)
   {
      if (this->mpc_Model != NULL)
      {
         //Core data
         const C_OSCNodeDataPoolListElement * const pc_Element = this->mpc_Model->GetOSCElement();
         if (pc_Element != NULL)
         {
            if (this->mpc_Model->GetArrayEditType() == C_SdNdeDataPoolUtil::eARRAY_EDIT_MIN)
            {
               //Adapt min
               C_OSCNodeDataPoolContent c_NewMin = pc_Element->c_MinValue;
               C_SdNdeDataPoolContentUtil::h_InitMin(c_NewMin);
               pc_Retval = C_SdNdeDataPoolUtil::h_CreateGenericEditor(opc_Parent, orc_Index, c_NewMin,
                                                                      pc_Element->c_MaxValue, pc_Element->f64_Factor,
                                                                      pc_Element->f64_Offset,
                                                                      static_cast<uint32>(orc_Index.column()), false);
            }
            else if (this->mpc_Model->GetArrayEditType() == C_SdNdeDataPoolUtil::eARRAY_EDIT_MAX)
            {
               //Adapt max
               C_OSCNodeDataPoolContent c_NewMax = pc_Element->c_MaxValue;
               C_SdNdeDataPoolContentUtil::h_InitMax(c_NewMax);
               pc_Retval = C_SdNdeDataPoolUtil::h_CreateGenericEditor(opc_Parent, orc_Index, pc_Element->c_MinValue,
                                                                      c_NewMax, pc_Element->f64_Factor,
                                                                      pc_Element->f64_Offset,
                                                                      static_cast<uint32>(orc_Index.column()), false);
            }
            else
            {
               pc_Retval = C_SdNdeDataPoolUtil::h_CreateGenericEditor(opc_Parent, orc_Index, pc_Element->c_MinValue,
                                                                      pc_Element->c_MaxValue, pc_Element->f64_Factor,
                                                                      pc_Element->f64_Offset,
                                                                      static_cast<uint32>(orc_Index.column()), false);
            }
         }
      }
   }
   return pc_Retval;
}
