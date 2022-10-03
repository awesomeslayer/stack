#include "../includes/stack_functions.h"

int main()
{
    size_t error = 0;
    Stack_t main_stack = {};
    
    error = stk_ctor(main_stack);
    
    if(error == 0)
    {
        //____________EXAMPLE_________________
        Elem_t elem =  0;
        for(Elem_t i = 0; i < 64; i++)
        {
            error |= stk_push(&main_stack, i);
            printf("Pushed: %d \n", i);
        }

        for(int i = 0; i < 65 ; i++)
        {
            error |= stk_pop(&main_stack, &elem);
            printf("Popouted: %d \n", elem);
        }
        //_______________EXAMPLE_________________//

        error |= stk_dtor(&main_stack);
        print_error(error, stdout);
        return 0;
    }
    else
    {
        print_error(error, stdout);
        return 0;
    }
}