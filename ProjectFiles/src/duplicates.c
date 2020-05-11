/* duplicates.c */

#include "hash.h"
#include "macros.h"
#include "table.h"

#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

/* Globals */

char * PROGRAM_NAME = NULL;

/* Structures */

typedef struct {
    bool count;
    bool quiet;
} Options;

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: %s paths...\n", PROGRAM_NAME);
    fprintf(stderr, "    -c     Only display total number of duplicates\n");
    fprintf(stderr, "    -q     Do not write anything (exit with 0 if duplicate found)\n");
    exit(status);
}

/**
 * Check if path is a directory.
 * @param       path        Path to check.
 * @return      true if Path is a directory, otherwise false.
 */
bool is_directory(const char *path) {
    

    // call stat on path and make sure stat worked
    struct stat s;
    if(stat(path, &s) < 0)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return false;
    }

    // return true if its a directory
    if(S_ISDIR(s.st_mode))
    {
        return true;   
    }
    

    return false;
}

/**
 * Check if file is in table of checksums.
 *
 *  If quiet is true, then exit if file is in checksums table.
 *
 *  If count is false, then print duplicate association if file is in
 *  checksums table.
 *
 * @param       path        Path to file to check.
 * @param       checksums   Table of checksums.
 * @param       options     Options.
 * @return      0 if Path is not in checksums, otherwise 1.
 */
size_t check_file(const char *path, Table *checksums, Options *options) {
    
    // see if we got a key
    char key[HEX_DIGEST_LENGTH];
    bool gotKey = hash_from_file(path, key);

    // see if the value exists in table already
    Value * v= table_search(checksums, key);
    
    // file found: either print that we found it or exit with success (if -q)
    if(v && gotKey)
    {
        if(!options->count && !options->quiet)
            printf("%s is duplicate of %s\n", path, v->string);

        if(options->quiet)
            exit(0);

        return 1;
    }

    // file not found, add to table
    else
    {
        Value newVal;
        newVal.string = (char *)path;
        table_insert(checksums, key, newVal, STRING);
        return 0;
    }

    
}

/**
 * Check all entries in directory (recursively).
 * @param       root        Path to directory to check.
 * @param       checksums   Table of checksums.
 * @param       options     Options.
 * @return      Number of duplicate entries in directory.
 */
size_t check_directory(const char *root, Table *checksums, Options *options) {

    // open directory and initalize total count for root directory
    int dupCount = 0;
    DIR *d = opendir(root);
    if(!d)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }


    for(struct dirent *e = readdir(d); e; e = readdir(d))
    {
        // skip current and previous directory
        if(strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        
        // get full path
        char fullPath[BUFSIZ];
        sprintf(fullPath, root);
        char * temp = fullPath + strlen(fullPath);
        sprintf(temp, "/%s", e->d_name);

        // recursive call if it is a directory, or call check_file if it is a file
        if(is_directory(fullPath))
        {
            dupCount += check_directory(fullPath, checksums, options);
        }
        else
        {
            dupCount += check_file(fullPath, checksums, options);
        }
    }
    
    // close directory and return total count
    closedir(d);
    return dupCount;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    /* Parse command line arguments */
    int argInd = 1;
    char * arg;

    // set options quiet and count to false
    Options o = {.count = false, .quiet = false};

    // initialze count adn create table
    size_t dupCount = 0;
    Table * t = table_create(0);

    if(argc < 2)
        usage(1);

    // loop through flags
    while(argv[argInd][0] == '-' && argInd < argc)
    {
        arg = argv[argInd];
        if(strcmp(arg, "-h") == 0)
            usage(0);
        else if(strcmp(arg, "-c") == 0)
            o.count = true;
        else if(strcmp(arg, "-q") == 0)
            o.quiet = true;
        else
            usage(1);
        argInd++;
    }


    // loop through files and directories    
    while(argInd < argc)
    {
        arg = argv[argInd];

        // call either check_file or check_directory on arg
        if(is_directory(arg))
        {
            dupCount += check_directory(arg, t, &o);
        }
        else
        {
            dupCount += check_file(arg, t, &o);
        }
        argInd++;

    }

    // exit if quiet and we found no dupes
    if(o.quiet)
        exit(1);


    /* Display count if necessary */
    if(o.count)    
        printf("%zu\n", dupCount);

    // free allocated space
    table_delete(t);

    return 0;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
