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
            recParser(x->next);
    }

    QString recReturnEquation(const node * x, QString equation = "")
    {
        if(x != nullptr)
        {
            equation.append(x->data + ' ');
            return recReturnEquation(x->next, equation);
        }
        return equation;
    }

    void recCopy(node * oldNode)
    {
        if(oldNode != nullptr)
        {
            insert(oldNode->data);
            return recCopy(oldNode->next);
        }
    }

    node * head{nullptr}, * tail{nullptr};
    unsigned int numItems{0};
    QString answer;
    bool copy{false};
public:

    dLL() = default;

    dLL(const dLL& old) // "Deep copy". Not really, I think. Creates temporarily, since after this, the new dLL does its destructor.
    {
        recCopy(old.head);
        answer = old.answer;
        copy = true; // debug purposes
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

        //qDebug() << "inserted:" << tail->data << "\tat address: " << &tail->data;

        numItems++;
    }

    void remove()
    {
        if(head == nullptr)
            return;

        node * doomed = tail;
        //qDebug() << "removing a(n)" << ((copy == true) ? "copy" : "original") << ": " << doomed->data << "\tat address: " << &doomed->data; // Hmm... why does it do this twice? Problem with copier?

        if(head != tail)
        {
            tail = tail->prev;
            tail->next = nullptr;
            delete doomed;
        }
        else
        {
            delete doomed;
            head = nullptr;
        }

        --numItems;
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
        double first{answer.toDouble()},
               last{(tail->data).toDouble()};

        if(tail->prev->data == "+")
            answer = QString::number(first + last, 'g', 16);
        else if(tail->prev->data == "-")
            answer = QString::number(first - last, 'g', 16);
        else if(tail->prev->data == "*")
            answer = QString::number(first * last, 'g', 16);
        else if(tail->prev->data == "/")
            answer = QString::number(first / last, 'g', 16);

        return answer.toDouble();
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

    QString getAns() const
    {
        return answer;
    }

    void clear()
    {
        while(!isEmpty())
        {
            remove();
        }
        head = nullptr;
        //numItems = 0;
    }

    ~dLL()
    {
        qDebug() << "calling destructor for " << &head << " arithmetics.h";
        clear();
        qDebug() << "done calling destructor for arithmetics.h";
    }

};

}
#endif // NUMBERSTORER_H

