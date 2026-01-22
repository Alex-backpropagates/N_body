#ifndef _vect_h_
#define _vect_h_

typedef struct {
  double x; /* coord x   */
  double y; /* coord y */
} vect;

vect vect_prod(double k, vect v);

vect vect_add(vect v1, vect v2);

vect vect_soustract(vect v1, vect v2);

double vect_norm(vect v1);

#endif
