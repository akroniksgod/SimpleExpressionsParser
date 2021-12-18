#ifndef SEQUENCEOFOPERATIONS_H
#define SEQUENCEOFOPERATIONS_H

#include <QDialog>
#include <QChar>
#include <QString>
#include <string>

namespace Ui {
class SequenceOfOperations;
}

class SequenceOfOperations : public QDialog
{
    Q_OBJECT

public:
    explicit SequenceOfOperations(QWidget *parent = nullptr);
    ~SequenceOfOperations();

    float CalcByPostfixNotation(std::string s, bool DegOrRad);
    bool IsDigit(char op);
private slots:
    void PrintExprInTheWin(float op1, float op2, char op, int i);
    void PrintExprInTheWin(float op1, QString op, int i);
    void PrintExprInTheWin(float op1, float op2, QString op, int i);

private:
    Ui::SequenceOfOperations *ui;
};

#endif // SEQUENCEOFOPERATIONS_H
