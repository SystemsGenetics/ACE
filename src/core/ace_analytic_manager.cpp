#include "ace_analytic_manager.h"



using namespace Ace::Analytic;
//






/*!
 *
 * @param type  
 *
 * @param index  
 *
 * @param size  
 *
 * @return a 
 */
std::unique_ptr<Ace::Analytic::Manager> Manager::makeManager(int type, int index, int size)
{}






/*!
 *
 * @param type  
 */
Manager::Manager(int type)
{}






/*!
 *
 * @return a 
 */
int Manager::size() const
{}






/*!
 *
 * @param index  
 *
 * @return a 
 */
EAbstractAnalytic::Input::Type Manager::type(int index) const
{}






/*!
 *
 * @param index  
 *
 * @param role  
 *
 * @return a 
 */
QVariant Manager::data(int index, EAbstractAnalytic::Input::Role role) const
{}






/*!
 *
 * @param index  
 *
 * @param value  
 */
void Manager::set(int index, const QVariant& value)
{}






/*!
 *
 * @param index  
 *
 * @param file  
 */
void Manager::set(int index, QFile* file)
{}






/*!
 *
 * @param index  
 *
 * @param data  
 */
void Manager::set(int index, EAbstractData* data)
{}






/*!
 */
void Manager::finish()
{}






/*!
 *
 * @return a 
 */
EAbstractAnalytic* Manager::analytic()
{}






/*!
 *
 * @return a 
 */
const EAbstractAnalytic* Manager::analytic() const
{}
