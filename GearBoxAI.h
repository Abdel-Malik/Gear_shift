/*///////////////////////////////////////////////////////////////
// * Author : Abdel-Malik Bouhassoun
// * Date : 26 Mai 2017
// Ce header contient ...todo...
///////////////////////////////////////////////////////////////*/
#ifndef _GearAI_h_
#define _GearAI_h_

#include <math.h>
#include <iostream>
#include <string.h>
#include "E_ModeConduite.h"
#include "Point.h"
#include "GrilleInterpolation.h"

class GearBoxAI{
    //attributs
    enum ModeConduite mode;
    int gears[2];
    GrilleInterpolation interpolation[2];
    double vitesse;
    double chargeMoteur;
    double acceleration;
    double consommation;
    //méthodes
    public:
    //Constructeurs
    GearBoxAI(){
//        recuperationDonnees();
        interpolation[0] = GrilleInterpolation(ModeConduite::ECO);
        interpolation[1] = GrilleInterpolation(ModeConduite::PERF);
        vitesse = 2;
        chargeMoteur = 7;
    };
    //méthodes
    void modeCourant(){
        //récupérer le mode;
        std::cout << InterpolationAcc() << std::endl;
        std::cout << InterpolationCons() << std::endl;
    }
    void recuperationDonnees();
    double InterpolationAcc(){
        return interpolation[0].interpolerPoint(vitesse,chargeMoteur);
    };
    double InterpolationCons(){
        return interpolation[1].interpolerPoint(vitesse,chargeMoteur);
    };
    void changeGear();
    void currentGear();
    //getter
    bool isGearUp();
    bool isGearDown();
    //setter

};

//signatures autres

#endif
