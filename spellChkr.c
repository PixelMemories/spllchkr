#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h> // speed, I am speed

#define BUFSIZE 4096

char words[104334][46]; // 2d global variable for dictionary 'words'
int num_words = 0;

// linked list node struct
struct Node {
    char word[46];
    struct Node *next;
};

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 == '\'') {
            s1++;
            continue;
        }
        if (*s2 == '\'') {
            s2++;
            continue;
        }
        if (*s1 != *s2) {
            return (*s1 - *s2);
        }
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

int binarySearch(int rows, char *word) {
    int low = 0;
    int high = rows - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = my_strcmp(words[mid], word);

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
    if (strstr(word, "'s") == NULL) { // Check if the substring "'s" is not present in the word
        strcpy(words[num_words], word);
        num_words++;
    }
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
void append(struct Node **headRef, const char *word) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
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

    struct Node *current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
}

// this function checks the list of words obtained from the example txt file
void checkList(struct Node *head) {
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
    struct Node *head = NULL;

    while ((r = read(fd, buf, BUFSIZE)) > 0) {
        char *p = buf;
        char *end = buf + r;

        while (p < end) {
            // skip the leading whitespace
            while (p < end && (*p == ' ' || *p == '\t' || *p == '\n')) {
                p++;
            }

            // copy word
            int i = 0;
            while (p < end && i < 46 - 1 && *p != ' ' && *p != '\t' && *p != '\n') {
                // skip double quotes
                if (*p == '"') {
                    p++;
                    continue;
                }
                // check for various forms of punct
                if (*p == '.' || *p == '!' || *p == '?' || *p == ',' || *p == ';' || *p == ':') {
                    // skip punct with space before
                    if (p == buf || *(p - 1) == ' ') {
                        p++;
                        continue;
                    }
                    // skip punct with space after
                    if (*(p + 1) == ' ' || *(p + 1) == '\t' || *(p + 1) == '\n') {
                        p++;
                        continue;
                    }
                }
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
    struct Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}


// silly main function to test searcher
int main() {
    prepare();

    
    clock_t start_time = clock();

    comparePrepare();

    
    clock_t end_time = clock();

    
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Elapsed time: %.6f seconds\n", elapsed_time);

    printf("%d\n", num_words);

    return 0;
}
