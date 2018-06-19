#include "ace_metadatadialog_imageviewdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <../core/ace_metadatamodel.h>



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
   // Create a new pixmap and load the image byte array from the given model index.
   _image = new QPixmap;
   QByteArray data = _parent->_model->data(index,MetadataModel::RawImageData).toByteArray();

   // Make sure the byte array is not empty.
   if ( !data.isEmpty() )
   {
      // Load the byte array as a PNG image into the pixmap.
      _image->loadFromData(data,"PNG");
   }

   // Else the byte array is empty.
   else
   {
      // Load a default image representing empty into the pixmap.
      _image->load(":/images/null_image.png");
   }

   // Create a new label and set its pixmap.
   _view = new QLabel;
   _view->setPixmap(*_image);

   // Create the buttons for this new dialog, disabling the load new button if this parent's
   // dialog contains system metadata.
   QPushButton* loadNew {new QPushButton(tr("&Load New Image"))};
   loadNew->setDisabled(_parent->_system);
   QPushButton* save {new QPushButton(tr("&Save to File"))};
   QPushButton* close {new QPushButton(tr("&Close"))};

   // Connect all button clicked signals.
   connect(loadNew,&QPushButton::clicked,this,&ImageViewDialog::loadNewClicked);
   connect(save,&QPushButton::clicked,this,&ImageViewDialog::saveClicked);
   connect(close,&QPushButton::clicked,this,&QDialog::reject);

   // Create a new vertical layout, adding the pixmap containing label and all buttons.
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_view);
   layout->addWidget(loadNew);
   layout->addWidget(save);
   layout->addWidget(close);

   // Set the layout of this dialog and its window title.
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
   // Get the user to select an image file to load.
   QString file = QFileDialog::getOpenFileName(this,tr("Open Image File"),QString()
                                               ,tr("Images (*.png *.xpm *.jpg)"));

   // If the user did not select an image file then return.
   if ( file.isEmpty() )
   {
      return;
   }

   // Load the image file into this object's pixmap, making sure it worked.
   if ( !_image->load(file) )
   {
      // Inform the user loading the image failed and return.
      QMessageBox::warning(this,tr("Failed Opening Image")
                           ,tr("Failed opening image from file %1.").arg(file));
      return;
   }

   // Update this object's pixmap label.
   _view->setPixmap(*_image);

   // Save the new image as a byte array and then update the underlying metadata object with
   // the new byte array.
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
   // Get teh user to select a location for saving the image of this dialog.
   QString file = QFileDialog::getSaveFileName(this,tr("Save Image File"),QString()
                                               ,tr("Images (*.png *.xpm *.jpg)"));

   // If the user did not select a location then return.
   if ( file.isEmpty() )
   {
      return;
   }

   // Save the image of this dialog, making sure it worked.
   if ( !_image->save(file) )
   {
      // Inform the user saving the image failed.
      QMessageBox::warning(this,tr("Failed Saving Image")
                           ,tr("Failed saving image to file %1.").arg(file));
   }
}
