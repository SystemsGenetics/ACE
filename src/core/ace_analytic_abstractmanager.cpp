#include "ace_analytic_abstractmanager.h"
#include "ace_analytic_single.h"
#include "ace_analytic_mpimaster.h"
#include "ace_analytic_mpislave.h"
#include "ace_analytic_chunk.h"
#include "ace_analytic_merge.h"
#include "ace_dataobject.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "eabstractanalyticfactory.h"
#include "eabstractanalytic_block.h"
#include "eabstractdata.h"
#include "emetaobject.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Makes a new manager with the correct implementation class and given analytic 
 * type, returning a pointer to the new manager. The index and size parameters are 
 * for chunk/merge runs. If this is not a chunk/merge run the index is 0 and size 
 * is 1. If this is a merge run the index is -1. 
 *
 * @param type The analytic type that is instantiated for this manager's run. 
 *
 * @param index The chunk index for this process. Unless this is a chunk or merge 
 *              run this is always 0. If this is a chunk run the index is the ID 
 *              for this chunk process so it knows what work to do. An index of -1 
 *              means this is a merge run that takes all the chunk files to produce 
 *              the finished output. 
 *
 * @param size The chunk size of this process. Unless this is a chunk run this is 
 *             always 1. If this is a chunk run this is the total number of chunk 
 *             processes running the analytic process. 
 *
 * @return Pointer to the new manager object. 
 */
std::unique_ptr<Ace::Analytic::AbstractManager> AbstractManager::makeManager(quint16 type, int index, int size)
{
   if ( size > 1 )
   {
      if ( index < 0 )
      {
         return unique_ptr<AbstractManager>(new Merge(type,size));
      }
      else
      {
         return unique_ptr<AbstractManager>(new Chunk(type,index,size));
      }
   }
   else if ( QMPI::instance().size() > 1 )
   {
      if ( QMPI::instance().isMaster() )
      {
         return unique_ptr<AbstractManager>(new MPIMaster(type));
      }
      else
      {
         return unique_ptr<AbstractManager>(new MPISlave(type));
      }
   }
   else
   {
      return unique_ptr<AbstractManager>(new Single(type));
   }
}






/*!
 * Constructs a new manager object with the given analytic type. If the given type 
 * is out of range then an exception is thrown. 
 *
 * @param type Analytic type that is used for this manager's analytic run. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given analytic type is out of range then throw an exception, else go 
 *    to the next step. 
 *
 * 2. Create a new abstract analytic object of the given type and set it as this 
 *    manager's analytic. 
 *
 * 3. Create a new abstract analytic input object for this manager and resize the 
 *    array of arguments to the analytic input size. 
 */
AbstractManager::AbstractManager(quint16 type)
{
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   if ( type >= factory.size() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("%1 is not a valid analytic type. (max is %2)")
                   .arg(type)
                   .arg(factory.size() - 1));
      throw e;
   }
   _analytic = factory.make(type).release();
   _analytic->setParent(this);
   _input = _analytic->makeInput();
   _inputs.resize(_input->size());
}






/*!
 * Returns the number of arguments this manager's analytic has for user input. 
 *
 * @return Number of analytic arguments for this manager. 
 */
int AbstractManager::size() const
{
   return _input->size();
}






/*!
 * Returns the argument type for the given argument index of this manager's 
 * analytic. 
 *
 * @param index Argument index whose type is returned. 
 *
 * @return Argument type for the given index. 
 */
EAbstractAnalytic::Input::Type AbstractManager::type(int index) const
{
   return _input->type(index);
}






/*!
 * Returns data about an analytic argument with the given role and index. 
 *
 * @param index Argument index whose data is returned with the given role. 
 *
 * @param role Role of the data that is returned with the given index. 
 *
 * @return Data with the given role and index. 
 */
QVariant AbstractManager::data(int index, EAbstractAnalytic::Input::Role role) const
{
   return _input->data(index,role);
}






/*!
 * Sets the analytic argument to the given value with the given index. The value is 
 * not given to the underlying analytic until initialize is called. For file and 
 * data arguments the file path should be given as a string. 
 *
 * @param index Argument index whose value is set to the given value. 
 *
 * @param value Value that the argument with the given index is set to. 
 */
void AbstractManager::set(int index, const QVariant& value)
{
   _inputs[index] = value;
}






/*!
 * Finalizes all input to this manager's analytic and calls the analytic object's 
 * initialize interface. This also emits a signal to call the start slot once the 
 * event loop is started. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set analytic input arguments for basic types, files, and data objects. 
 *
 * 2. Clear the analytic input pointer, run the initialize interface of this 
 *    manager's analytic, and prepare the start slot to be called once the event 
 *    loop begins. 
 */
