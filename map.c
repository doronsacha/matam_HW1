

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "map.h"

typedef struct node* Node;

struct node
{
    MapDataElement data;
    MapKeyElement key;
    Node next;
};


//the map itself
struct Map_t {
    Node Head; //first element of the map
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
    Node iterator; //iterator used to move into the map
};
//----------------------------------------------------------------------------------------------------------------------

static void deleteAllTheNodes(Map map);


Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if( copyDataElement==NULL || copyKeyElement==NULL || freeDataElement==NULL || freeKeyElement==NULL || compareKeyElements==NULL)
    {
        return NULL;
    }
    Map NewMap = malloc(sizeof(*NewMap));
    if (NewMap==NULL)
    {
        return NULL;
    }

    NewMap->Head=NULL;
    NewMap->copyDataElement=copyDataElement;
    NewMap->copyKeyElement=copyKeyElement;
    NewMap->freeDataElement=freeDataElement;
    NewMap->freeKeyElement=freeKeyElement;
    NewMap->compareKeyElements=compareKeyElements;
    NewMap->iterator=NULL;

    return NewMap;
}

void mapDestroy(Map map)
{
    if(map==NULL)
    {
        return ;
    }
    mapClear(map);
    free(map);
}

Map mapCopy(Map map)
{
    if(map==NULL)
    {
        return NULL;
    }
    Map  map_copy = mapCreate(map->copyDataElement, map->copyKeyElement, map->freeDataElement, map->freeKeyElement, map->compareKeyElements);
    if(map_copy==NULL)
    {
        return NULL;
    }
    if(map->Head==NULL)
    {
        return map_copy;
    }

    map_copy->iterator = map->Head;
    map_copy->Head=malloc(sizeof(*map_copy->Head));//
    map_copy->Head->key=map_copy->copyKeyElement(map_copy->iterator->key);
    map_copy->Head->data=map_copy->copyDataElement(map_copy->iterator->data);
    map_copy->iterator=map_copy->iterator->next;

    Node tempNode =map_copy->Head;

    while(map_copy->iterator!=NULL)
    {
        tempNode->next=malloc(sizeof(*tempNode->next));
        if(tempNode->next==NULL)
        {
            return NULL;
        }
        tempNode->next->key=map_copy->copyKeyElement(map_copy->iterator->key);
        if(tempNode->next->key==NULL)
        {
            free(tempNode);
            mapDestroy(map_copy);
        }

        tempNode->next->data=map_copy->copyDataElement(map_copy->iterator->data);

        if(tempNode->next->data == NULL)
        {
            free(tempNode->next->key);
            free(tempNode);
            mapDestroy(map_copy);
        }
        map_copy->iterator=map_copy->iterator->next;
        tempNode=tempNode->next;
    }

    map_copy->iterator=NULL;
    return map_copy;
}


int mapGetSize(Map map)
{
    if (map==NULL)
    {
        return -1;
    }
    int counter=0;
    map->iterator=map->Head;
    while(map->iterator!=NULL)
    {
        map->iterator=map->iterator->next;
        counter++;
    }
    return counter;
}


bool mapContains(Map map, MapKeyElement element)
{
    if(map==NULL || element==NULL)
    {
        return false;
    }

    map->iterator= map->Head;

    while(map->iterator!=NULL)
    {

        if((map->compareKeyElements)(map->iterator->key, element)==0)
        {

            return true;
        }
        map->iterator=map->iterator->next;
    }
    return false;
}

