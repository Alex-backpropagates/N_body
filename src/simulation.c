#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <simulation.h>
#include <gif.h>
#include <vect.h>
#include <math.h>
#include <forces.h>

corps* bigbang(int nb_astres, double m_trou_noir)
{
    srand( time (NULL));
    double alpha,d;
    double s = 10; //echelle de l'univers, ce paramètre est purement esthétique car à l'affichage la position du pixel est divisé alors par 10, si vous le modifier il convient de modifier aussi gif.c à l'endroit concerné
    double G = 9.50829*pow(10,3);
    corps* univers = (corps*) malloc(nb_astres * sizeof(corps));
    m_trou_noir = 4.152*pow(10,6);

    (*(univers+0)).r.x = 256*s;
    (*(univers+0)).r.y = 256*s;
    (*(univers+0)).v.x = 0;
    (*(univers+0)).v.y = 0;
    (*(univers+0)).m = m_trou_noir;

    for ( int i = 1; i < nb_astres; i++)
    {
        alpha = ((double)rand()/RAND_MAX)*2*(3.141593);
        d = ((2+((double)rand()/RAND_MAX))/3)*200*s;
        (*(univers+i)).r.x = cos(alpha)*d+256*s;
        (*(univers+i)).r.y = sin(alpha)*d+256*s;
        //(*(univers+i)).r.x = cos(alpha)*d+90000;
        //(*(univers+i)).r.y = sin(alpha)*d+90000;
        printf("  %f", (*(univers+i)).r.x);
        (*(univers+i)).v.x = sqrt((m_trou_noir * G )/ d) * sin(alpha);
        (*(univers+i)).v.y = -sqrt((m_trou_noir * G )/ d) * cos(alpha);
        //(*(univers+i)).v.x = 0;
        //(*(univers+i)).v.y = 0;
        //(*(univers+i)).m = ((double)rand()/RAND_MAX)*2.47*pow(10,28);
        (*(univers+i)).m = 1;
    }  
    return univers;
}




void euler(double Time, corps* univers_init, int NB_CORPS)
{
    int framerate = 100;
    double dt = 1.0/framerate;
    //int NB_PAS = framerate*Time;
    int NB_PAS = framerate*Time;
    vect* F = (vect*) malloc(NB_CORPS * sizeof(vect)); 
    corps* univers_now = (corps*) malloc(NB_CORPS * sizeof(corps));
    univers_now = univers_init;
    corps* univers_prec = univers_now;


    printf("%d ",NB_CORPS);
    printf("%d",NB_PAS);

    for (int pas = 0; pas <NB_PAS; pas++)
    {
        *univers_prec = *univers_now;
    
        for (int c = 0; c <NB_CORPS; c++)
        {
            F[c] = force_grav(c,univers_now, NB_CORPS); //On calcule la force
            /* m.x = 0;
            m.y = 0;
            F[c] = m; */
            //printf("  %lf", F[c].x);
        }
        for (int c = 0; c < NB_CORPS; c++)
        {
            univers_now[c].r = vect_add(univers_prec[c].r, vect_prod(dt,univers_prec[c].v)); //On peut alors caluler la position suivante
        }
        for (int c = 0; c < NB_CORPS; c++)
        {
            univers_now[c].v = vect_add(univers_prec[c].v,vect_prod(dt/univers_prec[c].m,F[c]));
        }
        gif(univers_now, pas, NB_CORPS);
    }
}



void verlet(double Time ,corps* univers_init, int NB_CORPS)
{
    int framerate = 100;
    double dt = 1.0/framerate;
    //int NB_PAS = framerate*Time;
    int NB_PAS = framerate*Time;
    vect* Fcurr = (vect*) malloc(NB_CORPS * sizeof(vect));
    vect* Fprev = (vect*) malloc(NB_CORPS * sizeof(vect)); 
    corps* univers_now = (corps*) malloc(NB_CORPS * sizeof(corps));
    univers_now = univers_init;
    corps* univers_prec = univers_now;

    for (int pas = 0; pas <NB_PAS; pas++)
    {
        *univers_prec = *univers_now;
        for (int c = 0; c <NB_CORPS; c++)
        {

            Fprev[c] = force_grav(c,univers_prec, NB_CORPS); //On calcule la force précédente 
        }
        for (int c = 0; c <NB_CORPS; c++)
        {
            univers_now[c].r = vect_add(vect_add(univers_prec[c].r, vect_prod(dt,univers_prec[c].v)), vect_prod((dt*dt)/(univers_prec[c].m*2),Fprev[c])); //On peut alors caluler la position suivante
        }
        for (int c = 0; c <NB_CORPS; c++)
        {
            Fcurr[c] = force_grav(c,univers_now, NB_CORPS); // On calcule la force actuelle
            printf("  %lf", Fcurr[c].x);
        }
        for (int c = 0; c <NB_CORPS; c++)
        {
            univers_now[c].v = vect_add(univers_prec[c].v, vect_prod(dt/(2*univers_prec[c].m),vect_add(Fprev[c],Fcurr[c]))); //On peut alors calculer la vitesse suivante
        }
        gif(univers_now, pas, NB_CORPS);
    }
}

