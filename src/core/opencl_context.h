#ifndef OPENCL_CONTEXT_H
#define OPENCL_CONTEXT_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"

namespace OpenCL
{




/*!
 * This contains an OpenCL context. This is a very basic class that simply maintains ownership over
 * an underlying context along with providing basic information about it such as the list of devices
 * associated with it. This class in turn is used by other OpenCL classes that derive from an OpenCL
 * context.
 */
class Context
:
    public QObject
{
    Q_OBJECT


/****************/
/* Constructors */
/****************/
public:


    /*!
     * Constructs a new OpenCL context with the given list of devices and optional parent. If the
     * given device list is empty or all devices are not part of the same platform then an exception
     * is thrown.
     *
     * @param devices : List of pointers to devices this context is created from. All devices must
     *                  be part of the given platform.
     *
     * @param parent : Optional parent for this new context.
     */
    Context(
        const QList<Device*>& devices
        ,QObject* parent=nullptr
    );


/**************/
/* Destructor */
/**************/
public:


    /*!
     * Releases the underlying OpenCL context that this object represents.
     */
    virtual ~Context() override final;


/***********/
/* Methods */
/***********/
public:


    /*!
     * Returns the OpenCL context ID of this object.
     *
     * @return : OpenCL context ID of this object.
     */
    cl_context id() const;


    /*!
     * Returns a read only reference to the list of device pointers which is part of this context.
     *
     * @return : Read only reference to list of devices of this context.
     */
    const QList<Device*>& devices() const;


/*************/
/* Variables */
/*************/
private:


    /*!
     * The OpenCL context ID of this object.
     */
    cl_context _id;


    /*!
     * Pointer list of devices that are part of this context.
     */
    QList<Device*> _devices;
};

}

#endif
