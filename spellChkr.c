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

// Colin: I added more fields to our node to make things easier for me.
struct Node {
    char word[46];
    int wordNum;
    int lineNum;
    char filename[255];
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
int my_NCSstrcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 == '\'') {
            s1++;
            continue;
        }
        if (*s2 == '\'') {
            s2++;
            continue;
        }
        if (tolower(*s1) != tolower(*s2)) {
            return (tolower(*s1) - tolower(*s2));
        }
        s1++;
        s2++;
    }
    return (tolower(*s1) - tolower(*s2));
}

int NCSbinarySearch(int rows, char *word) {
    int low = 0;
    int high = rows - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = my_NCSstrcmp(words[mid], word);
        //printf ("strcmp: %d mid word: %s \n", cmp, words[mid]);
        //printf("debug ASCII mid: %d, and word: %d \n", words[mid], word);
        //printf("debug the word is between low: %d, and high: %d. \n", low, high);
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
// Colin: The binary search on my end can hand the 's case and more because of our 
// updated string comparison function
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
// Colin: I added new things to be added to each node
void append(struct Node **headRef, const char *word, int wordNum, int lineNum, const char *filename) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->word, word);
    newNode->wordNum = wordNum;
    newNode->lineNum = lineNum;
    strcpy(newNode->filename, filename);
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
int ttl_upper(char a[46]){
    int i = 0; 
    int counter;
    char ch;
    ch = a[0];
 
    // counting of upper case
    while (ch != '\0') {
        ch = a[i];
        if (isupper(ch))
            counter++;
 
        i++;
    }
 
    // returning total number of upper case present in sentence
    return (counter);
}

int ttl_lower(char a[46]){
    int i = 0; 
    int counter;
    char ch;
    ch = a[0];
 
    // counting of upper case
    while (ch != '\0') {
        ch = a[i];
        if (islower(ch))
            counter++;
 
        i++;
    }
 
    // returning total number of upper case present in sentence
    return (counter);
}

// this function checks the list of words obtained from the example txt file
void checkList(struct Node* head) {
    while (head != NULL) {
        char word_to_search[46];
        strcpy(word_to_search, head->word);

        int index = NCSbinarySearch(num_words, word_to_search);
        
        char Dword[46]; 
        strcpy(Dword, words[index]);
        char temp_Dword[46];
        for (int i = 1; i<strlen(Dword); i++){
            strcat(temp_Dword, &Dword[i]);
        }

        char FLcap[46];
        strcpy(FLcap, Dword);
        FLcap[0] = toupper(FLcap[0]);

        char Allcap[46];
        strcpy(Allcap, Dword);
        for(int i = 0; Allcap[i]; i++){
            Allcap[i] = toupper(Allcap[i]);
        }

        char AllLC[46];
        strcpy(AllLC, Dword);
        for(int i = 0; AllLC[i]; i++){
            AllLC[i] = tolower(AllLC[i]);
        }
        
        int UpCount;
        printf("DEBUG: Dword: %s, AllLC: %s, Allcap: %s, FLcap: %s \n", Dword, AllLC, Allcap, FLcap);

        if (index != -1) {
            printf("'%s' found at index %d.\n", word_to_search, index);
        }else if (ttl_upper(Dword) == strlen(Dword)){
            if (my_strcmp(FLcap, word_to_search)==0){
                printf("'%s' found at index %d.\n", word_to_search, index);
            } else if (my_strcmp(AllLC, word_to_search)==0){
                printf("'%s' found at index %d.\n", word_to_search, index);
            } else{
                printf("SCREAM");
            }
        } else if (ttl_lower(Dword)==strlen(Dword)) {
            if (my_strcmp(FLcap, word_to_search)==0){
                printf("'%s' found at index %d.\n", word_to_search, index);
            } else if (my_strcmp(Allcap, word_to_search)==0){
                printf("'%s' found at index %d.\n", word_to_search, index);
            } else {
                printf("SCREAM2");
            }
        } else if (isupper(Dword[0]) && ttl_lower(temp_Dword)==strlen(temp_Dword)){
            if (my_strcmp(AllLC, word_to_search)==0){
                printf("'%s' found at index %d.\n", word_to_search, index);
            } else if (my_strcmp(Allcap, word_to_search)==0) {
                printf("'%s' found at index %d.\n", word_to_search, index);
            } else {
                printf("SCREAM3");
            }
        } else if (isupper(Dword[0]) && !(ttl_lower(temp_Dword)==strlen(temp_Dword))){
            if (my_strcmp(Allcap, word_to_search)==0) {
                printf("'%s' found at index %d.\n", word_to_search, index);
            }
        }else {
            printf("'%s' not found.\n", word_to_search);
        }
        
        head = head->next;
    }
    printf("\n");
}

// Colin: updated comparePrepare, check my comments within the function to figure out
// exactly what it does
int comparePrepare() {
    char filename[255] = "constitution.txt";


    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    char buf[BUFSIZE];
    int lineNum = -1;
    ssize_t r;
    char word[46];
    struct Node *head = NULL;



    while ((r = read(fd, buf, BUFSIZE)) > 0) {
        
        char *p = buf;
        char *end = buf + r;

        int wordNumber = 0;
        while (p < end) {
            lineNum++;

            // skip the leading whitespace
            while (p < end && (*p == ' ' || *p == '\t' || *p == '\n')) {
                p++;
            }

            // copy word
            int i = 0;
            while (p < end && i < 46 - 1 && *p != ' ' && *p != '\t' && *p != '\n') {
                
                
                // keep single quote/apostrophe and dash, skip double quotes, and don't look at anything that is outside of a-z or A-Z
                if ((*p != '\'' && *p != '-') && (*p != '"' && ((*p < 'a' || *p > 'z') && (*p < 'A' || *p > 'Z')))) {
                    p++;
                    continue;
                }

                // check for various forms of punct
                if (*p == '.' || *p == '!' || *p == '?' || *p == ',' || *p == ';' || *p == ':' || *p == '"') {
                    // skip punct with space before
                    if (p == buf || *(p - 1) == ' ' || *(p - 1) == '\n') {
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
                wordNumber++;
                append(&head, word, wordNumber, lineNum, filename);
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
