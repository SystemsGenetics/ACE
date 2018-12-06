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
#include "edebug.h"



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
   EDEBUG_FUNC(type,index,size)

   // If the given size is greater than 1 and the given index is less than zero then 
   // return a new merge manager, else if the index is greater to or equal to 0 then 
   // return a new chunk manager, else go to the next step. 
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

   // If the MPI size is greater than 1 and this is the master process then return a 
   // new MPI master manager, else if this is a slave process return a new MPI slave 
   // manager, else go to the next step. 
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

   // Return a new single manager. 
   else
   {
      return unique_ptr<AbstractManager>(new Single(type));
   }
}






/*!
 * Returns the analytic type this analytic manager contains. 
 *
 * @return Analytic type this analytic manager contains. 
 */
quint16 AbstractManager::analyticType() const
{
   EDEBUG_FUNC(this)

   return _type;
}






/*!
 * Returns the number of arguments this manager's analytic has for user input. 
 *
 * @return Number of analytic arguments for this manager. 
 */
int AbstractManager::size() const
{
   EDEBUG_FUNC(this)

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
   EDEBUG_FUNC(this,index)

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
   EDEBUG_FUNC(this,role);

   return _input->data(index,role);
}






/*!
 * Returns a list of all command line arguments this manager's analytic provides as 
 * input arguments. 
 *
 * @return List of all command line arguments for this manager's analytic. 
 */
QList<QString> AbstractManager::commandLineArguments() const
{
   // Build the list of command line arguments from this manager's analytic input 
   // instance. 
   QList<QString> ret;
   for (int i = 0; i < _input->size() ;++i)
   {
      ret << _input->data(i,EAbstractAnalytic::Input::CommandLineName).toString();
   }

   // Return the command line argument list. 
   return ret;
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
   EDEBUG_FUNC(this,index,value)

   _inputs[index] = value;
}






/*!
 * Called to Finalize all input to this manager's analytic and calls the analytic 
 * object's initialize interface. This also emits a signal to call the start slot 
 * once the event loop is started. 
 */
void AbstractManager::initialize()
{
   EDEBUG_FUNC(this)

   // Set analytic input arguments for basic types, files, and data objects. 
   inputBasic();
   inputFiles();
   inputData();

   // Clear the analytic input pointer, run the initialize interface of this 
   // manager's analytic, and prepare the start slot to be called once the event loop 
   // begins. 
   _input = nullptr;
   _analytic->initialize();
   QTimer::singleShot(0,this,&AbstractManager::start);
}






/*!
 * Called to request this manager terminate its analytic run before completion. 
 */
void AbstractManager::terminationRequested()
{
   EDEBUG_FUNC(this)

   deleteLater();
}






/*!
 * Called to Complete this manager's analytic run, signaling completion and 
 * requesting deletion. 
 */
void AbstractManager::finish()
{
   EDEBUG_FUNC(this)

   // Call the finish interface for this manager's analytic. 
   _analytic->finish();

   // Call all this object's output abstract data finish interfaces and then their 
   // finalize methods. 
   for (auto data: qAsConst(_outputData))
   {
      data->data()->finish();
      data->finalize();
   }

   // Signal this manager is finished with execution and ready to be deleted. 
   emit finished();
}






/*!
 * This interface opens a new file set to write only and truncate with the given 
 * path. If the file fails to open then an exception is thrown. The default 
 * implementation opens the file unless the path is an empty string. 
 *
 * @param path Path to the output file that is opened. 
 *
 * @return Pointer to qt file device of opened file or null if no path given. 
 */
