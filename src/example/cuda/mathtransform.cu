#define ADDITION 0
#define SUBTRACTION 1
#define MULTIPLICATION 2
#define DIVISION 3






__global__ void mathTransform(float* globalBuffer, int globalSize, int type, int amount)
{
   __shared__ float localValue;

   int i = blockIdx.x * blockDim.x + threadIdx.x;

   if ( i >= globalSize )
   {
      return;
   }

   localValue = globalBuffer[i];
   switch (type)
   {
   case ADDITION:
      localValue += amount;
      break;
   case SUBTRACTION:
      localValue -= amount;
      break;
   case MULTIPLICATION:
      localValue *= amount;
      break;
   case DIVISION:
      localValue /= amount;
      break;
   }
   globalBuffer[i] = localValue;
}
