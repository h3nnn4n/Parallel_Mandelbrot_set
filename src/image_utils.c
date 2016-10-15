#include <png.h>
#include <stdint.h>

#include "types.h"

int save_png_to_file (_color *bitmap, unsigned int screenx, unsigned int screeny, const char *path){
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
    int status = -1;

    int pixel_size = 3;
    int depth = 8;

    fp = fopen (path, "wb");
    if ( !fp )
        goto fopen_failed;

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
        goto png_create_write_struct_failed;

    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL)
        goto png_create_info_struct_failed;

    if (setjmp (png_jmpbuf (png_ptr)))
        goto png_failure;

    png_set_IHDR (png_ptr,
            info_ptr,
            screenx,
            screeny,
            depth,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    row_pointers = png_malloc (png_ptr, screeny * sizeof (png_byte *));
    for (y = 0; y < screeny; ++y) {
        png_byte *row = png_malloc (png_ptr, sizeof (uint8_t) * screenx * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < screenx; ++x) {
            _color pixel;

            pixel.r = (uint8_t) bitmap[x + y * screenx].r;
            pixel.g = (uint8_t) bitmap[x + y * screenx].g;
            pixel.b = (uint8_t) bitmap[x + y * screenx].b;

            *row++ = pixel.r;
            *row++ = pixel.g;
            *row++ = pixel.b;
        }
    }

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    status = 0;

    for (y = 0; y < screeny; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
png_create_write_struct_failed:
    fclose (fp);
fopen_failed:
    return status;
}
