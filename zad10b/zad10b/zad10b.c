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
    citiesPosition next;
}cities;

struct Country;
typedef struct Country* countryPosition;
typedef struct Country
{
    char name[20];
    citiesPosition city;
    countryPosition right;
    countryPosition left;
}country;

countryPosition addCountry(countryPosition countryRoot, char* countryName, char* datcity);
int addCities(citiesPosition cityRoot, char* datcity);
int printCountryInorder(countryPosition countryRoot);
int printCities(citiesPosition cityRoot);
int compareCities(citiesPosition cityRoot, citiesPosition secondCity);
int find(countryPosition countryRoot, char* countryName, int residents);
countryPosition findCountry(countryPosition countryRoot, char* countryName);

int main(void)
{
    countryPosition root = NULL;
    FILE* file = NULL;
    char countryName[20];
    char datcity[20];
    int residents = 0;

    file = fopen("countries.txt", "r");
    if (file == NULL)
    {
        perror("error while opening file\n");
        return 0;
    }

    while (!feof(file))
    {
        fscanf(file, "%s %s", countryName, datcity);
        root = addCountry(root, countryName, datcity);
    }
    fclose(file);
    printCountryInorder(root);

    printf("Enter the country that you want to check : ");
    scanf("%s", countryName);
    printf("Minimal number of residents by city: ");
    scanf("%d", &residents);
    find(root, countryName, residents);

    return 0;
}

int printCountryInorder(countryPosition countryRoot)
{
    if (countryRoot == NULL)
        return 0;
    printCountryInorder(countryRoot->left);
    printf("%s\n", countryRoot->name);
    printCities(countryRoot->city->next);
    printCountryInorder(countryRoot->right);
    return 0;
}

int printCities(citiesPosition cityRoot)
{
    while (cityRoot != NULL)
    {
        printf("\t%s %d\n", cityRoot->name, cityRoot->residents);
        cityRoot = cityRoot->next;
    }
    return 0;
}

countryPosition addCountry(countryPosition countryRoot, char* countryName, char* datcity)
{
    countryPosition secondCity = NULL;
    citiesPosition h = NULL;
    if (countryRoot == NULL)
    {
        secondCity = (countryPosition)malloc(sizeof(country));
        if (secondCity == NULL)
        {
            perror("couldn't allocate memory\n");
            return NULL;
        }
        strcpy(secondCity->name, countryName);

        h = (citiesPosition)malloc(sizeof(cities));
        if (secondCity == NULL)
        {
            perror("couldn't allocate memory\n");
            return NULL;
        }
        strcpy(h->name, "");
        h->residents = 0;
        h->next = NULL;
        secondCity->city = h;

        addCities(secondCity->city, datcity);
        secondCity->left = NULL;
        secondCity->right = NULL;

        return secondCity;
    }
    else if (strcmp(countryRoot->name, countryName) > 0)
        countryRoot->left = addCountry(countryRoot->left, countryName, datcity);
    else if (strcmp(countryRoot->name, countryName) < 0)
        countryRoot->right = addCountry(countryRoot->right, countryName, datcity);
    return countryRoot;
}

int addCities(citiesPosition cityRoot, char* datcity)
{
    citiesPosition secondCity = NULL;
    FILE* file = NULL;
    char name[20];
    int residents = 0;
    citiesPosition prev = cityRoot;
    citiesPosition begin = cityRoot;

    file = fopen(datcity, "r");
    if (file == NULL)
    {
        perror("error while opening file\n");
        return -1;
    }

    while (!feof(file))
    {
        prev = begin;
        cityRoot = begin;
        fscanf(file, "%s %d", name, &residents);
        secondCity = (citiesPosition)malloc(sizeof(cities));
        if (secondCity == NULL)
        {
            perror("couldn't allocate memory\n");
            return -1;
        }
        strcpy(secondCity->name, name);
        secondCity->residents = residents;
        while (cityRoot != NULL)
        {
            if (cityRoot->next == NULL)
            {
                secondCity->next = cityRoot->next;
                cityRoot->next = secondCity;
                break;
            }
            if (compareCities(secondCity, prev->next) == 1)
            {
                secondCity->next = prev->next;
                prev->next = secondCity;
                break;
            }
            prev = cityRoot;
            cityRoot = cityRoot->next;
        }
    }
    fclose(file);
    return 0;
}

int compareCities(citiesPosition cityRoot, citiesPosition secondCity)
{
    if (cityRoot->residents > secondCity->residents)
        return 1;
    else if (cityRoot->residents < secondCity->residents)
        return -1;
    else
    {
        if (strcmp(cityRoot->name, secondCity->name) > 0)
            return 1;
        else if (strcmp(cityRoot->name, secondCity->name) < 0)
            return -1;
    }
    return 0;
}

int find(countryPosition countryRoot, char* countryName, int residents)
{
    countryPosition drzava = NULL;
    citiesPosition temp = NULL;
    int x = 0;

    drzava = findCountry(countryRoot, countryName);
    if (drzava == NULL)
    {
        printf("that country doesn't exist\n");
        return 0;
    }
    temp = drzava->city;

    if (temp == NULL)
    {
        printf("that country doesn't have any city\n");
        return 0;
    }
    temp = temp->next;
    while (temp != NULL)
    {
        if (temp->residents >= residents)
        {
            printf("\t%s\n", temp->name);
            x = 1;
        }
        else
            break;
        temp = temp->next;
    }
    if (x == 0)
        printf("that kind of city doens't exist\n");

    return 0;
}

countryPosition findCountry(countryPosition countryRoot, char* countryName)
{
    if (countryRoot == NULL)
        return NULL;
    if (strcmp(countryName, countryRoot->name) == 0)
        return countryRoot;
    if (strcmp(countryName, countryRoot->name) < 0)
        return findCountry(countryRoot->left, countryName);
    if (strcmp(countryName, countryRoot->name) > 0)
        return findCountry(countryRoot->right, countryName);
}




