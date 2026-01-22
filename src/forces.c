#include <stdlib.h>
#include <stdio.h>
#include <simulation.h>
#include <stdbool.h>
#include <vect.h>
#include <math.h>

//calcule toutes les forces appliquees à l'astre i 
vect force_grav(int i, corps* other, int NB_CORPS)
{
    vect f;
    f.x = 0;
    f.y = 0;
    double G = 9.50829*pow(10,3);
    double mi =  other[i].m; //masse du corps    //Problème avec le quadtree 
    vect ri = other[i].r; //position du corp
    double d = 1;
    double Csoft=0; //paramètre de softening
    for (int c = 0; c < i; c++)
    {
        d = vect_norm(vect_soustract(ri,other[c].r));
        f = vect_add(f,vect_prod(-(other[c].m*mi*G)/(pow(d,3)+Csoft), vect_soustract(ri,other[c].r)) ); //Ajout d'une force d'un autre astre à la force résultante f
    }
    for (int c = i+1; c < NB_CORPS; c++)
    {
        d = vect_norm(vect_soustract(ri,other[c].r));
        f = vect_add(f,vect_prod(-(other[c].m*mi*G)/(pow(d,3)+Csoft), vect_soustract(ri,other[c].r)) ); //Ajout d'une force d'un autre astre à la force résultante f
    } 
    return f;
}



vect barycentre(corps* univers, int n) 
{
    double mtot = 0;
    vect rbary = {0, 0}; 
    for (int i = 0; i < n; i++) 
    {
        mtot += univers[i].m;
    }
    if (mtot == 0) 
    { // Éviter une division par zéro
        printf("Erreur : masse totale nulle\n");
        return rbary; // Retourne un vecteur nul
    }
    for (int i = 0; i < n; i++) 
    {
        rbary.x += (univers[i].m / mtot) * univers[i].r.x;
        rbary.y += (univers[i].m / mtot) * univers[i].r.y;
    }
    return rbary;
}

bool condition(double theta, corps* univers, int n,double c) 
{
    for (int i = 0; i < n; i++) 
    {
        double norme = vect_norm(vect_soustract(univers[i].r, barycentre(univers,n)));
        if ((norme / c) < theta) 
        {
            return false;  
        }
    }
    return true;
}

/*Penser à initialiser le quadtree root dans le main, il n'est pas initialisé dans cette fonction*/ // Le code ne marche que pour un corps// 
corps Quadtree(corps* univers, vect centre, double cote, int NB_CORPS, double theta)
{
    corps resultant; 
    int i; 
    corps* univ1 = (corps*) malloc(NB_CORPS*sizeof(corps));
    corps* univ2 = (corps*) malloc(NB_CORPS*sizeof(corps));
    corps* univ3 = (corps*) malloc(NB_CORPS*sizeof(corps));
    corps* univ4 = (corps*) malloc(NB_CORPS*sizeof(corps));
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    i = 0;
    corps* univers_post = (corps*) malloc(4*sizeof(corps));
    vect C1,C2,C3,C4;
    if (NB_CORPS==1)
    {
        return *univers;
    } 
    while ( i < NB_CORPS)
    {     
        if (condition(theta, univers, NB_CORPS, cote) == true)
        {
            if (univers[i].v.x>centre.x && univers[i].v.y>centre.y)
            {
                univ1[a] = univers[i];
                i += 1;
                a += 1;
            }
            else if (univers[i].v.x>centre.x && univers[i].v.y<centre.y)
            {
                univ2[b] = univers[i];
                i += 1;
                b += 1;
            }
            else if (univers[i].v.x<centre.x && univers[i].v.y<centre.y)
            {
                univ3[c] = univers[i];
                i += 1;
                c += 1;
            }
            else if (univers[i].v.x<centre.x && univers[i].v.y>centre.y) //Problème Valgring ne marche que pour un corps
            {
                univ4[d] = univers[i];
                i += 1;
                d += 1;
            }
        }
        else
        {
            i +=1;
        }
    }
    C1.x = centre.x + (cote)/2;
    C1.y = centre.y + (cote)/2;
    C2.x = centre.x + (cote)/2;
    C2.y = centre.y - (cote)/2;
    C3.x = centre.x - (cote)/2;
    C3.y = centre.y - (cote)/2;
    C4.x = centre.x - (cote)/2;
    C4.y = centre.y + (cote)/2;
    resultant.m = Quadtree(univ1, C1, cote/2, a, theta).m + Quadtree(univ2, C2, cote/2, b, theta).m + Quadtree(univ3, C3, cote/2, c, theta).m + Quadtree(univ4, C4, cote/2, d, theta).m;
    univers_post[0] = Quadtree(univ1, C1, cote/2, a, theta);
    univers_post[1] = Quadtree(univ2, C2, cote/2, b, theta);
    univers_post[2] = Quadtree(univ3, C3, cote/2, c, theta);
    univers_post[3] = Quadtree(univ4, C4, cote/2, d, theta);
    resultant.v = barycentre(univers_post,4);
    return resultant; 
}