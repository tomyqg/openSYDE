//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Dialog for code generation report (header)

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_IMPCODEGENERATIONREPORTWIDGET_H
#define C_IMPCODEGENERATIONREPORTWIDGET_H

#include <QWidget>
#include "stwtypes.h"
#include "C_OgePopUpDialog.h"

namespace Ui
{
class C_ImpCodeGenerationReportWidget;
}
/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */
class C_ImpCodeGenerationReportWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_ImpCodeGenerationReportWidget(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent);
   ~C_ImpCodeGenerationReportWidget(void);

   class C_ReportData ///< Class to put together all report-relevant information of one Data Block
   {
   public:
      QString c_NodeName;                ///< Node name where the Data Block belongs to
      QString c_AppName;                 ///< Data Block name
      QString c_CodeGeneratorPath;       ///< Path of used code generator
      stw_types::uint16 u16_CodeVersion; ///< Path of used code generator
      QString c_Directory;               ///< Directory where generated files were saved at
      QStringList c_GeneratedFiles;      ///< List absolute file paths of all generated files
   };

   void InitStaticNames(void) const;
   void CreateReport(const std::vector<C_ReportData> & orc_ExportInfo) const;

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void keyPressEvent(QKeyEvent * const opc_KeyEvent) override;
   //lint -restore

private:
   Ui::C_ImpCodeGenerationReportWidget * mpc_Ui;
   //lint -e{1725} Only problematic if copy or assignment is allowed
   stw_opensyde_gui_elements::C_OgePopUpDialog & mrc_ParentDialog;

   static const QString mhc_HTML_TABLE_DATA_START;

   void m_OkClicked(void);

   //Avoid call
   C_ImpCodeGenerationReportWidget(const C_ImpCodeGenerationReportWidget &);
   C_ImpCodeGenerationReportWidget & operator =(const C_ImpCodeGenerationReportWidget &);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
