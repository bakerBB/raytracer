#include <stdio.h>
#include <stdint.h>
#include "Bitmap.h"

#pragma pack(1)
  typedef struct {
    unsigned char magic[2];
  } bmpfile_magic;

  typedef struct {
    uint32_t filesz;
    uint16_t creator1;
    uint16_t creator2;
    uint32_t bmp_offset;
  } bmpfile_header;

  typedef struct {
    uint32_t header_sz;
    int32_t  width;
    int32_t  height;
    uint16_t nplanes;
    uint16_t bitspp;
    uint32_t compress_type;
    uint32_t bmp_bytesz;
    int32_t  hres;
    int32_t  vres;
    uint32_t ncolors;
    uint32_t nimpcolors;
  } BITMAPINFOHEADER;

  typedef struct {
    bmpfile_magic    magic;
    bmpfile_header   header;
    BITMAPINFOHEADER extended;
  } bmp_header;
#pragma pack()

void write_bmp(char* filename, int image_width, int image_height, void* image_buffer) {
  FILE* fd = 0;
  bmp_header header_data;
  header_data.magic.magic[0] = 'B';
  header_data.magic.magic[1] = 'M';
  header_data.header.creator1 = 0x0000;
  header_data.header.creator2 = 0x0000;
  header_data.header.bmp_offset = sizeof(bmp_header);
  header_data.extended.header_sz = sizeof(BITMAPINFOHEADER);
  header_data.extended.width = image_width;
  header_data.extended.height = image_height;
  header_data.extended.nplanes = 1;
  header_data.extended.bitspp = 32;
  header_data.extended.compress_type = 0;
  header_data.extended.bmp_bytesz = 0;
  header_data.extended.hres = 128;
  header_data.extended.vres = 128;
  header_data.extended.ncolors = 0;
  header_data.extended.nimpcolors = 0;
  header_data.header.filesz = header_data.header.bmp_offset + image_width * image_height * 4;

  fd = fopen(filename, "w");
  fwrite(&header_data, sizeof(bmp_header), 1, fd); /* header field. */
  fwrite(image_buffer, image_width * image_height, 4, fd);
  fclose(fd);
}