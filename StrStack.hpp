//Stack with cstrings instead of chars class that uses a linked list internally; the head is the top of the stack.
#include <iostream>
#pragma once

struct StrNode{
    char token[10];
    StrNode* next = NULL;
};

class StrStack{
    public:
        char* peek();
        char* pop();
        void push(char* token);
    private:
        StrNode* head = NULL;
};