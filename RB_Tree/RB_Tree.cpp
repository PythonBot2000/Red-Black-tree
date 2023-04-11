#include <iostream>
#include <Windows.h>

using namespace std;

enum NodeColor { Black, Red };

typedef int T;

struct Node
{
	Node* right;
	Node* left;
	Node* parent;
	NodeColor color;
	T key;
};

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

void rotateLeft(Node *&root, Node* x)
{
	Node* y = x->right;
	if (y)
	{
		x->right = y->left;
		y->parent = x->parent;

		if (y->left) y->left->parent = x;

		if (x->parent)
		{
			if (x == x->parent->left)
			{
				x->parent->left = y;
			}
			else
			{
				x->parent->right = y;
			}
		}
		else
		{
			root = y;
		}

		y->left = x;
		x->parent = y;
	}
}

void rotateRight(Node *&root, Node* x)
{
	Node* y = x->left;
	if (y)
	{
		x->left = y->right;
		y->parent = x->parent;

		if (y->right) y->right->parent = x;

		if (x->parent)
		{
			if (x == x->parent->right)
			{
				x->parent->right = y;
			}
			else
			{
				x->parent->left = y;
			}
		}
		else
		{
			root = y;
		}

		y->right = x;
		x->parent = y;
	}
}

void insertFixup(Node *&root, Node* x)
{
	while (x != root && x->parent->color == Red) {
		if (x->parent == x->parent->parent->left) {
			Node* y = x->parent->parent->right;
			if ((y) && (y->color == Red)) {
				x->parent->color = Black;
				y->color = Black;
				x->parent->parent->color = Red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->right) {
					x = x->parent;
					rotateLeft(root, x);
				}

				x->parent->color = Black;
				x->parent->parent->color = Red;
				rotateRight(root, x->parent->parent);
			}
		}
		else {
			Node* y = x->parent->parent->left;
			if ((y) && (y->color == Red)) {
				x->parent->color = Black;
				y->color = Black;
				x->parent->parent->color = Red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->left) {
					x = x->parent;
					rotateRight(root, x);
				}
				x->parent->color = Black;
				x->parent->parent->color = Red;
				rotateLeft(root, x->parent->parent);
			}
		}
	}
	root->color = Black;
}

Node* insertNode(Node *&root, T key)
{
	Node* current, * parent;
	parent = NULL;
	current = root;
	while (current)
	{
		if (key == current->key) return current;
		parent = current;
		current = key < current->key ? current->left : current->right;
	}
	Node* x = new Node;
	x->key = key;
	x->parent = parent;
	x->left = NULL;
	x->right = NULL;
	x->color = Red;

	if (parent)
	{
		if (key < parent->key)
		{
			parent->left = x;
		}
		else
		{
			parent->right = x;
		}
	}
	else
	{
		root = x;
	}
	insertFixup(root, x);
	return(x);
}

Node* findNode(Node* root, T key)
{
	Node* current = root;
	while (current)
	{
		if (key == current->key)
		{
			return (current);
		}
		else
		{
			current = key < current->key ? current->left : current->right;
		}
	}
	return(NULL);
}

