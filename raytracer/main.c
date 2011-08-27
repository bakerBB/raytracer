#include <stdio.h>
#include <stdlib.h>
#include "hr_time.h"
#include "tracer.h"

int main()
{
  double dElapsedTime;
  stopWatch timer;
  startTimer(&timer);

  ray_trace("output.bmp");
  stopTimer(&timer);
  dElapsedTime = getElapsedTime(&timer);
  printf("Trace Complete.  Duration= %2.3f\n", dElapsedTime);
  return system("mspaint output.bmp");
}
