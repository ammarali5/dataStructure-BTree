#include <iostream>
using namespace std;

template <typename T, int Order>
class BTree_Node {
public:
	int n;
	T keys[Order - 1];
	BTree_Node<T, Order>* children[Order];
	bool leaf;

	BTree_Node(bool isLeaf = true) {
		n = 0;
		leaf = isLeaf;
		for (int i = 0; i < Order - 1; i++) keys[i] = T();
		for (int i = 0; i < Order; i++) children[i] = nullptr;
	}
};

template <typename T, int Order>
class BTree {
private:
	BTree_Node<T, Order>* root;

	void printNode(BTree_Node<T, Order>* x, int depth) const {
		if (!x) return;
		for (int s = 0; s < depth * 2; ++s) cout << ' ';
		for (int i = 0; i < x->n; ++i) {
			cout << x->keys[i];
			if (i + 1 < x->n) cout << ',';
		}
		cout << '\n';
		if (!x->leaf) {
			for (int i = 0; i <= x->n; ++i)
				printNode(x->children[i], depth + 1);
		}
	}

	void splitChild(BTree_Node<T, Order>* parent, int i, BTree_Node<T, Order>* y) {
		int t = (Order - 1) / 2; // Pivot index

		BTree_Node<T, Order>* z = new BTree_Node<T, Order>(y->leaf);
		z->n = y->n - t - 1;

		// نسخ المفاتيح اليمنى إلى z
		for (int j = 0; j < z->n; j++)
			z->keys[j] = y->keys[j + t + 1];

		// نسخ الأبناء اليمنى إذا لم تكن ورقية
		if (!y->leaf) {
			for (int j = 0; j <= z->n; j++)
				z->children[j] = y->children[j + t + 1];
		}

		y->n = t; // تحديث عدد المفاتيح في y

		// تحريك أبناء parent لإفساح مكان للطفل الجديد
		for (int j = parent->n; j >= i + 1; j--)
			parent->children[j + 1] = parent->children[j];

		parent->children[i + 1] = z;

		// تحريك مفاتيح parent لإفساح مكان للمفتاح الأوسط
		for (int j = parent->n - 1; j >= i; j--)
			parent->keys[j + 1] = parent->keys[j];

		parent->keys[i] = y->keys[t]; // رفع المفتاح الأوسط
		parent->n++;
	}

	void insertNonFull(BTree_Node<T, Order>* x, T k) {
		int i = x->n - 1;
		if (x->leaf) {
			while (i >= 0 && x->keys[i] > k) {
				x->keys[i + 1] = x->keys[i];
				i--;
			}
			x->keys[i + 1] = k;
			x->n++;
		}
		else {
			while (i >= 0 && x->keys[i] > k) i--;
			i++;
			if (x->children[i]->n == Order - 1) {
				splitChild(x, i, x->children[i]);
				if (k > x->keys[i]) i++;
			}
			insertNonFull(x->children[i], k);
		}
	}

public:
	BTree() {
		root = new BTree_Node<T, Order>(true);
	}

	void Insert(T k) {
		if (root->n == Order - 1) {
			BTree_Node<T, Order>* s = new BTree_Node<T, Order>(false);
			s->children[0] = root;
			splitChild(s, 0, root);

			int i = (k > s->keys[0]) ? 1 : 0;
			insertNonFull(s->children[i], k);

			root = s;
		}
		else {
			insertNonFull(root, k);
		}
	}

	void Print() const {
		printNode(root, 0);
	}
};


int main() {
	// BTree order 3 (int)
	BTree<int, 3> t1;

	t1.Insert(1);
	t1.Insert(5);
	t1.Insert(0);
	t1.Insert(4);
	t1.Insert(3);
	t1.Insert(2);

	t1.Print();

	/*
	Expected:
	1,4
	  0
	  2,3
	  5
	*/

	// BTree order 5 (char)
	BTree<char, 5> t;

	t.Insert('G');
	t.Insert('I');
	t.Insert('B');
	t.Insert('J');
	t.Insert('C');
	t.Insert('A');
	t.Insert('K');
	t.Insert('E');
	t.Insert('D');
	t.Insert('S');
	t.Insert('T');
	t.Insert('R');
	t.Insert('L');
	t.Insert('F');
	t.Insert('H');
	t.Insert('M');
	t.Insert('N');
	t.Insert('P');
	t.Insert('Q');

	t.Print();

	/*
	Expected:
	K
	  C,G
		A,B
		D,E,F
		H,I,J
	  N,R
		L,M
		P,Q
		S,T
	*/

	return 0;
}
