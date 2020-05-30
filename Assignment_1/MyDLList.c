// Author: Mohit Khanna
// Student ID: z5266543
// Platform: Mac
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#define MAX 1024 * 1024 // constant MAX

typedef struct DLListNode
{
	int value; // value (int) of this list item
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList
{
	int size;		   // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;

	L = malloc(sizeof(struct DLList));
	assert(L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}
/* --------------------------- My functions ----------------------------------- */
char *my_itoa(int number) //converts integer to ascii
{
	static char str[20];		//create an empty string to store number
	sprintf(str, "%d", number); //make the number into string using sprintf function
	return str;					//returning the string str
}

void removeLeadingTrailingSpaces(char *input, int stringLen)
{
	while (stringLen > 0 && isspace(input[stringLen - 1])) // removing leading and trailing blank spaces
	{
		stringLen--;
	}
	input[stringLen] = '\0'; // marking the end of the string after trimming the string
}

int isValidInteger(char *element) // checks for valid integer
{

	return strcmp(my_itoa(atoi(element)), element) == 0 || strcmp(element, "end") == 0; //return 1 for valid integer and 0 for invalid integer
}
/* --------------------------- My functions ----------------------------------- */

// create a DLList from a text file
// Lets assume there are M lines in the file/user-input to be read and each line has N integers
// Therefore outer while loop will be executed M times and inner N times.
// ======> Hence Time Complexity :O(MN) <======
DLList *CreateDLListFromFileDlist(const char *filename)
{
	DLList *L;
	L = newDLList();					// L is the new empty list
	DLListNode *curr;					// 'curr' node
	DLListNode *new;					// 'new' node
	char *element;						// each character in the string
	char input[MAX];					// input string both from a file and stdin
	char *delim = " ;\n";				// intergers are separated by spaces or blank lines
	FILE *fp;							// file poiner
	if (strcmp(filename, "stdin") == 0) // checking if the input is userinput
	{
		while (fgets(input, MAX, stdin) && strcmp(input, "end\n")) // gets input untill the reaches MAX or user inputs 'end'
		{
			removeLeadingTrailingSpaces(input, strlen(input)); // function call to remove extra spaces, call by reference
			if (strlen(input) > 0)							   // checking if the string exists
			{
				element = strtok(input, delim); // breaks the string into series of tokens using the delimiter
				if (L->first == NULL)			// creating the first node in the DLList
				{
					if (!isValidInteger(element)) // function call to validate integer, call by reference
					{
						puts("Invalid input!");
						exit(0);
					}
					if (strcmp(element, "end") == 0)
					{
						L->last = curr; // marking the last node
						return L;
					}
					L->first = newDLListNode(atoi(element)); // converting the string input to integer and creating a new node
					L->size += 1;							 // increamenting the size of DLList by 1
					curr = L->first;						 // pointing the first to curr to facilitate the appending of other nodes
					element = strtok(NULL, delim);			 // check for any remaining tokens
				}
				while (element != NULL) // appending all the integers to the DLList
				{
					if (!isValidInteger(element)) // checks for valid integer input
					{
						puts("Invalid input!");
						exit(0);
					}
					if (strcmp(element, "end") == 0)
					{
						L->last = curr; // marking the last node
						return L;
					}
					new = newDLListNode(atoi(element)); // converting the string input to integer and creating a new node
					curr->next = new;					// appening the new node to the curr node
					new->prev = curr;					// connecting the prev of new with curr [doubly linked list]
					L->size += 1;						// increamenting the size of DLList by 1
					curr = curr->next;					// pointing the next to curr to facilitate the appending of other nodes
					element = strtok(NULL, delim);		// check for any remaining tokens
				}
			}
		}
		L->last = curr; // marking the last node
	}
	else if ((fp = fopen(filename, "r")) != NULL)
	{
		while (!feof(fp))
		{
			fgets(input, 1024, fp);							   // get the input from a file
			removeLeadingTrailingSpaces(input, strlen(input)); // function cal to remove extra spaces, call by reference

			if (strlen(input) > 0) // checking if the string exists
			{
				element = strtok(input, delim); // breaks the string into series of tokens using the delimiter
				if (L->first == NULL)			// creating the first node in the DLList
				{
					if (!isValidInteger(element)) // checks for valid integer input
					{
						puts("Invalid input!");
						exit(0);
					}
					L->first = newDLListNode(atoi(element)); // converting the string input to integer and creating a new node
					L->size += 1;							 // increamenting the size of DLList by 1
					curr = L->first;						 // pointing the first to curr to facilitate the appending of other nodes
					element = strtok(NULL, delim);			 // check for any remaining tokens
				}
				while (element != NULL) // appending all the integers to the DLList
				{
					if (!isValidInteger(element)) // checks for valid integer input
					{
						puts("Invalid input!");
						exit(0);
					}
					new = newDLListNode(atoi(element)); // converting the string input to integer and creating a new node
					curr->next = new;					// appening the new node to the curr node
					new->prev = curr;					// connecting the prev of new with curr [doubly linked list]
					L->size += 1;						// increamenting the size of DLList by 1
					curr = curr->next;					// pointing the next to curr to facilitate the appending of other nodes
					element = strtok(NULL, delim);		// check for any remaining tokens
				}
			}
		}
		L->last = curr; // marking the last node
		fclose(fp);		// closing the file, imp to clear the memory
	}
	else //checking for any misadventure
	{
		puts("Invalid input!"); // printing the error message
		exit(0);				// exiting the code and returning NULL
	}
	return L; // returing the DLList L pointer
}

// clone a DLList
// Lets assume the DLList u has N integers and the while loop will be executed till the end N times to clone this list
// ======> Hence Time Complexity :O(N) <======
DLList *cloneList(DLList *u)
{
	DLList *L;
	L = newDLList();
	DLListNode *current = u->first; // setting first node of DLList of u as the current node
	DLListNode *new;
	while (current != NULL)
	{
		if (L->first == NULL) // creating the first node if the list is empty
		{
			L->last = newDLListNode(current->value);
			L->first = L->last;
			L->size += 1;
		}
		else // appending other nodes if the list is already present
		{
			new = newDLListNode(current->value);
			L->last->next = new;
			new->prev = L->last;
			L->last = L->last->next;
			L->size += 1;
		}
		current = current->next;
	}
	return L; // returing the DLList L pointer
}
// check whether the value is already in the DLList u.
// Lets assume the DLList u has N integers the while loop will be executed for each node to check whether the value of this node is equal to the parameter.
// ======> Hence Time Complexity :O(N) <======
int isPresent(struct DLList *u, int data)
{
	DLListNode *current = u->first;
	while (current != NULL) // loops unitll the end of the list
	{
		if (current->value == data) // checks the current value with the data passed in the params
			return 1;				// returns true if matches
		current = current->next;	// iterates to the next node
	}
	return 0; // returns false
}

// compute the union of two DLLists u and v
// Lets assume the size of DLList u and DLList v are N and M respectively. function cloneList() is O(N) [see above].
// The while loop will be executed M times for every node in v ,and the function isPresent() is O(N) [see above].
// ======> Hence Time Complexity :O(MN) <======
DLList *setUnion(DLList *u, DLList *v)
{
	DLList *L;
	DLListNode *new;
	DLListNode *ls2 = v->first;
	L = cloneList(u);	//cloning DLList u to DLList L
	while (ls2 != NULL) //loops untill reached the end og ls2
	{
		if (isPresent(L, ls2->value) == 0) // checking if the value is present in the DLList
		{
			new = newDLListNode(ls2->value);
			L->last->next = new;
			new->prev = L->last;
			L->last = L->last->next;
			L->size += 1;
		}
		ls2 = ls2->next; // appending the next ptr to ls2
	}
	return L; // returing the DLList L pointer
}

// compute the insection of two DLLists u and v
// Lets assume the size of DLList u and DLList v are N and M respectively
// The while loop will be executed M times for every node in u ,and the function isPresent() is O(N) [see above].
// ======> Hence Time Complexity :O(MN) <======
DLList *setIntersection(DLList *u, DLList *v)
{
	DLList *L;
	L = newDLList();
	DLListNode *new;
	DLListNode *ls1 = u->first;
	while (ls1 != NULL)
	{
		if (isPresent(v, ls1->value) == 1) // checking if the value is present in the DLList
		{
			if (L->first == NULL)
			{
				L->last = newDLListNode(ls1->value);
				L->first = L->last;
				L->size += 1;
			}
			else
			{
				new = newDLListNode(ls1->value);
				L->last->next = new;
				new->prev = L->last;
				L->last = L->last->next;
				L->size += 1;
			}
		}
		ls1 = ls1->next; // appending the next ptr to ls2
	}
	return L; // returing the DLList L pointer
}

// free up all space associated with list
// Lets assume DLList u has N integers
// While loop will be executed N times to free each node.
// ======> Hence Time Complexity :O(N) <======
void freeDLList(DLList *L)
{
	assert(L != NULL);
	DLListNode *current, *prev;
	current = L->first;
	while (current != NULL) // loops untill the end
	{
		prev = current;			 // marking the current node as the prev
		current = current->next; //detaching the link from prev node
		free(prev);				 // removing the prev node
	}
	L->last = L->first = NULL;
	free(L); // after removing all the nodes, freeing up the list compeletely
}

// display items of a DLList
// Lets assume DLList u has N integers
// While loop will be executed N times to print each node.
// ======> Hence Time Complexity :O(N) <======
void printDLList(DLList *u)
{
	DLListNode *node;
	node = u->first;
	// printf("%d\n", u->first->value); // printing last node
	// printf("%d\n", u->last->value); // printing last node
	if (u->first && u->last)
	{
		while (node != u->last) // loops unitll the end
		{
			printf("%d\n", node->value); // printing the value of the current node
			node = node->next;			 // iterating over to next node
		}
		printf("%d\n", u->last->value); // printing last node
	}
	else
	{
		puts("Empty DLList");
	}
}

int main()
{
	DLList *list1, *list2, *list3, *list4;
	list1 = CreateDLListFromFileDlist("File1.txt");
	puts("<=======list1=======>");
	printDLList(list1);

	list2 = CreateDLListFromFileDlist("File2.txt");
	puts("<=======list2=======>");
	printDLList(list2);

	list3 = setUnion(list1, list2);
	puts("<=======setUnion=======>");
	printDLList(list3);

	list4 = setIntersection(list1, list2);
	puts("<=======setIntersection=======>");
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	printf("please type all the integers of list1\n");
	list1 = CreateDLListFromFileDlist("stdin");

	printf("please type all the integers of list2\n");
	list2 = CreateDLListFromFileDlist("stdin");

	list3 = cloneList(list1);
	puts("<=======cloneList1=======>");
	printDLList(list3);
	list4 = cloneList(list2);
	puts("<=======cloneList2=======>");
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	return 0;
}