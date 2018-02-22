#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <QtCore>

#include "utilities.h"
#include "exception.h"



namespace Ace { class DataReference; }
class EAbstractData;



/// Interface class that defines an analytic type.
class EAbstractAnalytic : public QThread
{
   Q_OBJECT
public:
   /// Contains capabiliites enumeration.
   struct Capabilities
   {
      /// Specifies capabilities of an analytic.
      enum
      {
         Serial = 1 ///< Serial execution
         ,OpenCL = 2 ///< OpenCL execution
         ,MPI = 3 ///< MPI execution
      };
   };
   /// All possible argument types.
   enum class ArgumentType
   {
      Bool ///< Boolean
      ,Integer ///< Integer
      ,Double ///< Real number
      ,String ///< String
      ,Combo ///< Combo selection
      ,FileIn ///< Input file
      ,FileOut ///< Output file
      ,DataIn ///< Input data objecct
      ,DataOut ///< Output data object
   };
   /// All possible roles for argument data
   enum class Role
   {
      Type ///< Argument type
      ,Title ///< Title of argument
      ,WhatsThis ///< Qt "What's This?" string
      ,DefaultValue ///< Default value of argument
      ,Minimum ///< Minimum value of integer/double argument
      ,Maximum ///< Maximum value of integer/double argument
      ,Decimals ///< Decimal precision of double argument
      ,CommandLineName ///< Unique command line name of argument
      ,ComboValues ///< List of strings of valid combo selection argument
      ,FileFilters ///< Qt list of file filters for file argument
      ,DataType ///< Data type for data object argument
   };
   EAbstractAnalytic() = default;
   virtual ~EAbstractAnalytic();
   /// Get total number of arguments of an analytic.
   virtual int getArgumentCount() = 0;
   /// Get argument type.
   ///
   /// @param argument Index of argument.
   ///
   /// @return Argument type.
   virtual ArgumentType getArgumentData(int argument) = 0;
   /// Get argument data.
   ///
   /// @param argument Index of argument.
   /// @param role Argument data role.
   ///
   /// @return Argument data.
   virtual QVariant getArgumentData(int argument, Role role) = 0;
   /// Set argument data. This is for boolean, integer, double, string, and combo arguments.
   ///
   /// @param argument Index of argument.
   /// @param value Value for argument.
   virtual void setArgument(int /*argument*/, QVariant /*value*/) {}
   /// Set argument data. This is for file input/output arguments.
   ///
   /// @param argument Index of argument.
   /// @param file Pointer to opened Qt file. For input it is Qt::ReadOnly and for output it is
   /// Qt::WriteOnly|Qt::Truncate.
   virtual void setArgument(int /*argument*/, QFile* /*file*/) {}
   /// Set argument data. This is for data input/output arguments.
   ///
   /// @param argument Index of argument.
   /// @param data Pointer to abstract data object.
   virtual void setArgument(int /*argument*/, EAbstractData* /*data*/) {}
   /// Get capabilities of analytic.
   ///
   /// @return Returns bitmask of capabilities.
   virtual quint32 getCapabilities() const = 0;
   /// Called at very beginning of analytic execution.
   ///
   /// @return Returns true if output data objects should pre allocate space on file.
   virtual bool initialize() = 0;
   /// Run serial execution of analytic.
   virtual void runSerial() {}
   /// Gets number of blocks for execution.
   ///
   /// @return Number of blocks.
   virtual int getBlockSize() { return 0; }
   /// Runs a block for execution.
   ///
   /// @param block Index of block.
   ///
   /// @return Returns false if this block is done with execution.
   virtual bool runBlock(int /*block*/) { return false; }
   /// Builds a new work block by the master node that will be processed by a slave node.
   ///
   /// @return New block that contains information to work on.
   virtual QByteArray buildMPIBlock() { return QByteArray(); }
   /// Read in an information block produced from a slave node working on a work block.
   ///
   /// @param block Information block produced from the finished work of a slave node.
   ///
   /// @return Returns true if the block was processed and saved or false if it was not ready to be
   /// processed.
   virtual bool readMPIBlock(const QByteArray& /*block*/) { return false; }
   /// Process a work block as a slave node returning an information block.
   ///
   /// @param block Work block to be processed.
   ///
   /// @return Information block that holds the results of processing the work block.
   virtual QByteArray processMPIBlock(const QByteArray& /*block*/) { return QByteArray(); }
   /// Called after all blocks are done with execution.
   virtual void finish() {}
   /// Internal command DO NOT USE.
   void run() override final;
   /// Internal command DO NOT USE.
   void mpiRun(/*Ace::MPI& mpi*/);
   /// Internal command DO NOT USE.
   void stop();
   /// Internal command DO NOT USE.
   void addFileIn(int argument, const QString& path);
   /// Internal command DO NOT USE.
   void addFileOut(int argument, const QString& path);
   /// Internal command DO NOT USE.
   void addDataIn(int argument, const QString& path, quint16 type);
   /// Internal command DO NOT USE.
   void addDataOut(int argument, const QString& path, quint16 type);
   /// Internal command DO NOT USE.
   void setCommand(const QString& command) { _command = command; }
   void startMPI();
signals:
   void sendData(int toRank, const QByteArray& data);
   /// Signal that is emitted whenever the percentage complete of the analytic has changed.
   ///
   /// @param percentComplete Percentage complete from 0 to 100.
   void progressed(int perceptComplete);
   /// Internal command DO NOT USE.
   void finished();
   /// Internal command DO NOT USE.
   void exceptionThrown(QString file, int line, QString function, QString title, QString details);
protected:
   /// Get extra input data object. WARNING CAN ONLY BE CALLED WITHIN INITIALIZE FUNCTION.
   ///
   /// @param path File path to data object.
   /// @param type Data type.
   ///
   /// @return Pointer to abstract data object.
   EAbstractData* getDataIn(const QString& path, quint16 type);
   /// Get extra output data object. WARNING CAN ONLY BE CALLED WITHIN INITIALIZE FUNCTION.
   ///
   /// @param path File path to data object.
   /// @param type Data type.
   ///
   /// @return Pointer to abstract data object.
   EAbstractData* getDataOut(const QString& path, quint16 type);
private slots:
   void receiveData(const QByteArray& data, int fromRank);
private:
   void processMPI();
   void prepareRun();
   void finishRun();
   static QMutex _mutex;
   QList<Ace::DataReference*> _dataIn;
   QList<Ace::DataReference*> _dataOut;
   QList<QFile*> _files;
   QString _command;
   QList<QByteArray> _mpiBlocks;
   int _mpiOut {0};
   int _nextOut {1};
};



#endif
