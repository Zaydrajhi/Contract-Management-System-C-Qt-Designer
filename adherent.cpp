#include "adherent.h"
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDebug>


Adherent::Adherent()
{
   id_a = 0;
   nom = "";
   prenom = "";
   email = "";
   ville = "";
   num_tel = 0;
   fidelite = 0;
   cin = 0;
   image_a="";
}

Adherent::Adherent(int id_a, QString nom, QString prenom, QString email, QString ville, int num_tel, int fidelite, int cin, QString image_a)
{
   this->id_a = id_a;
   this->nom = nom;
   this->prenom = prenom;
   this->email = email;
   this->ville = ville;
   this->num_tel = num_tel;
   this->fidelite = fidelite;
   this->cin = cin;
   this->image_a = image_a;
}

Adherent::Adherent(QString nom, QString prenom, QString email, QString ville, int num_tel, int fidelite, int cin, QString image_a)
{
   this->nom = nom;
   this->prenom = prenom;
   this->email = email;
   this->ville = ville;
   this->num_tel = num_tel;
   this->fidelite = fidelite;
    this->cin = cin;
    this->image_a = image_a;
}

bool Adherent::ajouter()
{
   QSqlQuery query;

   query.prepare("INSERT INTO adherent (nom, prenom, email, ville, num_tel, fidelite, cin, image_a, id_e) VALUES (:nom, :prenom, :email, :ville, :num_tel, :fidelite, :cin, :image_a, :id_e)");

   query.bindValue(":nom", nom);
   query.bindValue(":prenom", prenom);
   query.bindValue(":email", email);
   query.bindValue(":ville", ville);
   query.bindValue(":num_tel", num_tel);
   query.bindValue(":fidelite", fidelite);
   query.bindValue(":cin", cin);
   query.bindValue(":image_a", image_a); // Ajout de l'image
   query.bindValue(":id_e", id_e); // Ajout de l'image

   //historique(1, nom + " " + prenom, "","");

   return query.exec();
}

QSqlQueryModel *Adherent::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT CAST(id_a AS VARCHAR(255)), nom, prenom, email, ville, CAST(num_tel AS VARCHAR(255)), fidelite, CAST(cin AS VARCHAR(255)), image_a FROM adherent");

    // Définir les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Fidélité"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Image")); // Ajout de l'en-tête pour l'image

    return model;
}



#include "ui_dialogadherent.h" // Assurez-vous que le nom du fichier d'en-tête correspond à votre classe
#include "dialogadherent.h" // Assurez-vous que le nom du fichier d'en-tête correspond à votre classe

bool Adherent::supprimer(int id, int idEmploye)
{
    //*****************************************************************************************************************************
                // Récupérer l'ID de l'employé
                //int idEmploye = ui->label_id_employe_join->text().toInt();

                // Exécuter une requête SQL pour récupérer le nom de l'employé correspondant à l'ID
                QSqlQuery queryE;
                QString nomEmploye;
                queryE.prepare("SELECT NOM FROM employe WHERE ID = :id");
                queryE.bindValue(":id", idEmploye);
                if (queryE.exec() && queryE.next()) {
                    // Si la requête s'exécute avec succès et qu'elle retourne un résultat
                    // Récupérer le nom de l'employé
                     nomEmploye = queryE.value(0).toString();

                    // Afficher le nom de l'employé dans label_id_e
                    //*****************/ui->label_id_e->setText(nomEmploye);
                } else {
                    // Gérer les erreurs ou l'absence de résultat
                    // Par exemple, afficher un message d'erreur
                    qDebug() << "Erreur lors de la récupération du nom de l'employé";
                }

    //*****************************************************************************************************************************

    QSqlQuery query;
   QString res=QString::number(id);

   QSqlQuery query2;
   query2.prepare("SELECT nom, prenom FROM adherent WHERE ID_A = :id");
   query2.bindValue(":id", id); // Correction : Utiliser id au lieu de nom et prenom
   query2.exec();
   if (query2.next()) {
       QString nom = query2.value(0).toString();
       QString prenom = query2.value(1).toString();

       historique(3, nom + " " + prenom, "",nomEmploye);



      // historique(3, nom + " " + prenom,"","");
   }

   query.prepare("Delete from adherent where ID_A= :id");
   query.bindValue(":id",res);

   //----------------------------------------------------------------------------------------------------------------------------------
   QSqlQuery deleteConstatsQuery;
      deleteConstatsQuery.prepare("DELETE FROM constat WHERE numero IN (SELECT numero FROM contrat WHERE id_a = :id)");
      deleteConstatsQuery.bindValue(":id", id);
      bool deleteConstatsSuccess = deleteConstatsQuery.exec();

      if (!deleteConstatsSuccess) {
          qDebug() << "Failed to delete associated constats for adherent ID: " << id;
          // Vous pouvez choisir de gérer l'erreur ici si nécessaire
      }


   QSqlQuery deleteContratQuery;
   deleteContratQuery.prepare("DELETE FROM contrat WHERE id_a = :id");
   deleteContratQuery.bindValue(":id", id);

   bool deleteContratSuccess = deleteContratQuery.exec();
   if (!deleteContratSuccess) {
       qDebug() << "Failed to delete associated constats for contract number: " << id;
       // Vous pouvez choisir de gérer l'erreur ici si nécessaire
   }



   //-------------------------------------------------------------------------------------------------------------------------------


   return query.exec();
}




