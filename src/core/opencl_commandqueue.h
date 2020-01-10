#ifndef OPENCL_COMMANDQUEUE_H
#define OPENCL_COMMANDQUEUE_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"

namespace OpenCL
{




/*!
 * This contains an OpenCL command queue. This is a very basic class that simply maintains ownership
 * of an underlying command queue. The main purpose of this class is to be used by other classes
 * that add commands to its underlying command queue.
 */
class CommandQueue
:
    public QObject
{
    Q_OBJECT


/****************/
/* Constructors */
/****************/
public:


    /*!
     * Constructs a new command queue with the given context, device, and optional parent. If the
     * given device is not part of the given context then an exception is thrown.
     *
     * @param context : Pointer to context this command queue is created from.
     *
     * @param device : Pointer to device this command queue is created from.
     *
     * @param parent : Optional parent for this new command queue.
     */
    explicit CommandQueue(
        Context* context
        ,Device* device
        ,QObject* parent=nullptr
    );


/**************/
/* Destructor */
/**************/
public:


    /*!
     * Releases the underlying OpenCL command queue resource.
     */
    virtual ~CommandQueue() override final;


/***********/
/* Methods */
/***********/
public:


    /*!
     * Returns the OpenCL command queue ID of this object.
     *
     * @return : OpenCL command queue ID of this object.
     */
    cl_command_queue id() const;


    /*!
     * Returns a pointer to the device that this command queue uses.
     *
     * @return : Pointer to device for this command queue.
     */
    Device* device() const;


    /*!
     * Blocks until all previous commands added to this command queue are done.
     */
    void wait();


/*************/
/* Variables */
/*************/
private:


    /*!
     * The OpenCL command queue ID of this object.
     */
    cl_command_queue _id;


    /*!
     * Pointer to the device this command queue uses.
     */
    Device* _device;
};

}

#endif
