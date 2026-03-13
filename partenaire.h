#ifndef PARTENAIRE_H
#define PARTENAIRE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDate>
#include <QSqlQueryModel>
class Partenaire
{
private:
    int id_p;
    QString nom;
    QString contact;
    QString avantage;
    QDate date_debut,date_fin;

public:
    Partenaire();
    Partenaire(int,QString,QString,QString,QDate,QDate);

    int Getid_p() const{ return id_p; }
    void Setid_p(int val) { id_p = val; }

    QString Getnom() const{ return nom; }
    void Setnom(QString val) { nom = val; }

    QString Getcontact() const{ return contact; }
    void Setcontact(QString val) { contact = val; }

    QString Getavantage() const{ return avantage; }
    void Setavantage(QString val) { avantage = val; }

    QDate Getdate_debut() const{ return date_debut; }
    void Setdate_debut(QDate val) { date_debut = val; }

    QDate Getdate_fin() const{ return date_fin; }
    void Setdate_fin(QDate val) { date_fin = val; }

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int id_p);
    bool numeroExiste(int id_p);
    Partenaire rechercher(int id_p);
    bool modifier();

    QSqlQueryModel* tri_date_de_debut_asc();
    QSqlQueryModel* tri_date_de_fin_asc();
    QSqlQueryModel* tri_date_de_fin_DESC();
    QSqlQueryModel* tri_date_de_debut_DESC();


};

#endif // PARTENAIRE_H
