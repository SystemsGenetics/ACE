#define ADDITION 0
#define SUBTRACTION 1
#define MULTIPLICATION 2
#define DIVISION 3






__kernel void mathTransform(__global float* globalBuffer, int globalSize, __local float* localValue, int type, int amount)
{
   int i = get_global_id(0);

   if ( i >= globalSize )
   {
      return;
   }

   *localValue = globalBuffer[i];
   switch (type)
   {
   case ADDITION:
      *localValue += amount;
      break;
   case SUBTRACTION:
      *localValue -= amount;
      break;
   case MULTIPLICATION:
      *localValue *= amount;
      break;
   case DIVISION:
      *localValue /= amount;
      break;
   }
   globalBuffer[i] = *localValue;
}
