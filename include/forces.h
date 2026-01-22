#ifndef _forces_h_
#define _forces_h_

#include <vect.h>
#include <stdbool.h>

typedef struct {
  double m; /* masse       */
  vect r; /* position initiale   */
  vect v; /* vitesse initiale */
} corps;

typedef struct {
  int p; /*profondeur du carré*/
  double m; /*masse du quadtree*/
  double c; /*côté du quadtree*/
  vect b; /*barycentre du quadtree*/
  corps* univers; /*univers dans le quadtree*/
} quadtree;


vect force_grav(int i, corps* other, int NB_CORPS);

vect barycentre(corps* univers, int n);

bool condition(double theta, corps* univers, int n,double c);

corps Quadtree(corps* univers, vect centre, double cote, int NB_CORPS, double theta);

#endif