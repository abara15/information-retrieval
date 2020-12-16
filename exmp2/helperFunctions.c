#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "invertedIndex.h"
#include "helperFunctions.h"

#include <math.h>

#define MAX_WORD_LEN 100

char *strdup(const char *str) {
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if (dup) {
        strcpy(dup, str);
    }
    return dup;
}

/////////////////////////////////////////////////////////////////////////////////////////
//                            normaliseWord HELPER FUNCTIONS                           //
/////////////////////////////////////////////////////////////////////////////////////////

// Get rid of leading spaces in string.
void leadingSpace(char *str) {
    // Finds the last index of whitespace.
    int lastSpace = 0;
    while (str[lastSpace] == ' ') {
        lastSpace++;
    }

    // Shifts all characters left if there is leading whitespace.
    if (lastSpace != 0) {
        int i = 0;
        for (; str[i + lastSpace] != '\0'; i++) {
            str[i] = str[i+lastSpace];
        }
        str[i] = '\0';
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//                       generateInvertedIndex HELPER FUNCTIONS                        //
/////////////////////////////////////////////////////////////////////////////////////////
// Sort the fileNames into alphabetical order (i.e. infix order)
void alphabeticalSort(InvertedIndexBST tree) {
    if (tree != NULL) {
        FileList theFile = tree->fileList;
        alphabeticalSort(tree->left);
        bubbleSortFL(theFile);
        alphabeticalSort(tree->right);
    }
}

// Bubble sort the file list into ascending order.
void bubbleSortFL(FileList head) {
    FileList i, j;
    j = head;
    i = head->next;
    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->filename, j->filename) > 0) {
                // Swap so it becomes ascending order.
                swapFiles(i, j);
            }
        }
    }
}

// Swap files (for the bubbleSortFL function).
void swapFiles(FileList x, FileList y) {
    char *tempName = x->filename;
    x->filename = strdup(y->filename);
    y->filename = strdup(tempName);
}

// Create a new BST.
InvertedIndexBST newTree(void) {
    return NULL;
}

// Create a new node.
InvertedIndexBST createNode(char* newWords, char* file) {
    // Reserve memory for both structs.
    InvertedIndexBST temp = malloc(sizeof(struct InvertedIndexNode));
    FileList tempFile = malloc(sizeof(struct FileListNode));

    // Copy the word and fileName into the new node and set left/right/next to NULL.
    temp->word = strdup(newWords);
    tempFile->filename = strdup(file);
    tempFile->next = NULL;
    temp->left = NULL;
    temp->right = NULL;
    tempFile->tf = 0;

    // Link both structs together.
    temp->fileList = tempFile;

    // Return a new node.
    return temp;
}

// Insert node into the tree.
InvertedIndexBST insertNode(InvertedIndexBST tree, InvertedIndexBST newElement) {
    if (tree == NULL) {
        // Inserting node into an empty tree (root node).
        return newElement;
    } else {
        if (strcmp(newElement->word, tree->word) < 0) {
            // Word is smaller alphabetically, so set it to the left node.
            if (tree->left == NULL) {
                tree->left = newElement;
            } else {
                // If node is full, call the function recursively until an empty node is found.
                tree->left = insertNode(tree->left, newElement);
            }
        } else if (strcmp(newElement->word, tree->word) > 0) {
            // Word is larger alphabetically, so set it to the right node.
            if (tree->right == NULL) {
                tree->right = newElement;
            } else {
                // If node is full, call the function recursively until an empty node is found.
                tree->right = insertNode(tree->right, newElement);
            }
        }
        // Return our tree with the new node inserted into it.
        return tree;
    }
}

// Check if node is already in the tree (RETURN 1 IF NO, RETURN 0 IF YES)
int checkForNode(InvertedIndexBST tree, char* newWords, char* file) {
    if (tree == NULL) {
        // This is an empty BST.
        return 1;
    } else {
        if (strcmp(newWords, tree->word) == 0) {
            // Word is already in the tree, so we need to add the current file name to the node.
            // Reserve memory for our tempFile.
            FileList tempFile = malloc(sizeof(struct FileListNode));
            // Set our current node to be the first FileList node.
            FileList curr = tree->fileList;
            if (curr == NULL) {
                // Shouldn't be called in theory, but adds filename if no filename exists in the node.
                curr->filename = strdup(file);
                curr->tf = 0;
                curr->next = NULL;
            } else {
                // Iterate until our next is NULL (i.e. space for new file).
                while (curr->next != NULL) {
                    curr = curr->next;
                }
                // Prevents a duplicate being added.
                if (strcmp(curr->filename, file) == 0) {
                    return 0;
                }
                // Adds filename to the node.
                curr->next = tempFile;
                tempFile->filename = strdup(file);
                tempFile->tf = 0;
            }   
            return 0;
        } else if (strcmp(newWords, tree->word) < 0) {
            // Keep checking until found or NULL.
            checkForNode(tree->left, newWords, file);
        } else if (strcmp(newWords, tree->word) > 0) {
            // Keep checking until found or NULL.
            checkForNode(tree->right, newWords, file);
        } else {
            // Doesn't satisfy any of the conditions, so not in tree.
            return 1;
        }
    }
    return 1;
}



