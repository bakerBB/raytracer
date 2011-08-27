#include "color.h"
#include "utility.h"

color_t mix_colors(color_t* color1, color_t* color2, double ratio) {
  color_t out_color;

  if (ISTINY(ratio)) ratio = 0.0;

  out_color.r = (color1->r * ratio) + (color2->r * (1-ratio));
  out_color.g = (color1->g * ratio) + (color2->g * (1-ratio));
  out_color.b = (color1->b * ratio) + (color2->b * (1-ratio));
  out_color.a = 1.0;

  return out_color;
}