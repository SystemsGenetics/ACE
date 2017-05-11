#ifndef OPENCLDEVICEDIALOG_H
#define OPENCLDEVICEDIALOG_H
#include <QDialog>
#include <QtCore>



class QTreeView;
class QLabel;
class QPushButton;



namespace Ace {



class OpenCLDeviceModel;



class OpenCLDeviceDialog : public QDialog
{
   Q_OBJECT
public:
   static OpenCLDeviceDialog& getInstance();
private slots:
   void updateDetails(const QModelIndex& index);
   void refresh();
   void cancel();
   void set();
private:
   explicit OpenCLDeviceDialog(QWidget *parent = 0);
   static OpenCLDeviceDialog* _instance;
   QTreeView* _list;
   QPushButton* _refresh;
   QLabel* _details;
   QPushButton* _set;
   QPushButton* _cancel;
   OpenCLDeviceModel* _model;
   QModelIndex _selected;
};



}
#endif
