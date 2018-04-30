#ifndef ACE_ANALYTIC_ABSTRACTINPUT_H
#define ACE_ANALYTIC_ABSTRACTINPUT_H
#include <memory>
#include <QMap>
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class AbstractInput
      {
      public:
         /*!
          */
         AbstractInput() = default;
         ~AbstractInput();
         /*!
          */
         virtual bool isFinished() const = 0;
         virtual void saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result);
      protected:
         virtual int index() const;
         virtual void writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result);
      private:
         /*!
          */
         QMap<int,EAbstractAnalytic::Block*> _hopper;
      };
   }
}



#endif
