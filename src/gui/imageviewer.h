#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <QDialog>



class QLabel;



namespace Ace
{
class MetadataModel;



class ImageViewer : public QDialog
{
   Q_OBJECT
public:
   ImageViewer(MetadataModel* model, const QModelIndex& index, QWidget* parent);
private slots:
   void loadNewClicked();
   void saveClicked();
private:
   MetadataModel* _model;
   const QModelIndex& _index;
   QPixmap* _image;
   QLabel* _view;
   QPushButton* _loadNew;
   QPushButton* _save;
   QPushButton* _close;
};
}



#endif
