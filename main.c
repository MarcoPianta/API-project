#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Types definition

typedef struct relation relation;

/**
 * This list is used to save all relation that involves a specific entity
 */
typedef struct elemRelationList {
    struct elemRelationList *next;
    relation                *rel;
}elemRelList;

/**
* This struct is the node of the tree(BST) used to store entities in alphabetical order
*/
typedef struct entitiesNode {
    struct entitiesNode     *parent;
    struct entitiesNode     *left;
    struct entitiesNode     *right;
    char                    RB; //This field indicates if the node if red or black, value is 'r' if red, 'b' if black
    char                    *value; //The relation name
    elemRelList             *relations;
} entNode;

/**
* This struct is the node of the tree containing outcoming entities, the tree is in relation tree.
*/
typedef struct outcomingElem {
    struct outcomingElem    *parent;
    struct outcomingElem    *left;
    struct outcomingElem    *right;
    char                    RB; //This field indicates if the node if red or black, value is 'r' if red, 'b' if black
    entNode                 *outEnt; //Entity with outcoming relation
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
* This struct is the node of a tree which contains pointer to a node of relations subtree to keep references to entity
* in subtree alphabetically sorted
*/
typedef struct relationRef {
    struct relationRef      *parent;
    struct relationRef      *left;
    struct relationRef      *right;
    char                    RB; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    relation                *reference; //The relation reference
}relRef;

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
    relation                *relationsByCounter; //The tree which contains relations sorted by counter
    relRef                  *relationByName; //The root of the tree which keep relations sorted by incoming entity name
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
relation        *treeRelationEntityNil;
outElem         *treeOutElemNil;
relRef          *treeRelRefNil;

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

    //Relation subtree initialization
    treeRelationEntityNil = malloc(sizeof(relation));
    treeRelationEntityNil->parent = treeRelationEntityNil;
    treeRelationEntityNil->left = treeRelationEntityNil;
    treeRelationEntityNil->right = treeRelationEntityNil;
    treeRelationEntityNil->RB = 'b';

    //outElem subtree initialization
    treeOutElemNil = malloc(sizeof(outElem));
    treeOutElemNil->parent = treeOutElemNil;
    treeOutElemNil->left = treeOutElemNil;
    treeOutElemNil->right = treeOutElemNil;
    treeOutElemNil->RB = 'b';

    //relRef subtree initialization
    treeRelRefNil = malloc(sizeof(relRef));
    treeRelRefNil->parent = treeRelRefNil;
    treeRelRefNil->left = treeRelRefNil;
    treeRelRefNil->right = treeRelRefNil;
    treeRelRefNil->RB = 'b';
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
    char            *relationName;

    entNode         *treeEntityRoot = treeEntityNil; //The root of the entities tree
    relNode         *treeRelationRoot = treeRelationNil; //The root of the relations tree
    //End of variables declaration

    //Functions' prototype
    void addent(char entity[], entNode **root);
    void inorder_entity_tree_walk(entNode *x);
    void addrel(char relation[], char origEnt[], char destEnt[], entNode* entRoot, relNode **root);
    //End of functions' prototype

    while (scanf("%s", command)) {
        if (0 == strcmp(command, "addent")) {
            scanf("%ms", &entityName);
            addent(entityName, &treeEntityRoot);
            printf("addent\n\n");
        } else if (0 == strcmp(command, "delent")) {
            //TODO call delent function
        } else if (0 == strcmp(command, "addrel")) {
            scanf("%ms", &entityName);
            scanf("%ms", &entityDestName);
            scanf("%ms", &relationName);
            addrel(relationName, entityName, entityDestName, treeEntityRoot, &treeRelationRoot);
            printf("addrel\n\n");
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
    return newNode;
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
 * This function add an entity to the system, if the entity is already in the system it does nothing
 * @param entity the entity to add
 * @param root the root of the tree of entity
 */
void addent(char entity[], entNode **root){
    entNode*    x = *root;
    entNode*    y = treeEntityNil;
    entNode     *newEntity;

    while (x != treeEntityNil) {
        y = x;
        if(strcmp(entity, x->value) == 0)
            return;
        else if (strcmp(entity, x->value) < 0)
            x = x->left;
        else
            x = x->right;
    }
    newEntity = create_ent_node(entity);
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


//------------------------------ Functions for entity tree ------------------------------
/**
 * This function search in the entity tree for the value specified as parameter
 * @param root the entity tree root
 * @param name the value to search for
 * @return the node which contain the value or treeEntityNil if the value is not in the tree
 */
entNode* search_entity(entNode *root, char name[]){
    entNode *x = root;
    int stringCmp;
    while ((x != treeEntityNil)){
        stringCmp = strcmp(name, x->value);
        if (stringCmp != 0) {
            if (stringCmp < 0)
                x = x->left;
            else
                x = x->right;
        } else break;
    }
    return x;
}
//------------------------------ End functions for entity tree ------------------------------


//------------------------------ Functions for relation tree ------------------------------
/**
* This function create an outElem node with values specified for the relation subtree
* @param root the entity tree root
* @param outEnt the entity name
* @return an outElem struct with values specified as parameters
*/
outElem *create_outElem(entNode* root, char *outEnt){
    outElem *newNode = malloc(sizeof(outElem));
    newNode -> parent    = treeOutElemNil;
    newNode -> left      = treeOutElemNil;
    newNode -> right     = treeOutElemNil;
    newNode -> RB        = 'r'; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    entNode *entity      = search_entity(root, outEnt);
    if (entity != treeEntityNil) {
        newNode->outEnt = entity;
        return newNode;
    } else {
        free(newNode);
        return NULL;
    }
}

/**
* This function create a relation node with values specified for the relation subtree
* @param incomingEnt the name of the entity receiving relation
* @param outEnt the name of the outgoing relation entity
* @param root the entity tree root used to search entities in tree
* @return a relation struct values specified by parameters
*/
relation *create_relation(char *incomingEnt, char *outEnt, entNode* root){
    relation *newNode = malloc(sizeof(relation));
    newNode -> parent    = treeRelationEntityNil;
    newNode -> left      = treeRelationEntityNil;
    newNode -> right     = treeRelationEntityNil;
    newNode -> RB        = 'r'; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    newNode -> inEnt     = search_entity(root, incomingEnt); //Entity with incoming relation
    if (newNode -> inEnt == treeEntityNil) {
        free(newNode);
        return NULL;
    }
    newNode -> counter   = 1; //Relations counter
    newNode -> outelems  = create_outElem(root, outEnt); //Entities with outcoming relation
    if (newNode -> outelems == NULL) {
        free(newNode);
        return NULL;
    }

    elemRelList *newElem = malloc(sizeof(elemRelList));
    newElem -> next = newNode -> inEnt -> relations;
    newElem -> rel = newNode;
    newNode -> inEnt -> relations = newElem;

    newElem = malloc(sizeof(elemRelList));
    newElem -> next = newNode -> outelems ->outEnt -> relations;
    newElem -> rel = newNode;
    newNode -> outelems -> outEnt -> relations = newElem;

    return newNode;
}

void relation_left_rotate(relation **root, relation *x){
    relation *y = x->right;
    x->right = y->left;
    if (y->left != treeRelationEntityNil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == treeRelationEntityNil)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

/**
 * This function made a right rotation of the node specified in the relation tree
 * @param root the relation tree root
 * @param x the node to be rotated
 */
void relation_right_rotate(relation **root, relation *x){
    relation *y = x->left;
    x->left = y->right;
    if (y->right != treeRelationEntityNil)
        y->right->parent = x;
    y->parent = x->parent;
    if (y->parent == treeRelationEntityNil)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

/**
 * This function fix relation rb tree after an insertion
 * @param root the root of the relation tree
 * @param z the new node added to the tree
 */
void addrelation_fixup(relation **root, relation *z){
    relation* y;
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
                    relation_left_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                relation_right_rotate(root, z->parent->parent);
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
                    relation_right_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                relation_left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->RB = 'b';
}

/**
 * This function add an entity to the system, if the entity is already in the system it does nothing
 * @param entity the entity to add
 * @param root the root of the tree of entity
 */
void addrelation(relation* newEntity, relation **root){
    relation*    x = *root;
    relation*    y = treeRelationEntityNil;

    while (x != treeRelationEntityNil) {
        y = x;
        if (newEntity->counter < x->counter)
            x = x->left;
        else
            x = x->right;
    }
    newEntity->parent = y;
    if (y == treeRelationEntityNil) {
        *root = newEntity;
    }
    else if (newEntity->counter < y->counter)
        y->left = newEntity;
    else
        y->right = newEntity;
    newEntity->left = treeRelationEntityNil;
    newEntity->right = treeRelationEntityNil;
    newEntity->RB = 'r';
    //printf("Added entity: %s\n", newEntity->value);
    //printf("Before fixup \nparent: %s\t\tleft: %s\t\tright: %s\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
    addrelation_fixup(root, newEntity);
    //printf("After fixup \nparent: %s\t\tleft: %s\t\tright: %s\n\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
}
//------------------------------ End functions for relation tree ------------------------------


//------------------------------ Functions for relation reference tree ------------------------------
/**
* This function create a relRef node with reference specified
* @param rel the relation to keep referenced
* @return a relRef struct with relation reference specified
*/
relRef *create_relRef(relation *rel){
    relRef *newNode = malloc(sizeof(relRef));
    newNode -> parent    = treeRelRefNil;
    newNode -> left      = treeRelRefNil;
    newNode -> right     = treeRelRefNil;
    newNode -> RB        = 'r'; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    newNode -> reference = rel;
    return newNode;
}

/**
 * This function made a left rotation of the node specified in the relation tree
 * @param root the relation tree root
 * @param x the node to be rotated
 */
void ref_left_rotate(relRef **root, relRef *x){
    relRef *y = x->right;
    x->right = y->left;
    if (y->left != treeRelRefNil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == treeRelRefNil)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

/**
 * This function made a right rotation of the node specified in the relation tree
 * @param root the relation tree root
 * @param x the node to be rotated
 */
void ref_right_rotate(relRef **root, relRef *x){
    relRef *y = x->left;
    x->left = y->right;
    if (y->right != treeRelRefNil)
        y->right->parent = x;
    y->parent = x->parent;
    if (y->parent == treeRelRefNil)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

/**
 * This function fix relation rb tree after an insertion
 * @param root the root of the relation tree
 * @param z the new node added to the tree
 */
void addrel_ref_fixup(relRef **root, relRef *z){
    relRef* y;
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
                    ref_left_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                ref_right_rotate(root, z->parent->parent);
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
                    ref_right_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                ref_left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->RB = 'b';
}

/**
 * This function add a relation to the system, if the relation is already in the system it updates the values of exiting
 * relation
 * @param relation the relation to add/update
 * @param root the root of the tree of relation
 */
void addrel_ref(relRef **root, char *inEnt, char *outEnt, entNode *entRoot, relation **relRoot){
    relRef*     x = *root;
    relRef*     y = treeRelRefNil;
    relRef      *newEntity;
    int comparison;

    while (x != treeRelRefNil) {
        y = x;
        comparison = strcmp(inEnt, x->reference->inEnt->value);
        if(comparison == 0) {
            //TODO update value in existing relation;
            return;
        }
        else if (comparison < 0)
            x = x->left;
        else
            x = x->right;
    }
    relation* newRel = create_relation(inEnt, outEnt, entRoot);
    if (newRel == NULL)
        return;

    addrelation(newRel, relRoot);
    newEntity = create_relRef(newRel);
    newEntity->parent = y;
    if (y == treeRelRefNil) {
        *root = newEntity;
    }
    else if (strcmp(newEntity->reference->inEnt->value, y->reference->inEnt->value) < 0)
        y->left = newEntity;
    else
        y->right = newEntity;
    newEntity->left = treeRelRefNil;
    newEntity->right = treeRelRefNil;
    newEntity->RB = 'r';
    //printf("Added relation: %s\n", newEntity->value);
    //printf("Before fixup \nparent: %s\t\tleft: %s\t\tright: %s\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
    addrel_ref_fixup(root, newEntity);
    //printf("After fixup \nparent: %s\t\tleft: %s\t\tright: %s\n\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
}
//------------------------------ End functions for relation reference tree ------------------------------


//------------------------------ Functions for addrel ------------------------------
/**
* This function create a relNode with name value specified for the relation tree
* @param name the relation name
* @return a relNode struct with name value and pointer referencing NIL
*/
relNode *create_rel_node(char *name, char *origEnt, char *destEnt, entNode* root){
    relNode *newNode = malloc(sizeof(relNode));
    newNode -> value            = name;
    newNode -> RB               = 'r';
    newNode -> parent           = treeRelationNil;
    newNode -> right            = treeRelationNil;
    newNode -> left             = treeRelationNil;
    newNode -> relationsByCounter = create_relation(origEnt, destEnt, root);
    if (newNode -> relationsByCounter == NULL){
        free(newNode);
        return NULL;
    }
    newNode -> relationByName   = create_relRef(newNode -> relationsByCounter);
    return newNode;
}

/**
 * This function made a left rotation of the node specified in the relation tree
 * @param root the relation tree root
 * @param x the node to be rotated
 */
void rel_left_rotate(relNode **root, relNode *x){
    relNode *y = x->right;
    x->right = y->left;
    if (y->left != treeRelationNil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == treeRelationNil)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

/**
 * This function made a right rotation of the node specified in the relation tree
 * @param root the relation tree root
 * @param x the node to be rotated
 */
void rel_right_rotate(relNode **root, relNode *x){
    relNode *y = x->left;
    x->left = y->right;
    if (y->right != treeRelationNil)
        y->right->parent = x;
    y->parent = x->parent;
    if (y->parent == treeRelationNil)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

/**
 * This function fix relation rb tree after an insertion
 * @param root the root of the relation tree
 * @param z the new node added to the tree
 */
void addrel_fixup(relNode **root, relNode *z){
    relNode* y;
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
                    rel_left_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                rel_right_rotate(root, z->parent->parent);
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
                    rel_right_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                rel_left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->RB = 'b';
}

/**
 * This function add a relation to the system, if the relation is already in the system it updates the values of exiting
 * relation
 * @param relation the relation to add/update
 * @param root the root of the tree of relation
 */
void addrel(char relation[], char origEnt[], char destEnt[], entNode* entRoot, relNode **root){
    relNode*    x = *root;
    relNode*    y = treeRelationNil;
    relNode     *newEntity;

    while (x != treeRelationNil) {
        y = x;
        if(strcmp(relation, x->value) == 0) {
            addrel_ref(&(x->relationByName), origEnt, destEnt, entRoot, &(x->relationsByCounter)); //Update existing relation values
            return;
        }
        else if (strcmp(relation, x->value) < 0)
            x = x->left;
        else
            x = x->right;
    }
    newEntity = create_rel_node(relation, origEnt, destEnt, entRoot);
    if (newEntity == NULL)
        return;
    newEntity->parent = y;
    if (y == treeRelationNil) {
        *root = newEntity;
    }
    else if (strcmp(newEntity->value, y->value) < 0)
        y->left = newEntity;
    else
        y->right = newEntity;
    newEntity->left = treeRelationNil;
    newEntity->right = treeRelationNil;
    newEntity->RB = 'r';
    //printf("Added relation: %s\n", newEntity->value);
    //printf("Before fixup \nparent: %s\t\tleft: %s\t\tright: %s\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
    addrel_fixup(root, newEntity);
    //printf("After fixup \nparent: %s\t\tleft: %s\t\tright: %s\n\n", newEntity->parent->value, newEntity->left->value, newEntity->right->value);
}
//------------------------------ End functions for addrel ------------------------------
