#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <simulation.h>
#include <forces.h>

#define _USE_MATH_DEFINES

int main(int ac, char **av)
{
    double m_trou_noir = 1;
    int nb_astres;
    double temps;
    int select = 0;
    //printf("Quelle est la masse du trou noir choisie?");
    //scanf("%lf",&m_trou_noir);
    printf("Bonjour bienvenu dans la simulation, pour exécuter la configuration initiale sélectionner Verlet (=1), 2 astres, ainsi que le temps que vous souhaitez (entre 1 et 5 secondes) \n");
    printf("Quelle simulation? (euler = 0, verlet = 1, verletavecquadtree = 2, rungekutta(à l'ordre 2) = 3 )");
    scanf("%d",&select);
    printf("Quelle est le nombre d'astres choisi?");
    scanf("%d",&nb_astres);
    printf("Quelle est le temps choisi?");
    scanf("%lf",&temps);
    corps* univers;
    univers = bigbang(nb_astres,m_trou_noir);
    if(select == 0){euler(temps, univers, nb_astres);}
    if(select == 1){verlet(temps, univers, nb_astres);}
    if(select == 2){verlet_q(temps, univers, nb_astres);}
    if(select == 3){runge_kutta(temps, univers, nb_astres);}
    int ex = system("convert -delay 5 -loop 0 ./output/*.pgm anim.gif");
    getchar();
    return 1;
}