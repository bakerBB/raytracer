#ifndef _bitmap_h_
#define _bitmap_h_

#include <stdint.h>

typedef struct {
  uint8_t b,g,r,a;
} bmp_pixel32_t;

void write_bmp(char* filename, int image_width, int image_height, void* image_buffer);

#endif /* _bitmap_h_ */
