#ifndef EABSTRACTANALYTICINPUT_H
#define EABSTRACTANALYTICINPUT_H
#include <QObject>
#include "global.h"
class QFile;



/*!
 * This represents the input class that handles taking in all user input for its
 * parent analytic. An implementation of this class must define all inputs for
 * its analytic type and then handle setting all values for that input to its
 * parent analytic. For file and data object inputs/outputs the ACE system
 * handles closing them on its own so an implementation of this class does not
 * need to worry about that; for those types only a pointer is passed and not
 * actual ownership of the underlying objects.
 */
class EAbstractAnalyticInput : public QObject
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
   virtual EAbstractAnalyticInput::Type type(int index) const = 0;
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
   explicit EAbstractAnalyticInput(EAbstractAnalytic* parent);
};

#endif
