#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include"arduino.h"
#include "mainwindow.h"//--------------------------------------------------------
namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogin(QWidget *parent = nullptr);
    ~DialogLogin();
QString getUserRole() const;
private slots:
    //void on_pushButton_login_clicked();
    void loginn();
    //void on_loginButton_clicked();

    void on_loginButton_2_clicked();

    //void on_loginButton_clicked();

    void on_checkBox_2_stateChanged(int arg1);
void  update_label();
    void on_pushButton_Quit_clicked();


signals:
  void loginSuccessful();
private:
    Ui::DialogLogin *ui;
    Arduino Ar;
    QByteArray code;
};

#endif // DIALOGLOGIN_H
