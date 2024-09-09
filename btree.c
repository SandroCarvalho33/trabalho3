#include <stdio.h>
#include <stdlib.h>

#define ORDER 5 // Ordem da árvore B

// Estrutura para o nó da Árvore B
typedef struct BTreeNode {
    int keys[ORDER - 1];  // Chaves
    struct BTreeNode *children[ORDER]; // Ponteiros para os filhos
    int n; // Número de chaves atualmente armazenadas no nó
    int leaf; // Se o nó é uma folha (1) ou não (0)
} BTreeNode;

// Estrutura da Árvore B
typedef struct BTree {
    BTreeNode *root; // Ponteiro para a raiz
} BTree;

// Função para criar um novo nó da Árvore B
BTreeNode* createNode(int leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->leaf = leaf;
    node->n = 0; // Inicialmente, o nó não tem chaves
    for (int i = 0; i < ORDER; i++) {
        node->children[i] = NULL; // Inicializa os ponteiros dos filhos como NULL
    }
    return node;
}

// Função para criar uma nova árvore B
BTree* createTree() {
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    tree->root = createNode(1); // A raiz começa como uma folha
    return tree;
}

// Função para dividir um nó que está cheio
void splitChild(BTreeNode *parent, int i, BTreeNode *fullNode) {
    BTreeNode *newNode = createNode(fullNode->leaf);
    newNode->n = ORDER / 2 - 1;

    // Move as chaves de fullNode para newNode
    for (int j = 0; j < ORDER / 2 - 1; j++) {
        newNode->keys[j] = fullNode->keys[j + ORDER / 2];
    }

    // Se o nó não for folha, mova os filhos
    if (!fullNode->leaf) {
        for (int j = 0; j < ORDER / 2; j++) {
            newNode->children[j] = fullNode->children[j + ORDER / 2];
        }
    }

    fullNode->n = ORDER / 2 - 1;

    // Insere o novo nó no pai
    for (int j = parent->n; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = newNode;

    // Insere a chave no pai
    for (int j = parent->n - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = fullNode->keys[ORDER / 2 - 1];
    parent->n++;
}

// Função para inserir uma nova chave quando o nó não está cheio
void insertNonFull(BTreeNode *node, int key) {
    int i = node->n - 1;

    if (node->leaf) {
        // Encontra a posição correta para a nova chave
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        // Encontra o filho onde a nova chave deve ser inserida
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        
        // Se o filho estiver cheio, divida
        if (node->children[i]->n == ORDER - 1) {
            splitChild(node, i, node->children[i]);

            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// Função para inserir uma chave na árvore
void insert(BTree *tree, int key) {
    BTreeNode *root = tree->root;

    if (root->n == ORDER - 1) {
        BTreeNode *newRoot = createNode(0);
        newRoot->children[0] = root;
        splitChild(newRoot, 0, root);

        int i = 0;
        if (newRoot->keys[0] < key) {
            i++;
        }
        insertNonFull(newRoot->children[i], key);

        tree->root = newRoot;
    } else {
        insertNonFull(root, key);
    }
}

// Função auxiliar para imprimir a árvore
void printTree(BTreeNode *node, int level) {
    if (node != NULL) {
        for (int i = 0; i < node->n; i++) {
            if (!node->leaf) {
                printTree(node->children[i], level + 1);
            }
            printf(" %d", node->keys[i]);
        }
        if (!node->leaf) {
            printTree(node->children[node->n], level + 1);
        }
    }
}

// Função principal para teste
int main() {
    BTree *tree = createTree();

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        insert(tree, keys[i]);
    }

    printf("Árvore B após inserção das chaves:\n");
    printTree(tree->root, 0);
    printf("\n");

    return 0;
}
