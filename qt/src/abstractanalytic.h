#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <QtCore>

#include "utilities.h"



class EAbstractData;



/// Interface class that defines an analytic type.
class EAbstractAnalytic : public QThread
{
   Q_OBJECT
public:
   /// All possible argument types.
   enum class ArgumentType
   {
      Bool
      ,Integer
      ,Float
      ,Double
      ,String
      ,Combo
      ,FileIn
      ,FileOut
      ,DataIn
      ,DataOut
   };
   EAbstractAnalytic() = default;
   virtual ~EAbstractAnalytic() = default;
   /// Get the total number of arguments.
   ///
   /// @return Total number of arguments.
   virtual int getArgumentCount() = 0;
   /// Get command line name for given argument.
   ///
   /// @param argument Argument number.
   /// @return Command line name.
   virtual QString getCommandLineName(int argument) = 0;
   /// Get argument type.
   ///
   /// @param argument Argument number.
   /// @return Argument type.
   virtual ArgumentType getArgumentType(int argument) = 0;
   /// Get argument title which is displayed in analytic form.
   ///
   /// @param argument Argument number.
   /// @return Argument title.
   virtual QString getArgumentTitle(int argument) = 0;
   /// Get argument combo box values only if argument is of Combo type.
   ///
   /// @param argument Argument number.
   /// @return List of valid values for combo box.
   virtual QStringList getComboValues(int argument) = 0;
   /// Get argument Qt's "What's This?" value.
   ///
   /// @param argument Argument number.
   /// @return "What's This" Qt string.
   virtual QString getArgumentWhatsThis(int argument) = 0;
   /// Get file filters for file input/output argument.
   ///
   /// @param argument Argument number.
   /// @return List of file filters.
   virtual QString getFileArgumentFilters(int argument) = 0;
   /// Get data object type for data input/output argument.
   ///
   /// @param argument Argument number.
   /// @return Data object type identifier.
   virtual quint16 getDataArgumentType(int argument) = 0;
   /// Set argument value for Bool, Integer, Float, Double, String, and Combo types.
   ///
   /// @param argument Argument number.
   /// @param value New value for argument.
   virtual void setArgument(int argument, QVariant value) = 0;
   /// Set argument value for FileIn and FileOut types.
   ///
   /// @param argument Argument number.
   /// @param value Reference to file.
   virtual void setArgument(int argument, QFile* file) = 0;
   /// Set argument value for DataIn and DataOut types.
   ///
   /// @param argument Argument number.
   /// @param value Reference to data object.
   virtual void setArgument(int argument, EAbstractData* data) = 0;
   /// Initialize analytic for execution.
   ///
   /// @return True if all output data objects must pre-allocate file space.
   virtual bool initialize() = 0;
   /// Get number of blocks that will be executed.
   ///
   /// @return Number of blocks to execute.
   virtual int getBlockSize() = 0;
   /// Execute block.
   ///
   /// @param block Block number.
   /// @param True if this block is done with execution or false if it should be called again.
   virtual bool runBlock(int block) = 0;
   /// Finish analytic after execution.
   virtual void finish() = 0;
   void run() override final;
signals:
   void progressed(int perceptComplete);
   void finished();
};



#endif
