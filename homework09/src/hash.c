/* hash.c: hash functions */

#include "hash.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


/**
 * Constants
 * http://isthe.com/chongo/tech/comp/fnv/
 */

#define FNV_OFFSET_BASIS    (0xcbf29ce484222325ULL)
#define FNV_PRIME           (0x100000001b3ULL)

/**
 * Compute FNV-1 hash.
 * @param   data            Data to hash.
 * @param   n               Number of bytes in data.
 * @return  Computed hash as 64-bit unsigned integer.
 */
uint64_t    hash_from_data(const void *data, size_t n) {

    // perform algorithm from pseudocode
    uint64_t hash = FNV_OFFSET_BASIS;
    uint8_t *bytes = (uint8_t *) data;
    for(int i = 0; i < n; i++)
    {
        hash = hash ^ bytes[i];
        hash = hash * FNV_PRIME;
    }

    // return hash number
    return hash;
}

/**
 * Compute MD5 digest.
 * @param   path            Path to file to checksum.
 * @param   hexdigest       Where to store the computed checksum in hexadecimal.
 * @return  Whether or not the computation was successful.
 */
bool        hash_from_file(const char *path, char hexdigest[HEX_DIGEST_LENGTH]) {

    // intialize variables and open file
    MD5_CTX c;
    FILE *f = fopen(path, "r");
    unsigned char md5_string[MD5_DIGEST_LENGTH]; 
    
    // make sure file is successfully opened
    if(!f)
    {
        fprintf(stderr, "fopen: %s\n", strerror(errno));
        return false;
    }

    // read from file with help of buffer
    MD5_Init(&c);
    char buf[BUFSIZ];
    ssize_t bytes = fread(buf, 1, BUFSIZ, f);
    while(bytes > 0)
    {
        MD5_Update(&c, buf, bytes);
        bytes = fread(buf, 1, BUFSIZ, f);
    }

    MD5_Final(md5_string, &c);

    // copy hash to hexdigest
    char * temp = hexdigest;
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
       
        int test = sprintf(temp, "%02x", md5_string[i]);
        if(test < 0)
            return false;        
        temp = temp + 2;

               
    }
   

    // close file
    fclose(f);

    return true;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