Adherent Adherent::rechercher(int id_a)
{
    Adherent resultat;
    QSqlQuery query;
    query.prepare("SELECT * FROM adherent WHERE id_a = ?");
    query.addBindValue(id_a);

    if (query.exec() && query.first()) {
        resultat.Setid_a(query.value("id_a").toInt());
        resultat.Setnom(query.value("nom").toString());
        resultat.Setprenom(query.value("prenom").toString());
        resultat.Setemail(query.value("email").toString());
        resultat.Setville(query.value("ville").toString());

        resultat.Setnum_tel(query.value("num_tel").toInt());
        resultat.Setfidelite(query.value("fidelite").toInt());
        resultat.Setcin(query.value("cin").toInt());
        resultat.Setimage_a(query.value("image_a").toString());


    }

    return resultat;
}

bool Adherent::modifier(int idEmploye)
{

    QSqlQuery query;
    query.prepare("UPDATE adherent SET nom = ?, prenom = ?, email = ?, ville = ?, num_tel = ?, fidelite = ?, cin = ?, image_a = ? WHERE id_a = ?");
    query.addBindValue(this->nom);
    query.addBindValue(this->prenom);
    query.addBindValue(this->email);
    query.addBindValue(this->ville);
    query.addBindValue(this->num_tel);
    query.addBindValue(this->fidelite);
    query.addBindValue(this->cin);
    query.addBindValue(this->image_a);
    query.addBindValue(this->id_a);
    //*****************************************************************************************************************************
                // Récupérer l'ID de l'employé
               // int idEmploye = ui->label_id_employe_join->text().toInt();

                // Exécuter une requête SQL pour récupérer le nom de l'employé correspondant à l'ID
                QSqlQuery queryE;
                QString nomEmploye;
                queryE.prepare("SELECT NOM FROM employe WHERE ID = :id");
                queryE.bindValue(":id", idEmploye);
                if (queryE.exec() && queryE.next()) {
                    // Si la requête s'exécute avec succès et qu'elle retourne un résultat
                    // Récupérer le nom de l'employé
                     nomEmploye = queryE.value(0).toString();

                    // Afficher le nom de l'employé dans label_id_e
                    //*****************/ui->label_id_e->setText(nomEmploye);
                } else {
                    // Gérer les erreurs ou l'absence de résultat
                    // Par exemple, afficher un message d'erreur
                    qDebug() << "Erreur lors de la récupération du nom de l'employé";
                }

    //*****************************************************************************************************************************

    historique(2, this->nom + " " + this->prenom,"",nomEmploye);

    return query.exec();
}


QSqlQueryModel* Adherent::chercherParPrenom(const QString& prenom) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CAST(id_a AS VARCHAR(255)), nom, prenom, email, ville, CAST(num_tel AS VARCHAR(255)), fidelite, CAST(cin AS VARCHAR(255)), image_a FROM adherent WHERE LOWER(prenom) LIKE LOWER(:prenom)");
    query.bindValue(":prenom", "%" + prenom.toLower() + "%");
    if (query.exec()) {
        model->setQuery(query);
        // Définir les en-têtes
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Fidélité"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("image_a"));

        return model;
    } else {
        delete model; // Clear memory if an error occurs
        return nullptr;
    }
}


