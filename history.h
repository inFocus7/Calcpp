#ifndef HISTORY_H
#define HISTORY_H
#include <QDebug>
#include "arithmetics.h"

// When using for history storage: save as equation form, then parse (not sure if possible yet)
// Might need to switch to array/vector to access history quickly when user clicks on an equation...
namespace datastore {

class dLL
{
private:
    class node
    {
    public:
        maths::dLL data;
        node * next, * prev;
        node(maths::dLL & x) // Why
        {
            next = nullptr;
            prev = nullptr;
            data = x;
        }
    };
    node * head, * tail;
    unsigned int numItems, maxItems;

public:
    dLL()
    {
        numItems = 0;
        maxItems = 15;
    }

    QString insert(maths::dLL data) // Returns equation to inset into history button.
    {
    node * tmp = new node(data);

    if(numItems == maxItems)
        remove();

    if(head == nullptr)
    {
        head = tmp;
        tail = tmp;
    }
    else
    {
        tail->next = tmp;
        tmp->prev = tail;
        tail = tmp;
    }

    numItems++;

    // Insert latest data into History.
    // Create a new qpushbutton in the scrollarea with parse as text.
    return tail->data.returnEquation();
    }

    unsigned int returnNumItems()
    {
        return numItems;
    }

    void remove() // Removes front.
    {
        if(head != tail)
        {
            node * doomed = head;
            head = head->next;
            head->prev = nullptr;
            delete doomed;
        }
        else
        {
            head = nullptr;
        }
        numItems--;
    }

    bool isEmpty()
    {
        if(numItems == 0)
            return true;
        return false;
    }

    ~dLL()
    {
        while(!isEmpty())
        {
            remove();
        }
    }
};

}
#endif // HISTORY_H
