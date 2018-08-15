#include "eexception.h"



//






/*!
 * Returns the function name where this exception was thrown. 
 *
 * @return Function name where this function was thrown. 
 */
QString EException::functionName() const noexcept
{
   return _functionName;
}






/*!
 * Returns the file name where this exception was thrown. 
 *
 * @return File name where this exception was thrown. 
 */
QString EException::fileName() const noexcept
{
   return _fileName;
}






/*!
 * Returns the line number where this exception was thrown. 
 *
 * @return Line number where this exception was thrown. 
 */
int EException::line() const noexcept
{
   return _line;
}






/*!
 * Returns the title describing the error that caused this exception. 
 *
 * @return Title describing the error that caused this exception. 
 */
QString EException::title() const noexcept
{
   return _title;
}






/*!
 * Returns the detailed description of what caused this exception. 
 *
 * @return Detailed description of what caused this exception. 
 */
QString EException::details() const noexcept
{
   return _details;
}






/*!
 * Sets the function name where this exception is thrown. This should only be set 
 * where the exception is created and thrown, unless it is caught elsewhere, 
 * modified, and thrown again. 
 *
 * @param functionName Function name where this exception is thrown. 
 */
void EException::setFunctionName(const QString& functionName) noexcept
{
   _functionName = functionName;
}






/*!
 * Sets the file name where this exception is thrown. This should only be set where 
 * the exception is created and thrown, unless it is caught elsewhere, modified, 
 * and thrown again. 
 *
 * @param fileName File name where this exception is thrown. 
 */
void EException::setFileName(const QString& fileName) noexcept
{
   _fileName = fileName;
}






/*!
 * Sets the line number where this exception is thrown. This should only be set 
 * where the exception is created and thrown, unless it is caught elsewhere, 
 * modified, and thrown again. 
 *
 * @param line Line number where this exception is thrown. 
 */
void EException::setLine(int line) noexcept
{
   _line = line;
}






/*!
 * This sets the title describing the error that caused this exception. 
 *
 * @param title Title describing what caused this exception. 
 */
void EException::setTitle(const QString& title) noexcept
{
   _title = title;
}






/*!
 * This sets the detailed description of what caused this exception. 
 *
 * @param details Detailed description of what caused this exception. 
 */
void EException::setDetails(const QString& details) noexcept
{
   _details = details;
}
