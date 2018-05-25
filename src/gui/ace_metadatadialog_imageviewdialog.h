#ifndef ACE_METADATADIALOG_IMAGEVIEWDIALOG_H
#define ACE_METADATADIALOG_IMAGEVIEWDIALOG_H
#include "ace_metadatadialog.h"



class QLabel;
//



namespace Ace
{
   /*!
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
       */
      MetadataDialog* _parent;
      /*!
       */
      const QModelIndex& _index;
      /*!
       */
      QPixmap* _image;
      /*!
       */
      QLabel* _view;
   };
}



#endif
