// Author: Mohit Khanna
// Student ID: z5266543
// Platform: Mac
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define MAX 1024 // constant MAX

// data type for avl tree nodes
typedef struct AVLTreeNode
{
	int key;					//key of this item
	int value;					//value (int) of this item
	int height;					//height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left;	//pointer to left child
	struct AVLTreeNode *right;	//pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree
{
	int size;		   // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v)
{
	// puts("value of kkkkkkkk and vvvvvvv");
	// printf("%d\n", k);
	// printf("%d\n", v);
	// puts("-------------------------------");
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0;  // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof(AVLTree));
	assert(T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

int InsertNode(AVLTree *T, int k, int v);	   // InsertNode function declaration
AVLTreeNode *Search(AVLTree *T, int k, int v); // Search function declaration

/* --------------------------- My functions ----------------------------------- */

//converts integer to ascii
// Is converted in constant time
// ======> Hence Time Complexity :O(1) <======
char *my_itoa(int number)
{
	static char str[20];		//create an empty string to store number
	sprintf(str, "%d", number); //make the number into string using sprintf function
	return str;					//returning the string str
}

// removes line breaks from the string
// Lets assume the given string has N characters and for loop will be executed N times to check every character
// ======> Hence Time Complexity :O(N) <======
void removeLineBreaks(char *str)
{
	for (int i = 0; str[i] != '\0'; i++) // reads string character-by-character
	{
		if (str[i] == '\n') // check for any line breaks
		{
			str[i] = ' '; // replaces it with a single blank space
		}
	}
}

// checks for valid integer
// Is checked in constant time
// ======> Hence Time Complexity :O(1) <======
int isValidInteger(char *element)
{
	return strcmp(my_itoa(atoi(element)), element) == 0; //return 1 for valid integer and 0 for invalid integer
}

// get (key,value) pair from the tree
// Lets assume the input string has N characters and the for loop will be executed N times to read every character present
// ======> Hence Time Complexity :O(N) <======
int *getKeyValue(char *input) // extracts key and value from the input string
{
	char parentheses = '\0';
	char integer[MAX] = "";
	int len;
	int j = 0;
	int *values = (int *)malloc(MAX * sizeof(int));
	for (int i = 0; i < strlen(input); i++) // traversing through the whole input string
	{
		if (input[i] == '(' || input[i] == ')')
		{
			if (parentheses == '\0') // check for empty parentheses
			{
				parentheses = input[i]; // sets it to the input parantheses
			}
			else if (input[i] == parentheses) // check for multiple parantheses
			{
				puts("Invalid input!"); // printing the error message
				exit(0);				// exiting the code
			}
			else
			{
				int num;
				if (!isValidInteger(integer)) // check if the integer is a valid one
				{
					puts("Invalid input!"); // printing the error message
					exit(0);				// exiting the code
				}
				num = atoi(integer);
				values[j + 1] = num; // insert valid integer into the values int array
				j++;
				strcpy(integer, ""); //reset the integer to empty string
				parentheses = '\0';	 // reset it to blank/NULL char
			}
		}
		else if (input[i] == ',')
		{
			int num;
			if (!isValidInteger(integer)) // check if the integer is a valid one
			{
				puts("Invalid input!"); // printing the error message
				exit(0);				// exiting the code
			}
			num = atoi(integer);
			values[j + 1] = num; // insert valid integer into the values int array
			j++;
			strcpy(integer, ""); //reset the integer to empty string
		}
		else if (input[i] != ' ')
		{
			len = strlen(integer); // get the integer string length
			integer[len] = input[i];
			integer[len + 1] = '\0'; // mark the end of the integer string
		}
	}
	values[0] = j; // insert the size into the head of values
	return values;
}

// Compare two items/nodes
// Comparing two integer values is done in the constent time.
// ======> Hence Time Complexity :O(1) <======
int compareItems(int key1, int value1, int key2, int value2)
{
	if (key1 == key2)
	{
		if (value1 == value2)
			return 0; //returns 0 when the itmes are equal
		else
			return value1 > value2 ? 1 : -1; //returns 1 when the about-to-insert node is smaller, -1 when it is bigger.
	}
	else
		return key1 > key2 ? 1 : -1; //returns 1 when the about-to-insert node is smaller, -1 when it is bigger.
}

// returns height of the given node
// Is calculated in the constent time.
// ======> Hence Time Complexity :O(1) <======
int height(AVLTreeNode *N)
{
	return N != NULL ? N->height : -1; // returns the height of the node if it is not null or else returns -1
}

// returns max of given two numbers
// Is calculated in the constent time.
// ======> Hence Time Complexity :O(1) <======
int maximum(int a, int b)
{
	return a > b ? a : b; // returns the max value
}

// right rotation
// Is done in constent time.
// O(1) + O(1) + O(1)
// ======> Hence Time Complexity :O(1) <======
void rightRotate(AVLTree *T, AVLTreeNode *node)
{
	AVLTreeNode *curr = node->left;
	// break and reconnect
	if (node == T->root) // check if the given node is the root itself
	{
		T->root = curr;		 // make the current node as the parent
		curr->parent = NULL; // make parent of the current node as NULL
	}
	else // if the given node is not the parent
	{
		if (node == node->parent->left)
			node->parent->left = curr;
		else
			node->parent->right = curr;

		curr->parent = node->parent; // make parent of given node as the parent of current node
	}
	//rotate
	node->left = curr->right; // right rotation
	if (curr->right != NULL)
		curr->right->parent = node;

	curr->right = node;	 // make the given node as right of the current node
	node->parent = curr; // make the current node as the parent of the given node
	//recalculate the height
	node->height = maximum(height(node->left), height(node->right)) + 1; // O(1) - height , O(1) - maximum
	curr->height = maximum(height(curr->left), height(curr->right)) + 1; // O(1) - height , O(1) - maximum
}

// left rotation
// Is done in constent time.
// O(1) + O(1) + O(1)
// ======> Hence Time Complexity :O(1) <======
void leftRotate(AVLTree *T, AVLTreeNode *node)
{
	AVLTreeNode *curr = node->right;
	// break and reconnect
	if (node == T->root) // check if the given node is the root itself
	{
		T->root = curr;		 // make the current node as the parent
		curr->parent = NULL; // make parent of the current node as NULL
	}
	else // if the given node is not the parent
	{
		if (node == node->parent->left)
			node->parent->left = curr;
		else
			node->parent->right = curr;

		curr->parent = node->parent; // make parent of given node as the parent of current node
	}
	//rotate
	node->right = curr->left; // left rotation
	if (curr->left != NULL)
		curr->left->parent = node;

	curr->left = node;	 // make the given node as left of the current node
	node->parent = curr; // make the current node as the parent of the given node
	//recalculate the height
	node->height = maximum(height(node->left), height(node->right)) + 1; // O(1) - height , O(1) - maximum
	curr->height = maximum(height(curr->left), height(curr->right)) + 1; // O(1) - height , O(1) - maximum
}

// returns difference of heights of the chidren of the given node
// Is calculated in the constent time.
// O(1) + O(1)
// ======> Hence Time Complexity :O(1) <======
int getBalance(AVLTreeNode *N)
{
	return N != NULL ? height(N->left) - height(N->right) : -1; // return difference if the node is not null or -1
}

//sets the height for each node in the tree
// Lets assume AVL tree has N nodes and it will take N recursion calls to traverse each node.
// ======> Hence Time Complexity :O(N) <======
void setHeight(AVLTree *T)
{
	AVLTreeNode *curr = T->root;
	AVLTree *subtreeLeft, *subtreeRight;
	subtreeLeft = newAVLTree();
	subtreeRight = newAVLTree();
	if (curr->left != NULL) //traversing till the left node is not NULL
	{
		subtreeLeft->root = curr->left;
		setHeight(subtreeLeft); // recursive call
	}

	curr->height = maximum(height(curr->left), height(curr->right)) + 1; // recalculating the height

	if (curr->right != NULL) //traversing till the right node is not NULL
	{
		subtreeRight->root = curr->right;
		setHeight(subtreeRight); // recursive call
	}
}

// rebalance the tree
// Lets assume that the AVL Tree has N nodes and to rebalance it will run for logN times
// O(1) + O(1) + O(logN)
// ======> Hence Time Complexity :O(log(N)) <======
void rebalance(AVLTree *T, AVLTreeNode *newNode)
{
	AVLTreeNode *curr = newNode;
	while (curr != NULL)
	{
		//recalculate the height
		int balance;
		balance = getBalance(curr); //get Balance of heights of left and right
		//find first unbalanced ancestor
		if (balance < -1) //check if right subtree is unbalanced
		{
			if (getBalance(curr->right) < 0) // check if only left rotation needed
				leftRotate(T, curr);
			else //or else double rotation needed
			{
				rightRotate(T, curr->right); // firstly, rotate right
				leftRotate(T, curr);		 // now, rotate left
			}
		}
		else if (balance > 1) //check if left subtree is unbalanced
		{
			if (getBalance(curr->left) > 0) // check if only right rotation needed
				rightRotate(T, curr);
			else //or else double rotation needed
			{
				leftRotate(T, curr->left); // firstly, rotate left
				rightRotate(T, curr);	   // now, rotate right
			}
		}
		curr->height = maximum(height(curr->left), height(curr->right)) + 1; // recalculating the height
		curr = curr->parent;
	}
}

// get all nodes items from a tree
// Lets assume avl tree has N nodes
// ======> Hence Time Complexity :O(N) <======
void getNodesArray(AVLTree *T, int *nodesArr)
{
	AVLTreeNode *stack[T->size];
	int size = 0, i = nodesArr[0] + 1;
	AVLTreeNode *curr = T->root;
	while (1)
	{
		if (curr != NULL)
		{
			stack[size] = curr;
			size++;			   // creament the size
			curr = curr->left; // start from left subtree
		}
		if (curr == NULL && size > 0) // if the curr is null and size of stack > 0
		{
			size--;								  //pop
			nodesArr[i] = stack[size]->key;		  // pop key from stack and set into nodesArr
			nodesArr[i + 1] = stack[size]->value; // pop value from stack and set into nodesArr
			nodesArr[0] = nodesArr[0] + 2;
			i += 2;					   // increament the counter by 2
			curr = stack[size]->right; // traverse to right subtree
		}
		if (curr == NULL && size == 0)
			break;
	}
}

// get intersection of two sorted int arrays
// Lets assume nodesT1 avl tree has N nodes and nodeT2 avl tree has M nodes.
// ======> Hence Time Complexity :O(N+M) <======
int *getIntersectionNodes(int *nodesT1, int *nodesT2, int m, int n)
{
	int *nodesT = (int *)malloc(MAX * sizeof(int));
	int i = 1, j = 1, k = 1;

	while (i <= m && j <= n)
	{
		if (compareItems(nodesT1[i], nodesT1[i + 1], nodesT2[j], nodesT2[j + 1]) < 0) // check if node of nodesT1 is smaller
		{
			i += 2;
		}
		else if (compareItems(nodesT1[i], nodesT1[i + 1], nodesT2[j], nodesT2[j + 1]) > 0) //check if node of nodesT2 is smaller
		{
			j += 2;
		}
		else // if both are equal
		{
			nodesT[k] = nodesT1[i];
			nodesT[k + 1] = nodesT1[i + 1];
			nodesT[0] = nodesT[0] + 2;
			j = j + 2;
			k = k + 2;
		}
	}
	return nodesT;
}

// get union of two sorted int arrays
// Lets assume nodesT1 avl tree has N nodes and nodeT2 avl tree has M nodes.
// ======> Hence Time Complexity :O(N+M) <======
int *getUnionNodes(int *nodesT1, int *nodesT2, int m, int n)
{
	int *nodesT = (int *)malloc(MAX * sizeof(int));
	int i = 1, j = 1, k = 1;

	while (i <= m && j <= n)
	{
		if (compareItems(nodesT1[i], nodesT1[i + 1], nodesT2[j], nodesT2[j + 1]) < 0) // check if node of nodesT1 is smaller
		{
			nodesT[k] = nodesT1[i];
			nodesT[k + 1] = nodesT1[i + 1];
			nodesT[0] = nodesT[0] + 2;
			i = i + 2;
			k = k + 2;
		}
		else if (compareItems(nodesT1[i], nodesT1[i + 1], nodesT2[j], nodesT2[j + 1]) > 0) //check if node of nodesT2 is smaller
		{
			nodesT[k] = nodesT2[j];
			nodesT[k + 1] = nodesT2[j + 1];
			nodesT[0] = nodesT[0] + 2;
			j = j + 2;
			k = k + 2;
		}
		else // if both are equal
		{
			nodesT[k] = nodesT1[i];
			nodesT[k + 1] = nodesT1[i + 1];
			nodesT[0] = nodesT[0] + 2;
			i = i + 2;
			j = j + 2;
			k = k + 2;
		}
	}

	while (i < m) // all remaining elements in nodes of nodesT1
	{
		nodesT[k] = nodesT1[i];
		nodesT[k + 1] = nodesT1[i + 1];
		nodesT[0] = nodesT[0] + 2;
		i = i + 2;
		k = k + 2;
	}

	while (j < n) // all remaining elements in nodes of nodesT2
	{
		nodesT[k] = nodesT2[j];
		nodesT[k + 1] = nodesT2[j + 1];
		nodesT[0] = nodesT[0] + 2;
		j = j + 2;
		k = k + 2;
	}
	return nodesT;
}

// converts the sorted array to avl tree
// Lets assume the array has N nodes and it will take N iterations to form the given array into the avl tree
// we can get middle element in O(1) time
// ======> Hence Time Complexity :O(N) <======
AVLTreeNode *sortedArrayToBST(int *arr, int start, int end)
{
	if (start > end) // base case
	{
		return NULL;
	}
	int mid = (start + end) / 2; // get middle element, make it root
	if (mid % 2 == 0)
		mid += 1;

	AVLTreeNode *root = newAVLTreeNode(arr[mid], arr[mid + 1]);
	root->left = sortedArrayToBST(arr, start, mid - 2); // Recursively construct the left subtree and make it left child of root
	root->right = sortedArrayToBST(arr, mid + 2, end);	// Recursively construct the right subtree and make it right child of root

	return root;
}

//get the largest node
// Lets assume avl tree has N nodes and the largest node will be the righmost node
// while loop will take logrithmic time to traverse and reach the rightmost node
// ======> Hence Time Complexity :O(log(N)) <======
AVLTreeNode *getLargestNode(AVLTreeNode *nd)
{
	AVLTreeNode *curr = nd;
	while (curr->right != NULL)
	{
		curr = curr->right;
	}
	return curr;
}

/* --------------------------- My functions ----------------------------------- */

// creats the AVL tree
// Lets assume we have N items to be inserted in the AVL tree and each insert operation takes log(N).
// O(N) + O(N) + O(N) + O(Nlog(N))
// ======> Hence Time Complexity :O(Nlog(N)) <======
AVLTree *CreateAVLTree(const char *filename)
{
	// create an empty tree
	FILE *fp;
	AVLTree *T;
	T = newAVLTree();
	char input[MAX * MAX] = "";
	char line[MAX] = "";
	if (strcmp(filename, "stdin") == 0) // checking if the input is userinput
	{
		while (fgets(line, MAX, stdin) && strcmp(line, "end\n")) // gets input untill the reaches MAX or user inputs 'end'
		{
			if (strstr(line, "end") != NULL) // check if the line contains 'end'
			{
				char *splitLine = strtok(line, "end"); // split and ignore nodes entered after 'end'
				strcat(input, splitLine);			   // concatenate the same into the input string
				break;
			}
			strcat(input, line); // concatenate the line into the input string
		}
	}
	else if ((fp = fopen(filename, "r")) != NULL) // check if the file can be opened in the read mode
	{
		while (!feof(fp)) // read the file untill the end of file encountered
		{
			fscanf(fp, "%s", line); // get the line from a file
			strcat(input, line);	// concatenate the same into the input string
		}
	}
	else //checking for any misadventure
	{
		puts("Invalid input!"); // printing the error message
		exit(0);				// exiting the code
	}
	int *items;
	removeLineBreaks(input);			// call by reference - O(N)
	items = getKeyValue(input);			// call by value - O(N)
	for (int i = 1; i <= items[0]; i++) // iterate over each item over N items
	{
		InsertNode(T, *(items + i), *(items + i + 1)); // call by reference - O(log(N))
		i++;
	}
	return T; // return the tree T
}

// clons the given AVL tree
// Lets assume the AVL tree has N nodes and recursion will happen N times to clone the given tree
// ======> Hence Time Complexity :O(N) <======
AVLTree *CloneAVLTree(AVLTree *T)
{
	AVLTree *cloneTree, *leftSubtree, *rightSubtree, *CL, *CR;
	if (T->root == NULL)
		return T;
	cloneTree = newAVLTree();
	leftSubtree = newAVLTree();
	rightSubtree = newAVLTree();
	AVLTreeNode *curr = T->root;
	AVLTreeNode *new = newAVLTreeNode(curr->key, curr->value);
	cloneTree->root = new;		// clone the root
	new->height = curr->height; // clone the height
	leftSubtree->root = curr->left;
	rightSubtree->root = curr->right;
	CL = CloneAVLTree(leftSubtree);	 // clone the left subtree, recursive call
	CR = CloneAVLTree(rightSubtree); // clone the right subtree, recursive call
	if (CL->root != NULL)
		CL->root->parent = new->left; // iterate over the left subtree
	if (CR->root != NULL)
		CR->root->parent = new->right; // iterate over the right subtree
	new->left = CL->root;
	new->right = CR->root;
	return cloneTree; // return the cloned tree
}

// returns a intersection avl tree
// Lets assume avl tree T1 has N nodes and avl tree T2 has M nodes
// O(N) + O(M) + O(N+M) + O(N+M) + O(N)
// ======> Hence Time Complexity :O(N+M) <======
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	int *nodesT1 = (int *)malloc(MAX * sizeof(int)), *nodesT2 = (int *)malloc(MAX * sizeof(int)), *nodesT;
	//get all vaules of two trees separately
	getNodesArray(T1, nodesT1); // call by reference - O(N)
	getNodesArray(T2, nodesT2); // call by reference - O(M)
	// get the union of these two sorted int arrays
	nodesT = getUnionNodes(nodesT1, nodesT2, nodesT1[0], nodesT2[0]); // call by value - O(N+M)
	AVLTree *T = newAVLTree();
	T->root = sortedArrayToBST(nodesT, 1, nodesT[0]); // call by value O(N+M)
	T->size = nodesT[0];
	setHeight(T); //call by reference - O(N)
	return T;
}

// returns a intersection avl tree
// Lets assume avl tree T1 has N nodes and avl tree T2 has M nodes
// O(N) + O(M) + O(N+M) + O(N+M) + O(N)
// ======> Hence Time Complexity :O(N+M) <======
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	int *nodesT1 = (int *)malloc(MAX * sizeof(int)), *nodesT2 = (int *)malloc(MAX * sizeof(int)), *nodesT;
	//get all vaules of two trees separately
	getNodesArray(T1, nodesT1); // call by reference - O(N)
	getNodesArray(T2, nodesT2); // call by reference - O(M)
	// get the intersection of these two sorted int arrays
	nodesT = getIntersectionNodes(nodesT1, nodesT2, nodesT1[0], nodesT2[0]); // call by value - O(N+M)
	AVLTree *T = newAVLTree();
	// create the intersected tree
	T->root = sortedArrayToBST(nodesT, 1, nodesT[0]); // call by value O(N+M)
	T->size = nodesT[0];
	setHeight(T); //call by reference - O(N)
	return T;
}

// insert newNode in the AVL tree
// Lets assume AVL tree has N nodes and to insert a node into the AVL tree, it has to iterated over log(N) times
// O(1) + O(1) + O(log(N))
// ======> Hence Time Complexity :O(log(N)) <======
int InsertNode(AVLTree *T, int k, int v)
{
	AVLTreeNode *newNode;
	newNode = newAVLTreeNode(k, v);

	if (T->root == NULL) // check for empty tree
	{
		T->root = newNode;
		T->size++;
		return 1;
	}
	AVLTreeNode *curr = T->root;
	while (1) // while loop untill its false
	{
		if (compareItems(curr->key, curr->value, k, v) > 0) // check place to insert the node in left subtree
		{
			if (curr->left == NULL)
			{
				curr->left = newNode;
				break; // break if cannot insert in the left subtree
			}
			else
				curr = curr->left; // iterate over left subtree
		}
		else if (compareItems(curr->key, curr->value, k, v) < 0) // check place to insert the node in left subtree
		{
			if (curr->right == NULL)
			{
				curr->right = newNode;
				break; // break if cannot insert in the right subtree
			}
			else
				curr = curr->right; // iterate over right subtree
		}
		else
			return 0; // return 0 if the duplicate node exists
	}
	T->size++;				// increase the AVL tree size by 1
	newNode->parent = curr; // set the parent of the newNode as current node
	rebalance(T, newNode);	// rebalancing the tree on every insert - O(log(N))
	return 1;				// return 1 if the node has been successfully inserted in the AVL tree
}

// O(log(n))
// find a delete the node from the tree
// Lets assume avl tree has N nodes, to search and delete the given node and again rebalancing the tree
// O(log(N)) + O(log(N)) + O(log(N)) + O(log(N))
// ======> Hence Time Complexity :O(log(N)) <======
int DeleteNode(AVLTree *T, int k, int v)
{
	AVLTreeNode *deleteNode = Search(T, k, v); // O(log(N))
	AVLTreeNode *parentNode;
	if (deleteNode == NULL)
		return 0;
	else
	{
		//deleteNode has no children
		if (deleteNode->left == NULL && deleteNode->right == NULL)
		{
			//deleteNode is the root
			if (deleteNode == T->root)
				T->root = NULL;
			else
			{
				if (deleteNode == deleteNode->parent->left) //deleteNode is in the left
					deleteNode->parent->left = NULL;

				else //deleteNode is in the right
					deleteNode->parent->right = NULL;

				parentNode = deleteNode->parent;
				deleteNode->parent = NULL;
			}
		}
		else if (deleteNode->left != NULL && deleteNode->right != NULL)
		{
			//deleteNode has two children
			if (deleteNode == T->root)
			{
				T->root = deleteNode->left;
				deleteNode->left->parent = NULL;
			}
			else
			{
				if (deleteNode == deleteNode->parent->left)
					deleteNode->parent->left = deleteNode->left;
				else
					deleteNode->parent->right = deleteNode->left;
				deleteNode->left->parent = deleteNode->parent;
				deleteNode->parent = NULL;
			}
			//find the largest node in the left part of deleteNode
			AVLTreeNode *largestNode = getLargestNode(deleteNode->left); // call by value - O(log(N))
			largestNode->right = deleteNode->right;
			deleteNode->right->parent = largestNode;
			deleteNode->left = NULL;
			deleteNode->right = NULL;
			parentNode = largestNode;
		}
		else if (deleteNode->left != NULL)
		{
			//deleteNode has left child
			if (deleteNode == T->root)
			{
				T->root = deleteNode->left;
				deleteNode->left->parent = NULL;
			}
			else
			{
				if (deleteNode == deleteNode->parent->left)
					deleteNode->parent->left = deleteNode->left;
				else
					deleteNode->parent->right = deleteNode->left;
				deleteNode->left->parent = deleteNode->parent;
				deleteNode->parent = NULL;
			}
			parentNode = deleteNode->left->parent;
			deleteNode->left = NULL;
		}
		else
		{
			//deleteNode has right child
			if (deleteNode == T->root)
			{
				T->root = deleteNode->right;
				deleteNode->right->parent = NULL;
			}
			else
			{
				if (deleteNode == deleteNode->parent->left)
					deleteNode->parent->left = deleteNode->right;

				else
					deleteNode->parent->right = deleteNode->right;
				deleteNode->right->parent = deleteNode->parent;
				deleteNode->parent = NULL;
			}
			parentNode = deleteNode->right->parent;
			deleteNode->right = NULL;
		}
		free(deleteNode);
		T->size--;
	}
	rebalance(T, parentNode); //recalculate the height of all the ancestors - O(log(N))
	return 1;
}

// search for a node
// Lets assume AVLTree T has N nodes and to search the given item
// while loop will take logrithmic time to traverse
// ======> Hence Time Complexity :O(log(N)) <======
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
	if (T->root == NULL)
		return NULL; // tree is empty, return NULL
	else
	{
		AVLTreeNode *curr = T->root;
		// return a node if it finds the item, otherwise, return NULL.
		while (curr != NULL)
		{
			if (compareItems(curr->key, curr->value, k, v) == 0) // check if the node matches
				return curr;									 // return the current node
			else
				// if the node is smaller traverse in the left subtree or else traverse in the right subtree
				curr = compareItems(curr->key, curr->value, k, v) > 0 ? curr->left : curr->right;
		}
		return NULL; // node not found, return NULL
	}
}

// free up all space associated with the tree
// Lets assume AVLTree T has N nodes
// recursion will be executed N times to to free each node.
// ======> Hence Time Complexity :O(N) <======
void FreeAVLTree(AVLTree *T)
{
	// put your code here
	AVLTreeNode *curr = T->root;
	AVLTree *subtreeLeft, *subtreeRight;
	subtreeLeft = newAVLTree();
	subtreeRight = newAVLTree();
	if (curr->left != NULL)
	{
		subtreeLeft->root = curr->left;
		curr->left->parent = NULL;
		curr->left = NULL;
		FreeAVLTree(subtreeLeft);
	}
	if (curr->right != NULL)
	{
		subtreeRight->root = curr->right;
		curr->right->parent = NULL;
		curr->right = NULL;
		FreeAVLTree(subtreeRight);
	}
	T->size = 0;
	free(curr);
}

// display items of the AVLTree
// Lets assume AVLTree T has N nodes
// recursion will be executed N times to print each node.
// ======> Hence Time Complexity :O(N) <======
void PrintAVLTree(AVLTree *T)
{
	AVLTreeNode *curr = T->root;
	AVLTree *subtreeLeft, *subtreeRight;
	subtreeLeft = newAVLTree();
	subtreeRight = newAVLTree();
	if (curr->left != NULL) //traversing till the left node is not NULL
	{
		subtreeLeft->root = curr->left;
		PrintAVLTree(subtreeLeft); // recursive call
	}

	printf("(%d,%d),%d\n", curr->key, curr->value, curr->height); // printing the AVLtree by inorder traversal

	if (curr->right != NULL) //traversing till the right node is not NULL
	{
		subtreeRight->root = curr->right;
		PrintAVLTree(subtreeRight); // recursive call
	}
}

//sample main for testing
int main()
{
	int i, j;
	AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
	AVLTreeNode *node1;
	puts("please type all the nodes for tree1");
	tree1 = CreateAVLTree("stdin");
	puts("<=======tree1=======>");
	PrintAVLTree(tree1);
	FreeAVLTree(tree1);
	//you need to create the text file file1.txt
	// to store a set of items without duplicate items
	tree2 = CreateAVLTree("File1.txt");
	puts("<=======tree2=======>");
	PrintAVLTree(tree2);
	tree3 = CloneAVLTree(tree2);
	puts("<=======CloneAVLTree=======>");
	PrintAVLTree(tree3);
	FreeAVLTree(tree2);
	FreeAVLTree(tree3);
	//Create tree4
	tree4 = newAVLTree();
	j = InsertNode(tree4, 10, 10);
	for (i = 0; i < 15; i++)
	{
		j = InsertNode(tree4, i, i);
		if (j == 0)
			printf("(%d, %d) already exists\n", i, i);
	}
	puts("<=======tree4=======>");
	PrintAVLTree(tree4);
	puts("<=======SearchNode=======>");
	node1 = Search(tree4, 20, 20);
	if (node1 != NULL)
		printf("key= %d value= %d\n", node1->key, node1->value);
	else
		printf("Key 20 does not exist\n");

	for (i = 17; i > 0; i--)
	{
		j = DeleteNode(tree4, i, i);
		puts("<=======AfterDeletion=======>");
		if (j == 0)
			printf("Key %d does not exist\n", i);
		PrintAVLTree(tree4);
	}
	FreeAVLTree(tree4);
	//  Create tree5
	tree5 = newAVLTree();
	j = InsertNode(tree5, 6, 25);
	j = InsertNode(tree5, 6, 12);
	j = InsertNode(tree5, 6, 10);
	j = InsertNode(tree5, 6, 20);
	j = InsertNode(tree5, 9, 25);
	j = InsertNode(tree5, 10, 25);
	puts("<=======tree5=======>");
	PrintAVLTree(tree5);
	//Create tree6
	tree6 = newAVLTree();
	j = InsertNode(tree6, 6, 25);
	j = InsertNode(tree6, 5, 10);
	j = InsertNode(tree6, 6, 12);
	j = InsertNode(tree6, 6, 20);
	j = InsertNode(tree6, 8, 35);
	j = InsertNode(tree6, 10, 25);
	puts("<=======tree6=======>");
	PrintAVLTree(tree6);
	tree7 = AVLTreesIntersection(tree5, tree6);
	tree8 = AVLTreesUnion(tree5, tree6);
	puts("<=======tree7=======>");
	PrintAVLTree(tree7);
	puts("<=======tree8=======>");
	PrintAVLTree(tree8);
	return 0;
}