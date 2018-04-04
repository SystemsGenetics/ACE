#include "ace_analytic_manager.h"



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
   return nullptr;
   // TODO; for now just implemented serial
}






/*!
 * Constructs a new manager object with the given analytic type. 
 *
 * @param type Analytic type that is used for this manager's analytic run. 
 */
Manager::Manager(int type)
{}






/*!
 * Returns the number of arguments this manager's analytic has for user input. 
 *
 * @return Number of analytic arguments for this manager. 
 */
int Manager::size() const
{}






/*!
 * Returns the argument type for the given argument index of this manager's 
 * analytic. 
 *
 * @param index Argument index whose type is returned. 
 *
 * @return Argument type for the given index. 
 */
EAbstractAnalytic::Input::Type Manager::type(int index) const
{}






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
{}






/*!
 * Sets the analytic argument to the given value with the given index. If the 
 * argument with the given index is a file or data type then an exception is 
 * thrown. 
 *
 * @param index Argument index whose value is set to the given value. 
 *
 * @param value Value that the argument with the given index is set to. 
 */
void Manager::set(int index, const QVariant& value)
{}






/*!
 * Sets the input file analytic argument with the given index using the given path. 
 * If the file fails to open or the argument with the given index is not an input 
 * file type than an exception is thrown. 
 *
 * @param path Path the the input file that is opened. 
 */
void Manager::setInputFile(const QString& path)
{}






/*!
 * This interface sets the output file analytic argument with the given index using 
 * the given path. If the file fails to open or the argument with the given index 
 * is not an output file type than an exception is thrown. The default 
 * implementation always sets the output. 
 *
 * @param path Path the the output file that is opened. 
 */
void Manager::setOutputFile(const QString& path)
{}






/*!
 * Sets the input data analytic argument with the given index using the given path. 
 * If the data object fails to open or the argument with the given index is not an 
 * input data type than an exception is thrown. 
 *
 * @param path Path the the input data object that is opened. 
 */
void Manager::setInputData(const QString& path)
{}






/*!
 * This interface sets the output data analytic argument with the given index using 
 * the given path. If the data object fails to open or the argument with the given 
 * index is not an output data type than an exception is thrown. The default 
 * implementation always sets the output. 
 *
 * @param path Path the the output data object that is opened. 
 */
void Manager::setOutputData(const QString& path)
{}






/*!
 * Finalizes all input to this manager's analytic, deleting the analytic input 
 * object and preparing the analytic to run. No input methods can be called after 
 * calling this and if they are an exception is thrown. 
 */
void Manager::finish()
{}






/*!
 * Called to request this manager terminates its analytic run before completion. 
 */
void Manager::terminationRequested()
{}






/*!
 * Returns a pointer to the abstract analytic for this manager. 
 *
 * @return Pointer to this manager's abstract analytic. 
 */
EAbstractAnalytic* Manager::analytic()
{}






/*!
 * Returns a read only pointer to the abstract analytic for this manager. 
 *
 * @return Read only pointer to this manager's abstract analytic. 
 */
const EAbstractAnalytic* Manager::analytic() const
{}
