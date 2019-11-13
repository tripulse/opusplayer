#include <cstdio>
typedef struct {
    /**
     * Pointer to the data allocated in the RAM.
     * Void pointers doesn't have any defined size
     * to use them they've to be casted to views first.
     */
    void  *data;
    
    /**
     * Size of the data allocated in the RAM.
     * On modern systems, accessing pointer index more than
     * this size would result in an error.
     */
    size_t size;
} Memory;