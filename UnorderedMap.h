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
    int movieCount;

    void addMovie(const MovieInfo& movie) {
        movies.push_back(movie);
        movieCount = movies.size();
    }
};

class UnorderedMap {
private:
    struct KeyValuePair {
        string key; //director's name
        DirectorMovies value; //movies vector of struct that contains all infor for that movie
        KeyValuePair(const std::string& key, const DirectorMovies& value)
            : key(key), value(value) {}
    };

    vector<list<KeyValuePair>> buckets;
    hash<string> hasher;
    size_t bucketCount;

    size_t getBucketIndex(const string& key) const {
        return hasher(key) % bucketCount;
    }

public: