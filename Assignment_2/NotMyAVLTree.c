#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#define COUNT 10

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

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
// declaration
int InsertNode(AVLTree *T, int k, int v);
AVLTreeNode *Search(AVLTree *T, int k, int v);
void PrintAVLTree(AVLTree *T);
int DeleteNode(AVLTree *T, int k, int v);

//O(1)
int convert(char *string)
{
	int num = 0;
	int pre = 1;
	if (string[0] == '-')
	{
		pre = -1;
	}
	for (int i = 0; i < strlen(string); i++)
	{
		if (string[i] != '-')
		{
			int temp = string[i] - '0';
			num = num * 10 + temp;
		}
	}
	num = num * pre;
	return num;
}

//O(n)
int *getKV(char *data_string)
{
	char parenthses = '\0';
	char integer[255] = "";
	int len;
	int j = 0;
	int *values = (int *)malloc(255 * sizeof(int));
	for (int i = 0; i < strlen(data_string); i++)
	{
		if (data_string[i] == '(' || data_string[i] == ')')
		{
			if (parenthses == '\0')
			{
				parenthses = data_string[i];
			}
			else if (data_string[i] == parenthses)
			{
				printf("Error syntax!!\n");
				exit(0);
			}
			else
			{
				int num;
				num = convert(integer);
				//no problem!!! how stupid am I !!!
				values[j + 1] = num;
				// printf("%d\n",values[j]);
				j++;
				strcpy(integer, "");
				parenthses = '\0';
			}
		}
		else if (data_string[i] == ',')
		{
			int num;
			num = convert(integer);
			//no problem!!!
			values[j + 1] = num;
			// printf("%d\n",values[j]);
			j++;
			strcpy(integer, "");
		}
		else if (data_string[i] != ' ')
		{
			len = strlen(integer);
			integer[len] = data_string[i];
			integer[len + 1] = '\0';
		}
	}
	// insert the szie into the head of values
	values[0] = j;
	return values;
}

//O(1)
// Compare two nodes
// returns 1 when the about-to-insert node is smaller;
// returns 0 when they are equal; returns -1 when he about-to-insert node is bigger.
int comparasion(int r, int s, int k, int v)
{
	if (r > k)
	{
		return 1;
	}
	else if (r == k)
	{
		if (s > v)
		{
			return 1;
		}
		else if (s == v)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

//O(1)
int Ht(AVLTreeNode *N)
{
	if (N == NULL)
	{
		return -1;
	}
	return N->height;
}

// O(1)
// right rotation
void Rrotation(AVLTree *uT, AVLTreeNode *un)
{
	AVLTreeNode *b = un->left;
	// break and reconnect
	if (un == uT->root)
	{
		uT->root = b;
		b->parent = NULL;
	}
	else
	{
		if (un == un->parent->left)
		{
			un->parent->left = b;
		}
		else
		{
			un->parent->right = b;
		}
		b->parent = un->parent;
	}
	//rotate
	un->left = b->right;
	if (b->right != NULL)
	{
		b->right->parent = un;
	}
	b->right = un;
	un->parent = b;
	//recalculate the height
	un->height = max(Ht(un->left), Ht(un->right)) + 1;
	b->height = max(Ht(b->left), Ht(b->right)) + 1;
}

// O(1)
//left rotation
void Lrotation(AVLTree *uT, AVLTreeNode *un)
{
	AVLTreeNode *b = un->right;
	// break and reconnect
	if (un == uT->root)
	{
		uT->root = b;
		b->parent = NULL;
	}
	else
	{
		if (un == un->parent->left)
		{
			un->parent->left = b;
		}
		else
		{
			un->parent->right = b;
		}
		b->parent = un->parent;
	}
	//rotate
	un->right = b->left;
	if (b->left != NULL)
	{
		b->left->parent = un;
	}
	b->left = un;
	un->parent = b;
	//recalculate the height
	un->height = max(Ht(un->left), Ht(un->right)) + 1;
	b->height = max(Ht(b->left), Ht(b->right)) + 1;
}

// O(1)
//get difference of heights of its chidren
int difference(AVLTreeNode *N)
{
	return Ht(N->left) - Ht(N->right);
}

// O(log(n))
//rebalance the tree
void rebalance(AVLTree *ubT, AVLTreeNode *newN)
{
	AVLTreeNode *crt = newN;
	while (crt != NULL)
	{
		//recalculate the height
		// increaseH(crt);
		int diff;
		//get the difference of heights of left and right
		diff = difference(crt);
		//find first unbalanced ancestor
		if (diff < -1)
		{
			//right subtree heavy
			if (difference(crt->right) < 0)
			{
				Lrotation(ubT, crt);
			}
			else
			{
				//need double rotation
				Rrotation(ubT, crt->right);
				Lrotation(ubT, crt);
			}
		}
		else if (diff > 1)
		{
			//left subtree heavy
			if (difference(crt->left) > 0)
			{
				Rrotation(ubT, crt);
			}
			else
			{
				//need double rotation
				Lrotation(ubT, crt->left);
				Rrotation(ubT, crt);
			}
		}
		crt->height = max(Ht(crt->left), Ht(crt->right)) + 1;
		crt = crt->parent;
	}
}

// O(nlog(n))
// put your time complexity analysis of CreateAVLTree() here
AVLTree *CreateAVLTree(const char *filename)
{
	// create an empty tree
	AVLTree *tree;
	tree = newAVLTree();
	int t;
	char data_string[255];
	char data_pieces[255];
	if (strncmp(filename, "stdin", 5) == 0)
	{
		while (1)
		{
			// assgin the input string to the data_string line by line
			gets(data_pieces);
			// if it is a empty line, then the input is done, ending up reading from the standard input
			if (strlen(data_pieces) == 0)
			{
				break;
			}
			strcat(data_string, data_pieces);
		}
	}
	else
	{
		FILE *fp;
		fp = fopen(filename, "r");
		if (fp == NULL)
		{
			printf("Error occurs when opening file!");
			return NULL;
		}
		//iteratively read tuples until the end of the file
		while (1)
		{
			//reads string and stops when encounter white-space or new line
			fscanf(fp, "%s", data_pieces);
			//end of the file
			if (feof(fp))
			{
				break;
			}
			strcat(data_string, data_pieces);
		}
	}
	int *pp;
	pp = getKV(data_string);
	//pp[0] is the size of the array!!
	for (int k = 1; k <= pp[0]; k++)
	{
		//got the (key,value)!!
		t = InsertNode(tree, *(pp + k), *(pp + k + 1));
		//make sure the loop goes by step 2
		k++;
	}
	return tree;
}

//O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
	AVLTree *clone, *subL, *subR, *CL, *CR;
	int t;
	if (T->root == NULL)
	{
		return T;
	}
	clone = newAVLTree();
	subL = newAVLTree();
	subR = newAVLTree();
	AVLTreeNode *new = newAVLTreeNode(T->root->key, T->root->value);
	clone->root = new;
	new->height = T->root->height;
	subL->root = T->root->left;
	subR->root = T->root->right;
	CL = CloneAVLTree(subL);
	CR = CloneAVLTree(subR);
	if (CL->root != NULL)
	{
		CL->root->parent = new->left;
	}
	if (CR->root != NULL)
	{
		CR->root->parent = new->right;
	}
	new->left = CL->root;
	new->right = CR->root;
	return clone;
}

// O(n)
//get all nodes items from a tree
void getAllN(AVLTree *T, int *values)
{
	//pseudo stack (array of pointers)
	AVLTreeNode *stack[T->size];
	int size_of_stack = 0;
	int i = values[0] + 1;
	AVLTreeNode *crt = T->root;
	while (1)
	{
		//push the node
		if (crt != NULL)
		{
			stack[size_of_stack] = crt;
			size_of_stack++;
			crt = crt->left;
		}
		if (crt == NULL && size_of_stack > 0)
		{
			//fake poping the node
			size_of_stack--;
			values[i] = stack[size_of_stack]->key;
			values[i + 1] = stack[size_of_stack]->value;
			values[0] = values[0] + 2;
			i = i + 2;
			//go to the poped node's right chil
			crt = stack[size_of_stack]->right;
		}
		if (crt == NULL && size_of_stack == 0)
		{
			break;
		}
	}
}

// O((m+n)log(m+n))
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	int *values = (int *)malloc(255 * sizeof(int));
	values[0] = 0;
	//combine all iterms into one int array
	getAllN(T1, values);
	getAllN(T2, values);
	AVLTree *uniT = newAVLTree();
	for (int j = 1; j <= values[0]; j++)
	{
		int temp;
		temp = InsertNode(uniT, values[j], values[j + 1]);
		j++;
	}
	return uniT;
}

// O(m+n)
// get intersection of two sorted int arrays
int *getIntersction(int *valuesofT1, int *valuesofT2)
{
	int *valuesofinsrtT = (int *)malloc(255 * sizeof(int));
	int m = valuesofT1[0], n = valuesofT2[0];
	int j = 1;
	int k = 1;
	// get intersection
	for (int i = 1; i <= m; i = i + 2)
	{
		if (comparasion(valuesofT1[i], valuesofT1[i + 1], valuesofT2[j], valuesofT2[j + 1]) == 0)
		{
			valuesofinsrtT[k] = valuesofT1[i];
			valuesofinsrtT[k + 1] = valuesofT1[i + 1];
			valuesofinsrtT[0] = valuesofinsrtT[0] + 2;
			j = j + 2;
			k = k + 2;
		}
		else if (comparasion(valuesofT1[i], valuesofT1[i + 1], valuesofT2[j], valuesofT2[j + 1]) > 0)
		{
			for (; j <= n; j = j + 2)
			{
				if (comparasion(valuesofT1[i], valuesofT1[i + 1], valuesofT2[j], valuesofT2[j + 1]) == 0)
				{
					valuesofinsrtT[k] = valuesofT1[i];
					valuesofinsrtT[k + 1] = valuesofT1[i + 1];
					valuesofinsrtT[0] = valuesofinsrtT[0] + 2;
					k = k + 2;
				}
				else if (comparasion(valuesofT1[i], valuesofT1[i + 1], valuesofT2[j], valuesofT2[j + 1]) < 0)
				{
					break;
				}
			}
		}
		if (j > n)
		{
			break;
		}
	}
	return valuesofinsrtT;
}

// O(m+n+klog(k))
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	int *valuesofT1 = (int *)malloc(255 * sizeof(int));
	int *valuesofT2 = (int *)malloc(255 * sizeof(int));
	int *valuesofinsrtT = (int *)malloc(255 * sizeof(int));
	//get all vaules of two trees separately
	getAllN(T1, valuesofT1);
	getAllN(T2, valuesofT2);
	// get the intersection of these two sorted int arrays
	valuesofinsrtT = getIntersction(valuesofT1, valuesofT2);
	AVLTree *insrtT = newAVLTree();
	// create the intersected tree
	for (int k = 1; k <= valuesofinsrtT[0]; k = k + 2)
	{
		int temp;
		temp = InsertNode(insrtT, valuesofinsrtT[k], valuesofinsrtT[k + 1]);
	}
	return insrtT;
}

