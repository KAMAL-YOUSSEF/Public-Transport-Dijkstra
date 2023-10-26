#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include <fstream>

using namespace std;

#include "Generic_mapper.hpp"

namespace travel {
	class File_read : public Generic_mapper
	{
	private :
		// Methode pour calculer la shortest distance entre 2 stations avec la methode Dijkstra
		vector<uint64_t> shortestPath(uint64_t start, uint64_t finish);

	public:
		File_read(); // Constructeur
		~File_read(); // Destructeur
		// Lire les datas des stations et la stock dans le map stations_hashmap dans Generic_station_parser
		void read_stations(const std::string& _filename)override; 
		// Lire les datas des connections et la stock dans le map connections_hashmap dans Generic_connection_parser
		void read_connections(const std::string& _filename)override;

		std::vector<std::pair<uint64_t, uint64_t> > compute_travel(uint64_t _start, uint64_t _end)override;
		// Affiche le trajet à réaliser par l'utilisateur en appelant la methode shortestPath
		std::vector<std::pair<uint64_t, uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end)override;
	};
}
