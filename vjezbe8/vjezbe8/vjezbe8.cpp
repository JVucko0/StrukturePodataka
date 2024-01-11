#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Napisati program koji omogućava rad s binarnim stablom pretraživanja. Treba
omogućiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.
*/

struct _binaryTree;
typedef struct _binaryTree* Position;
typedef struct _binaryTree {
	int el;
	Position left;
	Position right;
} BinaryTree;

Position insert(int el, Position root);
Position search(int el, Position root);
Position deleteEl(int el, Position root);
int inorder(Position root);
int preorder(Position root);
int postorder(Position root);
int levelorder(Position root);

int main() {
	Position root = NULL;
	int el = 0;

	printf("Unesi vrijednost prvog clana: ");
	scanf("%d", &el);

	root = insert(el, root);

	int command = 1;

	do {
		
		printf("\nMenu\n");
		printf("1-Unesi element u stablo\n");
		printf("2-Brisanje elementa iz stabla\n");
		printf("3-Pronalazak elementa iz stabla\n");
		printf("4-Inorder ispis\n");
		printf("5-Preorder ispis\n");
		printf("6-Postorder ispis\n");
		printf("7-Level order ispis\n");
		printf("8-izlaz\n");
		printf("-----------------\n");
		printf("Izaberi: ");
		scanf("%d", &command);

		switch (command) {
		case 1:
			printf("Unesi vrijednost elementa: ");
			scanf("%d", &el);
			insert(el, root);
			break;
		case 2:
			printf("Unesi element koji brišete: ");
			scanf("%d", &el);
			deleteEl(el, root);
			break;
		case 3:
		{
			printf("Unesi element koji tražiš: ");
			scanf("%d", &el);
			Position result = search(el, root);
			if (!result) {
				printf("Nema elementa u stablu.\n");
			}
			else {
				printf("Element je u stablu.\n");
			}
			break;
		}
		case 4:
			inorder(root);
			break;
		case 5:
			preorder(root);
			break;
		case 6:
			postorder(root);
			break;
		case 7:
			levelorder(root);
			break;
		case 8:
			printf("Izlaz\n");
			break;
		default:
			printf("Krivo");
		}
		

	} while (command != 8);

	return 0;
}



Position insert(int el, Position root) {
	if (root == NULL) {
		Position NewElement = (Position)malloc(sizeof(BinaryTree));
		if (!NewElement) {
			printf("Error in memory allocation.\n");
			return NULL;
		}
		else {
			NewElement->el = el;
			NewElement->left = NULL;
			NewElement->right = NULL;
		}
		root = NewElement;
	}
	else if (el > root->el) {
		root->right = insert(el, root->right);
	}
	else if (el < root->el) {
		root->left = insert(el, root->left);
	}
	return root;
}

Position search(int el, Position root) {
	if (root == NULL) 
		return NULL;
	else if (el > root->el) {
		root->right = (el, root->right);
	}
	else if (el < root->el) {
		root->left = (el, root->left);
	}
	else {
		return root;
	}
}

Position deleteEl(int el, Position root) {
	if (root == NULL) {
		printf("Nema ga");
	}

	// Pronalazak cvora koji se treba obrisati
	else if (el < root->el) {
		root->left = deleteEl(el, root->left);
	}
	else if (el > root->el) {
		root->right = deleteEl(el, root->right);
	}
	//pronaden element za brisanje

	//ako element koji se brise ima livo i desno dite
	else if (root->left != NULL && root->right != NULL) {
		//pronalazak najmanjeg djeteta na desnoj strani(moze i najveceg na livoj strani)
		Position temp = root->right;
		while (temp->left != NULL) {
			temp = temp->left;
		}
		//element za brisanje se zamjenjuje sa pronadenim najmanjim elementom
		root->el = temp->el;
		//ne zamjenjuje se zapravo element nego vrijednost najmanjeg se stavlja na element kojeg se tribalo brisat

		//brisanje
		root->right = deleteEl(root->el, root->right);
	}
	//ako element za brisanje nema oba diteta
	else {
		Position temp = root;
		if (root->left == NULL) {
			root = root->right;
		}
		else {
			root = root->left;
		}
		free(temp);
	}

	return root;
}


int inorder(Position root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%d ", root->el);
		inorder(root->right);
	}
	return 0;
}


int preorder(Position root) {
	if (root != NULL) {
		printf("%d ", root->el);
		preorder(root->left);
		preorder(root->right);
	}
	return 0;
}


int postorder(Position root) {
	if (root != NULL) {
		postorder(root->left);
		postorder(root->right);
		printf("%d ", root->el);
	}
	return 0;
}



int levelorder(Position root) {
	if (root == NULL) {
		return 0;
	}

	Position element[20]; //ukupan broj elemenata u stablu
	int front = 0, rear = 0;

	element[rear++] = root;

	while (front < rear) {
		Position current = element[front++];

		printf("%d ", current->el);

		if (current->left != NULL) {
			element[rear++] = current->left;
		}

		if (current->right != NULL) {
			element[rear++] = current->right;
		}
	}
	return 0;
}