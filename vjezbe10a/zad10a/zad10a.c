#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

struct Cities;
typedef struct Cities* citiesPosition;
typedef struct Cities
{
    char name[20];
    int residents;
    citiesPosition right;
    citiesPosition left;
}cities;

struct Countries;
typedef struct Countries* countriesPosition;
typedef struct Countries
{
    char name[20];
    citiesPosition city;
    countriesPosition next;
}countries;

int addCountry(countriesPosition p, char* name, char* dat);
int addCity(citiesPosition p, char* dat);
citiesPosition insertCity(citiesPosition p, char* name, int residents);
int printCountry(countriesPosition p);
int printCities(citiesPosition p);
countriesPosition findCountry(countriesPosition p, char* name);
int findCity(citiesPosition p, int number);

int main(void)
{
    countries countriesHead = { .name = "", .city = NULL, .next = NULL };
    FILE* file = NULL;
    char nameCountry[20];
    char datcity[20];
    countriesPosition p = NULL;
    int number = 0;
    citiesPosition h = NULL;

    file = fopen("countries.txt", "r");
    if (file == NULL)
    {
        perror("error while opening file\n");
        return -1;
    }
    while (!feof(file))
    {
        fscanf(file, "%s %s", nameCountry, datcity);
        addCountry(&countriesHead, nameCountry, datcity);
    }
    fclose(file);
    printCountry(countriesHead.next);

    printf("\nEnter the state you want to check: ");
    scanf("%s", nameCountry);
    p = findCountry(countriesHead.next, nameCountry);
    if (p == NULL)
    {
        printf("can't find that country\n");
        return 0;
    }
    printf("minimal number of residents: ");
    scanf("%d", &number);
    printf("\ncities from country (%s) with more than %d residents:\n", p->name, number);
    findCity(p->city, number);

    return 0;
}

int addCountry(countriesPosition countriesHead, char* name, char* dat)
{
    countriesPosition newElement = NULL;
    countriesPosition previous = NULL;
    citiesPosition newCity = NULL;
    newElement = (countriesPosition)malloc(sizeof(countries));
    if (newElement == NULL)
    {
        perror("couldn't allocate memory\n");
        return -1;
    }
    strcpy(newElement->name, name);
    newElement->city = newCity;

    newCity = (citiesPosition)malloc(sizeof(cities));
    if (newCity == NULL)
    {
        perror("couldn't allocate memory\n");
        return -1;
    }
    strcpy(newCity->name, "");
    newCity->residents = 0;
    newCity->right = NULL;
    newCity->left = NULL;
    newElement->city = newCity;
    addCity(newElement->city, dat);

    previous = countriesHead;
    while (countriesHead != NULL)
    {
        if (countriesHead->next == NULL)
        {
            countriesHead->next = newElement;
            newElement->next = NULL;
            break;
        }
        if (strcmp(newElement->name, previous->next->name) < 0)
        {
            newElement->next = previous->next;
            previous->next = newElement;
            break;
        }
        countriesHead = countriesHead->next;
    }
    return 0;
}

int addCity(citiesPosition newCity, char* dat)
{
    FILE* file = NULL;
    char name[20];
    int residents = 0;

    file = fopen(dat, "r");
    if (file == NULL)
    {
        perror("error while opening file\n");
        return -1;
    }

    while (!feof(file))
    {
        fscanf(file, "%s %d", name, &residents);
        newCity = insertCity(newCity, name, residents);
    }
    fclose(file);
    return 0;
}

citiesPosition insertCity(citiesPosition newCity, char* name, int residents)
{
    citiesPosition temp = NULL;
    if (newCity == NULL)
    {
        temp = (citiesPosition)malloc(sizeof(cities));
        if (temp == NULL)
        {
            perror("couldn't allocate memory\n");
            return NULL;
        }
        strcpy(temp->name, name);
        temp->residents = residents;
        temp->left = NULL;
        temp->right = NULL;
        return temp;
    }
    if (residents > newCity->residents)
        newCity->right = insertCity(newCity->right, name, residents);
    if (residents < newCity->residents)
        newCity->left = insertCity(newCity->left, name, residents);
    if (residents == newCity->residents)
    {
        if (strcmp(name, newCity->name) >= 0)
            newCity->right = insertCity(newCity->right, name, residents);
        else
            newCity->left = insertCity(newCity->left, name, residents);
    }

    return newCity;
}

int printCountry(countriesPosition countryHead)
{
    printf("countries list:\n");
    while (countryHead != NULL)
    {
        printf("\t%s\n", countryHead->name);
        printCities(countryHead->city->right);
        countryHead = countryHead->next;
    }
    return 0;
}

int printCities(citiesPosition city)
{
    if (city == NULL)
        return 0;
    printCities(city->left);
    printf("\t\t%s\n", city->name);
    printCities(city->right);
    return 0;
}

countriesPosition findCountry(countriesPosition countryHead, char* name)
{
    while (countryHead != NULL)
    {
        if (strcmp(countryHead->name, name) == 0)
            return countryHead;
        countryHead = countryHead->next;
    }
    return countryHead;
}

int findCity(citiesPosition city, int number)
{
    if (city == NULL)
        return 0;
    findCity(city->left, number);
    if (city->residents >= number)
        printf("\t%s %d\n", city->name, city->residents);
    findCity(city->right, number);
    return 0;
}













