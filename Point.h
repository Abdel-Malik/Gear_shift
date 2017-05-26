/*///////////////////////////////////////////////////////////////
// * Author : Abdel-Malik Bouhassoun
// * Date : 26 Mai 2017
// Ce header contient ...todo...
///////////////////////////////////////////////////////////////*/
#ifndef _Point_h_
#define _Point_h_

class Point{
    //attributs
    double vitesse;
    double chargeMoteur;
    double csq;
    //méthodes
    public:
    //Constructeurs
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

    //méthodes
    //getter
    double get1eD(){
        return vitesse;
    }
    double get2eD(){
        return chargeMoteur;
    }
    double get3eD(){
        return csq;
    }
    //setter

};


//signatures autres

#endif

