#include "dialogpartenaire.h"
#include "ui_dialogpartenaire.h"
#include "dialoglogin.h"
#include "mainwindow.h"
#include <QTextDocument>
#include <QPrinter>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
DialogPartenaire::DialogPartenaire(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPartenaire)
{
    ui->setupUi(this);
    ui->tablepartenariat->setModel(Ptmp.afficher());

}

DialogPartenaire::~DialogPartenaire()
{
    delete ui;
}

void DialogPartenaire::on_pushButton_retour_Partenaire_clicked()
{
    int nombreEntier = ui->label_id_employe->text().toInt(); // Convertir en entier directement
    MainWindow *M = new MainWindow();
    M->setResultRP(nombreEntier); // Appeler une fonction de la deuxième interface pour définir le résultat
    M->show();
    //this->close();
    hide();

}
void DialogPartenaire::setResultP(int result)
{
    ui->label_id_employe->setText(QString::number(result)); // Afficher le résultat dans le Label
}


void DialogPartenaire::on_pushButton_logout_Partenaire_clicked()
{
    DialogLogin *dLogin = new DialogLogin();
    dLogin->show();
    hide();

}

void DialogPartenaire::on_pushButton_ajouter_clicked()
{
        int id_p = ui->lineEdit_id_p->text().toInt();
        QString nom = ui->lineEdit_nom->text();
        QString contact = ui->lineEdit_contact->text();
        QString avantage = ui->lineEdit_avantage->text();
        QDate date_debut = ui->dateEdit_date_debut->date();
        QDate date_fin = ui->dateEdit_date_fin->date();

        Partenaire P ( id_p, nom, contact, avantage, date_debut, date_fin);


 /*                      //controle de saisir

       if (ui->lineEdit_numero->text().length() != 8) {
              QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                                    QObject::tr("Le numéro doit contenir exactement 8 chiffres."), QMessageBox::Cancel);
              return;
          }
       if (adresse.isEmpty() || description.isEmpty() || degat.isEmpty()) {
               QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                                     QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
               return;
        }
*/


       bool test=P.ajouter();




       if(test)
       {
           QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Ajouter effectué\n"
                                                                           "Click Cancel to exit."),QMessageBox::Cancel);

           ui->tablepartenariat->setModel(Ptmp.afficher());



       }
       else
           QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                 QObject::tr("Ajout non effectué.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);

       //ui->tableView->setModel(Cons.afficher());


}

void DialogPartenaire::on_pushButton_supprimer_clicked()
{
    QString numeroString = ui->lineEdit_id_p_supp->text();

    if (numeroString.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                              QObject::tr("Veuillez entrer un numéro de constat."),
                              QMessageBox::Cancel);
        return;
    }

    int num = numeroString.toInt();

    if (!Ptmp.numeroExiste(num)) {
        QMessageBox::critical(nullptr, QObject::tr("Partenaire non trouvé"),
                              QObject::tr("ID spécifié n'existe pas."),
                              QMessageBox::Cancel);
        ui->lineEdit_id_p_supp->clear();
        return;
    }

    bool test = Ptmp.supprimer(num);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
        ui->lineEdit_id_p_supp->clear();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);

    }
    ui->tablepartenariat->setModel(Ptmp.afficher());


}

void DialogPartenaire::on_pushButton_ok_clicked()
{
    int id_p = ui->lineEdit_id_pm->text().toInt();
    QString numeroString = ui->lineEdit_id_pm->text();
    const Partenaire& P = Ptmp.rechercher(id_p);
    if (numeroString.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                              QObject::tr("Veuillez entrer un numéro de partenaire."),
                              QMessageBox::Cancel);
        return;
    }

    if (!Ptmp.numeroExiste(id_p)) {
        QMessageBox::critical(nullptr, QObject::tr("Partenaire non trouvé"),
                              QObject::tr("Le numéro de partenaire spécifié n'existe pas."),
                              QMessageBox::Cancel);
        return;
    }

    if (P.Getid_p() != -1) {

        ui->lineEdit_id_p_2->setText(QString::number(P.Getid_p()));
        ui->lineEdit_nom_2->setText(P.Getnom());
        ui->lineEdit_contact_2->setText(P.Getcontact());
        ui->lineEdit_avantage_2->setText(P.Getavantage());
        ui->dateEdit_date_debut_2->setDate(P.Getdate_debut());
        ui->dateEdit_date_fin_2->setDate(P.Getdate_fin());

    } else {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Aucun Partenaire trouvé avec ce numéro."), QMessageBox::Ok);
    }

}

