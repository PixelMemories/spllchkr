#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h> // speed, I am speed
#include <ctype.h>

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
    int hyphen; // If hyphen == 1 then the word has a hypen
    struct Node *next;
};

//Richard: here are my custom strcmps
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
        if (!isascii(*s1)){
            //printf("IM NOT ASCII 1: %s \n", s1);
            s1++;
            continue;
        }
        if (!isascii(*s2)){
            //printf("IM NOT ASCII 2: %s \n", s2);
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
        if (!isascii(*s1)){
            //printf("IM NOT ASCII 1: %s \n", s1);
            s1++;
            continue;
        }
        if (!isascii(*s2)){
            //printf("IM NOT ASCII 2: %s \n", s2);
            s2++;
            continue;
        }
        char c1 = tolower(*s1);
        char c2 = tolower(*s2);

        if (c1 != c2) {
            return (c1 - c2);
        }
        s1++;
        s2++;
    }
    return tolower(*s1) - tolower(*s2);
}

//Richard: a bunch of search functions. some are N/A but here they are
int CSbinarySearch(int rows, char *word) {
    int low = 0;
    int high = rows - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = my_strcmp(words[mid], word);
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

//DEFUNCT AND IS NOT CASE SENSITIIVE FOR OUT APPLICATION
int NCSbinarySearch(int rows, char *word) {
    int low = 0;
    int high = rows - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = my_NCSstrcmp(words[mid], word);
        printf ("strcmp: %d mid word: %s \n", cmp, words[mid]);
        printf("debug ASCII mid: %d, and word: %d, and wordword: %s \n", words[mid], word, word);
        printf("debug the word is between low: %d, and high: %d. \n", low, high);
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

int NCSseqSearch (int rows, char *word) {
    for (int i = 0; i< rows; i++){
        if(my_NCSstrcmp(words[i], word)==0){
            return i;
        }
    }
    return -1;
}

//Richard Case Sensitivity helper functions. these just count the number of upper and lower case numbers.
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

//Richard: a little helper function. capitolizes the first character (no clue why I have it here I was brain marathoning)
void capFL(char *str) {
    if (str == NULL || *str == '\0') {
        return; // Handle empty string or NULL pointer
    }
    *str = toupper(*str); // Capitalize the first character
}

//Richard: well I saw I was about to dupe this function 3 times in checkList so I just made it its own function to save some typing. 
int DOcheckL (char *word_to_search, struct Node* head){
    int index = CSbinarySearch(num_words, word_to_search);
    if (index == -1) {
        //THUS THE CASE SENSITIVE SEARCH WAS A FAIL
        //printf("'%s' THIS WORD FAILED CS TEST \n", word_to_search);
            
        //do a NCS test to find if word exists in dictionary letterwise
        int NCSindex = NCSseqSearch(num_words, word_to_search);
        if (NCSindex == -1){
            return -1;
        }else{
            char Dword[46]; 
            strcpy(Dword, words[NCSindex]);

            char FLcap[46];
            strcpy(FLcap, Dword);
            capFL(FLcap);

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

            //printf("DEBUG: Dword: %s, AllLC: %s, Allcap: %s, FLcap: %s \n", Dword, AllLC, Allcap, FLcap);

            if (my_strcmp(FLcap, word_to_search)==0){
                //printf("'%s' FLcap found at index %d in dict SPELLED CORRECT.\n", word_to_search, NCSindex);
                return 0;
            } else if (my_strcmp(Allcap, word_to_search)==0){
                //printf("'%s' Allcap found at index %d in dict SPELLED CORRECT.\n", word_to_search, NCSindex);
                return 0;
            } else if (my_strcmp(AllLC, word_to_search)==0){
                //printf("'%s' found at index %d.\n", word_to_search, NCSindex);
                return 0;
            } else{
                return -1;
            }
        }
    }
}

//Richard: this function checks the list of words obtained from the example txt file 
//It is now fully case sensitive and follows the logic mentioned in write up about uppercase letters
void checkList(struct Node* head) {
    while (head != NULL) {
        char word_to_search[46];
        strcpy(word_to_search, head->word);
        
        char *hyphen_position = strchr(word_to_search, '-');
        if (hyphen_position == NULL) {
            head->hyphen = 0;
        } else {
            head->hyphen = 1;
        }
        int ifHyph = head->hyphen;

        if (ifHyph == 1){
            char first[46];
            char last[46];
            char *hyphen_position = strchr(word_to_search, '-');

            int HyphPos = hyphen_position - word_to_search;

            strncpy(first, word_to_search, HyphPos);
            first[HyphPos] = '\0';
            strcpy(last, word_to_search + HyphPos + 1);

            //printf("first: %s, last: %s\n", first, last);

            int fCheck = DOcheckL(first, head);
            int lCheck = DOcheckL(last, head);
            if (fCheck == -1 || lCheck == -1){
                printf("'%s' not found. This word is %d word(s) into file %s on line %d\n", word_to_search, head->wordNum, head->filename, head->lineNum);
            }

        } else{
            int Check = DOcheckL(word_to_search, head);
            if (Check == -1){
                printf("'%s' not found. This word is %d word(s) into file %s on line %d\n", word_to_search, head->wordNum, head->filename, head->lineNum);
            }
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
