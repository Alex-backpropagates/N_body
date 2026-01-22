#ifndef _simulation_h_
#define _simulation_h_

#include <vect.h>
#include <stdbool.h>
#include <forces.h>

void verlet(double Time, corps* univers, int NB_CORPS);

void verlet_q(double Time, corps* univers, int NB_CORPS);

void euler(double Time, corps* univers, int NB_CORPS);

corps* bigbang(int nb_astres, double m_trou_noir);

void runge_kutta(double Time, corps* univers_init, int NB_CORPS);

#endif
