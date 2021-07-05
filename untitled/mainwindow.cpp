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

    connect (ui->buttnPow, SIGNAL(clicked()), this, SLOT(Operations()));
    ui->buttnPow->setCheckable(true);

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
    case '^':
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
    case '^':
        return 4;

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

float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, xV, yV, z;
string Astr, Bstr, Cstr, Dstr, Estr, Fstr, Gstr, Hstr, Istr, Jstr, Kstr, Lstr, Mstr, Nstr,
Ostr, Pstr, Qstr, Rstr, Sstr, Tstr, Ustr, Vstr, Wstr, Xstr, Ystr, Zstr;

float MainWindow::getValues()
{
    QString str = ui->aEquals->text();
    string st = str.toStdString();
    size_t size = str.size();
    for (int i = 2; i < size; i++){
        if (a == 0)
            a = st[i] - 48;
        else{
            a *= 10;
            a += st[i] - 48;
        }
        Astr += st[i];
    }

    str = ui->bEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (b == 0)
            b = st[i] - 48;
        else{
            b *= 10;
            b += st[i] - 48;
        }
        Bstr += st[i];
    }

    str = ui->cEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (c == 0)
            c = st[i] - 48;
        else{
            c *= 10;
            c += st[i] - 48;
        }
        Cstr += st[i];
    }

    str = ui->dEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (d == 0)
            d = st[i] - 48;
        else{
            d *= 10;
            d += st[i] - 48;
        }
        Dstr += st[i];
    }

    str = ui->eEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (e == 0)
            e = st[i] - 48;
        else{
            e *= 10;
            e += st[i] - 48;
        }
        Estr += st[i];
    }

    str = ui->fEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (f == 0)
            f = st[i] - 48;
        else{
            f *= 10;
            f += st[i] - 48;
        }
        Fstr += st[i];
    }

    str = ui->gEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (g == 0)
            g = st[i] - 48;
        else{
            g *= 10;
            g += st[i] - 48;
        }
        Gstr += st[i];
    }

    str = ui->hEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (h == 0)
            h = st[i] - 48;
        else{
            h *= 10;
            h += st[i] - 48;
        }
        Hstr += st[i];
    }

    str = ui->iEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (i == 0)
            i = st[i] - 48;
        else{
            i *= 10;
            i += st[i] - 48;
        }
        Istr += st[i];
    }

    str = ui->jEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (j == 0)
            j = st[i] - 48;
        else{
            j *= 10;
            j += st[i] - 48;
        }
        Jstr += st[i];
    }

    str = ui->kEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (k == 0)
            k = st[i] - 48;
        else{
            k *= 10;
            k += st[i] - 48;
        }
        Kstr += st[i];
    }

    str = ui->lEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (l == 0)
            l = st[i] - 48;
        else{
            l *= 10;
            l += st[i] - 48;
        }
        Lstr += st[i];
    }

    str = ui->mEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (m == 0)
            m = st[i] - 48;
        else{
            m *= 10;
            m += st[i] - 48;
        }
        Mstr += st[i];
    }

    str = ui->nEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (n == 0)
            n = st[i] - 48;
        else{
            n *= 10;
            n += st[i] - 48;
        }
        Nstr += st[i];
    }

    str = ui->oEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (o == 0)
            o = st[i] - 48;
        else{
            o *= 10;
            o += st[i] - 48;
        }
        Ostr += st[i];
    }

    str = ui->pEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (p == 0)
            p = st[i] - 48;
        else{
            p *= 10;
            p += st[i] - 48;
        }
        Pstr += st[i];
    }

    str = ui->qEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (q == 0)
            q = st[i] - 48;
        else{
            q *= 10;
            q += st[i] - 48;
        }
        Qstr += st[i];
    }

    str = ui->rEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (r == 0)
            r = st[i] - 48;
        else{
            r *= 10;
            r += st[i] - 48;
        }
        Rstr += st[i];
    }

    str = ui->sEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (s == 0)
            s = st[i] - 48;
        else{
            s *= 10;
            s += st[i] - 48;
        }
        Sstr += st[i];
    }

    str = ui->tEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (t == 0)
            t = st[i] - 48;
        else{
            t *= 10;
            t += st[i] - 48;
        }
        Tstr += st[i];
    }

    str = ui->uEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (u == 0)
            u = st[i] - 48;
        else{
            u *= 10;
            u += st[i] - 48;
        }
        Ustr += st[i];
    }

    str = ui->vEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (v == 0)
            v = st[i] - 48;
        else{
            v *= 10;
            v += st[i] - 48;
        }
        Vstr += st[i];
    }

    str = ui->wEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (w == 0)
            w = st[i] - 48;
        else{
            w *= 10;
            w += st[i] - 48;
        }
        Wstr += st[i];
    }

    str = ui->xEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (xV == 0)
            xV = st[i] - 48;
        else{
            xV *= 10;
            xV += st[i] - 48;
        }
        Xstr += st[i];
    }

    str = ui->yEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (yV == 0)
            yV = st[i] - 48;
        else{
            yV *= 10;
            yV += st[i] - 48;
        }
        Ystr += st[i];
    }

    str = ui->zEquals->text();
    st = str.toStdString();
    size = str.size();
    for (int i = 2; i < size; i++){
        if (z == 0)
            z = st[i] - 48;
        else{
            z *= 10;
            z += st[i] - 48;
        }
        Zstr += st[i];
    }
}

