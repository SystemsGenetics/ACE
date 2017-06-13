#include "abstractanalytic.h"






void EAbstractAnalytic::run()
{
   // call initialize function of analytic
   initialize();

   // initialize block info
   int blockSize {getBlockSize()};
   int done {0};
   bool blocks[blockSize] {true};

   // begin block while loop
   while ( done < blockSize )
   {
      for (int i = 0; i < blockSize ;++i)
      {
         if ( blocks[i] )
         {
            // if block is still alive run it
            if ( !runBlock(i) )
            {
               // block is done, remove it from active list
               blocks[i] = false;
               ++done;
            }
         }
      }
   }

   // call finish function of analytic
   finish();
}
