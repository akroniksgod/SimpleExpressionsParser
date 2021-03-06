#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sequenceofoperations.h"
#include <stack>
using namespace std;

size_t FONT_SIZE = 26; //declaring size of font
size_t TO_COMPARE_FONT_SIZE = 30; // size of the full string
bool DEG1_RAD0 = 1;

//constr
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

    connect (ui->buttnPow, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnPow->setCheckable(true);

    connect (ui->buttnDot, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnDot->setCheckable(true);
    connect (ui->buttnSin, SIGNAL(clicked()), this, SLOT(Operations()));
    connect (ui->buttnCos, SIGNAL(clicked()), this, SLOT(Operations()));
    connect (ui->buttnTan, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnSin->setCheckable(true);
    ui->buttnCos->setCheckable(true);
    ui->buttnTan->setCheckable(true);
    connect (ui->buttnExp, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnExp->setCheckable(true);
    connect (ui->buttnRev, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnRev->setCheckable(true);
    connect (ui->buttnMod, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnMod->setCheckable(true);
    connect (ui->buttnLg, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnLg->setCheckable(true);
    connect (ui->buttnLog, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnLog->setCheckable(true);
    connect (ui->buttnInt, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnInt->setCheckable(true);

    connect (ui->buttnLn, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnLn->setCheckable(true);

    //making operations checkable to be able to add them to string
    ui->buttnPlus->setCheckable(true);
    ui->buttnMinus->setCheckable(true);
    ui->buttnMulti->setCheckable(true);
    ui->buttnDiv->setCheckable(true);

    ui->buttnOpenBr->setCheckable(true);
    ui->buttnClosingBr->setCheckable(true);

    on_DegOn_toggled(true);
    ui->buttnInv->setCheckable(true);
    ui->buttnExpand->setCheckable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//checking if a sybmbol is digit
bool MainWindow::isDigit(char op)
{
    string str = "0123456789.";
    if (str.find(op) != -1)
        return true;
    return false;
}

//checking if a symbol is operation
bool MainWindow::IsOperator(char op)
{
    string str = "*/+-^SINCOTAEXPRVMDLGJ";
    if (str.find(op) != -1)
        return true;
    return false;    
}

// the function is needed for checking if an input string contains proper symbols
// such as {0...9} or {+, -, *, /, (, )}; USED IN the "Parsing" method
bool MainWindow::ProperSymbCheck(string result)
{
    for (int i = 0; i < result.size(); i++) {
        if (!isDigit(result[i]) && !IsOperator(result[i]) && result[i] != '(' && result[i] != ')')
                        return false;
    }
    return true;
}

// the method defines what precedence each operation has
int MainWindow::ReturnPriorityVal(char op)
{
    switch (op)
    {
    case '(':
        return 1;
    case '+':
    case '-':
    case 'M':           //stands for mod
        return 2;
    case '*':
    case '/':
        return 3;
    case '^':
        return 4;
    case 'S':           //stands for sin()
    case 'C':           //stands for cos()
    case 'T':           //stands for tg()
    case 'E':           //stands for exp()
    case 'R':           //stands for 1/x
    case 'O':           //let this one stand for log10()
    case 'J':           //let this one stand for log2()
    case 'I':           //stands for int
    case 'X':           //stands for natural log
        return 5;
    default:
        return -1;
    }
}

// the method removes comma at the end of the string
string MainWindow::RemoveLastEl(string result)
{
    string tmp;
    for (int i = 0; i < result.size() - 1; i++)
        tmp += result[i];
    return tmp;
}

//converting infix to postfix notation using stack
string MainWindow::ToPostfixNotation(string s)
{
    stack <char> stk;
    string result;
    int k = 0; // iterator in "for each" loop
    bool flag = true; // declaring flag to count digits in a number
    for (char CurrVar : s)
    {
        if (isDigit(CurrVar))
        {
            result += CurrVar;              //
            if (k < s.size() - 1 && flag)   // checking if a number is more than 9
                if (!isDigit(s[k + 1]))
                {   // if the next thing isn't a number than we put comma as a spacer
                    flag = false;           //
                    result += ',';          //
                }                           //
                else                        //
                    flag = true;            // else we're fine
            if (k == s.size() - 1) // in case if the last symbol in the string is a digit
                result += ',';     // we put spacer
        }
        else
        {
            if (IsOperator(CurrVar))
            {
                int CurrPrior = ReturnPriorityVal(CurrVar), StackPrior = 0;  //checking priority of the current variable
                if (!stk.empty())
                    StackPrior = ReturnPriorityVal(stk.top()); // checking priority of the variable on top of stack
                if (!stk.empty() && (StackPrior < CurrPrior))
                    stk.push(CurrVar);
                else
                {
                    while (!stk.empty() && (StackPrior >= CurrPrior))   // if there is/are elements
                    {
                        result += stk.top();                            // having greater precedence on top of stack
                        result += ',';                                  // then we put them in to the result string
                        stk.pop();
                        if(!stk.empty())
                            StackPrior = ReturnPriorityVal(stk.top());          // resetting priority of the top element of stack
                    }
                    stk.push(CurrVar);
                }
            }
            if (CurrVar == '(')
                stk.push(CurrVar);
            if (CurrVar == ')')
            {
                while (stk.top() != '(')            // moving untill the element is not an open bracket
                {
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
    while (!stk.empty())
    {
        result += stk.top();
        result += ',';  //
        stk.pop();
    }
    result = RemoveLastEl(result);
    return result;
}

//declaring variables to work with expressions such as a+b-3,
float a, b, c, d, e, f, g, h, iV, j, k, l, m, n, o, p, q, r, s, t, u, v, w, xV, yV, z;
string Astr, Bstr, Cstr, Dstr, Estr, Fstr, Gstr, Hstr, Istr, Jstr, Kstr, Lstr, Mstr, Nstr,
Ostr, Pstr, Qstr, Rstr, Sstr, Tstr, Ustr, Vstr, Wstr, Xstr, Ystr, Zstr;

namespace RemoveGlobalVarsFromVision
{
    string ReturnValue(string RandStr, float temp, QString str)
    {
        string st = str.toStdString();
        int size = str.size();
        for (int i = 2; i < size; i++){
            if (temp == 0)
                temp = st[i] - 48;
            else {
                temp *= 10;
                temp += st[i] - 48;
            }
            RandStr += st[i];
        }
        return RandStr;
    }
}

//getting values to variables from the table
float MainWindow::GetValues()
{
    Astr = RemoveGlobalVarsFromVision::ReturnValue(Astr, a, ui->aEquals->text());
    Bstr = RemoveGlobalVarsFromVision::ReturnValue(Bstr, b, ui->bEquals->text());
    Cstr = RemoveGlobalVarsFromVision::ReturnValue(Cstr, c, ui->cEquals->text());
    Dstr = RemoveGlobalVarsFromVision::ReturnValue(Dstr, d, ui->dEquals->text());
    Estr = RemoveGlobalVarsFromVision::ReturnValue(Estr, e, ui->eEquals->text());
    Fstr = RemoveGlobalVarsFromVision::ReturnValue(Fstr, f, ui->fEquals->text());
    Gstr = RemoveGlobalVarsFromVision::ReturnValue(Gstr, g, ui->gEquals->text());
    Hstr = RemoveGlobalVarsFromVision::ReturnValue(Hstr, h, ui->hEquals->text());
    Istr = RemoveGlobalVarsFromVision::ReturnValue(Istr, iV, ui->iEquals->text());
    Jstr = RemoveGlobalVarsFromVision::ReturnValue(Jstr, j, ui->jEquals->text());
    Kstr = RemoveGlobalVarsFromVision::ReturnValue(Kstr, k, ui->kEquals->text());
    Lstr = RemoveGlobalVarsFromVision::ReturnValue(Lstr, l, ui->lEquals->text());
    Mstr = RemoveGlobalVarsFromVision::ReturnValue(Mstr, m, ui->mEquals->text());
    Nstr = RemoveGlobalVarsFromVision::ReturnValue(Nstr, n, ui->nEquals->text());
    Ostr = RemoveGlobalVarsFromVision::ReturnValue(Ostr, o, ui->oEquals->text());
    Pstr = RemoveGlobalVarsFromVision::ReturnValue(Pstr, p, ui->pEquals->text());
    Qstr = RemoveGlobalVarsFromVision::ReturnValue(Qstr, q, ui->qEquals->text());
    Rstr = RemoveGlobalVarsFromVision::ReturnValue(Rstr, r, ui->rEquals->text());
    Sstr = RemoveGlobalVarsFromVision::ReturnValue(Sstr, s, ui->sEquals->text());
    Tstr = RemoveGlobalVarsFromVision::ReturnValue(Tstr, t, ui->tEquals->text());
    Ustr = RemoveGlobalVarsFromVision::ReturnValue(Ustr, u, ui->uEquals->text());
    Vstr = RemoveGlobalVarsFromVision::ReturnValue(Vstr, v, ui->vEquals->text());
    Wstr = RemoveGlobalVarsFromVision::ReturnValue(Wstr, w, ui->wEquals->text());
    Xstr = RemoveGlobalVarsFromVision::ReturnValue(Xstr, xV, ui->xEquals->text());
    Ystr = RemoveGlobalVarsFromVision::ReturnValue(Ystr, yV, ui->yEquals->text());
    Zstr = RemoveGlobalVarsFromVision::ReturnValue(Zstr, z, ui->zEquals->text());
}

//checking if the string contains russian characters
bool MainWindow::CheckIfContainsRusCharacters(string currStr)
{
    string russChar = "??????????????????????????????????????????????????????????????????";
    for (size_t i = 0; i < currStr.size(); i++){
        if (russChar.find(currStr[i]) != -1)
            return true;
    }
    return false;
}

//converting a+b-2 -> 12+23-2 by getting values from the table
string MainWindow::InsertValuesInsteadOfVars(string newStr)
{
    GetValues(); // getting values from the table next to the calculator
    string result, exp = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < newStr.size() - 1; i++){                            //for loop
        if (exp.find(newStr[i]) != -1 && exp.find(newStr[i + 1]) != -1      // is for adding multiplacation
                || exp.find(newStr[i]) != -1 && isDigit(newStr[i + 1])      // operation sign
                || exp.find(newStr[i + 1]) != -1 && isDigit(newStr[i])){    // for instance: 5ab-3 -> 5*a*b-3
            result += newStr[i];
            result += '*';
        }
        else
            result += newStr[i];
    }
    result += newStr[newStr.size() - 1];
    string converted = result;
    result.clear();
    for (int i = 0; i < converted.size(); i++){ // for loop is for putting numbers, where numbers are taken from the tale,
        if (isDigit(converted[i]))              // instead of characters
            result += converted[i];             // for instance 5*a*b -> 5*1*2
        else if (IsOperator(converted[i]) || converted[i] == '(' || converted[i] == ')')
            result += converted[i];
        else{
            if (converted[i] == 'a'){
                result += Astr;
                continue;
            }
            if (converted[i] == 'b'){
                result += Bstr;
                continue;
            }
            if (converted[i] == 'c'){
                result += Cstr;
                continue;
            }
            if (converted[i] == 'd'){
                result += Dstr;
                continue;
            }
            if (converted[i] == 'e'){
                result += Estr;                
                continue;
            }
            if (converted[i] == 'f'){
                result += Fstr;                
                continue;
            }
            if (converted[i] == 'g'){
                result += Gstr;                
                continue;
            }
            if (converted[i] == 'h'){
                result += Hstr;                
                continue;
            }
            if (converted[i] == 'i'){
                result += Istr;                
                continue;
            }
            if (converted[i] == 'j'){
                result += Jstr;               
                continue;
            }
            if (converted[i] == 'k'){
                result += Kstr;                
                continue;
            }
            if (converted[i] == 'l'){
                result += Lstr;                
                continue;
            }
            if (converted[i] == 'm'){
                result += Mstr;                
            }
            if (converted[i] == 'n'){
                result += Nstr;
                continue;
            }
            if (converted[i] == 'o'){
                result += Ostr;                
                continue;
            }
            if (converted[i] == 'p'){
                result += Pstr;                
                continue;
            }
            if (converted[i] == 'q'){
                result += Qstr;                
                continue;
            }
            if (converted[i] == 'r'){
                result += Rstr;                
                continue;
            }
            if (converted[i] == 's'){
                result += Sstr;                
                continue;
            }
            if (converted[i] == 't'){
                result += Tstr;               
                continue;
            }
            if (converted[i] == 'u'){
                result += Ustr;                
                continue;
            }
            if (converted[i] == 'v'){
                result += Vstr;               
                continue;
            }
            if (converted[i] == 'w'){
                result += Wstr;
                continue;
            }
            if (converted[i] == 'x'){
                result += Xstr;                
                continue;
            }
            if (converted[i] == 'y'){
                result += Ystr;
                continue;
            }
            if (converted[i] == 'z'){
                result += Zstr;                
                continue;
            }
        }
    }
    return result;
}

//converting 4.3^(-1.2) -> 4.3^(0-1.2)
string MainWindow::CheckForUnarMinus(string start)
{
    string tmp;    
    for (size_t i = 0; i < start.size() - 2; i++)
    {
        if (!isDigit(start[i]) && start[i] != ')' && start[i + 1] == '-' && isDigit(start[i + 2]))
        {
            tmp += '0';
            tmp += start[i];
        }
        else
            tmp += start[i];

    }
    tmp += start[start.size() - 2];
    tmp += start[start.size() - 1];
    return tmp;
}

// method which returns the string converted to Postfix Polish notation
string MainWindow::Parsing(QString s)
{
    string converted = s.toStdString();
    //if the string contains characters such as abcd and etc
    if (!ProperSymbCheck(converted)) {
          converted = InsertValuesInsteadOfVars(converted);
    }
    converted = CheckForUnarMinus(converted);
    converted = ToPostfixNotation(converted);
    return converted;
}

//chaning SIN(30) -> S(30) for the following processing
QString MainWindow::ShortenStrForOperations(QString start)
{
    QString res;
    for (int i = 0; i < start.size(); i++){
        if (start[i] == 'S' || start[i] == 'C' || start[i] == 'T' || start[i] == 'E'
                || start[i] == 'R' || start[i] == 'M' || start[i] == 'L' || start[i] == 'I')
        {
            if (start[i] == 'L' && start[i + 1] != 'O')
            {
                if (start[i + 1] == 'G')
                    res += 'O';
                else if (start[i + 1] == 'N')
                    res += 'X';
                ++i;
                continue;
            }
            else if (start[i] == 'L' && start[i + 1] == 'O')
            {
                res += 'J';
                i += 2;
                continue;
            }
            else
            {
                res += start[i];
                i += 2;
                continue;
            }
        }
        else
            res += start[i];
    }
    return res;
}

bool MainWindow::ContainsSpaceAfterOperator(string currStr)
{
    for (size_t i = 0; i < currStr.size() - 1; i++)
    {
        if (IsOperator(currStr[i]) && currStr[i + 1] == ' ')
            return true;
    }
    return false;
}

//the event that launches the second window where a user is able to see maths operations made
void MainWindow::on_buttnEqual_clicked()
{
    ui->statusbar->showMessage("'=' WAS PRESSED!");
    QString currStr = ui->expr->text();    
    if (currStr.contains("S") || currStr.contains("C") || currStr.contains("T") || currStr.contains("E")
            || currStr.contains("X") || currStr.contains("O") || currStr.contains("I") || currStr.contains("J")
            || currStr.contains("M") || currStr.contains("R") || currStr.contains("L"))
        currStr = ShortenStrForOperations(currStr);

    //if the string doesn't contain '=' and isn't empty and doesn't contain russian characters then we're fine
    if (!currStr.contains('=') && !currStr.isEmpty() && !CheckIfContainsRusCharacters(currStr.toStdString())
            && !ContainsSpaceAfterOperator(currStr.toStdString()))
    {
        string postFixNotation = Parsing(currStr); //getting postfix notation
        SequenceOfOperations w;                             //creating an object of the second window
        w.setWindowTitle("???????????????????????????????????? ????????????????");
        w.setModal(true);
        float res = w.CalcByPostfixNotation(postFixNotation, DEG1_RAD0);           //getting the result of the expression
        QString toWrite = QString::number(res);
        QString prev = ui->expr->text();
        prev += "=";
        ui->expr->setText(prev + toWrite);                  //printing the result in the first window
        w.exec();
    }
    ClearVariables();
}

// the slot is for adding numbers to the string that a user observes
void MainWindow::Digits()
{
    QPushButton *key = (QPushButton*)sender();
    QString prevNum = ui->expr->text() + key->text();    
    QFont font ("Segoe UI", FONT_SIZE, QFont::Bold);
    if (prevNum.size() > TO_COMPARE_FONT_SIZE){      // if there are too many characters
            --FONT_SIZE;                   // we make font smaller
            TO_COMPARE_FONT_SIZE += 1;
    }

    if (prevNum.at(0) == '0' && !prevNum.contains('.'))
    {          // delete first zero in the input string
        QString tmp;
        for (int i = 1; i < prevNum.size(); i++)
            tmp += prevNum.at(i);
        prevNum = tmp;
    }    
    font.setPointSize(FONT_SIZE);
    ui->expr->setFont(font);
    ui->expr->setText(prevNum);
}

// the slot is used in the "Operations" slot to print operations signs on the screen
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

// the slot is used in the "Operations" slot to print operations signs on the screen
void MainWindow::UPD(QPushButton *&key, QString op)
{
    QString prev = ui->expr->text();
    op += '(';
    op += prev;
    op += ')';
    ui->expr->setText(op);

    QString toStatusBar = op;
    toStatusBar += " WAS PRESSED!";
    ui->statusbar->showMessage(toStatusBar);

    key->setChecked(false);
}

// the slot is used in the "Operations" slot to print operations signs on the screen
void MainWindow::UPD(QString op)
{
    QString prev = ui->expr->text();
    prev += op;
    ui->expr->setText(prev);

    QString toStatusBar = op;
    toStatusBar += " WAS PRESSED!";
    ui->statusbar->showMessage(toStatusBar);

    //key->setChecked(false);
}

// the slot is for adding operations symbols to the string that a user observes
void MainWindow::Operations()
{
    QPushButton *key = (QPushButton*)sender();
    key->setChecked(true);

    if (ui->buttnPlus->isChecked())
        Update(key, '+');

    else if (ui->buttnMinus->isChecked())
        Update(key, '-');

    else if (ui->buttnMulti->isChecked())
        Update(key, '*');

    else if (ui->buttnDiv->isChecked())
        Update(key, '/');

    else if (ui->buttnOpenBr->isChecked())
        Update(key, '(');

    else if (ui->buttnClosingBr->isChecked())
        Update(key, ')');

    else if (ui->buttnPow->isChecked())
        Update(key, '^');

    else if (ui->buttnDot->isChecked())
        Update(key, '.');

    else if (ui->buttnSin->isChecked())
        UPD(key, "SIN");

    else if (ui->buttnCos->isChecked())
        UPD(key, "COS");

    else if (ui->buttnTan->isChecked())
        UPD(key, "TAN");

    else if (ui->buttnExp->isChecked())
        UPD(key, "EXP");

    else if (ui->buttnRev->isChecked())
        UPD(key, "REV");

    else if (ui->buttnMod->isChecked())
        UPD("MOD");

    else if (ui->buttnLg->isChecked())
        UPD(key, "LG");

    else if (ui->buttnLog->isChecked())
        UPD(key, "LOG");

    else if (ui->buttnInt->isChecked())
        UPD(key, "INT");

    else if (ui->buttnLn->isChecked())
        UPD(key, "LN");
}

void MainWindow::ClearVariables()
{
    a = b = c = d = e = f = g = h = iV = j = k = l = m = n = o = p = q = r = s = t = u = v = w = xV = yV = z = 0;
    Astr.clear(); Bstr.clear(); Cstr.clear(); Dstr.clear(); Estr.clear(); Fstr.clear(); Gstr.clear();
    Hstr.clear(); Istr.clear(); Jstr.clear(); Kstr.clear(); Lstr.clear(); Mstr.clear(); Nstr.clear();
    Ostr.clear(); Pstr.clear(); Qstr.clear(); Rstr.clear(); Sstr.clear(); Tstr.clear(); Ustr.clear();
    Vstr.clear(); Wstr.clear(); Xstr.clear(); Ystr.clear(); Zstr.clear();
}

// the event is for clicking on delete key
void MainWindow::on_buttnDelete_clicked()
{
    ui->buttnPlus->setChecked(false);
    ui->buttnDiv->setChecked(false);
    ui->buttnMulti->setChecked(false);
    ui->buttnMinus->setChecked(false);
    ui->buttnDot->setChecked(false);

    QFont font ("Segoe UI", 26, QFont::Bold);   //setting font to normal size
    font.setPointSize(26);                      //
    ui->expr->setFont(font);                    //

    ui->expr->setText("0");
    ui->buttnDelete->setChecked(false);
    ui->statusbar->clearMessage();

    ui->buttnSin->setChecked(false);
    ui->buttnCos->setChecked(false);
    ui->buttnTan->setChecked(false);
    ui->buttnExp->setChecked(false);
    ui->buttnMod->setChecked(false);
    ui->buttnLog->setChecked(false);
    ui->buttnLg->setChecked(false);
    ui->buttnInt->setChecked(false);
    ui->buttnLn->setChecked(false);

    ClearVariables();
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
    QFont font ("Segoe UI", FONT_SIZE, QFont::Bold);
    if (TO_COMPARE_FONT_SIZE > 30 && curr.size() < TO_COMPARE_FONT_SIZE){
        ++FONT_SIZE;
        TO_COMPARE_FONT_SIZE -= 1;
        font.setPointSize(FONT_SIZE);
        ui->expr->setFont(font);
    }
    ClearVariables();
}

// the event is for swithcing degrees button on
void MainWindow::on_DegOn_toggled(bool checked)
{
    DEG1_RAD0 = true;
}

// the event is for swithcing radians button on
void MainWindow::on_RadOn_toggled(bool checked)
{
    DEG1_RAD0 = false;
}

const int EXPAND_VARS_WIND = 1248,
        SHORTEN_VARS_WIND = 648,
        EXPAND_OPERATION_WIND = 570,
        SHORTEN_OPERATION_WIND = 435;


//resizing main window
void MainWindow::on_buttnInv_clicked()
{    
    if (ui->buttnInv->isChecked() && ui->buttnExpand->isChecked()){
        this->resize(EXPAND_VARS_WIND, EXPAND_OPERATION_WIND);
        ui->buttnInv->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: #191970;"
                                    "border-radius: 10px;");
    }
    else if (!ui->buttnInv->isChecked() && ui->buttnExpand->isChecked()){
        this->resize(EXPAND_VARS_WIND, SHORTEN_OPERATION_WIND);
        ui->buttnInv->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: rgb(85, 198, 228);"
                                    "border-radius: 10px;");
    }
    else if (ui->buttnInv->isChecked() && !ui->buttnExpand->isChecked()){
        this->resize(SHORTEN_VARS_WIND, EXPAND_OPERATION_WIND);
        ui->buttnInv->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: #191970;"
                                    "border-radius: 10px;");
    }
    else if (!ui->buttnInv->isChecked() && !ui->buttnExpand->isChecked()){
        this->resize(SHORTEN_VARS_WIND, SHORTEN_OPERATION_WIND);
        ui->buttnInv->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: rgb(85, 198, 228);"
                                    "border-radius: 10px;");
    }
}

void MainWindow::on_buttnExpand_clicked()
{
    if (ui->buttnInv->isChecked() && ui->buttnExpand->isChecked()){
        this->resize(EXPAND_VARS_WIND, EXPAND_OPERATION_WIND);
        ui->buttnExpand->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: #191970;"
                                    "border-radius: 10px;");
    }
    else if (ui->buttnExpand->isChecked() && !ui->buttnInv->isChecked()){
        this->resize(EXPAND_VARS_WIND, SHORTEN_OPERATION_WIND);
        ui->buttnExpand->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: #191970;"
                                    "border-radius: 10px;");
    }
    else if (!ui->buttnInv->isChecked() && !ui->buttnInv->isChecked()){
        this->resize(SHORTEN_VARS_WIND, SHORTEN_OPERATION_WIND);
        ui->buttnExpand->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: rgb(85, 198, 228);"
                                    "border-radius: 10px;");
    }
    else if (!ui->buttnExpand->isChecked() && ui->buttnInv->isChecked()){
        this->resize(SHORTEN_VARS_WIND, EXPAND_OPERATION_WIND);
        ui->buttnExpand->setStyleSheet("color: rgb(255, 255, 255);"
                                    "background-color: rgb(85, 198, 228);"
                                    "border-radius: 10px;");
    }
}

