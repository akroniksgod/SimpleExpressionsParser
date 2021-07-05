#include "sequenceofoperations.h"
#include "ui_sequenceofoperations.h"
#include <stack>
#include <cmath>
using namespace std;

SequenceOfOperations::SequenceOfOperations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceOfOperations)
{
    ui->setupUi(this);
}

//used in the "Calculate" function
void SequenceOfOperations::PrintExprInTheWin(float op1, float op2, char op, int i)
{
    QString prevText = ui->label->text(),
            num = QString::number(i) + ". ",
            o1 = QString::number(op1),
            o2 = QString::number(op2),
            res = "\n" + num + o2 + op + o1;

    res = prevText + res;
    ui->label->setText(res);
}

//returning solution + printing steps of calculating solution in the second window
float SequenceOfOperations::Calculate(string s)
{
    stack <float> storage;
    bool state = 1;
    ui->label->setText("====History====");
    int count = 1;
    for (int i = 0; i < s.size() && state; i++)
    {
        if (isdigit(s[i])) {
            int n = s[i] - 48; // ascii thing
            while (s[++i] != ',') { //moving utill its not a spacer ","
                if (i < s.size()) {
                    n *= 10;
                    n += (s[i] - 48);
                }
            }
            --i;
            storage.push(n);
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*' || s[i] == '^') {
            float op1 = storage.top(); //getting second operand
            storage.pop();
            float op2 = storage.top(); //getting first operand
            storage.pop();

            if (s[i] == '^'){
                PrintExprInTheWin(op1, op2, '^', count);
                op1 = pow(op2, op1);
            }

            if (s[i] == '*') { // if the operation is multiplication
                PrintExprInTheWin(op1, op2, '*', count);
                op1 *= op2;
            }
            else if (s[i] == '/') { // if the operation is division
                //checking if the second operand is 0
                if (op1) {
                    PrintExprInTheWin(op1, op2, '/', count);
                    op1 = op2 / op1;
                }
                else {
                    ui->label->setText("\nCANNOT DIVIDE BY ZERO");
                    state = false;
                    return 99999.9;
                }
            }
            if (s[i] == '+') {  // if the operation is additon
                PrintExprInTheWin(op1, op2, '+', count);
                op1 += op2;
            }
            else if (s[i] == '-') { // if the operation is substraction
                PrintExprInTheWin(op1, op2, '-', count);
                op1 = op2 - op1;
            }
            storage.push(op1);
            ++count;
        }
    }
    if (state){
        QString prevText = ui->label->text(),
                newText = "\n====RESULT====\n",
                num = QString::number(storage.top());
        ui->label->setText(prevText + newText + num); //printing result
        return storage.top();
    }
    else
        return 99999.9;
}

SequenceOfOperations::~SequenceOfOperations()
{
    delete ui;
}
