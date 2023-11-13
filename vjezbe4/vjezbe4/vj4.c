/*
Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE (1024)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	int coefficent;
	int exponent;
	Position next;
}Element;

int readFromFile(Position pol1, Position pol2);
int stringToList(char* buffer, Position pol);
int addSorted(Position pol, int coef, int exp);
int printList(Position pol);
int addition(Position pol1, Position pol2, Position resultA);
int multiply(Position pol1, Position pol2, Position resultM);

int main(){
	Element polinom1 = { .coefficent = 0 ,.exponent = 0,.next = NULL };
	Element polinom2 = { .coefficent = 0 ,.exponent = 0,.next = NULL };
	Element resultA = { .coefficent = 0 ,.exponent = 0,.next = NULL };
	Element resultM = { .coefficent = 0 ,.exponent = 0,.next = NULL };
	readFromFile(&polinom1, &polinom2);

	printf("First Polinom: \n");
	printList(&polinom1);
	printf("\nSecond Polinom: \n");
	printList(&polinom2);

	addition(polinom1.next, polinom2.next, &resultA);
	multiply(polinom1.next, polinom2.next, &resultM);
	printf("\nAddition: \n");
	printList(&resultA);
	printf("\nMultiplying: \n");
	printList(&resultM);

	return 0;
}

int readFromFile(Position pol1, Position pol2) {
	FILE *filePointer = NULL;

	filePointer = fopen("polinomi.txt", "r");

	if (!filePointer) {
		perror("Error in file opening");
		return 0;
	}

	char buffer[MAX_SIZE] = { 0 };

	fgets(buffer, MAX_SIZE, filePointer);
	stringToList(buffer, pol1);

	fgets(buffer, MAX_SIZE, filePointer);
	stringToList(buffer, pol2);

	fclose(filePointer);

	return 0;
}

int stringToList(char* buffer, Position pol) {
	char* currentBuffer = buffer;
	int numBytes, coef, expo;

	while (strlen(currentBuffer) > 0) {
		sscanf(currentBuffer, "%dx^%d %n", &coef, &expo, &numBytes);
		addSorted(pol, coef, expo);
		currentBuffer += numBytes;
	}
	return 0;
}

int addSorted(Position pol, int coef, int exp) {
	Position temp = pol->next;
	Position previous = pol;
	Position new_element = (Position)malloc(sizeof(Element));

	new_element->coefficent = coef;
	new_element->exponent = exp;

	while (temp != NULL) {
		if (new_element->exponent > temp->exponent) {
			new_element->next = previous->next;
			previous->next = new_element;
			break;
		}
		else if (new_element->exponent == temp->exponent) {
			temp->coefficent = temp->coefficent + new_element->coefficent;
			free(new_element);
			break;
		}
		temp = temp->next;
		previous = previous->next;
	}
	if (temp == NULL) {
		new_element->next = previous->next;
		previous->next = new_element;
	}
	return 0;
}

int printList(Position head) {
	Position temp = head;

	while (temp->next != NULL) {
		temp = temp->next;
		printf("%dx^%d ", temp->coefficent, temp->exponent);
	}
	return 0;
}

int addition(Position pol1, Position pol2, Position result) {
	Position remainingPol = NULL;
	Position newElement = NULL;

	while (pol1 != NULL && pol2 != NULL) {
		if (pol1->exponent == pol2->next) {
			addSorted(result, (pol1->coefficent+ pol2->coefficent), pol1->exponent);
			pol1 = pol1->next;
			pol2 = pol2->next;
		}
		else if (pol1->exponent < pol2->exponent) {
			addSorted(result, pol2->coefficent, pol2->exponent);
			pol2 = pol2->next;
		}
		else {
			addSorted(result, pol1->coefficent, pol1->exponent);
			pol1 = pol1->next;
		}
	}
	if (pol1 == NULL) {
		remainingPol = pol2;
	}
	else {
		remainingPol = pol1;
	}
	while (remainingPol != NULL) {
		addSorted(result, remainingPol->coefficent, remainingPol->exponent);
		remainingPol = remainingPol->next;
	}
	return 0;
}

int multiply(Position pol1, Position pol2, Position result) {
	Position currentPol2 = pol2;
	while (pol1 != NULL) {
		while (pol2 != NULL) {
			addSorted(result, (pol1->coefficent * pol2->coefficent), pol1->exponent + pol2->exponent);
			pol2 = pol2->next;
		}
		pol2 = currentPol2;
		pol1 = pol1->next;
	}
}