#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <memory>
#include <QObject>
#include "eexception.h"
#include "opencl.h"
class QFile;
class EAbstractData;



/*!
 * This represents an analytic process for the ACE system. Because of the
 * complexity of an analytic its tasks are divided into multiple sub classes.
 * This class is responsible for making all other sub classes, making blocks of
 * work, reading blocks of results, initialization, and finishing the analytic.
 * An implementation of this class must do all of these things. The work of an
 * analytic is designed to be split up into individual blocks of work. This
 * class creates these blocks of work and then reads in the corresponding blocks
 * containing the results of the work. To accommodate very simple analytic types
 * an implementation of this class can choose to not produce work blocks which
 * causes the process interface to be called with empty generic result blocks. A
 * simple analytic only supports single process serial mode for operation and
 * does not override the serial or OpenCL methods. Simple analytic types also do
 * not need to implement any interfaces for making blocks since they are not
 * used.
 */
class EAbstractAnalytic : public QObject
{
   Q_OBJECT
public:
   /*!
    * This represents a block of work or results that an analytic produces and
    * consumes. This class handles storing the data needed for work and result
    * blocks for an analytic. This also handles the conversion to and from a byte
    * array so blocks can be communicated between different processes if MPI or
    * chunk running is used.
    */
   class Block : public QObject
   {
      Q_OBJECT
   public:
      static int extractIndex(const QByteArray& data);
   public:
      /*!
       * Constructs an uninitialized block. This is used in MPI and chunk runs to load
       * blocks from byte arrays.
       */
      explicit Block() = default;
      explicit Block(int index);
      int index() const;
      template<class T> const T* cast() const;
      template<class T> T* cast();
      QByteArray toBytes() const;
      void fromBytes(const QByteArray& data);
   protected:
      virtual void write(QDataStream& stream) const;
      virtual void read(QDataStream& stream);
   private:
      /*!
       * The index of this block.
       */
      int _index {-1};
   };
   /*!
    * This represents the input class that handles taking in all user input for its
    * parent analytic. An implementation of this class must define all inputs for
    * its analytic type and then handle setting all values for that input to its
    * parent analytic. For file and data object inputs/outputs the ACE system
    * handles closing them on its own so an implementation of this class does not
    * need to worry about that; for those types only a pointer is passed and not
    * actual ownership of the underlying objects.
    */
   class Input : public QObject
   {
      Q_OBJECT
   public:
      /*!
       * Defines all possible argument types that can be used as input for an
       * analytic.
       */
      enum Type
      {
         /*!
          * Defines the boolean argument type.
          */
         Boolean
         /*!
          * Defines the integer argument type.
          */
         ,Integer
         /*!
          * Defines the real number argument type. Double precision is always used.
          */
         ,Double
         /*!
          * Defines the string argument type.
          */
         ,String
         /*!
          * Defines the selection argument type. A selection is a list of predefined
          * strings where only one can be chosen.
          */
         ,Selection
         /*!
          * Defines a raw input file argument type. A qt file device is used.
          */
         ,FileIn
         /*!
          * Defines a raw output file argument type. A qt file device is used.
          */
         ,FileOut
         /*!
          * Defines an input data object argument type.
          */
         ,DataIn
         /*!
          * Defines an output data object argument type.
          */
         ,DataOut
      };
      /*!
       * Defines all possible data roles an argument can possess.
       */
      enum Role
      {
         /*!
          * Defines the command line name for an argument. This data is a string and
          * unique within all other arguments for an analytic type.
          */
         CommandLineName
         /*!
          * Defines the title for an argument. This data is a string that is displayed in
          * the analytic GUI as an argument's title.
          */
         ,Title
         /*!
          * Defines the "What's this?" for an argument. This data is a string that is
          * used to describe what this argument is to the end user.
          */
         ,WhatsThis
         /*!
          * Defines the default value for an argument. This data is a variant type
          * depending on the argument type.
          */
         ,Default
         /*!
          * Defines the minimum value for a number argument. This data is an integer or
          * real number.
          */
         ,Minimum
         /*!
          * Defines the maximum value for a number argument. This data is an integer or
          * real number.
          */
         ,Maximum
         /*!
          * Defines the decimal precision used for a real number argument. This data is
          * an integer.
          */
         ,Decimals
         /*!
          * Defines the list of possible values for a selection argument. This data is a
          * list of strings containing all possible values for a selection.
          */
         ,SelectionValues
         /*!
          * Defines the file filters used for a file input/output argument. This data is
          * a string that defines file filters as defined by Qt.
          */
         ,FileFilters
         /*!
          * Defines the data type for a data input/output argument. This data is an
          * integer that identifies a unique data type.
          */
         ,DataType
      };
   public:
      /*!
       * This interface returns the total number of arguments this analytic type
       * contains.
       *
       * @return Total number of arguments for this analytic type.
       */
      virtual int size() const = 0;
      /*!
       * This interface returns the argument type for a given index.
       *
       * @param index Index of argument whose type is returned.
       *
       * @return Argument type for the given index.
       */
      virtual EAbstractAnalytic::Input::Type type(int index) const = 0;
      /*!
       * This interface returns data for a given role on an argument with the given
       * index.
       *
       * @param index Index of argument whose data with the given role is returned.
       *
       * @param role Role for the data that is returned.
       *
       * @return Variant data for the given role and index.
       */
      virtual QVariant data(int index, Role role) const = 0;
      /*!
       * This interface sets an argument with the given index to the given value.
       *
       * @param index Index of argument whose value is set to the given value.
       *
       * @param value Variant value which is set to the argument with the given index.
       */
      virtual void set(int index, const QVariant& value) = 0;
      /*!
       * This interface passes a pointer to the qt file device for a file input/output
       * argument with the given index.
       *
       * @param index Index of argument whose qt file device pointer is given.
       *
       * @param file Pointer to qt file device for the argument with the given index.
       */
      virtual void set(int index, QFile* file) = 0;
      /*!
       * This interface passes a pointer to the abstract data object for a data
       * input/output argument with the given index.
       *
       * @param index Index of argument whose abstract data object pointer is given.
       *
       * @param data Pointer to abstract data object for the argument with the given
       *             index.
       */
      virtual void set(int index, EAbstractData* data) = 0;
   public:
      explicit Input(EAbstractAnalytic* parent);
   };
   /*!
    * This represents a working class of an analytic. This class handles actual
    * execution of work blocks, returning result blocks. An implementation of this
    * class is responsible for obtaining the work data from a work block and
    * returning the results in a results block. This is the most basic type of
    * working analytic class where only serial execution is given with no special
    * devices.
    */
   class Serial : public QObject
   {
      Q_OBJECT
   public:
      /*!
       * This interface reads in the given work block, executes the algorithms
       * necessary to produce its results, and saves those results in a new results
       * block whose pointer is returned.
       *
       * @param block Pointer to work block that is used to produce the returned
       *              results block.
       *
       * @return Pointer to results block produced from the given work block.
       */
      virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) = 0;
   public:
      explicit Serial(EAbstractAnalytic* parent);
   };
   /*!
    * This represents the base OpenCL class for an analytic. This class is
    * responsible for initializing and creating all OpenCL resources that all work
    * threads will use. The most common task that should be expected from an
    * implementation of this class is creating an OpenCL program and compiling the
    * kernels used for its analytic type.
    */
   class OpenCL : public QObject
   {
      Q_OBJECT
   public:
      /*!
       * This represents a single OpenCL worker for an analytic type that processes
       * work blocks and returns result blocks using OpenCL acceleration. Multiple
       * instances of this class are used in parallel on separate threads so thread
       * safety must always be considered for any code used by the the methods of any
       * implementation of this class.
       */
      class Worker : public QObject
      {
         Q_OBJECT
      public:
         /*!
          * This interface reads in the given work block, executes the algorithms
          * necessary to produce its results using OpenCL acceleration, and saves those
          * results in a new results block whose pointer is returned.
          *
          *
          * @return Pointer to results block produced from the given work block.
          */
         virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) = 0;
      protected:
         static int threadSize();
      };
   public:
      /*!
       * This interface creates and returns a new OpenCL worker for the
       * implementation's analytic type.
       *
       * @return Pointer to a new OpenCL worker object.
       */
      virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> makeWorker() = 0;
      /*!
       * This interface initializes all OpenCL resources used by this object's
       * implementation.
       *
       */
      virtual void initialize(::OpenCL::Context* context) = 0;
   public:
      explicit OpenCL(EAbstractAnalytic* parent);
   };
   /*!
    * This represents the base CUDA class for an analytic. This class is
    * responsible for initializing and creating all CUDA resources that all work
    * threads will use. The most common task that should be expected from an
    * implementation of this class is creating a CUDA program and compiling the
    * kernels used for its analytic type.
    */
   class CUDA : public QObject
   {
      Q_OBJECT
   public:
      /*!
       * This represents a single CUDA worker for an analytic type that processes work
       * blocks and returns result blocks using CUDA acceleration. Multiple instances
       * of this class are used in parallel on separate threads so thread safety must
       * always be considered for any code used by the the methods of any
       * implementation of this class.
       */
      class Worker : public QObject
      {
         Q_OBJECT
      public:
         /*!
          * This interface reads in the given work block, executes the algorithms
          * necessary to produce its results using CUDA acceleration, and saves those
          * results in a new results block whose pointer is returned.
          *
          *
          * @return Pointer to results block produced from the given work block.
          */
         virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) = 0;
      protected:
         static int threadSize();
      };
   public:
      /*!
       * This interface creates and returns a new CUDA worker for the implementation's
       * analytic type.
       *
       * @return Pointer to a new CUDA worker object.
       */
      virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> makeWorker() = 0;
      /*!
       * This interface initializes all CUDA resources used by this object's
       * implementation.
       *
       */
      virtual void initialize(::OpenCL::Context* context) = 0;
   public:
      explicit CUDA(EAbstractAnalytic* parent);
   };
