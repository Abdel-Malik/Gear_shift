/**------------------------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 31 Mai 2017
 * \file Ce header contient une classe intermédiaire
 * Cette classe permet de déplacer le problème de la réception de données
 * des classes principales pour pouvoir terminer la création de celles-ci.
 */

#ifndef _Intermediaire_h_
#define _Intermediaire_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>

class Intermediaire{

    //-**attributs**-//
    //Véhicule
    int currentGear;
    int nbRoues;
    const int GEAR_MIN = 1;
    const int GEAR_MAX = 12;
    double vitesseVehicule;
    double vitesseRoues;
    double rayonRoues[];
    double chargeMoteur;

    public:
    static constexpr double VITESSE_MIN = 0;
    static constexpr double VITESSE_MAX = 138.6;

    static constexpr double CHARGE_MIN = 0;
    static constexpr double CHARGE_MAX = 1;
    //Interpolation
    int echAxe;


    //-**Méthodes**-//

    public:
    //Constructeurs
    Intermediaire(){
        recuperationDonnees();
    };
    /*Méthodes publiques*/

    /*getter*/
    int getGear(){
        return currentGear;
    };
    int getGearMin(){
        return GEAR_MIN;
    };
    int getGearMax(){
        return GEAR_MAX;
    };
    double getVitesse(){
        return vitesseVehicule;
    };
    double getChargeMoteur(){
        return chargeMoteur;
    };
    double getEchAxe(){
        return echAxe;
    };

    double* getAxeZ(int gear, ModeConduite mode){
        double* valeurs = new double[echAxe*echAxe];
        for(int i = 0; i < echAxe*echAxe; i++)
            valeurs[i] = (((i%echAxe)*(i%echAxe-10))*((i/echAxe)*(i/echAxe-10)))/625.0;
        return valeurs;
    };
    /*setter*/

    //-**Méthodes privées**-//

    private:
    void recuperationDonnees(){
        currentGear = 2;
        vitesseVehicule = 17;
        chargeMoteur = .3;
        //Interpolation
        echAxe = 20;
    };
};

//signatures autres

#endif
