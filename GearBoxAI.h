/*///////////////////////////////////////////////////////////////
// * Author : Abdel-Malik Bouhassoun
// * Date : 26 Mai 2017
// Ce header contient ...todo...
///////////////////////////////////////////////////////////////*/
#ifndef _GearAI_h_
#define _GearAI_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include "E_ModeConduite.h"
#include "Point.h"
#include "Intermediaire.h"
#include "GrilleInterpolation.h"

class GearBoxAI{

    /**attributs**/
    enum ModeConduite mode;
    std::vector<std::pair<GrilleInterpolation,GrilleInterpolation>> interpolation;
    int gear;
    const int GEAR_MIN = informations.getGearMin();
    const int GEAR_MAX = informations.getGearMax();
    Intermediaire informations = Intermediaire();
    double vitesse;
    double chargeMoteur;
    double acceleration;
    double consommation;

    /**Méthodes**/

    public:
    //Constructeurs
    GearBoxAI(){
        recuperationDonnees();
        initialisationGrilles();
        acceleration = InterpolationAcc(gear);
        consommation = InterpolationCons(gear);
    };
    /*Méthodes publiques*/
    void modeCourant(){
        //récupérer le mode;
        std::cout << InterpolationAcc(gear) << std::endl;
        std::cout << InterpolationCons(gear) << std::endl;
    };

    double InterpolationAcc(int gear){
        return interpolation[gear].first.interpolerPoint(vitesse,chargeMoteur);
    };
    double InterpolationCons(int gear){
        return interpolation[gear].second.interpolerPoint(vitesse,chargeMoteur);
    };
    void changeGear(int gear);

    void optimiserAcceleration(){
        int ng = boucleMeilleurVitesseAcceleration(-1);
        if(ng == gear)
            ng = boucleMeilleurVitesseAcceleration(1);
        changeGear(ng);
    };
    void optimiserConsommation(){
        int ng = boucleMeilleurConso(-1);
        if(ng == gear)
            ng = boucleMeilleurConso(1);
        changeGear(ng);
    };

    int boucleMeilleurVitesseAcceleration(int delta){
        double accCalculee;
        int gearT = gear+delta;
        int ng = gear;
        if(delta < 0){
            while(gearT >= GEAR_MIN){
                accCalculee = InterpolationAcc(gearT);
                if(acceleration < accCalculee){
                    ng = gearT;
                    acceleration = accCalculee;
                }
                gearT += delta;
            }
        }else{
            while(gearT <= GEAR_MAX){
                accCalculee = InterpolationAcc(gearT);
                if(acceleration < accCalculee){
                    ng = gearT;
                    acceleration = accCalculee;
                }
                gearT += delta;
            }
        }
        return ng;
    };
    int boucleMeilleurConso(int delta){
        double consoCalculee;
        int gearT = gear+delta;
        int ng = gear;
        if(delta < 0){
            while(gearT >= GEAR_MIN){
                consoCalculee = InterpolationCons(gearT);
                if(consommation > consoCalculee){
                    ng = gearT;
                    consommation = consoCalculee;
                }
                gearT += delta;
            }
        }else{
            while(gearT <= GEAR_MAX){
                consoCalculee = InterpolationCons(gearT);
                if(consommation > consoCalculee){
                    ng = gearT;
                    consommation = consoCalculee;
                }
                gearT += delta;
            }
        }
        return ng;
    };
    /*getter*/

    /*setter*/

    void recuperationDonnees(){
        gear = informations.getGear();
        vitesse = informations.getVitesse();
        chargeMoteur = informations.getChargeMoteur();
    };
    void initialisationGrilles(){
        for(int i = 0; i < (informations.getGearMax()-informations.getGearMin()+1);i++){
            GrilleInterpolation eco = GrilleInterpolation(ModeConduite::ECO,informations.getAxeZ(i,ModeConduite::ECO),informations.getEchAxe());
            GrilleInterpolation perf = GrilleInterpolation(ModeConduite::PERF,informations.getAxeZ(i,ModeConduite::PERF),informations.getEchAxe());
            std::pair<GrilleInterpolation,GrilleInterpolation> p (eco,perf);
            interpolation.push_back(p);
        }
    };
};

//signatures autres

#endif
