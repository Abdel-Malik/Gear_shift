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

    void optimiserRapport(){
        //récupérer le mode et la chargeMoteur;
        if((mode == ModeConduite::ECO && chargeMoteur <= 0.7) || (mode == ModeConduite::PERF && chargeMoteur < 0.3))
            optimiserConsommation();
        else
            optimiserAcceleration();
    };

    void modeCourant(){
        //récupérer le mode;
        std::cout << InterpolationAcc(gear) << std::endl;
        std::cout << InterpolationCons(gear) << std::endl;
    };

    /*getter*/

    /*setter*/

    /**Méthodes privées**/

    private:

    //but : Récupérer les données du véhicule prmettant le bon fonctionnement de cette classe
    void recuperationDonnees(){
        gear = informations.getGear();
        vitesse = informations.getVitesse();
        chargeMoteur = informations.getChargeMoteur();
    };

    //but : initialiser les grillesInterpolation utilisées pour optimiser le rapport courant
    //Pour chaque vitesse du véhicule, un couple de 'GrilleInterpolation' [ECO ; PERF] est créé
    void initialisationGrilles(){
        for(int i = 0; i < (informations.getGearMax()-informations.getGearMin()+1);i++){
            GrilleInterpolation eco = GrilleInterpolation(ModeConduite::ECO,informations.getAxeZ(i,ModeConduite::ECO),informations.getEchAxe());
            GrilleInterpolation perf = GrilleInterpolation(ModeConduite::PERF,informations.getAxeZ(i,ModeConduite::PERF),informations.getEchAxe());
            std::pair<GrilleInterpolation,GrilleInterpolation> p (eco,perf);
            interpolation.push_back(p);
        }
    };

    //param :
    //in : gear : Le rapport de vitesse sur lequel appliquer la fonction
    //out : La valeur interpolée
    //but : Réaliser une interpolation de l'accélération pour 'gear' passé en paramètre
    //cette interpolation est réalisé avec les valeurs de vitesse ; charge moteur du véhicule
    double InterpolationAcc(int gear){
        return interpolation[gear].first.interpolerPoint(vitesse,chargeMoteur);
    };

    //param :
    //in : gear : Le rapport de vitesse sur lequel appliquer la fonction
    //out : La valeur interpolée
    //but : Réaliser une interpolation de la consommation pour 'gear' passé en paramètre
    //cette interpolation est réalisé avec les valeurs de vitesse ; charge moteur du véhicule
    double InterpolationCons(int gear){
        return interpolation[gear].second.interpolerPoint(vitesse,chargeMoteur);
    };

    //param :
    //in : gear : Le rapport de vitesse à appliquer
    //but : Modifier le rapport de vitesse courant du véhicule
    void changeGear(int gear);

    //but : trouver le rapport optimisant l'accéleration du véhicule
    //Appel à une boucle externe
    void optimiserAcceleration(){
        int ng = boucleMeilleurVitesseAcceleration(-1);
        if(ng == gear)
            ng = boucleMeilleurVitesseAcceleration(1);
        changeGear(ng);
    };


    //but : trouver le rapport optimisant la consommation du véhicule
    //Appel à une boucle externe
    void optimiserConsommation(){
        int ng = boucleMeilleurConso(-1);
        if(ng == gear)
            ng = boucleMeilleurConso(1);
        changeGear(ng);
    };

    //param :
    //in : delta : incrémente/décrémente le rapport de vitesse testé
    //out : Le rapport[supérieur/inférieur] optimal
    //but : Retourner le rapport optimal
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
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

    //param :
    //in : delta : incrémente/décrémente le rapport de vitesse testé
    //out : Le rapport[supérieur/inférieur] optimal
    //but : Retourner le rapport optimal
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
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
};

//signatures autres

#endif
