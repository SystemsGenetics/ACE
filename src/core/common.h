#ifndef COMMON_H
#define COMMON_H



class EDataStream;
class EMetadata;



const EDataStream& operator>>(const EDataStream& stream, EMetadata& meta);
EDataStream& operator<<(EDataStream& stream, const EMetadata& meta);



#endif
