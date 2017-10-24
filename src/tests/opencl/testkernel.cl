__kernel void hello(__global char* string, __local char* ok, int fine)
{
   string[0] = 'H';
   string[1] = 'e';
   string[2] = 'l';
   string[3] = 'l';
   string[4] = 'o';
   string[5] = ' ';
   string[6] = 'K';
   string[7] = 'e';
   string[8] = 'r';
   string[9] = 'n';
   string[10] = 'e';
   string[11] = 'l';
   string[12] = '!';
   string[13] = '\0';
}
