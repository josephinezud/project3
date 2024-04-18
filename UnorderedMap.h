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

class UnorderedMap {
private:
    struct IMDBPair { 
        string key;
        string value;
        IMDBPair(const string &key, const string &value) : key(key), value(value) {} 
    };

    vector<std::list<IMDBPair>> buckets; 
    hash<std::string> hasher;
    size_t bucketCount; //used size t to avoid errors since it is a type that is used in the standard library
    //in addition, it is normally used to represent the size of an object
    //https://pvs-studio.com/en/blog/terms/0044/#:~:text=size_t%20is%20a%20special%20unsigned,possible%20array%20or%20an%20object.

    size_t getBucketIndex(const string &key) const {
        return hasher(key) % bucketCount;
    }

