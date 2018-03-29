#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <QObject>
//



/*!
 */
class EAbstractAnalytic : public QObject
{
   Q_OBJECT
public:
   class Input;
   class Serial;
   class OpenCL;
   /*!
    */
   virtual int size() const = 0;
   /*!
    *
    * @param index  
    *
    * @return a 
    */
   virtual QByteArray makeBlock(int index) const = 0;
   /*!
    *
    * @param index  
    *
    * @param results  
    */
   virtual void readBlock(int index, const QByteArray& results) = 0;
   /*!
    *
    * @return a 
    */
   virtual EAbstractAnalytic::Input* makeInput() = 0;
   /*!
    *
    * @return a 
    */
   virtual EAbstractAnalytic::Serial* makeSerial() const = 0;
   virtual EAbstractAnalytic::OpenCL* makeOpenCL() const;
   virtual void initialize();
   virtual void finish();
};



#endif
