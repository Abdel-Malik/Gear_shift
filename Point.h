/**-------------------------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 26 Mai 2017
 * \file Ce header contient une implémentation d'une classe point (3D)
 */
#ifndef _Point_h_
#define _Point_h_

class Point{

    //-**attributs**-//
    double vitesse;
    double chargeMoteur;
    double csq;

    //-**Méthodes**-//
    public:
    /*Constructeurs*/
    Point(){
        vitesse = 0;
        chargeMoteur = 0;
        csq = 0;
    };
    Point(double v, double chM, double val){
        vitesse = v;
        chargeMoteur = chM;
        csq = val;
    };

    /*getter*/
    double get1eD(){
        return vitesse;
    }
    double get2eD(){
        return chargeMoteur;
    }
    double get3eD(){
        return csq;
    }
    /*setter*/

};


//signatures autres

#endif

