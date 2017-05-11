#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>

#include "opencldevicedialog.h"
#include "opencldevicemodel.h"
#include "opencldevice.h"
#include "exception.h"
namespace Ace {






OpenCLDeviceDialog* OpenCLDeviceDialog::_instance = nullptr;






OpenCLDeviceDialog &OpenCLDeviceDialog::getInstance()
{
   if ( !_instance )
   {
      _instance = new OpenCLDeviceDialog;
   }
   return *_instance;
}






void OpenCLDeviceDialog::updateDetails(const QModelIndex &index)
{
   try
   {
      _details->setText(_model->getDetailedInfo(index));
      _set->setDisabled(!_model->isDevice(index));
      _selected = index;
   }
   catch (EException e)
   {
      e.display();
      exit(1);
   }
}






void OpenCLDeviceDialog::refresh()
{
   try
   {
      _model->reset();
      _details->setText("");
      _set->setDisabled(true);
   }
   catch (EException e)
   {
      e.display();
      exit(1);
   }
}






void OpenCLDeviceDialog::cancel()
{
   hide();
}






void OpenCLDeviceDialog::set()
{
   try
   {
      _model->setDevice(_selected);
      hide();
   }
   catch (EException e)
   {
      e.display();
      exit(1);
   }
}






OpenCLDeviceDialog::OpenCLDeviceDialog(QWidget *parent):
   QDialog(parent)
{
   // initialize view and model for opencl devices
   _list = new QTreeView;
   _list->setHeaderHidden(true);
   _model = new OpenCLDeviceModel;
   _list->setModel(_model);
   connect(_list,SIGNAL(pressed(QModelIndex)),this,SLOT(updateDetails(QModelIndex)));

   // initialize opencl platform/device details label
   _details = new QLabel;
   _details->setWordWrap(true);
   _details->setAlignment(Qt::AlignLeft);
   _details->setIndent(16);

   // initialize refresh, cancel, and set buttons
   _refresh = new QPushButton(tr("&Refresh"));
   _set = new QPushButton(tr("&Set"));
   _set->setDisabled(true);
   _cancel = new QPushButton(tr("&Cancel"));
   connect(_refresh,SIGNAL(clicked(bool)),this,SLOT(refresh()));
   connect(_cancel,SIGNAL(clicked(bool)),this,SLOT(cancel()));
   connect(_set,SIGNAL(clicked(bool)),this,SLOT(set()));

   // build upper part of dialog
   QSplitter* upper = new QSplitter(Qt::Horizontal);
   upper->addWidget(_list);
   upper->addWidget(_details);

   // build lower part of dialog
   QHBoxLayout* lower = new QHBoxLayout;
   lower->addWidget(_refresh);
   lower->addStretch();
   lower->addWidget(_cancel);
   lower->addWidget(_set);

   // add upper and lower to main layout
   QVBoxLayout* main = new QVBoxLayout;
   main->addWidget(upper);
   main->addLayout(lower);

   // set layout to main and set title
   setLayout(main);
   setWindowTitle(tr("OpenCL Device Selection"));
}



}
