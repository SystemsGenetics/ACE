#ifndef EABSTRACTDATAITERATOR_H
#define EABSTRACTDATAITERATOR_H



class EAbstractDataIterator
{
public:
   virtual bool next() = 0;
   virtual bool isEnd() = 0;
   virtual bool read() = 0;
   virtual bool write() = 0;
};



#endif
