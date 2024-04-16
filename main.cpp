#include <iostream>
#include <string>
#include <vector>
#include "Bridges.h"
#include "DataSource.h"
#include "BSTElement.h"
#include <queue>
#include <stdlib.h>
using namespace std;
using namespace bridges;

/*
  Step 1: Get the Bridges USER ID and API KEY for your program
          by creating an account and signing up (You can use any
          email): https://bridges-cs.herokuapp.com/signup

  Step 2: User ID (Username) and Key (API Sha1 Key) can be
          found under Profile on the top left
*/
BSTElement<string, string>* insertIntoBST(string actor, string movie, BSTElement<string, string> *root)
{
    if (root == nullptr)
    {
        root = new BSTElement<string, string>(actor);
        root->setLabel(movie);
        return root;
    }

    if (actor > root->getKey())
    {
        root->setRight(insertIntoBST(actor, movie, root->getRight()));
    }
    else
    {
        root->setLeft(insertIntoBST(actor, movie, root->getLeft()));
    }

    return root;
}

void destructor(BSTElement<string, string> *root){
    if(root == nullptr) {
        return;
    }
    destructor(root->getLeft());
    destructor(root->getRight());
    delete root;
}


int main(int argc, char **argv)
{

    // Step 3: Test if the following print statement is being run
    cout << "Bridges: IMDB Data in a BST\n";

    // Step 4: Add your User ID and API Key as secrets on Replit

    /* Step 5: Print your User ID and API Key from secrets to the console
       to test if secrets were properly loaded
    */
    char* user_id = getenv("SECRET_HOLDING_USER_ID");
    cout << user_id << endl;
    char* api_key = getenv("SECRET_HOLDING_API_KEY");
    cout << api_key << endl;
    /*
     Step 6: Create a Bridges object by uncommenting the next line
     and inserting the values from steps 1 and 2
    */
    Bridges bridges(1, getenv("SECRET_HOLDING_USER_ID"), getenv("SECRET_HOLDING_API_KEY"));

    /*
       Step 7: Import IMDB data into a vector<ActorMovieIMDB> by
               referring to the Bridges IMDB documentation:
               https://bridgesuncc.github.io/tutorials/Data_IMDB.html
    */
    bridges.setTitle("Accessing IMDB Data");
    DataSource ds (&bridges);
    vector<ActorMovieIMDB> actor_list = ds.getActorMovieIMDBData(1814);

    
    /*
       Step 8: Open the file "insertIntoBST.txt" and copy the provided
               function for inserting an actor/movie pair into a BST.
               Paste the function into this file above your "main" function.
    */
    
    /*
       Step 9: Use the insertion function from the previous step to insert
               any 100 actor/movie pairs into a Bridges BST. Refer to the
               Bridges IMDB documentation:
               https://bridgesuncc.github.io/tutorials/Data_IMDB.html
    */
    BSTElement<string, string>* root = nullptr;
    for(int i = 0; i < 100; i++){
      root = insertIntoBST(actor_list[i].getActor(), actor_list[i].getMovie(), root);
    }
    /*
       Step 10: Visualize this tree by referring to the Bridges BST documentation:
               https://bridgesuncc.github.io/tutorials/BinarySearchTree.html
    */
    // bridges.setDataStructure(root);
    // bridges.visualize();
    /*
       Step 11: Color each level of the tree using a level-order traversal.
               Every node in a given level should have the same color.
               Do not use the same color in two consecutive levels. A starter
               queue has been provided in case you wish to use an iterative
               implementation of BFS.

               Refer to the Bridges BST Styling documentation:
               https://bridgesuncc.github.io/tutorials/BinarySearchTree.html

    */
    //referenced some code from replit's ghostwriter
    queue<BSTElement<string, string> *> q;
    int count=0;
    q.push(root);
    while(!q.empty()){
      int size = q.size();
      for(int i = 0; i < size; i++){
        BSTElement<string, string> *node = q.front();
        q.pop();
        if(count % 4 == 0){
          node->setColor("magenta");
        } else if(count % 4 == 1){
          node->setColor("cyan");
        } else if(count % 4 == 2){
          node->setColor("yellow");
        } else if(count % 4 == 3){
          node->setColor("purple");
        } else{
          node->setColor("blue");
        }
        if(node->getLeft() != nullptr){
          q.push(node->getLeft());
        }
        if(node->getRight() != nullptr){
          q.push(node->getRight());
        }
      
      
      }
      count++;
      
    }

    /*
      Step 12: Visualize the updated tree. Comment out or remove the old
               visualization code from Step 10
    */
  bridges.setDataStructure(root);
  bridges.visualize();
  destructor(root);
  return 0;
}