#pragma once
#include <vector>
#include <list>
#include <string>
#include <functional> //need to do this the hash the values
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::hash;
using std::list;
//cite slides in Final Exam discussion Powerpoint (54 and 55) 
//where it basically gave us a template to implement an unordered map
//also slide 86 of graphs, sets and maps which gave us psedocode
struct MovieInfo {
    std::string name;
    int year;
    double rating;
    MovieInfo(const std::string& name, int year, double rating)
        : name(name), year(year), rating(rating) {}
};

// A struct to hold a director's movie list and count
struct DirectorMovies {
    std::vector<MovieInfo> movies;
    size_t movieCount;

    void addMovie(const MovieInfo& movie) {
        movies.push_back(movie);
        movieCount = movies.size();
    }
};
