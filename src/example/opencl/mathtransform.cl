#DEFINE ADDITION 0
#DEFINE SUBTRACTION 1
#DEFINE MULTIPLICATION 2
#DEFINE DIVISION 3






__kernel void mathTransform(__global int* globalNum, __local int* localNum, int type, int amount)
{
   *localNum = *globalNum;
   switch (type)
   {
   case ADDITION:
      *localNum += amount;
      break;
   case SUBTRACTION:
      *localNum -= amount;
      break;
   case MULTIPLICATION:
      *localNum *= amount;
      break;
   case DIVISION:
      *localNum /= amount;
      break;
   }
   *globalNum = *localNum;
}
