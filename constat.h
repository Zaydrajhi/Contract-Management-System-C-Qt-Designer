#ifndef CONSTAT_H
#define CONSTAT_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Constat
{
public:
    Constat(){}
    Constat(int,int,QDateTime,QString,QString,QString);

    void setnum(int n){num = n;}
    void setnumero(int n){numero = n;}
    void setdate(QDateTime n){date_constat = n;}
    void setadresse(QString n){adresse = n;}
    void settype(QString n){type = n;}
    void setdesc(QString n){description = n;}
    void setdegat(QString n){degat = n;}

    int getnum () const{return num;}
    int getnumero () const{return numero;}
    QDateTime getdate() const{return date_constat;}
    QString getadresse() const{return adresse;}
    QString gettype() const{return type;}
    QString getdesc() const{return description;}
    QString getdegat() const{return degat;}

    bool ajouter();
    static QSqlQueryModel * afficher();
    bool supprimer(int);

    static Constat rechercher(int numero);
    bool modifier();
    bool numeroExiste(int numero);

    static QHash<QString, int> statistiques();
    static Constat getConstatDetails(int numConstat);

private:
    QString adresse,type,description,degat;
    QDateTime date_constat;
    int num,numero;

};

#endif // CONSTAT_H
