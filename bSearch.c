#include <stdio.h>
#include <string.h>

int binarySearch(char array[][20], int rows, char *word) {
    int low = 0;
    int high = rows - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(array[mid], word);

        if (cmp == 0) {
            return mid; // Found
        } else if (cmp < 0) {
            low = mid + 1; // Search in right half
        } else {
            high = mid - 1; // Search in left half
        }
    }

    return -1; // Not found
}


//silly main function to test searcher
int main(int argc, char *argv[]) {
    char sorted_array[][20] = {"apple", "banana", "carrot", "orange", "pear", "strawberry"};
    int rows = sizeof(sorted_array) / sizeof(sorted_array[0]);
    char word_to_search[] = "orange";

    int index = binarySearch(sorted_array, rows, word_to_search);
    if (index != -1) {
        printf("'%s' found at index %d.\n", word_to_search, index);
    } else {
        printf("'%s' not found.\n", word_to_search);
    }
}
