#ifndef HISTORY_H
#define HISTORY_H
#include <QDebug>
#include "arithmetics.h"

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
        explicit node(const maths::dLL & x)
        {
            next = nullptr;
            prev = nullptr;
            data = x;
        }
    };
    node * head{nullptr}, * tail{nullptr};
    unsigned int numItems{0}, maxItems{15};

    void recPrintAll(node * p) const
    {
        if(p != nullptr)
        {
            qDebug() << p->data.returnEquation();
            recPrintAll(p->next);
        }
    }

public:

    dLL() = default;

    QString insert(maths::dLL & data)
    {
    node * tmp = new node(data);

    if(isFull()) // first delete mainwindow pointer, then remove.
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
    return tail->data.returnEquation();
    }

    unsigned int getNumItems() const { return numItems; }

    unsigned int getMaxItems() const { return maxItems; }

    void remove()
    {
        qDebug() << "removing from history";
        if(head != tail) // Head is earliest/oldest EQUATION.
        {
            node * doomed = head;
            head = head->next;
            head->prev = nullptr;
            delete doomed;
        }
        else
            head = nullptr;

        numItems--;
    }

    void remove(int index)
    {
        node * tmp{ head };
        for(int i{0}; i != index && tmp != nullptr; ++i)
            tmp = tmp->next;

        if(tmp == head)
        {
            head = head->next;
            head->prev = nullptr;
        }
        else if(tmp == tail)
        {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
        {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
        }

        delete tmp;
        --numItems;
    }

    bool isFull() const
    {
        if(numItems == maxItems)
            return true;
        return false;
    }

    QString latestEquation() const
    {
        qDebug() << "getting latest equation";
        return tail->data.returnEquation();
    }

    void printAll() const { /*recPrintAll(head);*/ }

    bool isEmpty() const
    {
        if(numItems == 0)
            return true;
        return false;
    }

    void clear()
    {
        while (!isEmpty())
            remove();

        head = nullptr;
        numItems = 0;
    }

    ~dLL()
    {
        qDebug() << "calling destructor for " << &head << " history.h";
        clear();
        qDebug() << "done calling destructor for history.h";
    }
};

}
#endif // HISTORY_H
