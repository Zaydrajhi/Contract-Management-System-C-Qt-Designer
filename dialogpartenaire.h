#ifndef DIALOGPARTENAIRE_H
#define DIALOGPARTENAIRE_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class DialogPartenaire;
}

class DialogPartenaire : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPartenaire(QWidget *parent = nullptr);
    ~DialogPartenaire();
    void setResultP(int result); //2----------------------------------------------


private slots:
    void on_pushButton_retour_Partenaire_clicked();

    void on_pushButton_logout_Partenaire_clicked();

    void on_pushButton_ajouter_clicked();

    void on_pushButton_supprimer_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_modifier_clicked();




    void on_trier_clicked();


    void on_lineEdit_45_textChanged(const QString &arg1);

    void on_pdf_2_clicked();

private:
    Ui::DialogPartenaire *ui;
    Partenaire Ptmp;
};

#endif // DIALOGPARTENAIRE_H
