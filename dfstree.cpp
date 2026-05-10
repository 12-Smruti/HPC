#include<iostream>
#include<stack>
#include<omp.h>
#include<queue>

using namespace std;

//================ NODE CLASS =================
class Node
{
public:
    int data;
    Node *left, *right;

    Node(int val)
    {
        data = val;
        left = right = NULL;
    }
};

//================ TREE CLASS =================
class TreeDFS
{
public:
    Node* insert(Node*, int);
    void displayTree(Node*);
    void dfs(Node*);
};

//================ INSERT FUNCTION =================
Node* TreeDFS::insert(Node* root, int data)
{
    // Create first node
    if(root == NULL)
    {
        root = new Node(data);
        return root;
    }

    // Queue for level order insertion
    queue<Node*>q;
    q.push(root);

    while(!q.empty())
    {
        Node* temp = q.front();
        q.pop();

        // Insert left child
        if(temp->left == NULL)
        {
            temp->left = new Node(data);
            return root;
        }
        else
        {
            q.push(temp->left);
        }

        // Insert right child
        if(temp->right == NULL)
        {
            temp->right = new Node(data);
            return root;
        }
        else
        {
            q.push(temp->right);
        }
    }

    return root;
}

//================ DISPLAY TREE =================
void TreeDFS::displayTree(Node* root)
{
    if(root == NULL)
        return;

    queue<Node*> q;
    q.push(root);

    cout << "\nTree Structure:\n\n";

    while(!q.empty())
    {
        int size = q.size();

        for(int i = 0; i < size; i++)
        {
            Node* temp = q.front();
            q.pop();

            cout << temp->data << " ";

            if(temp->left)
                q.push(temp->left);

            if(temp->right)
                q.push(temp->right);
        }

        cout << endl;
    }
}

//================ PARALLEL DFS =================
void TreeDFS::dfs(Node* root)
{
    if(root == NULL)
        return;

    stack<Node*> s;

    s.push(root);

    cout << "\nDFS Traversal of Tree:\n";

    while(!s.empty())
    {
        Node* curr = s.top();
        s.pop();

        cout << curr->data << " ";

        // Parallel sections for left and right subtree
#pragma omp parallel sections
        {
#pragma omp section
            {
                if(curr->right)
                {
#pragma omp critical
                    s.push(curr->right);
                }
            }

#pragma omp section
            {
                if(curr->left)
                {
#pragma omp critical
                    s.push(curr->left);
                }
            }
        }
    }

    cout << endl;
}

//================ MAIN FUNCTION =================
int main()
{
    TreeDFS obj;

    Node* root = NULL;

    int data;
    char ans;

    // Input nodes
    do
    {
        cout << "\nEnter data: ";
        cin >> data;

        root = obj.insert(root, data);

        cout << "Do you want to insert another node? (y/n): ";
        cin >> ans;

    } while(ans == 'y' || ans == 'Y');

    // Display Tree
    obj.displayTree(root);

    // DFS Traversal
    obj.dfs(root);

    return 0;
}
