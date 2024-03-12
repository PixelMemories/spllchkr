#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 

#define BUFSIZE 4096

int main() {
    int fd = open("ex.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    char buf[BUFSIZE];
    ssize_t r;
    char *p;
    int buf_pos = 0;

    while ((r = read(fd, buf + buf_pos, BUFSIZE - buf_pos)) > 0) {
        // Update buffer position
        buf_pos += r;

        // Process each character in the buffer
        p = buf;
        while ((p = memchr(p, '\n', buf_pos - (p - buf))) != NULL) {
            *p = '\0';  // Null-terminate the line
            
            // empty buffer into 2d array


            printf("%s\n", buf);  // Print the line
            p++;  // Move to the next character after the newline
            buf_pos -= (p - buf);  // Update buffer position
            memmove(buf, p, buf_pos);  // Shift remaining data to the beginning of the buffer
            p = buf;  // Reset buffer pointer
        }
    }

    if (r == -1) {
        perror("Error reading file");
        return 1;
    }

    // Print the remaining content of the buffer
    if (buf_pos > 0) {
        buf[buf_pos] = '\0';  // Null-terminate the remaining content
        printf("%s\n", buf);  // Print the remaining content
    }

    close(fd);


    return 0;
}
