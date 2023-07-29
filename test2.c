#include <time.h>
#include <stdio.h>
 
int main(void)
{
   time_t ltime;

   time(&ltime); 

   printf("the time is %s", ctime(&ltime));
}