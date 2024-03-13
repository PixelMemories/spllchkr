#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 

#define BUFSIZE 4096

char words[104334][46]; // 2d global variable for dictionary 'words'
int num_words = 0;

// linked list node struct
struct Node {
    char word[46];
    struct Node* next;
};

int binarySearch(int rows, char *word) {
    int low = 0;
    int high = rows - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(words[mid], word);

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

// stores word in 2d global dictionary array and increments the number of words in it
void process_word(char *word) {
    strcpy(words[num_words], word);
    num_words++;
}

void prepare() {
    int fd = open("words", O_RDONLY); 
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buf[BUFSIZE];
    ssize_t r;
    char *word = NULL;
    size_t word_length = 0;

    while ((r = read(fd, buf, BUFSIZE)) > 0) {
        for (int i = 0; i < r; i++) {
            if (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t') {
                // if we encounter a whitespace character, process the current word

                // null terminates the word, processes it and resets its length
                if (word_length > 0) {
                    word[word_length] = '\0';
                    process_word(word);
                    word_length = 0;
                }
            } else {
                word = realloc(word, (word_length + 2) * sizeof(char)); // specifically allocates space for the character and null terminator
                if (word == NULL) {
                    perror("Error allocating memory");
                    close(fd);
                    return;
                }
                word[word_length++] = buf[i];
            }
        }
    }

    if (r == -1) {
        perror("Error reading file");
        close(fd);
        return;
    }

    close(fd);

    // process the last word if exists
    // might be having problems with this picking up the weird question mark characters at the end of a .txt
    if (word_length > 0) {
        word[word_length] = '\0';
        process_word(word);
    }

    free(word);
}


// Adds node to linked list
void append(struct Node** headRef, const char* word) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->word, word);
    newNode->next = NULL;

    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }

    struct Node* current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
}

// this function checks the list of words obtained from the example txt file
void checkList(struct Node* head) {
    while (head != NULL) {
        char word_to_search[46];

        strcpy(word_to_search, head->word);

        int index = binarySearch(num_words, word_to_search);
        if (index != -1) {
            printf("'%s' found at index %d.\n", word_to_search, index);
        } else {
            printf("'%s' not found.\n", word_to_search);
        }
        head = head->next;
    }
    printf("\n");
}

int comparePrepare() {
    int fd = open("road.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    char buf[BUFSIZE];
    ssize_t r;
    char word[46];
    struct Node* head = NULL;

    while ((r = read(fd, buf, BUFSIZE)) > 0) {
        char* p = buf;
        char* end = buf + r;

        while (p < end) {
            // skip the leading whitespace
            while (p < end && (*p == ' ' || *p == '\t' || *p == '\n')) {
                p++;
            }

            // copy word
            int i = 0;
            while (p < end && i < 46 - 1 && *p != ' ' && *p != '\t' && *p != '\n') {
                word[i++] = *p++;
            }
            word[i] = '\0';

            if (i > 0) {
                append(&head, word);
            }
        }
    }

    if (r == -1) {
        perror("Error reading file");
        close(fd);
        return -1;
    }

    close(fd);

    // check the linked list of words obtained from txt file
    checkList(head);

    // free LL
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}


//silly main function to test searcher
int main() {
    prepare();
    
    comparePrepare();

    
    /*
    char word_to_search[] = "Apple";

    int index = binarySearch(num_words, word_to_search);
    if (index != -1) {
        printf("'%s' found at index %d.\n", word_to_search, index);
    } else {
        printf("'%s' not found.\n", word_to_search);
    }
    */
    return 0;
}


