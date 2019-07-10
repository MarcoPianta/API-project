#include <stdlib.h>
#include <string.h>

//Types definition

/**
* This struct is the node of the tree(BST) used to store relations in
* alphabetical order
*/
typedef struct relationsNode {
  struct relationsNode *left;
  struct relationsNode *right;
  char *value; //The relation name
} relNode;

//End of types definition


/**
* The main read a string of character from stdin and call the appropriate
* function
*/
int main() {
  //Variables declaration
  char command[6]; //This array contains the command read from stdin
  //End of variables declaration

  //Functions' prototype
  void addent(char entity[]);
  //End of functions' prototype

  while (scanf("%5s", command)) {
    if (0 == strcmp(command, "addent")) {
      //TODO call addent function
    } else if (0 == strcmp(command, "delent")) {
      //TODO call delent function
    } else if (0 == strcmp(command, "addrel")) {
      //TODO call addrel function
    } else if (0 == strcmp(command, "delrel")) {
      //TODO call delrel function
    } else if (0 == strcmp(command, "report")) {
      //TODO call report function
    }else
      return 0;
  }
}

/**
* This function add an entity to the system
* @param entity the entity to add
*/
void addent(char entity[]){

}
