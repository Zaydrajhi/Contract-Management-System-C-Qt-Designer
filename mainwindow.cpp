#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogadherent.h"
#include "dialogemployes.h"
#include "dialogcontrat.h"
#include "dialogconstat.h"
#include "dialogpartenaire.h"
#include "dialoglogin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_adherent_clicked()
{
    /*
    DialogAdherent dAdherent;
    dAdherent.exec();
    */
    int nombreEntier2 = ui->label_id->text().toInt(); //2---------------------------------------------
    DialogAdherent *dAdherent = new DialogAdherent();
    dAdherent->setResult2(nombreEntier2); //2---------------------------------------


    dAdherent->show();
    //this->close();
    hide();

}

void MainWindow::on_pushButton_employe_clicked()
{
    int nombreEntier2 = ui->label_id->text().toInt(); //2---------------------------------------------
    DialogEmployes *dEmploye = new DialogEmployes;
    dEmploye->setResultE(nombreEntier2); //2---------------------------------------
    dEmploye->show();
    hide();
}

void MainWindow::on_pushButton_contrat_clicked()
{
    int nombreEntier2 = ui->label_id->text().toInt(); //2---------------------------------------------

    DialogContrat *dContrat = new DialogContrat;
    dContrat->setResultCont(nombreEntier2); //2---------------------------------------
    dContrat->show();
    hide();
}

void MainWindow::on_pushButton_constat_clicked()
{
    int nombreEntier2 = ui->label_id->text().toInt(); //2---------------------------------------------

    DialogConstat *dConstat = new DialogConstat;
    dConstat->setResultCons(nombreEntier2); //2---------------------------------------
    dConstat->show();
    hide();
}

void MainWindow::on_pushButton_partenaire_clicked()
{
    int nombreEntier2 = ui->label_id->text().toInt(); //2---------------------------------------------

    DialogPartenaire *dPartenaire = new DialogPartenaire;
    dPartenaire->setResultP(nombreEntier2); //2---------------------------------------
    dPartenaire->show();
    hide();
}

void MainWindow::on_pushButton_LOGOUT_clicked()
{
    DialogLogin *dLogin = new DialogLogin();
    dLogin->show();
    //this->close();
    hide();

}
void MainWindow::setResult(int result)
{
    ui->label_id->setText(QString::number(result)); // Afficher le résultat dans le Label
}
void MainWindow::setResultRA(int result)
{
    ui->label_id->setText(QString::number(result)); // Afficher le résultat dans le Label
}
void MainWindow::setResultRP(int result)
{
    ui->label_id->setText(QString::number(result)); // Afficher le résultat dans le Label
}
void MainWindow::setResultRE(int result)
{
    ui->label_id->setText(QString::number(result)); // Afficher le résultat dans le Label
}
void MainWindow::setResultRCons(int result)
{
    ui->label_id->setText(QString::number(result)); // Afficher le résultat dans le Label
}
void MainWindow::setResultRCont(int result)
{
    ui->label_id->setText(QString::number(result)); // Afficher le résultat dans le Label
}