QSqlQueryModel* Adherent::chercherParId(const QString& id_a) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT CAST(id_a AS VARCHAR(255)), nom, prenom, email, ville, CAST(num_tel AS VARCHAR(255)), fidelite, CAST(cin AS VARCHAR(255)), image_a FROM adherent WHERE id_a=:id_a");
    query.bindValue(":id_a",id_a);
    if (query.exec()) {
        model->setQuery(query);
        // Définir les en-têtes
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Fidélité"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Image"));

        return model;
    } else {
        delete model; // Clear memory if an error occurs
        return nullptr;
    }
}




QSqlQueryModel* Adherent::trie(QString croissance, QString critere, int idEmploye)
{
    QSqlQueryModel* modal = new QSqlQueryModel();
    QSqlQuery query;

    QString sqlQuery = "SELECT CAST(id_a AS VARCHAR(255)), nom, prenom, email, ville, CAST(num_tel AS VARCHAR(255)), fidelite, CAST(cin AS VARCHAR(255)), image_a FROM adherent ORDER BY %1 %2";
    QString type;

    sqlQuery = sqlQuery.arg(critere).arg(croissance);
    type=critere;
    if(critere=="id_a"){type="ID";}
    if(critere=="fidelite"){type="fidélité";}
    if(critere=="num_tel"){type="numéro de téléphone";}
    if(critere=="cin"){type="CIN";}
    //*****************************************************************************************************************************
                // Récupérer l'ID de l'employé
               // int idEmploye = ui->label_id_employe_join->text().toInt();

                // Exécuter une requête SQL pour récupérer le nom de l'employé correspondant à l'ID
                QSqlQuery queryE;
                QString nomEmploye;
                queryE.prepare("SELECT NOM FROM employe WHERE ID = :id");
                queryE.bindValue(":id", idEmploye);
                if (queryE.exec() && queryE.next()) {
                    // Si la requête s'exécute avec succès et qu'elle retourne un résultat
                    // Récupérer le nom de l'employé
                     nomEmploye = queryE.value(0).toString();

                    // Afficher le nom de l'employé dans label_id_e
                    //*****************/ui->label_id_e->setText(nomEmploye);
                } else {
                    // Gérer les erreurs ou l'absence de résultat
                    // Par exemple, afficher un message d'erreur
                    qDebug() << "Erreur lors de la récupération du nom de l'employé";
                }

    //*****************************************************************************************************************************

    if(croissance=="ASC"){historique(5,"",type,nomEmploye);}
    if(croissance=="DESC"){historique(6,"",type,nomEmploye);}

    query.prepare(sqlQuery);
    if (query.exec()) {
        modal->setQuery(query);
        modal->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        modal->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
        modal->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        modal->setHeaderData(4, Qt::Horizontal, QObject::tr("Adresse"));
        modal->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
        modal->setHeaderData(6, Qt::Horizontal, QObject::tr("Fidélité"));
        modal->setHeaderData(7, Qt::Horizontal, QObject::tr("CIN"));
        modal->setHeaderData(8, Qt::Horizontal, QObject::tr("Image"));

    }

    return modal;
}

