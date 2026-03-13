#include "contrat.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QPdfWriter>
#include <QPainter>
#include <QFont>
#include <QMessageBox>
#include <QSqlRecord>



Contrat::Contrat(int numero,  QDate date_debut, QDate date_fin, QString type, QString remise,int id_a)
{
    this->numero = numero;
    this->id_a = id_a;
    this->date_debut = date_debut;
    this->date_fin = date_fin;

    this->type = type;

    this->remise = remise;

}

bool Contrat::ajouter()
{
    QSqlQuery query;
    QString res = QString::number(numero);
    QString resc = QString::number(id_a);
    query.prepare("insert into contrat (numero, date_debut, date_fin, type, remise, id_a) "
                  "values(:numero,  :date_debut, :date_fin,  :type,  :remise, :id_a)"); // Modified query
    query.bindValue(":numero", res);
    query.bindValue(":id_a", resc);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);

    query.bindValue(":type", type);

    query.bindValue(":remise", remise);


    return query.exec();
}


QSqlQueryModel* Contrat::trie(QString croissance, QString critere)
{
    QSqlQueryModel* modal = new QSqlQueryModel();

    QString queryString;

    if (croissance == "ASC" || croissance == "DESC")
    {
        queryString = "SELECT * FROM contrat ORDER BY " + critere + " " + croissance;

        QSqlQuery query;
        query.prepare(queryString);

        query.exec();
        modal->setQuery(query);

        if (query.lastError().isValid()) {
            qDebug() << "Error in SQL Query: " << query.lastError().text();
            delete modal; // Release memory in case of an error
            return nullptr;
        }

        // You can add additional code here if needed
    }
    else
    {
        // Handle invalid croissance
        qDebug() << "Invalid croissance value. Please use ASC or DESC.";
        delete modal; // Release memory in case of an error
        return nullptr;
    }

    return modal;
}


/*bool Contrat::supprimer(int numero)
{
    QSqlQuery query;
    query.prepare("DELETE FROM contrat WHERE NUMERO = :numero");
    query.bindValue(":numero", numero);
    return query.exec();
}*/

//***************************************************************************

bool Contrat::supprimer(int numero)
{
    QSqlQuery query;
    query.prepare("DELETE FROM contrat WHERE NUMERO = :numero");
    query.bindValue(":numero", numero);

    // Supprimer les constats associés à ce contrat
    QSqlQuery deleteConstatsQuery;
    deleteConstatsQuery.prepare("DELETE FROM constat WHERE numero = :numero");
    deleteConstatsQuery.bindValue(":numero", numero);

    bool deleteConstatsSuccess = deleteConstatsQuery.exec();
    if (!deleteConstatsSuccess) {
        qDebug() << "Failed to delete associated constats for contract number: " << numero;
        // Vous pouvez choisir de gérer l'erreur ici si nécessaire
    }

    return query.exec();
}

//**************************************************************************


bool Contrat::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE contrat SET id_a=:id_a, date_debut=:date_debut, date_fin=:date_fin, "
                  " type=:type, remise=:remise WHERE numero=:numero");
    query.bindValue(":numero", numero);
    query.bindValue(":id_a", id_a);
    query.bindValue(":date_debut", date_debut);
    query.bindValue(":date_fin", date_fin);

    query.bindValue(":type", type);

    query.bindValue(":remise", remise);


    return query.exec();
}
QSqlQuery Contrat::recherchername(QString name)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM contrat WHERE nom like :nom");
    query.bindValue(":nom", "%" + name + "%"); // Adding wildcards for partial matching

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        // Handle error here, return an empty QSqlQuery or throw an exception
    }

    return query;
}
QSqlQuery Contrat::recherchernum(int numero)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM contrat WHERE numero = :numero");
    query.bindValue(":numero", numero); // No need to use wildcards for exact match

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        // Handle error here, return an empty QSqlQuery or throw an exception
    }

    return query;
}

QSqlQuery Contrat::rechercherncin(int id_a)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM contrat WHERE id_a = :id_a");
    query.bindValue(":id_a", id_a); // No need to use wildcards for exact match

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        // Handle error here, return an empty QSqlQuery or throw an exception
    }
    return query;

}


Contrat Contrat::rechercher(int numero)
{
    Contrat resultat;
    QSqlQuery query;
    query.prepare("SELECT * FROM contrat WHERE numero = ?");
    query.addBindValue(numero);

    if (query.exec() && query.first()) {
        resultat.setnum(query.value("numero").toInt());

        resultat.settype(query.value("type").toString());
        resultat.setremise(query.value("remise").toString());


        resultat.setdated(query.value("date_debut").toDate());
        resultat.setdatef(query.value("date_fin").toDate());
        resultat.setid_a(query.value("id_a").toInt());


    }

    return resultat;
}
void Contrat::on_pdf_clicked() {
    QSqlQueryModel *model = afficher();

    if (model->rowCount() > 0) {
        QPdfWriter writer("assurance_report.pdf"); // create PDF writer
        QPainter painter(&writer); // create painter
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // set painter options
        painter.setFont(QFont("Helvetica", 12)); // set font

        int x = 100;
        int y = 100;
        painter.drawText(x, y, "information: "); // draw text
        y += 300; // Adjust y-coordinate for next line

        // Iterate over each row in the model
        for (int row = 0; row < model->rowCount(); ++row) {
            QSqlRecord record = model->record(row);
            // Extract patient information from the record
            int id_a = record.value("id_a").toInt();

            QString date_debut = record.value("date_debut").toString();
            QString date_fin = record.value("date_fin").toString();
            QString type = record.value("type").toString();
            QString remise = record.value("remise").toString();


            // Write patient information to the PDF
            painter.drawText(x, y, "ID Adhérent : " + QString::number(id_a));
            y += 200;

            painter.drawText(x, y, "Date Debut: " + date_debut);
            y += 200;
            painter.drawText(x, y, "Date Fin: " + date_fin);
            y += 200;
            painter.drawText(x, y, "Type: " + type);
            y += 200;
            painter.drawText(x, y, "Remise: " + remise);
            y += 200;

        }

        painter.end(); // end painting
        QMessageBox::information(nullptr, QObject::tr("PDF Report"),
                                QObject::tr("PDF report generated successfully!"),
                                QMessageBox::Ok);
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("No patient records found!\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}
QHash<QString, int> Contrat::statistiques()
{
    QHash<QString, int> statistiques;

    QSqlQueryModel* model = afficher();

    for (int row = 0; row < model->rowCount(); ++row) {
        QString type = model->data(model->index(row, 4)).toString();

        // Use 'type' as the grouping key instead of 'qteGroup'
        if (statistiques.contains(type)) {
            statistiques[type]++;
        } else {
            statistiques[type] = 1;
        }
    }

    return statistiques;
}
