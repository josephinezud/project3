#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Node {
    string id;
    string name;
    vector<string> genres;
    vector<Node*> connections;
    double averageRating = 0.0;
    int numVotes = 0;

    Node(string id, string name) : id(id), name(name) {}
};

class Graph {
    unordered_map<string, Node*> nodes;

public:
    ~Graph() {
        for (auto& pair : nodes) {
            delete pair.second;
        }
    }

    Node* addNode(string id, string name) {
        if (nodes.find(id) == nodes.end()) {
            nodes[id] = new Node(id, name);
        }
        return nodes[id];
    }

    void addEdge(string fromId, string toId) {
        if (nodes.find(fromId) != nodes.end() && nodes.find(toId) != nodes.end()) {
            nodes[fromId]->connections.push_back(nodes[toId]);
        }
    }

    Node* getNode(string id) {
        if (nodes.find(id) != nodes.end()) {
            return nodes[id];
        }
        return nullptr;
    }

    void display() {
        for (auto& pair : nodes) {
            cout << "Node " << pair.second->id << " (" << pair.second->name << ") has connections: ";
            for (auto* conn : pair.second->connections) {
                cout << conn->name << " (" << conn->id << "), ";
            }
            cout << "Rating: " << pair.second->averageRating << ", Votes: " << pair.second->numVotes << endl;
        }
    }
};
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Node {
    string id;
    string name;
    vector<string> genres;
    vector<Node*> connections;
    double averageRating = 0.0;
    int numVotes = 0;

    Node(string id, string name) : id(id), name(name) {}
};

class Graph {
    unordered_map<string, Node*> nodes;

public:
    ~Graph() {
        for (auto& pair : nodes) {
            delete pair.second;
        }
    }

    Node* addNode(string id, string name) {
        if (nodes.find(id) == nodes.end()) {
            nodes[id] = new Node(id, name);
        }
        return nodes[id];
    }

    void addEdge(string fromId, string toId) {
        if (nodes.find(fromId) != nodes.end() && nodes.find(toId) != nodes.end()) {
            nodes[fromId]->connections.push_back(nodes[toId]);
        }
    }

    Node* getNode(string id) {
        if (nodes.find(id) != nodes.end()) {
            return nodes[id];
        }
        return nullptr;
    }

    void display() {
        for (auto& pair : nodes) {
            cout << "Node " << pair.second->id << " (" << pair.second->name << ") has connections: ";
            for (auto* conn : pair.second->connections) {
                cout << conn->name << " (" << conn->id << "), ";
            }
            cout << "Rating: " << pair.second->averageRating << ", Votes: " << pair.second->numVotes << endl;
        }
    }
};
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Node {
    string id;
    string name;
    vector<string> genres;
    vector<Node*> connections;
    double averageRating = 0.0;
    int numVotes = 0;

    Node(string id, string name) : id(id), name(name) {}
};

class Graph {
    unordered_map<string, Node*> nodes;

public:
    ~Graph() {
        for (auto& pair : nodes) {
            delete pair.second;
        }
    }

    Node* addNode(string id, string name) {
        if (nodes.find(id) == nodes.end()) {
            nodes[id] = new Node(id, name);
        }
        return nodes[id];
    }

    void addEdge(string fromId, string toId) {
        if (nodes.find(fromId) != nodes.end() && nodes.find(toId) != nodes.end()) {
            nodes[fromId]->connections.push_back(nodes[toId]);
        }
    }

    Node* getNode(string id) {
        if (nodes.find(id) != nodes.end()) {
            return nodes[id];
        }
        return nullptr;
    }

    void display() {
        for (auto& pair : nodes) {
            cout << "Node " << pair.second->id << " (" << pair.second->name << ") has connections: ";
            for (auto* conn : pair.second->connections) {
                cout << conn->name << " (" << conn->id << "), ";
            }
            cout << "Rating: " << pair.second->averageRating << ", Votes: " << pair.second->numVotes << endl;
        }
    }
};
