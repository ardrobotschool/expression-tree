#include <iostream>
#include "StrStack.hpp"
#include <cstring>

char* StrStack::peek(){
    return head == NULL ? NULL : head->token;
}

void StrStack::push(char* token){
    StrNode* newNode = new StrNode();
    newNode->next = head;
    strcpy(newNode->token, token);
    head = newNode;
}

char* StrStack::pop(){
    if(head != NULL){
        char* temp = new char[10];
        strcpy(temp, head->token);
        StrNode* top = head;
        head = head->next;
        delete top;
        return temp;
    }
    else{
        return 0;
    }
}