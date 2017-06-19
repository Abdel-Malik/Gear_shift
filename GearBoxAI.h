/**-----------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 26 Mai 2017
 * \file Ce header contient ...todo...
 */

#ifndef _GearAI_h_
#define _GearAI_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include "E_ModeConduite.h"
#include "Point.h"
#include "../IntermediaireG/IntermediaireG.h"
#include "GrilleInterpolation.h"

/** \def Constante PI
 */
#define M_PI (3.1415269)

class GearBoxAI{

    //--**attributs**--//
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

    //--**Méthodes**--//
    public:
    //Constructeurs
    GearBoxAI(){
        for(int i = 0 ; i <(GEAR_MAX-GEAR_MIN+1); i++)
            demultiplication.push_back(1);
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

    //--**Méthodes privées**--//

    private:

    /** \brief Récupérer les données du véhicule prmettant le bon fonctionnement de cette classe
     */
    void recuperationDonnees(){
        gear = informations.getGear();
        vitesse = informations.getVitesse();
        chargeMoteur = informations.getChargeMoteur();
    };

    /** \brief initialiser les grillesInterpolation utilisées pour optimiser le rapport courant
     */
    //Pour chaque vitesse du véhicule, un couple de 'GrilleInterpolation' [ECO ; PERF] est créé
    void initialisationGrilles(){
        for(int i = 0; i < (GEAR_MAX-GEAR_MIN+1);i++){
            GrilleInterpolation perf = GrilleInterpolation(ModeConduite::PERF,informations.getAxeZ(i,ModeConduite::PERF),informations.getEchAxe());
            interpolation.push_back(perf);
        }
        BSFC = GrilleInterpolation(ModeConduite::ECO,informations.getAxeZ(-1,ModeConduite::ECO),informations.getEchAxe());
    };

    /** \brief Réaliser une interpolation de l'accélération pour 'gear' passé en paramètre
     * \param[in] gear Le rapport de vitesse sur lequel appliquer la fonction
     * \param[out] La valeur interpolée
     */
    //cette interpolation est réalisé avec les valeurs de vitesse ; charge moteur du véhicule
    double InterpolationAcc(int gear){
        return interpolation[gear-1].interpolerPoint(vitesse,chargeMoteur);
    };


    /** \brief Réaliser une interpolation de la consommation pour 'gear' passé en paramètre
     * \param[in] gear Le rapport de vitesse sur lequel appliquer la fonction
     * \param[out] La valeur interpolée
     */
    //cette interpolation est réalisé avec les valeurs de vitesse ; charge moteur du véhicule
    double InterpolationCons(int gear){
        return BSFC.interpolerPoint(demultiplication[gear-1]*(informations.getVitesse()/3.6)/(2*M_PI*informations.getRayonRoues(0)),chargeMoteur);
    };


    /** \brief Modifier le rapport de vitesse courant du véhicule
     * \param[in] gear Le rapport de vitesse à appliquer
     */
    void changeGear(int gear);

    /** \brief trouver le rapport optimisant l'accéleration du véhicule
     */
    //Appel à une boucle externe
    void optimiserAcceleration(){
        int ng = boucleMeilleurVitesseAcceleration(-1);
        if(ng == gear)
            ng = boucleMeilleurVitesseAcceleration(1);
        changeGear(ng);
    };


    /** \brief trouver le rapport optimisant la consommation du véhicule
     */
    //Appel à une boucle externe
    void optimiserConsommation(){
        int ng = boucleMeilleurConso(-1);
        if(ng == gear)
            ng = boucleMeilleurConso(1);
        changeGear(ng);
    };


    /** \brief Retourner le rapport optimal
     * \param [in] delta incrémente/décrémente le rapport de vitesse testé
     * \param[out] Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurVitesseAcceleration(int delta){
        double accCalculee;
        int gearT = gear+delta;
        int ng = gear;
        while(gearT >= GEAR_MIN && gearT <= GEAR_MAX){
            accCalculee = InterpolationAcc(gearT);
            if(acceleration < accCalculee){
                ng = gearT;
                acceleration = accCalculee;
            }
            gearT += delta;
        }
        return ng;
    };


    /** \brief Retourner le rapport optimal
     * \param[in] delta incrémente/décrémente le rapport de vitesse testé
     * \param[out] Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurConso(int delta){
        double consoCalculee;
        int gearT = gear+delta;
        int ng = gear;
        while(gearT >= GEAR_MIN && gearT <= GEAR_MAX){
            consoCalculee = InterpolationCons(gearT);
            if(consommation > consoCalculee){
                ng = gearT;
                consommation = consoCalculee;
            }
            gearT += delta;
        }
        return ng;
    };
};

//signatures autres

#endif
