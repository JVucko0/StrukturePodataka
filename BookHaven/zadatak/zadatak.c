#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Zadatak za oslobađanje praktičnog dijela ispita:

"BookHaven" je knjižnica koja želi unaprijediti svoj sustav praćenja knjiga, autora i korisnika.
Svaka knjiga ima informacije o naslovu, autoru, godini izdanja i dostupnim primjercima.
Korisnici mogu posuđivati knjige i vratiti ih nakon nekog vremena.

Program mora korisniku omogućiti:
	a) ispis svih knjiga abecedno (tako da se mogu vidjeti svi podatci o knjizi i korisnici koji trenutno posuđuju knjigu) 
	b) ispis svih korisnika abecedno (tako da se mogu vidjeti sve knjige koje je neki korisnik posudio)
	c) pretraživanje knjiga po godini izdanja (tako da se mogu vidjeti sve knjige iz te godine i njihova količina)
	d) pretraživanje knjiga po nazivu autora (tako da se mogu vidjeti sve knjige tog autora i njihova količina)
	e) unos novog korisnika
	f) posudba knjige korisniku
		- može više primjeraka iste knjige posuditi od puta
		- korisnik u sebe nikad ne smije imati više od 5 knjiga sve skupa
		- ako pokušava napravit posudbu koja će ga dovesti preko tog broja program treba upozoriti korisnika na to i ispisati mu broj koliko još knjiga posudit
		- ako korisnik pita za više primjeraka knjige, a knjižnica na stanju nema nema dovoljno treba korisniku omogućiti izbor hoće li posudit to što ima ili ne.
	g) povratak knjiga na stanje
	h) spremi (u datoteku/datoteke spremiti trenutno stanje u knjižnici, tako da kad se idući put pokrene program moguće nastaviti od spremljenog stanja)

Sami odaberite strukturu podataka i format datoteka koju će te koristiti za ovo izvesti.
*/

#define MAX_LENGHT (100)
#define MAX_BOOKS (5)
#define MEMORY_NOT_ALLOCATED (-1)
#define NOT_FOUND (1)

typedef struct book Books;
typedef Books* PositionBook;
struct book {
	char title[MAX_LENGHT];
	char author[MAX_LENGHT];
	int year;
	int available;
	PositionBook next;
};

typedef struct user Users;
typedef Users* PositionUser;
struct user {
	char name[MAX_LENGHT];
	int numberOFBooks;
	PositionBook books[MAX_BOOKS];
	PositionUser next;
};

int inputBookSorted(PositionBook head, PositionBook current);
PositionBook createBook(char* author, char* title, int year, int available);
int printBooksByAuthor(PositionBook head);
int searchByAuthor(PositionBook head, char* name);
int searchByYear(PositionBook head, int wishedYear);
PositionUser inputUserSorted(PositionUser head, char* name);
PositionUser createUser(char* name);
int menu(PositionBook BookHead, PositionUser UserHead);
int takeBook(PositionBook BookHead, PositionUser UserHead);
PositionBook searchBook(PositionBook head, char* book);
PositionUser searchUser(PositionUser head, char* user);
int returnBook(PositionBook BookHead, PositionUser UserHead);
int saveBooksInFile(PositionBook BookHead);
int importBooksFromFile(PositionBook BookHead);
int saveUsersInFile(PositionUser UserHead);
int importUsersFromFile(PositionUser UserHead, PositionBook BookHead);

int main() {
	Books BookHead = { .title = {0},.author = {0},.year = 0,.available = 0,.next = NULL };
	Users UserHead = { .name = {0}, .numberOFBooks = 0, .books = {NULL},.next = NULL};
	
	menu(&BookHead, &UserHead);

	return 0;
}

