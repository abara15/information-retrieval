# Information Retrieval

## Introduction
We are building an information retrieval system using a well known term-weighting scheme called "tf-idf". In this program, we will:
* calculate relative term frequency tf(t,d) adjusted for document (d) length - **tf(t, d) = (frequency of term t in d) / (number of words in d).**
* calculate inverse document frequency **idf(t, D)** by dividing the total number of documents by the number of documents containing the term, and then taking the logarithm of that quotient: **idf(this,D) = log(N/|d∈D : t∈D|)**
* calculate **tfidf(t, d) = tf(t,d) * idf(t,D)**.

## Setup and Installation
Run the following command:
```sh
$ gcc -Wall -Werror -lm -std=c11 *.c -o invertedIndex
```

## Inverted Index, using BST
The file ```invertedIndex.c``` reads data from a given collection of files in ```collection.txt``` and generates an "inverted index" that provides a sorted list (set) of filenames for every word in a given collection of files. We use a binary search ADT to implement the inverted index.

Each list of filenames (for a single word) in your inverted index should be alphabetically ordered, using ascending order, and importantly duplicate filenames are not allowed.

We assume a max. word length of 100 and use dynamic data structure(s) to handle words in a file and across files. There is no restriction on line length.

We also need to "normalise" words by:
* removing leading and trailing spaces,
* converting all characters to lowercase,
* remove the following punctuation marks, if they appear at the end of a word.
  * '.' (dot),
  * ',' (comma),
  * ';' (semicolon),
  * ? (question mark)

If there are  multiple punctuation marks at the end of a word, we need to remove only the last punctuation mark. We also don't need
to remove the above punctuation marks if they appear in the middle or at the start of a word.

We must also modify a given string and NOT create another copy. You can use the functions tolower and strlen.

We need to implement the following functions in the file ```invertedIndex.c```. The API file ```invertedIndex.h``` is provided, and we must implement the
required functions in your ```invertedIndex.c```.
* ```**char *normaliseWord(char *str);**``` - normalise a given string. Do not create another copy, just modify the original string.
* ```**InvertedIndexBST generateInvertedIndex(char *collectionFilename);**``` - read a given file with collection of file names, read each of these files, generate inverted index as discussed earlier in the specs and return the inverted index.
* ```**void printInvertedIndex(InvertedIndexBST tree);**``` - output a give inverted index tree to a file named ```invertedIndex.txt```. One line per word, words should be alphabetically ordered, using ascending order. Each list of filenames (for a single word) should be alphabetically ordered, using ascending order.

```invertedIndex.txt``` may look like the following (in terms of formatting):
```
design file11.txt file21.txt
mars nasa.txt news1.txt
weather info31.txt nasa.txt news1.txt
```

## Information Retrieval Engine
We implement an information retrieval function that finds files (documents) with one or more query terms, and uses the summation
of ```tf-idf``` values of all matching query terms (words) for ranking such files (documents). We need to calculate the ```tf-idf``` value for each matching query term in a file (document), and rank files (documents) based on the summation of ```tf-idf``` values for all matching query terms present in that file. We use the "inverted index" from before to locate files with one or more query terms and calculate the required ```tf-idf``` values for such files.

The following information retrieval function retrieves in the file ```invertedIndex.c``` that given search terms (words), returns an ordered list of
type ```TfIdfList```, where each node contains a filename and the corresponding summation of ```tf-idf``` values for the given ```searchWords```. The list must be in descending order of summation of ```tf-idf``` values. See ```invertedIndex.h``` for the type definition of ```TfIdfList```. We also need to implement another function ```calculateTfIdf```.

The following functions need to be implemented in ```invertedIndex.c```. Total number of documents ```D``` is provided as an argument in both the
functions.
* ```**TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D);**``` - returns an ordered list where each node contains a filename and the corresponding ```tf-idf``` value for a given searchWord. We only need to include documents (files) that contain the given searchWord. The list must be in descending order of ```tf-idf``` value. If there are multple files with same ```tf-idf``` value, order them by their filename in ascending order.
* ```**TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[], int D);**``` - returns an ordered list where each node contains a filename and the summation of ```tf-idf``` values of all the matching searchWords for that file. You only need to include documents (files) that contain one or more of the given searchWords. The list must be in descending order of summation of ```tf-idf``` values (```tfIdfSum```). If there are multiple files with the same ```tf-idf sum```, order them by their filename in ascending order.
The ```searchWords``` array will be terminated with a ```NULL``` pointer. Here's an example:
```
char *words[] = { "nasa", "mars", "earth", NULL };
TfIdfList list = retrieve(index, words, 7);
```

## Testing
See instructions provided at the top of the file ```test_Ass1.c```.
