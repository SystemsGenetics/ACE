#ifndef COMMON_H
#define COMMON_H
#include "global.h"
//



const EDataStream& operator>>(const EDataStream& stream, EMetadata& meta);
EDataStream& operator<<(EDataStream& stream, const EMetadata& meta);
EDebug& operator<<(EDebug& debug, const EMetadata*const meta);



#endif