int menu(PositionBook BookHead, PositionUser UserHead) {
	int command = 1, year = 0;
	char  author[MAX_LENGHT] = { 0 }, name[MAX_LENGHT] = { 0 };

	printf("Welcome to BookHaven!\n");

	do{
		printf("\n1- Create new user\n");
		printf("2- Print users\n");
		printf("3- Create new book\n");
		printf("4- Taking books\n");
		printf("5- Returning books\n");
		printf("6- Print all books in alphabetical order\n");
		printf("7- Search book by author\n");
		printf("8- Search book by year\n");
		printf("9- Save book and user infos in a file\n");
		printf("99- Take book and user infos from file\n");
		printf("0- Exit\n");
		printf("-----------------------------------------\n");
		printf("Choose: ");
		scanf("%d", &command);

		switch (command) {
		case 1:
			printf("Input name of a User: ");
			scanf(" %s", name);
			inputUserSorted(UserHead, name);
			break;
		case 2:
			printUsers(UserHead->next);
			break;
		case 3:
			inputBookSorted(BookHead, NULL);
			break;
		case 4:
			takeBook(BookHead, UserHead);
			break;
		case 5:
			returnBook(BookHead, UserHead);
			break;
		case 6:
			printBooksByAuthor(BookHead->next);
			break;
		case 7:
			printf("Type author name: ");
			scanf(" %s", &author);
			searchByAuthor(BookHead, author);
			break;
		case 8:
			printf("Type year: ");
			scanf("%d", &year);
			searchByYear(BookHead, year);
			break;
		case 9:
			saveBooksInFile(BookHead->next);
			saveUsersInFile(UserHead->next);
			break;
		case 99:
			importBooksFromFile(BookHead);
			importUsersFromFile(UserHead, BookHead);
			break;
		case 0:
			printf("Exit.");
			break;
		default:
			printf("Invalid.");
			break;
		}
	} while (command != 0);
}

int inputBookSorted(PositionBook head, PositionBook current) {
	PositionBook newBook = current;
	PositionBook temp = head->next;
	PositionBook prev = head;
	char title[MAX_LENGHT] = { 0 }, author[MAX_LENGHT] = { 0 };
	int year = 0, available= 0;
	//ako nije poslana neka vec postojeca knjiga, pitaj korisnika koju knjigu zeli unijet u knjiznicu
	if (newBook == NULL) {
		printf("Input title of a Book: ");
		scanf(" %s", title);
		printf("Input author name: ");
		scanf(" %s", author);
		printf("Input publishing year: ");
		scanf("%d", &year);
		printf("Input number of books: ");
		scanf("%d", &available);

		newBook = createBook(author, title, year, available);
	}
	
	while (temp != NULL) {
		//ako su isti naslovi knjige zbroji dostupne
		if (strcmp(newBook->title, temp->title) == 0) {
			temp->available += newBook->available;
			free(newBook);
			break;
		}
		//ako je naslov veci po ASCII kodu onda nastavi
		else if (strcmp(newBook->title, temp->title) > 0) {
			prev = temp;
			temp = temp->next;
		}
		//ako je manji po ASCII kodu upisi ga
		else{
			newBook->next = temp;
			prev->next = newBook;
			break;
		}
	}
	//ako je head->next NULL upisivamo ga na pocetak
	if (temp == NULL) {
		newBook->next = NULL;
		prev->next = newBook;
	}
	return 0;
}


PositionBook createBook(char* author, char* title, int year, int available) {
	PositionBook newBook = NULL;
	
	newBook = (PositionBook)malloc(sizeof(Books));
	if (!newBook) {
		printf("Error in memory allocation.");
		return MEMORY_NOT_ALLOCATED;
	}

	strcpy(newBook->title,title);
	strcpy(newBook->author,author);
	newBook->year = year;
	newBook->available = available;

	return newBook;
}

int printBooksByAuthor(PositionBook head) {
	PositionBook temp = head;
	printf("Author\tTitle\tYear\tAvailable\n");
	while (temp != NULL) {
		printf("%s %s %d %d\n", temp->author, temp->title, temp->year, temp->available);
		temp = temp -> next;
	}
	return 0;
}

