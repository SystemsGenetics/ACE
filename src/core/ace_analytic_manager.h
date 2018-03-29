#ifndef ACE_ANALYTIC_MANAGER_H
#define ACE_ANALYTIC_MANAGER_H
#include <memory>
#include <QObject>
#include "global.h"



class QFile;
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class Manager : public QObject
      {
         Q_OBJECT
      public:
         static std::unique_ptr<Ace::Analytic::Manager> makeManager(int type, int index, int size);
         Manager(int type);
         int size() const;
         EAbstractAnalytic::Input::Type type(int index) const;
         QVariant data(int index, EAbstractAnalytic::Input::Role role) const;
         void set(int index, const QVariant& value);
         void set(int index, QFile* file);
         virtual void set(int index, EAbstractData* data);
         void finish();
      signals:
         /*!
          *
          * @param percentComplete  
          */
         void progressed(int percentComplete);
      protected:
         EAbstractAnalytic* analytic();
         const EAbstractAnalytic* analytic() const;
      private:
         /*!
          */
         EAbstractAnalytic* _analytic;
         /*!
          */
         EAbstractAnalytic::Input* _input;
      };
   }
}



#endif