static Node ValidPlace(MapKeyElement keyElement, Map map) {
    Node ptrNode = map->Head;
    Node ptrNodeAfter = map->Head->next;
    while (ptrNodeAfter) {
        if (map->compareKeyElements(ptrNodeAfter->key, keyElement) == 0) {
            return ptrNodeAfter;
        }
        else if (map->compareKeyElements(ptrNodeAfter->key, keyElement)>0 ) {
            return ptrNode;
        }

        ptrNode = ptrNodeAfter;
        ptrNodeAfter = ptrNodeAfter->next;
    }
    return ptrNode;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if (!map || !keyElement || !dataElement ) {
        return MAP_NULL_ARGUMENT;
    }
    if (!map->Head) {
        Node NewNode = malloc(sizeof(*NewNode));
        if (!NewNode) {
            return MAP_OUT_OF_MEMORY;
        }
        map->Head = NewNode;
        map->Head->key = map->copyKeyElement(keyElement);
        map->Head->data = map->copyDataElement(dataElement);
        map->Head->next = NULL;
        if (!map->Head->key || !map->Head->data) {
            return MAP_OUT_OF_MEMORY;
        }
        //map->iterator = NULL;/
        return MAP_SUCCESS;
    }
    if (map->compareKeyElements(keyElement, map->Head->key)<0) {
        Node NewNode = malloc(sizeof(*NewNode));
        if (!NewNode) {
            return MAP_OUT_OF_MEMORY;
        }
        NewNode->next = map->Head;
        map->Head = NewNode;
        NewNode->key = map->copyKeyElement(keyElement);
        NewNode->data = map->copyDataElement(dataElement);
        if (!NewNode->key || !NewNode->data) {
            free(NewNode->key);
            free(NewNode->data);
            free(NewNode);
            return MAP_OUT_OF_MEMORY;
        }
        // map->iterator = NULL;
        return MAP_SUCCESS;
    }
    Node ptrNode = ValidPlace(keyElement, map);
    if (map->compareKeyElements(ptrNode->key, keyElement) == 0) {
        map->freeDataElement(ptrNode->data);
        ptrNode->data = map->copyDataElement(dataElement);
        // map->iterator = NULL;
        return MAP_SUCCESS;
    }
    Node NewNode = malloc(sizeof(*NewNode));
    if (!NewNode) {
        return MAP_OUT_OF_MEMORY;
    }
    NewNode->next = ptrNode->next;
    ptrNode->next = NewNode;
    NewNode->key = map->copyKeyElement(keyElement);
    NewNode->data = map->copyDataElement(dataElement);
    if (!NewNode->key || !NewNode->data) {
        free(NewNode);
        return MAP_OUT_OF_MEMORY;
    }

    // map->iterator = NULL;
    return MAP_SUCCESS;
}


MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if(map==NULL || keyElement==NULL)
    {
        return NULL;
    }

    map->iterator=map->Head;

    while(map->iterator!=NULL)
    {
        if(map->compareKeyElements(map->iterator->key,keyElement)==0)
        {
            return map->iterator->data;
        }
        map->iterator=map->iterator->next;
    }
    return NULL;
}

static void deleteAllTheNodes(Map map)
{
    if(map==NULL || map->Head==NULL)
    {
        return;
    }
    map->iterator= map->Head;
    while(map->iterator != NULL)
    {
        Node tod = map->iterator->next;
        (map->freeDataElement)(map->iterator->data);
        (map->freeKeyElement)(map->iterator->key);
        free(map->iterator);
        map->iterator=tod;
    }
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if (!map || !keyElement) {
        return MAP_NULL_ARGUMENT;
    }
    Node ptrNodeBefore = map->Head;
    if (!ptrNodeBefore) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if (map->compareKeyElements(ptrNodeBefore->key, keyElement) == 0) {
        map->freeKeyElement(ptrNodeBefore->key);
        ptrNodeBefore->key = NULL;
        map->freeDataElement(ptrNodeBefore->data);
        ptrNodeBefore->data = NULL;
        map->Head = ptrNodeBefore->next;
        ptrNodeBefore->next = NULL;
        free(ptrNodeBefore);
        map->iterator = NULL;
        return MAP_SUCCESS;
    }
    while (ptrNodeBefore->next) {
        if (map->compareKeyElements(ptrNodeBefore->next->key, keyElement) == 0) {
            map->freeKeyElement(ptrNodeBefore->next->key);
            map->freeDataElement(ptrNodeBefore->next->data);
            Node temp = ptrNodeBefore->next;
            ptrNodeBefore->next = ptrNodeBefore->next->next;
            free(temp);
            // map->iterator = NULL;
            return MAP_SUCCESS;
        }
        ptrNodeBefore = ptrNodeBefore->next;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}


MapKeyElement mapGetFirst(Map map)
{
    if(map==NULL || map->Head==NULL)
    {
        return NULL;
    }

    map->iterator=map->Head;

    MapKeyElement result= (map->copyKeyElement)(map->iterator->key);
    return result;
}


MapKeyElement mapGetNext(Map map)
{
    if (map==NULL || map->iterator==NULL || map->iterator->next ==NULL)
    {
        return NULL;
    }

    map->iterator=(map->iterator)->next;

    MapKeyElement result=(map->copyKeyElement)(map->iterator->key);
    return result;
}

MapResult mapClear(Map map)
{
    if(map==NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    if(mapGetSize(map)==0)
    {
        map->Head=NULL;
        map->iterator=NULL;
        return MAP_SUCCESS;
    }
    deleteAllTheNodes(map);
    map->Head=NULL;
    map->iterator=NULL;
    return MAP_SUCCESS;
}






