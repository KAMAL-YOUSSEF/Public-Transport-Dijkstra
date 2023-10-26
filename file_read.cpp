#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;


#include "file_read.h"

namespace travel {

    File_read::File_read() {}
    File_read::~File_read() 
    {
        cout << endl << "Merci d'utiliser notre application" << endl;
    }

    // Lire les datas des stations et la stock dans le map stations_hashmap dans Generic_station_parser
    void File_read::read_stations(const string& _filename) {
        ifstream indata; // indata is like cin. variable qui contient le file
        string line; // pour stocker chaque line
        Station s; // le struct Station

        indata.open(_filename); // open file
        if (!indata) { //Affiche erreur if file didnt open
            cerr << "Error !!!! File not opened" << endl;
            exit(1);
        }

        indata.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer la première line

        while (std::getline(indata, line)) // lire chaque ligne
        {
            vector<string> list; // Vecteur de la ligne contenant chaque colonne
            string colonne; // Contient les data de chaque colonne
            istringstream  sline(line); // transfomer string en istream

            while (std::getline(sline, colonne, ',')) { // Lire chaque element et le virgule sépare chaque élément
                list.push_back(colonne); // Mettre chaque element dans le vecteur list
            }
            // Mettre chaque element dans son endroit dans la Struct Station
            s.name = list.at(0);
            s.line_id = list.at(2);
            s.address = list.at(3);
            s.line_name = list.at(4);
            stations_hashmap.insert({stoi(list.at(1)) , s});
        }
    }

    // Lire les datas des connections et la stock dans le map connections_hashmap dans Generic_connection_parser
    void File_read::read_connections(const std::string& _filename)
    {
        ifstream indata; // indata is like cin
        string line;

        indata.open(_filename); // open file
        if (!indata) { //Affiche erreur if file didnt open
            cerr << "Error !!!! File not opened" << endl;
            exit(1);
        }

        indata.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer la première line

        while (std::getline(indata, line)) // chaque line
        {
            vector<string> list; // Vecteur de la line contenant chaque colonne
            string colonne;
            istringstream  sline(line); // transfomer string en istream

            while (std::getline(sline, colonne, ',')) {
                list.push_back(colonne);
            }

            connections_hashmap[stoi(list.at(0))].insert({ stoi(list.at(1)),stoi(list.at(2)) });
        }
    }

    std::vector<std::pair<uint64_t, uint64_t>> File_read::compute_travel(uint64_t _start, uint64_t _end)
    {
        std::vector<std::pair<uint64_t, uint64_t>> chemin;

        std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > connect;
        this->read_connections("data/connections.csv");
        connect = this->get_connections_hashmap();

        chemin.push_back(make_pair(_start, 0));
        chemin.push_back(make_pair(_end, connect[_start][_end]));

        // L’identifiant de la station et le cout associe de la connexion
        return chemin;
    }

    // Affiche le trajet à réaliser par l'utilisateur en appelant la methode shortestPath
    std::vector<std::pair<uint64_t, uint64_t>> File_read::compute_and_display_travel(uint64_t _start, uint64_t _end)
    {
        std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > connect;
        connect = get_connections_hashmap();

        std::unordered_map<uint64_t, Station> nomsStations;
        nomsStations = get_stations_hashmap();
        if (nomsStations.empty()) { // Affiche erreur si on n'a pas ouvert les data de stations avant
            cerr << "Error! You have to enter station data first";
            exit(1);
        }

        vector<uint64_t> path = shortestPath(_start, _end);
        if (path.empty()) { // Affiche erreur si l'utilisateur a mit une stations qui ne se trouve pas
            cerr << "Error! Enter a valid station number";
            exit(1);
        }

        if (nomsStations.size() != connect.size()) { // Affiche erreur si on n'a pas mit le même data pout stations et connections
            cerr << "Error! Enter the same data for stations and connections";
            exit(1);
        }
        uint64_t currentCost = 0;
        uint64_t lastCost = 0;
        std::cout << "To go from station '" << nomsStations[_start].name << "' to station '" << nomsStations[_end].name << "' :" << endl;
        for (auto it = path.rbegin(); it != path.rend(); it++) {
            std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> >::const_iterator station = connect.find(_start);
            std::unordered_map<uint64_t, uint64_t>::const_iterator vertex = station->second.find(*it);
            currentCost += vertex->second;
            std::cout << nomsStations[_start].name << " to " << nomsStations[*it].name << " (" << currentCost - lastCost << ")" << endl;
            _start = *it;
            lastCost = currentCost;
        }
        std::cout << "Total cost: " << currentCost << endl; // affiche le total cost du trajet

        return std::vector<std::pair<uint64_t, uint64_t>>();
    }


    // Methode pour calculer la shortest distance entre 2 stations avec la methode Dijkstra
    // 
    // On calcule la shortest distance entre les 2 stations seulement on ne rempli pas tout le graph 
    // C'est plus rapide et plus efficace
    vector<uint64_t> File_read::shortestPath(uint64_t start, uint64_t finish)
    {
        unordered_map<uint64_t, int> distances;
        unordered_map<uint64_t, uint64_t> previous;
        vector<uint64_t> nodes;
        vector<uint64_t> path;

        auto comparator = [&] (uint64_t left, uint64_t right) { return distances[left] > distances[right]; };

        std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t>> vertices;
        vertices = get_connections_hashmap();
        if (vertices.empty()) { // Affiche erreur si on n'a pas ouvert les data de connections avant
            cerr << "Error ! you have to enter connection data first ";
            exit(1);
        }

        for (auto& vertex : vertices)
        {
            if (vertex.first == start)
            {
                distances[vertex.first] = 0;
            }
            else
            {
                distances[vertex.first] = numeric_limits<int>::max();
            }

            nodes.push_back(vertex.first);
            push_heap(begin(nodes), end(nodes), comparator);
        }

        while (!nodes.empty())
        {
            pop_heap(begin(nodes), end(nodes), comparator);
            uint64_t smallest = nodes.back();
            nodes.pop_back();

            if (smallest == finish)
            {
                while (previous.find(smallest) != end(previous))
                {
                    path.push_back(smallest);
                    smallest = previous[smallest];
                }

                break;
            }

            if (distances[smallest] == numeric_limits<int>::max()){
                break;
            }

            for (auto& neighbor : vertices[smallest]){
                int alt = distances[smallest] + neighbor.second;
                if (alt < distances[neighbor.first]){
                    distances[neighbor.first] = alt;
                    previous[neighbor.first] = smallest;
                    make_heap(begin(nodes), end(nodes), comparator);
                }
            }
        }
        return path;
    }

}

