#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adherent.h"//2-------------------------------------------------------------------------
#include "employe.h"
#include "partenaire.h"
#include "contrat.h"
#include "constat.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
    void setResult(int result); //------------------------------------------------------
    void setResultRA(int result); // Déclaration de la fonction pour définir le résultat
    void setResultRP(int result); // Déclaration de la fonction pour définir le résultat
    void setResultRE(int result); // Déclaration de la fonction pour définir le résultat
    void setResultRCont(int result); // Déclaration de la fonction pour définir le résultat
    void setResultRCons(int result); // Déclaration de la fonction pour définir le résultat

private slots:
    void on_pushButton_adherent_clicked();

    void on_pushButton_employe_clicked();

    void on_pushButton_contrat_clicked();

    void on_pushButton_constat_clicked();

    void on_pushButton_partenaire_clicked();

    void on_pushButton_LOGOUT_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
