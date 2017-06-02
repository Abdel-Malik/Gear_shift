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
#include "../IntermediaireG.h"
#include "GrilleInterpolation.h"

#define M_PI (3.1415269)
class GearBoxAI{

    /**attributs**/
    enum ModeConduite mode;
    std::vector<GrilleInterpolation> interpolation;
    std::vector<double> demultiplication;
    GrilleInterpolation BSFC;
    int gear;
    const int GEAR_MIN = IntermediaireG::GEAR_MIN;
    const int GEAR_MAX = IntermediaireG::GEAR_MAX;
    IntermediaireG informations = IntermediaireG();
    double vitesse;
    double chargeMoteur;
    double acceleration;
    double consommation;

    /**M�thodes**/

    public:
    //Constructeurs
    GearBoxAI(){
        recuperationDonnees();
        initialisationGrilles();
        acceleration = InterpolationAcc(gear);
        consommation = InterpolationCons(gear);
    };

    /*M�thodes publiques*/

    void optimiserRapport(){
        //r�cup�rer le mode et la chargeMoteur;
        if((mode == ModeConduite::ECO && chargeMoteur <= 0.7) || (mode == ModeConduite::PERF && chargeMoteur < 0.3))
            optimiserConsommation();
        else
            optimiserAcceleration();
    };

    void modeCourant(){
        //r�cup�rer le mode;
        std::cout << InterpolationAcc(gear) << std::endl;
        std::cout << InterpolationCons(gear) << std::endl;
    };

    /*getter*/

    /*setter*/

    /**M�thodes priv�es**/

    private:

    //but : R�cup�rer les donn�es du v�hicule prmettant le bon fonctionnement de cette classe
    void recuperationDonnees(){
        gear = informations.getGear();
        vitesse = informations.getVitesse();
        chargeMoteur = informations.getChargeMoteur();
    };

    //but : initialiser les grillesInterpolation utilis�es pour optimiser le rapport courant
    //Pour chaque vitesse du v�hicule, un couple de 'GrilleInterpolation' [ECO ; PERF] est cr��
    void initialisationGrilles(){
        for(int i = 0; i < (GEAR_MAX-GEAR_MIN+1);i++){
            GrilleInterpolation perf = GrilleInterpolation(ModeConduite::PERF,informations.getAxeZ(i,ModeConduite::PERF),informations.getEchAxe());
            interpolation.push_back(perf);
        }
    };

    //param :
    //in : gear : Le rapport de vitesse sur lequel appliquer la fonction
    //out : La valeur interpol�e
    //but : R�aliser une interpolation de l'acc�l�ration pour 'gear' pass� en param�tre
    //cette interpolation est r�alis� avec les valeurs de vitesse ; charge moteur du v�hicule
    double InterpolationAcc(int gear){
        return interpolation[gear].interpolerPoint(vitesse,chargeMoteur);
    };

    //param :
    //in : gear : Le rapport de vitesse sur lequel appliquer la fonction
    //out : La valeur interpol�e
    //but : R�aliser une interpolation de la consommation pour 'gear' pass� en param�tre
    //cette interpolation est r�alis� avec les valeurs de vitesse ; charge moteur du v�hicule
    double InterpolationCons(int gear){
        return BSFC.interpolerPoint(demultiplication[gear]*(informations.getVitesse()/3.6)/(2*M_PI*informations.getRayonRoues(0)),chargeMoteur);
    };

    //param :
    //in : gear : Le rapport de vitesse � appliquer
    //but : Modifier le rapport de vitesse courant du v�hicule
    void changeGear(int gear);

    //but : trouver le rapport optimisant l'acc�leration du v�hicule
    //Appel � une boucle externe
    void optimiserAcceleration(){
        int ng = boucleMeilleurVitesseAcceleration(-1);
        if(ng == gear)
            ng = boucleMeilleurVitesseAcceleration(1);
        changeGear(ng);
    };


    //but : trouver le rapport optimisant la consommation du v�hicule
    //Appel � une boucle externe
    void optimiserConsommation(){
        int ng = boucleMeilleurConso(-1);
        if(ng == gear)
            ng = boucleMeilleurConso(1);
        changeGear(ng);
    };

    //param :
    //in : delta : incr�mente/d�cr�mente le rapport de vitesse test�
    //out : Le rapport[sup�rieur/inf�rieur] optimal
    //but : Retourner le rapport optimal
    //Si aucun rapport optimal n'est trouv�, le rapport courant est retourn�
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
    //in : delta : incr�mente/d�cr�mente le rapport de vitesse test�
    //out : Le rapport[sup�rieur/inf�rieur] optimal
    //but : Retourner le rapport optimal
    //Si aucun rapport optimal n'est trouv�, le rapport courant est retourn�
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
