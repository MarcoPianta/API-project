#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Types definition

/**
* This struct is the node of the tree(BST) used to store relations names in
* alphabetical order, every node point to another tree containing relations
*/
typedef struct relationsNode {
    struct relationsNode    *parent;
    struct relationsNode    *left;
    struct relationsNode    *right;
    char                    RB; //This field indicates if the node if red or black, value is 'r' if red, 'b' if black
    char                    *value; //The relation name
    relation                *relations; //The tree which contains relations
} relNode;

/**
* This struct is the node of a subtree containing relations between entities, it contains pointer to the entity
* involved, a counter to count number of incoming relations, and another tree which contains outcoming entities in
* alphabetical order. The subtree is contained in relations tree
*/
typedef struct relationElem {
    struct relationElem     *parent;
    struct relationElem     *left;
    struct relationElem     *right;
    char                    RB; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    struct entNode          *inEnt; //Entity with incoming relation
    int                     counter; //Entity with outcoming relation
    struct outElem          *outelems; //The pointer to the relation
} relation;

/**
* This struct is the node of the tree containing outcoming entities, the tree is in relation tree.
*/
typedef struct outcomingElem {
    struct outcomingElem    *parent;
    struct outcomingElem    *left;
    struct outcomingElem    *right;
    char                    RB; //This field indicates if the node if red or black, value is 'r' if red, 'b' if black
    struct entNode          *outEnt; //Entity with outcoming relation
} outElem;

/**
* This struct is the node of the tree(BST) used to store entities in alphabetical order
*/
typedef struct entitiesNode {
    struct entitiesNode     *parent;
    struct entitiesNode     *left;
    struct entitiesNode     *right;
    char                    RB; //This field indicates if the node if red or black, value is 'r' if red, 'b' if black
    char                    *value; //The relation name
    relation                *relations;
} entNode;

/**
* This struct is used for the list in entities tree which contains the relations in which the entity is involved.
*/
typedef struct reletionsList {
    struct reletionsList    *prev;
    struct reletionsList    *next;
    relation                *rel;
} relElem;

//End of types definition


/**
* The main read a string of character from stdin and call the appropriate function
*/
int main() {
    //Variables declaration
    char            command[6]; //This array contains the command read from stdin
    char            line[1024]; //This array contains values after the command read from stdin

    entNode         *treeEntityRoot = NULL; //The root of the entities tree
    relNode         *treeRelationRoot = NULL; //The root of the relations tree
    //End of variables declaration

    //Functions' prototype
    void addent(char entity[]);
    relNode create_rel_node();
    //End of functions' prototype

    while (scanf("%s", command)) {
        if (0 == strcmp(command, "addent")) {
            scanf("%s", line);
            addent(line);
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

/**
* This function create an entNode with name values specified as parameter
* @param name the entity name
* @return an entNode struct with unitialized values
*/
entNode create_ent_node(){

}

/**
* This function create a relNode with name value specified for the relation tree
* @param name the relation name
* @return a relNode struct with unitialized values
*/
relNode create_rel_node(char *name){
    relNode *newNode = malloc(sizeof(relNode));
    newNode -> value     = name;
    newNode -> RB        = 'r';
    newNode -> parent    = NULL;
    newNode -> right     = NULL;
    newNode -> left      = NULL;
    newNode -> relations = NULL;
}

/**
* This function create a relation node with values specified for the relation subtree
* @param name the relation name
* @return a relNode struct with unitialized values
*/
relation create_relation(entNode *incomingEnt, entNode){
    relation *newNode = malloc(sizeof(relation));
    newNode -> parent    = NULL;
    newNode -> left      = NULL;
    newNode -> right     = NULL;
    newNode -> RB        = 'r'; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    newNode -> inEnt     = incomingEnt; //Entity with incoming relation
    newNode -> counter; //Entity with outcoming relation
    newNode -> outelems; //The pointer to the relation
}
