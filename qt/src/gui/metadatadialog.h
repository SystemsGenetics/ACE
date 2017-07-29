#ifndef METADATADIALOG_H
#define METADATADIALOG_H
#include <QDialog>
#include <QtCore>



class QTreeView;
class QMenu;



namespace Ace
{
class MetadataModel;



class MetadataDialog : public QDialog
{
   Q_OBJECT
public:
   explicit MetadataDialog(MetadataModel* model, QWidget* parent = nullptr);
private slots:
   void metadataContextMenuRequested(const QPoint& point);
   void addTriggered();
private:
   void createActions();
   void createMenus();
   MetadataModel* _model;
   QTreeView* _view;
   QModelIndex _lastIndex;
   QList<QAction*> _addActions;
   QMenu* _mainMenu;
   QMenu* _addMenu;
};
}



#endif
