#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
7. Napisati program koji pomoću vezanih listi (stabala) predstavlja strukturu direktorija.
Omogućiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Točnije program treba preko menija simulirati
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz
*/

struct _directory;
typedef struct _directory* PositionDirectory;
typedef struct _directory {
	char name[50];
	PositionDirectory subDirectories;
	PositionDirectory next;
	PositionDirectory parentDirectory; 
} Directory;

PositionDirectory createDirectory(char* name);
int createSubdirectory(char* name, PositionDirectory currentDirectory);
PositionDirectory changeDirectory(char* name, PositionDirectory currentDirectory);
PositionDirectory exitDirectory(PositionDirectory currentDirectory);
int listDirectories(PositionDirectory currentDirectory);

int main() {
	Directory headDirectory = {
		.name = {0},
		.subDirectories = NULL,
		.next = NULL
	};

	PositionDirectory rootDirectory = createDirectory("C:");
	headDirectory.next = rootDirectory;

	PositionDirectory currentDirectory = rootDirectory;

	int choice = 0;

	while (1) {
		printf("\nOdaberite opciju: \n");
		printf("1 - make directory\n");
		printf("2 - change directory\n");
		printf("3 - exit directory\n");
		printf("4 - list directories\n");
		printf("5 - exit\n");
		scanf("%d", &choice);

		char directoryName[50] = { 0 };

		switch (choice) {
		case 1:
			printf("Enter directory name: ");
			scanf(" %s", directoryName);
			createSubdirectory(directoryName, currentDirectory);
			break;
		case 2:
			printf("Enter directory name: ");
			scanf(" %s", directoryName);
			currentDirectory = changeDirectory(directoryName, currentDirectory);
			break;
		case 3:
			currentDirectory = exitDirectory(currentDirectory);
			break;
		case 4:
			listDirectories(currentDirectory);
			break;
		case 5:
			break;
		default:
			printf("Undefined charachter.\n");
			break;
		}
		if (choice == 5)
			break;
	}
	free(rootDirectory);

	return 0;

}


PositionDirectory createDirectory(char* name) {
	PositionDirectory newDir = (PositionDirectory)malloc(sizeof(Directory));

	if (!newDir) {
		printf("Error in memory allocation\n");
		return NULL;
	}

	strcpy(newDir->name, name);
	newDir->next = NULL;
	newDir->subDirectories = NULL;
	newDir->parentDirectory = NULL;
	return newDir;
}

int createSubdirectory(char* name, PositionDirectory currentDirectory) {
	PositionDirectory newDir = createDirectory(name);

	if (!newDir) {
		printf("Error in memory allocation\n");
		return 1;
	}

	newDir->next = currentDirectory->subDirectories;
	newDir->parentDirectory = currentDirectory;
	currentDirectory->subDirectories = newDir;
	return 0;
}

PositionDirectory changeDirectory(char* name, PositionDirectory currentDirectory) {
	PositionDirectory current = currentDirectory->subDirectories;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			return current;
		}
		current = current->next;
	}
	
	printf("Nepostoji takav directory.\n");
	return currentDirectory;
}

PositionDirectory exitDirectory(PositionDirectory currentDirectory) {
	if(currentDirectory->parentDirectory != NULL)
		currentDirectory = currentDirectory->parentDirectory;
	else 
		printf("Error in directory construction.\n");

	return currentDirectory;
}

int listDirectories(PositionDirectory currentDirectory) {
	PositionDirectory current = currentDirectory->subDirectories;
	
	if (current == NULL) {
		printf("Directory has no subdirectories.\n");
		return 0;
	}
	while (current != NULL) {
		printf("%s ", current->name);
		current = current->next;
	}
	return 0;
}