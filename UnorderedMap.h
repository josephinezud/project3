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
    string name;
    int year;
    double rating;
    MovieInfo(const string& name, int year, double rating)
        : name(name), year(year), rating(rating) {}
};

struct DirectorMovies {
    vector<MovieInfo> movies;
    size_t movieCount;

    void addMovie(const MovieInfo& movie) {
        movies.push_back(movie);
        movieCount = movies.size();
    }
};
