#include<iostream>
#include<stdlib.h>
#include<queue>
#include<omp.h>

using namespace std;

//================ NODE CLASS =================
class node
{
public:
    int data;
    node *left, *right;

    node(int val)
    {
        data = val;
        left = right = NULL;
    }
};

//================ TREE CLASS =================
class Breadthfs
{
public:
    node *insert(node *, int);
    void bfs(node *);
    void printTree(node *);
};

//================ INSERT FUNCTION =================
node *Breadthfs::insert(node *root, int data)
{
    // If tree is empty
    if(root == NULL)
    {
        root = new node(data);
        return root;
    }

    queue<node*> q;
    q.push(root);

    while(!q.empty())
    {
        node *temp = q.front();
        q.pop();

        // Insert at left
        if(temp->left == NULL)
        {
            temp->left = new node(data);
            return root;
        }
        else
        {
            q.push(temp->left);
        }

        // Insert at right
        if(temp->right == NULL)
        {
            temp->right = new node(data);
            return root;
        }
        else
        {
            q.push(temp->right);
        }
    }

    return root;
}

//================ PARALLEL BFS =================
void Breadthfs::bfs(node *head)
{
    if(head == NULL)
        return;

    queue<node*> q;
    q.push(head);

    cout << "\n\nBFS Traversal:\n";

    while(!q.empty())
    {
        int qSize = q.size();

        // Process one complete level
        #pragma omp parallel for
        for(int i = 0; i < qSize; i++)
        {
            node *currNode;

            // Critical section for queue access
            #pragma omp critical
            {
                currNode = q.front();
                q.pop();

                cout << currNode->data << " ";
            }

            // Push children safely
            #pragma omp critical
            {
                if(currNode->left)
                    q.push(currNode->left);

                if(currNode->right)
                    q.push(currNode->right);
            }
        }

        cout << endl; // New line after every level
    }
}

//================ TREE STRUCTURE PRINT =================
void Breadthfs::printTree(node *root)
{
    if(root == NULL)
        return;

    queue<node*> q;
    q.push(root);

    cout << "\nTree Structure:\n\n";

    while(!q.empty())
    {
        int size = q.size();

        for(int i = 0; i < size; i++)
        {
            node *temp = q.front();
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

//================ MAIN FUNCTION =================
int main()
{
    Breadthfs obj;

    node *root = NULL;

    int data;
    char ans;

    // Input nodes
    do
    {
        cout << "\nEnter data => ";
        cin >> data;

        root = obj.insert(root, data);

        cout << "Do you want to insert one more node? (y/n): ";
        cin >> ans;

    }while(ans == 'y' || ans == 'Y');

    // Print Tree Structure
    obj.printTree(root);

    // BFS Traversal
    obj.bfs(root);

    return 0;
}
