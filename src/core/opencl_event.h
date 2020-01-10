#ifndef OPENCL_EVENT_H
#define OPENCL_EVENT_H
#include <CL/cl.h>

namespace OpenCL
{




/*!
 * This can contain an OpenCL event or be set to null. An event this class contains can be copied or
 * moved to another instance of this class. Copying an instance with an OpenCL event simply means
 * copying the ID and calling the retain method. Once all instances referencing an OpenCL event are
 * deleted the underlying event is discarded. This class also provides utility methods for testing
 * or waiting on its event, along with testing to see if it is null.
 */
class Event
{


/****************/
/* Constructors */
/****************/
public:


    /*!
     * Constructs a new event that is set to null (no OpenCL event).
     */
    Event() = default;


    /*!
     * Constructs a new event using the given OpenCL event ID.
     *
     * @param id : The OpenCL event ID that this new event contains. This must be a valid OpenCL
     *             event that has one reference count.
     */
    Event(
        cl_event id
    );


    /*!
     * Constructs a new event that is a copy of the given event.
     *
     * @param other : The other event object that is copied to this new event.
     */
    Event(
        const Event& other
    );


    /*!
     * Constructs a new event taking the OpenCL event or null state of the given event.
     *
     * @param other : The other event object whose data is taken and set to this new event.
     */
    Event(
        Event&& other
    );


/**************/
/* Destructor */
/**************/
public:


    /*!
     * Releases the underlying OpenCL event, reducing its reference count by one.
     */
    ~Event();


/*************/
/* Operators */
/*************/
public:


    /*!
     * Makes a copy of the given event object, releasing any OpenCL event this event may currently
     * contain.
     *
     * @param other : The other event object that is copied to this event.
     */
    void operator=(
        const Event& other
    );


    /*!
     * Takes the OpenCL event or null state of the given event object releasing any OpenCL event
     * this event may currently contain.
     *
     * @param other : The other event object whose OpenCL event or null state is taken.
     */
    void operator=(
        Event&& other
    );


/***********/
/* Methods */
/***********/
public:


    /*!
     * Tests if this event is null and does not contain an OpenCL event.
     *
     * @return : True if this event is null or false otherwise.
     */
    bool isNull() const;


    /*!
     * Waits for this object's OpenCL event to complete, blocking until it does. If this event is
     * null then this returns immediately.
     */
    void wait() const;


    /*!
     * Tests if this object's OpenCL event is complete.
     *
     * @return : True if this event is complete or null, otherwise false.
     */
    bool isDone() const;


/***********/
/* Methods */
/***********/
private:


    /*!
     * Reduces this object's OpenCL reference count by one.
     */
    void release();


    /*!
     * Increases this object's OpenCL reference count by one.
     */
    void retain();


/*************/
/* Variables */
/*************/
private:


    /*!
     * Pointer the OpenCL event ID of this object or null of this object is null and contains no
     * event.
     */
    cl_event* _id {nullptr};
};

}

#endif
