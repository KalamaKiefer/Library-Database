#include <iostream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

class Patron;   // forward declaration;


class Book {
public:
	Book() {
		patron = NULL; // if the book is checked out to that patron or not
	}
	bool operator== (const Book& bk) const {
		return strcmp(title, bk.title) == NULL; // compares the strings and returns a bool value
	}
private:
	char* title; // pointer to books title
	Patron* patron;
	ostream& printBook(ostream&) const; 
	friend ostream& operator<< (ostream& out, const Book& bk) { // overloaded << operator, prints book
		return bk.printBook(out);
	}
	friend class CheckedOutBook;
	friend Patron;
	friend void includeBook(); // function to include books
	friend void checkOutBook(); // function to check out books
	friend void returnBook(); // function to return books
};

class Author {
public:
	Author() {
	}
	bool operator== (const Author& ar) const {
		return strcmp(name, ar.name) == NULL; // compares authors name and returns boolean value
	}
private:
	char* name; // pointer to authors name
	list<Book> books; // list of books 
	ostream& printAuthor(ostream&) const; // output stream prints authors name
	friend ostream& operator<< (ostream& out, const Author& ar) { // overloaded << operator to print author
		return ar.printAuthor(out);
	}
	friend void includeBook();
	friend void checkOutBook();
	friend void returnBook();
	friend class CheckedOutBook;
	friend Patron;
};

class CheckedOutBook { // class checks if a book has been checked out or returned 
public:
	CheckedOutBook(list<Author>::iterator ar, // author list iterator and book list iterator
		list<Book>::iterator bk) {
		author = ar;
		book = bk;
	}
	bool operator== (const CheckedOutBook& bk) const {
		return strcmp(author->name, bk.author->name) == NULL && // compare strings for the authors name and books title
			strcmp(book->title, bk.book->title) == NULL;
	}
private:
	list<Author>::iterator author; // iterator for checked out author list
	list<Book>::iterator book; // iterator for checked out book list
	friend void checkOutBook();
	friend void returnBook();
	friend Patron;
};

class Patron {
public:
	Patron() {
	}
	bool operator== (const Patron& pn) const {
		return strcmp(name, pn.name) == NULL; // compares string patrons name 
	}
private:
	char* name; // pointer to patrons name
	list<CheckedOutBook> books;
	ostream& printPatron(ostream&) const;
	friend ostream& operator<< (ostream& out, const Patron& pn) {
		return pn.printPatron(out); // overloaded << operator that prints patrons name
	}
	friend void checkOutBook();
	friend void returnBook();
	friend Book;
};

list<Author> catalog['Z' + 1]; // author list
list<Patron> people['Z' + 1]; // patrons list

ostream& Author::printAuthor(ostream& out) const { // prints authors name , and iterates through authors book list
	out << name << endl;
	list<Book>::const_iterator ref = books.begin();
	for (; ref != books.end(); ref++)
		out << *ref; // overloaded <<
	return out;
}

ostream& Book::printBook(ostream& out) const { // prints book and the patron its checked out too if there is one
	out << "    * " << title;
	if (patron != NULL) // if the book is checked out
		out << " - checked out to " << patron->name; // overloaded <<
	out << endl;
	return out;
}

ostream& Patron::printPatron(ostream& out) const { // prints patrons name and the books they have
	out << name;
	if (!books.empty()) {
		out << " has the following books:\n";
		list<CheckedOutBook>::const_iterator bk = books.begin(); // iterates through checked out books list for patron
		for (; bk != books.end(); bk++)
			out << "    * " << bk->author->name << ", "
			<< bk->book->title << endl;
	}
	else out << " has no books\n";
	return out;
}

template<class T>
ostream& operator<< (ostream& out, const list<T>& lst) { // overloaded << operator that prints lists
	for (typename list<T>::const_iterator ref = lst.begin(); ref != lst.end(); ref++)
		out << *ref; // overloaded <<
	return out;
}

char* getString(const char* msg) { // gets strings for book title, author name, and patron name
	char s[82], i, * destin;
	cout << msg;
	cin.get(s, 80);
	while (cin.get(s[81]) && s[81] != '\n');  // discard overflowing
	destin = new char[strlen(s) + 1];           // characters;
	for (i = 0; destin[i] = toupper(s[i]); i++);
	return destin;
}

void status() { // checks the status of the library
	register int i;
	cout << "Library has the following books:\n\n";
	for (i = 'A'; i <= 'Z'; i++)
		if (!catalog[i].empty()) // iterates through the libraries catalog
			cout << catalog[i];
	cout << "\nThe following people are using the library:\n\n";
	for (i = 'A'; i <= 'Z'; i++) // iterates through patrons who are using the library
		if (!people[i].empty())
			cout << people[i];
}

