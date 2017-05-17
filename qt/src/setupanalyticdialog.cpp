#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSignalMapper>

#include "setupanalyticdialog.h"
#include "abstractanalytic.h"






Ace::SetupAnalyticDialog::SetupAnalyticDialog(EAbstractAnalytic* analytic, QWidget* parent):
   QDialog(parent),
   _analytic(analytic)
{
   setLayout(createInputs());
}






Ace::SetupAnalyticDialog::~SetupAnalyticDialog()
{
}






void Ace::SetupAnalyticDialog::findFile(int argument)
{
   using Type = EAbstractAnalytic::ArgumentType;
   switch (_analytic->getArgumentType(argument))
   {
   case Type::FileIn:
   case Type::FileOut:
   case Type::DataObjectIn:
   case Type::DataObjectOut:
   default:
      return;
   }
}






void Ace::SetupAnalyticDialog::cancel()
{
}






void Ace::SetupAnalyticDialog::executeButton()
{
}






QGridLayout* Ace::SetupAnalyticDialog::createInputs()
{
   using Type = EAbstractAnalytic::ArgumentType;
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
      case Type::Integer:
      {
         QRegExp exp;//TODO regular expression for integers
         _inputs.append(new QLineEdit);
         formLayout->addWidget(_inputs.back(),i,1,Qt::AlignLeft);
         break;
      }
      case Type::Float:
      case Type::Double:
      {
         QRegExp exp;//TODO regular expression for floating point numbers
         _inputs.append(new QLineEdit);
         formLayout->addWidget(_inputs.back(),i,1,Qt::AlignLeft);
         break;
      }
      case Type::String:
      {
         _inputs.append(new QLineEdit);
         formLayout->addWidget(_inputs.back(),i,1,Qt::AlignLeft);
         break;
      }
      case Type::FileIn:
      case Type::FileOut:
      case Type::DataObjectIn:
      case Type::DataObjectOut:
      {
         _inputs.append(new QLineEdit);
         formLayout->addWidget(_inputs.back(),i,1,Qt::AlignCenter);
         QPushButton* findFileButton = new QPushButton(tr("Browse"));
         formLayout->addWidget(findFileButton,i,2,Qt::AlignLeft);
         connect(findFileButton,SIGNAL(clicked(bool)),mapper,SLOT(map()));
         mapper->setMapping(findFileButton,i);
         break;
      }
      }
   }
   return formLayout;
}
