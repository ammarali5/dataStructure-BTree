// 20230246 : ammar ali ghazi afifi
// 20230100 : peter gerges sayed sbaa

// Allowed headers only.
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

// Template for BTree nodes.
// T = data tybe of tree content
// Order = maximum number of children per node, Max keys = Order-1
// We allocate arrays sized to allow a temporary overflow during insert:
//  - keys array size: Order (allows max keys = Order-1 normally, plus 1 temporarily)
//  - children array size: Order+1 (maximum children when keys temporarily overflow)
template <typename T, int Order>
class BTreeNode
{
  public:
    int n;                                // number of keys currently stored in node
    T keys[Order];                        // keys array (can temporarily hold one extra key during split)
    BTreeNode* children[Order + 1];       // children pointers (Order+1 possible)
    BTreeNode* parent;                    // parent pointer to simplify upward splits
    bool leaf;                            // whether node is leaf
    
    // constructor: initialize empty node as leaf, no keys
    BTreeNode(bool isLeaf = true)
    {
        n = 0;
        leaf = isLeaf;
        parent = NULL;

        // initialize children pointers to NULL
        for (int i = 0; i < Order + 1; ++i) children[i] = NULL;
    }
};

// Template B-Treee
// T = data tybe of tree content
// Order = maximum number of children per node, Max keys = Order-1
template <typename T, int Order>
class BTree
{
  private:
    BTreeNode<T, Order>* root;                // root pointer
    
    // helper to create a new node
    BTreeNode<T, Order>* newNode(bool isLeaf)
    {
        BTreeNode<T, Order>* p = new BTreeNode<T, Order>(isLeaf);
        return p;
    }

    // split node 'x' which has overflowed (x->n == Order)
    // After split, median key is promoted into parent (or new root is created).
    void splitUp(BTreeNode<T, Order>* x)
    {
        // loop upwards while node has too many keys
        while (x && x->n >= Order) {
            // compute median index (floor)
            int mid = x->n / 2;
            // create new sibling node z copying keys after mid
            BTreeNode<T, Order>* z = newNode(x->leaf);
            // number of keys for z
            int zcount = x->n - mid - 1;
            // copy keys to z
            for (int i = 0; i < zcount; ++i)
            {
                z->keys[i] = x->keys[mid + 1 + i];
            }
            z->n = zcount;
            // if internal, move corresponding children
            if (!x->leaf)
            {
                for (int i = 0; i <= zcount; ++i)
                {
                    z->children[i] = x->children[mid + 1 + i];
                    if (z->children[i]) z->children[i]->parent = z;
                }
            }
            // reduce keys in x
            x->n = mid;
            // median key to promote
            T median = x->keys[mid];
            // parent pointer
            BTreeNode<T, Order>* p = x->parent;
            // if no parent, create new root
            if (!p)
            {
                p = newNode(false);
                root = p;
                p->children[0] = x;
                x->parent = p;
            }
            // find position in parent to insert median (index pos)
            int pos = 0;
            while (pos < p->n && !(median < p->keys[pos])) pos++;
            // shift parent's keys and children to make room
            for (int i = p->n; i > pos; --i)
            {
                p->keys[i] = p->keys[i - 1];
            }
            for (int i = p->n + 1; i > pos + 1; --i)
            {
                p->children[i] = p->children[i - 1];
            }
            // insert median and new child pointer
            p->keys[pos] = median;
            p->children[pos + 1] = z;
            z->parent = p;
            p->n += 1;
            // continue upward in case parent overflowed
            x = p;
        }
    }
    
    // Recursive print function. depth controls indentation (2 spaces per depth).
    void printNode(BTreeNode<T, Order>* node, int depth)
    {
        if (!node) return;
        // print indentation
        for (int i = 0; i < depth * 2; ++i) cout << ' ';
        // print keys separated by commas, no trailing comma
        for (int i = 0; i < node->n; ++i)
        {
            cout << node->keys[i];
            if (i + 1 < node->n) cout << ',';
        }
        cout << '\n';
        // if not leaf, recurse on children left-to-right
        if (!node->leaf)
        {
            for (int i = 0; i <= node->n; ++i)
            {
                printNode(node->children[i], depth + 1);
            }
        }
    }

  public:
    // constructor: start with an empty leaf root
    BTree()
    {
        root = newNode(true);
    }

    // Insert key into B-tree
    void Insert(const T& key)
    {
        // find leaf where key should be inserted
        BTreeNode<T, Order>* cur = root;
        while (!cur->leaf)
        {
            int i = 0;
            // find child index
            while (i < cur->n && !(key < cur->keys[i])) i++;
            // move to child i
            cur = cur->children[i];
        }
        // insert key into leaf cur at correct position
        int i = cur->n - 1;
        // shift keys to make space
        while (i >= 0 && (key < cur->keys[i]))
        {
            cur->keys[i + 1] = cur->keys[i];
            --i;
        }
        cur->keys[i + 1] = key;
        cur->n += 1;
        // if overflow (we allow temporary up to Order keys), split upward
        if (cur->n >= Order) splitUp(cur);
    }

    // Print tree
    void Print()
    {
        if (!root) return;
        printNode(root, 0);
    }
};

int main()
{
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:

    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
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

    t.Print(); // Should output the following on the screen:

    /*
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
