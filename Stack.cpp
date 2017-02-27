#include <iostream>
#include "Stack.hpp"

char Stack::peek(){
    return head == NULL ? '\0' : head->token;
}

void Stack::push(char token){
    Node* newNode = new Node();
    newNode->next = head;
    newNode->token = token;
    head = newNode;
}

char Stack::pop(){
    if(head != NULL){
        char temp = head->token;
        Node* top = head;
        head = head->next;
        delete top;
        return temp;
    }
    else{
        return 0;
    }
}