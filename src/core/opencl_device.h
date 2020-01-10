#ifndef OPENCL_DEVICE_H
#define OPENCL_DEVICE_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
#include "edebug.h"
#include "eexception.h"

namespace OpenCL
{




/*!
 * This provides information about a single OpenCL device. This class is not intended to be
 * instantiated on its own but instead queried through the platform class. Any pointer to a device
 * the platform methods return will exist for the life time of the ACE program. This class also
 * provides its underlying OpenCL device ID that is used by other OpenCL classes.
 */
class Device
:
    public QObject
{
    Q_OBJECT


/****************/
/* Enumerations */
/****************/
public:


    /*!
     * Defines the different OpenCL device types a device can be.
     */
    enum class Type
    {
        /// Defines the CPU device type.
        CPU
        /// Defines the GPU device type.
        ,GPU
        /// Defines the accelerator device type.
        ,Accelerator
        /// Defines the default device type.
        ,Default
    };


/****************/
/* Constructors */
/****************/
public:


    /*!
     * Constructs a new OpenCL device object with the given OpenCL device ID and optional parent.
     *
     * @param id : OpenCL device ID used to construct this new device object.
     *
     * @param parent : Optional parent for this new object.
     */
    explicit Device(
        cl_device_id id
        ,QObject* parent=nullptr
    );


/***********/
/* Methods */
/***********/
public:


    /*!
     * Returns the OpenCL device ID for this device object.
     *
     * @return : OpenCL device ID for this object.
     */
    cl_device_id id() const;


    /*!
     * Indicates if this device object is available for use with OpenCL.
     *
     * @return : True if this device is available or false otherwise.
     */
    bool isAvailable() const;


    /*!
     * Indicates if this device has the ability to compile OpenCL kernel code.
     *
     * @return : True if this device can compile kernel code or false otherwise.
     */
    bool hasCompiler() const;


    /*!
     * Returns the OpenCL extensions supported by this device.
     *
     * @return : OpenCL extensions supported by this device.
     */
    const QStringList& extensions() const;


    /*!
     * Returns the global memory size of this device in bytes.
     *
     * @return : Global memory size of this device in bytes.
     */
    qint64 globalMemorySize() const;


    /*!
     * Returns the local memory size of this device in bytes.
     *
     * @return : Local memory size of this device in bytes.
     */
    qint64 localMemorySize() const;


    /*!
     * Returns the total number of compute units for this device's underlying hardware. In other
     * words how many compute units can operate at any one time in parallel.
     *
     * @return : Total number of compute units for this device's hardware.
     */
    int computeUnitSize() const;


    /*!
     * Returns the maximum clock frequency for this device's hardware in megahertz. Usually in all
     * modern GPU devices it can run at many different clock speeds.
     *
     * @return : Maximum clock frequency for this device's hardware in megahertz.
     */
    int clockFrequency() const;


    /*!
     * Returns the name of this device.
     *
     * @return : Name of this device.
     */
    QString name() const;


    /*!
     * Returns the OpenCL device type of this device.
     *
     * @return : OpenCL device type of this device.
     */
    Device::Type type() const;


    /*!
     * Returns the OpenCL platform ID that this device belongs to.
     *
     * @return : OpenCL platform ID of this device.
     */
    cl_platform_id platform() const;


/***********/
/* Methods */
/***********/
private:


    /*!
     * Gets single variable based information about this OpenCL device with the given info type.
     * Single variables means it is not array based.
     *
     * @tparam R : The variable type that is used for the return variable.
     *
     * @tparam T : The variable type that is used for the actual OpenCL query.
     *
     * @param type : The info type.
     *
     * @return : Information of this device with the given info type.
     */
    template<class R,class T>
    R getInfo(
        cl_device_info type
    ) const;


    /*!
     * Gets string based information about this OpenCL device with the given info type.
     *
     * @param type : The info type.
     *
     * @return : Information of this device with the given info type.
     */
    QString getStringInfo(
        cl_device_info type
    ) const;


/*************/
/* Variables */
/*************/
private:


    /*!
     * OpenCL device ID of this device.
     */
    cl_device_id _id;


    /*!
     * True if this device is available to use or false otherwise.
     */
    bool _available;


    /*!
     * True if this device can compile OpenCL kernel code or false otherwise.
     */
    bool _compiler;


    /*!
     * OpenCL extensions supported by this device.
     */
    QStringList _extensions;


    /*!
     * Global memory size of this device in bytes.
     */
    qint64 _globalMemorySize;


    /*!
     * Local memory size of this device in bytes.
     */
    qint64 _localMemorySize;


    /*!
     * Maximum number of compute units this device's hardware can use at any one point in parallel.
     */
    int _computeUnitSize;


    /*!
     * Maximum clock frequency of this device's hardware in megahertz.
     */
    int _clockFrequency;


    /*!
     * Name of this device.
     */
    QString _name;


    /*!
     * OpenCL device type of this device.
     */
    Type _type;


    /*!
     * OpenCL platform ID that this device belongs to.
     */
    cl_platform_id _platform;
};




template<class R,class T>
R Device::getInfo(
    cl_device_info type
) const
{
    EDEBUG_FUNC(this,type);
    T data;
    cl_int code {clGetDeviceInfo(_id,type,sizeof(T),&data,nullptr)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return static_cast<R>(data);
}

}

#endif
