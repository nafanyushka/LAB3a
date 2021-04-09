//
// Created by Nafany on 07.04.2021.
//

#ifndef LAB3_ITEM_H
#define LAB3_ITEM_H

typedef struct Item Item;
typedef struct KeySpace1 KeySpace1;
typedef struct KeySpace2 KeySpace2;
typedef struct Node1 Node1;

struct Item{
    char* info;
    KeySpace1* key1;
    KeySpace2* key2;
    Node1* node1;
};

Item* createItem();
void printInfo(Item*);
void freeItem(Item*);
Item* copyItem(Item*);

#endif //LAB3_ITEM_H