QFile* AbstractManager::addOutputFile(const QString& path)
{
   EDEBUG_FUNC(this,path)

   // If the given path is empty then return a null pointer, else go to the next 
   // step. 
   if ( path.isEmpty() )
   {
      return nullptr;
   }

   // Open the file with the given path set to write only and truncate, returning a 
   // pointer to the qt file device. If opening the file failed then throw an 
   // exception. 
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
 */
Ace::DataObject* AbstractManager::addOutputData(const QString& path, quint16 type, const EMetadata& system)
{
   EDEBUG_FUNC(this,path,type,&system)

   // If the given path is empty then return a null pointer, else go to the next 
   // step. 
   if ( path.isEmpty() )
   {
      return nullptr;
   }

   // Create a new data object with the given path, type, and system data, returning 
   // a pointer to the new object and setting its parent to this manager. 
   return new Ace::DataObject(path,type,system,this);
}






/*!
 * Constructs a new manager object with the given analytic type. If the given type 
 * is out of range then an exception is thrown. This is protected because this 
 * class should never created without an implementation. 
 *
 * @param type Analytic type that is used for this manager's analytic run. 
 */
AbstractManager::AbstractManager(quint16 type):
   _type(type)
{
   EDEBUG_FUNC(this,type)

   // If the given analytic type is out of range then throw an exception, else go to 
   // the next step. 
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   if ( _type >= factory.size() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("%1 is not a valid analytic type. (max is %2)")
                   .arg(_type)
                   .arg(factory.size() - 1));
      throw e;
   }

   // Create a new abstract analytic object of the given type and set it as this 
   // manager's analytic. 
   _analytic = factory.make(_type).release();
   _analytic->setParent(this);

   // Setup the abstract input and input fields. 
   setupInput();
}






/*!
 * Returns a new work block from this manager's analytic with the given index. This 
 * also does error checking on the new work block. 
 *
 * @param index  
 *
 * @return Pointer to a new work block with the given index. 
 */
std::unique_ptr<EAbstractAnalytic::Block> AbstractManager::makeWork(int index)
{
   EDEBUG_FUNC(this,index)

   // Call this manager's analytic interface to make a new work block with the given 
   // index. If the analytic returned a null pointer or the work block has an 
   // incorrect index then throw an exception, else return the new work block. 
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
 * Processes the given result block with this manager's analytic. This also does 
 * error checking and determines the progress of this analytic run. 
 *
 * @param result The result block that is processed by this manager's analytic. 
 *
 * @param expectedIndex The expected index that should be equal to the given result 
 *                      block's index. 
 */
void AbstractManager::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result, int expectedIndex)
{
   EDEBUG_FUNC(this,result.get(),expectedIndex)

   // If the given result block's index is not equal to the expected index then throw 
   // an exception, else go to the next step. 
   if ( result->index() != expectedIndex )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Given result block with index %1 when it should be %2.")
                   .arg(result->index())
                   .arg(expectedIndex));
      throw e;
   }

   // Process the result block with this manager's analytic and then delete it. 
   analytic()->process(result.get());
   result.reset();

   // Determine the progress made from processing this result block. If the percent 
   // complete has changed since last time this was called then emit the progressed 
   // signal. 
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
   EDEBUG_FUNC(this)

   return _analytic;
}






/*!
 * Returns a read only pointer to the abstract analytic for this manager. 
 *
 * @return Read only pointer to this manager's abstract analytic. 
 */
const EAbstractAnalytic* AbstractManager::analytic() const
{
   EDEBUG_FUNC(this)

   return _analytic;
}






/*!
 * This interface is called once to begin the analytic run for this manager after 
 * all argument input has been set. The default implementation does nothing. 
 */
void AbstractManager::start()
{
   EDEBUG_FUNC(this)
}






/*!
 * Initialize the abstract analytic input and input fields for this manager. 
 */
void AbstractManager::setupInput()
{
   EDEBUG_FUNC(this)

   // Create a new abstract analytic input from this manager's analytic, resize the 
   // array of inputs to the argument size of the abstract input object, and then 
   // populate all inputs with their default values. 
   _input = _analytic->makeInput();
   _inputs.resize(_input->size());
   for (int i = 0; i < _input->size() ;++i)
   {
      _inputs[i] = _input->data(i,EAbstractAnalytic::Input::Default);
   }
}






/*!
 * Sets all basic arguments to this manager's abstract analytic input object, 
 * excluding file or data arguments. 
 */
