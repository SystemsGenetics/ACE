#ifndef OPENCL_PLATFORM_H
#define OPENCL_PLATFORM_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"

namespace OpenCL
{




/*!
 * This provides information about a single OpenCL platform along with static methods for iterating
 * through all available platforms. This also provides methods for iterating through all OpenCL
 * devices of their respective platform. This class is not intended to be instantiated on its own
 * but instead queried through the static methods. Any pointer to a platform the static methods
 * return will exist for the lifetime of the ACE program.
 */
class Platform
:
    public QObject
{
    Q_OBJECT


/******************/
/* Static Methods */
/******************/
public:


    /*!
     * Returns the number of OpenCL platforms that exist.
     *
     * @return : Number of OpenCL platforms.
     */
    static int size();


    /*!
     * Returns a pointer to the OpenCL platform with the given index. If the index is out of range
     * an exception is thrown.
     *
     * @param index : Index of OpenCL platform whose reference is returned.
     *
     * @return : Pointer to OpenCL platform with given index.
     */
    static Platform* get(
        int index
    );


/***********/
/* Methods */
/***********/
public:


    /*!
     * Returns a pointer to the OpenCL device contained in this platform with the given index. If
     * the index is out of range then an exception is thrown.
     *
     * @param index : Index of OpenCL device of this platform whose pointer is returned.
     *
     * @return : Pointer to OpenCL device of this platform with the given index.
     */
    Device* device(
        int index
    ) const;


    /*!
     * Returns the number of OpenCL devices this platform contains.
     *
     * @return : Number of OpenCL devices this platform contains.
     */
    int deviceSize() const;


    /*!
     * Returns the OpenCL extensions this platform supports.
     *
     * @return : OpenCL extensions this platform supports.
     */
    QStringList extensions() const;


    /*!
     * Returns the OpenCL platform ID for this platform object.
     *
     * @return : OpenCL platform ID for this object.
     */
    cl_platform_id id() const;


    /*!
     * Returns the name of this platform.
     *
     * @return : Name of this platform.
     */
    QString name() const;


    /*!
     * Returns the profile of this platform.
     *
     * @return : Profile of this platform.
     */
    QString profile() const;


    /*!
     * Returns the vendor of this platform.
     *
     * @return : Vendor of this platform.
     */
    QString vendor() const;


    /*!
     * Returns the OpenCL version used by this platform.
     *
     * @return : OpenCL version used by this platform.
     */
    QString version() const;


/******************/
/* Static Methods */
/******************/
private:


    /*!
     * Populates the global list of OpenCL platforms if it has not already been populated.
     */
    static void populate();


/********************/
/* Static Variables */
/********************/
private:


    /*!
     * Global pointer list of OpenCL platform objects that is available on this system.
     */
    static QList<Platform*>* _platforms;


/****************/
/* Constructors */
/****************/
private:


    /*!
     * Constructs a new OpenCL platform using the given OpenCL platform ID. This is only used
     * internally by the static methods which initially populate the list of platforms.
     *
     * @param id : OpenCL platform ID used to construct this new platform object.
     *
     * @param parent : Optional parent for this object.
     */
    explicit Platform(
        cl_platform_id id
        ,QObject* parent=nullptr
    );


/***********/
/* Methods */
/***********/
private:


    /*!
     * Gets string based information about this OpenCL platform with the given info type.
     *
     * @param type : The information type that is returned.
     *
     * @return : Information of this platform with the given info type.
     */
    QString getInfo(
        cl_platform_info type
    ) const;


    /*!
     * Populates this platform's list of OpenCL devices.
     */
    void populateDevices();


/*************/
/* Variables */
/*************/
private:


    /*!
     * Pointer list of OpenCL devices this platform provides on this system.
     */
    QList<Device*> _devices;


    /*!
     * OpenCL extensions supported by this platform.
     */
    QStringList _extensions;


    /*!
     * OpenCL platform ID for this platform.
     */
    cl_platform_id _id;


    /*!
     * Name of this platform.
     */
    QString _name;


    /*!
     * OpenCL profile for this platform.
     */
    QString _profile;


    /*!
     * Vendor of this OpenCL platform.
     */
    QString _vendor;


    /*!
     * OpenCL version used by this platform.
     */
    QString _version;
};

}

#endif
