#ifndef GLOBAL_H
#define GLOBAL_H



class EException;
class ELog;
class EDebug;
class EAbstractAnalyticFactory;
class EAbstractDataFactory;
class EAbstractData;
class EAbstractAnalytic;
class EAbstractAnalyticInput;
class EAbstractAnalyticBlock;
class EAbstractAnalyticSerial;
class EAbstractAnalyticOpenCL;
class EAbstractAnalyticOpenCLWorker;
class EAbstractAnalyticCUDA;
class EAbstractAnalyticCUDAWorker;
class EDataStream;
class EMetaArray;
class EMetaObject;
class EMetadata;
const EDataStream& operator>>(const EDataStream& stream, EMetadata& meta);
EDataStream& operator<<(EDataStream& stream, const EMetadata& meta);
EDebug& operator<<(EDebug& debug, const EMetadata*const meta);
EDebug& operator<<(EDebug& debug, const EAbstractAnalyticBlock*const value);

#endif
