//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Tree model for HALC use case configuration.

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "stwtypes.h"
#include "stwerrors.h"
#include "constants.h"
#include "TGLUtils.h"
#include "C_GtGetText.h"

#include "C_SdNdeHalcConfigTreeModel.h"
#include "C_TblTreItem.h"

#include "C_PuiSdHandler.h"
#include "C_SdNdeDpContentUtil.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_tgl;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Default constructor

   \param[in,out]  opc_Parent    Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeHalcConfigTreeModel::C_SdNdeHalcConfigTreeModel(QObject * const opc_Parent) :
   C_TblTreModel(opc_Parent),
   mu32_NodeIndex(0),
   mu32_DomainIndex(0),
   mu32_ChannelIndex(0),
   mu32_UseCaseIndex(0),
   mq_ChannelCase(true)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Default destructor.

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeHalcConfigTreeModel::~C_SdNdeHalcConfigTreeModel()
{
   delete (this->mpc_InvisibleRootItem);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Return number of columns.

   Here: Exactly three columns: name, value, description.

   \param[in]  orc_Parent  Parent

   \return
   Number of columns, namely 3
*/
//----------------------------------------------------------------------------------------------------------------------
sintn C_SdNdeHalcConfigTreeModel::columnCount(const QModelIndex & orc_Parent) const
{
   Q_UNUSED(orc_Parent)
   return 3;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get header data

   \param[in]  osn_Section       Section
   \param[in]  oe_Orientation    Orientation
   \param[in]  osn_Role          Role

   \return
   Header string
*/
//----------------------------------------------------------------------------------------------------------------------
QVariant C_SdNdeHalcConfigTreeModel::headerData(const sintn osn_Section, const Qt::Orientation oe_Orientation,
                                                const sintn osn_Role) const
{
   QVariant c_Retval = C_TblTreModel::headerData(osn_Section, oe_Orientation, osn_Role);

   if (oe_Orientation == Qt::Orientation::Horizontal)
   {
      const C_SdNdeHalcConfigTreeModel::E_Columns e_Col = C_SdNdeHalcConfigTreeModel::h_ColumnToEnum(osn_Section);
      if (osn_Role == static_cast<sintn>(Qt::DisplayRole))
      {
         switch (e_Col)
         {
         case eNAME:
            c_Retval = C_GtGetText::h_GetText("Name");
            break;
         case eVALUE:
            c_Retval = C_GtGetText::h_GetText("Value");
            break;
         case eDESCRIPTION:
            c_Retval = C_GtGetText::h_GetText("Description");
            break;
         }
      }
      else if (osn_Role == msn_USER_ROLE_TOOL_TIP_HEADING)
      {
         switch (e_Col)
         {
         case eNAME:
            c_Retval = C_GtGetText::h_GetText("Name");
            break;
         case eVALUE:
            c_Retval = C_GtGetText::h_GetText("Value");
            break;
         case eDESCRIPTION:
            c_Retval = C_GtGetText::h_GetText("Description");
            break;
         }
      }
      else if (osn_Role == msn_USER_ROLE_TOOL_TIP_CONTENT)
      {
         switch (e_Col)
         {
         case eNAME:
            c_Retval = C_GtGetText::h_GetText("Parameter name.");
            break;
         case eVALUE:
            c_Retval = C_GtGetText::h_GetText("Value of this parameter.");
            break;
         case eDESCRIPTION:
            c_Retval = C_GtGetText::h_GetText("Description of this parameter.");
            break;
         }
      }
      else if (osn_Role == static_cast<sintn>(Qt::TextAlignmentRole))
      {
         c_Retval = QVariant(Qt::AlignLeft | Qt::AlignVCenter);
      }
      else
      {
         //No data in default case
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get data at index

   \param[in]  orc_Index   Index
   \param[in]  osn_Role    Data role

   \return
   Data
*/
//----------------------------------------------------------------------------------------------------------------------
QVariant C_SdNdeHalcConfigTreeModel::data(const QModelIndex & orc_Index, const sintn osn_Role) const
{
   QVariant c_Retval = C_TblTreModel::data(orc_Index, osn_Role);

   // first columns are handled by C_TblTreItem
   if (orc_Index.column() > 0)
   {
      // get parameter data (of parameter itself or of parameter element)
      const C_OSCHalcConfigParameter * pc_ParameterElement = m_GetParameterElement(orc_Index);
      if (pc_ParameterElement != NULL)
      {
         const C_SdNdeHalcConfigTreeModel::E_Columns e_Col =
            C_SdNdeHalcConfigTreeModel::h_ColumnToEnum(orc_Index.column());

         if ((osn_Role == static_cast<sintn>(Qt::DisplayRole)) || (osn_Role == static_cast<sintn>(Qt::EditRole)))
         {
            const std::map<stw_scl::C_SCLString, C_OSCNodeDataPoolContent> & rc_EnumItems =
               pc_ParameterElement->c_Value.GetEnumItems();

            // show data for column
            switch (e_Col)
            {
            case eNAME:
               // name is handled by tree item name
               break;
            case eVALUE:
               switch (pc_ParameterElement->c_Value.GetComplexType())
               {
               case C_OSCHalcDefContent::eCT_PLAIN:
                  c_Retval = C_SdNdeDpContentUtil::h_ConvertContentToGeneric(pc_ParameterElement->c_Value, 0);
                  break;
               case C_OSCHalcDefContent::eCT_ENUM:
                  c_Retval = C_GtGetText::h_GetText("unknown");
                  for (std::map<stw_scl::C_SCLString, C_OSCNodeDataPoolContent>::const_iterator c_It =
                          rc_EnumItems.begin(); c_It != rc_EnumItems.end(); ++c_It)
                  {
                     if (c_It->second == pc_ParameterElement->c_Value)
                     {
                        c_Retval = c_It->first.c_str();
                     }
                  }
                  break;
               case C_OSCHalcDefContent::eCT_BIT_MASK:
                  c_Retval = "TODO bitmask";
                  break;
               }
               break;
            case eDESCRIPTION:
               c_Retval = pc_ParameterElement->c_Comment.c_str();
               break;
            }
         }
         else if ((osn_Role == msn_USER_ROLE_INTERACTION_GENERIC_SPIN_BOX_PARAMETERS_LIST) && (e_Col == eVALUE))
         {
            const C_OSCHalcDefElement * pc_DefElement = this->m_GetDefParameterElement(orc_Index);
            if (pc_DefElement != NULL)
            {
               // Set generic spin box parameters
               QStringList c_List;
               QString c_TmpStr;
               // minimum
               c_TmpStr = C_SdNdeDpContentUtil::h_GetAllContentAsString(pc_DefElement->c_MinValue);
               c_List.push_back(c_TmpStr);
               // maximum
               c_TmpStr = C_SdNdeDpContentUtil::h_GetAllContentAsString(pc_DefElement->c_MaxValue);
               c_List.push_back(c_TmpStr);
               // factor
               c_TmpStr = QString::number(1.0);
               c_List.push_back(c_TmpStr);
               // offset
               c_TmpStr = QString::number(0.0);
               c_List.push_back(c_TmpStr);

               c_Retval = c_List;
            }
         }
         else if (((osn_Role == msn_USER_ROLE_INTERACTION_COMBO_BOX_VALUES_LIST) ||
                   (osn_Role == msn_USER_ROLE_INTERACTION_COMBO_BOX_STRINGS_LIST)) && (e_Col == eVALUE))
         {
            // TODO bitmask handling
            c_Retval = mh_ConvertEnumsToStringList(pc_ParameterElement->c_Value);
         }
         else if ((osn_Role == msn_USER_ROLE_INTERACTION_ELEMENT_TYPE) && (e_Col == eVALUE))
         {
            // use edit field depending on complex type
            switch (pc_ParameterElement->c_Value.GetComplexType())
            {
            case C_OSCHalcDefContent::eCT_PLAIN:
               c_Retval = static_cast<sintn>(eURIEL_GENERIC_SPIN_BOX);
               break;
            case C_OSCHalcDefContent::eCT_ENUM:
            case C_OSCHalcDefContent::eCT_BIT_MASK:
               c_Retval = static_cast<sintn>(eURIEL_COMBO_BOX);
               break;
            }
         }
         else
         {
            // Use default
         }
      }
   }

   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Set data at index

   Here: Get data from editor widgets into data handling.

   \param[in]  orc_Index   Model index
   \param[in]  orc_Value   New data
   \param[in]  osn_Role    Data role

   \return
   true  success
   false failure
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SdNdeHalcConfigTreeModel::setData(const QModelIndex & orc_Index, const QVariant & orc_Value,
                                         const sintn osn_Role)
{
   bool q_Retval = false;

   const C_SdNdeHalcConfigTreeModel::E_Columns e_Col =
      C_SdNdeHalcConfigTreeModel::h_ColumnToEnum(orc_Index.column());

   if ((data(orc_Index, osn_Role) != orc_Value) && (e_Col == eVALUE))
   {
      // get parameter data (of parameter itself or of parameter element)
      const C_OSCHalcConfigParameter * pc_ParameterElement = m_GetParameterElement(orc_Index);
      if (pc_ParameterElement != NULL)
      {
         sint32 s32_Return;
         uint32 u32_ParameterIndex;
         uint32 u32_ParameterElementIndex;
         const bool q_IsParameterElement = mh_GetParameterElementIndexe(orc_Index, u32_ParameterIndex,
                                                                        u32_ParameterElementIndex);

         // copy and adapt value
         C_OSCHalcConfigParameter c_NewParameterElement = *pc_ParameterElement;

         // extra handling of enum values
         // TODO bitmask handling
         if (c_NewParameterElement.c_Value.GetComplexType() == C_OSCHalcDefContent::eCT_ENUM)
         {
            QStringList c_Tmp = mh_ConvertEnumsToStringList(pc_ParameterElement->c_Value);
            const sint32 s32_ComboBoxIndex = static_cast<sint32>(orc_Value.toLongLong());

            if (s32_ComboBoxIndex < c_Tmp.size())
            {
               s32_Return =
                  c_NewParameterElement.c_Value.SetEnumValue(c_Tmp.at(s32_ComboBoxIndex).toStdString().c_str());
            }
            else
            {
               s32_Return = C_RANGE;
            }
         }
         else
         {
            s32_Return =
               C_SdNdeDpContentUtil::h_SetDataVariableFromGeneric(orc_Value, c_NewParameterElement.c_Value, 0);
         }

         if (s32_Return == C_NO_ERR)
         {
            const C_OSCHalcDefChannelUseCase * pc_UseCase =
               C_PuiSdHandler::h_GetInstance()->GetHALCDomainFileUseCaseData(this->mu32_NodeIndex,
                                                                             this->mu32_DomainIndex,
                                                                             this->mu32_UseCaseIndex);

            // in domain case pc_UseCase is null but we do not have to update any linked channel too
            if (pc_UseCase != NULL)
            {
               // update data of linked channels
               for (uint32 u32_AvailabilityCounter = 0; u32_AvailabilityCounter < pc_UseCase->c_Availability.size();
                    u32_AvailabilityCounter++)
               {
                  if (pc_UseCase->c_Availability.at(u32_AvailabilityCounter).u32_ValueIndex == this->mu32_ChannelIndex)
                  {
                     const std::vector<stw_types::uint32> & rc_DependentValues =
                        pc_UseCase->c_Availability.at(u32_AvailabilityCounter).c_DependentValues;

                     for (uint32 u32_Counter = 0; u32_Counter < rc_DependentValues.size(); u32_Counter++)
                     {
                        s32_Return = C_PuiSdHandler::h_GetInstance()->SetHALCDomainChannelParameterConfigElement(
                           this->mu32_NodeIndex, this->mu32_DomainIndex, rc_DependentValues.at(u32_Counter),
                           u32_ParameterIndex, u32_ParameterElementIndex, this->mq_ChannelCase, c_NewParameterElement);
                        tgl_assert(s32_Return == C_NO_ERR);
                     }
                  }
               }
            }
            // update data
            if (q_IsParameterElement == true)
            {
               // element case
               s32_Return = C_PuiSdHandler::h_GetInstance()->SetHALCDomainChannelParameterConfigElement(
                  this->mu32_NodeIndex, this->mu32_DomainIndex, this->mu32_ChannelIndex, u32_ParameterIndex,
                  u32_ParameterElementIndex, mq_ChannelCase, c_NewParameterElement);
               tgl_assert(s32_Return == C_NO_ERR);
            }
            else
            {
               // parameter case
               const C_OSCHalcConfigParameterStruct * pc_Parameter =
                  C_PuiSdHandler::h_GetInstance()->GetHALCDomainChannelParameterConfigData(
                     mu32_NodeIndex, mu32_DomainIndex, mu32_ChannelIndex, u32_ParameterIndex, mq_ChannelCase);
               tgl_assert(pc_Parameter != NULL);
               if (pc_Parameter != NULL)
               {
                  // create copy
                  C_OSCHalcConfigParameterStruct c_NewParameter = *pc_Parameter;
                  c_NewParameter.c_Value = c_NewParameterElement.c_Value;
                  s32_Return = C_PuiSdHandler::h_GetInstance()->SetHALCDomainChannelParameterConfig(
                     mu32_NodeIndex, mu32_DomainIndex, mu32_ChannelIndex, u32_ParameterIndex, mq_ChannelCase,
                     c_NewParameter);
                  tgl_assert(s32_Return == C_NO_ERR);
               }
            }
         }
      }
   }
   return q_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get item flags

   \param[in]  orc_Index   Index

   \return
   Item flags
*/
//----------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags C_SdNdeHalcConfigTreeModel::flags(const QModelIndex & orc_Index) const
{
   Qt::ItemFlags c_Retval = C_TblTreModel::flags(orc_Index);

   const C_SdNdeHalcConfigTreeModel::E_Columns e_Col =
      C_SdNdeHalcConfigTreeModel::h_ColumnToEnum(orc_Index.column());

   if ((m_GetParameterElement(orc_Index) != NULL) && (e_Col == eVALUE))
   {
      c_Retval = c_Retval | Qt::ItemIsEditable;
   }

   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Set node index.

   Create all tree children. Displayed data in columns other than the first is handled by data().

   \param[in]  ou32_NodeIndex    Node index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeHalcConfigTreeModel::SetNode(const stw_types::uint32 ou32_NodeIndex)
{
   this->mu32_NodeIndex = ou32_NodeIndex;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Set HALC domain, channel and use case index.

   Create all tree children. Displayed data in columns other than the first is handled by data().

   \param[in]  ou32_DomainIndex     HALC Domain index
   \param[in]  ou32_ChannelIndex    HALC Channel index
   \param[in]  ou32_UseCaseIndex    HALC Use case index
   \param[in]  oq_UseChannelIndex   True: manage channel- and use case specific parameters
                                    False: manage domain parameters standalone
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeHalcConfigTreeModel::SetHalcChannelUseCase(const uint32 ou32_DomainIndex, const uint32 ou32_ChannelIndex,
                                                       const uint32 ou32_UseCaseIndex, const bool oq_UseChannelIndex)
{
   // remember indexes
   this->mu32_DomainIndex = ou32_DomainIndex;
   this->mu32_ChannelIndex = ou32_ChannelIndex;
   this->mu32_UseCaseIndex = ou32_UseCaseIndex;
   this->mq_ChannelCase = oq_UseChannelIndex;

   // TODO SFI: Availability indexes stuff

   // clear
   this->Clear();

   // insert model data
   this->beginResetModel();
   if (this->mpc_InvisibleRootItem == NULL)
   {
      this->mpc_InvisibleRootItem = new C_TblTreItem();
   }

   // get domain
   const C_OSCHalcConfigDomain * pc_Domain =
      C_PuiSdHandler::h_GetInstance()->GetHALCDomainConfigDataConst(mu32_NodeIndex, mu32_DomainIndex);

   if (pc_Domain != NULL)
   {
      const std::vector<C_OSCHalcDefStruct> & rc_Parameters =
         mq_ChannelCase ? pc_Domain->c_ChannelValues.c_Parameters : pc_Domain->c_DomainValues.c_Parameters;

      // create tree items for parameters and elements that are relevant for selected channel and use case resp. domain
      for (uint32 u32_ParamIt = 0; u32_ParamIt < rc_Parameters.size(); u32_ParamIt++)
      {
         const C_OSCHalcDefStruct & rc_Parameter = rc_Parameters[u32_ParamIt];
         C_TblTreItem * const pc_ParameterTreeItem = new C_TblTreItem();
         pc_ParameterTreeItem->c_Name = rc_Parameter.c_Display.c_str();
         pc_ParameterTreeItem->c_ToolTipHeading = pc_ParameterTreeItem->c_Name;
         pc_ParameterTreeItem->c_ToolTipContent = rc_Parameter.c_Comment.c_str();
         pc_ParameterTreeItem->u32_Index = u32_ParamIt;

         // add parameter elements
         for (uint32 u32_ElementIt = 0; u32_ElementIt < rc_Parameter.c_StructElements.size(); u32_ElementIt++)
         {
            const C_OSCHalcDefElement & rc_Element = rc_Parameter.c_StructElements[u32_ElementIt];
            bool q_AvailableForUseCase = false;

            // in channel case: check if element is available for selected use case
            if (mq_ChannelCase == true)
            {
               for (std::vector<stw_types::uint32>::const_iterator c_ItAvail =
                       rc_Element.c_UseCaseAvailabilities.begin();
                    c_ItAvail != rc_Element.c_UseCaseAvailabilities.end(); ++c_ItAvail)
               {
                  if (*c_ItAvail == this->mu32_UseCaseIndex)
                  {
                     q_AvailableForUseCase = true;
                     break;
                  }
               }
            }

            if ((mq_ChannelCase == false) || (q_AvailableForUseCase == true))
            {
               C_TblTreItem * const pc_Child = new C_TblTreItem();
               pc_Child->c_Name = rc_Element.c_Display.c_str();
               pc_Child->c_ToolTipHeading = pc_Child->c_Name;
               pc_Child->c_ToolTipContent = rc_Element.c_Comment.c_str();
               pc_Child->u32_Index = u32_ElementIt;

               // add to tree
               pc_ParameterTreeItem->AddChild(pc_Child);
            }
         }

         // add to tree
         if (this->mpc_InvisibleRootItem != NULL)
         {
            this->mpc_InvisibleRootItem->AddChild(pc_ParameterTreeItem);
            // TODO: only add if at least one child is available for selected use case ?
         }
         else
         {
            delete pc_ParameterTreeItem;
         }
      }
   }

   this->endResetModel();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Clear all children.
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeHalcConfigTreeModel::Clear(void)
{
   if (this->mpc_InvisibleRootItem != NULL)
   {
      std::vector<C_TblTreSimpleItem *>::const_iterator c_ItChildren;

      this->beginResetModel();

      // clear parameter elements (children of children)
      for (c_ItChildren = this->mpc_InvisibleRootItem->c_Children.begin();
           c_ItChildren != this->mpc_InvisibleRootItem->c_Children.end(); ++c_ItChildren)
      {
         if (*c_ItChildren != NULL)
         {
            (*c_ItChildren)->ClearChildren();
         }
      }

      // clear parameter (children)
      this->mpc_InvisibleRootItem->ClearChildren();

      this->endResetModel();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get parameter and parameter element index corresponding to given model index

   \param[in]   orc_Index                    Model index
   \param[out]  oru32_ParameterIndex         Parameter index
   \param[out]  oru32_ParameterElementIndex  Parameter element index (only valid if return = true)

   \retval   true   Model index belongs to a parameter element
   \retval   false  Model index does not belong to any parameter element
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SdNdeHalcConfigTreeModel::mh_GetParameterElementIndexe(const QModelIndex & orc_Index,
                                                              uint32 & oru32_ParameterIndex,
                                                              uint32 & oru32_ParameterElementIndex)
{
   bool q_IsParent = true;

   if (orc_Index.isValid() == true)
   {
      // Decode model index: parent or child?
      //lint -e{925}  Result of Qt interface restrictions, set by index function
      const C_TblTreItem * const pc_CurrentItem = static_cast<const C_TblTreItem * const>(orc_Index.internalPointer());
      if (pc_CurrentItem != NULL)
      {
         //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
         const C_TblTreItem * const pc_Parent = dynamic_cast<const C_TblTreItem * const>(pc_CurrentItem->pc_Parent);
         if ((pc_Parent != NULL) && (pc_Parent->pc_Parent != NULL))
         {
            q_IsParent = false;
            oru32_ParameterIndex = pc_Parent->u32_Index;
            oru32_ParameterElementIndex = pc_CurrentItem->u32_Index;
            // do not use orc_Index.parent().row() because not all parameter elements are visible in tree!
         }
         else
         {
            oru32_ParameterIndex = pc_CurrentItem->u32_Index;
         }
      }
   }

   return !q_IsParent;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get pointer to parameter element data or parameter data.

   Decides if the provided model index belongs to a parameter element or a parameter itself.
   If it does not belong to a parameter struct, i.e. if the parameter has no elements, the data structure of
   the parameter itself is returned.

   Returns NULL if either some configuration is wrong or if the index belongs to a parameter that has elements.

   \param[in]  orc_Index   Model index

   \return
   NULL  no corresponding parameter data found
   else  corresponding parameter data
*/
//----------------------------------------------------------------------------------------------------------------------
const C_OSCHalcConfigParameter * C_SdNdeHalcConfigTreeModel::m_GetParameterElement(const QModelIndex & orc_Index) const
{
   const C_OSCHalcConfigParameter * pc_Return = NULL;

   uint32 u32_ParameterIndex;
   uint32 u32_ParameterElementIndex;
   const bool q_IsParameterElement = mh_GetParameterElementIndexe(orc_Index, u32_ParameterIndex,
                                                                  u32_ParameterElementIndex);
   const C_OSCHalcConfigParameterStruct * pc_Parameter =
      C_PuiSdHandler::h_GetInstance()->GetHALCDomainChannelParameterConfigData(mu32_NodeIndex, mu32_DomainIndex,
                                                                               mu32_ChannelIndex, u32_ParameterIndex,
                                                                               mq_ChannelCase);

   if (pc_Parameter != NULL)
   {
      if (q_IsParameterElement == true)
      {
         // case: model index belongs to parameter struct element
         if (u32_ParameterElementIndex < pc_Parameter->c_ParameterElements.size())
         {
            pc_Return = &pc_Parameter->c_ParameterElements[u32_ParameterElementIndex];
         }
      }
      else
      {
         // case: model index belongs to parameter itself
         if (pc_Parameter->c_ParameterElements.empty() == true)
         {
            pc_Return = pc_Parameter;
         }
      }
   }

   return pc_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get pointer to parameter element definition data or parameter definition data.

   Decides if the provided model index belongs to a parameter element or a parameter itself.
   If it does not belong to a parameter struct, i.e. if the parameter has no elements, the data structure of
   the parameter itself is returned.

   Returns NULL if either some configuration is wrong or if the index belongs to a parameter that has elements.

   \param[in]  orc_Index   Model index

   \return
   NULL  no corresponding parameter definition data found
   else  corresponding parameter definition data
*/
//----------------------------------------------------------------------------------------------------------------------
const C_OSCHalcDefElement * C_SdNdeHalcConfigTreeModel::m_GetDefParameterElement(const QModelIndex & orc_Index) const
{
   const C_OSCHalcDefElement * pc_Return = NULL;

   uint32 u32_ParameterIndex;
   uint32 u32_ParameterElementIndex;
   const bool q_IsParameterElement = mh_GetParameterElementIndexe(orc_Index, u32_ParameterIndex,
                                                                  u32_ParameterElementIndex);
   const C_OSCHalcDefStruct * pc_DefElement =
      C_PuiSdHandler::h_GetInstance()->GetHALCDomainFileVariableData(mu32_NodeIndex, mu32_DomainIndex,
                                                                     C_OSCHalcDefDomain::eVA_PARAM,
                                                                     u32_ParameterIndex);

   if (pc_DefElement != NULL)
   {
      if (q_IsParameterElement == true)
      {
         // case: model index belongs to parameter struct element
         if (u32_ParameterElementIndex < pc_DefElement->c_StructElements.size())
         {
            pc_Return = &pc_DefElement->c_StructElements[u32_ParameterElementIndex];
         }
      }
      else
      {
         if (pc_DefElement->c_StructElements.empty() == true)
         {
            // case: model index belongs to parameter itself
            pc_Return = pc_DefElement;
         }
      }
   }

   return pc_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Convert enum names to string list

   \param[in]  orc_Value    HALC Def Content value of type enum

   \return
   String list of enum value names.
*/
//----------------------------------------------------------------------------------------------------------------------
QStringList C_SdNdeHalcConfigTreeModel::mh_ConvertEnumsToStringList(const C_OSCHalcDefContent & orc_Value)
{
   QStringList c_Return;

   const std::map<stw_scl::C_SCLString, C_OSCNodeDataPoolContent> & rc_EnumItems = orc_Value.GetEnumItems();

   for (std::map<stw_scl::C_SCLString, C_OSCNodeDataPoolContent>::const_iterator c_It = rc_EnumItems.begin();
        c_It != rc_EnumItems.end(); ++c_It)
   {
      c_Return.append(c_It->first.c_str());
   }

   return c_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Column to enum conversion

   \param[in]  ors32_Column   Column

   \return
   Enum value
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeHalcConfigTreeModel::E_Columns C_SdNdeHalcConfigTreeModel::h_ColumnToEnum(const sint32 & ors32_Column)
{
   C_SdNdeHalcConfigTreeModel::E_Columns e_Retval;

   switch (ors32_Column)
   {
   case 0:
      e_Retval = eNAME;
      break;
   case 1:
      e_Retval = eVALUE;
      break;
   case 2:
      e_Retval = eDESCRIPTION;
      break;
   default:
      e_Retval = eNAME;
      break;
   }

   return e_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Enum to column conversion

   \param[in]  ore_Value   Enum value

   \return
   Column
   -1 Error
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_SdNdeHalcConfigTreeModel::h_EnumToColumn(const C_SdNdeHalcConfigTreeModel::E_Columns & ore_Value)
{
   sint32 s32_Retval;

   switch (ore_Value)
   {
   case eNAME:
      s32_Retval = 0;
      break;
   case eVALUE:
      s32_Retval = 1;
      break;
   case eDESCRIPTION:
      s32_Retval = 2;
      break;
   default:
      s32_Retval = -1;
      break;
   }

   return s32_Retval;
}
