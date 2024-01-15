//10. Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država.Uz
//ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
//države.Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
//broj_stanovnika.
//a) Potrebno je formirati sortiranu vezanu listu država po nazivu.Svaki čvor vezane liste
//sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
//b) Potrebno je formirati stablo država sortirano po nazivu.Svaki čvor stabla sadrži vezanu
//listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

// a)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _binaryTreeCity;
typedef struct _binaryTreeCity* PositionCity;
typedef struct _binaryTreeCity {
    char name[50];
    int numCitizens;
    PositionCity left;
    PositionCity right;
} City;

struct _listCountry;
typedef struct _listCountry* PositionCountry;
typedef struct _listCountry {
    char name[50];
    PositionCountry next;
    PositionCity root; //ovo ce vezat binarno stablo grada na drzavu
} Country;

int readCountryNames(PositionCountry Head);
PositionCountry createCountry(char* name);
PositionCountry intsertCountry(PositionCountry Head, char* countryName);
PositionCity readCities(char* fileName);
PositionCity insert(PositionCity root, int numOfCitizens, char* city);
int print(PositionCountry Head);
int inorder(PositionCity root);

int main() {

    Country Head = { .name = "", .next = NULL, .root = NULL };

    readCountryNames(&Head);
    print(&Head);

    //inorder(Head.next->next->root); zg st

    return 0;
}

int readCountryNames(PositionCountry Head) {
    FILE* filePointer = NULL;
    filePointer = fopen("drzave.txt", "r");
    if (!filePointer) {
        printf("Nije moguce otvoriti datoteku\n");
        return -1;
    }

    while (!feof(filePointer)) {
        char countryName[50] = "", fileName[50] = "";
        fscanf(filePointer, " %s %s", countryName, fileName);
        //printf("%s %s\n", countryName, fileName); provjera
        PositionCountry country = intsertCountry(Head, countryName);
        PositionCity root = readCities(fileName);
        country->root = root;
    }
    fclose(filePointer);
    return 0;
}

PositionCountry createCountry(char* name) {
    PositionCountry newElement = NULL;
    newElement = (PositionCountry)malloc(sizeof(Country));
    if (newElement == NULL) {
        printf("Krivo alocirana memorija\n");
        return 0;
    }
    strcpy(newElement->name, name);
    newElement->next = NULL;
    newElement->root = NULL;
    return newElement;
}

PositionCountry intsertCountry(PositionCountry Head, char* countryName) {
    PositionCountry element = NULL;
    element = createCountry(countryName);
    PositionCountry current = Head;

    while (current->next != NULL && strcmp(countryName, current->next->name) > 0) {
        current = current->next;
    }

    element->next = current->next;
    current->next = element;
    return element;
}

PositionCity readCities(char* fileName) {
    FILE* filePointer = NULL;
    filePointer = fopen(fileName, "r");
    if (!filePointer) {
        printf("Nije moguce otvoriti datoteku\n");
        return -1;
    }

    char city[50] = "";
    int numOfCitizens = 0;
    //stvaranje root elementa za stablo gradova
    PositionCity root = NULL;
    fscanf(filePointer, " %s %d", city, &numOfCitizens);
    //printf(" %s %d", city, numOfCitizens);
    root = insert(root, numOfCitizens, city);
    
    while (!feof(filePointer)) {
        char city[50] = "";
        int numOfCitizens = 0;
        fscanf(filePointer, " %s %d", city, &numOfCitizens);
        //printf("\n %s %d i pozvana funkcija TEST\n", city, numOfCitizens);
        insert(root, numOfCitizens, city);
    }
    
    fclose(filePointer);
    return root;
}

PositionCity insert(PositionCity root, int numOfCitizens, char* city) {
    if (root == NULL) {
        PositionCity newElement = (PositionCity)malloc(sizeof(City));
        if (newElement == NULL) {
            printf("Kivo alocirana memorija\n");
            return -1;
        }
        else {
            strcpy(newElement->name, city);
            newElement->left = NULL;
            newElement->right = NULL;
            newElement->numCitizens = numOfCitizens;
        }
        root = newElement;
    }
    else if (numOfCitizens < root->numCitizens) {
        root->left = insert(root->left, numOfCitizens, city);
    }
    else if (numOfCitizens > root->numCitizens) {
        root->right = insert(root->right, numOfCitizens, city);
    }
    return root;
}

int print(PositionCountry Head) {
    PositionCountry temp = Head->next;

    while (temp != NULL) {
        printf("\n%s\n", temp->name);
        inorder(temp->root);
        temp = temp->next;
    }
    return 0;
}

int inorder(PositionCity root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%s ", &root->name);
        inorder(root->right);
    }
    return 0;
}