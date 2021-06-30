#include "sequenceofoperations.h"
#include "ui_sequenceofoperations.h"
#include <stack>
using namespace std;

SequenceOfOperations::SequenceOfOperations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceOfOperations)
{
    ui->setupUi(this);
}

void SequenceOfOperations::PrintExprInTheWin(int op1, int op2, char op, int i)
{
    QString prevText = ui->label->text(),
            num = QString::number(i) + ". ",
            o1 = QString::number(op1),
            o2 = QString::number(op2),
            res = "\n" + num + o2 + op + o1;

    res = prevText + res;
    ui->label->setText(res);
}

int SequenceOfOperations::Calculate(string s)
{
    stack <int> storage;
    bool state = 1;
    ui->label->setText("====History====");
    int count = 1;
    for (int i = 0; i < s.size() && state; i++)
    {
        if (isdigit(s[i])) {
            int n = s[i] - 48; // ascii shit
            while (s[++i] != ',') {
                if (i < s.size()) {
                    n *= 10;
                    n += (s[i] - 48);
                }
            }
            --i;
            storage.push(n);
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*') {
            int op1 = storage.top();
            storage.pop();
            int op2 = storage.top();
            storage.pop();

            if (s[i] == '*') {
                PrintExprInTheWin(op1, op2, '*', count);
                op1 *= op2;
            }
            else if (s[i] == '/') {
                if (op1) {
                    PrintExprInTheWin(op1, op2, '/', count);
                    op1 = op2 / op1;
                }
                else {
                    ui->label->setText("\nCANNOT DIVIDE BY ZERO");
                    state = false;
                    return 999999999;
                }
            }
            if (s[i] == '+') {
                PrintExprInTheWin(op1, op2, '+', count);
                op1 += op2;
            }
            else if (s[i] == '-') {
                PrintExprInTheWin(op1, op2, '-', count);
                op1 = op2 - op1;

            }
            storage.push(op1);
            ++count;
        }
    }
    if (state){
        QString prevText = ui->label->text(),
                newText = "\nRESULT\n",
                num = QString::number(storage.top());
        ui->label->setText(prevText + newText + num);
        return storage.top();
    }
}

SequenceOfOperations::~SequenceOfOperations()
{
    delete ui;
}
