#ifndef NUMBERSTORER_H
#define NUMBERSTORER_H
#include <QDebug>

/* ENDGAME:
 * Have a Linked List Equation:
 * nullptr<->[#]<->[OP]<->...<->[OP]<->[#]<->nullptr
 *            ^h                        ^t
 */

namespace maths {

class dLL
{
private:

    class node
    {
    public:
        QString data;
        node * next, * prev;
        node(QString x)
        {
            next = nullptr;
            prev = nullptr;
            data = x;
        }
    };

    void recParser(node * x)
    {
        if(x != nullptr)
        {
            //qDebug() << x->data;
            recParser(x->next);
        }
    }

    QString recReturnEquation(node * x, QString equation = "")
    {
        if(x != nullptr)
        {
            equation.append(x->data + ' ');
            return recReturnEquation(x->next, equation);
        }
        return equation;
    }

    void recCopy(node * oldNodes, dLL & nue, QString finalAns)
    {
        nue.insert(oldNodes->data);
        if(oldNodes->next != nullptr)
        {
            recCopy(oldNodes->next, nue, finalAns);
        }
        nue.answer = finalAns;
    }

    node * head, * tail;
    unsigned int numItems;
    QString answer;

public:

    dLL()
    {
        numItems = 0;
        head = nullptr;
        tail = nullptr;
    }

    dLL(const dLL& old) //deep copy.
    {
        numItems = 0;
        head = nullptr;
        tail = head;

        recCopy(old.head, *this, old.answer);
    }

    void insert(QString x)
    {
        node *tmp = new node(x);

        if(head == nullptr)
        {
            head = tmp;
            tail = tmp;
            answer = x; //might need edit to NOT do this when using deep copy...
        }
        else
        {
            tail->next = tmp;
            tmp->prev = tail;
            tail = tmp;
        }

        numItems++;

    }

    void remove()
    {
        if(head != tail)
        {
            node * doomed = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete doomed;
        }
        else
        {
            head = NULL;
        }
        numItems--;
    }

    bool isEmpty()
    {
    if(numItems == 0)
        return true;
    return false;
    }

    bool ready2math()
    {
    if(numItems > 1 && numItems%2 != 0)
        return true;
    return false;
    }

    void replaceOperator(QString newOp)
    {
        tail->data = newOp;
    }

    double solve()
    {
        double output{0},
               first{answer.toDouble()},
               last{(tail->data).toDouble()};

        if(tail->prev->data == "+")
        {
            output = first + last;
        }
        else if(tail->prev->data == "-")
        {
            output = first - last;
        }
        else if(tail->prev->data == "*")
        {
            output = first * last;
        }
        else if(tail->prev->data == "/")
        {
            output = first/last;
        }

        /*
        head->number = QString::number(output, 'g', 15);

        while(head != tail)
        {
            remove();
        }
        */

        answer = QString::number(output);

        return output;
    }

    void parse()
    {
        recParser(head);
    }

    QString returnEquation()
    {
        return recReturnEquation(head);
    }

    void clear()
    {
        while(!isEmpty())
        {
            remove();
        }
        head = nullptr;
    }

    ~dLL()
    {
        clear();
    }

};

}
#endif // NUMBERSTORER_H

