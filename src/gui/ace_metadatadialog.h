#ifndef ACE_METADATADIALOG_H
#define ACE_METADATADIALOG_H
#include <QDialog>
#include <QModelIndex>
#include "../core/ace.h"



class QTreeView;
class QMenu;
//



namespace Ace
{
   /*!
    * This is the metadata dialog. This dialog allows the user to view and possibly 
    * edit the metadata of a data object. If this is the user metadata this dialog 
    * allows editing of it. If this is system metadata then this dialog is read only. 
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
       * Pointer to the metadata model for this dialog. 
       */
      MetadataModel* _model;
      /*!
       * True if this dialog is showing the system metadata or false if this is editing 
       * the user metadata. 
       */
      bool _system;
      /*!
       * Pointer to the data object this dialog uses to read and possibly write metadata. 
       */
      DataObject* _data;
      /*!
       * Pointer to the tree view that displays the metadata model for this dialog. 
       */
      QTreeView* _view;
      /*!
       * The last model index that was selected by the mouse in this dialog. 
       */
      QModelIndex _lastIndex;
      /*!
       * List of add actions for every possible metadata type for this dialog. 
       */
      QList<QAction*> _addActions;
      /*!
       * The remove action for this dialog. 
       */
      QAction* _removeAction;
      /*!
       * The set copy action for this dialog. This action sets the drag and drop action 
       * to copy. 
       */
      QAction* _setCopyAction;
      /*!
       * The set move action for this dialog. This action sets the drag and drop action 
       * to move. 
       */
      QAction* _setMoveAction;
      /*!
       * The custom context menu for this dialog used with its tree view. 
       */
      QMenu* _contextMenu;
      /*!
       * The add menu for this dialog containing all add actions for new metadata 
       * objects. 
       */
      QMenu* _addMenu;
   };
}



#endif
