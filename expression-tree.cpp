/*
 * A program that converts an infix expression into a postfix expression.
 * Supports multi-digit numbers, +, -, *, /, ^, and parentheses.
 * The program then builds an expression tree and allows the user
 * to display the original expression in infix, postfix, or prefix
 * notation (though prefix is the only notation into which the conversion
 * is not redundant).
 * Author: Artur Drobot
 * Mar 2, 2017
 */
#include <iostream>
#include "Stack.hpp"
#include "StrStack.hpp"
#include <cstring>

using namespace std;

struct BinNode{
    char* token;
    BinNode* left = NULL, *right = NULL;
    ~BinNode(){
        delete[] token;
    }
};

char* getPostfix(char *infix);
bool isOperator(char token);
bool isOperator(char* token);
bool isOperator(BinNode* binNode);
bool isLeft(char op);
int oppr(char op);
int oppr(char* op);
int oppr(BinNode* binNode);
BinNode* getExprTree(StrStack &postfix);
void printPrefix(BinNode* root);
void printPostfix(BinNode* root);
void printInfix(BinNode* root);
void deleteBinTree(BinNode* root);

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
        StrStack stack;
        int i = 0;
        while(postfix[i]){
            if(postfix[i] == ' '){
                i++;
                continue;
            }
            if(isdigit(postfix[i])){
                char num[10];
                int j = 0;
                while(isdigit(postfix[i])){
                    num[j++] = postfix[i++];
                }
                num[j++] = '\0';
                stack.push(num);
            }
            else{//Operator
                char op[2] = {postfix[i++], '\0'};
                stack.push(op);
            }
        }
        delete[] postfix;
        BinNode* treeRoot = getExprTree(stack); //Expression tree
        cout << "Enter \"prefix,\" \"postfix,\" or \"infix.\"" << endl;
        cin.getline(input, 128);
        //Convert to lower case:
        for(int i=0; input[i]; i++){
            input[i] = tolower(input[i]);
        }
        if(strcmp(input, "prefix") == 0){
            printPrefix(treeRoot);
            cout << endl;
        }
        else if(strcmp(input, "postfix") == 0){
            printPostfix(treeRoot);
            cout << endl;
        }
        else if(strcmp(input, "infix") == 0){
            printInfix(treeRoot);
            cout << endl;
        }
        else{
            cout << "Command not recognized." << endl;
        }
        deleteBinTree(treeRoot);
    }
    return 0;
}

char* getPostfix(char *infix){
    Stack stack;
    char* postfix = new char[((strlen(infix)+1)*3)]; //Output may be without spaces; allocate enough memory.
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

BinNode* getExprTree(StrStack &postfix){
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

void printPrefix(BinNode* root){
    if(isOperator(root->token)){
        cout << root->token << ' ';
        printPrefix(root->left);
        printPrefix(root->right);
    }
    else{
        cout << root->token << ' ';
    }
}

void printPostfix(BinNode* root){
    if(isOperator(root->token)){
        printPostfix(root->left);
        printPostfix(root->right);
        cout << root->token << ' ';
    }
    else{
        cout << root->token << ' ';
    }
}

void printInfix(BinNode* root){
    if(isOperator(root->token)){
        if(isOperator(root->left)){
            cout << "( ";
            printInfix(root->left);
            cout << ") ";
        }
        else{
            printInfix(root->left);
        }
        cout << root->token << ' ';
        if(isOperator(root->right)){
            cout << "( ";
            printInfix(root->right);
            cout << ") ";
        }
        else{
            printInfix(root->right);
        }
    }
    else{
        cout << root->token << ' ';
    }
}

int oppr(BinNode* binNode){
    return oppr(binNode->token);
}

bool isOperator(BinNode* binNode){
    return isOperator(binNode->token);
}

int oppr(char* token){
    return oppr(*token);
}

bool isOperator(char* token){
    return isOperator(*token);
}

void deleteBinTree(BinNode* root){
    if(root != 0){
        deleteBinTree(root->left);
        deleteBinTree(root->right);
        delete root;
    }
}