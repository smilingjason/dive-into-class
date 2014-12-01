#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "const_pool.h"
 
Const_Pool_Entry* cs_entries;

int
main(int argc, char * argv[])
{
    const char* file_name = "./helloworld.class";
    FILE* classFile = fopen(file_name, "r");
    
    if (!classFile) 
    {
       printf("Error: %s : %s \n", file_name, strerror(errno));
       return -1;
    }

    long currentPosition = ftell(classFile);
    printf("Start position: %ld\n", currentPosition);
    
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
    printf("Const Pool Count: %d = ", cons_pool_count[0] * 16 + cons_pool_count[1]);
    dump(cons_pool_count, 2);

    unsigned short cs_count = cons_pool_count[0] * 16 + cons_pool_count[1];
    cs_entries = malloc((cs_count + 1) * sizeof(Const_Pool_Entry));
    int cs_index = 1;
    for (cs_index = 1; cs_index < cs_count ; cs_index++) 
    {
        fread(&cs_entries[cs_index].tag, 1, 1, classFile);
        switch (cs_entries[cs_index].tag) 
        {
            case 7: // class info
                cs_entries[cs_index].info.class_info.class_index = readindex(classFile);
                break;
            case 9: // field ref
                ;
                unsigned short class_index = readindex(classFile);
                cs_entries[cs_index].info.fieldref_info.class_index = class_index;
                unsigned short nameandtype_index = readindex(classFile);
                cs_entries[cs_index].info.fieldref_info.name_and_type_index = nameandtype_index;
                break;
            case 10: // method ref
                cs_entries[cs_index].info.methodref_info.class_index = readindex(classFile);
                cs_entries[cs_index].info.methodref_info.name_and_type_index = readindex(classFile);
                break;
            case 11: // interface method ref
                cs_entries[cs_index].info.interfacemethodref_info.class_index = readindex(classFile);
                cs_entries[cs_index].info.interfacemethodref_info.name_and_type_index = readindex(classFile);
                break;
            case 8: // string
                cs_entries[cs_index].info.string_info.string_index = readindex(classFile);
                break;
            case 3: // integer
                break;
            case 4: // float
                break;
            case 5: // long
                break;
            case 6: // double
                break;
            case 12: // name and type
                cs_entries[cs_index].info.nameandtype_info.name_index = readindex(classFile);
                cs_entries[cs_index].info.nameandtype_info.descriptor_index = readindex(classFile);
                break;
            case 1: // utf-8
                cs_entries[cs_index].info.utf8_info.length = readindex(classFile);
                cs_entries[cs_index].info.utf8_info.bytes 
                    = readarray(cs_entries[cs_index].info.utf8_info.length, classFile);

                break;
            case 15: // method handle
                break;
            case 16: // method type
                break;
            case 18: // invoke dynamic
                break;
            default:
                  printf("Error: #%d unknown constant pool type %d\n", 
                          cs_index, cs_entries[cs_index].tag);   
        }
    }
    dumpConstPool(cs_count, cs_entries);
    // parse access flag
    unsigned short access_flag = readindex(classFile);
    printf("Access Flag: %d \n", access_flag);

    // parse this class
    unsigned short this_class_index = readindex(classFile); 
    printf("This class: #%d \n", this_class_index);

    // parse super class
    unsigned short super_class_index = readindex(classFile); 
    printf("Super class: #%d \n", super_class_index);

    // parse interface count
    unsigned short interface_count = readindex(classFile); 
    printf("Interface Count: #%d \n", interface_count);
    
    // parse interface indexes
    unsigned short* interfaces;
    if (interface_count > 0) 
    {
        interfaces = readshortarray(interface_count, classFile);
        int i = 0;
        for ( i = 0; i < interface_count; i++) 
        {
            printf("\t Interface #%d \n", interfaces[i]);
        }
    }

    //parse field 
    unsigned short field_count = readindex(classFile);
    printf("Field count: %d \n", field_count);

    Field_Info field_infos[field_count];
    int i = 0;
    for(i = 0; i < field_count; i++) 
    {
        field_infos[i].access_flag = readindex(classFile);
        field_infos[i].name_index = readindex(classFile);
        field_infos[i].descriptor_index = readindex(classFile);
        printf("    method #%d ", i + 1);
        printf("name = #%d ", field_infos[i].name_index);
        printf("\t // %s", cs_entries[field_infos[i].name_index].info.utf8_info.bytes);
        printf("\n");
        field_infos[i].attribute_count = readindex(classFile);
        field_infos[i].attribute_infos = readmethodattribute(
                field_infos[i].attribute_count, classFile);

        printf("\n");
    }

    //parse method
    unsigned short method_count = readindex(classFile);
    printf("Method count: %d \n", method_count);
    Method_Info method_infos[method_count];
    for(i = 0; i < method_count; i++) 
    {
        method_infos[i].access_flag = readindex(classFile);
        method_infos[i].name_index = readindex(classFile);
        method_infos[i].descriptor_index = readindex(classFile);
        printf("    method #%d ", i + 1);
        printf("name = #%d ", method_infos[i].name_index);
        printf("\t // %s", cs_entries[method_infos[i].name_index].info.utf8_info.bytes);
        printf("\n");
        method_infos[i].attribute_count = readindex(classFile);
        method_infos[i].attribute_infos = readmethodattribute(
                method_infos[i].attribute_count, classFile);

        printf("\n");
    }

    //parse class attribute 
    unsigned short attribute_count = readindex(classFile);
    printf("Attribute count: %d \n", attribute_count);

    int isEOF = feof(classFile);
    printf("eof set?: %d \n", isEOF);
    readmethodattribute(attribute_count, classFile);

    currentPosition = ftell(classFile);
    printf("Stop position: %ld\n", currentPosition);

    fclose(classFile);
    free(cs_entries);    
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
    int i = 1;
    for(i = 1; i < count; i++) 
    {
        dumpOneConstantPoolEntry(i, cs_entries);
    }
}

