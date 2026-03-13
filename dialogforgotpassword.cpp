#include "dialogforgotpassword.h"
#include "ui_dialogforgotpassword.h"
#include "ui_dialoglogin.h"
#include "dialogemployes.h"
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDate>
#include <QValidator>
#include <QSqlError>
#include <QDoubleValidator>
#include <QSortFilterProxyModel>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QDir>
//#include <QtCharts/QPieSeries>
//#include <QtCharts/QPieSlice>
//#include <QtCharts/QChartView>
//#include <QtCharts/QChart>
#include <QtCharts>
#include<QChartView>
#include<QPieSlice>
#include<QChart>
#include <QSqlQueryModel>
#include <QDialog>
#include <QSqlRecord>
#include <QPrinter>
Dialogforgotpassword::Dialogforgotpassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogforgotpassword)
{
    ui->setupUi(this);
}

Dialogforgotpassword::~Dialogforgotpassword()
{
    delete ui;
}

void Dialogforgotpassword::on_valider_clicked()
{
    Employe E;
    QString code;
    code=ui->code->text();
    int id =E.mdp_oub(code);
    if(id!=-1)
    {

     QMessageBox::information(this,"Connexion","employé trouvé");
     qDebug()<< "employé trouvé";

    }

   else
    {
      QMessageBox::critical(this,"Connexion","employé introuvable");
      qDebug()<< "employé introuvable";
    }
}
#include "dialoglogin.h"
void Dialogforgotpassword::on_modifier_clicked()
{
    QString code=ui->code->text();
        QString mot_de_passe=ui->nouveaupass->text();

       // Employee E(mot_de_passe);

    Employe E1;

    int id=E1.mdp_oub(code);
    if(id!=-1)
    {

       if(int test1=E1.modifmdp(mot_de_passe,id)==true){
            QMessageBox::information(nullptr, QObject::tr("OK"),
                              QObject::tr("Mot de passe modifié.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

            DialogLogin *E =new DialogLogin;
            E->show();
            hide();
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("modifier un employee"),
                        QObject::tr("Erreur de modification!.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);

        }

    }

        else
        {
            QMessageBox::critical(nullptr, QObject::tr("modifier un employee"),
                        QObject::tr("Erreur !.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);

        }
}

void Dialogforgotpassword::on_Retour_clicked()
{
    DialogLogin *dLogin = new DialogLogin();
    dLogin->show();
    hide();

}
