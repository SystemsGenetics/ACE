#Summary

This document introduces the framework of the ACE library by providing a basic tutorial mimicking the classic hello world of programming introductions. It starts by creating a hello world data class, followed by a hello world analytic class, and finishes by wrapping it all together and including it in the ACE plugin factory object.

**Table of Conents**

[TOC]

#Data Class

Creating a data class is the first thing you should do when implementing a new idea when using the ACE library since analytic objects depend on and create new data objects. Our HelloWorld data object will be responsible with holding the value of a 32 bit integer.

First we will create a skeleton class which will compile, but does absolutely nothing useful.

```
#include <ace/core/core.h>



class HelloWorld : public EAbstractData
{
public:
   virtual qint64 dataEnd() const override final;
   virtual void readData() override final;
   virtual void writeNewData() override final;
   virtual QAbstractTableModel* model() override final;
   virtual void finish() override final;
   qint32 get();
   void put(quint32 value);
}; 
```
This skeleton inherits the required abstract data class from ACE which is the abstract class that defines a data object. The virtual functions that must be implemented from this class are dataEnd(), readData(), writeNewData(), and model(). The finish() function is optional and does not need to be overriden.

The dataEnd() method is called to determine the end of the data within this data object's file stream. The readData() method is called once when a data object file is opened and should read in its data from its data object file stream. The writeNewData() method is called once to initialize a new and empty data object file and should initialize its data object file stream. The model() method is called to get a pointer to a qt abstract table model that represents the data of its data object. The optional finish() method is called once for a new data object file once its analytic is finished running and is about to close.

A data class should have additional methods used by analytics for accessing and setting its data. These methods are not needed for the ACE platform to do its job though, and is left to the data class declaring as regular methods that its fellow analytics should be aware of. In our example data object these methods are get() and put().

##Protected Methods

To facilitate access to a data object's underlying file and metadata system a number of protected methods is provided for a data class implementation to use.

```
class EAbstractData : public QObject
{
...
protected:
   EMetadata systemMeta() const;
   EMetadata meta() const;
   void setMeta(const EMetadata& newMeta);
   const EDataStream& stream() const;
   EDataStream& stream();
   void seek(qint64 index) const;
   void allocate(qint64 size);
private:
   qint32 _value {0};
};
```

A detailed description of these methods can be read within the code documentation itself.  The systemMeta(), meta(), and setMeta() methods deal with interacting with the metadata system of an ACE data object. The stream() methods deal with the streaming object used for reading/writing to the data portion of a data object file. The seek() and allocate() methods deal with self-explanatory actions that can be done to the data object file.

##dataEnd()

```
qint64 HelloWorld::dataEnd() const;
{
   return sizeof(qint32);
}
```

##readData()

```
void HelloWorld::readData()
{
   seek(0);
   stream() >> _value;
}
```

##writeNewData()

```
void HelloWorld::writeNewData()
{
   seek(0);
   stream() << (qint32)0;
}
```

##model()

```
QAbstractTableModel* HelloWorld::model()
{
   return nullptr;
}
```

##finish()

```
void HelloWorld::finish()
{
   stream() << _value;
}
```
