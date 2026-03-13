#ifndef DIALOGEMPLOYES_H
#define DIALOGEMPLOYES_H
#include "employe.h"
#include <QDialog>
#include <QSortFilterProxyModel>
#include "mainwindow.h"

namespace Ui {
class DialogEmployes;
}

class DialogEmployes : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEmployes(QWidget *parent = nullptr);
    ~DialogEmployes();
    QSortFilterProxyModel *proxyModel;
    void setResultE(int result); //2----------------------------------------------


private slots:

    void update_label();   // slot permettant la mise à jour du label état de la lampe 1,
    // ce slot est lancé à chaque réception d'un message de Arduino


    void on_pushButton_34_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_39_clicked();

    void on_lineEdit_id3_textChanged(const QString &searchText);

    void on_pushButton_40_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_logout_Employe_clicked();

    void on_pushButton_retour_clicked();

private:
    Ui::DialogEmployes *ui;
    Employe emp;

};

#endif // DIALOGEMPLOYES_H
