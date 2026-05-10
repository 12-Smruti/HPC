#include<iostream>
#include<queue>
#include<stack>
#include<omp.h>

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
class TreeTraversal
{
public:
    Node* insert(Node*, int);

    void displayTree(Node*);

    void bfs(Node*);

    void dfs(Node*);
};

//================ INSERT FUNCTION =================
Node* TreeTraversal::insert(Node* root, int data)
{
    // Create first node
    if(root == NULL)
    {
        root = new Node(data);
        return root;
    }

    // Queue for level order insertion
    queue<Node*> q;
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
void TreeTraversal::displayTree(Node* root)
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

//================ PARALLEL BFS =================
void TreeTraversal::bfs(Node* root)
{
    if(root == NULL)
        return;

    queue<Node*> q;
    q.push(root);

    cout << "\nBFS Traversal:\n";

    while(!q.empty())
    {
        int qSize = q.size();

        #pragma omp parallel for
        for(int i = 0; i < qSize; i++)
        {
            Node* curr;

            // Critical section for queue access
            #pragma omp critical
            {
                curr = q.front();
                q.pop();

                cout << curr->data << " ";
            }

            // Push child nodes safely
            #pragma omp critical
            {
                if(curr->left)
                    q.push(curr->left);

                if(curr->right)
                    q.push(curr->right);
            }
        }

        cout << endl;
    }
}

//================ PARALLEL DFS =================
void TreeTraversal::dfs(Node* root)
{
    if(root == NULL)
        return;

    stack<Node*> s;

    s.push(root);

    cout << "\nDFS Traversal:\n";

    while(!s.empty())
    {
        Node* curr = s.top();
        s.pop();

        cout << curr->data << " ";

        // Parallel sections
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
    TreeTraversal obj;

    Node* root = NULL;

    int data;
    char ans;

    // Input tree nodes
    do
    {
        cout << "\nEnter data: ";
        cin >> data;

        root = obj.insert(root, data);

        cout << "Do you want to insert another node? (y/n): ";
        cin >> ans;

    }while(ans == 'y' || ans == 'Y');

    // Display Tree
    obj.displayTree(root);

    // BFS Traversal
    obj.bfs(root);

    // DFS Traversal
    obj.dfs(root);

    return 0;
}