// O(log(n))
// return 0 if item exists in the tree, otherwise return 1.
int InsertNode(AVLTree *T, int k, int v)
{
	AVLTreeNode *newNode;
	newNode = newAVLTreeNode(k, v);
	// empty tree
	if (T->root == NULL)
	{
		T->root = newNode;
		T->size++;
		return 1;
	}
	AVLTreeNode *current = T->root;
	while (1)
	{
		if (comparasion(current->key, current->value, k, v) > 0)
		{
			if (current->left == NULL)
			{
				current->left = newNode;
				break;
			}
			else
			{
				current = current->left;
			}
		}
		else if (comparasion(current->key, current->value, k, v) == 0)
		{
			return 0;
		}
		else
		{
			if (current->right == NULL)
			{
				current->right = newNode;
				break;
			}
			else
			{
				current = current->right;
			}
		}
	}
	T->size++;
	newNode->parent = current;
	// T->root->height = max(T->root->left->height,T->root->right->height) + 1;
	// increaseH(newNode);
	rebalance(T, newNode);
	return 1;
}

// O(log(n))
//get the largest node
AVLTreeNode *getLgrstN(AVLTreeNode *nd)
{
	AVLTreeNode *crnt = nd;
	while (crnt->right != NULL)
	{
		crnt = crnt->right;
	}
	return crnt;
}

// O(log(n))
int DeleteNode(AVLTree *T, int k, int v)
{
	AVLTreeNode *dltN = Search(T, k, v);
	AVLTreeNode *rbStrtN;
	if (dltN == NULL)
	{
		return 0;
	}
	else
	{
		//dltN has no children
		if (dltN->left == NULL && dltN->right == NULL)
		{
			//dltN is the root
			if (dltN == T->root)
			{
				T->root = NULL;
			}
			else
			{
				if (dltN == dltN->parent->left)
				{
					//dltN is in the left
					dltN->parent->left = NULL;
				}
				else
				{
					//dltN is in the right
					dltN->parent->right = NULL;
				}
				rbStrtN = dltN->parent;
				dltN->parent = NULL;
			}
		}
		else if (dltN->left != NULL && dltN->right != NULL)
		{
			//dltN has two children
			if (dltN == T->root)
			{
				T->root = dltN->left;
				dltN->left->parent = NULL;
			}
			else
			{
				if (dltN == dltN->parent->left)
				{
					dltN->parent->left = dltN->left;
				}
				else
				{
					dltN->parent->right = dltN->left;
				}
				dltN->left->parent = dltN->parent;
				dltN->parent = NULL;
			}
			//find the largest node in the left part of dltN
			AVLTreeNode *RgtMost = getLgrstN(dltN->left);
			RgtMost->right = dltN->right;
			dltN->right->parent = RgtMost;
			dltN->left = NULL;
			dltN->right = NULL;
			rbStrtN = RgtMost;
		}
		else if (dltN->left != NULL)
		{
			//dltN has left child
			if (dltN == T->root)
			{
				T->root = dltN->left;
				dltN->left->parent = NULL;
			}
			else
			{
				if (dltN == dltN->parent->left)
				{
					dltN->parent->left = dltN->left;
				}
				else
				{
					dltN->parent->right = dltN->left;
				}
				dltN->left->parent = dltN->parent;
				dltN->parent = NULL;
			}
			rbStrtN = dltN->left->parent;
			dltN->left = NULL;
		}
		else
		{
			//dltN has right child
			if (dltN == T->root)
			{
				T->root = dltN->right;
				dltN->right->parent = NULL;
			}
			else
			{
				if (dltN == dltN->parent->left)
				{
					dltN->parent->left = dltN->right;
				}
				else
				{
					dltN->parent->right = dltN->right;
				}
				dltN->right->parent = dltN->parent;
				dltN->parent = NULL;
			}
			rbStrtN = dltN->right->parent;
			dltN->right = NULL;
		}
		free(dltN);
		T->size--;
	}
	// //recalculate the height of all the ancestors
	// increaseH(rbStrtN);
	rebalance(T, rbStrtN);
	return 1;
}