void AbstractManager::inputBasic()
{
   EDEBUG_FUNC(this)

   // Iterate through all of this manager's array of argument settings for the 
   // proceeding steps. 
   for (int i = 0; i < _inputs.size() ;++i)
   {
      // If the argument type is not a file or data object then set the argument using 
      // the set interface of this manager's abstract analytic input object. 
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
 */
void AbstractManager::inputFiles()
{
   EDEBUG_FUNC(this)

   // Iterate through all of this manager's array of argument settings for the 
   // proceeding steps. 
   for (int i = 0; i < _inputs.size() ;++i)
   {
      switch(_input->type(i))
      {
      // If the argument type is an input file then call the add input file method, 
      // setting the abstract analytic input if it returns a qt file device, else go to 
      // the next step. 
      case EAbstractAnalytic::Input::Type::FileIn:
         if ( QFile* file = addInputFile(_inputs.at(i).toString()) )
         {
            _input->set(i,file);
         }
         break;

      // If the argument type is an output file then call the add output file method, 
      // setting the abstract analytic input if it returns a qt file device. 
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
 */
QFile* AbstractManager::addInputFile(const QString& path)
{
   EDEBUG_FUNC(this,path)

   // if the given path string is empty then return a null pointer, else go to the 
   // next step. 
   if ( path.isEmpty() )
   {
      return nullptr;
   }

   // Create a new qt file device with the given path and setting its parent as this 
   // manager. 
   QFile* ret {new QFile(path,this)};

   // Open the file with the qt file device. If opening failed then throw an 
   // exception, else return a pointer to the qt file device. 
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
 */
void AbstractManager::inputData()
{
   EDEBUG_FUNC(this)

   // Initialize all input data objects, build the system metadata for output data 
   // objects, and initialize all output data objects. 
   EMetadata system{inputDataIn()};
   inputDataOut(system);
}






/*!
 * Sets input data object arguments to this manager's abstract analytic input 
 * object, opening the data objects and constructing the system metadata for output 
 * data objects in the process. 
 */
EMetadata AbstractManager::inputDataIn()
{
   EDEBUG_FUNC(this)

   // Iterate through all of this manager's array of argument settings for steps 2 
   // and 3. 
   QList<Ace::DataObject*> inputs;
   for (int i = 0; i < _inputs.size() ;++i)
   {
      // If the argument type is an input data object then go to the next step, else 
      // iterate to next argument. 
      if ( _input->type(i) == EAbstractAnalytic::Input::Type::DataIn )
      {
         // Call the add input data method and if it returns a valid data object pointer 
         // then set the abstract analytic input and add the data object pointer to a list 
         // of data inputs. 
         if ( Ace::DataObject* object = addInputData(_inputs.at(i).toString()) )
         {
            _input->set(i,object->data());
            inputs << object;
         }
      }
   }

   // Build and return the system metadata used for output data objects from the list 
   // of input data objects. 
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
 */
Ace::DataObject* AbstractManager::addInputData(const QString& path)
{
   EDEBUG_FUNC(this,path)

   // If the given path string is empty then return a null pointer, else go to the 
   // next step. 
   if ( path.isEmpty() )
   {
      return nullptr;
   }

   // Open an existing data object with the given path, setting the data object's 
   // parent as this manager and return a pointer to it. 
   return new Ace::DataObject(path,this);
}






/*!
 * Builds the system metadata for new output data objects with the given list of 
 * input data objects. 
 *
 * @param inputs List of input data objects used to build the metadata. 
 *
 * @return System metadata for new output data objects. 
 */
EMetadata AbstractManager::buildMeta(const QList<Ace::DataObject*>& inputs)
{
   EDEBUG_FUNC(this,&inputs)

   // Create and return a metadata object type, inserting the input, command, uuid, 
   // and version keys with the input and command sections of system metadata. 
   EMetadata ret(EMetadata::Object);
   ret.toObject().insert("uuid",EMetadata(QUuid::createUuid().toString()));
   ret.toObject().insert("version",buildMetaVersion());
   ret.toObject().insert("input",buildMetaInput(inputs));
   ret.toObject().insert("command",buildMetaCommand());
   return ret;
}






/*!
 * Builds the version section of the system metadata for new output data objects. 
 *
 * @return Version section of system metadata for new output data objects. 
 */
EMetadata AbstractManager::buildMetaVersion()
{
   EDEBUG_FUNC(this)

   // Create a new metadata object for the application setting the keys for the major 
   // version, minor version, and revision. 
   EMetadata application(EMetadata::Object);
   application.toObject().insert("major",EMetadata(Settings::appMajorVersion()));
   application.toObject().insert("minor",EMetadata(Settings::appMinorVersion()));
   application.toObject().insert("revision",EMetadata(Settings::appRevision()));
   Settings& settings {Settings::instance()};

   // Create a new metadata object for the ACE library setting the keys for the major 
   // version, minor version, and revision. 
   EMetadata ace(EMetadata::Object);
   ace.toObject().insert("major",EMetadata(settings._majorVersion));
   ace.toObject().insert("minor",EMetadata(settings._minorVersion));
   ace.toObject().insert("revision",EMetadata(settings._revision));

   // Create a metadata return object setting the ace and application name keys and 
   // setting them to the previously made objects, returning the created object. 
   EMetadata ret(EMetadata::Object);
   ret.toObject().insert("ace",ace);
   ret.toObject().insert(Settings::application(),application);
   return ret;
}






/*!
 * Builds the input section of the system metadata for new output data objects with 
 * the given list of input data objects. 
 *
 * @param inputs List of input data objects used to build the metadata. 
 *
 * @return Input section of system metadata for new output data objects. 
 */
EMetadata AbstractManager::buildMetaInput(const QList<Ace::DataObject*>& inputs)
{
   EDEBUG_FUNC(this,&inputs)

   // Iterate through all input data objects in given list of them for step 2. 
   EMetadata ret(EMetadata::Object);
   for (auto input: inputs)
   {
      // Create a new metadata object type, inserting the system and user keys for the 
      // system and user metadata of the input data object, and insert the new metadata 
      // object type into the return metadata object type using the input data object's 
      // raw path as the key. 
      EMetadata inputMeta(EMetadata::Object);
      inputMeta.toObject().insert("system",input->systemMeta());
      inputMeta.toObject().insert("user",input->userMeta());
      ret.toObject().insert(input->rawPath(),inputMeta);
   }

   // Return the metadata object type containing a list of all input data objects 
   // with their corresponding system and user metadata. 
   return ret;
}






/*!
 * Builds the command section of the system metadata for new output data objects 
 * using this managers list of set arguments. 
 *
 * @return Command section of system metadata for new output data objects. 
 */
EMetadata AbstractManager::buildMetaCommand()
{
   EDEBUG_FUNC(this)

   // Create a metadata object for the command options, populating it with all 
   // analytic input options using the command line name for the keys and values for 
   // the values. 
   EMetadata options(EMetadata::Object);
   for (int i = 0; i < _inputs.size() ;++i)
   {
      options.toObject().insert(_input->data(i,EAbstractAnalytic::Input::CommandLineName).toString()
                                ,_inputs.at(i).toString());
   }

   // Create a new metadata return object, setting the options key to the previously 
   // made options metadata and the analytic key to the full name of the analytic 
   // type this manager is using, returning the return object. 
   EMetadata ret(EMetadata::Object);
   ret.toObject().insert("options",options);
   ret.toObject().insert("analytic",EAbstractAnalyticFactory::instance().name(_type));
   return ret;
}






/*!
 * Sets output data object arguments to this manager's abstract analytic input 
 * object, opening the new data objects with the given system metadata in the 
 * process. 
 *
 * @param system System metadata used for creating new output data objects. 
 */
void AbstractManager::inputDataOut(const EMetadata& system)
{
   EDEBUG_FUNC(this,&system)

   // Iterate through all of this manager's array of argument settings for the 
   // proceeding steps. 
   for (int i = 0; i < _inputs.size() ;++i)
   {
      // If the argument type is an output data object then go to the next step, else 
      // iterate to the next argument. 
      if (_input->type(i) == EAbstractAnalytic::Input::Type::DataOut )
      {
         // Call the add output data method with the given system metadata and if it 
         // returns a valid pointer then set the abstract analytic input argument. 
         if ( Ace::DataObject* object = addOutputData(_inputs.at(i).toString(),_input->data(i,EAbstractAnalytic::Input::Role::DataType).toUInt(),system) )
         {
            _input->set(i,object->data());
            _outputData << object;
         }
      }
   }
}
