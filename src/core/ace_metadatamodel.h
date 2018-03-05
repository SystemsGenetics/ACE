#ifndef ACE_METADATAMODEL_H
#define ACE_METADATAMODEL_H
#include <QAbstractItemModel>



namespace Ace
{
   /*!
    * This provides a Qt item model for a metadata structure. The root of the 
    * metadata structure must be an object type. This model is primarily used for 
    * the GUI dialog to edit the metadata of a data object. It can however be used 
    * as a general model for any purpose. The model can insert or remove a row, and 
    * drag and drop a single row that either copies or pastes the row. It consists 
    * of three columns. The first column is the name, the second is the type, and 
    * the last column is the value. 
    */
   class MetadataModel : public QAbstractItemModel
   {
      Q_OBJECT
   public:
   private:
      class Node;
      /*!
       * The root node of the metadata this model contains. The node must be the 
       * metadata object type. If the model contains no metadata then this is null. 
       */
      Node* _root;
   };
}



#endif
