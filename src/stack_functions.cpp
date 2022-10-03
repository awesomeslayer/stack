#include "../includes/stack_functions.h"

#ifdef HASH_PROT
unsigned int rot13(void* ptr, size_t size)
{   
    unsigned int temp = 0;

    for(size_t i = 0; i < size; i++) 
    {    
        temp += (unsigned char) (*((char*)ptr + i));
        temp -= (temp >> 19) | (temp << 13);
    }

    return temp;
}
#endif

void print_error(size_t error, FILE* output)
{
    if(error == 1)
        fprintf(output, "STACK NULLPTR ERROR %lu\n", error);

    else
    {
        error /= 2;

        int i = 0;
        
        while(error)
        {
            i++;

            if(error % 2)
            {
                switch (i)
                {
                
                case 1:
                    fprintf(output, "STACK DATAPTR ERROR\n");
                    break;

                case 2:
                    fprintf(output, "STACK CAPACITY ERROR\n");
                    break;

                case 3:
                    fprintf(output, "STACK SIZE ERROR\n");
                    break;

                case 4:
                    fprintf(output, "STACK CANNARY ERROR\n");
                    break;

                case 5:
                    fprintf(output, "STACK HASH ERROR\n");
                    break;

                case 6:
                    fprintf(output, "LOGFILE ERROR\n");
                    break;

                case 7:
                    fprintf(output, "DATA CANNARY ERROR\n");
                    break;

                case 8:
                    fprintf(output, "DATA REALLOC ERROR\n");
                    break;

                case 9:
                    fprintf(output, "EMPTY POP ERROR\n");
                    break;

                default:
                    fprintf(output, "UNKNOWN ERROR CODE\n");
                    break;
                }
            }
            
            error /= 2;
        }
    }
    return;
} 


void set_stk_info(Stack* stk, const int line, const char* name, const char* function, const char* file, FILE* output)
{
    (line && name && function && file && output) ASSERTED;

    stk->info.file = file;
    stk->info.function = function;
    stk->info.log = output;
    stk->info.name = name;
    stk->info.line = line;
    return;
}

size_t stk_verificator(Stack_t* stk)
{   
    size_t error = 0;
    
    if(stk != NULL)
    {
        if(stk->data == NULL)
            error |= DATAPTR_ERROR;

        #ifdef CANNARY_PROT
            
            else if(*((unsigned long long*)((char*)stk->data-CANNARY_SIZE)) != CANNARY ||
                     *((unsigned long long*)((char*)stk->data+(sizeof(Elem_t))*stk->capacity)) != CANNARY) 
            {
                error |= DATA_CANNARY_ERROR;
            }

            if(stk->RIGHT_CANNARY != CANNARY || stk->LEFT_CANNARY != CANNARY)
                error |= STK_CANNARY_ERROR;
        #endif
        
        if(stk->size > stk->capacity)
            error |= SIZE_ERROR;

        if(stk->capacity <= 0)
            error |= CPCT_ERROR;

        #ifdef HASH_PROT
            if(stk->hash != rot13(stk->data, (stk->capacity)*sizeof(Elem_t)))
                error |= HASH_ERROR;
        #endif
        
        if(stk->info.log == NULL)
            error |= LOGFILE_ERROR;  
    }
    else 
    {
        error |= STKPTR_ERROR;
        return error;    
    }
    return error;
}

void stk_dump(Stack* stk, const char* function, int line, const char* file, FILE* output, size_t error)
{
    assert(function && line && file && output);

    fprintf(output, "DUMPED IN: %s at %s(%d):\n", function, file, line);
    fprintf(output, "STACK: %s[%p]\n", stk->info.name, (void*)stk);

    if(error == 0) 
        printf("STACK IS OK\n");
    else 
    {
        fprintf(output, "at FUNCTION: %s ", stk->info.function);
        fprintf(output, "IN FILE: %s ", stk->info.file);
        fprintf(output, "LINE:(%d)\n", stk->info.line);
        fprintf(output, "\nSTACK INFO:\n size = %lu;\n capacity = %lu;\n dataptr [%p]\n", stk->size, stk->capacity, (void*)(stk->data));
        
        #ifdef HASH_PROT
            fprintf(output, " hash = %u \n\n", stk->hash);
        #endif
        
        #ifdef CANNARY_PROT
            fprintf(output, "\t[-1] = %llu CANNARY\n",*((unsigned long long*)((char*)stk->data - CANNARY_SIZE)));
        #endif
        for(size_t i = 0; i < stk->size && i < stk->capacity; i++) 
            fprintf(output, "\t*[%lu] = %d\n", i, stk->data[i]);
        
        for(size_t i = stk->size; i < stk->capacity; i++) 
        {    
            fprintf(output, "\t[%lu] = %d ", i, stk->data[i]);
            if (stk->data[i] == POISONED_ELEM) fprintf(output, "(POISON)");
            fprintf(output, "\n");
        }

        #ifdef CANNARY_PROT
            fprintf(output, "\t[%lu] = %llu CANNARY\n", stk->capacity, *((unsigned long long*)((char*)stk->data + (sizeof(Elem_t))*stk->capacity)));
        #endif

        print_error(error, output);
        fprintf(output, "\n");
    }
}

