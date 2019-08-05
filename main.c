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
    struct relation         *prev;
    struct relation         *next;
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
    relation                *relationsByCounterHead; //The array which contains the relation sorted by counter
    relation                *relationsByCounterEnd; //The array which contains the relation sorted by counter
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
outElem         *treeOutElemNil;
relRef          *treeRelRefNil;

/**
 * This variable is used to keep trace of the maximum number of relation
 */
unsigned long maxRel = 0;
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
    char            entityName[100]; //This variable contains the entity name read from stdin (used for addent and first entity of addrel and delrel)
    char            entityDestName[100]; //This variable contains the entity name read from stdin (used for id_dest entity in addrel and delrel)
    char            relationName[100];

    entNode         *treeEntityRoot = treeEntityNil; //The root of the entities tree
    relNode         *treeRelationRoot = treeRelationNil; //The root of the relations tree
    //End of variables declaration

    //Functions' prototype
    void addent(char entity[], entNode **root);
    void inorder_entity_tree_walk(entNode *x);
    void addrel(char relation[], char origEnt[], char destEnt[], entNode* entRoot, relNode **root);
    void inorder_rel_tree_walk(relNode *x, int flag);
    entNode* search_entity(entNode *root, char name[]);
    //End of functions' prototype

    while (scanf("%s", command)) {
        if (0 == strcmp(command, "addent")) {
            scanf("%s\n", entityName);
            addent(entityName, &treeEntityRoot);
            search_entity(treeEntityRoot, entityName);
        } else if (0 == strcmp(command, "delent")) {
            continue;
            //TODO call delent function
        } else if (0 == strcmp(command, "addrel")) {
            scanf("%s", entityName);
            scanf("%s", entityDestName);
            scanf("%s\n", relationName);
            addrel(relationName, entityDestName, entityName, treeEntityRoot, &treeRelationRoot);
        } else if (0 == strcmp(command, "delrel")) {
            continue;
            //TODO call delrel function
        } else if (0 == strcmp(command, "report")) {
            inorder_rel_tree_walk(treeRelationRoot, 1);
            fputs("\n", stdout);
        }else {
            return 0;
        }
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
    newNode -> value     = strdup(name);
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
    //printf("Addent fixup for : %s", z->value);
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
    int stringCmp = strcmp(name, x->value);
    while ((x != treeEntityNil) && stringCmp != 0){
        if (stringCmp < 0)
            x = x->left;
        else
            x = x->right;
        stringCmp = strcmp(name, x->value);
    }
    return x;
}
//------------------------------ End functions for entity tree ------------------------------


//------------------------------ Functions for outelems tree ------------------------------
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
 * This function made a left rotation of the node specified in the outElem tree
 * @param root the outElem tree root
 * @param x the node to be rotated
 */
