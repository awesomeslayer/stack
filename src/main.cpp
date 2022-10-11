#include "../includes/stack_functions.h"

int main()
{
    Stack_t main_stack = {};
    
    if(!stk_ctor(main_stack))   
    {
        //____________EXAMPLE___________________//
        Elem_t elem;

        for(Elem_t i = 0; i < 64; i++)
        {
            stk_push(&main_stack, i);
            printf("Pushed: %d \n", i);
        }

        for(int i = 0; i < 66 ; i++)
        {
            elem = stk_pop(&main_stack);

            if(elem != POISONED_ELEM)
                printf("Popouted: %d \n", elem);
            else 
                printf("NOTHING TO POP\n");
        }
        //_______________EXAMPLE_________________//

        stk_dtor(&main_stack);
        return 0;
    }
    else
    {
        printf("Bad ctor, retry\n");
        return 0;
    }
}