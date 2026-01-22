#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vect.h>

vect vect_prod(double k, vect v){
    v.x = k*v.x;
    v.y = k*v.y;
    return v;
}

vect vect_add(vect v1, vect v2){
    vect v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    return v;
}

vect vect_soustract(vect v1, vect v2){
    vect v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    return v;
}

double vect_norm(vect v){
    return sqrt(pow((v.x),2)+pow((v.y),2));
}