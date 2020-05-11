/* pair.c: Key/Value Pair */

#include "pair.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * Create Pair structure.
 * @param   key             Pair's key.
 * @param   value           Pair's value.
 * @param   next            Reference to next pair.
 * @param   type            Pair's value's type.
 * @return  Allocated Pair structure.
 */
Pair *	    pair_create(const char *key, const Value value, Pair *next, Type type) {
    
    // allocate space for new pair
    Pair * new = calloc(1,sizeof(Pair));

    // if allocation works, set key, next, value, and type
    if(new)
    {
        new->key = strdup(key);

        new->next = next;

        pair_update(new, value, type);

    }
    else
    {

        fprintf(stderr, "calloc: %s\n", strerror(errno));
        return NULL;

    }

    // return pointer
    return new;
    
}

/**
 * Delete Pair structure.
 * @param   p               Pair structure.
 * @param   recursive       Whether or not to delete remainder of pairs.
 * return   NULL.
 */
Pair *	    pair_delete(Pair *p, bool recursive) {
    
     // make sure p exists
     if(!p)
         return NULL;
    

     // resursively call pair delete
     if(recursive)
     {
         // loop through each pair and call pair_delete non-recursively
         Pair * curr_temp = p->next;
         Pair * next_temp = NULL;
         while(curr_temp)
         {
           
             next_temp = curr_temp->next;
             pair_delete(curr_temp, false);
             curr_temp = next_temp;                              
         }
            

     }
    

     // free string, key, and then pair
     if(p->type == STRING && p->value.string)
         free(p->value.string);
     free(p->key);    
     free(p);
     return NULL;
        
}

/**
 * Update Pair's value.
 * @param   p               Pair structure.
 * @param   value           New value for Pair.
 * @param   type            New value's type.
 */
void        pair_update(Pair *p, const Value value, Type type) {


    if(!p)
        return;
    
    // free old string (if it is a string)
    if(p->type == STRING && p->value.string)
        free(p->value.string);

    // set new type
    p->type = type;

    // set new value, allocate space if it is a string
    if(type == STRING)
    {
        p->value.string = strdup(value.string);
    }
    else
    {
        p->value.number = value.number;
    }
}

/**
 * Format Pair by writing to stream.
 * @param   p               Pair structure.
 * @param   stream          File stream to write to.
 */
void        pair_format(Pair *p, FILE *stream) {
   
    //print key and value to stream
    if(p->type == STRING)
        fprintf(stream, "%s\t%s\n",p->key, p->value.string);
    else
        fprintf(stream, "%s\t%ld\n",p->key, p->value.number);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
