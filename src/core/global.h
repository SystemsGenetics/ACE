#ifndef GLOBAL_H
#define GLOBAL_H
#include "eabstractanalytic.h"



class EException;
class ELog;
class EDebug_;
class EAbstractAnalyticFactory_;
class EAbstractDataFactory_;
class EAbstractData_;
class EAbstractAnalytic_;
class EDataStream_;
class EMetaArray_;
class EMetaObject_;
class EMetadata_;
const EDataStream& operator>>(const EDataStream& stream, EMetadata& meta);
EDataStream& operator<<(EDataStream& stream, const EMetadata& meta);
EDebug& operator<<(EDebug& debug, const EMetadata*const meta);
EDebug& operator<<(EDebug& debug, const EAbstractAnalytic::Block*const value);

#endif