unsigned short readindex(FILE* classFile)
{
    unsigned char temp[2];
    fread(temp, 1, 2, classFile);    
    return temp[0] * pow(2,8) + temp[1];
}

unsigned int readInt(FILE* classFile)
{
    unsigned char temp[4];
    int read = fread(temp, 1, 4, classFile);    
    int result = temp[0] * pow(2,24) + temp[1] * pow(2,16) + temp[2] * pow(2,8) + temp[3];
    return result;
}



unsigned char* readarray(unsigned int size, FILE* classFile)
{
    //while create "size + 1"? --> the string in utf8_info are not null-terminated (jvmspeci 4.4.7)
    unsigned char* p = malloc((size +1) * sizeof(unsigned char));
    if (p == NULL) {
        fprintf(stderr, "unable to allocate memory\n");
        return NULL;
    }
    fread(p, 1, size, classFile);    
    return p;
}

unsigned short* readshortarray(unsigned short size, FILE* classFile)
{
    //while create "size + 1"? --> the string in utf8_info are not null-terminated (jvmspeci 4.4.7)
    unsigned short* p = malloc((size +1) * sizeof(unsigned short));
    if (p == NULL) {
        fprintf(stderr, "unable to allocate memory\n");
        return NULL;
    }
    fread(p, 2, size, classFile);    
    return p;
}

void dumpOneConstantPoolEntry(unsigned short current, Const_Pool_Entry* cs_entries) 
{   
    printf("\t #%d = ", current);
    switch (cs_entries[current].tag) 
        {
            case 7: // class info
                ;
                unsigned short class_index = cs_entries[current].info.class_info.class_index;
                printf("%s\t\t #%d \t\t // %s\n", "Class", 
                        class_index, cs_entries[class_index].info.utf8_info.bytes); 
                break;
            case 9: // field ref
                printf("%s\t\t #%d.#%d", "FieldRef",
                        cs_entries[current].info.fieldref_info.class_index,
                        cs_entries[current].info.fieldref_info.name_and_type_index); 
                printf("\n");
                break;
            case 10: // method ref
                printf("%s\t\t #%d.#%d", "MethodRef",
                        cs_entries[current].info.methodref_info.class_index,
                        cs_entries[current].info.methodref_info.name_and_type_index); 
                class_index = 
                        cs_entries[current].info.methodref_info.class_index;
                unsigned short nameandtype_index =
                        cs_entries[current].info.methodref_info.name_and_type_index; 
                unsigned short class_utf8_index = cs_entries[class_index].info.class_info.class_index;
                printf("\t\t // %s", cs_entries[class_utf8_index].info.utf8_info.bytes);
                unsigned short name_utf8_index = cs_entries[nameandtype_index].info.nameandtype_info.name_index;
                unsigned short descriptor_utf8_index = cs_entries[nameandtype_index].info.nameandtype_info.descriptor_index;
                printf(".%s:%s", cs_entries[name_utf8_index].info.utf8_info.bytes,
                        cs_entries[descriptor_utf8_index].info.utf8_info.bytes);
                printf("\n");
                break;
            case 11: // interface method ref
                printf("%s\n", "InterfaceMethodRef"); 
                break;
            case 8: // string
                printf("%s\t\t #%d", "String",
                        cs_entries[current].info.string_info.string_index); 
                printf("\n");
                break;
            case 3: // integer
                printf("%s\n", "Integer"); 
                break;
            case 4: // float
                printf("%s\n", "Float"); 
                break;
            case 5: // long
                printf("%s\n", "Long"); 
                break;
            case 6: // double
                printf("%s\n", "Double"); 
                break;
            case 12: // name and type
                ;
                unsigned short name_index = cs_entries[current].info.nameandtype_info.name_index;
                unsigned short descriptor_index = cs_entries[current].info.nameandtype_info.descriptor_index; 
                printf("%s\t #%d.#%d", "NameAndType",
                        name_index,
                        cs_entries[current].info.nameandtype_info.descriptor_index); 
                printf("\t\t // %s:%s ", cs_entries[name_index].info.utf8_info.bytes, 
                        cs_entries[descriptor_index].info.utf8_info.bytes);
                printf("\n");
                break;
            case 1: // utf-8
                printf("%s\t\t %s \n", "UTF8", cs_entries[current].info.utf8_info.bytes); 
                break;
            case 15: // method handle
                printf("%s\n", "MethodHanle"); 
                break;
            case 16: // method type
                printf("%s\n", "MethodType"); 
                break;
            case 18: // invoke dynamic
                printf("%s\n", "InvokeDynamic"); 
                break;
            default:
                  printf("Error: unknown constant pool type %d\n", cs_entries[current].tag);   
        }
}

Attribute_Info* readmethodattribute(unsigned short count, FILE* classFile)
{
    printf ("\tAttribute count: %d\n", count);
    Attribute_Info* p = malloc(count * sizeof(Attribute_Info));

    int i = 0;
    for( i = 0; i < count; i++) 
    {
        p->attribute_name_index = readindex(classFile);
        printf("\tAttribute index: %d", p->attribute_name_index);
        printf("\t // %s", cs_entries[p->attribute_name_index].info.utf8_info.bytes);
        printf("\n");
        p->attribute_length = readInt(classFile);
        printf ("\tAttribute length: %d\n", p->attribute_length);
        p->attributes = readarray(p->attribute_length, classFile);
    }
    return p;
}
