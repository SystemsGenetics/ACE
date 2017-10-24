#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "imageviewer.h"
#include "metadatamodel.h"



using namespace Ace;






ImageViewer::ImageViewer(MetadataModel* model, const QModelIndex& index, QWidget* parent):
   QDialog(parent),
   _model(model),
   _index(index)
{
   // initialize pixmap and grab raw image byte array from model
   _image = new QPixmap;
   QByteArray data = _model->data(index,MetadataModel::RawImageData).toByteArray();

   // load image if byte array is not empty, else load default empty image logo
   if ( !data.isEmpty() )
   {
      _image->loadFromData(data,"PNG");
   }
   else
   {
      _image->load(":/images/null_image.png");
   }

   // create all buttons
   _loadNew = new QPushButton(tr("&Load New Image"));
   _save = new QPushButton(tr("&Save to File"));
   _close = new QPushButton(tr("&Close"));

   // create label that displays pixmap
   _view = new QLabel;
   _view->setPixmap(*_image);

   // create main layout and add label and buttons
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_view);
   layout->addWidget(_loadNew);
   layout->addWidget(_save);
   layout->addWidget(_close);
   setLayout(layout);

   // connect all signals
   connect(_loadNew,SIGNAL(clicked(bool)),this,SLOT(loadNewClicked()));
   connect(_save,SIGNAL(clicked(bool)),this,SLOT(saveClicked()));
   connect(_close,SIGNAL(clicked(bool)),this,SLOT(reject()));

   // set window's title
   setWindowTitle(tr("Image Viewer"));
}






void ImageViewer::loadNewClicked()
{
   // query user for image file to load from
   QString file = QFileDialog::getOpenFileName(this,tr("Open Image File"),QString()
                                               ,tr("Images (*.png *.xpm *.jpg)"));
   if ( !file.isEmpty() )
   {
      // if user selected file attempt to load it
      if ( _image->load(file) )
      {
         // if image was loaded successfully update label and load new byte array data to model
         _view->setPixmap(*_image);
         QByteArray data;
         QBuffer buffer(&data);
         buffer.open(QIODevice::WriteOnly);
         _image->save(&buffer,"PNG");
         _model->setData(_index,data,Qt::EditRole);
      }
      else
      {
         // if image failed to load tell user
         QMessageBox::warning(this,tr("Failed Opening Image")
                              ,tr("Failed opening image from file %1.").arg(file));
      }
   }
}






void ImageViewer::saveClicked()
{
   // query user for file to save image to
   QString file = QFileDialog::getSaveFileName(this,tr("Save Image File"),QString()
                                               ,tr("Images (*.png *.xpm *.jpg)"));
   if ( !file.isEmpty() )
   {
      // if user selected file attempt to save it, reporting the to the user any failure
      if ( !_image->save(file) )
      {
         QMessageBox::warning(this,tr("Failed Saving Image")
                              ,tr("Failed saving image to file %1.").arg(file));
      }
   }
}
