#ifndef DIALOGCONSTAT_H
#define DIALOGCONSTAT_H
#include "constat.h"
#include <QDialog>
#include <QSortFilterProxyModel>
//#include "constatstatistics.h"
#include <QLabel>
#include <QWidget>
#include <QVariant>
#include <QtCore>
#include <QtGui>
#include <QtQuick>
#include "mainwindow.h"

namespace Ui {
class DialogConstat;
}

class DialogConstat : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConstat(QWidget *parent = nullptr);
    ~DialogConstat();
    void setQmlObject(QQuickItem *object) {
           qmlObject = object;
       }
    void setResultCons(int result); //2----------------------------------------------


private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_modifier_clicked();

    void on_pushButton_supprimer_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_refreche_clicked();

    void chercherConstat(const QString &text);

    void chercherConstatNum(const QString &text);

    void chercherConstatNumCon(const QString &text);

    void on_pushButton_trier_clicked();

    void on_pushButton_trier_croi_clicked();

     void on_tableView_clicked(const QModelIndex &index);

     void on_pushButton_statistique_clicked();

     void on_pushButton_35_clicked();

     void on_pushButton_pdfS_clicked();



      void mailSent(const QString &status);


      void on_sendButton_clicked();


      void on_pushButtonq8_clicked();

      void on_pushButtonq1_clicked();

      void on_pushButtonq2_clicked();

      void on_pushButtonq3_clicked();

      void on_pushButtonq4_clicked();

      void on_pushButtonq5_clicked();

      void on_pushButtonq6_clicked();

      void on_pushButtonq7_clicked();

      void on_pushButton_retour_clicked();

      void on_pushButton_logout_Adherent_clicked();


private:
    Ui::DialogConstat *ui;
    Constat Cons;
    QSortFilterProxyModel *proxyModel;
     QQuickItem *qmlObject;


};


#endif // DIALOGCONSTAT_H