QHash<QString, int> Adherent::statistiques(int idEmploye)
{
    QHash<QString, int> statistiques;

    // Récupérez la liste des élèves depuis votre base de données
    // Par exemple, vous pouvez utiliser votre fonction afficher() pour obtenir une liste de tous les clients.
    QSqlQueryModel* model = afficher();

    // Calculez l'âge de chaque élève et mettez à jour la répartition par âge
    QDateTime now = QDateTime::currentDateTime();
    for (int row = 0; row < model->rowCount(); ++row) {
        int fidelite = model->data(model->index(row, 6)).toInt(); // Supposons que la date de naissance est dans la colonne 3

        // Classifiez l'âge dans des groupes
        QString qteGroup;
        if (fidelite < 2) {
            qteGroup = "silver";
        } else if (fidelite < 4 && fidelite >=2) {
            qteGroup = "premiuem";
        } else if (fidelite < 6 && fidelite >=4) {
            qteGroup = "gold";
        } else {
            qteGroup = "VIP";
        }

        // Mettez à jour la répartition par âge
        if (statistiques.contains(qteGroup)) {
            statistiques[qteGroup]++;
        } else {
            statistiques[qteGroup] = 1;
        }
    }
    //*****************************************************************************************************************************
                // Récupérer l'ID de l'employé
               // int idEmploye = ui->label_id_employe_join->text().toInt();

                // Exécuter une requête SQL pour récupérer le nom de l'employé correspondant à l'ID
                QSqlQuery queryE;
                QString nomEmploye;
                queryE.prepare("SELECT NOM FROM employe WHERE ID = :id");
                queryE.bindValue(":id", idEmploye);
                if (queryE.exec() && queryE.next()) {
                    // Si la requête s'exécute avec succès et qu'elle retourne un résultat
                    // Récupérer le nom de l'employé
                     nomEmploye = queryE.value(0).toString();

                    // Afficher le nom de l'employé dans label_id_e
                    //*****************/ui->label_id_e->setText(nomEmploye);
                } else {
                    // Gérer les erreurs ou l'absence de résultat
                    // Par exemple, afficher un message d'erreur
                    qDebug() << "Erreur lors de la récupération du nom de l'employé";
                }

    //*****************************************************************************************************************************

    historique(4,"","",nomEmploye);
      return statistiques;
  }

