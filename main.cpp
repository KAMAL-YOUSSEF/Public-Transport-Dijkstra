// Projet_RATP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "file_read.h"


using namespace std;


int main()
{

    //
    // test la methode read_stations
    //// Erruer message s'affiche si le fichier ne se trouve pas (le nom est incorrect)
    /*
    std::unordered_map<uint64_t, travel::Station> les_stations;
    travel::File_read f;

    f.read_stations("data/stations.csv"); // Erruer message s'affiche si le fichier ne se trouve pas (le nom est incorrect)
    les_stations = f.get_stations_hashmap();

    cout << les_stations[1].name << endl;
    cout << les_stations[2].name << endl;
    cout << les_stations[3].name << endl;
    */


    //
    // test la methode read_connections
    //// Erruer message s'affiche si le fichier ne se trouve pas (le nom est incorrect)
    /*
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > connect;
    travel::File_read f;

    f.read_connections("data/connections.csv"); 
    connect = f.get_connections_hashmap();
    cout << connect[9][6] << endl;
    cout << connect[9][10] << endl;
    cout << connect[10][5] << endl;
    */


    //
    // test compute and display travel
    // The user have to enter the start and end station
    //// Erruer message s'affiche si on n'ouvre pas data et stations avant de calculer le trajet
    //// Erreur s'affiche si la station ne se trouve pas
    
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > connect;
    travel::File_read f;

    f.read_stations("data/s.csv");
    f.read_connections("data/c.csv");

    uint64_t currentStation ;
    uint64_t targetStation ;

    cout << "Enter start station number :"; 
    cin >> currentStation;
    cout << "Enter end station number :";
    cin >> targetStation;

    f.compute_and_display_travel(currentStation, targetStation);
    
    


    return 0;
}


