#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "const_pool.h"
 
int
main(int argc, char * argv[])
{
    printf("Hello world\n");
    const char* file_name = "./helloworld.class";
    FILE* classFile = fopen(file_name, "r");
    
    if (!classFile) 
    {
       printf("Error: %s : %s \n", file_name, strerror(errno));
       return -1;
    }
    
    // parse magic number
    unsigned char magic[4];
    fread(magic, 1, 4, classFile);    
    printf("Magic:");
    dump(magic, 4);

    // parse version
    unsigned char minor_version[2];
    unsigned char major_version[2];

    fread(minor_version, 1, 2, classFile);    
    fread(major_version, 1, 2, classFile);    

    printf("Minor_Version:");
    dump(minor_version, 2);
    printf("Major_Version:");
    dump(major_version, 2);

    // parse constant pool
    unsigned char cons_pool_count[2];
    fread(cons_pool_count, 1, 2, classFile);    
    printf("Const Pool Count:");
    dump(cons_pool_count, 2);

    unsigned short cs_count = cons_pool_count[0] * 16 + cons_pool_count[1];
    Const_Pool_Entry cs_entries[cs_count];
    dumpConstPool(cs_count, cs_entries);
    fclose(classFile);
    return 0;
}


void dump(unsigned char* magic, int size) 
{
    int i;
    for(i = 0; i < size; i++)
        printf("%02X", magic[i]);
    printf("\n");
}

void dumpConstPool(short count, Const_Pool_Entry cs_entries[])
{
    
}

