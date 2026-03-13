#ifndef CONTRAT_H
#define CONTRAT_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Contrat
{
public:
    Contrat(){}
    Contrat(int,QDate,QDate,QString,QString ,int); // Updated constructor

    void setnum(int n){numero = n;}

    void setdated(QDate n){date_debut = n;}
    void setdatef(QDate n){date_fin = n;}

    void settype(QString n){type = n;}
    void setid_a(int n){id_a = n;}


    void setremise(QString n){remise = n;}
    int getnum()const{return numero;}

    QDate getdated()const{return date_debut;}
    QDate getdatef()const{return date_fin;}

    QString gettype()const{return type;}
    int getid_a()const{return id_a;}
    QString getremise()const{return remise;}


    bool ajouter();
   static QSqlQueryModel * afficher();
   bool supprimer(int);
  static  Contrat rechercher(int numero);
   bool modifier();
   QSqlQueryModel* trie(QString croissance, QString critere);
QSqlQuery  recherchernum(int numero);
void  on_pdf_clicked();
QSqlQuery  recherchername(QString name);
QSqlQuery  rechercherncin(int cin);
static Contrat getContratDetails(int numero);
    static QHash<QString, int> statistiques();

private:
    QString type,remise;
    QDate date_debut,date_fin;
    int numero,id_a;
};

#endif // CONTRAT_H
