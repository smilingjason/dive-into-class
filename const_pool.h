
 
typedef struct Class_Info
{
    unsigned char class_index[2];
} Class_Info ; 


typedef struct _Ref_Info
{
    unsigned short class_index;
    unsigned short name_and_type_index;
} Ref_Info; 

typedef union  _Const_Pool_Info
{
    Class_Info class_info;
} Const_Pool_Info;

typedef struct _Const_Pool_Entry
{
    unsigned char tag;
    Const_Pool_Info info;
} Const_Pool_Entry;

void dumpConstPool(short count, Const_Pool_Entry cs_entries[]);
void dump(unsigned char magic[], int size);
