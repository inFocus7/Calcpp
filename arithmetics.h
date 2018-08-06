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
        explicit node(QString x)
        {
            next = nullptr;
            prev = nullptr;
            data = x;
        }
    };

    void recParser(node * x) const
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

    node * head{nullptr}, * tail{nullptr};
    unsigned int numItems{0};
    QString answer;

public:

    dLL() = default;

    dLL(const dLL& old) //"Deep copy". Not really, I think.
    {
        recCopy(old.head, *this, old.answer);
    }

    void insert(QString x)
    {
        node *tmp = new node(x);

        if(head == nullptr)
        {
            head = tmp;
            tail = tmp;
            answer = x;
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
            head = NULL;

        numItems--;
    }

    bool isEmpty() const
    {
    if(numItems == 0)
        return true;
    return false;
    }

    bool ready2math() const
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

        answer = QString::number(output);

        return output;
    }

    void parse() const
    {
        recParser(head);
    }

    QString returnEquation()
    {
        return recReturnEquation(head);
    }

    QString getTail() const
    {
        return tail->data;
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

