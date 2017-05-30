#ifndef EABSTRACTANALYTICFACTORY_H
#define EABSTRACTANALYTICFACTORY_H
#include <QtCore>
#include <memory>

#include "utilities.h"



class EAbstractAnalytic;



/// Factory object that informs ACE of all possible analytic types and makes them.
class EAbstractAnalyticFactory
{
public:
   EAbstractAnalyticFactory() = default;
   virtual ~EAbstractAnalyticFactory() = default;
   ACE_DISBALE_COPY_AND_MOVE(EAbstractAnalyticFactory)
   /// Get instance of analytic factory.
   ///
   /// @return Reference to factory.
   static EAbstractAnalyticFactory& getInstance();
   /// Set new analytic factory instance.
   ///
   /// @param factory New analytic factory.
   static void setInstance(std::unique_ptr<EAbstractAnalyticFactory>&& factory);
   /// Get total number of possible analytic types.
   virtual quint16 getCount() = 0;
   /// Get name of specific analytic type.
   ///
   /// @param type Analytic type identifier.
   virtual QString getName(quint16 type) = 0;
   /// Make new analytic of given type.
   ///
   /// @param type Analytic type identifier.
   virtual std::unique_ptr<EAbstractAnalytic> make(quint16 type) = 0;
private:
   static std::unique_ptr<EAbstractAnalyticFactory> _instance;
};



#endif
