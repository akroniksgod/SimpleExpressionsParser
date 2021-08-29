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
    QString ChangeStr(QString start);

    void on_buttnEqual_clicked();
    void Digits();
    void Operations();
    void on_buttnDelete_clicked();
    void on_buttnBackspace_clicked();

    bool isOperator(char op);
    bool isDigit(char op);
    bool Check(std::string result);
    int Priority(char op);
    std::string unarMinus(std::string start);
    std::string removeLastEl(std::string result);
    std::string toPostfix(std::string s);
    bool containsRusCharacters(std::string currStr);
    void ClearVariables();

    float getValues();
    std::string convertString(std::string newStr);

    std::string Parsing(QString s);


    void on_DegOn_toggled(bool checked);

    void on_RadOn_toggled(bool checked);

    void on_buttnInv_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
