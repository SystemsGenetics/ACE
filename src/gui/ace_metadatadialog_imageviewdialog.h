#ifndef ACE_METADATADIALOG_IMAGEVIEWDIALOG_H
#define ACE_METADATADIALOG_IMAGEVIEWDIALOG_H
#include "ace_metadatadialog.h"



class QLabel;
//



namespace Ace
{
   /*!
    * This is the image view dialog. It views an image of a metadata object that is of 
    * the byte type. The image can be saved to a separate file. If the metadata is 
    * part of the user metadata a new image can also be loaded in. 
    */
   class MetadataDialog::ImageViewDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit ImageViewDialog(const QModelIndex& index, MetadataDialog* parent);
   private slots:
      void loadNewClicked();
      void saveClicked();
   private:
      /*!
       * Pointer to the parent metadata dialog of this dialog where the metadata object 
       * of its image is stored. 
       */
      MetadataDialog* _parent;
      /*!
       * The model index for this object's parent's model where the image this dialog 
       * displays is stored. 
       */
      const QModelIndex& _index;
      /*!
       * The pixel map used to display the metadata image of this dialog. 
       */
      QPixmap* _image;
      /*!
       * The label that contains the pixel map which displays the metadata image of this 
       * dialog. 
       */
      QLabel* _view;
   };
}



#endif