// O(log(n))
// The tree is empty, so return NULL; return a node if it finds the item, otherwise, return NULL.
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
	if (T->root == NULL)
	{
		// The tree is empty, so return NULL.
		return NULL;
	}
	else
	{
		AVLTreeNode *crnt = T->root;
		// return a node if it finds the item, otherwise, return NULL.
		while (crnt != NULL)
		{
			if (comparasion(crnt->key, crnt->value, k, v) == 0)
			{
				return crnt;
			}
			else if (comparasion(crnt->key, crnt->value, k, v) > 0)
			{
				crnt = crnt->left;
			}
			else
			{
				crnt = crnt->right;
			}
		}
		return NULL;
	}
}
// O(n)
void FreeAVLTree(AVLTree *T)
{
	// put your code here
	AVLTree *subLeft, *subRight;
	subLeft = newAVLTree();
	subRight = newAVLTree();
	if (T->root->left != NULL)
	{
		subLeft->root = T->root->left;
		T->root->left->parent = NULL;
		T->root->left = NULL;
		FreeAVLTree(subLeft);
	}
	if (T->root->right != NULL)
	{
		subRight->root = T->root->right;
		T->root->right->parent = NULL;
		T->root->right = NULL;
		FreeAVLTree(subRight);
	}
	T->size = 0;
	free(T->root);
}

// O(n)
void PrintAVLTree(AVLTree *T)
{
	// put your code here
	AVLTreeNode *current = T->root;
	AVLTree *subL, *subR;
	subL = newAVLTree();
	subR = newAVLTree();
	if (current->left != NULL)
	{
		subL->root = current->left;
		PrintAVLTree(subL);
	}
	printf("(%d,%d),%d\n", current->key, current->value, current->height);
	if (current->right != NULL)
	{
		subR->root = current->right;
		PrintAVLTree(subR);
	}
}

//sample main for testing
int main()
{
	int i, j;
	AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
	AVLTreeNode *node1;

	// tree1 = CreateAVLTree("File1.txt");
	// PrintAVLTree(tree1);
	// FreeAVLTree(tree1);
	//you need to create the text file file1.txt
	// to store a set of items without duplicate items
	tree2 = CreateAVLTree("file1.txt");
	PrintAVLTree(tree2);
	tree3 = CloneAVLTree(tree2);
	puts("-----tree3--------");
	PrintAVLTree(tree3);
	puts("-----------------");
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
	PrintAVLTree(tree4);
	node1 = Search(tree4, 20, 20);
	if (node1 != NULL)
		printf("key= %d value= %d\n", node1->key, node1->value);
	else
		printf("Key 20 does not exist\n");
	for (i = 17; i > 0; i--)
	{
		j = DeleteNode(tree4, i, i);
		if (j == 0)
			printf("Key %d does not exist\n", i);
		PrintAVLTree(tree4);
	}
	FreeAVLTree(tree4);
	//  Create tree5
	tree5 = newAVLTree();
	j = InsertNode(tree5, 6, 25);
	j = InsertNode(tree5, 6, 10);
	j = InsertNode(tree5, 6, 12);
	j = InsertNode(tree5, 6, 20);
	j = InsertNode(tree5, 9, 25);
	j = InsertNode(tree5, 10, 25);
	PrintAVLTree(tree5);
	//Create tree6
	tree6 = newAVLTree();
	j = InsertNode(tree6, 6, 25);
	j = InsertNode(tree6, 5, 10);
	j = InsertNode(tree6, 6, 12);
	j = InsertNode(tree6, 6, 20);
	j = InsertNode(tree6, 8, 35);
	j = InsertNode(tree6, 10, 25);
	PrintAVLTree(tree6);
	// tree7 = AVLTreesIntersection(tree5, tree6);
	tree8 = AVLTreesUnion(tree5, tree6);
	// puts("terererererererereee7777777777");
	// PrintAVLTree(tree7);
	puts("terererererererereee888888888");
	PrintAVLTree(tree8);
	return 0;
}