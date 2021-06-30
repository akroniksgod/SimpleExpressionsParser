#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sequenceofoperations.h"
#include <stack>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

bool MainWindow::isOperand(char op)
{
    switch (op)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return true;
    }
    return false;
}

bool MainWindow::isOperator(char op)
{
    switch (op)
    {
    case '*':
    case '/':
    case '+':
    case '-':
        return true;
    default:
        return false;
    }
    return false;
}

bool MainWindow::Check(string result)
{
    for (int i = 0; i < result.size(); i++) {
        if (!isOperand(result[i]) && !isOperator(result[i]) && result[i] != '(' && result[i] != ')')
                        return false;
    }
    return true;
}

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

string MainWindow::removeLastEl(string result)
{
    string tmp;
    for (int i = 0; i < result.size() - 1; i++)
        tmp += result[i];
    return tmp;
}

string MainWindow::toPostfix(string s)
{
    stack <char> stk;
    string result;// = "12,+,4,*,3,+";
    int k = 0; // iterator in "for each" loop
    bool flag = true;
    for (char CurrVar : s) {
        if (isdigit(CurrVar)) {
            result += CurrVar;              //
            if (k < s.size() - 1 && flag)   // checking if a number is more than 9
                if (!isdigit(s[k + 1])) {   //
                    flag = false;           //
                    result += ',';          //
                }                           //
                else                        //
                    flag = true;            //
            if (k == s.size() - 1) // in case if the last symbol in the string is a digit
                result += ',';
        }
        else {
            if (isOperator(CurrVar)) {
                int CurrPrior = Priority(CurrVar), StackPrior = 0;
                if (!stk.empty())
                    StackPrior = Priority(stk.top());
                if (!stk.empty() && (StackPrior < CurrPrior))
                    stk.push(CurrVar);
                else {
                    while (!stk.empty() && (StackPrior >= CurrPrior)) {
                        result += stk.top();
                        result += ',';  //
                        stk.pop();
                        if(!stk.empty())
                            StackPrior = Priority(stk.top());
                    }
                    stk.push(CurrVar);
                }
            }
            if (CurrVar == '(')
                stk.push(CurrVar);
            if (CurrVar == ')') {
                while (stk.top() != '(') {
                    result += stk.top();
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

void MainWindow::on_buttnEqual_clicked()
{
    ui->statusbar->showMessage("'=' was pressed!");
    string postFixNotation = Parsing(ui->expr->text());
    SequenceOfOperations w;
    w.setWindowTitle("Последовательность операций");
    w.setModal(true);
    int res = w.Calculate(postFixNotation);
    QString toWrite = QString::number(res);
    ui->expr->setText(toWrite);
    w.exec();
}

void MainWindow::Update(QPushButton *&key, QChar op)
{
    QString prev = ui->expr->text();
    prev += op;
    ui->expr->setText(prev);
    key->setChecked(false);
}

void MainWindow::Digits()
{
    QPushButton *key = (QPushButton*)sender();
    QString prevNum = ui->expr->text() + key->text();
    if (prevNum.at(0) == '0'){
        QString tmp;
        for (int i = 1; i < prevNum.size(); i++)
            tmp += prevNum.at(i);
        prevNum = tmp;
    }
    ui->expr->setText(prevNum);
}

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

void MainWindow::on_buttnDelete_clicked()
{
    ui->buttnPlus->setChecked(false);
    ui->buttnDiv->setChecked(false);
    ui->buttnMulti->setChecked(false);
    ui->buttnMinus->setChecked(false);
    ui->expr->setText("0");
    ui->buttnDelete->setChecked(false);
    ui->statusbar->clearMessage();
}

void MainWindow::on_buttnBackspace_clicked()
{
    ui->buttnPlus->setChecked(false);
    ui->buttnDiv->setChecked(false);
    ui->buttnMulti->setChecked(false);
    ui->buttnMinus->setChecked(false);
    QString prev = ui->expr->text();
    prev.chop(1);
    if (prev.isEmpty())
        prev += "0";
    ui->expr->setText(prev);
    ui->buttnBackspace->setChecked(false);
    ui->statusbar->clearMessage();
}
