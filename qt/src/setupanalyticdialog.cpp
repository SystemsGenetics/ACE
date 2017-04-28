#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSignalMapper>

#include "setupanalyticdialog.h"
#include "abstractanalytic.h"






SetupAnalyticDialog::SetupAnalyticDialog(AbstractAnalytic* analytic, QWidget* parent):
   QDialog(parent),
   _analytic(analytic)
{
   setLayout(createInputs());
}






SetupAnalyticDialog::~SetupAnalyticDialog()
{
}






void SetupAnalyticDialog::findFile(int /*argument*/)
{
}






void SetupAnalyticDialog::cancel()
{
}






void SetupAnalyticDialog::executeButton()
{
}






QGridLayout* SetupAnalyticDialog::createInputs()
{
   QSignalMapper* mapper = new QSignalMapper(this);
   connect(mapper,SIGNAL(mapped(int)),this,SLOT(findFile(int)));
   QGridLayout* formLayout = new QGridLayout;
   for (int i = 0; i < _analytic->getArgumentCount() ;++i)
   {
      QString title = _analytic->getArgumentTitle(1);
      title.append(":");
      QLabel* label = new QLabel(title);
      formLayout->addWidget(label,i,0,Qt::AlignRight);
      switch (_analytic->getArgumentType(i))
      {
      case AbstractAnalytic::ArgumentType::Integer:
         break;
      case AbstractAnalytic::ArgumentType::Float:
      case AbstractAnalytic::ArgumentType::Double:
         break;
      case AbstractAnalytic::ArgumentType::String:
         break;
      case AbstractAnalytic::ArgumentType::File:
      case AbstractAnalytic::ArgumentType::DataObjectFileIn:
      case AbstractAnalytic::ArgumentType::DataObjectFileOut:
      {
         QVBoxLayout* fileBox = new QVBoxLayout;
         _inputs.append(new QLineEdit);
         fileBox->addWidget(_inputs.back());
         QPushButton* findFileButton = new QPushButton(tr("Browse"));
         fileBox->addWidget(findFileButton);
         formLayout->addLayout(fileBox,i,1,Qt::AlignLeft);
         connect(findFileButton,SIGNAL(clicked(bool)),mapper,SLOT(map()));
         mapper->setMapping(findFileButton,i);
         break;
      }
      }
   }
   return formLayout;
}
