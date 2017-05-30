#include "abstractanalytic.h"






void EAbstractAnalytic::run()
{
   initialize();
   int blockSize {getBlockSize()};
   int done {0};
   bool blocks[blockSize] {true};
   while ( done < blockSize )
   {
      for (int i = 0; i < blockSize ;++i)
      {
         if ( blocks[i] )
         {
            if ( !runBlock(i) )
            {
               blocks[i] = false;
               ++done;
            }
         }
      }
   }
   finish();
}
