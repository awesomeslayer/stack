#ifndef STKFNCS
#define STKFNCS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//____DEBUG_MODES
#define HASH_PROT
#define CANNARY_PROT
#define DEBUG_MODE //add

#define STK_SIZE 16

#ifndef CANNARY_PROT
    #define CANNARY_SIZE 0
#endif

#ifdef CANNARY_PROT
    #define CANNARY_SIZE sizeof(unsigned long long)
    const unsigned long long CANNARY = 0x98967F; 
#endif

#define ASSERTED != 0 || printf("ERROR");
#define stk_ctor(stk) stk_ctor_(&stk, STK_SIZE, __LINE__, #stk, __FUNCTION__, __FILE__)
#define do_stk_dump(stk, log, error) stk_dump(&stk, __FUNCTION__, __LINE__, __FILE__, log, error)
//#define macros for cannary temps

const int stk_resize_up_const = 2;
const int stk_resize_down_const = 4;

typedef int Elem_t;
const Elem_t POISONED_ELEM = 0x423A35C7;

typedef struct Stack_info_
{
    const char* name;
    const char* file;
    const char* function;
    int line;
    FILE* log;
} Stack_info;

typedef struct Stack
{
    unsigned long long LEFT_CANNARY;

    Elem_t* data;
    size_t size;
    size_t capacity;
    Stack_info info;

    unsigned int hash;
    
    unsigned long long RIGHT_CANNARY;
} Stack_t;

enum Errors
{
    STKPTR_ERROR = 1,
    DATAPTR_ERROR = 1 << 1,
    CPCT_ERROR = 1 << 2,
    SIZE_ERROR = 1 << 3,
    STK_CANNARY_ERROR = 1 << 4,
    HASH_ERROR = 1 << 5,
    LOGFILE_ERROR = 1 << 6,
    DATA_CANNARY_ERROR = 1 << 7,
    DATA_REALLOC_ERROR = 1 << 8,
    EMPTY_POP_ERROR = 1 << 9,
};

#ifdef HASH_PROT
    unsigned int rot13(void* ptr, size_t size);
#endif

void print_error(size_t error, FILE* output);

void set_stk_info(Stack* stk, const int line, const char* name, const char* function, const char* file, FILE* output);

size_t stk_verificator(Stack_t* stk);

void stk_dump(Stack* stk, const char* function, int line, const char* file, FILE* output, size_t error);

size_t stk_ctor_(Stack_t* stk, size_t size, const int line, const char* name, const char* function, const char* file);

size_t stk_dtor(Stack_t* stk);

size_t stk_push(Stack_t* stk, Elem_t value);

Elem_t stk_pop(Stack_t* stk);

size_t stk_resize_up(Stack* stk);

size_t stk_resize_down(Stack* stk);

#endif