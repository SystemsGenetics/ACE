#ifndef ACE_ANALYTIC_IOBASE_H
#define ACE_ANALYTIC_IOBASE_H
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
      class IOBase
      {
      public:
         /*!
          */
         IOBase() = default;
         ~IOBase();
         /*!
          */
         virtual bool hasWork() const = 0;
         /*!
          */
         virtual bool isFinished() const = 0;
         /*!
          */
         virtual std::unique_ptr<EAbstractAnalytic::Block> makeWork() = 0;
         /*!
          *
          * @param result  
          */
         virtual void saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) = 0;
      protected:
         void hopperSaveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result, EAbstractAnalytic* analytic, int* next);
      private:
         /*!
          */
         QMap<int,EAbstractAnalytic::Block*> _hopper;
      };
   }
}



#endif
