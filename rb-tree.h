struct node {
    int key;
    node *parent;
    char color;
    node *left;
    node *right;
};

class RBtree
{
    public: node *root;
    int size;

    private:
    void delfix(node *);
    void insertfix(node *);
    node* successor(node *);

    public :
    RBtree()
    {
        root = NULL;
        size = 0;
    }
    void insert(int);
    void leftrotate(node *);
    void rightrotate(node *);
    void del(int);
    bool search(int);
};

const bool RED = true;
const bool BLACK = false;