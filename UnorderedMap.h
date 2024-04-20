#pragma once
#include <vector>
#include <list>
#include <string>
#include <functional> //need to do this the hash the values
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::hash;
using std::list;
//also slide 86 of graphs, sets and maps which gave us psedocode
struct MovieInfo {
    string name;
    int year;
    MovieInfo(string name, int year)
        : name(name), year(year) {} //wont do rating since file wont work
};

struct DirectorMovies {
    vector<MovieInfo> movies;
    size_t movieCount;//used size t to avoid errors since it is a type that is used in the standard library
    //in addition, it is normally used to represent the size of an object
    //https://pvs-studio.com/en/blog/terms/0044/#:~:text=size_t%20is%20a%20special%20unsigned,possible%20array%20or%20an%20object.

    void addMovie(const MovieInfo& movie) {
        movies.push_back(movie);
        movieCount = movies.size();
    }
};

class UnorderedMap {
private:
    struct KeyValuePair {
        //cite copilot for simple help like completing simple parts of repetitive code like parameters for for loops and helps with comments that explains thinking
        //only simple use not whole use
        //cite slides in Final Exam discussion Powerpoint (54 and 55) 
        //where it basically gave us a template to implement an unordered map
        string key; //director's name (will be code)
        DirectorMovies value; //movies vector of struct that contains all infor for that movie
        KeyValuePair(const string& key, const DirectorMovies& value)
            : key(key), value(value) {}
    };

    vector<list<KeyValuePair>> buckets;
    hash<string> hasher;
    size_t bucketCount;
    size_t size_;

    void resizeAndRehash(size_t newCapacity) {
        vector<list<KeyValuePair>> newBuckets(newCapacity);
        for (auto& each_bucket : buckets) {
            for (auto& each_item : each_bucket) {
                size_t newBucketIndex = hasher(each_item.key) % newCapacity;
                newBuckets[newBucketIndex].push_back(std::move(each_item));
            }
        }
        buckets = std::move(newBuckets);
        bucketCount = newCapacity;
    }
    size_t getBucketIndex(const string& key) const {
        return hasher(key) % bucketCount; //from slack from TA's advice to reduce size of hash table
    }


public:
    UnorderedMap(size_t numBuckets)
        : bucketCount(numBuckets), buckets(numBuckets), size_(0) {}

    void insert(const string& directorID, const MovieInfo& movieInfo) {
        if (size_ >= 0.75 * bucketCount) {
            resizeAndRehash(bucketCount * 2); //from slides of final exam review discussion
        }

        size_t bucketIndex = getBucketIndex(directorID); 
        auto& bucket = buckets[bucketIndex];
        
        for (auto& pair : bucket) {
            if (pair.key == directorID) {
                pair.value.addMovie(movieInfo);
                return;
            }
        }
        DirectorMovies directorMovies;
        directorMovies.addMovie(movieInfo);
        bucket.emplace_back(directorID, directorMovies);
        ++size_; //increment size of hash table to calculate load factor
    }

    //below function literally code version of slide 54 from final exam review discussion
    bool find(const string& directorName, DirectorMovies& directorMovies) const {
        size_t bucketIndex = getBucketIndex(directorName);
        const auto& bucket = buckets[bucketIndex];

        for (const auto& element : bucket) {
            if (element.key == directorName) {
                directorMovies = element.value;
                return true;
            }
        }

        return false;
    }
    
    

    size_t size() const {
        size_t size = 0;
        for (const auto& bucket : buckets) {
            size += bucket.size();
        }
        return size;
    }


pair<string, DirectorMovies> getDirectorWithMostMovies() const {
    pair<string, DirectorMovies> maxDirector;
    size_t maxMovies = 0;
    //standard for loop to iterate through the buckets
    for (const auto& bucket : buckets) { 
        for (const auto& keyvalp : bucket) { 
            if (keyvalp.value.movieCount > maxMovies) {
                maxMovies = keyvalp.value.movieCount;
                maxDirector = std::make_pair(keyvalp.key, keyvalp.value); //want KeyValuePair to be in pair

            }
        }
    }

    if (maxMovies == 0) {
        throw std::runtime_error("No directors found in the map.");
    }

    return maxDirector;
}
pair<string, DirectorMovies> getNthDirectorByMovieCount(size_t n) const {
    if (n == 0) {
        throw std::invalid_argument("Ranking starts at 1 (n must be greater than 0).");
    }
    if (n > size_) {
        throw std::out_of_range("Not enough directors in the map to get the nth highest count.");
    }

    pair<string, DirectorMovies> nthMaxDirector;
    string lastMaxDirectorId;
    size_t lastMaxCount = std::numeric_limits<size_t>::max();

    for (size_t i = 0; i < n; ++i) { //we want to run this n times to find the nth highest so basically iterate over the map n times to get max each time
        size_t maxMovies = 0;
        for (const auto& bucket : buckets) {
            for (const auto& kvp : bucket) {
                // we want to find the max that is less than the last max
                if (kvp.value.movieCount > maxMovies && kvp.value.movieCount < lastMaxCount) {
                    maxMovies = kvp.value.movieCount;
                    nthMaxDirector = std::make_pair(kvp.key, kvp.value);
                }
            }
        }
        //by preparing for the next iteration, we store the current max as the last max
        lastMaxCount = maxMovies;
        lastMaxDirectorId = nthMaxDirector.first;
    }

    return nthMaxDirector;
}




};