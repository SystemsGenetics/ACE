# Summary

This document introduces the framework of the ACE library by providing a basic tutorial mimicking the classic hello world of programming introductions. It starts by creating a hello world data class, followed by a hello world analytic class, and finishes by wrapping it all together and including it in the ACE plugin factory object.

# Prerequisite Knowledge

In order to use ACE one should be well versed in C++ up to the C++11 standard, have basic knowledge with Qt, and be competent in GPGPU programming using the OpenCL standard. While general knowledge of both is required there are specific subjects that ACE uses frequently.

For C++ ACE uses abstract classes and the abstract factory design pattern for interfacing with an application's data and analytic classes. C++ Stream operators are also used heavily with data classe's data object's file IO.

For Qt the most important topics to understand is using its basic string classes QString and QByteArray.

- C++11
    - [Abstract Classes](https://www.tutorialspoint.com/cplusplus/cpp_interfaces.htm)
    - [Abstract Factory Design Pattern](https://www.codeproject.com/Articles/331304/Understanding-and-Implementing-Abstract-Factory)
    - [Streaming Operators](https://www.cprogramming.com/tutorial/c++-iostreams.html)
- [Qt Core](http://doc.qt.io/qt-5/qtcore-index.html)
    - [OString](https://doc.qt.io/qt-5/qstring.html)
    - [QByteArray](https://doc.qt.io/qt-5/qbytearray.html)
- [OpenCL](https://www.khronos.org/opencl/)

# Data Class

Creating a data class is the first thing you should do when implementing a new idea when using the ACE library since analytic objects depend on and create new data objects. Our HelloWorld data object will be responsible with holding the value of a 32 bit integer.

First we will create a skeleton class which will compile, but does absolutely nothing useful.

```
class HelloWorld : public EAbstractData
{
public:
   virtual qint64 dataEnd() const override final;
   virtual void readData() override final;
   virtual void writeNewData() override final;
   virtual QAbstractTableModel* model() override final;
   virtual void finish() override final;
   qint32 size();
   qint32 get(int index);
   void add(quint32 value);
private:
   qint32 _size {0};
}; 
```

This skeleton inherits the required abstract data class from ACE which is the abstract class that defines a data object. The virtual functions that must be implemented from this class are dataEnd(), readData(), writeNewData(), and model(). The finish() function is optional and does not need to be overriden.

The dataEnd() method is called to determine the end of the data within this data object's file stream. The readData() method is called once when a data object file is opened and should read in its data from its data object file stream. The writeNewData() method is called once to initialize a new and empty data object file and should initialize its data object file stream. The model() method is called to get a pointer to a qt abstract table model that represents the data of its data object. The optional finish() method is called once for a new data object file once its analytic is finished running and is about to close.

A data class should have additional methods used by analytics for accessing and setting its data. These methods are not needed for the ACE platform to do its job though, and is left to the data class declaring as regular methods that its fellow analytics should be aware of. In our example data object these methods are size(), get(), and add().

## Protected Methods

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
...
};
```

A detailed description of these methods can be read within the code documentation itself.  The systemMeta(), meta(), and setMeta() methods deal with interacting with the metadata system of an ACE data object. The stream() methods deal with the streaming object used for reading/writing to the data portion of a data object file. The seek() and allocate() methods deal with self-explanatory actions that can be done to the data object file.

## Data Interface Methods

Now we will define all interface methods that ACE uses to interact with your data class.

### dataEnd()

This interface informs ACE where in the data object file your data ends. This is used to safely write part of the metadata to the file after your data so nothing is overwritten. Another way to think about the number returned is it is effectively the size of your data. For our example class that is the number of integers it contains plus one for the integer size header.

```
qint64 HelloWorld::dataEnd() const;
{
   return sizeof(qint32)*(1 + _size);
}
```

### readData()

This interface is called when opening an existing data object file. This allows a data class to initialize itself by reading in whatever information it keeps in memory instead of the file. It is assumed the majority of data for a data object is kept in the file because ACE is intended for very large data. In our example class we simply read in the header that informs about the number of integer values the data object holds in the file.

```
void HelloWorld::readData()
{
   seek(0);
   stream() >> _size;
}
```

### writeNewData()

This interface is called when opening a new data object file. This allows a data class to initialize a new data object file to a null state. In our example class we set the header to zero to make it empty with zero integer values contained within in.

```
void HelloWorld::writeNewData()
{
   seek(0);
   stream() << (qint32)0;
}
```

### model()

This interface is called to get a pointer to the abstract table model that represents the data of a data object instance. To understand how to properly use this model please look at Qt documentation, specifically the QAbstractTableModel class. In our example class we simply return a null pointer.

```
QAbstractTableModel* HelloWorld::model()
{
   return nullptr;
}
```

### finish()

This interface is called once with new data object files once the analytic has finished execution. This allows a data class to write any final values to the object file that can only be calculated once the analytic is finished adding data to it. This is an optional interface. In our example class we write the number of integers the new data object contains to its header location within the file.

```
void HelloWorld::finish()
{
   seek(0);
   stream() << _size;
}
```

## Additional Methods

Now we will define the additional methods unique to our specific data class. These methods primary job our to access and write data it is responsible for holding in its data object file. Because ACE is designed for large data, reading and writing data should be done directly with the file object IO.

### get()

This method is used to get an integer value stored by its data object with the given index. In a real world implementation this method should also check for valid index ranges.

```
qint32 HelloWorld::get(int index)
{
   qint32 ret;
   seek(sizeof(qint32)*(1 + index));
   stream() >> ret;
   return ret;
}
```

### add()

This method is used to append new integer values to its data object. Notice that while the new integer value is immediately written to disk, the header size value is not. This is recommended to greatly increase efficiency of file IO. The header information can be written at the end in the finish() interface.

```
void HelloWorld::add(qint32 value)
{
   seek(sizeof(qint32)*(1 + _size);
   stream() << value;
   ++_size;
}
```

## Metadata

The metadata system for data objects is split into two regions.

The first is read only and is called the system metadata. This is generated by the ACE system and cannot be modified. It is accessed with the systemMeta() method. The information contained deals mainly with information about the inputs used to generate the metadata's data object.

The second is read and write and is called the user metadata. If the data object class does nothing with it a new data object will have a blank user metadata. It can be accessed with meta() and written to with setMeta(). The user metadata can only be written to at the very end of a new data object inside the finish() interface. It is entirely up to the data object class to decide what should be put into this metadata. The user can also manipulate this metadata through the GUI interface.

# Analytic Class

Now we will implement a hello world analytic class that takes in a hello world data object and outputs a new hello world data object. First we will make the header declaration.

```
class HelloAnalyticWorld : public EAbstractAnalytic
{
   Q_OBJECT
public:
   class Input;
   class Block;
   class Serial;
   class OpenCL;
public:
   virtual int size() const override final;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeWork(int index) const override final;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeWork() const override final;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeResult() const override final;
   virtual void process(const EAbstractAnalytic::Block* result) override final;
   virtual EAbstractAnalytic::Input* makeInput() override final;
   virtual EAbstractAnalytic::Serial* makeSerial() override final;
   virtual EAbstractAnalytic::OpenCL* makeOpenCL() override final;
   virtual void initialize() override final;
   virtual void initializeOutputs() override final;
private:
   HelloWorld* _in {nullptr};
   HelloWorld* _out {nullptr};
   int _amount {0};
};
```

The new object will have the array of integers from the input data with a given amount added to each integer. Because of the complexity of an analytic class it is divided into multiple sub classes. Each sub class is created by the base analytic class. The base analytic class is responsible for making the more specialized classes along with basic initialization, producing blocks and processing blocks.

The work of an analytic is sub divided into blocks of work that are then processed and returned to the base analytic. The number of blocks to be produced must be constant through the entire execution of an analytic. The block system will be discussed in further detail later on.

The other sub classes that handle specific parts of analytic execution are the input class and engine classes. In this case the engine classes are serial and OpenCL. These topics will be discussed when their class headers are defined.

## size()

This interface returns the total number of blocks an analytic must process before it is done. The ACE system will use this multiple times through an analytic run so this must remain constant and be determined after initialization.

```
int HelloAnalyticWorld::size() const
{
   return _in->size();
}
```

## makeWork()

These interfaces make new work blocks.

The interface with the index argument is responsible for making a new block of work that will be processed by an engine that is explained later. This interface should return a block of work at the given index. The maximum index is determined by the previous size() interface.

The interface with no arguments is responsible for making a new blank work block that is in a null state. This is used internally by ACE system to fill in its data when running in MPI mode.

```
std::unique_ptr<EAbstractAnalytic::Block> HelloAnalyticWorld::makeWork(int index) const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block(index,_in->get(index)));
}

std::unique_ptr<EAbstractAnalytic::Block> HelloAnalyticWorld::makeWork() const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block);
}
```

## makeResult()

This interface makes a new blank results block that is in a null state. This is used internally by the ACE system to fill in its data when running in MPI mode.

```
std::unique_ptr<EAbstractAnalytic::Block> HelloAnalyticWorld::makeResult() const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block);
}
```

## process()

This interface must process the results of the given result block. It is promised by the ACE system that the result blocks will be given in order, starting with index 0 and ending with size() - 1. Typically this is where data is written to the output data objects of the analytic instance.

```
void HelloAnalyticWorld::process(const EAbstractAnalytic::Block* result)
{
   _out->add(result->cast<Block>()->value());
}
```

## makeInput()

This creates a new input class instance for this analytic that handles all input argument. Details are discussed lateron.

```
EAbstractAnalytic::Input* HelloAnalyticWorld::makeInput()
{
   return new Input(this);
}
```

## makeSerial()

This creates a new serial class engine for this analytic that handles executing work blocks and returning result blocks in serial(CPU) mode. Details are discussed lateron.

```
EAbstractAnalytic::Input* HelloAnalyticWorld::makeSerial()
{
   return new Serial(this);
}
```

## makeOpenCL()

This creates a new OpenCL class engine for this analytic that handles executing work blocks and returning result blocks in OpenCL(GPGPU) mode. Details are discussed lateron.

```
EAbstractAnalytic::Input* HelloAnalyticWorld::makeOpenCL()
{
   return new OpenCL(this);
}
```

## initialize()

This initializes an analytic instance right before processing of work and result blocks begin and after all input arguments have been processed. This is called on any analytic instance type so it is not safe to check for valid output arguments in this interface. Only valid input should be checked inside this interface.

```
void HelloAnalyticWorld::initialize()
{
   if ( !_in )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("The required input data object was not set."));
      throw e;
   }
}
```

## initializeOutputs()

This initializes a valid output analytic instance, called right after the initialize() interface. This is where validation of all output arguments should be done or any other initialization that only needs to exist for processing the output of result blocks.

```
void HelloAnalyticWorld::initializeOutputs()
{
   if ( !_out )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("The required output data object was not set."));
      throw e;
   }
}
```

## Input

```
class HelloAnalyticWorld::Input : public EAbstractAnalytic::Input
{
   Q_OBJECT
public:
   enum Argument
   {
      InputData
      ,OutputData
      ,Amount
      ,Total
   };
public:
   explicit Input(HelloAnalyticWorld* parent);
   virtual int size() const override final;
   virtual EAbstractAnalytic::Input::Type type(int index) const override final;
   virtual QVariant data(int index, Role role) const override final;
   virtual void set(int index, const QVariant& value) override final;
   virtual void set(int index, QFile* file) override final;
   virtual void set(int index, EAbstractData* data) override final;
private:
   HelloAnalyticWorld* _base;
};
```

### size()

```
int HelloAnalyticWorld::Input::size() const
{
   return Total;
}
```

### type()

```
EAbstractAnalytic::Input::Type HelloAnalyticWorld::Input::type(int index) const
{
   switch (index)
   {
   case InputData: return Type::DataIn;
   case OutputData: return Type::DataOut;
   case Amount: return Type::Integer;
   default: return Type::Boolean;
   }
}
```

### data()

```
QVariant HelloAnalyticWorld::Input::data(int index, Role role) const
{
   switch (index)
   {
   case InputData: return inputDataData(role);
   case OutputData: return outputDataData(role);
   case Amount: return amountData(role);
   default: return QVariant();
   }
}




QVariant HelloAnalyticWorld::Input::inputDataData(Role role) const
{
   // Based off the data role given return the appropriate argument data. 
   switch (role)
   {
   case Role::CommandLineName: return QString("in");
   case Role::Title: return tr("Input:");
   case Role::WhatsThis: return tr("Input data object of type Integer Array.");
   case Role::DataType: return DataFactory::IntegerArrayType;
   default: return QVariant();
   }
}



QVariant HelloAnalyticWorld::Input::outputDataData(Role role) const
{
   // Based off the data role given return the appropriate argument data. 
   switch (role)
   {
   case Role::CommandLineName: return QString("out");
   case Role::Title: return tr("Output:");
   case Role::WhatsThis: return tr("Output data object of type Integer Array.");
   case Role::DataType: return DataFactory::IntegerArrayType;
   default: return QVariant();
   }
}



QVariant HelloAnalyticWorld::Input::amountData(Role role) const
{
   // Based off the data role given return the appropriate argument data. 
   switch (role)
   {
   case Role::CommandLineName: return QString("amount");
   case Role::Title: return tr("Amount:");
   case Role::WhatsThis: return tr("The number used for the given type of operation.");
   case Role::Default: return 0;
   case Role::Minimum: return std::numeric_limits<int>::min();
   case Role::Maximum: return std::numeric_limits<int>::max();
   default: return QVariant();
   }
}
```

### set()

```
void HelloAnalyticWorld::Input::set(int index, const QVariant& value)
{
   if ( index == Amount )
   {
      _base->_amount = value.toInt();
   }
}



