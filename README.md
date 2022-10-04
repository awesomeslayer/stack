### Stack

##Introduction
There are a library with Stack data structure realisation. It contains 3 levels of DEBUG_MODE and protection. Moreover, it can be used in some projects like CPU. 

##Modes
1. HASH_PROT - mode, which is protecting our structure with rot13 hash. If something goes wrong - dump and return signalizing us about it.
2. CANNARY_PROT - mode, which is protecting our structure with CANNARIES, whis is located before and after alloced memory. It is comparing in every function, and if something goes wrong - our programm signalizing us.
3. DEBUG_MODE - mode, which is turning on default protection and dump

##Finally...
You can use this library in any condition. It can work with any Elem_t and you can modify default sizes and constants as you with. 
