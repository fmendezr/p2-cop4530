#include "NotationConverter.hpp"

template <typename T>
class Deque;

template <typename T>
class DequeNode
{
public:
    friend class Deque<T>;
    DequeNode<T>() : data(), next(nullptr), prev(nullptr) {}
    DequeNode<T>(T d) : data(d), next(nullptr), prev(nullptr) {}
private:
    T data;
    DequeNode<T>* next;
    DequeNode<T>* prev;
};

template <typename T>
class Deque
{
public:
    Deque() : head(new DequeNode<T>()), tail(new DequeNode<T>()), size(0) 
    {
        head->next = tail;
        tail->prev = head;
    }
    
    ~Deque<T>()
    {
        // delete all nodes
        while (!empty()) { removeFromTail(); }
        
        // delete sentinel nodes
        delete head;
        delete tail;
    }
    
    bool empty() { return head->next == tail; }
    
    int getSize() { return size; }
        
    void addToHead(T value)
    {
        // create & setup new node
        DequeNode<T>* newNode =  new DequeNode<T>(value);
        newNode->prev = head;
        newNode->next = head->next;
        
        // update nodes that were already in deque
        head->next = newNode;
        newNode->next->prev = newNode;
        size++;
    }
    
    void addToTail(T value)
    {
        // create & setup new node
        DequeNode<T>* newNode = new DequeNode<T>(value);
        newNode->next = tail;
        newNode->prev = tail->prev;
        
        // update nodes that were already in deque
        tail->prev = newNode;
        newNode->prev->next = newNode;
        size++;
    }
    
    T removeFromHead()
    {
        // handle case were empty deque
        if (empty()) { return T(); }
        
        // catch node that needs to be deleted
        DequeNode<T>* nodeToDelete = head->next;
        T data = nodeToDelete->data;
        
        // update link list to not include node to be deleted
        nodeToDelete->prev->next = nodeToDelete->next;
        nodeToDelete->next->prev = nodeToDelete->prev;
        size--;
        
        // free memory
        delete nodeToDelete;
        
        return data;
    }
    
    T removeFromTail()
    {
        // handle case were empty deque
        if (empty()) { return T(); }
        
        // catch node that needs to be deleted
        DequeNode<T>* nodeToDelete = tail->prev;
        T data = nodeToDelete->data;
        
        // update link list to not include node to be deleted
        nodeToDelete->prev->next = nodeToDelete->next;
        nodeToDelete->next->prev = nodeToDelete->prev;
        size--;
        
        // free memory
        delete nodeToDelete;
        
        return data;
    }
    
private:
    //sentinel nodes
    DequeNode<T>* head;
    DequeNode<T>* tail;
    int size;
};

class NotationConverter : public NotationConverterInterface 
{
public:
    std::string postfixToInfix(std::string inStr) override 
    {
        Deque<std::string> numbersStack;
        std::string infixString;
        
        for (char c : inStr)
        {
            if (c == ' ') { continue; }
            else if (c == '+' || c == '-' || c == '*' || c == '/')
            {
                std::string n1 = numbersStack.removeFromHead();
                std::string n2 = numbersStack.removeFromHead();
                infixString = "(" + n2 + c + n1 + ")";
                numbersStack.addToHead(infixString);
            }
            else 
            {
                // convert char into str and add it to stack
                std::string str = "";
                numbersStack.addToHead(str + c);
            }
        }
        
        return numbersStack.removeFromHead();
    }

    std::string postfixToPrefix(std::string inStr) override 
    {
        return infixToPrefix(postfixToInfix(inStr));
    }

    std::string infixToPostfix(std::string inStr) override {
        Deque<char> operandStack;
        Deque<std::string> numbersStack;
        std::string postfixString;
        
        for (char c : inStr)
        {
            
            if (c == ' ') { continue; }
            else if (c == ')')
            {
                std::string n1 = numbersStack.removeFromHead();
                std::string n2 = numbersStack.removeFromHead();
                postfixString = n2 + n1 + operandStack.removeFromHead();
                numbersStack.addToHead(postfixString);
                
                // ditch parenthesis
                operandStack.removeFromHead();
            }
            else if (c == '(' || c == '+' || c == '-' || c == '*' || c == '/') { operandStack.addToHead(c); }
            else
            {
                // convert char into str and add it to stack
                std::string str = "";
                numbersStack.addToHead(str + c);
            }
        }
        
        return numbersStack.removeFromHead();
    }

    std::string infixToPrefix(std::string inStr) override 
    {
        Deque<char> operandStack;
        Deque<std::string> numbersStack;
        std::string prefixString;
        
        for (char c : inStr)
        {
            
            if (c == ' ') { continue; }
            else if (c == ')')
            {
                std::string n1 = numbersStack.removeFromHead();
                std::string n2 = numbersStack.removeFromHead();
                prefixString = operandStack.removeFromHead() + n2 + n1;
                numbersStack.addToHead(prefixString);
                
                // ditch parenthesis
                operandStack.removeFromHead();
            }
            else if (c == '(' || c == '+' || c == '-' || c == '*' || c == '/') { operandStack.addToHead(c); }
            else
            {
                // convert char into str and add it to stack
                std::string str = "";
                numbersStack.addToHead(str + c);
            }
        }
        
        return numbersStack.removeFromHead();
    }

    std::string prefixToInfix(std::string inStr) override 
    {
        Deque<std::string> numbersStack;
        std::string infixString;
        char c;
        
        for (int i = int(inStr.size())-1; i > -1; i--)
        {
            c = inStr[i];
            if (c == ' ') { continue; }
            else if (c == '+' || c == '-' || c == '*' || c == '/')
            {
                std::string n1 = numbersStack.removeFromHead();
                std::string n2 = numbersStack.removeFromHead();
                infixString = "(" + n1 + c + n2 + ")";
                numbersStack.addToHead(infixString);
            }
            else
            {
                // convert char into str and add it to stack
                std::string str = "";
                numbersStack.addToHead(str + c);
            }
        }
        
        return infixString;
    }

    std::string prefixToPostfix(std::string inStr) override 
    {
        return infixToPostfix(prefixToInfix(inStr));
    }


};
