#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "history.h"
#include "xtrafunctions.h"
#include <QShortcut>
#include <QPropertyAnimation>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QGraphicsBlurEffect>
#include <initializer_list>

// GLOBAL VARS.
int initOutputSize{40};
maths::dLL EQUATION;
datastore::dLL HISTORY;
QString recentNumInput,
        recentAnswer;
Qt::WindowFlags flags;
bool arithmitted{false},
     prevOp{false},
     decimalExists{false},
     negated{false},
     solved{false},
     cont{false};
unsigned int overallItems{0};
QPushButton ** historyStore = new QPushButton*[15];

//Qt::FramelessWindowHint
//Qt::WindowStaysOnTopHint
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Shortcuts
    ui->decimal->setShortcut(Qt::Key_Period);
    ui->num0->setShortcut(Qt::Key_0);
    ui->num1->setShortcut(Qt::Key_1);
    ui->num2->setShortcut(Qt::Key_2);
    ui->num3->setShortcut(Qt::Key_3);
    ui->num4->setShortcut(Qt::Key_4);
    ui->num5->setShortcut(Qt::Key_5);
    ui->num6->setShortcut(Qt::Key_6);
    ui->num7->setShortcut(Qt::Key_7);
    ui->num8->setShortcut(Qt::Key_8);
    ui->num9->setShortcut(Qt::Key_9);
    ui->actClear->setShortcut(Qt::Key_Delete);
    ui->actBack->setShortcut(Qt::Key_Backspace);
    ui->signAdd->setShortcut(Qt::Key_Plus);
    ui->signSub->setShortcut(Qt::Key_Minus);
    ui->signDiv->setShortcut(Qt::Key_Slash);
    ui->signMult->setShortcut(Qt::Key_Asterisk);
    ui->signEquals->setShortcut(Qt::Key_Enter);

    // Signal Connectors
    connect(ui->num0, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num1, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num2, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num3, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num4, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num5, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num6, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num7, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num8, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->num9, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->decimal, SIGNAL(released()), this, SLOT(digitPressed()));
    connect(ui->actClear, SIGNAL(released()), this, SLOT(clear()));
    connect(ui->actBack, SIGNAL(released()), this, SLOT(backspace()));
    connect(ui->signAdd, &QPushButton::released, this, [this]{arithmetics(0);});
    connect(ui->signSub, &QPushButton::released, this, [this]{arithmetics(1);});
    connect(ui->signMult, &QPushButton::released, this, [this]{arithmetics(2);});
    connect(ui->signDiv, &QPushButton::released, this, [this]{arithmetics(3);});
    connect(ui->signEquals, &QPushButton::released, this, [this]{arithmetics(4);});
    connect(ui->actSettings, &QPushButton::released, this, [this]{ocSettings(true);});
    connect(ui->actHistory, SIGNAL(released()), this, SLOT(ocHistory()));
    connect(ui->s_github, &QPushButton::released, this, [this]{openSocial(QUrl("https://www.github.com/infocus7/"));});
    connect(ui->s_linkedin, &QPushButton::released, this, [this]{openSocial(QUrl("https://www.linkedin.com/in/fabiangonz98/"));});
    connect(ui->s_twitter, &QPushButton::released, this, [this]{openSocial(QUrl("https://www.twitter.com/fabiangonz98/"));});
    connect(ui->signNegate, SIGNAL(released()), this, SLOT(negateNum()));
    connect(ui->flag_OnTop, SIGNAL(toggled(bool)), this, SLOT(updatePreview()));
    connect(ui->aboutSettings, &QPushButton::released, this, [this]{gotoSetting(4);});
    connect(ui->generalSettings, &QPushButton::released, this, [this]{gotoSetting(1);});
    connect(ui->memorySettings, &QPushButton::released, this, [this]{gotoSetting(3);});
    connect(ui->visualSettings, &QPushButton::released, this, [this]{gotoSetting(2);});
    connect(ui->resetSettings, SIGNAL(released()), this, SLOT(reset()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateScreen(double s_num, QString t_num = "")
{
    if(t_num != "")
    {
        ui->screenOutput->setText(t_num);
    }
    else
    {
        QString s_text{QString::number(s_num, 'g', 16)};
        ui->screenOutput->setText(s_text);
    }
    resizeScreen();
}

void MainWindow::updateEquation(QString string)
{
    ui->equationScreen->setText(string);
    ui->equationScreen->setToolTip(ui->equationScreen->text());
}

void MainWindow::digitPressed()
{
    if(arithmitted)
    {
        clearScreen();
        recentNumInput.clear();
        arithmitted = false;
    }
    if(solved)
    {
        if(!cont)
            ui->equationScreen->clear();
        solved = false;
    }

    if(!(!solved || (ui->equationScreen->text().isEmpty()) || (xtra::is_in(ui->equationScreen->text().back(), {"+", "-", "/", "*"}))))
        ui->equationScreen->clear();

    QPushButton *button = (QPushButton*)sender();

    // Doesn't allow any zero to be inserted. I should allow a singular 0 before "."
    // Tried .startsWith("00"); but doesn't work.
    if((!(ui->equationScreen->text().isEmpty()) && ui->equationScreen->text().back() == "0" && button->text() == "0" && ui->screenOutput->text().length() <= 1) || ui->screenOutput->text().length() == 16)
        return;

    if(button->text() == ".")
    {
        if(decimalExists)
            return;
        else
            decimalExists = true;
    }

    updateEquation((ui->equationScreen->text().append(button->text())));
    recentNumInput.append(button->text());
    updateScreen(recentNumInput.toDouble(), recentNumInput);
    solved = false;
}

void MainWindow::backspace()
{
    if(arithmitted)
    {
        return;
    }

    if(ui->equationScreen->text() != "" && !(xtra::is_in(ui->equationScreen->text().back(), {'+', '-', '/', '*', '%'})) || (ui->screenOutput->text().at(0) != '-' && negated))
    {
        if(ui->equationScreen->text().back() == ".")
            decimalExists = false;
        if(ui->equationScreen->text().back() == "-")
            negated = false;

        QString number{ui->screenOutput->text()};
        number.chop(1);
        updateScreen(number.toDouble(), number);

        QString eq{ui->equationScreen->text()};
        eq.chop(1);
        updateEquation(eq);

        recentNumInput.chop(1);
    }
}

void MainWindow::reset()
{
    clear();
    HISTORY.clear();
    ui->answerOutput->clear();
}

void MainWindow::clear()
{
    EQUATION.clear();
    ui->screenOutput->clear();
    ui->equationScreen->clear();
    recentNumInput.clear();
    decimalExists = false;
    negated = false;
    // erase created buttons.
}

void MainWindow::clearScreen()
{
    QString number{ui->screenOutput->text()};
    number.clear();
    recentNumInput.clear();
    updateScreen(number.toDouble());
}

void MainWindow::arithmetics(unsigned int operation)
{
    QLabel * eqScreen{ui->equationScreen};
    if(eqScreen->text().isEmpty() || (negated && eqScreen->text().back() == "-"))
        return;
    bool alteredOperation{false};
    QPushButton * button = (QPushButton*)sender();

    if(operation == 4) // "="
    {
        if(!recentNumInput.isEmpty())
            EQUATION.insert(recentNumInput);
        if(!recentAnswer.isEmpty() && solved)
            EQUATION.insert(recentAnswer);

        if(EQUATION.ready2math())
        {
            updateScreen(EQUATION.solve());
            ui->answerOutput->setText(ui->screenOutput->text());
        }
        else if(xtra::is_in(EQUATION.getTail(), {'+', '-', '/', '*'})) // Removes hanging operation.
            EQUATION.remove();

        // Makes a new qpushbutton in history storing equation data.
        ++overallItems;
        maths::dLL inEquation(EQUATION);
        QPushButton * historyButton = new QPushButton(HISTORY.insert(inEquation), ui->HISTORY);
        qDebug() << "latest equation: " << HISTORY.latestEquation();
        // when you click historyButton bring up equation, show final answer on screen; show/use full equation by clearing whatevers onscreen.
        historyButton->setFixedHeight(51);
        historyButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        ui->historyLayout->insertWidget(0, historyButton);
        if(ui->historyArea->height() < 460)
            ui->historyArea->setGeometry(QRect(0, 50, 241, 51 * overallItems));
        QString tooltipText{historyButton->text()};
        tooltipText = tooltipText.append(" = " + EQUATION.getAns());
        historyButton->setToolTip(tooltipText);
        historyButton->show();

        qDebug() << "about to clear main equation.";
        EQUATION.clear();
        qDebug() << "done clearing main equation.";
        qDebug() << "about to print all EQUATIONS.";
        //HISTORY.printAll();
        qDebug() << "done printing all EQUATIONS.";

        recentAnswer = ui->screenOutput->text();
        updateEquation(recentAnswer);
        solved = true;
        cont = false;
        }
    else
    {
        if(eqScreen->text() != NULL && xtra::is_in(eqScreen->text().back(), {'+', '-', '/', '*', '%'}) && ((!negated || (negated && eqScreen->text().back() != "-"))))
            alteredOperation = true;

        if(!(!solved || recentAnswer.isEmpty() || alteredOperation))
        {
            EQUATION.insert(recentAnswer);
            cont = true;
        }

        if(!(recentNumInput.isEmpty() || alteredOperation))
            EQUATION.insert(recentNumInput);

        if(alteredOperation)
        {
            QString eq{eqScreen->text()};
            eq.chop(1);
            updateEquation(eq);
            EQUATION.replaceOperator(button->text());
        }

        eqScreen->setText(eqScreen->text().append(button->text()));

        if(EQUATION.ready2math())
        {
            updateScreen(EQUATION.solve());
            ui->answerOutput->setText(ui->screenOutput->text());
        }

        if(!alteredOperation)
        {
            EQUATION.insert(button->text());
        }
    }

    recentNumInput.clear();
    arithmitted = true;
    decimalExists = false;
    negated = false;
}

void MainWindow::resizeScreen()
{
    QFont myFont(ui->screenOutput->font());
    QFontMetrics fm(myFont);
    QRect bound = fm.boundingRect(0,0,ui->screenOutput->width(),ui->screenOutput->height(),Qt::AlignRight, ui->screenOutput->text());

        // If text longer than width of Output Screen.
        if(bound.width() > ui->screenOutput->width())
        {
            while(bound.width() > ui->screenOutput->width())
            {
                QFontMetrics fm(myFont);
                bound = fm.boundingRect(0,0,ui->screenOutput->width(),ui->screenOutput->height(),Qt::AlignRight, ui->screenOutput->text());
                myFont.setPointSize(myFont.pointSize() - 1);
            }
        }
        // If text shorter than width of output screen & size smaller than 40pt.
        else if(bound.width() < ui->screenOutput->width() && myFont.pointSize() < 40)
        {
            while(bound.width() < ui->screenOutput->width() && myFont.pointSize() < 40)
            {
                QFontMetrics fm(myFont);
                bound = fm.boundingRect(0,0,ui->screenOutput->width(),ui->screenOutput->height(),Qt::AlignRight, ui->screenOutput->text());
                myFont.setPointSize(myFont.pointSize() + 1);
            }
            // Fixes over/under-size issue when text is too long and pt gets altered.
            if(bound.width() > ui->screenOutput->width())
            {
                while(bound.width() > ui->screenOutput->width())
                {
                    QFontMetrics fm(myFont);
                    bound = fm.boundingRect(0,0,ui->screenOutput->width(),ui->screenOutput->height(),Qt::AlignRight, ui->screenOutput->text());
                    myFont.setPointSize(myFont.pointSize() - 1);
                }

            }
        }

    ui->screenOutput->setFont(myFont);
}

void MainWindow::negateNum()
{
    solved = false;
    QString num{ui->screenOutput->text()};
    if(ui->equationScreen->text().length() >= 1 && !(xtra::is_in(ui->equationScreen->text().back(), {'+', '-', '/', '*', '%'})))
    {
        QString newEq{ui->equationScreen->text()};
        // If positive
        if(ui->screenOutput->text().at(0) != '-')
        {
            QString newNum{num.prepend('-')};

            updateScreen(newNum.toDouble());
            int toDelete = newNum.length() - 1;
            newEq.chop(toDelete);
            updateEquation(newEq.append(newNum));

            recentNumInput = newNum;
            negated = true;
        }
        // If negative
        else
        {
            QString newNum{num.remove(0, 1)};

            updateScreen(newNum.toDouble());
            int toDelete = newNum.length() + 1;
            newEq.chop(toDelete);
            updateEquation(newEq.append(newNum));

            recentNumInput = newNum;
            negated = false;
        }
    }
}

void MainWindow::openSocial(QUrl website)
{
    QDesktopServices::openUrl(website);
}

void MainWindow::ocSettings(bool uiButtonPressed)
{
    if(ui->settingsScreen->currentIndex() != 0 && uiButtonPressed)
    {
        ui->settingsScreen->setCurrentIndex(0);
        ui->actSettings->setIcon(QIcon());
        ui->actSettings->setText("⋮");
        return;
    }

    QPropertyAnimation *animation = new QPropertyAnimation(ui->settingsScreen, "geometry");
    animation->setDuration(100);
    QGraphicsBlurEffect * blur{new QGraphicsBlurEffect}, * blur2{new QGraphicsBlurEffect};
    blur->setBlurHints(QGraphicsBlurEffect::QualityHint);
    blur2->setBlurHints(QGraphicsBlurEffect::QualityHint);
    bool isOpen{false};

    if(ui->settingsScreen->x() == 0)
        isOpen = true;


    if(!isOpen)
    {
        animation->setKeyValueAt(0, QRect(-211, 0, 211, 701));
        animation->setKeyValueAt(1, QRect(0, 0, 211, 701));
        ui->actSettings->setStyleSheet("QPushButton { font: 75 20pt \"Century Gothic\"; background-color: none; border: none; border-bottom: 3px solid rgba(231, 4, 91, 225); }"
                                       "QPushButton:hover:!pressed { border-bottom: 3px solid rgba(231, 4, 91, 150); }"
                                       "QPushButton:hover:pressed { border-bottom: 3px solid rgba(231, 4, 91, 75); }"
                                       "QPushButton:pressed { border-bottom: 3px solid rgba(231, 4, 91, 75); }");
        ui->mainScreen->setEnabled(false);
        ui->historyScreen->setEnabled(false);
        ui->settingsScreen->setCurrentIndex(0);
    }
    else
    {
        blur->setBlurRadius(0);
        blur2->setBlurRadius(0);
        animation->setKeyValueAt(0, QRect(0, 0, 211, 701));
        animation->setKeyValueAt(1, QRect(-211, 0, 211, 701));
        ui->actSettings->setStyleSheet("QPushButton { font: 75 20pt \"Century Gothic\"; background-color: none; border: none; border-bottom: 3px solid rgba(255, 255, 255, 225); }"
                                       "QPushButton:hover:!pressed { border-bottom: 3px solid rgba(255, 255, 255, 150); }"
                                       "QPushButton:hover:pressed { border-bottom: 3px solid rgba(255, 255, 255, 75); }"
                                       "QPushButton:pressed { border-bottom: 3px solid rgba(255, 255, 255, 75); }");
        ui->mainScreen->setEnabled(true);
        ui->historyScreen->setEnabled(true);
    }

    ui->mainScreen->setGraphicsEffect(blur);
    ui->historyScreen->setGraphicsEffect(blur2);
    ui->actSettings->setText("⋮");
    animation->start();
}

void MainWindow::ocHistory()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->historyScreen, "geometry");
    animation->setDuration(100);

    bool isOpen{false};
    if(ui->historyScreen->y() == 190)
        isOpen = true;

    if(isOpen) // Close History
    {
        animation->setKeyValueAt(0, QRect(0, 190, 421, 816));
        animation->setKeyValueAt(1, QRect(0, 700, 421, 816));
        ui->actHistory->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); border: none; border-bottom: 3px solid rgba(255, 255, 255, 255); }"
                                      "QPushButton:hover:!pressed { border-bottom: 3px solid rgba(255, 255, 255, 150); }"
                                      "QPushButton:hover:pressed { border-bottom: 3px solid rgba(255, 255, 255, 75); }" ""
                                      "QPushButton:pressed { border-bottom: 3px solid rgba(255, 255, 255, 75); }");
    }
    else // Open History
    {
        animation->setKeyValueAt(0, QRect(0, 700, 421, 816));
        animation->setKeyValueAt(1, QRect(0, 190, 421, 816));
        ui->actHistory->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); border: none; border-bottom: 3px solid rgba(231, 4, 91, 255); }"
                                      "QPushButton:hover:!pressed { border-bottom: 3px solid rgba(231, 4, 91, 150); }"
                                      "QPushButton:hover:pressed { border-bottom: 3px solid rgba(231, 4, 91, 75); }"
                                      "QPushButton:pressed { border-bottom: 3px solid rgba(231, 4, 91, 75); }");
    }

    animation->start();

}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Escape)
       ocSettings();
    else if(event->key() == Qt::Key_H)
       ocHistory();
}

void MainWindow::updatePreview() // Closing program
{
    if(ui->flag_OnTop->isChecked())
        flags |= Qt::WindowStaysOnTopHint;

    MainWindow::setWindowFlags(flags);
}

void MainWindow::gotoSetting(int index)
{
    ui->settingsScreen->setCurrentIndex(index);
    ui->actSettings->setText("");
    ui->actSettings->setIcon(QIcon(":/symbol/back"));
}
