#include "ace_metadatadialog_imageviewdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include "../core/ace_metadatamodel.h"



using namespace Ace;
//






/*!
 * Constructs a new image view dialog with the given model index and parent. 
 *
 * @param index The model index where the metadata image this new dialog uses is 
 *              located. 
 *
 * @param parent Pointer to the parent metadata dialog that opens this new image 
 *               view dialog. 
 */
MetadataDialog::ImageViewDialog::ImageViewDialog(const QModelIndex& index, MetadataDialog* parent):
   _parent(parent),
   _index(index)
{
   // .
   _image = new QPixmap;
   QByteArray data = _parent->_model->data(index,MetadataModel::RawImageData).toByteArray();

   // .
   if ( !data.isEmpty() )
   {
      // .
      _image->loadFromData(data,"PNG");
   }

   // .
   else
   {
      // .
      _image->load(":/images/null_image.png");
   }

   // .
   _view = new QLabel;
   _view->setPixmap(*_image);

   // .
   QPushButton* loadNew {new QPushButton(tr("&Load New Image"))};
   loadNew->setDisabled(_parent->_system);
   QPushButton* save {new QPushButton(tr("&Save to File"))};
   QPushButton* close {new QPushButton(tr("&Close"))};

   // .
   connect(loadNew,&QPushButton::clicked,this,&ImageViewDialog::loadNewClicked);
   connect(save,&QPushButton::clicked,this,&ImageViewDialog::saveClicked);
   connect(close,&QPushButton::clicked,this,&QDialog::reject);

   // .
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_view);
   layout->addWidget(loadNew);
   layout->addWidget(save);
   layout->addWidget(close);

   // .
   setLayout(layout);
   setWindowTitle(tr("Image Viewer"));
}






/*!
 * Called when the load new button is clicked. This opens a file selection dialog 
 * to load a new image for the metadata image this dialog contains, loading it if a 
 * new image was selected by the user. 
 */
void MetadataDialog::ImageViewDialog::loadNewClicked()
{
   // .
   QString file = QFileDialog::getOpenFileName(this,tr("Open Image File"),QString()
                                               ,tr("Images (*.png *.xpm *.jpg)"));

   // .
   if ( file.isEmpty() )
   {
      return;
   }

   // .
   if ( !_image->load(file) )
   {
      // .
      QMessageBox::warning(this,tr("Failed Opening Image")
                           ,tr("Failed opening image from file %1.").arg(file));
      return;
   }

   // .
   _view->setPixmap(*_image);

   // .
   QByteArray data;
   QBuffer buffer(&data);
   buffer.open(QIODevice::WriteOnly);
   _image->save(&buffer,"PNG");
   _parent->_model->setData(_index,data,Qt::EditRole);
}






/*!
 * Called when the save button is clicked. This opens a file selection dialog for 
 * where to save the image of this dialog and then saves it. 
 */
void MetadataDialog::ImageViewDialog::saveClicked()
{
   // .
   QString file = QFileDialog::getSaveFileName(this,tr("Save Image File"),QString()
                                               ,tr("Images (*.png *.xpm *.jpg)"));

   // .
   if ( file.isEmpty() )
   {
      return;
   }

   // .
   if ( !_image->save(file) )
   {
      // .
      QMessageBox::warning(this,tr("Failed Saving Image")
                           ,tr("Failed saving image to file %1.").arg(file));
   }
}