int searchByAuthor(PositionBook head, char* name) {
	PositionBook temp = head->next;
	PositionBook prev = head;
	int i = 0;

	printf("\nBooks by %s: ", name);
	while (temp != NULL) {
		if (strcmp(temp->author, name) == 0) {
			printf("%s %s %d %d\n", temp->author, temp->title, temp->year, temp->available);
			i++;
		}
		prev = temp;
		temp = temp->next;
	}

	if (i==0) {
		printf("No available books by %s.\n", name);
		return NOT_FOUND;
	}
	return 0;
}

int searchByYear(PositionBook head, int wishedYear) {
	PositionBook temp = head->next;
	PositionBook prev = head;
	int i = 0;
	
	printf("\nBooks from %d. year: ", wishedYear);
	while (temp != NULL) {
		if (temp->year == wishedYear) {
			printf("%s %s %d %d\n", temp->author, temp->title, temp->year, temp->available);
			i++;
		}
		prev = temp;
		temp = temp->next;
	}

	if (i == 0) {
		printf("No available books from %d.\n", wishedYear);
		return NOT_FOUND;
	}
	return 0;
}


PositionUser inputUserSorted(PositionUser head, char* name) {
	PositionUser newUser = NULL, temp = head->next, prev = head;
	
	newUser = createUser(name);
	
	while (temp != NULL) {
		if (strcmp(newUser->name, temp->name) == 0) {
			printf("User already exists.");
			break;
		}
		else if (strcmp(newUser->name, temp->name) > 0) {
			prev = temp;
			temp = temp->next;
		}
		else {
			newUser->next = temp;
			prev->next = newUser;
			break;
		}
	}
	if (temp == NULL) {
		newUser->next = NULL;
		prev->next = newUser;
	}
	return newUser;
}


PositionUser createUser(char* name) {
	PositionUser newUser = NULL;
	int i = 0;

	newUser = (PositionUser)malloc(sizeof(Users));
	if (!newUser) {
		printf("Error in memory allocation.");
		return MEMORY_NOT_ALLOCATED;
	}
	strcpy(newUser->name, name);
	//sve knjige postavljamo na NULL
	for (i = 0;i < MAX_BOOKS;i++) {
		newUser->books[i] = NULL;
	}
	newUser->numberOFBooks = 0;
	
	return newUser;
}

int printUsers(PositionUser head) {
	PositionBook book = NULL;
	int i = 0;

	if (head == NULL) {
		printf("No users.\n");
		return NOT_FOUND;
	}
	while(head != NULL){
		printf("%s:\n", head->name);
		for (i = 0; head->books[i] != NULL; i++) {
			printf("%s %s %d\n", head->books[i]->author, head->books[i]->title, head->books[i]->year);
		}
		head = head->next;
	}

	return 0;
}

