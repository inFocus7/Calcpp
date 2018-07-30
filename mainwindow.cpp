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
maths::dLL EQUATION;
datastore::dLL HISTORY;
QString recentNumInput;
Qt::WindowFlags flags;
bool solved{false}, prevOp{false}, decimalExists{false};

//Qt::FramelessWindowHint
//Qt::WindowStaysOnTopHint
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Shortcuts
    ui->decimal->setShortcut(Qt::Key_Period);
    ui->sign_percent->setShortcut(Qt::Key_Percent);
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
    connect(ui->sign_percent, &QPushButton::released, this, [this]{arithmetics(5);});
    connect(ui->actSettings, &QPushButton::released, this, [this]{ocSettings(true);});
    connect(ui->actHistory, SIGNAL(released()), this, SLOT(ocHistory()));
    connect(ui->s_github, &QPushButton::released, this, [this]{openSocial(QUrl("https://www.github.com/infocus7/"));});
    connect(ui->s_linkedin, &QPushButton::released, this, [this]{openSocial(QUrl("https://www.linkedin.com/in/fabiangonz98/"));});
    connect(ui->s_twitter, &QPushButton::released, this, [this]{openSocial(QUrl("https://www.twitter.com/fabiangonz98/"));});
    connect(ui->b_plusminus, SIGNAL(released()), this, SLOT(negateNum()));
    connect(ui->flag_OnTop, SIGNAL(toggled(bool)), this, SLOT(updatePreview()));
    connect(ui->aboutSettings, &QPushButton::released, this, [this]{gotoSetting(4);});
    connect(ui->generalSettings, &QPushButton::released, this, [this]{gotoSetting(1);});
    connect(ui->memorySettings, &QPushButton::released, this, [this]{gotoSetting(3);});
    connect(ui->visualSettings, &QPushButton::released, this, [this]{gotoSetting(2);});
    //Make a back button when not in normal settings screen
    //connect(ui->settingsBack, &QPushButton::released, this, [this]{gotoSetting(0);});
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
    QString s_text{QString::number(s_num, 'g', 15)};
    ui->screenOutput->setText(s_text);
    }
}

void MainWindow::updateEquation(QString string)
{
    ui->equationScreen->setText(string);
}

void MainWindow::digitPressed()
{
    if(solved == true)
    {
        clearScreen();
        solved = false;
    }

    QPushButton *button = (QPushButton*)sender();
    updateEquation((ui->equationScreen->text().append(button->text())));
    recentNumInput.append(button->text());
    updateScreen(recentNumInput.toDouble(), recentNumInput);
}

void MainWindow::backspace()
{
    if(recentNumInput == ui->screenOutput->text())
    {
        QString number{ui->screenOutput->text()};
        number.chop(1);
        updateScreen(number.toDouble());

        QString eq{ui->equationScreen->text()};
        eq.chop(1);
        updateEquation(eq);

        recentNumInput.chop(1);
    }
}

void MainWindow::clear()
{
    EQUATION.clear();
    ui->screenOutput->clear();
    ui->equationScreen->clear();
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
    if(eqScreen->text().isEmpty())
        return;
    bool alteredOperation{false};
    QPushButton * button = (QPushButton*)sender();

    if(!recentNumInput.isEmpty())
        EQUATION.insert(recentNumInput);

    if(operation == 4) // "="
    {
        if(EQUATION.ready2math())
        {
            updateScreen(EQUATION.solve());
            ui->answerOutput->setText(ui->screenOutput->text());
        }
        else // Removes hanging operation.
            EQUATION.remove();

        updateEquation("");

        // Make a new qpushbutton in hisotry storing equation data. Get scrollbar to work. If doesn't work, manually add the buttons in form.
        QPushButton * historyButton = new QPushButton(HISTORY.insert(maths::dLL(EQUATION)), ui->HISTORY);
        historyButton->show();
        historyButton->setGeometry(0, 51 * (HISTORY.returnNumItems() - 1), 404, 51); // set y 51*(amt of items in history)
        historyButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        //ui->historyArea->
        // historyButton->setStyleSheet("");
        // ui->b_history1->setText(HISTORY.insert(maths::dLL(EQUATION)));

        EQUATION.clear();
    }
    else
    {
        if(eqScreen->text() != NULL && xtra::is_in(eqScreen->text().back(), {'+', '-', '/', '*', '%'}))
        {
            QString eq{eqScreen->text()};
            eq.chop(1);
            updateEquation(eq);
            alteredOperation = true;
        }

        eqScreen->setText(eqScreen->text().append(button->text()));

        if(alteredOperation == true)
        {
            EQUATION.replaceOperator(button->text());
        }

        if(EQUATION.ready2math())
        {
            updateScreen(EQUATION.solve());
            ui->answerOutput->setText(ui->screenOutput->text());
        }

        if(alteredOperation == false)
        {
            EQUATION.insert(button->text());
        }
    }

    recentNumInput.clear();
    solved = true;
}

void MainWindow::negateNum() // Not working as of 7.25.18 11.30AM
{
    QString num{ui->screenOutput->text()};
    if(ui->equationScreen->text().length() >= 1 && !(xtra::is_in(ui->equationScreen->text().back(), {'+', '-', '/', '*'})))
    {
        if(ui->screenOutput->text().at(0) != '-') // If positive
        {
            QString newNum{num.prepend('-')};

            // edit screenoutput
            updateScreen(newNum.toDouble());
            // edit equationscreen

            // edit number inputted variable
            recentNumInput = newNum;
        }
        else // If negative
        {
            QString newNum{num.remove(0, 1)};

            // edit screenoutput
            updateScreen(newNum.toDouble());
            // edit equationscreen

            // edit number inputted variable
            recentNumInput = newNum;
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


    if(isOpen == false)
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
    ui->actSettings->setText("↵");
}
