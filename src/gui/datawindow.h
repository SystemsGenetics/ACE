#ifndef DATAWINDOW_H
#define DATAWINDOW_H
#include <QMainWindow>



namespace Ace {
class DataReference;



class DataWindow : public QMainWindow
{
   Q_OBJECT
public:
   DataWindow(Ace::DataReference* data, QWidget *parent = nullptr);
   ~DataWindow();
   void setWindowTitle(const QString& title);
private slots:
   void metadataTriggered();
private:
   void createActions();
   void createMenus();
   Ace::DataReference* _data;
   QMenu* _fileMenu;
   QAction* _metadataAction;
   QAction* _closeAction;
   QString _title;
};
}



#endif
