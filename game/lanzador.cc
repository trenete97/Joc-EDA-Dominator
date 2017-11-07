// Lanzador de Domination para compañeros de EDA otoño 2016 de la FIB (UPC).
// Genera escenarios nuevos con cada ejecución.
 
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <ctime>
using namespace std;
 
 
const string usage1 = "Se necesitan 4 nombres de jugadores, los que faltan se añadirán como Nulos (Null).";
const string usage2 = "Se necesitan solo 4 nombres de jugadores como parámetros.";
 
 
int sec_this_day() {
    time_t rawtime;
    struct tm * ptm;
   
    time ( &rawtime );
   
    ptm = gmtime(&rawtime);  
   
    return (ptm->tm_hour * 3600) + (ptm->tm_min * 60) + ptm->tm_sec;
   
}
 
 
int main(int argc, char *argv[]) {
   
    vector<string> jugadors(4, "Null");
       
    for (int i = 1; i < argc; ++i) {
        jugadors[i-1] = argv[i];      
       
       
    }
    int aux = sec_this_day() % clock();
    std::string seed = std::to_string(aux);
          
       
    execlp("./Game",
           "Game",
           jugadors[0].c_str(),
           jugadors[1].c_str(),
           jugadors[2].c_str(),
           jugadors[3].c_str(),
           "-s",
           seed.c_str(),
           "-i",
           "default.cnf",
           "-o",
           "default.res",
           NULL);
   
}
