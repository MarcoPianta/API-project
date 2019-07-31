#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Types definition

typedef struct relation relation;

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
* This struct is the node of a subtree containing relations between entities, it contains pointer to the entity
* involved, a counter to count number of incoming relations, and another tree which contains outcoming entities in
* alphabetical order. The subtree is contained in relations tree
*/
struct relation {
    struct relation         *parent;
    struct relation         *left;
    struct relation         *right;
    char                    RB; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    entNode                 *inEnt; //Entity with incoming relation
    int                     counter; //Entity with outcoming relation
    outElem                 *outelems; //The pointer to the relation
};

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
* This struct is used for the list in entities tree which contains the relations in which the entity is involved.
*/
typedef struct reletionsList {
    struct reletionsList    *prev;
    struct reletionsList    *next;
    relation                *rel;
} relElem;

//End of types definition

//Global variables declaration

/**
 * Variables used as NIL value for trees, declared global since are accessed from multiple functions but are never
 * modified
 */
entNode         *treeEntityNil;
relNode         *treeRelationNil;

//End of global variables declaration

/**
 * This function initialize all the global variables values
 */
void initialization(){
    //Entity tree initialization
    treeEntityNil = malloc(sizeof(entNode));
    treeEntityNil->parent = treeEntityNil;
    treeEntityNil->left = treeEntityNil;
    treeEntityNil->right = treeEntityNil;
    treeEntityNil->RB = 'b';

    //Relation tree initialization
    treeRelationNil = malloc(sizeof(relNode));
    treeRelationNil->parent = treeRelationNil;
    treeRelationNil->left = treeRelationNil;
    treeRelationNil->right = treeRelationNil;
    treeRelationNil->RB = 'b';
}

/**
* The main read a string of character from stdin and call the appropriate function
*/
int main() {
    initialization();
    //Variables declaration
    char            command[6]; //This array contains the command read from stdin
    char            *entityName; //This variable contains the entity name read from stdin (used for addent and first entity of addrel and delrel)
    char            *entityDestName; //This variable contains the entity name read from stdin (used for id_dest entity in addrel and delrel)

    entNode         *treeEntityRoot = treeEntityNil; //The root of the entities tree
    relNode         *treeRelationRoot = treeRelationNil; //The root of the relations tree
    //End of variables declaration

    //Functions' prototype
    void addent(char entity[], entNode **root);
    void inorder_entity_tree_walk(entNode *x);
    //End of functions' prototype

    while (scanf("%s", command)) {
        if (0 == strcmp(command, "addent")) {
            scanf("%ms", &entityName);
            addent(entityName, &treeEntityRoot);
        } else if (0 == strcmp(command, "delent")) {
            //TODO call delent function
        } else if (0 == strcmp(command, "addrel")) {
            scanf("%ms", &entityName);
            scanf("%ms", &entityDestName);
            //TODO call addrel function
        } else if (0 == strcmp(command, "delrel")) {
            //TODO call delrel function
        } else if (0 == strcmp(command, "report")) {
            //TODO call report function
        }else {
            inorder_entity_tree_walk(treeEntityRoot);
            return 0;
        }
        command[0] = 'm'; entityName = NULL; entityDestName = NULL; //Change variables to set invalid values
    }
    return 0;
}

//------------------------------ Functions for addent ------------------------------
/**
* This function create an entNode with name values specified as parameter
* @param name the entity name
* @return an entNode struct with name value and pointer referencing NIL
*/
entNode *create_ent_node(char *name){
    entNode *newNode = malloc(sizeof(entNode));
    newNode -> value     = name;
    newNode -> RB        = 'r';
    newNode -> parent    = treeEntityNil;
    newNode -> right     = treeEntityNil;
    newNode -> left      = treeEntityNil;
    newNode -> relations = NULL;
}

/**
 * This function made a left rotation of the node specified in the entity tree
 * @param root the entity tree root
 * @param x the node to be rotated
 */