void AbstractManager::initialize()
{
   inputBasic();
   inputFiles();
   inputData();
   _input = nullptr;
   _analytic->initialize();
   QTimer::singleShot(0,this,&AbstractManager::start);
}






/*!
 * Called to request this manager terminate its analytic run before completion. 
 */
void AbstractManager::terminationRequested()
{
   deleteLater();
}






/*!
 * Called to Complete this manager's analytic run, signaling completion and 
 * requesting deletion. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call the finish interface for this manager's analytic. 
 *
 * 2. Call all this object's output abstract data finish interfaces and set their 
 *    user metadata to an empty object. 
 *
 * 3. Signal this manager is finished with execution and call on qt to delete this 
 *    manager. 
 */
void AbstractManager::finish()
{
   _analytic->finish();
   for (auto data: qAsConst(_outputData))
   {
      data->data()->finish();
      data->setUserMeta(EMetadata(EMetadata::Object));
   }
   emit finished();
}






/*!
 * This interface is called once to begin the analytic run for this manager after 
 * all argument input has been set. The default implementation does nothing. 
 */
void AbstractManager::start()
{}






/*!
 * This interface opens a new file set to write only and truncate with the given 
 * path. If the file fails to open then an exception is thrown. The default 
 * implementation opens the file unless the path is an empty string. 
 *
 * @param path Path to the output file that is opened. 
 *
 * @return Pointer to qt file device of opened file or null if no path given. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given path is empty then return a null pointer, else go to the next 
 *    step. 
 *
 * 2. Open the file with the given path set to write only and truncate, returning a 
 *    pointer to the qt file device. If opening the file failed then throw an 
 *    exception. 
 */
QFile* AbstractManager::addOutputFile(const QString& path)
{
   if ( path.isEmpty() )
   {
      return nullptr;
   }
   QFile* ret {new QFile(path,this)};
   if ( !ret->open(QIODevice::WriteOnly|QIODevice::Truncate) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed opening file %1: %2").arg(path).arg(ret->errorString()));
      throw e;
   }
   return ret;
}






/*!
 * This interface opens a new data object with the given path, erasing any data the 
 * file may have contained and returning a pointer to the new data object. The 
 * default implementation opens the data object unless the given path string is 
 * empty. 
 *
 * @param path Path to the output data object that is opened. 
 *
 * @param type Data type the new data object file is initialized to. 
 *
 * @param system The system metadata for the new data object. 
 *
 * @return Pointer to new data object with the given path or null if no path given. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given path is empty then return a null pointer, else go to the next 
 *    step. 
 *
 * 2. Create a new data object with the given path, type, and system data, 
 *    returning a pointer to the new object and setting its parent to this manager. 
 */
Ace::DataObject* AbstractManager::addOutputData(const QString& path, quint16 type, const EMetadata& system)
{
   if ( path.isEmpty() )
   {
      return nullptr;
   }
   return new Ace::DataObject(path,type,system,this);
}






/*!
 *
 * @param index  
 */
std::unique_ptr<EAbstractAnalytic::Block> AbstractManager::makeWork(int index)
{
   unique_ptr<EAbstractAnalytic::Block> ret {analytic()->makeWork(index)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null work block pointer."));
      throw e;
   }
   if ( ret->index() != index )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned work block with index %1 when it should be %2.")
                   .arg(ret->index())
                   .arg(index));
      throw e;
   }
   return ret;
}






/*!
 *
 * @param result  
 *
 * @param expectedIndex  
 */
void AbstractManager::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result, int expectedIndex)
{
   if ( result->index() != expectedIndex )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Given result block with index %1 when it should be %2.")
                   .arg(result->index())
                   .arg(expectedIndex));
      throw e;
   }
   analytic()->process(result.get());
   result.reset();
   int percentComplete {expectedIndex*100/analytic()->size()};
   if ( percentComplete != _percentComplete )
   {
      _percentComplete = percentComplete;
      emit progressed(_percentComplete);
   }
}






/*!
 * Returns a pointer to the abstract analytic for this manager. 
 *
 * @return Pointer to this manager's abstract analytic. 
 */
EAbstractAnalytic* AbstractManager::analytic()
{
   return _analytic;
}






/*!
 * Returns a read only pointer to the abstract analytic for this manager. 
 *
 * @return Read only pointer to this manager's abstract analytic. 
 */
const EAbstractAnalytic* AbstractManager::analytic() const
{
   return _analytic;
}