void includeBook() { // adds new books to the library
	Author newAuthor;
	Book newBook;
	list<Author>::iterator oldAuthor; // author list iterator
	list<Book>::iterator bookRef; // book list iterator
	newAuthor.name = getString("Enter author's name: "); // gets new authors name
	newBook.title = getString("Enter the title of the book: "); // gets new book title


	oldAuthor = find(catalog[newAuthor.name[0]].begin(), // finds the author in the catalog
		catalog[newAuthor.name[0]].end(), newAuthor);

	// catalog is ordered by the first letter of authors name
	if (catalog[newAuthor.name[0]].empty()) // checks if the authors list in the catalog is empty
	{
		
		if (oldAuthor == catalog[newAuthor.name[0]].end()) { // if the author isnt in the catalog, add him/her
			newAuthor.books.push_front(newBook);
			catalog[newAuthor.name[0]].push_front(newAuthor);

		}
		else (*oldAuthor).books.push_front(newBook); // else push the new book onto the old authors list
	}
	else
	{
		// checks if the book is in the authors list
		bookRef = find((*oldAuthor).books.begin(), (*oldAuthor).books.end(), newBook);
		if (bookRef == (*oldAuthor).books.end()) // if the book isnt in the authors list 
		{
			if (oldAuthor == catalog[newAuthor.name[0]].end()) { // if the author isnt in the catalog, add him/her
				newAuthor.books.push_front(newBook);
				catalog[newAuthor.name[0]].push_front(newAuthor);

			}
			else (*oldAuthor).books.push_front(newBook); // else push the new book onto the old authors list
		}
		else // if the book is in the authors list that means it already exists in the library
		{
			cout << newBook.title << " already exists in the library" << endl;
			return;
		}
	}
}


void checkOutBook() {
	Patron patron;
	Author author;
	Book book;
	list<Author>::iterator authorRef; // author list iterator
	list<Book>::iterator bookRef; // book list iterator
	patron.name = getString("Enter patron's name: "); // gets patrons name
	while (true) {
		author.name = getString("Enter author's name: "); // gets authors name
		authorRef = find(catalog[author.name[0]].begin(), // looks in catalog for authors name
			catalog[author.name[0]].end(), author);
		if (authorRef == catalog[author.name[0]].end()) // if the authors name is not in the catalog
		{
			cout << "Author does not exist in catalog\n";
			return; // return to menu
		}
		else break;
	}
	while (true) {
		book.title = getString("Enter the title of the book: "); // gets book title
		bookRef = find((*authorRef).books.begin(), // looks in authors list for the book
			(*authorRef).books.end(), book);
		if (bookRef == (*authorRef).books.end()) // if the book is not in the authors list
		{
			cout << "Book is not in the Authors list\n";
			return; // return to menu
		} else if ((*bookRef).patron != NULL) // if the book is checked out then print checked out and return
		{
			cout << "Book already checked out." << endl;
			return; // return to menu
		}
		else break;
	}
	list<Patron>::iterator patronRef; // patron list iterator
	patronRef = find(people[patron.name[0]].begin(), // look for patrons name in people list
		people[patron.name[0]].end(), patron);
	CheckedOutBook checkedOutBook(authorRef, bookRef); // check out the authors book
	if (patronRef == people[patron.name[0]].end()) { // a new patron 
		patron.books.push_front(checkedOutBook);    // in the library;
		people[patron.name[0]].push_front(patron);
		(*bookRef).patron = &*people[patron.name[0]].begin(); 
	}
	else {
		(*patronRef).books.push_front(checkedOutBook); // if the patron already exists then check it out to them
		(*bookRef).patron = &*patronRef;
	}
}

void returnBook() { // return book to the library
	Patron patron;
	Book book;
	Author author;
	list<Patron>::iterator patronRef; // patron list iterator
	list<Book>::iterator bookRef; // book list iterator
	list<Author>::iterator authorRef; // author list iterator
	while (true) {
		patron.name = getString("Enter patron's name: "); // get patrons name
		patronRef = find(people[patron.name[0]].begin(), // find patron in peoples list
			people[patron.name[0]].end(), patron);
		if (patronRef == people[patron.name[0]].end()) // if the patron doesnt exist
		{
			cout << "Patron does not exist\n";
			return; // return to menu
		}
		else break;
	}
	while (true) {
		author.name = getString("Enter author's name: "); // get authors name
		authorRef = find(catalog[author.name[0]].begin(), // find author in catalog
			catalog[author.name[0]].end(), author);
		if (authorRef == catalog[author.name[0]].end()) // if author isnt in the catalog
		{
			cout << "Author does not exist\n";
			return; // return to menu
		}
		else break;
	}
	while (true) {
		book.title = getString("Enter the title of the book: "); // get book title
		bookRef = find((*authorRef).books.begin(), // look through author list for book
			(*authorRef).books.end(), book);
		if (bookRef == (*authorRef).books.end()) // if the book is not in the authors list
		{
			cout << "You do not have that book checked out\n";
			return; // return to menu
		}
		else if ((*bookRef).patron != &*patronRef) // if the person who is trying to return the book 
			// does not match the address of the patron who checked out the book
		{
			cout << "That book is not checked out to you" << endl;
			return; //return to menu
		}
		else break;
	}
	CheckedOutBook checkedOutBook(authorRef, bookRef); 
	(*bookRef).patron = NULL; // set books patron to null or no one
	(*patronRef).books.remove(checkedOutBook); // remove book from checked out list
}

int menu() { // menu function 
	int option;
	cout << "\nEnter one of the following options:\n"
		<< "1. Include a book in the catalog\n2. Check out a book\n"
		<< "3. Return a book\n4. Status\n5. Exit\n"
		<< "Your option? ";
	cin >> option; // gets option from user
	cin.get();         // discard '\n';
	return option;
}

int main() {
	while (true)
		switch (menu()) { // switch case for int returned from menu function
		case 1: includeBook();  break;
		case 2: checkOutBook(); break;
		case 3: returnBook();   break;
		case 4: status();       break;
		case 5: return 0;
		default: cout << "Wrong option, try again: ";
		}
	return 0;
}