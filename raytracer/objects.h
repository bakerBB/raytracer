#ifndef _objects_h_
#define _objects_h_

#include "algebra.h"
#include "color.h"

typedef struct {
  sphere_t sphere;
  color_t  color;
} colored_sphere_t;

typedef struct {
  point3_t origin;
} light_t;

#endif /* _objects_h_ */