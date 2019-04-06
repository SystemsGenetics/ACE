#ifndef EABSTRACTANALYTICSERIAL_H
#define EABSTRACTANALYTICSERIAL_H
#include <memory>
#include <QObject>
#include "global.h"



/*!
 * This represents a working class of an analytic. This class handles actual
 * execution of work blocks, returning result blocks. An implementation of this
 * class is responsible for obtaining the work data from a work block and
 * returning the results in a results block. This is the most basic type of
 * working analytic class where only serial execution is given with no special
 * devices.
 */
class EAbstractAnalyticSerial : public QObject
{
   Q_OBJECT
public:
   /*!
    * This interface reads in the given work block, executes the algorithms
    * necessary to produce its results, and saves those results in a new results
    * block whose pointer is returned.
    *
    * @param block Pointer to work block that is used to produce the returned
    *              results block.
    *
    * @return Pointer to results block produced from the given work block.
    */
   virtual std::unique_ptr<EAbstractAnalyticBlock> execute(const EAbstractAnalyticBlock* block) = 0;
public:
   explicit EAbstractAnalyticSerial(EAbstractAnalytic* parent);
};

#endif
