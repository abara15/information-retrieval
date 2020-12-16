// COMP2521 20T1 Assignment 1
// Author:  Anthony Barakat
//    zID:  z5258213

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "invertedIndex.h"
#include "helperFunctions.h"


#include <math.h>

#define MAX_WORD_LEN 100

// retrieve
TfIdfList addNodes(TfIdfList list, TfIdfList newList);
void bubbleSortRetrieve(TfIdfList head);
double sumNodes(double a, double b);


// Normalise a given string by modifying it to remove leading/trailing spaces, make all 
// characters lowercase, and get rid of punctuation at the end of words.
char *normaliseWord(char *str) {
    // Remove leading whitespace.
    leadingSpace(str);

    // Loop through each character in the string.
    for (int current = 0; str[current] != '\0'; current++) {
        // Make any uppercase letters lowercase.
        if (isupper(str[current])) { 
            str[current] = tolower(str[current]);
        }
        // Remove trailing whitespace.
        if (str[current] != ' ' && str[current+1] == ' ') { 
            str[current+1] = '\0';
            break;
        }
        // Get rid of '.' or ',' or ';' or '?' at end of word.
        if (str[current] == '.' || str[current] == ',' || str[current] == ';' || str[current] == '?') {
            if (str[current+1] == '\0') {
                str[current] = '\0';
                break;
            }
        }
    }

    // Return modified string.
    return str;
}


// Read through a given file with a collection of file names. Read each of these files, and 
// generate an inverted index of the all words in the files and what file(s) each word appears in. 
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    // Open collectionFilename.
    FILE *readFile = fopen(collectionFilename, "r");

    // Our string variable, array for fileNames and a count variable
    char str[MAX_WORD_LEN];
    char fileNames[MAX_WORD_LEN];

    // Create binary search tree and a temporary node.
    InvertedIndexBST tree = newTree();
    InvertedIndexBST tempNode;

    // Loop over the array of file names.
    for (int i = 0; fscanf(readFile, "%s", fileNames) != EOF; i++) {
        // Open each file corresponding to the current file name.
        FILE *readStringFile = fopen(fileNames, "r");
        // Scan each string into the variable str.
        for (int j = 0; fscanf(readStringFile, "%s", str) != EOF; j++) {
            char *var = normaliseWord(str);
            // Add node into the tree if it isn't already in there.
            if (checkForNode(tree, var, fileNames) == 1) {        
                tempNode = createNode(var, fileNames);     
                tree = insertNode(tree, tempNode);
            }
        }
        // Close file.
        fclose(readStringFile); 
    }
    fclose(readFile);
    alphabeticalSort(tree);
    return tree;
}


// Output a given inverted index tree to a file named invertedIndex.txt. 
void printInvertedIndex(InvertedIndexBST tree) {
    // Open and write to the given file, then close the file.
    FILE *writeFile = fopen("invertedIndex.txt", "w");
    printTree(tree, writeFile);
    fclose(writeFile);
}



// Return an ordered list where each node contains a filename and the corresponding tf-idf value for a given searchWord.
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    // Create a list to put the filenames and the tf-idf sums in
    TfIdfList list = newList();
    // Find the word in the BST.
    FileList node = search(tree, searchWord);
    // Count the total number of files in node.
    int totalFiles = countList(node);
    // idf(t, D) = log(total number of documents / number of documents containing the term)
    double invDocFreq = log10((double)D / (double)totalFiles);
    for (; node != NULL; node = node->next) {
        // Calculate tf-idf sum
        double relTermFreq = relativeTermFrequency(node->filename, searchWord);
        double sum = relTermFreq * invDocFreq;

        // Create and insert node into the list
        if (sum != 0) {
            TfIdfList n = newNode(sum, node->filename);
            list = insertListNode(list, n);
        }
    }

    // Bubble sort the list into descending order (and ascending file names if the tf-idf sums are equal)
    bubbleSortLL(list);
    return list;
}


// Returns an ordered list where each node contains a filename and the summation of tf-idf values of all the matching searchWords for that file.
TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[], int D) {
    TfIdfList list = newList();
    TfIdfList newLLNode;
    // Read through all the words, get their fileList and append the fileList to a big Linked List.
    for (int i = 0; searchWords[i] != NULL; i++) {
        newLLNode = calculateTfIdf(tree, searchWords[i], D);
        list = insertListNode(list, newLLNode);
    }

    // Same bubble sort as in calculateTfIdf.
    bubbleSortLL(list);

    /* 
        THE FOLLOWING STILL DOES NOT WORK. THE CONCEPT WAS TO COMPARE ALL THE ELEMENTS IN THE LIST, 
        AND IF THEIR FILENAME WAS THE SAME, ADD THEIR TF-IDF VALUES TOGETHER, AND THEN DELETE THE 
        DUPLICATE NODES. WASN'T LETTING ME ADD THE SUMS OF THE TWO NODES TOGETHER.
    */
    TfIdfList temp1 = list;
    for (; temp1 != NULL; temp1 = temp1->next) {
        TfIdfList temp2 = list->next;
        double sum = temp1->tfIdfSum;
        for (; temp2 != NULL; temp2 = temp2->next) {
            if (strcmp(temp1->filename, temp2->filename) == 0) {
                sum += temp2->tfIdfSum;
            }
        }
        temp1->tfIdfSum = sum;
        // printf("%f %s\n", temp1->tfIdfSum, temp1->filename);
    }

    return list;
}