#include "ace_analytic_mpislave.h"
#include "ace_analytic_serialrun.h"
#include "ace_analytic_openclrun.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "eabstractanalytic_block.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param type  
 */
MPISlave::MPISlave(quint16 type):
   Manager(type),
   _mpi(QMPI::instance())
{
   setupOpenCL();
   setupSerial();
   connect(_runner,&Run::finished,this,&Manager::finish);
   connect(&_mpi,&QMPI::dataReceived,this,&MPISlave::dataReceived);
}






/*!
 */
MPISlave::~MPISlave()
{
   QMPI::shutdown();
}






/*!
 */
bool MPISlave::isFinished() const
{
   return _finished && _workSize == 0;
}






/*!
 *
 * @param path  
 */
QFile* MPISlave::addOutputFile(const QString& path)
{
   Q_UNUSED(path)
   return nullptr;
}






/*!
 *
 * @param path  
 *
 * @param type  
 *
 * @param system  
 */
Ace::DataObject* MPISlave::addOutputData(const QString& path, quint16 type, const EMetadata& system)
{
   Q_UNUSED(path)
   Q_UNUSED(type)
   Q_UNUSED(system)
   return nullptr;
}






/*!
 *
 * @param result  
 */
void MPISlave::saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   _mpi.sendData(0,result->toBytes());
   result.reset();
   --_workSize;
}






/*!
 *
 * @param data  
 *
 * @param fromRank  
 */
void MPISlave::dataReceived(const QByteArray& data, int fromRank)
{
   Q_UNUSED(fromRank)
   int index {EAbstractAnalytic::Block::extractIndex(data)};
   if ( index == -1 )
   {
      _finished = true;
      if ( isFinished() )
      {
         finish();
      }
      return;
   }
   unique_ptr<EAbstractAnalytic::Block> work {analytic()->makeBlock(index)};
   work->fromBytes(data);
   ++_workSize;
   _runner->addWork(std::move(work));
}






/*!
 */
void MPISlave::setupOpenCL()
{
   if ( Settings::instance().openCLDevicePointer() )
   {
      OpenCL::Device* device {nullptr};
      int rank {_mpi.localRank() - 1};
      for (int p = 0; ( p < OpenCL::Platform::size() ) && !device ;++p)
      {
         for (int d = 0; d < OpenCL::Platform::get(p)->deviceSize() ;++d)
         {
            if ( rank-- == 0 )
            {
               device = OpenCL::Platform::get(p)->device(d);
               break;
            }
         }
      }
      if ( device )
      {
         _runner = new OpenCLRun(analytic()->makeOpenCL(),device,this,this);
      }
   }
}






/*!
 */
void MPISlave::setupSerial()
{
   if ( !_runner )
   {
      if ( EAbstractAnalytic::Serial* serial = analytic()->makeSerial() )
      {
         _runner = new SerialRun(serial,this,this);
      }
      else
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Logic Error"));
         e.setDetails(tr("Cannot run simple analytic in MPI mode."));
         throw e;
      }
   }
}
