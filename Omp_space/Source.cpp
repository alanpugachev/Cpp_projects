#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const int SHIPSPEED = 1;
const int SYSTEMSIZE = 100;
const int TIME = 100;

//shared data
int cosmicSystem[SYSTEMSIZE][SYSTEMSIZE][SYSTEMSIZE][TIME] = { 0 };
int shipsCoordinates[SYSTEMSIZE][SYSTEMSIZE][SYSTEMSIZE][TIME] = { 0 };

int trueShipsAmount = 0, otherObjectsAmount = 0; //variables for generating part
int analysedShipsAmount = 0, analysedOblectsAmount = 0; //variables for analysing part


//generating part
void generateData();

void generateCosmicSystem() {
    srand(time(nullptr));
#pragma omp parallel for shared(otherObjectsAmount, cosmicSystem)
    for(int x = 0; x < SYSTEMSIZE; x++){
        for(int y = 0; y < SYSTEMSIZE; y++){
            for(int z = 0; z < SYSTEMSIZE; z++){
                int createSpaceObject = rand() % 49; // chance to spawn space object = 1/50
                if(createSpaceObject == 1){
                    cosmicSystem[x][y][z][0] = 1;
                    otherObjectsAmount++;
                }
            }
        }
    }
}

void shipsSpawn(){
    srand(time(nullptr));
#pragma omp parallel for shared(trueShipsAmount, cosmicSystem)
    for(int x = 0; x < SYSTEMSIZE; x++){
        for(int y = 0; y < SYSTEMSIZE; y++){
            for(int z = 0; z < SYSTEMSIZE; z++){
                if(cosmicSystem[x][y][z][0] != 1){
                    int createShip = rand() % 49999; // chance to spawn ship = 1/50000
                    if(createShip == 1 && cosmicSystem[x][y][z][0] == 0){
                        cosmicSystem[x][y][z][0] = 1;
                        shipsCoordinates[x][y][z][0] = 1;
                        trueShipsAmount++;
                    }
                }
            }
        }
    }
}

void shipsMovement(){
    srand(time(nullptr));
#pragma omp parallel for shared(cosmicSystem, shipsCoordinates)
    for(int x = 0; x < SYSTEMSIZE; x++){
        for(int y = 0; y < SYSTEMSIZE; y++){
            for(int z = 0; z < SYSTEMSIZE; z++){
                for(int time = 1; time < 100; time++){
                    if(shipsCoordinates[x][y][z][time] == 1){
                        returntorand:
                        int route = rand() % 7;

                        switch (route) {
                            case 0:
                                if(cosmicSystem[x - SHIPSPEED][y - SHIPSPEED][z - SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x - SHIPSPEED][y - SHIPSPEED][z - SHIPSPEED][time] = 1;
                                    // legacy code. was need for additional operations
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0]; //copy previously generated 3d-space with other space objects and ships

                                    // move the ship in 3d-space and time
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x - SHIPSPEED][y - SHIPSPEED][z - SHIPSPEED][time] = 1;
                                }
                                break;

                            case 1:
                                if(cosmicSystem[x + SHIPSPEED][y - SHIPSPEED][z - SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x + SHIPSPEED][y - SHIPSPEED][z - SHIPSPEED][time] = 1;
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0];
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x + SHIPSPEED][y - SHIPSPEED][z - SHIPSPEED][time] = 1;
                                }
                                break;

                            case 2:
                                if(cosmicSystem[x + SHIPSPEED][y + SHIPSPEED][z - SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x + SHIPSPEED][y + SHIPSPEED][z - SHIPSPEED][time] = 1;
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0];
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x + SHIPSPEED][y + SHIPSPEED][z - SHIPSPEED][time] = 1;
                                }
                                break;

                            case 3:
                                if(cosmicSystem[x - SHIPSPEED][y + SHIPSPEED][z - SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x - SHIPSPEED][y + SHIPSPEED][z - SHIPSPEED][time] = 1;
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0];
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x - SHIPSPEED][y + SHIPSPEED][z - SHIPSPEED][time] = 1;
                                }
                                break;

                            case 4:
                                if(cosmicSystem[x - SHIPSPEED][y - SHIPSPEED][z + SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x - SHIPSPEED][y - SHIPSPEED][z +SHIPSPEED][time] = 1;
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0];
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x - SHIPSPEED][y - SHIPSPEED][z + SHIPSPEED][time] = 1;
                                }
                                break;

                            case 5:
                                if(cosmicSystem[x + SHIPSPEED][y - SHIPSPEED][z + SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x + SHIPSPEED][y - SHIPSPEED][z + SHIPSPEED][time] = 1;
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0];
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x + SHIPSPEED][y - SHIPSPEED][z + SHIPSPEED][time] = 1;
                                }
                                break;

                            case 6:
                                if(cosmicSystem[x + SHIPSPEED][y + SHIPSPEED][z + SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x + SHIPSPEED][y + SHIPSPEED][z + SHIPSPEED][time] = 1;
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0];
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x + SHIPSPEED][y + SHIPSPEED][z + SHIPSPEED][time] = 1;
                                }
                                break;

                            case 7:
                                if(cosmicSystem[x - SHIPSPEED][y + SHIPSPEED][z + SHIPSPEED][0] == 1){ goto returntorand; }
                                else{
                                    /*
                                    shipsCoordinates[x][y][z][time] = 0;
                                    shipsCoordinates[x - SHIPSPEED][y + SHIPSPEED][z + SHIPSPEED][time] = 1;
                                    */

                                    cosmicSystem[x][y][z][time] = cosmicSystem[x][y][z][0];
                                    cosmicSystem[x][y][z][time] = 0;
                                    cosmicSystem[x - SHIPSPEED][y + SHIPSPEED][z + SHIPSPEED][time] = 1;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
}

void generateData(){
    generateCosmicSystem();
    shipsSpawn();
    shipsMovement();
}

//analysing part
void analyseSpace(){
#pragma omp parallel for shared(cosmicSystem, analysedShipsAmount)
    for(int x = 0; x < SYSTEMSIZE; x++){ //amount of all space objects
        for(int y = 0; y < SYSTEMSIZE; y++){
            for(int z = 0; z < SYSTEMSIZE; z++){
                for(int time = 1; time < 100; time++){
                    if(cosmicSystem[x][y][z][time] != cosmicSystem[x][y][z][time - 1]){
                        analysedOblectsAmount++;
                    }
                }
            }
        }
    }
}

int searchForShips(){
    analysedShipsAmount = analysedOblectsAmount - otherObjectsAmount;
    return analysedShipsAmount;
}



int main() {
    generateData();
    analyseSpace();

    cout << "Amount of space objects = " << otherObjectsAmount << endl;
    cout << "Amount of ships = " << searchForShips() << endl;

}