string MainWindow::convertString(string newStr)
{
    getValues();
    string result, exp = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < newStr.size() - 1; i++){
        if (exp.find(newStr[i]) != -1 && exp.find(newStr[i + 1]) != -1){
            result += newStr[i];
            result += '*';
        }
        else
            result += newStr[i];
    }
    result += newStr[newStr.size() - 1];
    string converted = result;
    result.clear();
    for (int i = 0; i < converted.size(); i++){
        if (isdigit(converted[i]))
            result += converted[i];
        else if (isOperator(converted[i]) || converted[i] == '(' || converted[i] == ')')
            result += converted[i];
        else{
            if (converted[i] == 'a' || converted[i] == 'A'){
                result += Astr;                
            }
            if (converted[i] == 'b' || converted[i] == 'B'){
                result += Bstr;
            }
            if (converted[i] == 'c' || converted[i] == 'C'){
                result += Cstr;                
            }
            if (converted[i] == 'd' || converted[i] == 'D'){
                result += Dstr;                
            }
            if (converted[i] == 'e' || converted[i] == 'E'){
                result += Estr;                
            }
            if (converted[i] == 'f' || converted[i] == 'F'){
                result += Fstr;                
            }
            if (converted[i] == 'g' || converted[i] == 'G'){
                result += Gstr;                
            }
            if (converted[i] == 'h' || converted[i] == 'H'){
                result += Hstr;                
            }
            if (converted[i] == 'i' || converted[i] == 'I'){
                result += Istr;                
            }
            if (converted[i] == 'j' || converted[i] == 'J'){
                result += Jstr;               
            }
            if (converted[i] == 'k' || converted[i] == 'K'){
                result += Kstr;                
            }
            if (converted[i] == 'l' || converted[i] == 'L'){
                result += Lstr;                
            }
            if (converted[i] == 'm' || converted[i] == 'M'){
                result += Mstr;                
            }
            if (converted[i] == 'n' || converted[i] == 'N'){
                result += Nstr;
                            }
            if (converted[i] == 'o' || converted[i] == 'O'){
                result += Ostr;                
            }
            if (converted[i] == 'p' || converted[i] == 'P'){
                result += Pstr;                
            }
            if (converted[i] == 'q' || converted[i] == 'Q'){
                result += Qstr;                
            }
            if (converted[i] == 'r' || converted[i] == 'R'){
                result += Rstr;                
            }
            if (converted[i] == 's' || converted[i] == 'S'){
                result += Sstr;                
            }
            if (converted[i] == 't' || converted[i] == 'T'){
                result += Tstr;               
            }
            if (converted[i] == 'u' || converted[i] == 'U'){
                result += Ustr;                
            }
            if (converted[i] == 'v' || converted[i] == 'V'){
                result += Vstr;               
            }
            if (converted[i] == 'w' || converted[i] == 'W'){
                result += Wstr;
            }
            if (converted[i] == 'x' || converted[i] == 'X'){
                result += Xstr;                
            }
            if (converted[i] == 'y' || converted[i] == 'Y'){
                result += Ystr;
            }
            if (converted[i] == 'z' || converted[i] == 'Z'){
                result += Zstr;                
            }
        }
    }
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
    }
    else{
        converted = convertString(converted);
        converted = toPostfix(converted);
    }
    return converted;
}

//the event that launches the second window where a user is able to see maths operations made
void MainWindow::on_buttnEqual_clicked()
{
    ui->statusbar->showMessage("'=' WAS PRESSED!");
    QString currStr = ui->expr->text();

    if (!currStr.contains('=') && !currStr.isEmpty()){
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
    ClearVariables();
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
    else if (ui->buttnPow->isChecked()){
        Update(key, '^');
    }
}

void MainWindow::ClearVariables()
{
    a = b = c = d = e = f = g = h = i = j = k = l = m = n = o = p = q = r = s = t = u = v = w = xV = yV = z = 0;
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

    QFont font ("Segoe UI", 26, QFont::Bold);   //setting font to normal size
    font.setPointSize(26);                      //
    ui->expr->setFont(font);                    //

    ui->expr->setText("0");
    ui->buttnDelete->setChecked(false);
    ui->statusbar->clearMessage();

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
    QFont font ("Segoe UI", fontSize, QFont::Bold);
    if (toCompare > 30 && curr.size() < toCompare){
        ++fontSize;
        toCompare -= 1;
        font.setPointSize(fontSize);
        ui->expr->setFont(font);
     }
    ClearVariables();
}
