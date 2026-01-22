#ifndef _gif_h_
#define _gif_h_
#include <simulation.h>

typedef unsigned char pixel_t;

typedef struct {
  int      height; /* Nombre de lignes   : M       */
  int       width; /* Nombre de colonnes : N       */
  int      nbchan; /* Nombre de canaux couleur     */
  pixel_t   **pix; /* Tableau des lignes de pixels */
} *image_t;

void gif(corps* simu_univers, int frame, int NB_CORPS);

image_t  image_new( int height, int width, int nbchan );

int image_get_height( image_t img );

int image_get_width( image_t img );

int image_set_pixel( image_t img, int r, int c, pixel_t pix );

int image_delete( image_t img );

int pgm_write( char *filename, image_t img );

image_t img_write(corps* univers, int NB_CORPS);

image_t astral_set(image_t img, corps* univers, int NB_CORPS);

#endif