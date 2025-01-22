/*
 * It is forbidden to use AI tools such as ChatGPT and Github Copilot
 * to complete this technical test.
 *
 * Using them on the job is also strongly discouraged. We are not
 * luddites, but programmers pushing code that they don't understand
 * is not something that we need. Delivering quality software is
 * hard enough already.
 *
 * If you are invited for an interview, we will ask questions about
 * this program. If the code was written by AI, you will not be able
 * to answer them and you will be rejected.
 * Please do not waste our time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fifo {
    int size;
    int empty;
    int produce;
    int consume;
    char *contents[];
};


// Create a new string FIFO object that can contain up to size elements.
struct fifo *fifo_new(int size) {
    // COMPLETE THIS PART
    struct fifo *fifo = malloc(sizeof(struct fifo) + size * sizeof(char *)); // allocate memory for the struct (size, empty, produce, consume and the consume) and the array (elements of consume)
    fifo->size = size;
    fifo->empty = 1;
    fifo->produce = 0;
    fifo->consume = 0;
    memset(fifo->contents, 0, size * sizeof(char *)); // set the consume array's size
    return fifo;
}


// Free a FIFO object and all its contents.
void fifo_free(struct fifo *fifo) {
    // COMPLETE THIS PART
    for (int i = 0; i < fifo->size; i++) {
        if (fifo->contents[i] != NULL) {
            free(fifo->contents[i]); //free consume array
        }
    }
    free(fifo); // free the object
}

// Push a string into the FIFO.
// Returns whether there was space in the FIFO to store the string.
// If successful, the FIFO stores a copy of the string dynamically allocated on the heap.
// If unsuccessful, the FIFO remains unchanged and no memory is allocated.
int fifo_push(struct fifo *fifo, const char *str) {
    // COMPLETE THIS PART
    if (!fifo->empty && fifo->produce == fifo->consume) { //if the FIFO is full i.e. produce and consume are at the same index and the FIFO is not empty
        return 0;
    }
    fifo->contents[fifo->produce] = strdup(str); // copy the string to the consume array. Using copy to avoid memory leaks, e.g. if the string is freed before the FIFO is freed
    fifo->produce = (fifo->produce + 1) % fifo->size;
    fifo->empty = 0;
    return 1;

}

// Pull a string from the FIFO.
// Returns NULL if the FIFO is empty.
// If the returned value is not NULL, the caller takes ownership of the string and 
// is responsible for freeing it.
char *fifo_pull(struct fifo *fifo) {
    // COMPLETE THIS PART
    if (fifo->empty) { // if the FIFO is empty
        return NULL;
    }
    char *str = fifo->contents[fifo->consume]; // get the string from the consume array. asterisk (*) is used to get the value of the pointer
    fifo->contents[fifo->consume] = NULL; // set the value of the consume array to NULL
    fifo->consume = (fifo->consume + 1) % fifo->size; // increment the consume index
    if (fifo->consume == fifo->produce) {
        fifo->empty = 1; // if the consume index is equal to the produce index, the FIFO is empty
    }
    return str;
}

void fifo_dump(struct fifo *fifo) {
    char *str;
    while(str = fifo_pull(fifo)) {
        printf("%s\n", str);
        free(str);
    }
}

#define TEST(condition) if(!(condition)) { printf("TEST FAILED\n"); return 1; }

int main() {
    struct fifo *fifo;
    char *str;

    fifo = fifo_new(4);
    TEST(fifo_push(fifo, "hello"));
    TEST(fifo_push(fifo, "world"));
    fifo_dump(fifo);
    fifo_free(fifo);

    fifo = fifo_new(4);
    TEST(fifo_push(fifo, "elem1"));
    TEST(fifo_push(fifo, "elem2"));
    TEST(fifo_push(fifo, "elem3"));
    TEST(fifo_push(fifo, "elem4"));
    fifo_dump(fifo);
    TEST(fifo_push(fifo, "A"));
    fifo_dump(fifo);
    TEST(fifo_push(fifo, "X"));
    TEST(fifo_push(fifo, "Y"));
    TEST(fifo_push(fifo, "Z"));
    TEST(fifo_push(fifo, "T"));
    TEST(!fifo_push(fifo, "U"));
    fifo_dump(fifo);
    fifo_free(fifo);

    fifo = fifo_new(4);
    TEST(fifo_push(fifo, "elem1"));
    TEST(fifo_push(fifo, "elem2"));
    TEST(fifo_push(fifo, "elem3"));
    TEST(fifo_push(fifo, "elem4"));
    fifo_free(fifo);

    fifo = fifo_new(4);
    TEST(fifo_push(fifo, "elem1"));
    TEST(fifo_push(fifo, "elem2"));
    str = fifo_pull(fifo);
    TEST(!strcmp(str, "elem1"));
    free(str);
    TEST(fifo_push(fifo, "elem3"));
    TEST(fifo_push(fifo, "elem4"));
    str = fifo_pull(fifo);
    TEST(!strcmp(str, "elem2"));
    free(str);
    str = fifo_pull(fifo);
    TEST(!strcmp(str, "elem3"));
    free(str);
    str = fifo_pull(fifo);
    TEST(!strcmp(str, "elem4"));
    free(str);
    fifo_free(fifo);

    return 0;
}
