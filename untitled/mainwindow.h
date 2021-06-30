#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QChar>
#include <QString>
#include <QStatusBar>
#include <QMessageBox>
#include <string>

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
    void on_buttnEqual_clicked();
    void Digits();
    void Operations();
    void on_buttnDelete_clicked();
    void on_buttnBackspace_clicked();

    bool isOperand(char op);
    bool isOperator(char op);
    bool Check(std::string result);
    int Priority(char op);
    std::string removeLastEl(std::string result);
    std::string toPostfix(std::string s);
    //void Calculate(std::string s);
    std::string Parsing(QString s);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
