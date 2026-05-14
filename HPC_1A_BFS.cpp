#include <iostream>
#include <queue>
#include <omp.h> // REQUIRED for OpenMP [cite: 94]

using namespace std;

struct Node {
    int data;
    Node *left, *right;
    Node(int val) {
        data = val;
        left = right = NULL;
    }
};

// Manual tree creation for quick testing
Node* createTree() {
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    return root;
}

void parallelBFS(Node* root) {
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();

        // Level-by-level parallel processing [cite: 111, 115]
        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            Node* curr;

            // Use 'critical' to ensure thread-safe access to the shared queue [cite: 125]
            #pragma omp critical
            {
                curr = q.front();
                q.pop();
                cout << curr->data << " ";
            }

            // Add child nodes to the queue for the next level [cite: 37, 45]
            #pragma omp critical
            {
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
        }
    }
}

int main() {
    Node* root = createTree();
    
    // Setting thread count (Optional but good for Orals)
    omp_set_num_threads(4); 

    cout << "Parallel BFS Traversal: ";
    parallelBFS(root);
    cout << endl;

    return 0;
}