#ifndef ACE_METADATADIALOG_H
#define ACE_METADATADIALOG_H
#include <QDialog>
#include <QModelIndex>
#include <../core/ace.h>



class QTreeView;
class QMenu;
//



namespace Ace
{
   /*!
    */
   class MetadataDialog : public QDialog
   {
      Q_OBJECT
   public:
      explicit MetadataDialog(DataObject* data, bool system = true);
   private slots:
      void addTriggered(int type);
      void removeTriggered();
      void setCopyTriggered();
      void setMoveTriggered();
      void okClicked();
      void applyClicked();
      void indexDoubleClicked(const QModelIndex& index);
      void contextMenuRequested(const QPoint& point);
   private:
      class ImageViewDialog;
      void createActions();
      void createMenu();
      /*!
       */
      MetadataModel* _model;
      /*!
       */
      bool _system;
      /*!
       */
      DataObject* _data;
      /*!
       */
      QTreeView* _view;
      /*!
       */
      QModelIndex _lastIndex;
      /*!
       */
      QList<QAction*> _addActions;
      /*!
       */
      QAction* _removeAction;
      /*!
       */
      QAction* _setCopyAction;
      /*!
       */
      QAction* _setMoveAction;
      /*!
       */
      QMenu* _contextMenu;
      /*!
       */
      QMenu* _addMenu;
   };
}



#endif
