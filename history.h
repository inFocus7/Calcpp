#ifndef HISTORY_H
#define HISTORY_H
#include <QDebug>
#include <QPushButton>
#include <QPointer>
#include <QObject>
#include "arithmetics.h"
#include "globals.h"
#include "mainwindow.h"

// Might need to switch to array/vector to access history quickly when user clicks on an equation...
namespace datastore {

class dLL : public QObject
{
    /*
    Q_OBJECT

public slots:
    void recall()
    {
        qDebug() << "ok";
    }
*/
private:
    class node
    {
    public:
        maths::dLL data;
        node * next, * prev;
        QPointer<QPushButton> button;
        explicit node(const maths::dLL & x, QPointer<QPushButton> y)
        {
            next = nullptr;
            prev = nullptr;
            data = x;
            button = y;
            //connect(button, SIGNAL(released()), this, SLOT(datastore::dLL::recall()));
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

    QString insert(maths::dLL & data, QPointer<QPushButton> button)
    {
    node * tmp = new node(data, button);

    if(isFull())
    {
        //from history
        remove();

        // from historyStorage by shifting everything down to add a spot for new one.
    }

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
    return tail->data.returnEquation();
    }

    unsigned int getNumItems() const
    { return numItems; }

    unsigned int getMaxItems() const
    { return maxItems; }

    void remove()
    {
        if(head == nullptr)
            return;

        if(head != tail) // Head is earliest/oldest EQUATION.
        {
            node * doomed = head;
            doomed->button->deleteLater(); // why does this lead to a crash???
            head = head->next;
            head->prev = nullptr;
            delete doomed;
        }
        else
        {
            head->button->deleteLater();
            head = nullptr;
        }

        numItems--;
        qDebug() << "done removing from history";
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
        //qDebug() << "getting latest equation";
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
        {
            qDebug() << "there are still " << numItems << " left to remove.";
            remove();
        }
    }

    ~dLL()
    {
        qDebug() << "calling destructor for history.h";
        clear();
        qDebug() << "done calling destructor for history.h";
    }
};

}
#endif // HISTORY_H
