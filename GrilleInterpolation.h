﻿/*///////////////////////////////////////////////////////////////
// * Author : Abdel-Malik Bouhassoun
// * Date : 26 Mai 2017
// Ce header contient la classe servant à stocker et traiter
les données relatives à l'interpolation
///////////////////////////////////////////////////////////////*/
#ifndef _GrilleInterpolation_h_
#define _GrilleInterpolation_h_

#define X_AXE (10)
#define Y_AXE (10)
#define Z_AXE (10)

class GrilleInterpolation{

    /**attributs**/
    Point grillePoints[X_AXE][Y_AXE];

    /**Méthodes**/

    public:
    /*Constructeurs*/
    GrilleInterpolation(){
    };
    GrilleInterpolation(ModeConduite mode){
        if(mode == ModeConduite::ECO)
            initialisationPointsEco();
        else
            initialisationPointsPerf();
    };

    /*Méthodes publiques*/

    //param :
    //in : La vitesse du véhicule / la charge moteur appliquée
    //out : La valeur interpolée
    //but : Réaliser une interpolation à l'aide des données de grillesPoints
    //Avec les paramêtres d'entrées, deux points (extrémitées opposées du rectangle) sont récupérées, un calcul d'interpolation est appellé à l'aide de ces points
    double interpolerPoint(double vit, double chM){
        int bas[]={-1,-1}, haut[]={-1,-1};
        int i =1, j=1;
        while(haut[0] == -1 && i<10){
            if(grillePoints[i][0].get1eD() > vit){
                bas[0] = grillePoints[i-1][0].get1eD();
                haut[0] = grillePoints[i][0].get1eD();
            }
            i++;
        }
        while(haut[1] == -1 && j<10){
            if(grillePoints[0][j].get2eD() > chM){
                bas[1] = grillePoints[0][j-1].get2eD();
                haut[1] = grillePoints[0][j].get2eD();
            }
            j++;
        }
        return interpoler(bas,haut,vit,chM);
    };

    private:

    /*Méthodes privées*/

    void initialisationPointsEco(){
        for(int i = 0; i < X_AXE; i++){
            for(int j = 0; j < Y_AXE ; j++){
                grillePoints[i][j] = Point(i,j,((i*(i-10)/25.0)*(j*(j-10)/25.0))); // valeurs à récupérer dans les mesures prises
            }
        }
    };
    void initialisationPointsPerf(){
        for(int i = 0; i < X_AXE; i++){
            for(int j = 0; j < Y_AXE ; j++){
                grillePoints[i][j] = Point(i,j,1+i); // valeurs à récupérer dans les mesures prises
            }
        }
    };
    double interpoler(int coinIG[], int coinSD[], double vit, double chM){
        double d = grillePoints[coinIG[0]][coinIG[1]].get3eD();
        double a = grillePoints[coinSD[0]][coinIG[1]].get3eD()-d;
        double b = grillePoints[coinIG[0]][coinSD[1]].get3eD()-d;
        double c = grillePoints[coinSD[0]][coinSD[1]].get3eD()-grillePoints[coinSD[0]][coinIG[1]].get3eD()-grillePoints[coinIG[0]][coinSD[1]].get3eD()-d;
        double dx = (vit-coinIG[0])/(coinSD[0]-coinIG[0]);
        double dy = (chM-coinIG[1])/(coinSD[1]-coinIG[1]);
        return ((a*dx)+(b*dy)+(c*dx*dy)+d);
    };
    /*getter*/
    /*setter*/

};


//signatures autres

#endif

