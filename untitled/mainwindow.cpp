#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sequenceofoperations.h"
#include <stack>
using namespace std;

size_t fontSize = 26; //declaring size of font
size_t toCompare = 30; // size of the full string

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connecting keys to slots
    connect (ui->buttn0, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn1, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn2, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn3, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn4, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn5, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn6, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn7, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn8, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttn9, SIGNAL(clicked()), this, SLOT(Digits()));
    connect (ui->buttnMulti, SIGNAL(clicked()), this, SLOT(Operations()));
    connect (ui->buttnDiv, SIGNAL(clicked()), this, SLOT(Operations()));
    connect (ui->buttnPlus, SIGNAL(clicked()), this, SLOT(Operations()));
    connect (ui->buttnMinus, SIGNAL(clicked()), this, SLOT(Operations()));

    connect (ui->buttnOpenBr, SIGNAL(clicked()), this, SLOT(Operations()));
    connect (ui->buttnClosingBr, SIGNAL(clicked()), this, SLOT(Operations()));

    //making operations checkable to be able to add them to string
    ui->buttnPlus->setCheckable(true);
    ui->buttnMinus->setCheckable(true);
    ui->buttnMulti->setCheckable(true);
    ui->buttnDiv->setCheckable(true);

    ui->buttnOpenBr->setCheckable(true);
    ui->buttnClosingBr->setCheckable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//checking if a symbol is operation
bool MainWindow::isOperator(char op)
{
    switch (op)
    {
    case '*':
    case '/':
    case '+':
    case '-':
        return true;    
    }
    return false;
}

// the function is needed for checking if an input string contains proper symbols
// such as {0...9} or {+, -, *, /, (, )}; USED IN the "Parsing" method
bool MainWindow::Check(string result)
{
    for (int i = 0; i < result.size(); i++) {
        if (!isdigit(result[i]) && !isOperator(result[i]) && result[i] != '(' && result[i] != ')')
                        return false;
    }
    return true;
}

// the method defines what precedence each operation has
int MainWindow::Priority(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 2;

    case '*':
    case '/':
        return 3;

    case '(':
        //case ')':
        return 1;

    default:
        return -1;
    }
}

// the method removes comma at the end of the string
string MainWindow::removeLastEl(string result)
{
    string tmp;
    for (int i = 0; i < result.size() - 1; i++)
        tmp += result[i];
    return tmp;
}

//converting infix to postfix notation using stack
string MainWindow::toPostfix(string s)
{
    stack <char> stk;
    string result;
    int k = 0; // iterator in "for each" loop
    bool flag = true; // declaring flag to count digits in a number
    for (char CurrVar : s) {
        if (isdigit(CurrVar)) {
            result += CurrVar;              //
            if (k < s.size() - 1 && flag)   // checking if a number is more than 9
                if (!isdigit(s[k + 1])) {   // if the next thing isn't a number than we put comma as a spacer
                    flag = false;           //
                    result += ',';          //
                }                           //
                else                        //
                    flag = true;            // else we're fine
            if (k == s.size() - 1) // in case if the last symbol in the string is a digit
                result += ',';     // we put spacer
        }
        else {
            if (isOperator(CurrVar)) {
                int CurrPrior = Priority(CurrVar), StackPrior = 0;  //checking priority of the current variable
                if (!stk.empty())
                    StackPrior = Priority(stk.top()); // checking priority of the variable on top of stack
                if (!stk.empty() && (StackPrior < CurrPrior))
                    stk.push(CurrVar);
                else {
                    while (!stk.empty() && (StackPrior >= CurrPrior)) { // if there is/are elements
                        result += stk.top();                            // having greater precedence on top of stack
                        result += ',';                                  // then we put them in to the result string
                        stk.pop();
                        if(!stk.empty())
                            StackPrior = Priority(stk.top());          // resetting priority of the top element of stack
                    }
                    stk.push(CurrVar);
                }
            }
            if (CurrVar == '(')
                stk.push(CurrVar);
            if (CurrVar == ')') {
                while (stk.top() != '(') {          // moving untill the element is not an open bracket
                    result += stk.top();            // putting to the result string operators from stack
                    result += ',';  //
                    stk.pop();
                }
                stk.pop();
            }
            flag = true;
        }
        ++k;
    }                                                                              // infix expr    postfix expr
    // if stack contains some operators then they should be put in the result string: (1+2)*4+3 --> 12+4*3+
    while (!stk.empty()) {
        result += stk.top();
        result += ',';  //
        stk.pop();
    }
    result = removeLastEl(result);  //
    return result;
}