int takeBook(PositionBook BookHead, PositionUser UserHead) {
	PositionBook book = NULL;
	PositionUser user = NULL;
	char bookName[MAX_LENGHT] = { 0 }, userName[MAX_LENGHT] = { 0 }, choose = '\0';;
	int amount = 0, i = 0;

	//informacije koje trebaju za posudivanje
	printf("User taking book: ");
	scanf(" %s", userName);
	printf("Name of book: ");
	scanf(" %s", bookName);
	printf("Amount of books: ");
	scanf("%d", &amount);
	
	//trazimo u vezanim listama koji user posudiva knjigu i koja je to knjiga
	user = searchUser(UserHead, userName);
	book = searchBook(BookHead, bookName);

	
	if (book == NULL || book->available == 0) {
		printf("No book available.\n");
		return NOT_FOUND;
	}
	else if (user == NULL) {
		printf("User doesn't exist.\n");
		return NOT_FOUND;
	}
	else if (user->numberOFBooks == MAX_BOOKS) {
		printf("User has maximum amount of books.\n");
		return NOT_FOUND;
	}
	//ako zbroj kolicina knjiga koje zeli korisnik posudit i kolicina knjiga koje posjeduje prelaze maksimalan broj knjiga izdajemo upozorenje
	else if ((amount + user->numberOFBooks) > MAX_BOOKS) {
		printf("Unable to take that many books. User has %d books.\n", user->numberOFBooks);
		return NOT_FOUND;
	}
	//ako nemamo dovoljno knjiga koje korisnik zahtjeva pitamo ga zeli li uzet onoliko koliko imamo
	else if (amount > book->available) {
		printf("We don't have enough books, do you want to take ones left? Y/N\n");
		scanf(" %c", &choose);
		
		//ako zeli onda
		if (choose == 'Y') {
			//odvojeno dvi for petlje ovisno o situaciji ako korisnik ima 0 knjiga ili ako vec ima nekih knjiga u sebe, napravljeno zato jer sam se toga prvo sitio 
			//a index ide izvan opsega za numberOfBooks - 1 kad je numberOfBooks = 0
			if (user->numberOFBooks == 0) {
				//petlja ide od prvog slobodnog indexa pa do tog + koliko knjiga unosimo
				for (i = user->numberOFBooks; i < (user->numberOFBooks+book->available); i++) {
					user->books[i] = book;
				}
			}
			else {
				for (i = user->numberOFBooks - 1; i < (user->numberOFBooks + book->available); i++) {
					user->books[i] = book;
				}
			}
			//povecavamo broj knjiga za onoliko koliko ih je bilo dostupno i smanjivamo broj dostupnih za isto
			user->numberOFBooks += book->available;
			book->available -= book->available;
		}
		else if (choose == 'N') {
			return 0;
		}
	}
	else {
		//isti princip kao i petlje iznad samo umjesto svih dostupnih koristimo broj koliko korisnik zeli
		if (user->numberOFBooks == 0) 
		{
			for (i = user->numberOFBooks;i < (user->numberOFBooks + amount);i++) {
				user->books[i] = book;
			}
		}
		else {
			for (i = user->numberOFBooks-1;i < (user->numberOFBooks + amount);i++) {
				user->books[i] = book;
			}
		}
		user->numberOFBooks += amount;
		book->available -= amount;
	}
	return 0;
}

PositionBook searchBook(PositionBook head, char* name) {
	while (head->next != NULL) {
		if (strcmp(head->next->title, name) == 0) {
			return head->next;
		}
		head = head->next;
	}
	return NULL;
}

PositionUser searchUser(PositionUser head, char* user) {
	while (head->next != NULL) {
		if (strcmp(head->next->name, user) == 0) {
			return head->next;
		}
		head = head->next;
	}
	return NULL;
}

int returnBook(PositionBook BookHead, PositionUser UserHead) {
	char username[MAX_LENGHT] = { 0 }, bookname[MAX_LENGHT] = { 0 };
	int i = 0, j = 0;
	PositionBook book = NULL, post = NULL;
	PositionUser user = NULL;

	//ime korisnika koji vraca knjigu i naziv knjige
	printf("User returning book: ");
	scanf(" %s", username);
	printf("Book title to return: ");
	scanf(" %s", bookname);

	//u pokazivace spremamo tog korisnika i knjigu
	user = searchUser(UserHead, username);
	book = searchBook(BookHead, bookname);

	if (book == NULL) {
		printf("Book doesn't exist.\n");
		return NOT_FOUND;
	}
	else if (user == NULL) {
		printf("User doesn't exist.\n");
		return NOT_FOUND;
	}
	else {
		//idemo po listi i kad naidemo na knjigu koja se vraca povecavamo broj dostupnih knjiga za 1
		while (BookHead->next != NULL) {
			if (BookHead->next == book) {
				book->available++;
				break;
			}
			BookHead = BookHead->next;
		}
		//petlja se ponavlja onoliko puta koliko max knjiga mozemo imat
		for (i = 0; i < MAX_BOOKS;i++) {
			//ako se knjiga nalazi na i-tom mjestu u nizu smanjivamo broj knjiga korisnika za 1
			if (user->books[i] == book) {
				user->numberOFBooks--;
				//postavljamo taj clan niza na NULL jer tu knjigu korisnik vise ne posjeduje al najprije sve elemente niza koji se nalaze iza vracene knjige micemo za mjesto prije
				for (j = i+1; j < MAX_BOOKS;j++) {
					if (user->books[j] != NULL) {
						post = user->books[j];
						user->books[i] = user->books[j];
						user->books[i] = post;
					}
					else {
						user->books[j-1] = NULL;
						break;
					}
				}
				break;
			}
		}
	}
	return 0;
}