void verlet_q(double Time ,corps* univers_init, int NB_CORPS)
{
    int framerate = 100;
    double dt = 1.0/framerate;
    //int NB_PAS = framerate*Time;
    int NB_PAS = framerate*Time;
    vect* Fcurr = (vect*) malloc(NB_CORPS * sizeof(vect));
    vect* Fprev = (vect*) malloc(NB_CORPS * sizeof(vect)); 
    corps* univers_now = (corps*) malloc(NB_CORPS * sizeof(corps));
    univers_now = univers_init;
    corps* univers_prec = univers_now;
    corps* univers_quadtree = (corps*) malloc(2 * sizeof(corps));

    for (int pas = 0; pas <NB_PAS; pas++)
    {
        *univers_prec = *univers_now;

        vect center;
        center.x = 2560;
        center.y = 2560;
        for (int c = 0; c <NB_CORPS; c++)
        {
            *univers_quadtree = univers_prec[c];
            *(univers_quadtree+1) = Quadtree(univers_prec, center, 0, NB_CORPS, 1);
            Fprev[c] = force_grav(0,univers_quadtree, 2); //On calcule la force avec la simplification quadtree en un seul astre opposant
        }
        for (int c = 0; c <NB_CORPS; c++)
        {
            univers_now[c].r = vect_add(vect_add(univers_prec[c].r, vect_prod(dt,univers_prec[c].v)), vect_prod((dt*dt)/(univers_prec[c].m*2),Fprev[c])); //On peut alors caluler la position suivante
        }
        for (int c = 0; c <NB_CORPS; c++)
        {
            Fcurr[c] = force_grav(c,univers_now, NB_CORPS); // On calcule la force actuelle
            printf("  %lf", Fcurr[c].x);
        }
        for (int c = 0; c <NB_CORPS; c++)
        {
            univers_now[c].v = vect_add(univers_prec[c].v, vect_prod(dt/(2*univers_prec[c].m),vect_add(Fprev[c],Fcurr[c]))); //On peut alors calculer la vitesse suivante
        }
        gif(univers_now, pas, NB_CORPS);
    }
}


void runge_kutta(double Time, corps* univers_init, int NB_CORPS)
{
    int framerate = 100;
    double dt = 1.0/framerate;
    int NB_PAS = framerate*Time;
    vect* F = (vect*) malloc(NB_CORPS * sizeof(vect)); 
    vect* F_approx = (vect*) malloc(NB_CORPS * sizeof(vect));
    corps* univers_now = (corps*) malloc(NB_CORPS * sizeof(corps));
    corps* univers_now_approx = (corps*) malloc(NB_CORPS * sizeof(corps));
    univers_now = univers_init;
    univers_now_approx = univers_init;
    corps* univers_prec = univers_now;
    corps* univers_prec_approx = univers_now_approx;
    for (int pas = 0; pas <NB_PAS; pas++)
    {
        *univers_prec = *univers_now;


        //Calcul de y+1 approximé par euler//
        *univers_prec_approx = *univers_now_approx;
        for (int c = 0; c <NB_CORPS; c++)
        {
            F_approx[c] = force_grav(c,univers_now_approx, NB_CORPS);
        }
        for (int c = 0; c < NB_CORPS; c++)
        {
            univers_now_approx[c].r = vect_add(univers_prec_approx[c].r, vect_prod(dt,univers_prec_approx[c].v)); //On peut alors caluler la position suivante
        }
        for (int c = 0; c < NB_CORPS; c++)
        {
            univers_now_approx[c].v = vect_add(univers_prec_approx[c].v,vect_prod(dt/univers_prec_approx[c].m,F[c]));
        } 

        // Calcul de y+1 en utilisant l'approximation d'euler//
        for (int c = 0; c <NB_CORPS; c++)
        {
            F[c] = force_grav(c,univers_now, NB_CORPS);
            F_approx[c] = F[c] = force_grav(c,univers_now_approx, NB_CORPS);
        }
        for (int c = 0; c < NB_CORPS; c++)
        {
            univers_now[c].r = vect_prod((dt/2),vect_add(univers_prec[c].v,univers_now_approx[c].v));
        }
        for (int c = 0; c < NB_CORPS; c++)
        {
            univers_now[c].v = vect_prod((dt/2),vect_add(vect_prod(1/univers_prec[c].m,F[c]),vect_prod(1/univers_now_approx[c].m,F_approx[c])));
        }
        

        //Réinitialisation des variables//
        *univers_now_approx=*univers_now;
        gif(univers_now, pas, NB_CORPS);
    }
}