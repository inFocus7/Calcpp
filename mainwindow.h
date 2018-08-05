#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QUrl>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //void options();
    //void history();

private slots: // Slot = function signal catcher(?)
    void digitPressed();
    void updateScreen(double, QString);
    void updateEquation(QString);
    void backspace();
    void clear();
    void clearScreen();
    void arithmetics(unsigned int);
    void openSocial(QUrl);
    void ocSettings(bool = false);
    void ocHistory();
    void keyPressEvent(QKeyEvent *);
    void negateNum();
    void updatePreview();
    void gotoSetting(int);
    void reset();
    void resizeScreen();
    //void percentNum();
    //void enterAnimation();
    //void saveHistory();
    //void settings();
};

#endif // MAINWINDOW_H
