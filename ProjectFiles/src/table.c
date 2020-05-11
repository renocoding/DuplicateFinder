/* table.c: Separate Chaining Hash Table */

#include "table.h"
#include "hash.h"
#include "macros.h"
#include <errno.h>

/**
 * Create Table data structure.
 * @param   capacity        Number of buckets in the hash table.
 * @return  Allocated Table structure.
 */
Table *	    table_create(size_t capacity) {
    
    // allocate table space
    Table * new = calloc(1, sizeof(Table));

    // define size and capacity and then allocate space for buckets
    if(new)
    {
        new->size = 0;

        if(capacity == 0)
        {
            new->capacity = DEFAULT_CAPACITY;
        }
        else
        {
            new->capacity = capacity;
        }

        new->buckets = calloc(1, sizeof(Pair)*new->capacity);
       
        // make sure allocation for buckets worked 
        if(new->buckets)
            return new;
        else
        { 
            fprintf(stderr, "calloc: %s\n", strerror(errno));
            return NULL;
        }

    }
    else
    {
        fprintf(stderr, "calloc: %s\n", strerror(errno));
        return NULL;
    }
}

/**
 * Delete Table data structure.
 * @param   t               Table data structure.
 * @return  NULL.
 */
Table *	    table_delete(Table *t) {
    
    // loop through each row of table and recursively delete buckets
    for(int i = 0; i < t->capacity; i++)
    {
        if(&(t->buckets[i]))
            pair_delete(t->buckets[i].next, true);
    }

    // deallocate sapce for buckets and then table
    free(t->buckets);
    free(t);

    return NULL;
}

/**
 * Insert or update Pair into Table data structure.
 * @param   m               Table data structure.
 * @param   key             Pair's key.
 * @param   value           Pair's value.
 * @param   type            Pair's value's type.
 */
void        table_insert(Table *t, const char *key, const Value value, Type type) {
    
    // get key and set pointer to first bucket
    int bucket = hash_from_data(key, strlen(key)) % t->capacity;
    Pair * curr = &(t->buckets[bucket]);


    // make sure next pointer is not pointing to null
    while(curr->next)
    {
        // check if current key equals key passed in
        curr = curr->next;   
        if(strcmp(curr->key, key) == 0)            
        {
            pair_update(curr, value, type); // update value
            return;
        }
    }
    

    // create pointer and increment size (if key match not found)
    curr->next = pair_create(key, value, NULL, type);
    t->size++; 
}

/**
 * Search Table data structure by key.
 * @param   t               Table data structure.
 * @param   key             Key of the Pair to search for.
 * @return  Pointer to the Value associated with specified key (or NULL if not found).
 */
Value *     table_search(Table *t, const char *key) {
    
    // find key and set pointer to first bucket
    int bucket = hash_from_data(key, strlen(key)) % t->capacity; 
    Pair * curr = &(t->buckets[bucket]);

    // make sure pointer to next is not NULL
    while(curr->next)
    {

        // if current key equals key passed in, return value
        curr = curr->next;
        if(strcmp(curr->key, key) == 0)
        {
            return &(curr->value);
        }
        
    }

    // not found
    return NULL;
}

/**
 * Remove Pair from Table data structure with specified key.
 * @param   t               Table data structure.
 * @param   key             Key of the Pair to remove.
 * return   Whether or not the removal was successful.
 */
bool        table_remove(Table *t, const char *key) {
    
    // get key and set pointer to first bucket
    int bucket = hash_from_data(key, strlen(key)) % t->capacity;
    Pair * curr = &(t->buckets[bucket]);
    Pair * prev;

    // make sure pointer to next is not NULL
    while(curr->next)
    {
        prev = curr;
        curr = curr->next;

        // call pair_delete if current key equals key passed in
        if(strcmp(curr->key, key) == 0)
        {
            prev->next = curr->next; // connect pointers before deleting                
            pair_delete(curr, false);
            t->size--;
            return true;
        }

    }
    
    return false;
}

/**
 * Format all the entries in the Table data structure.
 * @param   m               Table data structure.
 * @param   stream          File stream to write to.
 */
void	    table_format(Table *t, FILE *stream) {

    // loop through each row of table and print each row's buckets
    for(int i = 0; i < t->capacity; i++)
    {
        Pair * curr = &(t->buckets[i]);

        // print each bucket in each row
        while(curr->next)
        {
            curr = curr->next;
            pair_format(curr, stream);     
        }
    }
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
