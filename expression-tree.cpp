/*
 * A program that converts an infix expression into a postfix expression.
 * Supports multi-digit numbers, +, -, *, /, ^, and parentheses.
 * Author: Artur Drobot
 * Feb. 2017
 */
#include <iostream>
#include "Stack.hpp"
#include <cstring>

using namespace std;

struct BinNode{
    char token;
    Node* left = NULL, *right = NULL;
};

char* getPostfix(char *infix);
bool isOperator(char token);
bool isLeft(char op);
int oppr(char op);
BinNode* getExprTree(stack &postfix);

int main(){
    char input[128];
    while(true){//Primary Loop
        cout << "Enter an infix expression (or \"q\" to quit): ";
        cin.getline(input, 128);
        //cout << endl;
        if(tolower(input[0]) == 'q'){
            cout << "Well fine then. Have a nice day." << endl;
            break;
        }
        
        char* postfix = getPostfix(input);
        Stack stack;
        for(int i=0; postfix[i]; i++){
            stack.push(postfix[i]);
        }
        delete[] postfix;
        BinNode* treeRoot = getExprTree(stack);
    }
    return 0;
}

char* getPostfix(char *infix){
    Stack stack;
    char* postfix = new char[strlen(infix) + 1];
    int indexin = 0, indexpost = 0;
    //The shunting-yard algorithm:
    while(infix[indexin]){
        if(infix[indexin] == ' '){
            //cout << "I see a space." << endl;
            indexin++;
            continue;
        }
        //Read a token. First check if it's a number.
        if(isdigit(infix[indexin])){
            //cout << infix[indexin] << " is a digit." << endl;
            while(isdigit(infix[indexin])){
                postfix[indexpost++] = infix[indexin++];
            }
            postfix[indexpost++] = ' ';
        }
        //We don't deal with function-related tokens.
        else if(isOperator(infix[indexin])){
            //cout << infix[indexin] << " is an operator." << endl;
            while(isOperator(stack.peek()) &&
               ( (isLeft(infix[indexin]) &&  oppr(infix[indexin]) <= oppr(stack.peek()))
            || (!isLeft(infix[indexin]) && oppr(infix[indexin]) < oppr(stack.peek())))){
                postfix[indexpost++] = stack.pop();
                postfix[indexpost++] = ' ';
            }
            stack.push(infix[indexin++]);
            //cout << "Stack peek: " << stack.peek() << endl;
        }
        else if(infix[indexin] == '('){
            //cout << infix[indexin] << " is a left parenthesis." << endl;
            stack.push(infix[indexin++]);
        }
        else if(infix[indexin] == ')'){
            //cout << infix[indexin] << " is a right parenthesis." << endl;
            while(stack.peek() != '('){
                postfix[indexpost++] = stack.pop();
                postfix[indexpost++] = ' ';
                if(stack.peek() == 0){
                    cout << "Error: mismatched parentheses." << endl;
                    break;
                }
            }
            stack.pop();
            //Again, we don't worry about function tokens.
            indexin++;
        }
    }
    //cout << "Reached end of infix string." << endl;
    //cout << "Stack peek: " << stack.peek() << endl;
    while(stack.peek()){
        postfix[indexpost++] = stack.pop();
        postfix[indexpost++] = ' ';
    }
    postfix[indexpost-1] = 0;
    return postfix;
}

bool isOperator(char token){
    return token == '+' || token == '-' || token =='*' || token =='/' || token == '^';
}

bool isLeft(char op){
    //Returns whether operator op, assumed to be one of the five we handle,
    //is left associative.
    return op != '^';
}

int oppr(char op){
    //Returns an int for operator op that can be used to compare operator presedence.
    if(op == '+' || op == '-'){
        return 1;
    }
    if(op == '*' || op == '/'){
        return 2;
    }
    if (op == '^'){
        return 3;
    }
    return -1;
}

BinNode* getExprTree(Stack &postfix){
    //Returns pointer to root of expression tree representing the given postfix expression.
    //(The top of the stack should be the right-most element.)
    if(isOperator(postfix.peek())){
        BinNode* binNode = new BinNode();
        binNode->token = postfix.pop();
        binNode->right = getExprTree(postfix);
        binNode->left = getExprTree(postfix);
        return binNode;
    }
    else{
        BinNode* binNode = new BinNode();
        binNode->token = postfix.pop();
        return binNode;
    }
}