void outelem_left_rotate(outElem **root, outElem *x){
    outElem *y = x->right;
    x->right = y->left;
    if (y->left != treeOutElemNil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == treeOutElemNil)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

/**
 * This function made a right rotation of the node specified in the outElem tree
 * @param root the outElem tree root
 * @param x the node to be rotated
 */
void outelem_right_rotate(outElem **root, outElem *x){
    outElem *y = x->left;
    x->left = y->right;
    if (y->right != treeOutElemNil)
        y->right->parent = x;
    y->parent = x->parent;
    if (y->parent == treeOutElemNil)
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
void add_outelem_fixup(outElem **root, outElem *z){
    outElem* y;
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
                    outelem_left_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                outelem_right_rotate(root, z->parent->parent);
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
                    outelem_right_rotate(root, z);
                }
                z->parent->RB = 'b';
                z->parent->parent->RB = 'r';
                outelem_left_rotate(root, z->parent->parent);
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
outElem* add_outelem(outElem **root, char *outEnt, entNode *entRoot){
    outElem*    x = *root;
    outElem*    y = treeOutElemNil;
    outElem     *newEntity;
    int comparison;

    while (x != treeOutElemNil) {
        y = x;
        comparison = strcmp(outEnt, x->outEnt->value);
        if(comparison == 0)
            return NULL;
        else if (comparison < 0)
            x = x->left;
        else
            x = x->right;
    }
    newEntity = create_outElem(entRoot, outEnt);
    if (newEntity == NULL)
        return NULL;
    newEntity->parent = y;
    if (y == treeOutElemNil) {
        *root = newEntity;
    }
    else if (strcmp(newEntity->outEnt->value, y->outEnt->value) < 0)
        y->left = newEntity;
    else
        y->right = newEntity;
    newEntity->left = treeOutElemNil;
    newEntity->right = treeOutElemNil;
    newEntity->RB = 'r';
    add_outelem_fixup(root, newEntity);
    return newEntity;
}
//------------------------------ End functions for outelems tree ------------------------------


//------------------------------ Functions for relation list ------------------------------
/**
* This function create a relation node with values specified for the relation subtree
* @param incomingEnt the name of the entity receiving relation
* @param outEnt the name of the outgoing relation entity
* @param root the entity tree root used to search entities in tree
* @return a relation struct values specified by parameters
*/
relation *create_relation(char *incomingEnt, char *outEnt, entNode* root){
    relation *newNode = malloc(sizeof(relation));
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
    newNode->outelems->RB = 'b';

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

relation *correct_position(relation *rel){
    relation *currRel = rel;
    if(currRel->prev != NULL) {
        while (rel->counter > currRel->prev->counter) {
            currRel = currRel->prev;
            if (currRel->prev == NULL) break;
        }
    }
    if(currRel->prev != NULL) {
        while ((strcmp(rel->inEnt->value, currRel->prev->inEnt->value) < 0) && (rel->counter == currRel->prev->counter)) {
            currRel = currRel->prev;
            if (currRel->prev == NULL) break;
        }
    }
    if(currRel != rel){
        rel->prev->next = rel->next;
        if (rel->next != NULL) rel->next->prev = rel->prev;
        rel->prev = currRel->prev;
        if (currRel->prev != NULL) currRel->prev->next = rel;
        currRel->prev = rel;
        rel->next = currRel;
    }
    if(rel->prev == NULL) //rel is now the head of the list
        return rel;
    return NULL;
}

/**
 * This function add a relation to the list of relations
 * @param newEntity the entity to add
 * @param listEnd the end of the list of relation
 */
relation* addrelation(relation* newEntity, relation *listEnd){
    newEntity -> next = listEnd -> next;
    listEnd -> next = newEntity;
    newEntity -> prev = listEnd;

    return correct_position(newEntity);
}

/**
 * This function update an existing relation
 * @param rel the relation to be updated
 * @param outEntity the entity name with outgoing relation to be added to outelems
 */
relation *update_relation(relation* rel, char *outEntity, entNode *entRoot){
    outElem *newOutElem = add_outelem(&(rel -> outelems), outEntity, entRoot);
    if (NULL != newOutElem) {
        rel -> counter++;
        elemRelList *newElem = malloc(sizeof(elemRelList));
        newElem -> next = newOutElem -> outEnt-> relations;
        newElem -> rel = rel;
        newOutElem -> outEnt -> relations = newElem;
        return correct_position(rel);
    }
    return NULL;
}
//------------------------------ End functions for relation list ------------------------------


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
void addrel_ref(relRef **root, char *inEnt, char *outEnt, entNode *entRoot, relNode *rel){
    relRef*     x = *root;
    relRef*     y = treeRelRefNil;
    relRef      *newEntity;
    int comparison;

    while (x != treeRelRefNil) {
        y = x;
        comparison = strcmp(inEnt, x->reference->inEnt->value);
        if(comparison == 0) {
            relation *newHead = update_relation(x->reference, outEnt, entRoot);
            if (newHead != NULL)
                rel->relationsByCounterHead = newHead;
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

    relation *newHead = addrelation(newRel, rel->relationsByCounterEnd);
    if (newHead != NULL) rel->relationsByCounterHead = newHead;
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
    addrel_ref_fixup(root, newEntity);
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
    newNode -> value            = strdup(name);
    newNode -> RB               = 'r';
    newNode -> parent           = treeRelationNil;
    newNode -> right            = treeRelationNil;
    newNode -> left             = treeRelationNil;
    newNode -> relationsByCounterHead = create_relation(origEnt, destEnt, root);
    if (newNode -> relationsByCounterHead == NULL){
        free(newNode);
        return NULL;
    }
    newNode -> relationsByCounterEnd = newNode -> relationsByCounterHead;
    newNode -> relationsByCounterHead -> next = NULL;
    newNode -> relationsByCounterHead -> prev = NULL;
    newNode -> relationsByCounterHead -> outelems -> RB = 'b'; //Set the created root black

    newNode -> relationByName   = create_relRef(newNode -> relationsByCounterHead);
    newNode -> relationByName -> RB = 'b';
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
            addrel_ref(&(x->relationByName), origEnt, destEnt, entRoot, x); //Update existing relation values
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

/**
 * This function prints the values of the node specified formatted for report function
 * @param x the node of which to print the values
 */
void report(relNode *x){
    relation *current = x->relationsByCounterHead;
    fputs(x->value, stdout);
    fputs(" ", stdout);
    while (current->counter == x->relationsByCounterHead->counter) {
        fputs(current->inEnt->value, stdout);
        fputs(" ", stdout);
        if(current->next != NULL)
            current = current->next;
        else break;
    }
    printf("%d",x->relationsByCounterHead->counter);
    fputs("; ", stdout);
}

/**
 * This function is used to print tree values in alphabetical order
 * @param x the root of the tree
 */
void inorder_rel_tree_walk(relNode *x, int flag){
    if (x != treeRelationNil){
        inorder_rel_tree_walk(x->left, 0);
        report(x);
        //printf("%s ", x->value);
        inorder_rel_tree_walk(x->right, 0);
    } else if (flag == 1)
        fputs("none", stdout);
}
//------------------------------ End functions for addrel ------------------------------
