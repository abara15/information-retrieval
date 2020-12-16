// COMP2521 20T1 Assignment 1 - Helper Functions
// Author:  Anthony Barakat
//    zID:  z5258213

#include "invertedIndex.h"

// normaliseWord Function Prototypes
void leadingSpace(char *str);

// generateInvertedIndex Function Prototypes
InvertedIndexBST newTree(void);
InvertedIndexBST createNode(char* newWords, char* file);
InvertedIndexBST insertNode(InvertedIndexBST tree, InvertedIndexBST newElement);
int checkForNode(InvertedIndexBST tree, char* newWords, char* file);
void alphabeticalSort(InvertedIndexBST tree);
void bubbleSortFL(FileList head);
void swapFiles(FileList x, FileList y);

// printInvertedIndex Function Prototypes
void printTree(InvertedIndexBST tree, FILE* writeFile);

// calculateTfIdf Function Prototypes
FileList search(InvertedIndexBST t, char *word);
double relativeTermFrequency(char *filename, char *searchWord);
int countList(FileList list);
TfIdfList newList(void);
TfIdfList newNode(double sum, char* file);
TfIdfList insertListNode(TfIdfList list, TfIdfList n);
void bubbleSortLL(TfIdfList head);
void swapNodes(TfIdfList x, TfIdfList y);

// Other Function Prototypes
char *strdup(const char *str);