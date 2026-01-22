#include <stdlib.h>
#include <stdio.h>
#include <gif.h>

#include <simulation.h>
#include <string.h>
#include <errno.h>
#include <math.h>

void gif(corps* simu_univers, int frame, int NB_CORPS) 
{
    image_t I;
    I = img_write(simu_univers, NB_CORPS);
    const char zeroes[][85000] = { "output/000", "output/00", "output/0"};
    int i = 0;
    if (frame > 9)
    {
        i = 1;
    }
    if (frame > 99)
    {
        i = 2;
    }
    //char zeroes[32] = "000";
    sprintf(zeroes[i], "%s%d.pgm", zeroes[i], frame);
    pgm_write(zeroes[i], I);
    //pgm_write("/output/000.pgm", I);
    printf("on a cree %s\n", zeroes[i]);  
    return;
}

image_t  image_new( int height, int width, int nbchan ) 
{
  int i;
  image_t img = (image_t) calloc(1, sizeof(*img));    //Image 
  if (img == NULL) {
    return NULL;
  }
  if (nbchan == 2) {
    free(img);
    return NULL;
  }
  img->height = height;
  img->width = width;
  img->nbchan = nbchan;
  img->pix = (pixel_t**) calloc(height,sizeof(pixel_t*));    //Pointeurs
  if (img->pix == NULL) {
    free (img); 
    return NULL;
  }
  img->pix[0] = (pixel_t*) calloc(height*width*nbchan,sizeof(pixel_t));    //Pixels
  if (img->pix[0] == NULL) {
    free(img->pix);
    free(img);
    return NULL;
  }
  for (i=0;i<height;i++) {
    img->pix[i] = img->pix[0] + i*width*nbchan;
    if (img->pix[i] == NULL) {
      return NULL;
    }
  }
  return img;
}

int image_get_height( image_t img ) 
{
  if ( NULL == img ) {
    return 0;
  }

  return img->height;
}

int image_get_width( image_t img ) 
{
  if ( NULL == img ) {
    return 0;
  }

  return img->width;
}

int image_set_pixel( image_t img, int r, int c, pixel_t pix )
{
  if (img == NULL || img->pix == NULL || img->pix[0] == NULL || img->pix[r] == NULL) {
    return -1;
  }
  if (c >= img->width || r >= img->height || r < 0 || c < 0) {
    return -1;
  }
  img->pix[r][c] = 255;
  return 0;
}



int image_delete( image_t img ) 
{
  if (img==NULL)
  {
    return -1;
  }
  free(img->pix[0]);   //Pixels
  free(img->pix);    //Pointeurs
  free(img);    //Images
  return(0);
}


int pgm_write( char *filename, image_t img )
{
  int height, width;
  int     ret = 0;
    
  if( NULL == img ) {
    return -1 ;
  }
  /*if( 1 != img->nbchan ) {
    return -2 ;
  }*/
  
  FILE *fp = fopen( filename, "w" );
  if ( NULL == fp ) {
    fprintf( stderr, "ERROR pgm_write : Impossible d'ouvrir %s: %s.\n", filename, strerror( errno ) );
    return -3;
  }
  ret += fprintf( fp, "P5\n" );
  
  ret   += fprintf( fp, "%d \n %d\n", img->width, img->height  );
  ret   += fprintf( fp, "%d\n", 255 );
  
  height = image_get_height( img );
  width  = image_get_width( img );
  
  ret = fwrite( img->pix[ 0 ], sizeof( img->pix[ 0 ][ 0 ] ), width*height, fp );
  if ( ret < width*height ) {
    fprintf( stderr, "ERROR pgm_write : Impossible d'écrire les valeurs des pixels: %s (%d/%d pixels écrits).\n", strerror( errno ), ret, image_get_width( img ) * image_get_height( img ) );
    fclose( fp );
    return -4;
  }
  
  fclose( fp );
  
  return ret;
}


image_t img_write(corps* univers, int NB_CORPS)
{
    int i;
    int N =512;
    image_t img = image_new(N,N,1);
    img = astral_set(img, univers, NB_CORPS); 
    return img;
}

image_t astral_set(image_t img, corps* univers, int NB_CORPS)
{
    int e,i,j,k;
    //printf("Quelle épaisseur voulez vous pour les astres?");
    //scanf("%d",&e);
    e = 3;
    for (i =0; i<NB_CORPS;i++)
    {
        int x = round(univers[i].r.x*0.1);
        int y = round(univers[i].r.y*0.1);
        for(j=y-e; j<=y+e; j++)
        {
            for(k=x-e; k<=x+e;k++)
            {
                image_set_pixel(img, k, j, img->pix);
                //printf("  %d", x);
            }
        }
    } 

    return img;
}