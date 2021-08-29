#include "sequenceofoperations.h"
#include "ui_sequenceofoperations.h"
#include <stack>
#include <cmath>
using namespace std;

const float toConvertDegToRad = 0.01745329251;

SequenceOfOperations::SequenceOfOperations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SequenceOfOperations)
{
    ui->setupUi(this);
}

bool SequenceOfOperations::isDigit(char op)
{
    string str = "0123456789.";
    if (str.find(op) != -1)
        return true;
    return false;
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

//overloading the method above for "mod" operations
void SequenceOfOperations::PrintExprInTheWin(float op1, QString op, int i)
{
    QString prevText = ui->label->text(),
            num = QString::number(i) + ". ",
            o1 = QString::number(op1),
            res = "\n" + num + op + "(" + o1 + ")";

    res = prevText + res;
    ui->label->setText(res);
}

//overloading the method above for sin, tan, log and etc operations
void SequenceOfOperations::PrintExprInTheWin(float op1, float op2, QString op, int i)
{
    QString prevText = ui->label->text(),
            num = QString::number(i) + ". ",
            o1 = QString::number(op1),
            o2 = QString::number(op2),
            res = "\n" + num + o2 + op + o1;

    res = prevText + res;
    ui->label->setText(res);
}

bool periodCheck(float number)
{
    for (size_t i = 90; i < 1000; i += 180){
        if (number == i)
            return false;
    }
    return true;
}

//returning solution + printing steps of calculating solution in the second window
float SequenceOfOperations::Calculate(string s, bool DegOrRad)
{
    stack <float> storage;    
    bool flag = 1;
    ui->label->setText("=========History=========");
    int count = 1;
    QPixmap Err(":/pictures/pics/error.png"); // for printing the error pic
    for (int i = 0; i < s.size(); i++)
    {
        if (isDigit(s[i])) {
            float n = s[i] - 48;                                // ascii thing
            float count = 0.1;
                while (s[++i] != ',') {
                    if (i < s.size() && s[i] != '.' && flag) {  //if the current number is a natural number
                        n *= 10;
                        n += (s[i] - 48);
                    }
                    else if (i < s.size()) {                //if the current number is a fraction number
                        ++i;
                        if (s[i] != ',') {
                            n += ((s[i] - 48) * count);
                            count /= 10;                            
                            flag = false;
                        }
                        else {
                            --i;
                            n += ((s[i] - 48) * count);
                            count /= 10;
                            flag = true;
                        }                        
                    }
                }
                --i;
            storage.push(n);
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*' || s[i] == '^') {
            float op1 = storage.top();                              //getting second operand
            storage.pop();

            if (s[i] == '^'){
                float op2 = storage.top();                          //getting first operand
                storage.pop();
                PrintExprInTheWin(op1, op2, '^', count);
                op1 = pow(op2, op1);
            }

            if (s[i] == '*') {                                      // if the operation is multiplication
                float op2 = storage.top(); //getting first operand
                storage.pop();
                PrintExprInTheWin(op1, op2, '*', count);
                op1 *= op2;
            }
            else if (s[i] == '/') {                                 // if the operation is division
                //checking if the second operand is 0
                float op2 = storage.top();                          //getting first operand
                storage.pop();
                if (op1) {
                    PrintExprInTheWin(op1, op2, '/', count);
                    op1 = op2 / op1;
                }
                else {                    
                    ui->label->setPixmap(Err);
                    return 99999.9;
                }
            }
            if (s[i] == '+') {                                      // if the operation is additon
                float op2 = storage.top();                          //getting first operand
                storage.pop();
                PrintExprInTheWin(op1, op2, '+', count);
                op1 += op2;
            }
            else if (s[i] == '-') {                             // if the operation is substraction
                float op2 = storage.top();                      //getting first operand
                storage.pop();
                PrintExprInTheWin(op1, op2, '-', count);
                op1 = op2 - op1;
            }
            storage.push(op1);
            ++count;
        }
        else if (s[i] == 'S' || s[i] == 'C' || s[i] == 'T' || s[i] == 'E' || s[i] == 'R' || s[i] == 'M'
                 || s[i] == 'O' || s[i] == 'J' || s[i] == 'I' || s[i] == 'X'){
            float op1 = storage.top();                      //getting second operand
            storage.pop();
            if (s[i] == 'S'){                               // for the sin operation
                PrintExprInTheWin(op1, "sin", count);
                if (DegOrRad)
                    op1 = sin(op1 * toConvertDegToRad);
                else
                    op1 = sin(op1);
            }
            if (s[i] == 'C'){                               // for the cos operation
                PrintExprInTheWin(op1, "cos", count);
                if (DegOrRad){
                    if (periodCheck(op1))
                        op1 = cos(op1 * toConvertDegToRad);
                    else
                        op1 = 0;
                }
                else
                    op1 = cos(op1);
            }
            if (s[i] == 'T'){                               // for the tg operation
                PrintExprInTheWin(op1, "tg", count);
                if (DegOrRad)
                    if (periodCheck(op1))
                    //if (cos(op1 * toConvertDegToRad) != 0)
                        op1 = tan(op1 * toConvertDegToRad);
                    else{
                        //ui->label->setText("\nIVALID INPUT");
                        ui->label->setPixmap(Err);
                        //state = false;
                        return 99999.9;
                    }
                else{                    
                    op1 = tan(op1);
                }
            }
            if (s[i] == 'E'){                               // for the exp operation
                PrintExprInTheWin(op1, "exp", count);
                op1 = exp(op1);
            }
            if (s[i] == 'R'){                               // for the 1/x operation
                PrintExprInTheWin(op1, "reverse", count);
                if (op1)
                    op1 = 1 / op1;
                else {                    
                    ui->label->setPixmap(Err);
                    return 99999.9;
                }
            }
            if (s[i] == 'M'){
                if (!storage.empty()){
                    float op2 = storage.top();                  //getting first operand
                    storage.pop();
                    PrintExprInTheWin(op1, op2, "mod", count);
                    op1 = (int)op2 % (int)op1;
                }
                else{                    
                    ui->label->setPixmap(Err);
                    return 99999.9;
                }
            }
            if (s[i] == 'O'){                               // for the lg operation
                if (op1 > 0){
                PrintExprInTheWin(op1, "log10", count);
                op1 = log10(op1);
                }
                else{                    
                    ui->label->setPixmap(Err);
                    return 99999.9;
                }
            }
            if (s[i] == 'J'){                               // for the log2 operation
                if (op1 > 0){
                    PrintExprInTheWin(op1, "log2", count);
                    op1 = log2(op1);
                }
                else{
                    ui->label->setPixmap(Err);
                    return 99999.9;
                }
            }
            if (s[i] == 'I'){                               // for the int operation
                PrintExprInTheWin(op1, "int", count);
                op1 = ceil(op1);
            }
            if (s[i] == 'X'){                               // for the ln operation
                if (op1 > 0){
                    PrintExprInTheWin(op1, "ln", count);
                    op1 = log(op1);
                }
                else{                    
                    ui->label->setPixmap(Err);
                    return 99999.9;
                }
            }
            storage.push(op1);
            ++count;
        }
    }

    QString prevText = ui->label->text(),                   //printing result
            newText = "\n=========Result=========\n",       //printing result
            num = QString::number(storage.top());           //printing result
    ui->label->setText(prevText + newText + num);           //printing result
    return storage.top();
}

SequenceOfOperations::~SequenceOfOperations()
{
    delete ui;
}
