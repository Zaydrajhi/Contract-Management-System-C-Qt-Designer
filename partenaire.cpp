#include "partenaire.h"
#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QHeaderView>
Partenaire::Partenaire()
{
    id_p=0;
    nom = "";
    contact = "";
    avantage = "";
    date_debut = QDate();
    date_fin=QDate();
}

Partenaire::Partenaire(int id_p, QString nom, QString contact, QString avantage, QDate date_debut, QDate date_fin)
{
    this->id_p=id_p;
    this->nom = nom;
    this->contact = contact;
    this->avantage = avantage;
    this->date_debut = date_debut;
    this->date_fin=date_fin;

}
bool Partenaire::ajouter()
{
   QSqlQuery query;

   query.prepare("INSERT INTO partenaire (id_p, nom, contact, avantage, date_debut, date_fin) VALUES (:id_p, :nom, :contact, :avantage, :date_debut, :date_fin)");

   query.bindValue(":id_p", id_p);
   query.bindValue(":nom", nom);
   query.bindValue(":contact", contact);
   query.bindValue(":avantage", avantage);
   query.bindValue(":date_debut", date_debut);
   query.bindValue(":date_fin", date_fin);


   return query.exec();
}

QSqlQueryModel *Partenaire::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT id_p, nom, contact, avantage, date_debut, date_fin FROM partenaire");

    // Définir les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Avantage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date fin"));

    return model;
}

bool Partenaire::supprimer(int id_p)
{
        QSqlQuery query;

        query.prepare("SELECT * FROM PARTENAIRE WHERE ID_P = :id_p");
        query.bindValue(":id_p", id_p);
        if (query.exec() && query.next()) {
            // L'ID existe,
            query.prepare("DELETE FROM Partenaire WHERE ID_P = :id_p");
            query.bindValue(":id_p", id_p);
            return query.exec();
        } else {
            // L'ID n'existe pas,
            return false;
        }
    }

bool Partenaire::numeroExiste(int id_p) {

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM partenaire WHERE id_p = ?");
    query.addBindValue(id_p);

    if (!query.exec()) {
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return (count > 0);
    }

    return false;
}
Partenaire Partenaire::rechercher(int id_p)
{
    Partenaire resultat;
    QSqlQuery query;
    query.prepare("SELECT * FROM partenaire WHERE id_p = ?");
    query.addBindValue(id_p);

    if (query.exec() && query.first()) {
        resultat.Setid_p(query.value("id_p").toInt());
        resultat.Setnom(query.value("nom").toString());
        resultat.Setcontact(query.value("contact").toString());
        resultat.Setavantage(query.value("avantage").toString());
        resultat.Setdate_debut(query.value("date_debut").toDate());
        resultat.Setdate_fin(query.value("date_fin").toDate());


    }

    return resultat;
}
bool Partenaire::modifier()
{
//    QSqlQuery query;
//    query.prepare("UPDATE partenaire SET id_p = ?, nom = ?, contact = ?, avantage = ?, date_debut = ?, date_fin = ? WHERE id_p = ?");
//    query.addBindValue(this->id_p);
//    query.addBindValue(this->nom);
//    query.addBindValue(this->contact);
//    query.addBindValue(this->avantage);
//    query.addBindValue(this->date_debut);
//    query.addBindValue(this->date_fin);

//    return query.exec();
    QSqlQuery query;
    query.prepare("UPDATE partenaire SET id_p=:id_p, date_debut=:date_debut, date_fin=:date_fin, "
                  "nom=:nom, contact=:contact, avantage=:avantage WHERE id_p=:id_p");
    query.bindValue(":id_p", id_p);
    query.bindValue(":nom", nom);
    query.bindValue(":contact", contact);
    query.bindValue(":avantage", avantage);

    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);

    return query.exec();

}

QSqlQueryModel* Partenaire::tri_date_de_debut_asc()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM partenaire ORDER BY DATE_DEBUT ASC"); // Query modified to order by DATE_DE_DEBUT in ascending order
    // Définir les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Avantage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date fin"));


    // Create a table view
    QTableView* tableView = new QTableView;
    tableView->setModel(model);

    // Get the header view
    QHeaderView* headerView =tableView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);


    return model;
}

QSqlQueryModel* Partenaire::tri_date_de_fin_asc()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM partenaire ORDER BY DATE_FIN ASC"); // Query modified to order by DATE_DE_DEBUT in ascending order
    // Définir les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Avantage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date fin"));


    // Create a table view
    QTableView* tableView = new QTableView;
    tableView->setModel(model);

    // Get the header view
    QHeaderView* headerView =tableView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);


    return model;
}

QSqlQueryModel* Partenaire::tri_date_de_debut_DESC()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM partenaire ORDER BY DATE_DEBUT DESC"); // Query modified to order by DATE_DE_DEBUT in ascending order
    // Définir les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Avantage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date fin"));


    // Create a table view
    QTableView* tableView = new QTableView;
    tableView->setModel(model);

    // Get the header view
    QHeaderView* headerView =tableView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);


    return model;
}

QSqlQueryModel* Partenaire::tri_date_de_fin_DESC()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM partenaire ORDER BY DATE_FIN DESC"); // Query modified to order by DATE_DE_DEBUT in ascending order
    // Définir les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Avantage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date fin"));


    // Create a table view
    QTableView* tableView = new QTableView;
    tableView->setModel(model);

    // Get the header view
    QHeaderView* headerView =tableView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);


    return model;
}