void HelloAnalyticWorld::Input::set(int index, QFile* file)
{
   Q_UNUSED(index);
   Q_UNUSED(file);
}



void HelloAnalyticWorld::Input::set(int index, EAbstractData* data)
{
   switch (index)
   {
   case InputData:
      _base->_in = data->cast<HelloWorld>();
      break;
   case OutputData:
      _base->_out = data->cast<HelloWorld>();
      break;
   }
}
```

## Blocks

```
class HelloAnalyticWorld::Block : public EAbstractAnalytic::Block
{
   Q_OBJECT
public:
   explicit Block() = default;
   explicit Block(int index);
   explicit Block(int index, qint32 number);
   qint32 value() const;
   void set(qint32 value);
protected:
   virtual void write(QDataStream& stream) const override final;
   virtual void read(QDataStream& stream) override final;
private:
   qint32 _number;
};
```

### write/read

```
void HelloAnalyticWorld::Block::write(QDataStream& stream) const
{
   stream << _number;
}



void HelloAnalyticWorld::Block::read(QDataStream& stream)
{
   stream >> _number;
}
```

## Engines

### Serial

```
class HelloAnalyticWorld::Serial : public EAbstractAnalytic::Serial
{
   Q_OBJECT
public:
   explicit Serial(HelloAnalyticWorld* parent);
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) override final;
private:
   HelloAnalyticWorld* _base;
};
```

#### execute()

```
std::unique_ptr<EAbstractAnalytic::Block> HelloAnalyticWorld::Serial::execute(const EAbstractAnalytic::Block* block)
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block(block->index(),block->cast<Block>()->value() + _base->_amount));
}
```

### OpenCL

```
class HelloAnalyticWorld::OpenCL : public EAbstractAnalytic::OpenCL
{
   Q_OBJECT
public:
   class Kernel;
   class Worker;
   explicit OpenCL(HelloAnalyticWorld* parent);
   virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> makeWorker() override final;
   virtual void initialize(::OpenCL::Context* context) override final;
private:
   HelloAnalyticWorld* _base;
   ::OpenCL::Context* _context {nullptr};
   ::OpenCL::Program* _program {nullptr};
};
```

#### initialize()

```
void HelloAnalyticWorld::OpenCL::initialize(::OpenCL::Context* context)
{
   _context = context;
   _program = new ::OpenCL::Program(context,{":/opencl/helloanalyticworld.cl"},this);
}
```

#### makeWorker()

```
std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> HelloAnalyticWorld::OpenCL::makeWorker()
{
   return unique_ptr<EAbstractAnalytic::OpenCL::Worker>(new Worker(_base,_context,_program));
}
```

#### Worker

```
class HelloAnalyticWorld::OpenCL::Worker : public EAbstractAnalytic::OpenCL::Worker
{
   Q_OBJECT
public:
   explicit Worker(HelloAnalyticWorld* base, ::OpenCL::Context* context, ::OpenCL::Program* program);
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) override final;
private:
   HelloAnalyticWorld* _base;
   ::OpenCL::CommandQueue* _queue;
   OpenCL::Kernel* _kernel;
   ::OpenCL::Buffer<cl_int> _buffer;
};
```

##### execute()

```
std::unique_ptr<EAbstractAnalytic::Block> HelloAnalyticWorld::OpenCL::Worker::execute(const EAbstractAnalytic::Block* block)
{
   const HelloAnalyticWorld::Block* valid {block->cast<const HelloAnalyticWorld::Block>()};
   _buffer.mapWrite(_queue).wait();
   _buffer[0] = valid->_number;
   _buffer.unmap(_queue).wait();
   _kernel->execute(_queue,&_buffer,_base->_type,_base->_amount).wait();
   _buffer.mapRead(_queue).wait();
   qint32 answer {_buffer.at(0)};
   _buffer.unmap(_queue).wait();
   return unique_ptr<EAbstractAnalytic::Block>(new HelloAnalyticWorld::Block(block->index(),answer));
}
```
