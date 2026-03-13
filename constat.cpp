#include "constat.h"
#include <QDebug>
#include <QSqlError>

Constat::Constat(int num,int numero,QDateTime date_constat,QString adresse,QString description,QString degat)
{
    this->num=num;
    this->numero=numero;
    this->date_constat=date_constat;
    this->adresse=adresse;
    this->description=description;
    this->degat=degat;

}

bool Constat::ajouter()
{
    QSqlQuery query;
    QString res = QString::number(num);
    QString ress = QString::number(numero);
    query.prepare("insert into constat (num,numero,date_constat,adresse,description,degat)" "values(:num, :numero, :date_constat, :adresse, :description, :degat)");
    query.bindValue(":num",res);
    query.bindValue(":numero",ress);
    query.bindValue(":date_constat",date_constat);
    query.bindValue(":adresse",adresse);
    query.bindValue(":description",description);
    query.bindValue(":degat",degat);

    return query.exec();
}

bool Constat::supprimer(int num)
{

    QSqlQuery query;
    QString res=QString::number(num);
    query.prepare("Delete from constat where num= :num");
    query.bindValue(":num",res);
    return query.exec();
}

QSqlQueryModel * Constat::afficher()
{

    QSqlQueryModel * model=new QSqlQueryModel();
     model->setQuery("SELECT CAST(num AS VARCHAR(255)), CAST(numero AS VARCHAR(255)), date_constat, adresse, description, degat  FROM constat");
  //  model->setQuery("select * from constat");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("numero constat"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("numero contrat"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("date constat"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("adresse"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("description"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("degat (%)"));

    return model;
}









Constat Constat::rechercher(int num)
{
    Constat resultat;
    QSqlQuery query;
    query.prepare("SELECT * FROM constat WHERE num = ?");
    query.addBindValue(num);

    if (query.exec() && query.first()) {
        resultat.setnum(query.value("num").toInt());
        resultat.setnumero(query.value("numero").toInt());
        resultat.setdate(query.value("date_constat").toDateTime());
        resultat.setadresse(query.value("adresse").toString());
        resultat.setdesc(query.value("description").toString());
        resultat.setdegat(query.value("degat").toString());
    }

    return resultat;
}

bool Constat::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE constat SET numero= ?, date_constat = ?, adresse = ?, description = ?, degat = ? WHERE num = ?");
    query.addBindValue(this->numero);
    query.addBindValue(this->date_constat);
    query.addBindValue(this->adresse);
    query.addBindValue(this->description);
    query.addBindValue(this->degat);
    query.addBindValue(this->num);

    return query.exec();
}

bool Constat::numeroExiste(int num) {

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM constat WHERE num = ?");
    query.addBindValue(num);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return (count > 0);
    }

    return false;
}







/*QHash<QString, int> Constat::statistiques()
{
    QHash<QString, int> statistiques;

        QSqlQueryModel* model = afficher();

        for (int row = 0; row < model->rowCount(); ++row) {
            QString type = model->data(model->index(row, 3)).toString();

            if (statistiques.contains(type)) {
                statistiques[type]++;
            } else {
                statistiques[type] = 1;
            }
        }

        return statistiques;
}
QHash<QString, int> Constat::statistiques()
{
    QHash<QString, int> statistiques;

    QSqlQueryModel* model = afficher();

    for (int row = 0; row < model->rowCount(); ++row) {
        int degat = model->data(model->index(row, 5)).toInt();
        QString categorie;
        if (degat < 50)
            categorie = "< 50";
        else if (degat <= 75)
            categorie = "50 - 75";
        else
            categorie = "> 75";

        if (statistiques.contains(categorie)) {
            statistiques[categorie]++;
        } else {
            statistiques[categorie] = 1;
        }
    }

    return statistiques;
}*/

QHash<QString, int> Constat::statistiques()
{
    QHash<QString, int> statistiques;

    QSqlQueryModel* model = afficher();

    for (int row = 0; row < model->rowCount(); ++row) {
        QString degatStr = model->data(model->index(row, 5)).toString(); // Supposons que les dommages sont dans la colonne 5 (index 4)
        int degat = degatStr.toInt(); // Convertir la chaîne en entier

        QString categorie;

         if (degat <= 75 && degat >= 50)
            categorie = "50 - 75";
         else if (degat < 50 && degat>=25 )
            categorie = "25 - 50";
         else if( degat <25 && degat>=0)
             categorie = "0 - 25";
        else if(degat <= 100 && degat > 75)
            categorie = "> 75";

        if (statistiques.contains(categorie)) {
            statistiques[categorie]++;
        } else {
            statistiques[categorie] = 1;
        }

    }


    return statistiques;
}



Constat Constat::getConstatDetails(int numConstat)
{


       // Exécuter la requête SQL pour récupérer les détails du constat
       QSqlQuery query;
       query.prepare("SELECT numero, date_constat, adresse, description, degat FROM constat WHERE num = :num");
       query.bindValue(":num", numConstat);
       if (!query.exec()) {
           qDebug() << "Erreur lors de l'exécution de la requête SQL :" << query.lastError().text();
           return Constat(); // Retourner un objet Constat vide en cas d'erreur de requête
       }

       // Récupérer les informations de la requête et les utiliser pour initialiser un objet Constat
       Constat constat;
       if (query.next()) {
           constat.setnum(numConstat);
            constat.setnumero(query.value("numero").toInt());
           constat.setdate(query.value("date_constat").toDateTime());
           constat.setadresse(query.value("adresse").toString());
           constat.setdesc(query.value("description").toString());
           constat.setdegat(query.value("degat").toString());
       }

       // Retourner l'objet Constat avec les détails récupérés
       return constat;
}