void deleteFixup(Node *root, Node* x, Node* xParent, bool xSideLeft)
{
	while (x != root && (!x || x->color == Black))
	{
		if (xSideLeft)
		{
			Node* w = xParent->right;
			if (w && w->color == Red)
			{
				w->color = Black;
				xParent->color = Red;
				rotateLeft(root, xParent);
				w = xParent->right;
			}

			bool wLeftIsBlack = true;
			bool wRightIsBlack = true;
			if (!w || (!w->left || w->left->color == Black)) wLeftIsBlack = true;
			else wLeftIsBlack = false;
			if (!w || (!w->right || w->right->color == Black)) wRightIsBlack = true;
			else wRightIsBlack = false;

			if (wLeftIsBlack && wRightIsBlack)
			{
				if (w) w->color = Red;
				x = xParent;
				xParent = x->parent;
			}
			else
			{
				if (wRightIsBlack)
				{
					w->left->color = Black;
					w->color = Red;
					rotateRight(root, w);
					w = xParent->right;
				}
				w->color = xParent->color;
				xParent->color = Black;
				if (w->right) w->right->color = Black;
				rotateLeft(root, xParent);
				x = root;
			}
		}
		else
		{
			Node* w = xParent->left;
			if (w && (w->color == Red))
			{
				w->color = Black;
				xParent->color = Red;
				rotateRight(root, xParent);
				w = xParent->left;
			}

			bool wLeftIsBlack = true;
			bool wRightIsBlack = true;
			if (!w || (!w->left || w->left->color == Black)) wLeftIsBlack = true;
			else wLeftIsBlack = false;
			if (!w || (!w->right || w->right->color == Black)) wRightIsBlack = true;
			else wRightIsBlack = false;

			if (wLeftIsBlack && wRightIsBlack)
			{
				if (w) w->color = Red;
				x = xParent;
				xParent = x->parent;
			}
			else
			{
				if (wLeftIsBlack)
				{
					w->right->color = Black;
					w->color = Red;
					rotateLeft(root, w);
					w = xParent->left;
				}
				w->color = xParent->color;
				xParent->color = Black;
				if (w->left) w->left->color = Black;
				rotateRight(root, xParent);
				x = root;
			}
		}

		if (x != root)
		{
			if (x == xParent->left)
			{
				xSideLeft = true;
			}
			else
			{
				xSideLeft = false;
			}
		}
	}
	x->color = Black;
	root = x;
}

void deleteNode(Node *root, T key)
{
	Node* x, * y, * z;

	z = findNode(root, key);

	if (!z) return;

	if (!z->left || !z->right)
	{
		y = z;
	}
	else
	{
		y = z->right;
		while (y->left) y = y->left;
	}

	if (y->left)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}

	if (x) x->parent = y->parent;
	bool xSideLeft = true;
	if (y->parent)
	{
		if (y == y->parent->left)
		{
			xSideLeft = true;
			y->parent->left = x;
		}
		else
		{
			xSideLeft = false;
			y->parent->right = x;
		}
	}
	else
	{
		root = x;
	}

	if (y != z) z->key = y->key;

	if (y->color == Black) deleteFixup(root, x, y->parent, xSideLeft);

	free(y);
}

void DeleteTree(Node* node)
{
	if (node)
	{
		if (node->left) DeleteTree(node->left);
		if (node->right) DeleteTree(node->right);
		delete node;
	}
}

void TreeWalk(Node* node, int &count)
{
	if (!node) return;
	count++;
	TreeWalk(node->left, count);
	TreeWalk(node->right, count);
}

void FindMaxSubTreeNodes(Node* root)
{
	if (root)
	{
		int leftCount = 0;
		int rightCount = 0;
		TreeWalk(root->left, leftCount);
		TreeWalk(root->right, rightCount);
		cout << "Left nodes: " << leftCount << endl;
		cout << "Right nodes: " << rightCount << endl;
		
		if (leftCount > rightCount)
		{
			cout << "Left sutree is larger then the right one" << endl;
		}
		else if (leftCount == rightCount)
		{
			cout << "They are equal";
		}
		else
		{
			cout << "Right sutree is larger then the left one" << endl;
		}
	}
}

void PrintTree(Node* snode, int l)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	Node* node = snode;
	if (node != NULL)
	{
		PrintTree(node->right, l + 1);
		for (int i = 0; i < l; i++) cout << "     ";
		if (node->color == Red)
		{
			SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else
		{
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
		}
		cout << node->key;
		PrintTree(node->left, l + 1);
	}
	else
	{
		cout << endl;
	}
	SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

int main()
{   
	Node* root1 = NULL;

	insertNode(root1, 5);
	insertNode(root1, 2);
	insertNode(root1, 7);
	insertNode(root1, 4);
	insertNode(root1, 10);
	insertNode(root1, 1);
	insertNode(root1, 12);
	insertNode(root1, 13);
	insertNode(root1, 2);

	deleteNode(root1, 13);
	insertNode(root1, 3);

	PrintTree(root1, 1);

	FindMaxSubTreeNodes(root1);
	return 0;
}