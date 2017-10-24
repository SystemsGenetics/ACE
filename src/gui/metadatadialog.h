#ifndef METADATADIALOG_H
#define METADATADIALOG_H
#include <QDialog>
#include <QtCore>



class QTreeView;
class QMenu;



namespace Ace
{
class MetadataModel;
class DataObject;



class MetadataDialog : public QDialog
{
   Q_OBJECT
public:
   explicit MetadataDialog(DataObject* data, QWidget* parent = nullptr);
private slots:
   void metadataContextMenuRequested(const QPoint& point);
   void addTriggered();
   void removeTriggered();
   void setCopyTriggered();
   void setMoveTriggered();
   void applyClicked();
   void indexDoubleClicked(const QModelIndex& index);
   virtual void accept() override final;
   virtual void reject() override final;
private:
   void createActions();
   void createMenus();
   MetadataModel* _model;
   DataObject* _data;
   QTreeView* _view;
   QModelIndex _lastIndex;
   QList<QAction*> _addActions;
   QAction* _removeAction;
   QAction* _setCopyAction;
   QAction* _setMoveAction;
   QMenu* _mainMenu;
   QMenu* _addMenu;
   QMenu* _dragMenu;
};
}



#endif