/////////////////////////////////////////////////////////////////////////////////////////
//                         printInvertedIndex HELPER FUNCTIONS                         //
/////////////////////////////////////////////////////////////////////////////////////////
// Loop through tree and print each word and its corresponding files.
void printTree(InvertedIndexBST tree, FILE* writeFile) {
    if (tree != NULL) {
        FileList theFile = tree->fileList;

        // Print down the left side of the tree.
        printTree(tree->left, writeFile); 

        // Print the word.
        fprintf(writeFile, "%s", tree->word);

        // Loop through and print out each file for the word.
        while (theFile != NULL) {
            fprintf(writeFile, " %s", theFile->filename);
            theFile = theFile->next;
        }

        // New line after the word and its files have printed.
        fprintf(writeFile, "\n");

        // Print down the right side of the tree.
        printTree(tree->right, writeFile);
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
//                       calculateTfIdf AND ITS HELPER FUNCTIONS                       //
/////////////////////////////////////////////////////////////////////////////////////////
// Search for the word in the BST
FileList search(InvertedIndexBST t, char *word) {
    if (t == NULL) return NULL;
    if (strcmp(word, t->word) == 0) {
        // We found it! Return fileList.
        return t->fileList;
    } else if (strcmp(word, t->word) < 0) {
        // Check left side.
        return search(t->left, word);
    } else {
        // Check right side.
        return search(t->right, word);
    }
}

// Get the total number of files in the fileList.
int countList(FileList list) {
    int count = 0;
    for (; list != NULL; list = list->next) { count++; }
    return count;
}

// Find the Relative Term Frequency of the keyword in the file.
double relativeTermFrequency(char *filename, char *searchWord) {
    // Count the number of times searchWord appears in the file; and the total number of words in the file.
    double wordFrequency = 0;
    double totalWordCount = 0;
    char str[MAX_WORD_LEN];

    // Read through each element in the file - count the total words and times the keyword appears in the file.
    FILE *readFile = fopen(filename, "r");
    while (fscanf(readFile, "%s", str) == 1) {
        totalWordCount++;
        if (strcmp(normaliseWord(str), searchWord) == 0) {
            wordFrequency++;
        }
    }
    fclose(readFile);

    // tf(t, d) = (frequency of term t in d) / (number of words in d).
    double quotient =  wordFrequency / totalWordCount;
    return quotient;
}

// Create a list.
TfIdfList newList() {
    return NULL;
}

// Create a new node.
TfIdfList newNode(double sum, char* file) {
    // Reserve memory for struct
    TfIdfList temp = malloc(sizeof(struct TfIdfNode));
    // Copy the word and sum into the struct.
    temp->filename = strdup(file);
    temp->tfIdfSum = sum;
    temp->next = NULL;
    return temp;
}

// Insert node to end of list.
TfIdfList insertListNode(TfIdfList list, TfIdfList n) {
    if (list == NULL) {
        // Empty list, so n becomes the head element.
        list = n;
    } else {
        // Search through list until node points to NULL.
        TfIdfList node = list;
        while (node->next != NULL) {
            node = node->next;
        }
        // Replace our NULL space with n.
        node->next = n;
    }
    return list;
}

// Bubble sort the linked list into descending order (by td-idf) (ascending filenames if tf-idfs are equal).
void bubbleSortLL(TfIdfList head) {
    TfIdfList i, j;
    j = head;
    i = head->next;
    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->tfIdfSum < j->tfIdfSum) {
                // Swap so it becomes descending order.
                swapNodes(i, j);
            } else if (i->tfIdfSum == j->tfIdfSum) {
                // If equal, then ascending order by filename.
                if (strcmp(i->filename, j->filename) > 0) {
                    swapNodes(i, j);
                }
            }
        }
    }
}

// Swap nodes (for the bubbleSortLL function).
void swapNodes(TfIdfList x, TfIdfList y) {
    char *tempName = x->filename;
    double tempSum = x->tfIdfSum;
    x->filename = strdup(y->filename);
    x->tfIdfSum = y->tfIdfSum;
    y->filename = strdup(tempName);
    y->tfIdfSum = tempSum;
}