#include "employe.h"
#include <QString>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QObject>
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
using namespace std;

Employe::Employe(int id, QString nom, QString prenom, QString departement, QString sexe, int salaire, int num_tel, QString adresse_email, QString password,int code) {
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->departement = departement;
    this->sexe = sexe;
    this->salaire = salaire;
    this->num_tel = num_tel;
    this->adresse_email = adresse_email;
    this->password = password;
    this->code = code;
}


int Employe::getid() const
{
    return id;

}
QString Employe::getnom() const
{
    return nom;

}
QString Employe::getprenom() const
{
    return prenom;

}
QString Employe::getdepartement() const
{
    return departement;

}
QString Employe::getsexe() const
{
    return sexe;

}
int Employe::getsalaire() const
{
    return salaire;

}
int Employe::getnum_tel() const
{
    return num_tel;

}
QString Employe::getadresse_email() const
{
    return adresse_email;

}
QString Employe::getpassword() const
{
    return password;

}
int Employe::getcode() const
{
    return code;

}

void Employe::setid(int id)
{
    this->id=id;

}
void Employe::setnom(QString nom)
{
    this->nom=nom;
}
void Employe::setprenom(QString prenom)
{
    this->prenom=prenom;
}
void Employe::setdepartement(QString departement)
{
    this->departement=departement;
}
void Employe::setsexe(QString sexe)
{
    this->sexe=sexe;
}
void Employe::setsalaire(int salaire)
{
    this->salaire=salaire;
}
void Employe::setnum_tel(int num_tel)
{
    this->num_tel=num_tel;
}
void Employe::setadresse_email(QString adresse_email)
{
    this->adresse_email=adresse_email;
}
void Employe::setpassword(QString password)
{
    this->password=password;
}
void Employe::setcode(int code)
{
    this->code=code;
}
bool Employe::ajouter()
{

   QSqlQuery query;
   QString res=QString::number(id);
        query.prepare("INSERT INTO EMPLOYE (ID, NOM, PRENOM, DEPARTEMENT, SEXE,SALAIRE,NUM_TEL,ADRESSE_EMAIL,PASSWORD,CODE)" "values (:id, :nom, :prenom, :departement, :sexe, :salaire, :num_tel, :adresse_email, :password, :code)");
        query.bindValue(":id", res);
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":departement", departement);
        query.bindValue(":sexe", sexe);
        query.bindValue(":salaire", salaire);
        query.bindValue(":num_tel", num_tel);
        query.bindValue(":adresse_email", adresse_email);
        query.bindValue(":password", password);
        query.bindValue(":code", code);



        return query.exec();




}
QSqlQueryModel *Employe::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM EMPLOYE ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Département"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Numéro de téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Adresse email"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Mot de passe"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Code"));

    return model;
}


 bool Employe::supprimer(int id)
 {
         QSqlQuery query;

         query.prepare("SELECT * FROM EMPLOYE WHERE ID = :id");
         query.bindValue(":id", id);
         if (query.exec() && query.next()) {
             // L'ID existe,
             query.prepare("DELETE FROM EMPLOYE WHERE ID = :id");
             query.bindValue(":id", id);
             return query.exec();
         } else {
             // L'ID n'existe pas,
             return false;
         }
     }


Employe Employe::rechercher(int id)
 {
     Employe resultat;
     QSqlQuery query;
     query.prepare("SELECT * FROM EMPLOYE WHERE ID = ?");
     query.addBindValue(id);

     if (query.exec() && query.first()) {
         resultat.setid(query.value("id").toInt());
         resultat.setnom(query.value("nom").toString());
         resultat.setprenom(query.value("prenom").toString());
         resultat.setdepartement(query.value("departement").toString());
         resultat.setsexe(query.value("sexe").toString());
         resultat.setsalaire(query.value("salaire").toInt());
         resultat.setnum_tel(query.value("num_tel").toInt());
         resultat.setadresse_email(query.value("adresse_email").toString());
         resultat.setpassword(query.value("password").toString());
         resultat.setcode(query.value("code").toInt());
     }

     return resultat;
 }

 bool Employe::modifier()
 {
      QSqlQuery query;
      query.prepare("UPDATE EMPLOYE SET NOM= :nom, PRENOM= :prenom, DEPARTEMENT= :departement, SEXE= :sexe, SALAIRE= :salaire, NUM_TEL= :num_tel,   ADRESSE_EMAIL= :adresse_email, PASSWORD= :password, CODE= :code  WHERE ID= :id");
     query.bindValue(":id",id);
     query.bindValue(":nom",nom);
     query.bindValue(":prenom",prenom);
     query.bindValue(":departement",departement);
     query.bindValue(":sexe",sexe);
     query.bindValue(":salaire",salaire);
     query.bindValue(":num_tel",num_tel);
     query.bindValue(":adresse_email",adresse_email);
     query.bindValue(":password",password);
     query.bindValue(":code",code);
     return query.exec();

 }

 void Employe::notification(QString ch)
 {
     QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
     QIcon icon("C:/Users/mouha/Desktop/employe19/EMPLOYER/add1.png");
     // Set the loaded icon to the system tray icon
     notifyIcon->setIcon(icon);
     notifyIcon->show();
     notifyIcon->showMessage("!!!!!",ch,QSystemTrayIcon::Information,15000);
 }
 int Employe::mdp_oub(QString code)
 {
     QSqlDatabase bd = QSqlDatabase::database();
 int id;
         QSqlQuery query;
         query.prepare("SELECT ID FROM EMPLOYE WHERE CODE =:code");
  query.bindValue(":code", code);

         query.exec();
         if (query.next())
         {

             id=query.value(0).toInt();
              return id;
         }
         else {
             return -1;
         }



 }

 bool Employe::modifmdp(QString password,int id){
     QSqlQuery query;
     query.prepare("update EMPLOYE set password=:password where id=:id");
                       query.bindValue(":password", password);
                        query.bindValue(":id", id);
     return    query.exec();

 }
 QHash<QString, int> Employe::statistiques()
 {
     QHash<QString, int> statistiques;

     QSqlQueryModel* model = afficher();

     for (int row = 0; row < model->rowCount(); ++row) {
         QString degatStr = model->data(model->index(row, 5)).toString(); // Supposons que les dommages sont dans la colonne 5 (index 4)
         int degat = degatStr.toInt(); // Convertir la chaîne en entier

         QString categorie;

          if (degat <= 2400 && degat >= 1100)
             categorie = "1100 - 2400";
          //else if (degat < 50 && degat>=25 )
            // categorie = "25 - 50";
          else if( degat <1000 && degat>=600)
              categorie = "600 - 1000";
         else if(degat >= 2500)
             categorie = " >=2500";

         if (statistiques.contains(categorie)) {
             statistiques[categorie]++;
         } else {
             statistiques[categorie] = 1;
         }

     }


     return statistiques;
 }
