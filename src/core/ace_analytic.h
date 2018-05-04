
/*!
 * This contains all classes related to running an analytic. This required its own 
 * name space because of the immense complexity required for ACE to provide an 
 * abstract interface for running analytic types in heterogeneous environments. 
 */
namespace Ace
{
   namespace Analytic
   {
      class AbstractRun;
      class AbstractInput;
      class AbstractManager;
      class SimpleRun;
      class SerialRun;
      class OpenCLRun;
      class Single;
      class MPIBase;
      class MPIMaster;
      class MPISlave;
      class Chunk;
      class Merge;
   }
}
