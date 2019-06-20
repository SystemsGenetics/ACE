#ifndef ACE_ANALYTIC_H
#define ACE_ANALYTIC_H



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This contains all classes related to running an analytic. This required its
    * own name space because of the immense complexity required for ACE to provide
    * an abstract interface for running analytic types in heterogeneous
    * environments.
    */
   namespace Analytic
   {
      class AbstractRun;
      class AbstractInput;
      class AbstractManager;
      class SimpleRun;
      class SerialRun;
      class Single;
      class OpenCLRunThread;
      class OpenCLRun;
      class CUDARunThread;
      class CUDARun;
      class Chunk;
      class Merge;
      class AbstractMPI;
      class MPIMaster;
      class MPISlave;
   }
}

#endif