// method which returns the string converted to Postfix Polish notation
string MainWindow::Parsing(QString s)
{
    string converted = s.toStdString();    
    bool state;
    Check(converted) ? state = 1 : state = 0;
    if (state) {
        converted = toPostfix(converted);
        return converted;
    }
    else
        return "ERR";
}

//the event that launches the second window where a user is able to see maths operations made
void MainWindow::on_buttnEqual_clicked()
{
    ui->statusbar->showMessage("'=' WAS PRESSED!");
    string postFixNotation = Parsing(ui->expr->text()); //getting postfix notation
    SequenceOfOperations w;                             //creating an object of the second window
    w.setWindowTitle("Последовательность операций");
    w.setModal(true);
    float res = w.Calculate(postFixNotation);           //getting the result of the expression
    QString toWrite = QString::number(res);
    QString prev = ui->expr->text();
    prev += "=";
    ui->expr->setText(prev + toWrite);                  //printing the result in the first window
    w.exec();
}

// the slot is for adding numbers to the string that a user observes
void MainWindow::Digits()
{
    QPushButton *key = (QPushButton*)sender();
    QString prevNum = ui->expr->text() + key->text();    
    QFont font ("Segoe UI", fontSize, QFont::Bold);
    if (prevNum.size() > toCompare){      // if there are too many characters
            --fontSize;                   // we make font smaller
            toCompare += 1;      
    }

    if (prevNum.at(0) == '0'){          // delete first zero in the input string
        QString tmp;
        for (int i = 1; i < prevNum.size(); i++)
            tmp += prevNum.at(i);
        prevNum = tmp;
    }    
    font.setPointSize(fontSize);
    ui->expr->setFont(font);
    ui->expr->setText(prevNum);
}

// the slot is used in the "Operations" slot
void MainWindow::Update(QPushButton *&key, QChar op)
{
    QString prev = ui->expr->text();
    prev += op;
    ui->expr->setText(prev);
    QString toStatusBar = op;
    toStatusBar += " WAS PRESSED!";
    ui->statusbar->showMessage(toStatusBar);
    key->setChecked(false);
}

// the slot is for adding operations symbols to the string that a user observes
void MainWindow::Operations()
{
    QPushButton *key = (QPushButton*)sender();
    key->setChecked(true);

    if (ui->buttnPlus->isChecked()){
        Update(key, '+');
    }
    else if (ui->buttnMinus->isChecked()){
        Update(key, '-');
    }
    else if (ui->buttnMulti->isChecked()){
        Update(key, '*');
    }
    else if (ui->buttnDiv->isChecked()){
        Update(key, '/');
    }
    else if (ui->buttnOpenBr->isChecked()){
        Update(key, '(');
    }
    else if (ui->buttnClosingBr->isChecked()){
        Update(key, ')');
    }
}

// the event is for clicking on delete key
void MainWindow::on_buttnDelete_clicked()
{
    ui->buttnPlus->setChecked(false);
    ui->buttnDiv->setChecked(false);
    ui->buttnMulti->setChecked(false);
    ui->buttnMinus->setChecked(false);

    QFont font ("Segoe UI", 26, QFont::Bold);   //setting font to normal size
    font.setPointSize(26);                      //
    ui->expr->setFont(font);                    //

    ui->expr->setText("0");
    ui->buttnDelete->setChecked(false);
    ui->statusbar->clearMessage();

}

// the event is for clicking on backspace key
void MainWindow::on_buttnBackspace_clicked()
{
    ui->buttnPlus->setChecked(false);
    ui->buttnDiv->setChecked(false);
    ui->buttnMulti->setChecked(false);
    ui->buttnMinus->setChecked(false);
    QString prev = ui->expr->text();
    prev.chop(1);                                   // removing 1 characher in the string
    if (prev.isEmpty())                             //if we're deleting all characters
        prev += "0";                                // then the machine to print 0
    ui->expr->setText(prev);
    ui->buttnBackspace->setChecked(false);
    ui->statusbar->clearMessage();

    QString curr = ui->expr->text();
    QFont font ("Segoe UI", fontSize, QFont::Bold);
    if (toCompare > 30 && curr.size() < toCompare){
        ++fontSize;
        toCompare -= 1;
        font.setPointSize(fontSize);
        ui->expr->setFont(font);
     }

}

//here i wanted to add opening of external links but it doesn't work on my PC
void MainWindow::on_pushButton_4_clicked()
{    
    QString link = "https://habr.com/ru/post/100869/";    
    QDesktopServices::openUrl(QUrl(link));
}
