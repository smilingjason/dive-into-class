
 
typedef struct Class_Info
{
    unsigned short class_index;
} Class_Info ; 


typedef struct FieldRef_Info
{
    unsigned short class_index;
    unsigned short name_and_type_index;
} FieldRef_Info; 

typedef struct MethodRef_Info
{
    unsigned short class_index;
    unsigned short name_and_type_index;
} MethodRef_Info; 

typedef struct InterfaceMethodRef_Info
{
    unsigned short class_index;
    unsigned short name_and_type_index;
} InterfaceMethodRef_Info; 

typedef struct String_Info
{
    unsigned short string_index;
} String_Info ; 

typedef struct Integer_Info
{
    unsigned char integer_value[4];
} Integer_Info ; 

typedef struct Float_Info
{
    unsigned char float_value[4];
} Float_Info ; 

typedef struct Long_Info
{
    unsigned char long_value_high[4];
    unsigned char long_value_low[4];
} Long_Info ; 

typedef struct Double_Info
{
    unsigned char double_value_high[4];
    unsigned char double_value_low[4];
} Double_Info ; 

typedef struct NameAndType_Info
{
    unsigned short name_index;
    unsigned short descriptor_index;
} NameAndType_Info;

typedef struct UTF8_Info
{
    unsigned short length;
    unsigned char* bytes;
} UTF8_Info;

typedef struct MethodHandle_Info
{
    unsigned char reference_kind;
    unsigned char reference_index[2];    
} MethodHandle_Info;

typedef struct MethodType_Info
{
    unsigned short descriptor_index;
} MethodType_Info;


typedef struct InvokeDynamic_Info
{
    unsigned short bootstrap_method_attr_index;
    unsigned short name_and_type_index;
} InvokeDynamic_Info;

typedef union  _Const_Pool_Info
{
    Class_Info class_info;
    FieldRef_Info fieldref_info;
    MethodRef_Info methodref_info;
    InterfaceMethodRef_Info interfacemethodref_info;
    String_Info string_info;
    Integer_Info integer_info;
    Float_Info float_info;
    Long_Info long_info;
    Double_Info double_info;
    NameAndType_Info nameandtype_info;
    UTF8_Info utf8_info;
    MethodHandle_Info methodhandle_info;
    MethodType_Info methodtype_info;
    InvokeDynamic_Info invokedynamic_info;
} Const_Pool_Info;

typedef struct _Const_Pool_Entry
{
    unsigned char tag;
    Const_Pool_Info info;
} Const_Pool_Entry;

void dumpConstPool(short count, Const_Pool_Entry cs_entries[]);
void dump(unsigned char magic[], int size);
unsigned short readindex(FILE* classFile);
unsigned char* readarray(unsigned short size, FILE* classFile);
void dumpOneConstantPoolEntry(unsigned short current, Const_Pool_Entry* cs_entries); 
