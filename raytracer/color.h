#ifndef _color_h_
#define _color_h_

#include <stdint.h>

typedef struct {
  uint8_t r,g,b,a;
} color32_t;

typedef struct {
  double r,g,b,a;
} color_t;

color_t mix_colors(color_t* color1, color_t* color2, double ratio);

#endif /* _color_h_ */
