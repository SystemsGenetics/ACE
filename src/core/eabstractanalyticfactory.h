#ifndef EABSTRACTANALYTICFACTORY_H
#define EABSTRACTANALYTICFACTORY_H
#include <memory>
#include <QString>
#include "global.h"
//



/*!
 * This represents the factory for producing new abstract analytic objects. This 
 * also gives basic information that allows ACE to query and figure out all 
 * available analytic types the program that uses the ACE library provides. Unlike 
 * the data factory the integers defining analytic types does not need to be 
 * constant for backwards compatibility. 
 */
class EAbstractAnalyticFactory
{
public:
   static EAbstractAnalyticFactory& instance();
   static void setInstance(std::unique_ptr<EAbstractAnalyticFactory>&& instance);
   /*!
    * This is so any implementation of this class will be deconstructed correctly. 
    */
   virtual ~EAbstractAnalyticFactory() = default;
   /*!
    * This interface returns the total number of analytic types a program implements 
    * for ACE. All integers from 0 to one less than the size returned must be defined 
    * as a unique analytic type. 
    *
    * @return Total number of analytic types this program implements. 
    */
   virtual quint16 size() const = 0;
   /*!
    * This interface returns the display name for the given analytic type. 
    *
    * @param type The analytic type whose display name is returned. 
    *
    * @return Display name for the given analytic type. 
    */
   virtual QString name(quint16 type) const = 0;
   /*!
    * This interface returns the command line name for the given analytic type. This 
    * name must be unique among all other analytic command line names. 
    *
    * @param type The analytic type whose display name is returned. 
    *
    * @return Command line name for the given analytic type. 
    */
   virtual QString commandName(quint16 type) const = 0;
   /*!
    * This interface makes and returns a new abstract analytic object of the given 
    * type. 
    *
    * @param type The data type of the abstract data object that is made and returned. 
    *
    * @return Pointer to the new abstract analytic object of the given type. 
    */
   virtual std::unique_ptr<EAbstractAnalytic> make(quint16 type) const = 0;
private:
   static void checkCommandNames(EAbstractAnalyticFactory* factory);
   static void checkCommandArguments(EAbstractAnalytic* analytic);
   /*!
    * Pointer to the global instance of this program's analytic factory. 
    */
   static EAbstractAnalyticFactory* _instance;
};



#endif
