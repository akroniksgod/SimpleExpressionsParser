#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QChar>
#include <QString>
#include <QStatusBar>
#include <QMessageBox>
#include <string>
#include <QUrl>
#include <QtCore>
#include <QDesktopServices>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void Update(QPushButton *&key, QChar op);

    void UPD(QPushButton *&key, QString op);
    void UPD(QString op);
    QString ShortenStrForOperations(QString start);

    void on_buttnEqual_clicked();
    void Digits();
    void Operations();
    void on_buttnDelete_clicked();
    void on_buttnBackspace_clicked();

    bool IsOperator(char op);
    bool isDigit(char op);
    bool ProperSymbCheck(std::string result);
    int ReturnPriorityVal(char op);
    std::string CheckForUnarMinus(std::string start);
    std::string RemoveLastEl(std::string result);
    std::string ToPostfixNotation(std::string s);
    bool CheckIfContainsRusCharacters(std::string currStr);
    void ClearVariables();

    float GetValues();
    std::string InsertValuesInsteadOfVars(std::string newStr);

    std::string Parsing(QString s);


    void on_DegOn_toggled(bool checked);

    void on_RadOn_toggled(bool checked);

    void on_buttnInv_clicked();

    bool ContainsSpaceAfterOperator(std::string currStr);

    void on_buttnExpand_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
