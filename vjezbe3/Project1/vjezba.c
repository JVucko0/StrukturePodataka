/*
3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. upisuje listu u datoteku,
D. čita listu iz datoteke.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

typedef struct _Person Person;
typedef Person* Position;
struct _Person {
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthYear;
	Position next;
};

int append(Position P);
Position  createPerson();
int addToTheEnd(Position P);
Position findLast(Position P);
int PrintList(Position P);
int Find(Position P);
char* wishedSurname();
int PrintPerson(Position P);
int DeletePerson(Position P);
int menu(Position P);
int AddingAfterElement(Position P);
int AddingBeforeElement(Position P);
Position FindPosition(Position P);
Position FindPositionBefore(Position P);
int WriteInFile(Position P);

int main() {
	Person Head = { .next = NULL,.name = {0},.surname = {0},.birthYear = 0 };

	menu(&Head);

	return 0;
}

int menu(Position P) {
	char choice;


	while (1) {

		printf("A (append at beggining), E (add at the end), F (find person), P (Print list), D (Delete person), Add after specific person (G), Add before specific element (H), Write in File (W), X (Exit)\n");
		scanf(" %c", &choice);

		if (choice == 'A') {
			append(P);
			continue;
		}
		else if (choice == 'E') {
			addToTheEnd(P);
			continue;
		}
		else if (choice == 'F') {
			Find(P);
			continue;
		}
		else if (choice == 'P') {
			PrintList(P->next);
			continue;
		}
		else if (choice == 'D') {
			DeletePerson(P);
			continue;
		}
		else if (choice == 'G') {
			AddingAfterElement(P);
			continue;
		}
		else if (choice == 'H') {
			AddingBeforeElement(P);
			continue;
		}
		else if (choice == 'X') {
			break;
		}
		else if (choice == 'W') {
			WriteInFile(P->next);
			continue;
		}
		else {
			printf("Wrong input.");
			continue;
		}

	}
	return 0;
}

// dodavanje novog elementa na početak liste
int append(Position P) {
	Position newPerson;

	//stvaranje nove osobe
	newPerson = createPerson();

	//ako osoba nije uspjesno stvorena nece se izvrsit radnja
	if (newPerson) {
		newPerson->next = P->next;
		P->next = newPerson;
	}

	return 0;
}

Position createPerson() {
	Position newPerson;
	char name[MAX_LENGTH] = { 0 };
	char surname[MAX_LENGTH] = { 0 };
	int birthYear = 0;

	newPerson = (Position)malloc(sizeof(Person));
	if (!newPerson) {
		printf("Neuspješna alokacija memorije\n");
		return NULL;
	}
	printf("Unesi ime: ");
	scanf(" %s", name);
	printf("Unesi prezime: ");
	scanf(" %s", surname);
	printf("Unesi godinu rodenja: ");
	scanf("%d", &birthYear);

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	return newPerson;
}

int addToTheEnd(Position P) {
	Position newPerson;

	newPerson = createPerson();

	//Ako se memorija krivo alocira, funkcija ce vratit NULL u newPerson
	if (newPerson) {
		P = findLast(P);
		newPerson->next = P->next;
		P->next = newPerson;
	}

	return 0;
}

Position findLast(Position P) {
	while (P->next != NULL) {
		P = P->next;
	}
	return P;
}

int PrintList(Position P) {
	//U funkciju saljemo head.next zato provjeravamo je li NULL tj. je li prazna lista
	if (!P) {
		printf("Nema elemenata u listi");
		return NULL;
	}
	while (P != NULL) {
		printf("Name: %s\tSurname: %s\tBirthYear %d\t\n", P->name, P->surname, P->birthYear);
		P = P->next;
	}
	return 0;
}

int Find(Position P) {
	char surname[50] = { 0 };
	strcpy(surname, wishedSurname());
	if (P) {
		//da bi se petlja zavrsila p->next treba biti NULL ili razlicit od NULL ali da je prezime clana jednako trazenom prezimenu
		while (P->next && strcmp(P->next->surname, surname) != 0) {
			P = P->next;
		}
		if (P->next) {
			PrintPerson(P->next);
		}
		else {
			printf("Can't find the person.");
			return -1;
		}

	}
	else {
		printf("empty list");
	}
	return 0;
}
Position FindPosition(Position P) {
	char surname[50] = { 0 };
	strcpy(surname, wishedSurname());
	if (P) {
		//da bi se petlja zavrsila p->next treba biti NULL ili razlicit od NULL ali da je prezime clana jednako trazenom prezimenu
		while (P->next && strcmp(P->next->surname, surname) != 0) {
			P = P->next;
		}
		if (P->next) {
			return P->next;
		}
		else {
			printf("Can't find the person.");
			return NULL;
		}

	}
	else {
		printf("empty list");
	}
	return NULL;
}
Position FindPositionBefore(Position P) {
	char surname[50] = { 0 };
	strcpy(surname, wishedSurname());
	if (P) {
		//da bi se petlja zavrsila p->next treba biti NULL ili razlicit od NULL ali da je prezime clana jednako trazenom prezimenu
		while (P->next && strcmp(P->next->surname, surname) != 0) {
			P = P->next;
		}
		if (P->next) {
			return P;
		}
		else {
			printf("Can't find the person.");
			return NULL;
		}

	}
	else {
		printf("empty list");
	}
	return NULL;
}
char* wishedSurname() {
	char surname[50] = { 0 };
	printf("Type wished surname: ");
	scanf(" %s", surname);

	return surname;
}

int PrintPerson(Position P) {
	printf("Name: %s\tSurname: %s\tBirthYear %d\t\n", P->name, P->surname, P->birthYear);
	return 0;
}

int DeletePerson(Position P) {
	char surname[50] = { 0 };
	strcpy(surname, wishedSurname());
	//ako je p->next NULL ne izvrsavaju se naredbe
	if (P->next) {
		Position previous = NULL;
		while (P->next && strcmp(P->surname, surname) != 0) {
			previous = P;
			P = P->next;
		}
		if (strcmp(P->surname, surname) == 0) {
			previous->next = P->next;
			free(P);
		}
		else {
			perror("Can't find person.\n");
			return -1;
		}
	}
	else {
		printf("empty list\n");
		return -1;
	}
	return 0;
}

int AddingAfterElement(Position P) {
	Position prev = NULL;
	Position pers = NULL;
	prev = FindPosition(P);
	if (prev) {
		pers = createPerson();
		if (pers) {
			pers->next = prev->next;
			prev->next = pers;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
	return 0;
}

int AddingBeforeElement(Position P) {
	Position prev = NULL;
	Position pers = NULL;
	prev = FindPositionBefore(P);
	if (prev) {
		pers = createPerson();
		if (pers) {
			pers->next = prev->next;
			prev->next = pers;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
	return 0;
}


int WriteInFile(Position P) {
	FILE* filepointer = NULL;

	filepointer = fopen("names.txt", "w");
	if (!filepointer) {
		printf("opening file error");
		return NULL;
	}
	while (P != NULL) {
		fprintf(filepointer, "%s\t%s\t%d\n", P->name,P->surname,P->birthYear);
		P = P->next;
	}
	fclose(filepointer);
	return 0;
}