bool Adherent::idExiste(QString id_a) {
    QSqlQuery query;
    query.prepare("SELECT id_a FROM Adherent WHERE id_a = :id_a");
    query.bindValue(":id_a", id_a);

    if (query.exec() && query.next())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Adherent Adherent::getAdherentDetails(int cin, int idEmploye)
{


       // Exécuter la requête SQL pour récupérer les détails du constat
       QSqlQuery query;
       query.prepare("SELECT id_a, nom, prenom, email, ville, num_tel, fidelite, cin, image_a FROM adherent WHERE cin = :cin");
       query.bindValue(":cin", cin);
       if (!query.exec()) {
           //qDebug() << "Erreur lors de l'exécution de la requête SQL :" << query.lastError().text();
           qDebug() << "Erreur lors de l'exécution de la requête SQL :" ;
           return Adherent(); // Retourner un objet Constat vide en cas d'erreur de requête
       }

       // Récupérer les informations de la requête et les utiliser pour initialiser un objet Constat
       Adherent adherent;
       if (query.next()) {
           adherent.Setcin(cin);
           adherent.Setnom(query.value("nom").toString());
           adherent.Setprenom(query.value("prenom").toString());
           adherent.Setemail(query.value("email").toString());
           adherent.Setville(query.value("ville").toString());

           adherent.Setnum_tel(query.value("num_tel").toInt());
           adherent.Setid_a(query.value("id_a").toInt());
           adherent.Setfidelite(query.value("fidelite").toInt());
           adherent.Setimage_a(query.value("image_a").toString());


       }
       //*****************************************************************************************************************************
                   // Récupérer l'ID de l'employé
                  // int idEmploye = ui->label_id_employe_join->text().toInt();

                   // Exécuter une requête SQL pour récupérer le nom de l'employé correspondant à l'ID
                   QSqlQuery queryE;
                   QString nomEmploye;
                   queryE.prepare("SELECT NOM FROM employe WHERE ID = :id");
                   queryE.bindValue(":id", idEmploye);
                   if (queryE.exec() && queryE.next()) {
                       // Si la requête s'exécute avec succès et qu'elle retourne un résultat
                       // Récupérer le nom de l'employé
                        nomEmploye = queryE.value(0).toString();

                       // Afficher le nom de l'employé dans label_id_e
                       //*****************/ui->label_id_e->setText(nomEmploye);
                   } else {
                       // Gérer les erreurs ou l'absence de résultat
                       // Par exemple, afficher un message d'erreur
                       qDebug() << "Erreur lors de la récupération du nom de l'employé";
                   }

       //*****************************************************************************************************************************


       historique(7, adherent.Getnom() + " " + adherent.Getprenom(),"",nomEmploye);
       return adherent;
}
QList<Adherent> Adherent::getAdherentList(int idEmploye)
{
    QList<Adherent> adherentList;

    // Exécuter la requête SQL pour récupérer tous les adhérents
    QSqlQuery query("SELECT id_a, nom, prenom, email, ville, num_tel, fidelite, cin, image_a FROM adherent");
    while (query.next()) {
        Adherent adherent;
        adherent.Setid_a(query.value("id_a").toInt());
        adherent.Setnom(query.value("nom").toString());
        adherent.Setprenom(query.value("prenom").toString());
        adherent.Setemail(query.value("email").toString());
        adherent.Setville(query.value("ville").toString());
        adherent.Setnum_tel(query.value("num_tel").toInt());
        adherent.Setfidelite(query.value("fidelite").toInt());
        adherent.Setcin(query.value("cin").toInt());
        adherent.Setimage_a(query.value("image_a").toString());

        adherentList.append(adherent);
    }
    //*****************************************************************************************************************************
                // Récupérer l'ID de l'employé
               // int idEmploye = ui->label_id_employe_join->text().toInt();

                // Exécuter une requête SQL pour récupérer le nom de l'employé correspondant à l'ID
                QSqlQuery queryE;
                QString nomEmploye;
                queryE.prepare("SELECT NOM FROM employe WHERE ID = :id");
                queryE.bindValue(":id", idEmploye);
                if (queryE.exec() && queryE.next()) {
                    // Si la requête s'exécute avec succès et qu'elle retourne un résultat
                    // Récupérer le nom de l'employé
                     nomEmploye = queryE.value(0).toString();

                    // Afficher le nom de l'employé dans label_id_e
                    //*****************/ui->label_id_e->setText(nomEmploye);
                } else {
                    // Gérer les erreurs ou l'absence de résultat
                    // Par exemple, afficher un message d'erreur
                    qDebug() << "Erreur lors de la récupération du nom de l'employé";
                }

    //*****************************************************************************************************************************

    historique(8,"","", nomEmploye);
    return adherentList;
}

void Adherent::historique(int a, QString name, QString type, QString nomEmplye)
{
    QString text;
    if(a == 1)
    {
        text = QString(" L'adhérent '%1' a été ajouté par %2 \n ").arg(name).arg(nomEmplye);
    }
    else if(a == 2)
    {
        text = QString(" L'adhérent '%1' a été modifié par %2 \n ").arg(name).arg(nomEmplye);
    }
    else if(a == 3)
    {
        text = QString(" L'adhérent '%1' a été supprimé par %2 \n ").arg(name).arg(nomEmplye);
    }
    else if(a == 4)
    {
        text = QString(" La charte graphique des statistique a été affichée par %1 \n ").arg(nomEmplye);
    }
    else if(a == 5)
    {
        text = QString(" Les adhérents ont été triés selon %1 dans l'ordre croissant à la demande de %2 \n ").arg(type).arg(nomEmplye);
    }
    else if(a == 6)
    {
        text = QString(" Les adhérents ont été triés selon %1 dans l'ordre décroissant à la demande de %2 \n ").arg(type).arg(nomEmplye);
    }
    else if(a == 7 && name!="")
    {
        text = QString(" Le PDF de l'adhérent '%1' a été exporté à la demande de %2\n ").arg(name).arg(nomEmplye);
    }
    else if(a == 8)
    {
        text = QString(" Le PDF de la liste des adhérent a été exporté à la demande de %1\n ").arg(nomEmplye);
    }

    QFile file("his.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Erreur lors de l'ouverture du fichier en lecture :" << file.errorString();
        return;
    }

    QString oldContent = QTextStream(&file).readAll();
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Erreur lors de l'ouverture du fichier en écriture :" << file.errorString();
        return;
    }
    QDateTime dateTime = QDateTime::currentDateTime();
    QString formattedDateTime = dateTime.toString("dd-MM-yyyy hh:mm:ss");

    QTextStream out(&file);
    out << text+""<< formattedDateTime << "\n\n" << oldContent;
    file.close();
}
