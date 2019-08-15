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
    relation                *relationsByCounter; //The tree which contains the relation sorted by counter and name
    relRef                  *relationByName; //The root of the tree which keep relations sorted by incoming entity name
} relNode;

/**
* This struct is the node of a subtree containing relations between entities, it contains pointer to the entity
* involved, a counter to count number of incoming relations, and another tree which contains outcoming entities in
* alphabetical order. The subtree is contained in relations tree
*/
struct relation {
    struct relation         *parent;
    struct relation         *left;
    struct relation         *right;
    char                    RB; //This field indicates if the node if red or black, value is 'r' if red, 'b' if black
    entNode                 *inEnt; //Entity with incoming relation
    int                     counter; //Entity with outcoming relation
    outElem                 *outelems; //The pointer to the relation
    relNode                 *relationName; //The reference to the relNode
};

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
relation        *treeRelNil;
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

    //Relation subtree initialization
    treeRelNil = malloc(sizeof(relation));
    treeRelNil->parent = treeRelNil;
    treeRelNil->left = treeRelNil;
    treeRelNil->right = treeRelNil;
    treeRelNil->RB = 'b';
}
int allZero = 1;
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
    void delent(entNode **root, char *name, relNode *relNodeRoot);
    void delrel(relNode *root, char *relName, char *origEnt, char *destEnt);
    void inorder_rel_tree_walk(relNode *x, int flag);
    //End of functions' prototype

    while (scanf("%s", command)) {
        if (0 == strcmp(command, "addent")) {
            scanf("%s\n", entityName);
            addent(entityName, &treeEntityRoot);
        } else if (0 == strcmp(command, "delent")) {
            scanf("%s\n", entityName);
            delent(&treeEntityRoot, entityName, treeRelationRoot);
        } else if (0 == strcmp(command, "addrel")) {
            scanf("%s", entityName);
            scanf("%s", entityDestName);
            scanf("%s\n", relationName);
            addrel(relationName, (entityDestName), (entityName), treeEntityRoot, &treeRelationRoot);
        } else if (0 == strcmp(command, "delrel")) {
            scanf("%s", entityName);
            scanf("%s", entityDestName);
            scanf("%s\n", relationName);
            delrel(treeRelationRoot, relationName, entityName, entityDestName);
        } else if (0 == strcmp(command, "report")) {
            inorder_rel_tree_walk(treeRelationRoot, 1);
            fputs("\n", stdout);
            allZero = 1;
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
    if (x->parent == treeEntityNil)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
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
    long cmp;

    while (x != treeEntityNil) {
        y = x;
        cmp = strcmp(entity, x->value);
        if(cmp == 0)
            return;
        else if (cmp < 0)
            x = x->left;
        else
            x = x->right;
    }
    newEntity = create_ent_node(entity);
    newEntity->parent = y;
    if (y == treeEntityNil) {
        *root = newEntity;
    }
    else if (cmp < 0)
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
    int stringCmp;
    if (x != treeEntityNil) stringCmp = strcmp(name, x->value);
    while ((x != treeEntityNil) && stringCmp != 0){
        if (stringCmp < 0)
            x = x->left;
        else
            x = x->right;
        if (x != treeEntityNil) stringCmp = strcmp(name, x->value);
    }
    return x;
}

/**
 * This function search in the entity tree for the value specified as parameter
 * @param root the entity tree root
 * @param name the value to search for
 * @return the node which contain the value or treeEntityNil if the value is not in the tree
 */
void search_double_entity(entNode *root, char name1[], char name2[], entNode *ret[2]){
    entNode *x = root;
    entNode *lastCommonNode = x;
    int stringCmp1;
    int stringCmp2;
    if (x != treeEntityNil) stringCmp1 = strcmp(name1, x->value);
    if (x != treeEntityNil) stringCmp2 = strcmp(name2, x->value);
    int  compareCommon = 1;
    while ((x != treeEntityNil) && stringCmp1 != 0){
        if (stringCmp1 < 0) {
            if ((stringCmp2 < 0) && compareCommon) {
                stringCmp2 = strcmp(name2, x->value);
                lastCommonNode = x;
            } else compareCommon = 0;
            x = x->left;
        }
        else {
            if ((stringCmp2 > 0) && compareCommon) {
                lastCommonNode = x;
                stringCmp2 = strcmp(name2, x->value);
            } else compareCommon = 0;
            x = x->right;
        }
        if (x != treeEntityNil)
            stringCmp1 = strcmp(name1, x->value);
    }
    ret[0] = x;
    if (lastCommonNode != treeEntityNil) stringCmp2 = strcmp(name2, lastCommonNode->value);
    while ((lastCommonNode != treeEntityNil) && stringCmp2 != 0){
        if (stringCmp2 < 0)
            lastCommonNode = lastCommonNode->left;
        else
            lastCommonNode = lastCommonNode->right;
        if (lastCommonNode != treeEntityNil)
            stringCmp2 = strcmp(name2, lastCommonNode->value);
    }
    ret[1] = lastCommonNode;
}
//------------------------------ End functions for entity tree ------------------------------


//------------------------------ Functions for delent------------------------------
/**
 * This function return the maximum value in entities tree
 * @param root the root of the tree
 * @return the maximum in the tree
 */
entNode* tree_entity_max(entNode *root){
    entNode* x = root;
    while (x->right != treeEntityNil)
        x = x->right;
    return x;
}

/**
 * This function return the minimum value in entities tree
 * @param root the root of the tree
 * @return the minimum in the tree
 */
entNode* tree_entity_min(entNode *root){
    entNode* x = root;
    while (x->left != treeEntityNil)
        x = x->left;
    return x;
}

void entity_transplant(entNode **root, entNode **u, entNode **v){
    if ((*u)->parent == treeEntityNil)
        *root = *v;
    else if (*u == (*u)->parent->left)
        (*u)->parent->left = *v;
    else
        (*u)->parent->right = *v;
    (*v)->parent = (*u)->parent;
}

void entity_delete_fixup(entNode **root, entNode *x){
    entNode *w;
    while (x != *root && x->RB == 'b'){
        if (x == x->parent->left){
            w = x->parent->right;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                entity_left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->RB == 'b' && w->right->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->right->RB == 'b'){
                    w->left->RB = 'b';
                    w->RB = 'r';
                    entity_right_rotate(root, w);
                    w = x->parent->right;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->right->RB = 'b';
                entity_left_rotate(root, x->parent);
                x = *root;
            }
        } else{
            w = x->parent->left;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                entity_right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->RB == 'b' && w->left->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->left->RB == 'b'){
                    w->right->RB = 'b';
                    w->RB = 'r';
                    entity_left_rotate(root, w);
                    w = x->parent->left;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->left->RB = 'b';
                entity_right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->RB = 'b';
}

void entity_delete(entNode **root, entNode *z){
    entNode *toFree = z;
    entNode *x;
    entNode *y = z;
    char yOriginalColor = y->RB;
    if (z->left == treeEntityNil){
        x = z->right;
        entity_transplant(root, &z, &(z->right));
    } else if (z->right == treeEntityNil){
        x = z->left;
        entity_transplant(root, &z, &(z->left));
    } else{
        y = tree_entity_min(z->right);
        yOriginalColor = y->RB;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else{
            entity_transplant(root, &y, &(y->right));
            y->right = z->right;
            y->right->parent = y;
        }
        entity_transplant(root, &z, &y);
        y->left = z->left;
        y->left->parent = y;
        y->RB = z->RB;
    }
    if (yOriginalColor == 'b')
        entity_delete_fixup(root, x);
    free(toFree->value);
    free(toFree);
}

void delent(entNode **root, char *name, relNode *relNodeRoot){
    void decrement_relation(relation **root, relation* rel, entNode *outelem, outElem *out);
    relNode* search_rel_node(relNode *root, char name[]);
    void outelem_delete_all(outElem *x, relation *rel);
    relation* relation_delete(relation **root, relation *z);
    void ref_delete(relRef **root, relRef *z);
    relRef* search_ref(relRef *root, char *k);
    int sameRel = 0;
    char *entName = strdup(name);

    entNode *delNode = search_entity(*root, entName);
    if (delNode != treeEntityNil){
        elemRelList *current = delNode->relations;
        while (current != NULL){
            if (delNode != current->rel->inEnt) {
                //if(!sameRel) {
                    decrement_relation(
                            &(search_rel_node(relNodeRoot, current->rel->relationName->value)->relationsByCounter),
                            current->rel, delNode, NULL);
                    if (current->next != NULL) sameRel = current->rel == current->next->rel;
                    current = current->next;
                /*} else{
                    if (current->next != NULL) sameRel = current->rel == current->next->rel;
                    elemRelList *tmp = current->next;
                    free(current);
                    current = tmp;*/
                //}
            } else{
                if (!sameRel) {
                    /*elemRelList *prev = current;
                    elemRelList *tmp = current->next;
                    relation *currRel = current->rel;
                    while (tmp != NULL){
                        if (tmp->rel->relationName == currRel->relationName && tmp->rel->inEnt == currRel->inEnt){
                            prev->next = tmp->next;
                        }
                        prev = tmp;
                        tmp = tmp->next;
                    }*/
                    elemRelList *tmp;
                    if (current->next != NULL) sameRel = current->rel == current->next->rel;
                    if (sameRel) tmp = current->next->next;
                    else tmp = current->next;
                    if (search_ref(current->rel->relationName->relationByName, current->rel->inEnt->value) != treeRelRefNil) {
                        relation_delete(&(current->rel->relationName->relationsByCounter), current->rel);
                        ref_delete(&(current->rel->relationName->relationByName),
                                   search_ref(current->rel->relationName->relationByName, current->rel->inEnt->value));
                        outelem_delete_all(current->rel->outelems, current->rel);
                    }
                    current = tmp;
                    sameRel = 0;
                }else{
                    if (current->next != NULL) sameRel = current->rel == current->next->rel;
                    elemRelList *tmp = current->next;
                    free(current);
                    current = tmp;
                }
            }
        }
        entity_delete(root, delNode);
    }
    free(entName);
}
//------------------------------ End functions for delent ------------------------------


//------------------------------ Functions for delrel ------------------------------
void delrel(relNode *root, char *relName, char *origEnt, char *destEnt){
    void decrement_relation(relation **root, relation* rel, entNode *outelem, outElem *out);
    relNode* search_rel_node(relNode *root, char name[]);
    relRef* search_ref(relRef *root, char *k);
    outElem* search_outelem(outElem *root, char *k);

    char *origName = strdup(origEnt);
    char *destName = strdup(destEnt);
    char *relationName = strdup(relName);
    relNode *relNameNode = search_rel_node(root, relationName);
    if (relNameNode != treeRelationNil){
        relRef *inEntRef = search_ref(relNameNode->relationByName, destName);
        if (inEntRef != treeRelRefNil){
            outElem *outEntRef = search_outelem(inEntRef->reference->outelems, origName);
            if (outEntRef != treeOutElemNil){
                elemRelList *current = outEntRef->outEnt->relations;
                elemRelList *prev = NULL;
                while (current != NULL){
                    if (current->rel == inEntRef->reference){
                        if (prev == NULL) outEntRef->outEnt->relations = current->next;
                        else prev->next = current->next;
                        break;
                    } else{
                        prev = current;
                        current = current->next;
                    }
                }
                decrement_relation(&(relNameNode->relationsByCounter), inEntRef->reference, NULL, outEntRef);
                if(current != NULL) free(current);
            }
        }
    }
    free(origName);
    free(destName);
    free(relationName);
}
//------------------------------ End functions for delrel ------------------------------


//------------------------------ Functions for outelems tree ------------------------------
/**
* This function create an outElem node with values specified for the relation subtree
* @param root the entity tree root
* @param outEnt the entity name
* @return an outElem struct with values specified as parameters
*/
outElem *create_outElem(entNode *outEnt){
    outElem *newNode = malloc(sizeof(outElem));
    newNode -> parent    = treeOutElemNil;
    newNode -> left      = treeOutElemNil;
    newNode -> right     = treeOutElemNil;
    newNode -> RB        = 'r'; //This field indicates if the node is red or black, value is 'r' if red, 'b' if black
    entNode *entity      = outEnt;
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
    if (x->parent == treeOutElemNil)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
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
    newEntity = create_outElem(search_entity(entRoot, outEnt));
    if (newEntity == NULL)
        return NULL;
    newEntity->parent = y;
    if (y == treeOutElemNil) {
        *root = newEntity;
    }
    else if (comparison < 0)
        y->left = newEntity;
    else
        y->right = newEntity;
    newEntity->left = treeOutElemNil;
    newEntity->right = treeOutElemNil;
    newEntity->RB = 'r';
    add_outelem_fixup(root, newEntity);
    return newEntity;
}

/**
 * This function return the maximum value in outelems tree
 * @param root the root of the tree
 * @return the maximum in the tree
 */
outElem* tree_outelem_max(outElem *root){
    outElem* x = root;
    while (x->right != treeOutElemNil)
        x = x->right;
    return x;
}

/**
 * This function return the minimum value in entities tree
 * @param root the root of the tree
 * @return the minimum in the tree
 */
outElem* tree_outelem_min(outElem *root){
    outElem* x = root;
    while (x->left != treeOutElemNil)
        x = x->left;
    return x;
}

void outelem_transplant(outElem **root, outElem **u, outElem **v){
    if ((*u)->parent == treeOutElemNil)
        *root = *v;
    else if (*u == (*u)->parent->left)
        (*u)->parent->left = *v;
    else
        (*u)->parent->right = *v;
    (*v)->parent = (*u)->parent;
}

void outelem_delete_fixup(outElem **root, outElem *x){
    outElem *w;
    while (x != *root && x->RB == 'b'){
        if (x == x->parent->left){
            w = x->parent->right;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                outelem_left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->RB == 'b' && w->right->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->right->RB == 'b'){
                    w->left->RB = 'b';
                    w->RB = 'r';
                    outelem_right_rotate(root, w);
                    w = x->parent->right;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->right->RB = 'b';
                outelem_left_rotate(root, x->parent);
                x = *root;
            }
        } else{
            w = x->parent->left;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                outelem_right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->RB == 'b' && w->left->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->left->RB == 'b'){
                    w->right->RB = 'b';
                    w->RB = 'r';
                    outelem_left_rotate(root, w);
                    w = x->parent->left;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->left->RB = 'b';
                outelem_right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->RB = 'b';
}

void outelem_delete(outElem **root, outElem *z){
    outElem *x;
    outElem *y = z;
    char yOriginalColor = y->RB;
    if (z->left == treeOutElemNil){
        x = z->right;
        outelem_transplant(root, &z, &(z->right));
    } else if (z->right == treeOutElemNil){
        x = z->left;
        outelem_transplant(root, &z, &(z->left));
    } else{
        y = tree_outelem_min(z->right);
        yOriginalColor = y->RB;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else{
            outelem_transplant(root, &y, &(y->right));
            y->right = z->right;
            y->right->parent = y;
        }
        outelem_transplant(root, &z, &y);
        y->left = z->left;
        y->left->parent = y;
        y->RB = z->RB;
    }
    if (yOriginalColor == 'b')
        outelem_delete_fixup(root, x);
    free(z);
}

/**
 * This function delete all the node in outelems and delete also reference to relation in entity
 * @param x the root of the tree
 * @param rel the relation which contains the root
 */
void outelem_delete_all(outElem *x, relation *rel){
    if (x != treeOutElemNil){
        outelem_delete_all(x->left, rel);
        outelem_delete_all(x->right, rel);
        elemRelList *current = x->outEnt->relations;
        elemRelList *prev = NULL;
        while (current != NULL){
            if (current->rel == rel){
                if (prev == NULL) x->outEnt->relations = current->next;
                else prev->next = current->next;
                free(current);
                break;
            } else{
                prev = current;
                current = current->next;
            }
        }
        free(x);
    }
}

/**
 * This function search in the entity tree for the value specified as parameter
 * @param root the entity tree root
 * @param name the value to search for
 * @return the node which contain the value or treeEntityNil if the value is not in the tree
 */
outElem* search_outelem(outElem *root, char *k){
    outElem *x = root;
    int cmp;
    if (x != treeOutElemNil) cmp = strcmp(k, x->outEnt->value);
    while ((x != treeOutElemNil) && (cmp != 0)){
        if (cmp < 0)
            x = x->left;
        else
            x = x->right;
        if (x != treeOutElemNil) cmp = strcmp(k, x->outEnt->value);
    }
    return x;
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
relation *create_relation(char *incomingEnt, char *outEnt, entNode* root, relNode *relationName){
    entNode *entities[2];
    //search_double_entity(root, incomingEnt, outEnt, entities);
    entities[0] = search_entity(root, incomingEnt);
    entities[1] = search_entity(root, outEnt);
    relation *newNode = malloc(sizeof(relation));
    newNode->parent = treeRelNil;
    newNode->left = treeRelNil;
    newNode->right = treeRelNil;
    newNode -> inEnt  = entities[0]; //Entity with incoming relation
    if (newNode -> inEnt == treeEntityNil) {
        free(newNode);
        return NULL;
    }
    newNode -> counter   = 1; //Relations counter
    if (entities[1] == NULL) {
        free(newNode);
        return NULL;
    }
    newNode -> outelems  = create_outElem(entities[1]); //Entities with outcoming relation
    if (newNode -> outelems == NULL) {
        free(newNode);
        return NULL;
    }
    newNode-> outelems->RB = 'b';
    newNode->relationName = relationName;

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

//TODO from here starts changes
/**
 * This function made a left rotation of the node specified in the entity tree
 * @param root the entity tree root
 * @param x the node to be rotated
 */
void relation_left_rotate(relation **root, relation *x){
    relation *y = x->right;
    x->right = y->left;
    if (y->left != treeRelNil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == treeRelNil)
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
void relation_right_rotate(relation **root, relation *x){
    relation *y = x->left;
    x->left = y->right;
    if (y->right != treeRelNil)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == treeRelNil)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

/**
 * This function fix entity rb tree after an insertion
 * @param root the root of the entity tree
 * @param z the new node added to the tree
 */
void addRelation_fixup(relation **root, relation *z){
    //printf("Addent fixup for : %s", z->value);
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
void add_relation(relation *newNode, relation **root){
    relation*    x = *root;
    relation*    y = treeRelNil;
    int          cmp;

    while (x != treeRelNil) {
        y = x;
        cmp = strcmp(x->inEnt->value, newNode->inEnt->value);
        if ((newNode->counter < x->counter) || ((newNode->counter == x->counter) && (cmp < 0)))
            x = x->left;
        else
            x = x->right;
    }
    newNode->parent = y;
    if (y == treeRelNil) {
        *root = newNode;
    }
    else if ((newNode->counter < y->counter) || ((newNode->counter == y->counter) && (cmp < 0)))
        y->left = newNode;
    else
        y->right = newNode;
    newNode->RB = 'r';
    addRelation_fixup(root, newNode);
}

/**
 * This function return the maximum value in relation subtree
 * @param root the root of the tree
 * @return the maximum in the tree
 */
relation* tree_relation_max(relation *root){
    relation *x = root;
    while (x->right != treeRelNil)
        x = x->right;
    return x;
}

/**
 * This function return the minimum value in relation subtree
 * @param root the root of the tree
 * @return the minimum in the tree
 */
relation* tree_relation_min(relation *root){
    relation *x = root;
    while (x->left != treeRelNil)
        x = x->left;
    return x;
}

/**
 * This function find the successor of the element passed as argument
 * @param x the node of which find the successor
 * @return the successor of x
 */
relation* tree_successor(relation* x){
    if (x->right != treeRelNil)
        return tree_relation_min(x->right);
    relation *y = x->parent;
    while ((y != treeRelNil) && (x == y->right)){
        x = y;
        y = y->parent;
    }
    return y;
}

void relation_transplant(relation **root, relation **u, relation **v){
    if ((*u)->parent == treeRelNil)
        *root = *v;
    else if (*u == (*u)->parent->left)
        (*u)->parent->left = *v;
    else
        (*u)->parent->right = *v;
    (*v)->parent = (*u)->parent;
}

void relation_delete_fixup(relation **root, relation *x){
    relation *w;
    while (x != *root && x->RB == 'b'){
        if (x == x->parent->left){
            w = x->parent->right;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                relation_left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->RB == 'b' && w->right->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->right->RB == 'b'){
                    w->left->RB = 'b';
                    w->RB = 'r';
                    relation_right_rotate(root, w);
                    w = x->parent->right;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->right->RB = 'b';
                relation_left_rotate(root, x->parent);
                x = *root;
            }
        } else{
            w = x->parent->left;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                relation_right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->RB == 'b' && w->left->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->left->RB == 'b'){
                    w->right->RB = 'b';
                    w->RB = 'r';
                    relation_left_rotate(root, w);
                    w = x->parent->left;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->left->RB = 'b';
                relation_right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->RB = 'b';
}

relation* relation_delete(relation **root, relation *z){
    relation *x;
    relation *y = z;
    char yOriginalColor = y->RB;
    if (z->left == treeRelNil){
        x = z->right;
        relation_transplant(root, &z, &(z->right));
    } else if (z->right == treeRelNil){
        x = z->left;
        relation_transplant(root, &z, &(z->left));
    } else{
        y = tree_relation_min(z->right);
        yOriginalColor = y->RB;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else{
            relation_transplant(root, &y, &(y->right));
            y->right = z->right;
            y->right->parent = y;
        }
        relation_transplant(root, &z, &y);
        y->left = z->left;
        y->left->parent = y;
        y->RB = z->RB;
    }
    if (yOriginalColor == 'b')
        relation_delete_fixup(root, x);
    return z;
}

/**
 * This function update an existing relation adding one to the counter
 * @param rel the relation to be updated
 * @param outEntity the entity name with outgoing relation to be added to outelems
 */
void update_relation(relation **root, relation* rel, char *outEntity, entNode *entRoot){
    outElem *newOutElem = add_outelem(&(rel -> outelems), outEntity, entRoot);
    if (NULL != newOutElem) {
        if (((rel->left->counter < rel->counter+1) && (rel->right->counter > rel->counter+1) && (((rel->parent->left == rel) && (rel->parent->counter > rel->counter+1)) || ((rel->parent->right == rel) && (rel->parent->counter < rel->counter+1)))))
            rel->counter++;
        else{
            relation *z = relation_delete(root, rel);
            z->counter++;
            z->parent = treeRelNil;
            z->left = treeRelNil;
            z->right = treeRelNil;
            add_relation(z, root);
        }
        elemRelList *newElem = malloc(sizeof(elemRelList));
        newElem -> next = newOutElem -> outEnt-> relations;
        newElem -> rel = rel;
        newOutElem -> outEnt -> relations = newElem;
    }
}

/**
 * This function update an existing relation subtracting one to the relation
 * @param rel the relation to be updated
 */
void decrement_relation(relation **root, relation* rel, entNode *outelem, outElem *out){
    outElem *outElement;
    if (out != NULL) outElement = out;
    else outElement = search_outelem(rel->outelems, outelem->value);

    if ((( ( ((rel->left->counter < rel->counter - 1) && (rel->left != treeRelNil) ) &&
            ((rel->right->counter > rel->counter - 1) && (rel->right != treeRelNil)))) &&
            (((rel->parent->left == rel) && (rel->parent->counter > rel->counter - 1) && (rel->parent != treeRelNil)) ||
            ((rel->parent->right == rel) && (rel->parent->counter < rel->counter - 1) && (rel->parent != treeRelNil))))) {
        rel->counter--;
        outelem_delete(&(rel->outelems), outElement);
    } else {
        relation *z = relation_delete(root, rel);
        z->counter--;
        outelem_delete(&(z->outelems), outElement);
        z->parent = treeRelNil;
        z->left = treeRelNil;
        z->right = treeRelNil;
        add_relation(z, root);
    }
}

/**
 * This function find the predecessor of the element passed as argument
 * @param x the node of which find the predecessor
 * @return the predecessor of x
 */
relation* tree_predecessor(relation* x){
    if (x->left != treeRelNil)
        return tree_relation_max(x->left);
    relation *y = x->parent;
    while ((y != treeRelNil) && (x == y->left)){
        x = y;
        y = y->parent;
    }
    return y;
}

//TODO end of changes
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
    if (x->parent == treeRelRefNil)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
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
            update_relation(&(rel->relationsByCounter), x->reference, outEnt, entRoot);
            return;
        }
        else if (comparison < 0)
            x = x->left;
        else
            x = x->right;
    }
    relation* newRel = create_relation(inEnt, outEnt, entRoot, rel);
    if (newRel == NULL)
        return;

    add_relation(newRel, &(rel->relationsByCounter));
    newEntity = create_relRef(newRel);
    newEntity->parent = y;
    if (y == treeRelRefNil) {
        *root = newEntity;
    }
    else if (comparison < 0)
        y->left = newEntity;
    else
        y->right = newEntity;
    newEntity->left = treeRelRefNil;
    newEntity->right = treeRelRefNil;
    newEntity->RB = 'r';
    addrel_ref_fixup(root, newEntity);
}

/**
 * This function return the maximum value in reference tree
 * @param root the root of the tree
 * @return the maximum in the tree
 */
relRef* tree_ref_max(relRef *root){
    relRef* x = root;
    while (x->right != treeRelRefNil)
        x = x->right;
    return x;
}

/**
 * This function return the minimum value in reference tree
 * @param root the root of the tree
 * @return the minimum in the tree
 */
relRef* tree_ref_min(relRef *root){
    relRef* x = root;
    while (x->left != treeRelRefNil)
        x = x->left;
    return x;
}

void ref_transplant(relRef **root, relRef **u, relRef **v){
    if ((*u)->parent == treeRelRefNil)
        *root = *v;
    else if (*u == (*u)->parent->left)
        (*u)->parent->left = *v;
    else
        (*u)->parent->right = *v;
    (*v)->parent = (*u)->parent;
}

void ref_delete_fixup(relRef **root, relRef *x){
    relRef *w;
    while (x != *root && x->RB == 'b'){
        if (x == x->parent->left){
            w = x->parent->right;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                ref_left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->RB == 'b' && w->right->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->right->RB == 'b'){
                    w->left->RB = 'b';
                    w->RB = 'r';
                    ref_right_rotate(root, w);
                    w = x->parent->right;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->right->RB = 'b';
                ref_left_rotate(root, x->parent);
                x = *root;
            }
        } else{
            w = x->parent->left;
            if (w->RB == 'r'){
                w->RB = 'b';
                x->parent->RB = 'r';
                ref_right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->RB == 'b' && w->left->RB == 'b') {
                w->RB = 'r';
                x = x->parent;
            } else{
                if (w->left->RB == 'b'){
                    w->right->RB = 'b';
                    w->RB = 'r';
                    ref_left_rotate(root, w);
                    w = x->parent->left;
                }
                w->RB = x->parent->RB;
                x->parent->RB = 'b';
                w->left->RB = 'b';
                ref_right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->RB = 'b';
}

void ref_delete(relRef **root, relRef *z){
    relRef *x;
    relRef *y = z;
    char yOriginalColor = y->RB;
    if (z->left == treeRelRefNil){
        x = z->right;
        ref_transplant(root, &z, &(z->right));
    } else if (z->right == treeRelRefNil){
        x = z->left;
        ref_transplant(root, &z, &(z->left));
    } else{
        y = tree_ref_min(z->right);
        yOriginalColor = y->RB;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else{
            ref_transplant(root, &y, &(y->right));
            y->right = z->right;
            y->right->parent = y;
        }
        ref_transplant(root, &z, &y);
        y->left = z->left;
        y->left->parent = y;
        y->RB = z->RB;
    }
    if (yOriginalColor == 'b')
        ref_delete_fixup(root, x);
}

relRef* search_ref(relRef *root, char *k){
    relRef *x = root;
    int cmp;
    if (x != treeRelRefNil) cmp = strcmp(k, x->reference->inEnt->value);
    while ((x != treeRelRefNil) && (cmp != 0)){
        if (cmp < 0)
            x = x->left;
        else
            x = x->right;
        if (x != treeRelRefNil) cmp = strcmp(k, x->reference->inEnt->value);
    }
    return x;
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
    newNode -> relationsByCounter = create_relation(origEnt, destEnt, root, NULL);
    if (newNode -> relationsByCounter == NULL){
        free(newNode->value);
        free(newNode);
        return NULL;
    }
    newNode -> relationsByCounter -> outelems -> RB = 'b'; //Set the created root black

    newNode -> relationByName   = create_relRef(newNode -> relationsByCounter);
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
    if (x->parent == treeRelationNil)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
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
    long cmp;

    while (x != treeRelationNil) {
        y = x;
        cmp = strcmp(relation, x->value);
        if(cmp == 0) {
            addrel_ref(&(x->relationByName), origEnt, destEnt, entRoot, x); //Update existing relation values
            return;
        }
        else if (cmp < 0)
            x = x->left;
        else
            x = x->right;
    }
    newEntity = create_rel_node(relation, origEnt, destEnt, entRoot);
    if (newEntity == NULL)
        return;
    newEntity->relationsByCounter->relationName = newEntity;
    newEntity->parent = y;
    if (y == treeRelationNil) {
        *root = newEntity;
    }
    else if (cmp < 0)
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
 * This function search in the relNode tree for the value specified as parameter
 * @param root the relNode tree root
 * @param name the value to search for
 * @return the node which contain the value or treeRelationNil if the value is not in the tree
 */
relNode* search_rel_node(relNode *root, char name[]){
    relNode *x = root;
    int stringCmp;
    if (x != treeRelationNil) stringCmp = strcmp(name, x->value);
    while ((x != treeRelationNil) && stringCmp != 0){
        if (stringCmp < 0)
            x = x->left;
        else
            x = x->right;
        if (x != treeRelationNil) stringCmp = strcmp(name, x->value);
    }
    return x;
}

/**
 * This function prints the values of the node specified formatted for report function
 * @param x the node of which to print the values
 */
void report(relNode *x, relation *current){
    if(current != treeRelNil) {
        int max = current->counter;
        if (max != 0) {
            fputs(x->value, stdout);
            fputs(" ", stdout);
            while (current->counter == max) {
                fputs(current->inEnt->value, stdout);
                fputs(" ", stdout);
                current = tree_predecessor(current);
                if (current == treeRelNil)
                    break;
            }
            printf("%d", max);
            fputs("; ", stdout);
        }
    }
}

/**
 * This function is used to print tree values in alphabetical order
 * @param x the root of the tree
 */
void inorder_rel_tree_walk(relNode *x, int flag){
    if (x != treeRelationNil){
        relation *current = tree_relation_max(x->relationsByCounter);
        if (allZero && current->counter == 0) allZero = 1;
        else allZero = 0;
        inorder_rel_tree_walk(x->left, 0);
        report(x, current);
        inorder_rel_tree_walk(x->right, 0);
    } else if (flag == 1) {
        fputs("none", stdout);
        return;
    }
    if (allZero && flag)
        fputs("none", stdout);
}
//------------------------------ End functions for addrel ------------------------------
