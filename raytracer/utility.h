#ifndef _utility_h_
#define _utility_h_

#define NELEMENTS(x) (sizeof(x) / sizeof(x[0]));
#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (x > y ? y : x)
#define ISTINY(x) (x < 0.000001 ? 1 : 0)

#endif /* _utility_h_ */
