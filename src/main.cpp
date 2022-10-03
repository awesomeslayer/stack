#include "stack_functions.h"


int main()
{
    size_t error = 0;
    Stack_t main_stack = {};
    
    error = stk_ctor(main_stack);
    
    if(error == 0)
    {
        Elem_t elem = 123;
        stk_push(&main_stack, elem);
        error = stk_dtor(&main_stack);
        print_error(error, stdout);
        return 0;
    }
    else
    {
        print_error(error, stdout);
        return 0;
    }
}