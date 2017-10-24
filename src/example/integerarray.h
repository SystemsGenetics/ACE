#ifndef DATA_H
#define DATA_H
#include <core/AceCore.h>



// Data type that implements the Integer type from factory
//
// This data object stores a one dimensional array of integer values
class IntegerArray : public EAbstractData
{
public:
   IntegerArray();
   ~IntegerArray();
   void readData() override final;
   quint64 getDataEnd() const override final;
   void newData() override final;
   void prepare(bool preAllocate) override final;
   void finish() override final;
   virtual QAbstractTableModel* getModel() override final;
   // This is a contrived example, data should be private and iterator classes defined to access
   // it
   class Model;
   QList<int> _numbers;
   int _futureSize {0};
   Model* _model;
};



// Qt model class used to represent the data of an integer array
class IntegerArray::Model : public QAbstractTableModel
{
   Q_OBJECT
public:
   explicit Model(QList<int>* root, QObject* parent = nullptr):
      QAbstractTableModel(parent), _root(root) {}
   Model(const Model&) = delete;
   Model(Model&&) = delete;
   QVariant headerData(int section, Qt::Orientation orientation, int role) const;
   int rowCount(const QModelIndex& parent) const override final;
   int columnCount(const QModelIndex& parent) const override final;
   QVariant data(const QModelIndex& index, int role) const override final;
private:
   QList<int>* _root {nullptr};
};



#endif