void DialogPartenaire::on_pushButton_modifier_clicked()
{
    int id_p = ui->lineEdit_id_pm->text().toInt();
    const Partenaire& P = Ptmp.rechercher(id_p);

    if (P.Getid_p() != -1) {
        Partenaire copie_partenaire = P;
        int nouveau_id_p = ui->lineEdit_id_p_2->text().toInt();
        QString nouveau_nom = ui->lineEdit_nom_2->text();
        QString nouveau_contact = ui->lineEdit_contact_2->text();
        QString nouveau_avantage = ui->lineEdit_avantage_2->text();
        QDate nouvelle_date_debut = ui->dateEdit_date_debut_2->date();
        QDate nouvelle_date_fin = ui->dateEdit_date_fin_2->date();

        copie_partenaire.Setid_p(nouveau_id_p);
        copie_partenaire.Setnom(nouveau_nom);
        copie_partenaire.Setcontact(nouveau_contact);
        copie_partenaire.Setavantage(nouveau_avantage);
        copie_partenaire.Setdate_debut(nouvelle_date_debut);
        copie_partenaire.Setdate_fin(nouvelle_date_fin);
        if (nouveau_nom.isEmpty() || nouveau_contact.isEmpty() || nouveau_avantage.isEmpty()) {
                QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                                      QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
                return;
         }


        bool modification_reussie = copie_partenaire.modifier();
        if (modification_reussie) {
            QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Modification effectuée\n"
                                                                             "Click Cancel to exit."), QMessageBox::Cancel);

            ui->tablepartenariat->setModel(Ptmp.afficher());
            ui->lineEdit_id_p_2->clear();
            ui->lineEdit_nom_2->clear();
            ui->lineEdit_contact_2->clear();
            ui->lineEdit_avantage_2->clear();
            ui->dateEdit_date_debut_2->clear();
            ui->dateEdit_date_fin_2->clear();
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Modification non effectuée.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Aucun constat trouvé avec ce numéro."), QMessageBox::Ok);

    }

}


void DialogPartenaire::on_trier_clicked()
{
    Partenaire P;
QString tri=ui->triebox_2->currentText();

if(tri=="date_debut" && ui->asc_2->isChecked() )
{
    QSqlQueryModel* sortedModel =P.tri_date_de_debut_asc();

      // Assuming you have a member variable `tableView` of type QTableView in the MainWindow class
     ui->tablepartenariat->setModel(sortedModel);


}
if(tri=="date_debut" && ui->deso_2->isChecked() )
{
    QSqlQueryModel* sortedModel =P.tri_date_de_debut_DESC();

      // Assuming you have a member variable `tableView` of type QTableView in the MainWindow class
     ui->tablepartenariat->setModel(sortedModel);


}
if(tri=="date_fin" && ui->deso_2->isChecked() )
{
    QSqlQueryModel* sortedModel =P.tri_date_de_fin_DESC();

      // Assuming you have a member variable `tableView` of type QTableView in the MainWindow class
     ui->tablepartenariat->setModel(sortedModel);


}
if(tri=="date_fin" && ui->asc_2->isChecked() )
{
    QSqlQueryModel* sortedModel =P.tri_date_de_fin_asc();

      // Assuming you have a member variable `tableView` of type QTableView in the MainWindow class
     ui->tablepartenariat->setModel(sortedModel);


}


}



void DialogPartenaire::on_lineEdit_45_textChanged(const QString &arg1)
{


            QSqlQueryModel* searchModel = new QSqlQueryModel();
            searchModel->setQuery("SELECT * FROM partenaire WHERE ID_P LIKE '%" + arg1 + "%'");

            if (searchModel->rowCount() > 0) {
                // If the search result is not empty
                ui->tablepartenariat->setModel(searchModel);
            }
            if (searchModel->rowCount() == 0) {
                // If the search result is not empty
                ui->tablepartenariat->setModel(searchModel);
            }
                // Adjust the header view as needed
                QHeaderView* headerView = ui->tablepartenariat->horizontalHeader();
                headerView->setSectionResizeMode(QHeaderView::Stretch);



}

void DialogPartenaire::on_pdf_2_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tablepartenariat->model()->rowCount();
    const int columnCount = ui->tablepartenariat->model()->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           "<title>%1</title>\n"
           "<style>\n"
           "table {\n"
           "    width: 100%;\n"
           "    border-collapse: collapse;\n"
           "}\n"
           "th, td {\n"
           "    padding: 8px;\n"
           "    text-align: left;\n"
           "    border-bottom: 1px solid #ddd;\n"
           "}\n"
           "tr:nth-child(even) {\n"
           "    background-color: #f2f2f2;\n"
           "}\n"
           "</style>\n"
           "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center> <H1>Liste des Partenaires</H1></center><br/><br/>\n"
           "<table>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    {
        if (!ui->tablepartenariat->isColumnHidden(column))
        {
            out << QString("<th>%1</th>").arg(ui->tablepartenariat->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr> <td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; column++)
        {
            if (!ui->tablepartenariat->isColumnHidden(column))
            {
                QString data = ui->tablepartenariat->model()->data(ui->tablepartenariat->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }



    QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);
}