int saveBooksInFile(PositionBook BookHead) {
	FILE* filePointer = NULL;

	filePointer = fopen("BookHaven.txt", "w");

	if (!filePointer) {
		printf("Error in file opening!\n");
		return MEMORY_NOT_ALLOCATED;
	}
	fprintf(filePointer, "Author\tTitle\tYear\tAvailable\n");
	while (BookHead != NULL) {
		fprintf(filePointer, "%s\t%s\t%d\t%d\n", BookHead->author, BookHead->title, BookHead->year, BookHead->available);
		BookHead = BookHead->next;
	}

	fclose(filePointer);
	return 0;
}

int importBooksFromFile(PositionBook BookHead) {
	char author[MAX_LENGHT], title[MAX_LENGHT];
	int year = 0, available = 0;
	PositionBook book = NULL;
	FILE* filePointer = NULL;

	filePointer = fopen("BookHaven.txt", "r");

	if (!filePointer) {
		printf("Error in file opening!\n");
		return MEMORY_NOT_ALLOCATED;
	}
	char dummy[100];
	fgets(dummy, sizeof(dummy), filePointer);

	
	while (fscanf(filePointer, "%s\t%s\t%d\t%d", author,title,&year,&available) == 4) {
		book = createBook(author,title,year,available);
		inputBookSorted(BookHead, book);
	}
	fclose(filePointer);

	return 0;
}

int saveUsersInFile(PositionUser UserHead) {
	FILE* filePointer = NULL;
	int i = 0;

	filePointer = fopen("BookHavenUsers.txt", "w");

	if (!filePointer) {
		printf("Error in file opening!\n");
		return MEMORY_NOT_ALLOCATED;
	}
	
	while (UserHead != NULL) {
		fprintf(filePointer, "%s %d\n", UserHead->name, UserHead->numberOFBooks);
		for (i = 0; UserHead->books[i] != NULL; i++) {
			fprintf(filePointer, "%s\t%s\t%d\n", UserHead->books[i]->author, UserHead->books[i]->title, UserHead->books[i]->year);
		}
		UserHead = UserHead->next;
	}
	fclose(filePointer);
	return 0;
}

int importUsersFromFile(PositionUser UserHead, PositionBook BookHead) {
	char author[MAX_LENGHT] = { 0 }, title[MAX_LENGHT] = { 0 }, name[MAX_LENGHT] = { 0 };
	int year = 0, numberOfBooks = 0, i = 0, nbUsers = 0;
	PositionUser user = NULL;
	FILE* filePointer = NULL;

	filePointer = fopen("BookHavenUsers.txt", "r");

	if (!filePointer) {
		printf("Error in file opening!\n");
		return MEMORY_NOT_ALLOCATED;
	}
	while(fscanf(filePointer, "%s %d\n",name,&numberOfBooks)==2) {
		user = inputUserSorted(UserHead, name);
		user->numberOFBooks = numberOfBooks;
		i = 0;
		for (int j = 0; j < numberOfBooks;j++) {
			if (fscanf(filePointer, "%s\t%s\t%d\n", author, title, &year) == 3) {
				user->books[i] = searchBook(BookHead, title);
				i++;
			}
		}
		user = user->next;
	}
	fclose(filePointer);

	return 0;
}


