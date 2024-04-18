#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "unordered_map.h"

void loadTitleBasics(Graph& graph, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " + filename << endl;
        return;
    }

    string line, tconst, titleType, primaryTitle;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, tconst, '\t');
        getline(ss, titleType, '\t');
        getline(ss, primaryTitle, '\t');
        if (titleType == "movie") {
            graph.addNode(tconst, primaryTitle);
        }
    }
    file.close();
}

void loadTitleCrew(Graph& graph, const string& filename) {
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

        if (!directors.empty() && directors != "\\N") {
            string directorId;
            stringstream dirSS(directors);
            while (getline(dirSS, directorId, ',')) {
                if (!directorId.empty()) {
                    Node* movieNode = graph.getNode(tconst);
                    Node* directorNode = graph.addNode(directorId, "Director: Unknown");
                    if (movieNode && directorNode) {
                        graph.addEdge(directorId, tconst);
                    }
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
    Graph movieGraph;
    loadTitleBasics(movieGraph, "title-metadata.tsv");
    loadTitleCrew(movieGraph, "title-crew.tsv");
    loadTitleRatings(movieGraph, "title.ratings.tsv");
    movieGraph.display();

    return 0;
}
