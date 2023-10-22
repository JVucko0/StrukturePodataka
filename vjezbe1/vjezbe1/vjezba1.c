#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*
Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100*/


#define MAX_SIZE (128)
#define MAX_LINE (1024)
#define MAX_POINTS (15)

typedef struct _student
{
	char ime[MAX_SIZE];
	char prezime[MAX_SIZE];
	double bodovi;
} student;

int RowsInFile() 
{
	int counter = 0;
	FILE* FilePointer = NULL;
	char buffer[MAX_SIZE] = { 0 };

	FilePointer = fopen("studenti.txt", "r");

	if (!FilePointer) {
		printf("Pogreška u otvaranju datoteke!");
		return -1;
	}
	
	while (!feof(FilePointer)) {
		fgets(buffer, MAX_SIZE, FilePointer);
		counter++;
	}
	
	fclose(FilePointer);
	
	return counter;
}

student* ReadStudents(int noRows) 
{
	student* students = NULL;
	FILE* FilePointer = NULL;
	int counter = 0;
	
	students = (student*)malloc(noRows * sizeof(student));
	if (!students)
	{
		printf("Neuspješna alokacija memorije\n");
		return NULL;
	}
	
	FilePointer = fopen("studenti.txt", "r");
	if (FilePointer == NULL) 
	{
		printf("Greška u otvaranju datoteke!\n");
		return NULL;
	}
	while (!feof(FilePointer)) 
	{
		fscanf(FilePointer, "%s %s %lf", students[counter].ime, students[counter].prezime, &students[counter].bodovi);
		counter++;
	}
	return students;

}

double RelativePoints(double Bodovi)
{
	return ((Bodovi / MAX_POINTS) * 100);
}

int PrintStudents(int noRows, student* students) 
{
	int counter = 0;

	for (counter; counter < (noRows - 1); counter++) 
	{
		printf("Ime: %s Prezime: %s Bodovi: %.2lf Relativni bodovi: %.2lf %%\n", students[counter].ime,
			students[counter].prezime, students[counter].bodovi, RelativePoints(students[counter].bodovi) );
	}
	
	return 0;
}



int main()
{
	student* students = NULL;
	int noRows = 0;

	noRows = RowsInFile();
	

	if (noRows > 0) 
	{
		students = ReadStudents(noRows);
		PrintStudents(noRows, students);
		free(students);
	}

	return 0;
}