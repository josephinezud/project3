#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "Graph.h"
#include "UnorderedMap.h"
#include "DirectorHeap.h"

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

void loadTitleCrew(Graph& graph, const string& filename, UnorderedMap& directorMap, unordered_map<string, std::pair<string, int>>& movie_map, DirectorHeap& directorHeap) {
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
                    directorMap.insert(directorId, info);
                    //cout<< "directorId: " << directorId << endl; //for testing
                    directorHeap.insert(directorId, movieIt->second.second);

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
    DirectorHeap directorHeap;
    loadTitleBasics(movieGraph, "title-metadata.tsv", movieMap);
    //std::cout << "Title: " << movieMap["tt0044893"].first << ", Year: " << movieMap["tt0044893"].second << std::endl; //for testing
    loadTitleCrew(movieGraph, "title-crew.tsv", directorMap, movieMap, directorHeap); // added directorheap 
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
    //could comment out when testing
    // auto maxDirector = directorMap.getDirectorWithMostMovies();
    // cout << "The director with the most movies is: " << maxDirector.first
    //             << " with " << maxDirector.second.movieCount << " movies:" << std::endl;
    // for (const auto& movie : maxDirector.second.movies) {
    //     std::cout << " - " << movie.name << " (" << movie.year << ")" << std::endl;
    // }
    // //below has nth highest.
    // size_t nth = 1; 
    // auto nthDirector = directorMap.getNthDirectorByMovieCount(nth);
    //     cout << "The " << nth << "th director with the most movies is: " << nthDirector.first << " with " << nthDirector.second.movies.size() << " movies." << std::endl;
    // for (const auto& movie : nthDirector.second.movies) {
    //     std::cout << " - " << movie.name << " (" << movie.year << ")" << std::endl;
    // }     
      //    //displays director with highest movie count
//    if (!directorHeap.isEmpty()) {
//        DirectorEntry topDirector = directorHeap.extractMax();
//        std::cout << "Director with the most movies: " << topDirector.id
//                  << " with " << topDirector.movieCount << " movies." << std::endl;
//    }


   //will do do while loop from prog 1 and 2 for command line instead of while loop
       int choice, nth;
    do {
        cout << "Welcome to the IMDb dataset explorer. Discover the prolific directors and their cinematic contributions." << endl;
        cout << "1. Use the Unordered Map approach to analyze director-movie relationships." << endl;
        cout << "2. Use the Max Heap approach for a different analytical perspective." << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "1. List information about the movies that the director of most movies directed." << endl;
                cout << "2. Enter the nth director with the most movies." << endl;
                cout << "Enter your choice: ";
                cin >> nth;
                if (nth == 1) {
                    auto maxDirector = directorMap.getDirectorWithMostMovies();
                    cout << "The director with the most movies is: " << maxDirector.first
                              << " with " << maxDirector.second.movieCount << " movies:" << endl;
                    for (const auto& movie : maxDirector.second.movies) {
                        cout << " - " << movie.name << " (" << movie.year << ")" << endl;
                    }
                } else if (nth == 2) {
                    cout << "Enter the ranking of the director you want to find: ";
                    std::cin >> nth;
                    try {
                        auto nthDirector = directorMap.getNthDirectorByMovieCount(nth);
                        cout << "The " << nth << "th director with the most movies is: " << nthDirector.first
                                  << " with " << nthDirector.second.movieCount << " movies:" << endl;
                        for (const auto& movie : nthDirector.second.movies) {
                            cout << " - " << movie.name << " (" << movie.year << ")" << endl;
                        }
                    } catch (const std::exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                } else {
                    cout << "Invalid choice." << endl;
                }
                break;
            }
            case 2:
                cout << "Max heap method is not implemented in this example." << endl;
                break;
            case 3:
                cout << "Exiting the program." << std::endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 3);

    return 0;
}
