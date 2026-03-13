#ifndef EMPLOYE_H
#define EMPLOYE_H
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QMainWindow>
#include <QObject>
#include <QWidget>



class Employe
{
public:
    Employe(){};
    Employe(int,QString,QString,QString,QString,int,int,QString,QString,int);
    bool ajouter();
    bool supprimer(int id);
    static QSqlQueryModel * afficher();
    bool modifier();
    static Employe rechercher(int id);
    void notification(QString ch);
    int mdp_oub(QString code);
    bool modifmdp(QString,int);
    static QHash<QString, int> statistiques();


    int getid() const;
    QString getnom() const;
    QString getprenom() const;
    QString getdepartement() const;
    QString getsexe() const;
    int getsalaire() const;
    int getnum_tel() const;
    QString getadresse_email() const;
    QString getpassword() const;
    int getcode() const;

    void setid(int);
     void setnom(QString);
      void setprenom(QString);
       void setdepartement(QString);
       void setsexe(QString);
       void setsalaire(int);
       void setnum_tel(int);
       void setadresse_email(QString);
       void setpassword(QString);
       void setcode(int);


private:
    int id;
    QString nom;
    QString prenom;
    QString departement;
    QString sexe;
    int salaire;
    int num_tel;
    QString adresse_email;
    QString password;
    int code;












};

#endif // EMPLOYE_H