public:
   /*!
    * This interface returns the total number of blocks this analytic must process
    * as steps or blocks of work. If this analytic produces no work blocks then
    * this means the number of times the process interface is called.
    *
    * @return Total number of blocks or steps that this analytic must work on.
    */
   virtual int size() const = 0;
   /*!
    * This interface processes the given result block. The order of indexes called
    * will always be in order starting at 0. If this analytic does not produce work
    * blocks then the given block is a generic and empty block which only contains
    * the index.
    *
    * @param result Pointer to result block that is processed.
    */
   virtual void process(const EAbstractAnalytic::Block* result) = 0;
   /*!
    * This interface makes a new input object and returns its pointer. The returned
    * input object must have this abstract analytic set as its parent.
    *
    * @return Pointer to new input object.
    */
   virtual EAbstractAnalytic::Input* makeInput() = 0;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeWork(int index) const;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeWork() const;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeResult() const;
   virtual EAbstractAnalytic::Serial* makeSerial();
   virtual EAbstractAnalytic::OpenCL* makeOpenCL();
   virtual void initialize();
   virtual void initializeOutputs();
   virtual void finish();
protected:
   static bool isMaster();
};






/*!
 * Casts this abstract block object into another constant class type.
 *
 * @tparam T The type this abstract block is being cast to. This should be this
 *           block's implementation type.
 *
 * @return Cast constant pointer to this object.
 */
template<class T> const T* EAbstractAnalytic::Block::cast() const
{
   // Use qt object cast to cast this object to the given template type as read only,
   // making sure it worked.
   const T* ret {qobject_cast<const T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract analytic block to given type."));
      throw e;
   }

   // Return the cast pointer.
   return ret;
}






/*!
 * Casts this abstract block object into another class type.
 *
 * @tparam T The type this abstract block is being cast to. This should be this
 *           block's implementation type.
 *
 * @return Cast pointer to this object.
 */
template<class T> T* EAbstractAnalytic::Block::cast()
{
   // Use qt object cast to cast this object to the given template type, making sure
   // it worked.
   T* ret {qobject_cast<T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract analytic block to given type."));
      throw e;
   }

   // Return the cast pointer.
   return ret;
}

#endif
