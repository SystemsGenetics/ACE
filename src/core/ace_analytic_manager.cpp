#include "ace_analytic_manager.h"
#include "ace_analytic_singlerun.h"
#include "ace_dataobject.h"
#include "eabstractanalyticfactory.h"
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
std::unique_ptr<Ace::Analytic::Manager> Manager::makeManager(quint16 type, int index, int size)
{
   Q_UNUSED(index)
   Q_UNUSED(size)
   Q_UNUSED(type)
   return unique_ptr<Manager>(new SingleRun(type));
   // TODO; for now just implemented serial
}






/*!
 * Constructs a new manager object with the given analytic type. If the given type 
 * is out of range then an exception is thrown. 
 *
 * @param type Analytic type that is used for this manager's analytic run. 
 */
Manager::Manager(quint16 type)
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
int Manager::size() const
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
EAbstractAnalytic::Input::Type Manager::type(int index) const
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
QVariant Manager::data(int index, EAbstractAnalytic::Input::Role role) const
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
void Manager::set(int index, const QVariant& value)
{
   _inputs[index] = value;
}






/*!
 * Finalizes all input to this manager's analytic and calls the analytic object's 
 * initialize interface. This also emits a signal to call the start slot once the 
 * event loop is started. 
 */
void Manager::initialize()
{
   inputBasic();
   inputFiles();
   inputData();
   _input = nullptr;
   _analytic->initialize();
   QTimer::singleShot(0,this,&Manager::start);
}






/*!
 * Called to request this manager terminate its analytic run before completion. 
 */
void Manager::terminationRequested()
{
   deleteLater();
}






/*!
 * Called to Complete this manager's analytic run, signaling completion and 
 * requesting deletion. 
 */
void Manager::finish()
{
   _analytic->finish();
   for (auto data: qAsConst(_outputData)) data->finish();
   emit finished();
   deleteLater();
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
QFile* Manager::addOutputFile(const QString& path)
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
 */
Ace::DataObject* Manager::addOutputData(const QString& path, quint16 type, const EMetadata& system)
{
   if ( path.isEmpty() )
   {
      return nullptr;
   }
   return new Ace::DataObject(path,type,system,this);
}






/*!
 * Returns a pointer to the abstract analytic for this manager. 
 *
 * @return Pointer to this manager's abstract analytic. 
 */
EAbstractAnalytic* Manager::analytic()
{
   return _analytic;
}






/*!
 * Returns a read only pointer to the abstract analytic for this manager. 
 *
 * @return Read only pointer to this manager's abstract analytic. 
 */
const EAbstractAnalytic* Manager::analytic() const
{
   return _analytic;
}






/*!
 * Sets all basic arguments to this manager's abstract analytic input object, 
 * excluding file or data arguments. 
 */
void Manager::inputBasic()
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
 */
void Manager::inputFiles()
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
 */
QFile* Manager::addInputFile(const QString& path)
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
 */
void Manager::inputData()
{
   EMetadata system{inputDataIn()};
   inputDataOut(system);
}






/*!
 * Sets input data object arguments to this manager's abstract analytic input 
 * object, opening the data objects and constructing the system metadata for output 
 * data objects in the process. 
 */
EMetadata Manager::inputDataIn()
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
 */
Ace::DataObject* Manager::addInputData(const QString& path)
{
   if ( path.isEmpty() )
   {
      return nullptr;
   }
   return new Ace::DataObject(path,this);
}






/*!
 * Sets output data object arguments to this manager's abstract analytic input 
 * object, opening the new data objects with the given system metadata in the 
 * process. 
 *
 * @param system System metadata used for creating new output data objects. 
 */
void Manager::inputDataOut(const EMetadata& system)
{
   for (int i = 0; i < _inputs.size() ;++i)
   {
      if (_input->type(i) == EAbstractAnalytic::Input::Type::DataOut )
      {
         if ( Ace::DataObject* object = addOutputData(_inputs.at(i).toString(),_input->data(i,EAbstractAnalytic::Input::Role::DataType).toUInt(),system) )
         {
            _input->set(i,object->data());
         }
      }
   }
}






/*!
 * Builds the system metadata for new output data objects with the given list of 
 * input data objects. 
 *
 * @param inputs List of input data objects used to build the metadata. 
 *
 * @return System metadata for new output data objects. 
 */
EMetadata Manager::buildMeta(const QList<Ace::DataObject*>& inputs)
{
   EMetadata ret(EMetadata::Object);
   ret.toObject().insert("input",buildMetaInput(inputs));
   ret.toObject().insert("command",buildMetaCommand());
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
EMetadata Manager::buildMetaInput(const QList<Ace::DataObject*>& inputs)
{
   EMetadata ret(EMetadata::Object);
   for (auto input: inputs)
   {
      EMetadata inputMeta(EMetadata::Object);
      inputMeta.toObject().insert("system",input->systemMeta());
      inputMeta.toObject().insert("user",input->userMeta());
      ret.toObject().insert(input->path(),inputMeta);
   }
   return ret;
}






/*!
 * Builds the command section of the system metadata for new output data objects 
 * using this managers list of set arguments. 
 */
EMetadata Manager::buildMetaCommand()
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
