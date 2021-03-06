//
// Created by Nafany on 07.04.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyspace2.h"
#include "keyspace1.h"
#include "input.h"

KeySpace2* makeKeySpace2(int maxsize2){
    KeySpace2* keySpace2 = (KeySpace2*)calloc(sizeof(KeySpace2), maxsize2);
    return keySpace2;
}

KeySpace2* copyKeySpace2(KeySpace2* keySpace2, Node1* node, Node1* newNode, int releases){
    KeySpace2* newKeySpace2 = (KeySpace2*)malloc(releases * sizeof(KeySpace2));
    KeySpace2* newPointer = newKeySpace2;
    Node1* nodePointer = node;
    Node1* nodeNewPointer = newNode;
    while(nodePointer != NULL){
        newPointer->info = nodeNewPointer->item;
        newPointer->info->key2 = newPointer;
        newPointer->busy = BUSY;
        newPointer->key = nodePointer->item->key2->key;
        newPointer++;
        nodeNewPointer = nodeNewPointer->next;
        nodePointer = nodePointer->next;
    }
    return newKeySpace2;
}

int hash1(char* key){
    int keyInt = 0;
    char* helpKey = key;
    while(*helpKey != '\0'){
        keyInt += (int)*helpKey++;
    }
    return keyInt;
}
int hash2(char* key){
    int keyInt = 0;
    int steps[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    char* helpKey = key;
    while(*helpKey != '\0'){
        keyInt += (int)*helpKey++;
    }
    return steps[keyInt%10];
}
int getIndex(char* key, int step, int maxsize2){
    return (hash1(key) + step * hash2(key))%maxsize2;
}
Item* addNewKeySpace2(KeySpace2* keySpace2, int maxsize2, char* key, char* info){
    for(int i = 0; i < maxsize2; i++){
        int index = getIndex(key, i, maxsize2);
        if(keySpace2[index].busy == BUSY){
            if(strcmp(keySpace2[index].key, key) == 0) {
                printf("Такой ключ уже есть во втором пространстве ключей! Отказано.\n");
                return NULL;
            }
            continue;
        }
        else{
            KeySpace2* pointerKey = &keySpace2[index];
            pointerKey->info = createItem(info);
            pointerKey->info->key2 = pointerKey;
            pointerKey->key = key;
            pointerKey->busy = BUSY;
            return pointerKey->info;
        }
    }
    printf("Не удалось найти свободный индекс в таблице!\n");
    return NULL;
}

void addItemKeySpace2(Item* item, KeySpace2* keySpace2, int maxsize2){
    printf("Введите ключ для второго пространства (не более 4 символов): ");
    char* key;

    do{
        key = get_String();
    }while(strlen(key) > MAXSTRING || *key == '\0');

    for(int i = 0; i < maxsize2; i++){
        int index = getIndex(key, i, maxsize2);
        if(keySpace2[index].busy == BUSY){
            if(strcmp(keySpace2[index].key, key) == 0) {
                printf("Такой ключ уже есть во втором пространстве ключей! Отказано.\n");
                return;
            }
            continue;
        }
        else{
            KeySpace2* pointerKey = &keySpace2[index];
            pointerKey->info = item;
            pointerKey->info->key2 = pointerKey;
            pointerKey->key = key;
            pointerKey->busy = BUSY;
        }
    }
    printf("Не удалось найти свободный индекс в таблице!\n");
}

Item* findByKey2(KeySpace2* keySpace2, char* key, int maxsize2){
    for(int step = 0; step < maxsize2; step++){
        int index = getIndex(key, step, maxsize2);
        KeySpace2* pointerHelp = &keySpace2[index];
        if(pointerHelp->busy == BUSY && strcmp(pointerHelp->key, key) == 0){
            return pointerHelp->info;
        }
        if(pointerHelp->busy == FREE)
            break;
    }
    printf("Не удалось найти этот ключ!\n");
    return NULL;
}

void deleteByKey(KeySpace2* keySpace2, KeySpace1* keySpace1, char* key, int maxsize2, int* nsize1){
    for(int step = 0; step < maxsize2; step++){
        int index = getIndex(key, step, maxsize2);
        KeySpace2* pointerHelp = &keySpace2[index];
        if(pointerHelp->busy == BUSY && strcmp(pointerHelp->key, key) == 0) {
            pointerHelp->busy = DELETED;
            Item* item = pointerHelp->info;
            deleteOneKeySpace1(item, keySpace1, nsize1);
            return;
        }
        if(pointerHelp->busy == FREE)
            break;
    }
    printf("Не удалось найти такой ключ.\n");
}

void printKeySpace2(KeySpace2* keySpace2, int maxsize2){
    KeySpace2* pointerHelp = keySpace2;
    printf("\n\n/////////////////////// ТАБЛИЦА. ПРОСТРАНСТВО КЛЮЧЕЙ 2. ///////////////////////\n");
    for(int i = 0; i < maxsize2; i++, pointerHelp++){
        if(pointerHelp->busy == BUSY) {
            printf("%d. Информация: \"%s\". Ключ в первом пространстве: %d (вер. %d). Ключ во втором пространстве: %s."
                   "\n", i + 1, pointerHelp->info->info, pointerHelp->info->key1->key,
                   pointerHelp->info->node1->release, pointerHelp->key);
        }
        else{
            printf("%d.\n", i + 1);
        }
    }
    printf("\n\n");
}