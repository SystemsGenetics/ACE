#ifndef ACE_ANALYTIC_ABSTRACTINPUT_H
#define ACE_ANALYTIC_ABSTRACTINPUT_H
#include <memory>
#include <QMap>
#include "eabstractanalytic.h"



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This contains all classes related to running an analytic. This required its
    * own name space because of the immense complexity required for ACE to provide
    * an abstract interface for running analytic types in heterogeneous
    * environments.
    */
   namespace Analytic
   {
      /*!
       * This represents the input side an an analytic run which is used by the
       * abstract run for saving result blocks it produces. An implementation of this
       * class must implement interfaces for save result or write result. If write
       * result is defined and save result is not then index must be defined. An
       * implementation must also signal when it is has received all result blocks for
       * the analytic and is finished. This class offers a sorting hopper with the
       * default definition of the save result interface. This class is not a qt
       * object because implementations of this class must also inherit the abstract
       * manager class.
       */
      class AbstractInput
      {
      public:
         /*!
          * This interface tests if this abstract input is finished and received all
          * result blocks for its analytic.
          *
          * @return True if this abstract input is finished or false otherwise.
          */
         virtual bool isFinished() const = 0;
         virtual void saveResult(std::unique_ptr<EAbstractAnalyticBlock>&& result);
      public:
         /*!
          * Constructs a new abstract input with an default empty hopper.
          */
         AbstractInput() = default;
         virtual ~AbstractInput();
      protected:
         virtual int index() const;
         virtual void writeResult(std::unique_ptr<EAbstractAnalyticBlock>&& result);
      private:
         /*!
          * This contains pointers to all result blocks that cannot yet be added to the
          * underlying analytic by calling write result because the next index has not
          * yet been given by save result.
          */
         QMap<int,EAbstractAnalyticBlock*> _hopper;
      };
   }
}

#endif