size_t stk_ctor_(Stack_t* stk, size_t size, const int line, const char* name, const char* function, const char* file)
{
    size_t error = 0;
    FILE* logfile = fopen("../src/logfile.txt", "w");

    if(logfile == NULL)
    {
        error |= LOGFILE_ERROR;
        return error;
    }

    if(stk == NULL)
    {
        error |= STKPTR_ERROR;
        return error;
    }
    
    stk->size = 0;
    stk->capacity = size;

    if(stk->capacity > 0)
    { 
        stk->data = (Elem_t*)calloc(stk->capacity*(sizeof(Elem_t)) + 2*CANNARY_SIZE, sizeof(char));

        if(stk->data == NULL)
        {
            error |= DATAPTR_ERROR;
            return error;
        }

        #ifdef CANNARY_PROT
            stk->LEFT_CANNARY = CANNARY;
            stk->RIGHT_CANNARY = CANNARY;

            *((unsigned long long*)(stk->data)) = CANNARY;

            stk->data = (Elem_t*)((char*)stk->data + CANNARY_SIZE);

            *((unsigned long long *)((char*)stk->data + (sizeof(Elem_t))*stk->capacity)) = CANNARY;
        #endif

        for(size_t i = 0; i < stk->capacity; i++)     
            stk->data[i] = POISONED_ELEM;
        
        #ifdef HASH_PROT
            stk->hash = rot13(stk->data, (stk->capacity)*sizeof(Elem_t));
        #endif

        set_stk_info(stk, line, name, function, file, logfile);

        return error;
    }
    else
    {
        error |= CPCT_ERROR;
        return error;
    }
}

size_t stk_dtor(Stack_t* stk)
{
    size_t error = stk_verificator(stk);

    if(error == STKPTR_ERROR)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }

    stk->size = 0;
    stk->capacity = 0;
    
    #ifdef HASH_PROT
        stk->hash = 0;
    #endif
    
    fclose(stk->info.log);
    free((Elem_t*)((char*)stk->data - CANNARY_SIZE));
    
    return error;
}

size_t stk_push(Stack_t* stk, Elem_t value)
{
    size_t error = stk_verificator(stk);
    if(error != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }
    
    if(stk->size>= stk->capacity)
        error |= stk_resize_up(stk);
    
    stk->data[stk->size++] = value;
    
    #ifdef HASH_PROT
        stk->hash = rot13(stk->data, (stk->capacity)*sizeof(Elem_t));
    #endif
    
    //do_stk_dump(*stk, stk->info.log, DATA_REALLOC_ERROR) //example-review dump
    
    if((error = stk_verificator(stk)) != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }

    return error;
}

size_t stk_pop(Stack_t* stk, Elem_t* value)
{
    size_t error = 0;
    if((error = stk_verificator(stk)) != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }

    if(stk->size == 0)
    {
        error |= EMPTY_POP_ERROR;
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }

    *value = (stk->data)[stk->size - 1];
    stk->data[stk->size-1] = POISONED_ELEM;
    stk->size--;

    #ifdef HASH_PROT
        stk->hash = rot13(stk->data, (stk->capacity)*sizeof(Elem_t));
    #endif

    if(stk->size <= (stk->capacity)/stk_resize_down_const && stk->size != 0)
        error |= stk_resize_down(stk);

    //do_stk_dump(*stk, stk->info.log, DATA_REALLOC_ERROR) //example-review dump
    
    if((error = stk_verificator(stk)) != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }

    return error;

}

size_t stk_resize_up(Stack* stk)
{
    size_t error = 0;
    if((error = stk_verificator(stk)) != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }
    
    stk->data =(Elem_t*)((char*)stk->data - CANNARY_SIZE);

    void* temp = realloc(stk->data, sizeof(Elem_t)*(stk->capacity)*stk_resize_up_const + 2*CANNARY_SIZE);
    
    if(temp == NULL)
        return(DATA_REALLOC_ERROR);

    stk->data = (Elem_t*)temp;

    stk->data =(Elem_t*)((char*)stk->data + CANNARY_SIZE);

    for(size_t i = 0; i < stk->capacity*(stk_resize_up_const-1); i++)
        *(stk->data+stk->capacity+i) = POISONED_ELEM;
    
    stk->capacity = stk->capacity*stk_resize_up_const;
    
    #ifdef CANNARY_PROT
        *((unsigned long long*)((char*)stk->data + (sizeof(Elem_t))*stk->capacity)) = CANNARY;
    #endif

    #ifdef HASH_PROT
        stk->hash = rot13(stk->data, (stk->capacity)*sizeof(Elem_t));
    #endif
        
    if((error = stk_verificator(stk)) != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }

    return error;
}

size_t stk_resize_down(Stack* stk)
{
    size_t error = 0;

    if((error = stk_verificator(stk)) != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }
    
    stk->data = (Elem_t*)((char*)stk->data - CANNARY_SIZE);
    void* temp  = realloc(stk->data, sizeof(Elem_t)*(stk->capacity)/stk_resize_up_const + 2*CANNARY_SIZE);

    if(temp == NULL)
        return(DATA_REALLOC_ERROR);

    stk->data = (Elem_t*)temp;
    stk->data = (Elem_t*)((char*)stk->data + CANNARY_SIZE);
    stk->capacity = stk->capacity/stk_resize_up_const;

    #ifdef CANNARY_PROT
        *((unsigned long long*)((char*)stk->data + (sizeof(Elem_t))*stk->capacity)) = CANNARY;
    #endif

    #ifdef HASH_PROT
        stk->hash = rot13(stk->data, (stk->capacity)*sizeof(Elem_t));
    #endif

    if((error = stk_verificator(stk)) != 0)
    {
        do_stk_dump(*stk, stk->info.log, error);
        return error;
    }

    return error;

}