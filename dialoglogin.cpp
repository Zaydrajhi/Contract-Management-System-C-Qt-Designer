#include "dialoglogin.h"
#include "ui_dialoglogin.h"
#include "mainwindow.h"
#include "dialogemployes.h"
#include <QDebug>
#include<QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>
#include "dialogforgotpassword.h"
#include "arduino.h"

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    connect(ui->cancelButton_2, &QPushButton::clicked,this,&DialogLogin::close );
     connect(ui->loginButton, &QPushButton::clicked,this,&DialogLogin::loginn);
     int ret=Ar.connect_arduino(); // lancer la connexion à arduino

     switch(ret){
     case(0):qDebug()<< "arduino is available and connected to : "<< Ar.getarduino_port_name();
         break;
     case(1):qDebug() << "arduino is available but not connected to :" <<Ar.getarduino_port_name();
        break;
     case(-1):qDebug() << "arduino is not available";
     }
      QObject::connect(Ar.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
      //le slot update_label suite à la reception du signal readyRead (reception des données).

}
void DialogLogin::update_label()
{

    code = "";
    code = Ar.read_from_arduinoo();
    QString result;
    int nedhir=0;
    //qDebug() << "Voilà le code: "<<code;
    if (code != "") {
        if(code.length()==11){ result = Ar.loginnn(code);
        nedhir=1;}
        else if(code.length()!=11){ result = Ar.login(code);}


        if (result == "1" ) {
                Ar.write_to_arduino("1");
        } else if (result == "0" ){
            qDebug() << "ID does not exist";
           Ar.write_to_arduino("0");
        }

        qDebug() << result;
        QByteArray x = result.toUtf8();
        qDebug() << x;

        qDebug() << "validated";
    }
    if(nedhir==1){return;}
//*************************************************************************************
    //QString result;

    code1 = "";
    code1 = Ar.read_from_arduino();
    QString result1;
    //qDebug() << "Voilà le code: "<<code;
    if (code1 != "") {
        if(code1.length()==11){ result1 = Ar.loginnn(code1);}
        else if(code1.length()!=11){ result1 = Ar.login(code1);}


        if (result1 == "1" ) {
                Ar.write_to_arduino("1");
        } else if (result1 == "0" ){
            qDebug() << "ID does not exist";
           Ar.write_to_arduino("0");
        }

        qDebug() << result1;
        QByteArray x = result1.toUtf8();
        qDebug() << x;

        qDebug() << "validated";
    }

}

DialogLogin::~DialogLogin()
{
    delete ui;
}



void DialogLogin::loginn()
{
    QString enteredId = ui->userlineEdit->text();
    QString enteredPassword = ui->passlineEdit_2->text();
    //QString userRole;
    QSqlQuery query;

    query.prepare("SELECT * FROM EMPLOYE WHERE ID = :id AND PASSWORD = :password");
    query.bindValue(":id", enteredId);
    query.bindValue(":password", enteredPassword);

    if (query.exec() && query.next()) {
        // Identifiant et mot de passe corrects


        close();  // Ferme la fenêtre de connexion
        emit loginSuccessful();

        QString poste = query.value("DEPARTEMENT").toString();
        qDebug() << "Contenu de Role :" << poste;

        if (poste == "Informatique") {
            int nombreEntier = ui->userlineEdit->text().toInt();//-----------------------------------------------------------------------------

            MainWindow* mainWindow = new MainWindow;

            mainWindow->show();
            mainWindow->setResult(nombreEntier); //-------------------------------------------------------------------

        } else {
            // Afficher un message d'erreur si le département n'est pas "Informatique"
            QMessageBox::critical(this, "Erreur", "Le département de l'utilisateur n'est pas autorisé.");
        }
    } else {
        // Identifiant ou mot de passe incorrect
        QMessageBox m;
        QMessageBox::critical(this, "Erreur", "Identifiant ou mot de passe incorrect.");
        m.setIcon(QMessageBox::Critical);
    }
}

/*void DialogLogin::on_pushButton_login_clicked()
{
    MainWindow *M =new MainWindow;
    M->show();
    hide();
}
*/



void DialogLogin::on_loginButton_2_clicked()
{
    Dialogforgotpassword *D =new Dialogforgotpassword;
    D->show();
    hide();
}

void DialogLogin::on_checkBox_2_stateChanged(int arg1) {
    // Vérifie si la case à cocher est cochée
    if (arg1 == Qt::Checked) {
        // Affiche le mot de passe en texte clair
        ui->passlineEdit_2->setEchoMode(QLineEdit::Normal);
    } else {
        // Masque le mot de passe
        ui->passlineEdit_2->setEchoMode(QLineEdit::Password);
    }
}

void DialogLogin::on_pushButton_Quit_clicked()
{
    ui->passlineEdit_2->clear();
    ui->userlineEdit->clear();
}
