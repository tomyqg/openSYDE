//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Dialog for export report to DBC file (header)

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_CIEEXPORTREPORTWIDGET_H
#define C_CIEEXPORTREPORTWIDGET_H

#include <QWidget>
#include "stwtypes.h"
#include "C_OgePopUpDialog.h"
#include "C_CieExportDbc.h"

namespace Ui
{
class C_CieExportReportWidget;
}
/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */
class C_CieExportReportWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_CieExportReportWidget(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent, QString & orc_FilePath);
   ~C_CieExportReportWidget(void);

   void InitStaticNames(void) const;
   stw_types::sint32 SetMessageData(const std::map<stw_scl::C_SCLString, stw_scl::C_SCLString> & orc_NodeMapping,
                                    const stw_opensyde_gui_logic::C_CieExportDbc::C_ExportStatistic & orc_ExportStatistic, const stw_scl::C_SCLStringList & orc_Warnings);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void keyPressEvent(QKeyEvent * const opc_KeyEvent) override;
   //lint -restore

private:
   Ui::C_CieExportReportWidget * mpc_Ui;
   //lint -e{1725} Only problematic if copy or assignment is allowed
   stw_opensyde_gui_elements::C_OgePopUpDialog & mrc_ParentDialog;
   std::map<stw_scl::C_SCLString, stw_scl::C_SCLString> mc_NodeMapping;
   stw_opensyde_gui_logic::C_CieExportDbc::C_ExportStatistic mc_ExportStatistic;
   stw_scl::C_SCLStringList mc_Warnings;
   const QString mc_FilePath;
   static const QString mhc_HTML_TABLE_HEADER_START;
   static const QString mhc_HTML_TABLE_DATA_START;

   void m_OkClicked(void);
   stw_types::sint32 m_BuildReport(void);

   //Avoid call
   C_CieExportReportWidget(const C_CieExportReportWidget &);
   C_CieExportReportWidget & operator =(const C_CieExportReportWidget &);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
