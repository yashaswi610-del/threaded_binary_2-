#include <stdio.h>
#include <stdlib.h>

// Node structure
struct Node {
    int key;
    struct Node *left, *right;
    int lthread, rthread;  // flags: 1 = thread, 0 = child
};

// Create new node
struct Node* newNode(int key) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->key = key;
    temp->lthread = 1;
    temp->rthread = 1;
    temp->left = temp->right = NULL;
    return temp;
}

// Insert into threaded binary tree
struct Node* insert(struct Node* root, int key) {
    struct Node* parent = NULL;
    struct Node* curr = root;

    while (curr != NULL) {
        if (key == curr->key) {
            printf("Duplicate key not allowed!\n");
            return root;
        }
        parent = curr;
        if (key < curr->key) {
            if (curr->lthread == 0)
                curr = curr->left;
            else break;
        } else {
            if (curr->rthread == 0)
                curr = curr->right;
            else break;
        }
    }

    struct Node* temp = newNode(key);

    if (parent == NULL) {
        root = temp;
    }
    else if (key < parent->key) {
        temp->left = parent->left;
        temp->right = parent;
        parent->lthread = 0;
        parent->left = temp;
    }
    else {
        temp->right = parent->right;
        temp->left = parent;
        parent->rthread = 0;
        parent->right = temp;
    }
    return root;
}

// Inorder traversal using threads
void inorder(struct Node* root) {
    if (root == NULL) return;
    struct Node* curr = root;
    while (curr->lthread == 0)
        curr = curr->left;

    while (curr != NULL) {
        printf("%d ", curr->key);
        if (curr->rthread == 1)
            curr = curr->right;
        else {
            curr = curr->right;
            while (curr != NULL && curr->lthread == 0)
                curr = curr->left;
        }
    }
}

// Helper: inorder successor
struct Node* inorderSuccessor(struct Node* ptr) {
    if (ptr->rthread == 1)
        return ptr->right;
    ptr = ptr->right;
    while (ptr->lthread == 0)
        ptr = ptr->left;
    return ptr;
}

// Delete a node
struct Node* deleteNode(struct Node* root, int key) {
    struct Node* parent = NULL;
    struct Node* curr = root;
    int found = 0;

    // Search for node
    while (curr != NULL) {
        if (key == curr->key) { found = 1; break; }
        parent = curr;
        if (key < curr->key) {
            if (curr->lthread == 0) curr = curr->left;
            else break;
        } else {
            if (curr->rthread == 0) curr = curr->right;
            else break;
        }
    }

    if (!found) {
        printf("Key %d not found!\n", key);
        return root;
    }

    // Case 1: Node has two children
    if (curr->lthread == 0 && curr->rthread == 0) {
        struct Node* succ = inorderSuccessor(curr);
        int val = succ->key;
        root = deleteNode(root, val);
        curr->key = val;
    }
    // Case 2 & 3: Node has 0 or 1 child
    else {
        struct Node* child;
        if (curr->lthread == 0) child = curr->left;
        else if (curr->rthread == 0) child = curr->right;
        else child = NULL;

        if (parent == NULL) {
            root = child;
        }
        else if (curr == parent->left) {
            if (curr->lthread == 1 && curr->rthread == 1) {
                parent->lthread = 1;
                parent->left = curr->left;
            } else {
                parent->left = child;
            }
        }
        else {
            if (curr->lthread == 1 && curr->rthread == 1) {
                parent->rthread = 1;
                parent->right = curr->right;
            } else {
                parent->right = child;
            }
        }
        free(curr);
    }
    return root;
}

// Driver code
int main() {
    struct Node* root = NULL;

    root = insert(root, 20);
    root = insert(root, 10);
    root = insert(root, 30);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 25);
    root = insert(root, 35);

    printf("Inorder before deletion:\n");
    inorder(root);

    root = deleteNode(root, 10);
    printf("\nInorder after deleting 10:\n");
    inorder(root);

    root = deleteNode(root, 30);
    printf("\nInorder after deleting 30:\n");
    inorder(root);

    return 0;
}
