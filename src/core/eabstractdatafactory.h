#ifndef EABSTRACTDATAFACTORY_H
#define EABSTRACTDATAFACTORY_H
#include <memory>
#include <QString>
#include "global.h"
//



/*!
 * This represents the factory for producing new abstract data objects. This also 
 * gives basic information that allows ACE to query and figure out all available 
 * data types the program that uses the ACE library provides. If a program wishes 
 * to be backwards compatible any previously defined data types must maintain the 
 * same unique integer that identifies them. 
 */
class EAbstractDataFactory
{
public:
   static EAbstractDataFactory& instance();
   static void setInstance(std::unique_ptr<EAbstractDataFactory>&& instance);
   /*!
    * This is so any implementation of this class will be deconstructed correctly. 
    */
   virtual ~EAbstractDataFactory() = default;
   /*!
    * This interface returns the total number of data types a program that uses ACE 
    * implements. All integers from 0 to one less than the size returned must be 
    * defined as a unique data type. 
    *
    * @return Total number of data types this program implements. 
    */
   virtual quint16 size() const = 0;
   /*!
    * This interface returns the display name for the given data type. 
    *
    * @param type The data type whose display name is returned. 
    *
    * @return Display name for the given data type. 
    */
   virtual QString name(quint16 type) const = 0;
   /*!
    * This interface returns the file extension for the given data type as a string. 
    *
    * @param type The data type whose file extension is returned. 
    *
    * @return File extension for the given data type. 
    */
   virtual QString fileExtension(quint16 type) const = 0;
   /*!
    * Makes and returns a new abstract data object of the given type. 
    *
    * @param type The data type of the abstract data object that is made and returned. 
    *
    * @return Pointer to the new abstract data object of the given type. 
    */
   virtual std::unique_ptr<EAbstractData> make(quint16 type) const = 0;
private:
   /*!
    * Pointer to the global instance of this program's data factory. 
    */
   static EAbstractDataFactory* _instance;
};



#endif
