#ifndef DATAFRAME_ITERATOR_H
#define DATAFRAME_ITERATOR_H
#include "dataframe.h"
//



/*!
 * This class implements the row iterator for the dataframe data object. The
 * row iterator can read from or write to any row in the dataframe, or simply
 * iterate through each row. The iterator stores only one row of data in memory
 * at a time.
 */
class DataFrame::Iterator
{
public:
   float& operator[](int index);
public:
   Iterator(DataFrame* matrix, bool isInitialized = false);
   ~Iterator();
   void read(int index);
   bool readNext();
   void write(int index);
   bool writeNext();
   float at(int index) const;
private:
   /*!
    * Pointer to the parent dataframe.
    */
   DataFrame* _matrix;
   /*!
    * The iterator's current position in the dataframe.
    */
   int _index {0};
   /*!
    * Pointer to the data of the current row.
    */
   float* _data;
};



#endif