/*!
 * Sets all basic arguments to this manager's abstract analytic input object, 
 * excluding file or data arguments. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all of this manager's array of argument settings for the 
 *    proceeding steps. 
 *
 * 2. If the argument type is not a file or data object then set the argument using 
 *    the set interface of this manager's abstract analytic input object. 
 */
void AbstractManager::inputBasic()
{
   for (int i = 0; i < _inputs.size() ;++i)
   {
      switch(_input->type(i))
      {
      case EAbstractAnalytic::Input::Type::FileIn:
      case EAbstractAnalytic::Input::Type::FileOut:
      case EAbstractAnalytic::Input::Type::DataIn:
      case EAbstractAnalytic::Input::Type::DataOut:
         break;
      default:
         _input->set(i,_inputs.at(i));
         break;
      }
   }
}






/*!
 * Sets input and output file arguments to this manager's abstract analytic input 
 * object, opening the files in the process. If a file path argument is blank then 
 * no file is opened and the argument is not set. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all of this manager's array of argument settings for the 
 *    proceeding steps. 
 *
 * 2. If the argument type is an input file then call the add input file method, 
 *    setting the abstract analytic input if it returns a qt file device, else go 
 *    to the next step. 
 *
 * 3. If the argument type is an output file then call the add output file method, 
 *    setting the abstract analytic input if it returns a qt file device. 
 */
void AbstractManager::inputFiles()
{
   for (int i = 0; i < _inputs.size() ;++i)
   {
      switch(_input->type(i))
      {
      case EAbstractAnalytic::Input::Type::FileIn:
         if ( QFile* file = addInputFile(_inputs.at(i).toString()) )
         {
            _input->set(i,file);
         }
         break;
      case EAbstractAnalytic::Input::Type::FileOut:
         if ( QFile* file = addOutputFile(_inputs.at(i).toString()) )
         {
            _input->set(i,file);
         }
         break;
      default:
         break;
      }
   }
}






/*!
 * Opens an existing file set to read only with the given path. If the file fails 
 * to open then an exception is thrown. If the given path is an empty string this 
 * does nothing. 
 *
 * @param path Path the the input file that is opened. 
 *
 * @return Pointer to qt file device of opened file or null if no path given. 
 *
 *
 * Steps of Operation: 
 *
 * 1. if the given path string is empty then return a null pointer, else go to the 
 *    next step. 
 *
 * 2. Create a new qt file device with the given path and setting its parent as 
 *    this manager. 
 *
 * 3. Open the file with the qt file device. If opening failed then throw an 
 *    exception, else return a pointer to the qt file device. 
 */
QFile* AbstractManager::addInputFile(const QString& path)
{
   if ( path.isEmpty() )
   {
      return nullptr;
   }
   QFile* ret {new QFile(path,this)};
   if ( !ret->open(QIODevice::ReadOnly) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed opening file %1: %2").arg(path).arg(ret->errorString()));
      throw e;
   }
   return ret;
}






/*!
 * Sets input and output data object arguments to this manager's abstract analytic 
 * input object, opening the data objects in the process. If a data object path 
 * argument is blank then no data object is opened and the argument is not set. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize all input data objects, build the system metadata for output data 
 *    objects, and initialize all output data objects. 
 */
void AbstractManager::inputData()
{
   EMetadata system{inputDataIn()};
   inputDataOut(system);
}






/*!
 * Sets input data object arguments to this manager's abstract analytic input 
 * object, opening the data objects and constructing the system metadata for output 
 * data objects in the process. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all of this manager's array of argument settings for steps 2 
 *    and 3. 
 *
 * 2. If the argument type is an input data object then go to the next step, else 
 *    iterate to next argument. 
 *
 * 3. Call the add input data method and if it returns a valid data object pointer 
 *    then set the abstract analytic input and add the data object pointer to a 
 *    list of data inputs. 
 *
 * 4. Build and return the system metadata used for output data objects from the 
 *    list of input data objects. 
 */
EMetadata AbstractManager::inputDataIn()
{
   QList<Ace::DataObject*> inputs;
   for (int i = 0; i < _inputs.size() ;++i)
   {
      if ( _input->type(i) == EAbstractAnalytic::Input::Type::DataIn )
      {
         if ( Ace::DataObject* object = addInputData(_inputs.at(i).toString()) )
         {
            _input->set(i,object->data());
            inputs << object;
         }
      }
   }
   return buildMeta(inputs);
}






