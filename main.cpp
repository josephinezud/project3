#include <unordered_map>  
#include <fstream>       
#include <iostream>      
#include <sstream>        
#include <string>         
#include "UnorderedMap.h"
#include "DirectorHeap.h"
using std::cout;
using std::cin;
using std::unordered_map;
using std::ifstream;
using std::string;
using std::stringstream;
using std::cerr;
using std::endl;
using std::pair;
using std::stoi;
using std::make_pair;
using std::make_shared;

//g++ -std=c++14 -Werror -Wuninitialized -o main ./main.cpp
void loadTitleBasics(const string& filename, unordered_map<string, std::pair<string, int>>& movie_map) {
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
            int year = startYear != "\\N" ? std::stoi(startYear) : 0; //if N/A make it zero
            movie_map[tconst] = {primaryTitle, year}; //will use map to first map the tconst to the primary title for both unordered map and max heap
        }
    }
    file.close();
}

void loadTitleCrew(const string& filename, UnorderedMap& directorMap, unordered_map<string, std::pair<string, int>>& movie_map, DirectorHeap& directorHeap) {
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
                    
                    
                    //cout << "movie:" << movieIt->second.first << ", year: " << movieIt->second.second << endl; //for testing
                    MovieInfo info(movieIt->second.first, movieIt->second.second);
                    directorMap.insert(directorId, info);
                    //cout<< "directorId: " << directorId << endl; //for testing
                    directorHeap.insertOrUpdate(directorId, 1);

                }
            }
        }
    }
    file.close();
}



int main() {
    unordered_map<string, std::pair<string, int>> movieMap;
    UnorderedMap directorMap(100); //pick appropirate bucket starting size
    DirectorHeap directorHeap;
    loadTitleBasics("title-metadata.tsv", movieMap);
    loadTitleCrew("title-crew.tsv", directorMap, movieMap, directorHeap); // added directorheap 


   //will do do while loop from prog 1 and 2 for command line instead of while loop
    int choice, nth;
    do {
        cout << "\nWelcome to the IMDb dataset explorer. Discover the prolific directors and their cinematic contributions.\n";
        cout << "1. Use the Unordered Map approach to obtain the director of most movies directed.\n";
        cout << "2. Use the Max Heap approach to obtain the director of most movies directed.\n";
        cout << "3. Enter the nth director with the most movies.\n";
        cout << "4. View the corresponding movies and years associated with the selected director.\n";
        cout << "5. List all directors up to the nth rank based on the number of movies directed.\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                auto maxDirectorMap = directorMap.getDirectorWithMostMovies();
                cout << "The director with the most movies utilizing unordered maps is: " << maxDirectorMap.first
                     << " with " << maxDirectorMap.second.movieCount << " movies.\n";
                break;
            }
            case 2: {
                if (!directorHeap.isEmpty()) {
                    auto maxDirectorHeap = directorHeap.extractMax();
                    cout << "Director with the most movies utilizing max heaps is: " << maxDirectorHeap.id
                         << " with " << maxDirectorHeap.movieCount << " movies.\n";
                } else {
                    cout << "Heap is empty.\n";
                }
                break;
            }
            case 3: {
                cout << "Enter the ranking of the director you want to find: ";
                cin >> nth;
                try {
                    auto nthDirectorMap = directorMap.getNthDirectorByMovieCount(nth);
                    cout << "The " << nth << "th director with the most movies is: " << nthDirectorMap.first
                         << " with " << nthDirectorMap.second.movieCount << " movies:\n";
                } catch (const std::exception& e) {
                    cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 4: {
                cout << "Enter the ranking of the director whose movies and corresponding years you want to view: ";
                cin >> nth;
                try {
                    auto nthDirector = directorMap.getNthDirectorByMovieCount(nth);
                    cout << "The " << nth << "th director with the most movies is: " << nthDirector.first
                         << " with " << nthDirector.second.movieCount << " movies:\n";
                    for (const auto& movie : nthDirector.second.movies) {
                        cout << " - " << movie.name << " (" << movie.year << ")\n";
                    }
                } catch (const std::exception& e) {
                    cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 5: {
                cout << "Enter the maximum ranking of directors to list: ";
                cin >> nth;
                for (int i = 1; i <= nth; i++) {
                    try {
                        auto director = directorMap.getNthDirectorByMovieCount(i);
                        cout << i << ". " << director.first << " with " << director.second.movieCount << " movies\n";
                    } catch (const std::exception& e) {
                        cout << "Error retrieving data for rank " << i << ": " << e.what() << "\n";
                        break;
                    }
                }
                break;
            }
            case 6:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 6);

//how we tested  below:
    //DirectorMovies moviesOfDirector;
    //DirectorMovies testDirectorMovies;
    //std::cout << "Title: " << movieMap["tt0044893"].first << ", Year: " << movieMap["tt0044893"].second << std::endl; //for testing
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
    // auto maxDirectormap = directorMap.getDirectorWithMostMovies();
    // cout << "The director with the most movies is: " << maxDirectormap.first
    //             << " with " << maxDirectormap.second.movieCount << " movies:" << std::endl;
    // for (const auto& movie : maxDirectormap.second.movies) {
    //     std::cout << " - " << movie.name << " (" << movie.year << ")" << std::endl;
    // }
    // //below has nth highest.
    // size_t nthmap = 2; 
    // auto nthDirectormap = directorMap.getNthDirectorByMovieCount(nthmap);
    //     cout << "The " << nthmap << "th director with the most movies is: " << nthDirectormap.first << " with " << nthDirectormap.second.movies.size() << " movies." << std::endl;
    // for (const auto& movie : nthDirectormap.second.movies) {
    //     std::cout << " - " << movie.name << " (" << movie.year << ")" << std::endl;
    // }     
         //displays director with highest movie count

        // Extract the director with the most movies
    //  auto maxDirector = directorHeap.extractMax();
    //  std::cout << "Director with the most movies: " << maxDirector.id
    //           << " with " << maxDirector.movieCount << " movies." << std::endl;

    // size_t nth = 2;
    // auto nthDirector = directorHeap.getNthHighestDirector(nth);
    // std::cout << nth << "th highest director: " << nthDirector.id
    //               << " with " << nthDirector.movieCount << " movies." << std::endl;
    // for (const auto& movie : topDirector.movies) {
    //     std::cout << "Movie: " << movie.name << ", Year: " << movie.year << std::endl;
    // }


    // size_t nth = 3; // Example for the 3rd highest director

    // // Get the nth highest director
    // DirectorEntry nthDirector = directorHeap.getNthHighestDirector(nth);
    // std::cout << "The " << nth << "th director with the most movies is: " << nthDirector.id << std::endl;
    // std::cout << "Movies count: " << nthDirector.getMovieCount() << std::endl;
    // for (const auto& movie : nthDirector.movies) {
    //     std::cout << "Movie: " << movie.name << ", Year: " << movie.year << std::endl;
    // }
 
    return 0;
}