void entity_left_rotate(entNode **root, entNode *x){
    entNode *y = x->right;
    x->right = y->left;
    if (y->left != treeEntityNil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == treeEntityNil)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

/**
 * This function made a right rotation of the node specified in the entity tree
 * @param root the entity tree root
 * @param x the node to be rotated
 */
void entity_right_rotate(entNode **root, entNode *x){
    entNode *y = x->left;
    x->left = y->right;
    if (y->right != treeEntityNil)
        y->right->parent = x;
    y->parent = x->parent;
    if (y->parent == treeEntityNil)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

/**
 * This function fix entity rb tree after an insertion
 * @param root the root of the entity tree
 * @param z the new node added to the tree
 */
void addent_fixup(entNode **root, entNode *z){
    entNode* y;
    while (z->parent->RB == 'r'){
        if (z->parent == z->parent->parent->left){
            y = z->parent->parent->right;
            if (y->RB == 'r'){
                z->parent->RB = 'b';
                y->RB = 'b';
                z->parent->parent->RB = 'r';
                z = z->parent->parent;
            } else{
                if (z == z->parent->right) {
                    z = z->parent;
                    entity_left_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                entity_right_rotate(root, z->parent->parent);
            }
        } else{
            y = z->parent->parent->left;
            if (y->RB == 'r'){
                z->parent->RB = 'b';
                y->RB = 'b';
                z->parent->parent->RB = 'r';
                z = z->parent->parent;
            } else{
                if (z == z->parent->left) {
                    z = z->parent;
                    entity_right_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                entity_left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->RB = 'b';
}

/**
 * This function add an entity to the system
 * @param entity the entity to add
 * @param root the root of the tree af entity
 */
void addent(char entity[], entNode **root){
    entNode*    x = *root;
    entNode*    y = treeEntityNil;
    entNode     *newEntity = create_ent_node(entity);

    while (x != treeEntityNil) {
        y = x;
        if(strcmp(newEntity->value, x->value) == 0)
            return;
        else if (strcmp(newEntity->value, x->value) < 0)
            x = x->left;
        else
            x = x->right;
    }
    newEntity->parent = y;
    if (y == treeEntityNil) {
        *root = newEntity;
    }
    else if (strcmp(newEntity->value, y->value) < 0)
        y->left = newEntity;
    else
        y->right = newEntity;
    newEntity->left = treeEntityNil;
    newEntity->right = treeEntityNil;
    newEntity->RB = 'r';
    //printf("Added entity: %s\n", newEntity->value);
    //printf("Before fixup \nparent: %s\t\tleft: %s\t\tright: %s\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
    addent_fixup(root, newEntity);
    //printf("After fixup \nparent: %s\t\tleft: %s\t\tright: %s\n\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
}

/**
 * This function is used to print tree values in alphabetical order
 * @param x the root of the tree
 */
void inorder_entity_tree_walk(entNode *x){
    if (x != treeEntityNil){
        inorder_entity_tree_walk(x->left);
        printf("%s\n", x->value);
        inorder_entity_tree_walk(x->right);
    }
}
//------------------------------ End functions for addent ------------------------------

//------------------------------ Functions for addrel ------------------------------
/**
* This function create a relNode with name value specified for the relation tree
* @param name the relation name
* @return a relNode struct with name value and pointer referencing NIL
*/
relNode *create_rel_node(char *name){
    relNode *newNode = malloc(sizeof(relNode));
    newNode -> value     = name;
    newNode -> RB        = 'r';
    newNode -> parent    = treeRelationNil;
    newNode -> right     = treeRelationNil;
    newNode -> left      = treeRelationNil;
    newNode -> relations = NULL;
}

/**
* This function create a relation node with values specified for the relation subtree
* @param name the relation name
* @return a relNode struct with unitialized values
*/
relation *create_relation(entNode *incomingEnt, entNode entity){
    relation *newNode = malloc(sizeof(relation));
    newNode -> parent    = NULL;
    newNode -> left      = NULL;
    newNode -> right     = NULL;
    newNode -> RB        = 'r'; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    newNode -> inEnt     = incomingEnt; //Entity with incoming relation
    newNode -> counter; //Entity with outcoming relation
    newNode -> outelems; //The pointer to the relation
}
//------------------------------ End functions for addrel ------------------------------