/*!
 * Opens an existing data object for read only with the given path, returning a 
 * pointer to the data object. If the given path is an empty string this does 
 * nothing. 
 *
 * @param path Path the the input data object that is opened. 
 *
 * @return Pointer to the data object with the given path or null if no path given. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given path string is empty then return a null pointer, else go to the 
 *    next step. 
 *
 * 2. Open an existing data object with the given path, setting the data object's 
 *    parent as this manager and return a pointer to it. 
 */
Ace::DataObject* AbstractManager::addInputData(const QString& path)
{
   if ( path.isEmpty() )
   {
      return nullptr;
   }
   return new Ace::DataObject(path,this);
}






/*!
 * Builds the system metadata for new output data objects with the given list of 
 * input data objects. 
 *
 * @param inputs List of input data objects used to build the metadata. 
 *
 * @return System metadata for new output data objects. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and return a metadata object type, inserting the input, command, uuid, 
 *    and version keys with the input and command sections of system metadata. 
 */
EMetadata AbstractManager::buildMeta(const QList<Ace::DataObject*>& inputs)
{
   EMetadata ret(EMetadata::Object);
   ret.toObject().insert("uuid",EMetadata(QUuid::createUuid().toString()));
   ret.toObject().insert("version",buildMetaVersion());
   ret.toObject().insert("input",buildMetaInput(inputs));
   ret.toObject().insert("command",buildMetaCommand());
   return ret;
}






/*!
 */
EMetadata AbstractManager::buildMetaVersion()
{
   Settings& settings {Settings::instance()};
   EMetadata ret(EMetadata::Object);
   ret.toObject().insert("ace",EMetadata(settings.versionString()));
   ret.toObject().insert(Settings::application(),EMetadata(settings.versionString()));
   return ret;
}






/*!
 * Builds the input section of the system metadata for new output data objects with 
 * the given list of input data objects. 
 *
 * @param inputs List of input data objects used to build the metadata. 
 *
 * @return Input section of system metadata for new output data objects. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all input data objects in given list of them for step 2. 
 *
 * 2. Create a new metadata object type, inserting the system and user keys for the 
 *    system and user metadata of the input data object, and insert the new 
 *    metadata object type into the return metadata object type using the input 
 *    data object's filename as the key. 
 *
 * 3. Return the metadata object type containing a list of all input data objects 
 *    with their corresponding system and user metadata. 
 */
EMetadata AbstractManager::buildMetaInput(const QList<Ace::DataObject*>& inputs)
{
   EMetadata ret(EMetadata::Object);
   for (auto input: inputs)
   {
      EMetadata inputMeta(EMetadata::Object);
      inputMeta.toObject().insert("system",input->systemMeta());
      inputMeta.toObject().insert("user",input->userMeta());
      ret.toObject().insert(input->fileName(),inputMeta);
   }
   return ret;
}






/*!
 * Builds the command section of the system metadata for new output data objects 
 * using this managers list of set arguments. 
 *
 * @return Command section of system metadata for new output data objects. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all of this manager's array of argument settings for step 2. 
 *
 * 2. Insert a new metadata string type containing the value of the argument into 
 *    the return metadata object type using the command line name of the argument 
 *    as the key. 
 *
 * 3. Return the metadata object type containing a list of all argument settings 
 *    for this analytic run. 
 */
EMetadata AbstractManager::buildMetaCommand()
{
   EMetadata ret(EMetadata::Object);
   for (int i = 0; i < _inputs.size() ;++i)
   {
      EMetadata value(EMetadata::String);
      value.toString() = _inputs.at(i).toString();
      ret.toObject().insert(_input->data(i,EAbstractAnalytic::Input::CommandLineName).toString(),value);
   }
   return ret;
}






/*!
 * Sets output data object arguments to this manager's abstract analytic input 
 * object, opening the new data objects with the given system metadata in the 
 * process. 
 *
 * @param system System metadata used for creating new output data objects. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all of this manager's array of argument settings for the 
 *    proceeding steps. 
 *
 * 2. If the argument type is an output data object then go to the next step, else 
 *    iterate to the next argument. 
 *
 * 3. Call the add output data method with the given system metadata and if it 
 *    returns a valid pointer then set the abstract analytic input argument. 
 */
void AbstractManager::inputDataOut(const EMetadata& system)
{
   for (int i = 0; i < _inputs.size() ;++i)
   {
      if (_input->type(i) == EAbstractAnalytic::Input::Type::DataOut )
      {
         if ( Ace::DataObject* object = addOutputData(_inputs.at(i).toString(),_input->data(i,EAbstractAnalytic::Input::Role::DataType).toUInt(),system) )
         {
            _input->set(i,object->data());
            _outputData << object;
         }
      }
   }
}
