#ifndef OPENCL_PROGRAM_H
#define OPENCL_PROGRAM_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"

namespace OpenCL
{




/*!
 * This contains an OpenCL program. The primary purpose of this class is to build a given list of
 * files assumed to be OpenCL kernel source code. All devices part of the context this program is
 * derived from compiles all the source files given. The only other purpose of this class is to
 * provide its OpenCL program ID to create kernels from the built program kernel code.
 */
class Program
:
    public QObject
{
    Q_OBJECT


/****************/
/* Constructors */
/****************/
public:


    /*!
     * Construct a new program with the given context, list of OpenCL kernel source code file paths,
     * and optional parent. If any OpenCL error occurs then an exception is thrown.
     *
     * @param context : Pointer to context this program is created from.
     *
     * @param paths : List of file paths containing OpenCL kernel source code that this program
     *                builds for all devices of the given context.
     *
     * @param parent : Optional parent for this new program.
     */
    explicit Program(
        Context* context
        ,const QStringList& paths
        ,QObject* parent=nullptr
    );


/**************/
/* Destructor */
/**************/
public:


    /*!
     * Releases the underlying OpenCL program that this object represents.
     */
    ~Program();


/***********/
/* Methods */
/***********/
public:


    /*!
     * Returns the OpenCL program ID of this object.
     *
     * @return : OpenCL program ID of this object.
     */
    cl_program id() const;


/***********/
/* Methods */
/***********/
private:


    /*!
     * Builds an OpenCL program using the given device. If building fails then an exception is
     * thrown.
     *
     * @param device : Pointer to device that is used to build all the source code.
     */
    void build(
        Device* device
    );


    /*!
     * Returns the most recent build log from the given device that is generated whenever compiling
     * is attempted. If any OpenCL error occurs then an exception is thrown.
     *
     * @param device : Pointer to device whose most recent build log is returned.
     *
     * @return : Most recent build log of the given device.
     */
    QString buildLog(
        Device* device
    ) const;


    /*!
     * Reads in an OpenCL kernel source file with the given path, returning a C style string of the
     * read in source code and setting the string's size with the given pointer. If the source file
     * fails to open then an exception is thrown.
     *
     * @param path : Path to the OpenCL source code that is read in.
     *
     * @param size : Pointer to size variable that is set to the length of the C style string
     *               returned.
     *
     * @return : Pointer to C style string of OpenCL source code.
     */
    char* readSourceFile(
        const QString& path
        ,size_t* size
    );


/*************/
/* Variables */
/*************/
private:


    /*!
     * The OpenCL program ID of this object.
     */
    cl_program _id;
};

}

#endif
