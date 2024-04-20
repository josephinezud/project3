#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "Graph.h"
#include "UnorderedMap.h"
//g++ -std=c++14 -Werror -Wuninitialized -o main ./main.cpp
void loadTitleBasics(Graph& graph, const string& filename, unordered_map<string, std::pair<string, int>>& movie_map) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " + filename << endl;
        return;
    }

    string line, tconst, titleType, primaryTitle, originalTitle, isAdult, endYear, startYear;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, tconst, '\t');
        getline(ss, titleType, '\t');
        getline(ss, primaryTitle, '\t');
        getline(ss, originalTitle, '\t');
        getline(ss, isAdult, '\t'); 
        getline(ss, startYear, '\t'); 
        getline(ss, endYear, '\t'); 
        if (titleType == "movie") {
            graph.addNode(tconst, primaryTitle);
            int year = startYear != "\\N" ? std::stoi(startYear) : 0; //if N/A make it zero
            movie_map[tconst] = {primaryTitle, year}; //will use map to first map the tconst to the primary title for both unordered map and max heap
        }
    }
    file.close();
}

void loadTitleCrew(Graph& graph, const string& filename, UnorderedMap& directorMap, unordered_map<string, std::pair<string, int>>& movie_map) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " + filename << endl;
        return;
    }

    string line, tconst, directors, writers;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, tconst, '\t');
        getline(ss, directors, '\t');
        getline(ss, writers, '\t');
        if (movie_map.find(tconst) == movie_map.end()) {
            continue; //skip if movie not found
        }
        auto movieIt = movie_map.find(tconst);
        if (!directors.empty() && directors != "\\N") {
            string directorId;
            stringstream dirSS(directors);
            while (getline(dirSS, directorId, ',')) {
                if (!directorId.empty()) {
                    Node* movieNode = graph.getNode(tconst);
                    Node* directorNode = graph.addNode(directorId, "Director: Unknown");
                    if (movieNode && directorNode) {
                        graph.addEdge(directorId, tconst); //might need to delete if we arent using graphs anymore
                    }
                    //cout << "movie:" << movieIt->second.first << ", year: " << movieIt->second.second << endl; //for testing
                    MovieInfo info(movieIt->second.first, movieIt->second.second);
                    //cout<< "directorId: " << directorId << endl; //for testing
                    directorMap.insert(directorId, info);
                
                }
            }
        }
    }
    file.close();
}

void loadTitleRatings(Graph& graph, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " + filename << endl;
        return;
    }

    string line, tconst, averageRating, numVotes;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, tconst, '\t');
        getline(ss, averageRating, '\t');
        getline(ss, numVotes, '\t');
        Node* node = graph.getNode(tconst);
        if (node) {
            node->averageRating = stod(averageRating);
            node->numVotes = stoi(numVotes);
        }
    }
    file.close();
}

int main() {
     unordered_map<string, std::pair<string, int>> movieMap;
    Graph movieGraph;
    DirectorMovies moviesOfDirector;
    UnorderedMap directorMap(100); //pick appropirate bucket starting size
    loadTitleBasics(movieGraph, "title-metadata.tsv", movieMap);
    //std::cout << "Title: " << movieMap["tt0044893"].first << ", Year: " << movieMap["tt0044893"].second << std::endl; //for testing
    loadTitleCrew(movieGraph, "title-crew.tsv", directorMap, movieMap);
    //DirectorMovies testDirectorMovies;
    // if (directorMap.find("nm0139878", testDirectorMovies)) {
    //     std::cout << "Director found with " << testDirectorMovies.movieCount << " movies." << std::endl;
    //     for (const auto& movie : testDirectorMovies.movies) {
    //         std::cout << " - " << movie.name << " (" << movie.year << ")" << std::endl;
    //     }
    // } else {
    //     std::cout << "Director not found." << std::endl;
    // }
   //loadTitleRatings(movieGraph, "title.ratings.tsv"); doesnt work and wont need
    //movieGraph.display(); comment out for now for testing
    // auto maxDirector = directorMap.getDirectorWithMostMovies();
    // cout << "The director with the most movies is: " << maxDirector.first
    //             << " with " << maxDirector.second.movieCount << " movies:" << std::endl;
    // for (const auto& movie : maxDirector.second.movies) {
    //     std::cout << " - " << movie.name << " (" << movie.year << ")" << std::endl;
    // }
    size_t nth = 1; 
    auto nthDirector = directorMap.getNthDirectorByMovieCount(nth);
        cout << "The " << nth << "th director with the most movies is: " << nthDirector.first << " with " << nthDirector.second.movies.size() << " movies." << std::endl;
    for (const auto& movie : nthDirector.second.movies) {
        std::cout << " - " << movie.name << " (" << movie.year << ")" << std::endl;
    }              
   
    return 0;
}
