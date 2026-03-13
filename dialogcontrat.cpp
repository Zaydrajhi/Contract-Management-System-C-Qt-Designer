#include "dialogcontrat.h"
#include "ui_dialogcontrat.h"
#include <QDate>
#include "contrat.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "qrcodegen.hpp"
#include <QtSql/QSqlRecord> // For QSqlRecord
#include <Windows.h>
#include <QProcess>
#include "sms.h"
#include<QSslSocket>
#include "qrcodegen.hpp"
#include <QtSql/QSqlRecord> // For QSqlRecord
#include <Windows.h>
#include <QProcess>
#include <QTextDocument>
#include <QPdfWriter>
#include <QDir>
#include <QFileDialog>
#include <QPainter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

DialogContrat::DialogContrat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogContrat)
{
    ui->setupUi(this);
    ui->tableView_4->setModel(Con.afficher());
}

DialogContrat::~DialogContrat()
{
    delete ui;
}
void DialogContrat::showNotification(const QString& title, const QString& message) {
    QString command = "powershell.exe -Command \""
                      "Add-Type -AssemblyName System.Windows.Forms;"
                      "Add-Type -AssemblyName System.Drawing;"
                      "$notification = New-Object System.Windows.Forms.NotifyIcon;"
                      "$notification.Icon = [System.Drawing.SystemIcons]::Information;"
                      "$notification.Visible = $true;"
                      "$notification.ShowBalloonTip(5000, '%1', '%2', [System.Windows.Forms.ToolTipIcon]::Info);"
                      "\"";
    QProcess::startDetached(command.arg(title).arg(message));
}
void DialogContrat::on_pushButton_4_clicked()
{
    int numero = ui->lineEdit_32->text().toInt();
    int id_a = ui->lineEdit_36->text().toInt();
    QDate date_debut = ui->dateEdit_4->date();
    QDate date_fin = ui->dateEdit_5->date();

    QString type = ui->comboBox_3->currentText();

    QString remise = ui->lineEdit_33->text();

    // Retrieve the phone number from the "phone" label

    // Verify if the phone number length is less than 8

QString phoneNumber = "46092023";
    // Add the country code at the beginning of the phone number
    phoneNumber.prepend("+216");

    Contrat Con(numero, date_debut, date_fin, type, remise, id_a);
     bool test = Con.ajouter();

    if (test) {
        showNotification("Success", "contrat added successfully.");

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Ajouter effectué\n"
                                                                           "Click Cancel to exit."), QMessageBox::Cancel);
        ui->lineEdit_32->clear();
        ui->lineEdit_36->clear();
        ui->dateEdit_4->clear();
        ui->dateEdit_5->clear();


        ui->lineEdit_33->clear();
    } else {
        showNotification("Error", "Failed to add contrat.");

        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
    ui->tableView_4->setModel(Con.afficher());

    // Send SMS with the data
    sms s;
    QString message = "Your contract has been added successfully.";
   // phoneNumber = "46092023";
    s.sendSMS(phoneNumber, message);
}

QSqlQueryModel * Contrat::afficher()
{
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select numero,id_a,date_debut,date_fin,type,remise from contrat");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("numero"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("ID Adhérent"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("date_debut"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("date_fin"));

    model->setHeaderData(4,Qt::Horizontal,QObject::tr("type"));

    model->setHeaderData(5,Qt::Horizontal,QObject::tr("remise"));

    return model;
}

void DialogContrat::on_pushButton_41_clicked()
{

    {
        // Get the contract number from the line edit
        int numero = ui->lineEdit_39->text().toInt();

        // Search for the contract using the rechercher function
        Contrat contratToDelete = Con.rechercher(numero);

        // Check if the contract was found
        if (contratToDelete.getnum() != -1) {
            // Open the file for appending
            QFile file("contract_backup.txt");
            if (file.open(QIODevice::Append | QIODevice::Text)) {
                // Write the contract data to the file
                QTextStream out(&file);
                out << "Contract Number: " << contratToDelete.getnum() << "\n";

                out << "Type: " << contratToDelete.gettype() << "\n";
                out << "Remise: " << contratToDelete.getremise() << "\n";
                out << "Date Debut: " << contratToDelete.getdated().toString("yyyy-MM-dd") << "\n";
                out << "Date Fin: " << contratToDelete.getdatef().toString("yyyy-MM-dd") << "\n";
                out << "ID Adhérent: " << contratToDelete.getid_a() << "\n";
                file.close();
            }

            // Proceed with contract deletion
            bool test = Con.supprimer(numero);
            if (test) {
                showNotification("Success", "contrat deleted successfully.");

                QMessageBox::information(nullptr, QObject::tr("OK"),
                                         QObject::tr("Suppression effectuée\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            } else {
                showNotification("Error", "Failed to delete employee.");

                QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                      QObject::tr("Suppression non effectuée.\n"
                                                  "Click Cancel to exit."), QMessageBox::Cancel);
            }
        } else {    showNotification("Error", "Failed to delete employee.");
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("Contrat non trouvé.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
        }
    }


}
















void DialogContrat::on_pushButton_50_clicked()
{
    int numero = ui->lineEdit_34->text().toInt();
    int id_a = ui->lineEdit_48->text().toInt();
    QDate date_debut = ui->dateEdit_8->date();
    QDate date_fin = ui->dateEdit_9->date();

    QString type = ui->comboBox_5->currentText();

    QString remise = ui->lineEdit_50->text();

    // Verify if the phone number length is less than 8


    // Create Contrat object with updated phone number
    Contrat ContratToUpdate(numero, date_debut, date_fin, type, remise, id_a);
    bool test = ContratToUpdate.modifier();
    QMessageBox msgBox;

    if (test) {
        showNotification("Success", "contrat modified successfully.");

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("modification effectué\n"
                                                                         "Click Cancel to exit."), QMessageBox::Cancel);

        ui->tableView_4->setModel(ContratToUpdate.afficher());
        ui->lineEdit_34->clear();
        ui->lineEdit_48->clear();
        ui->dateEdit_8->clear();
        ui->dateEdit_9->clear();

        ui->lineEdit_50->clear();
    } else {
        showNotification("Error", "Failed to modify contrat.");

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("modification failed\n"
                                                                         "Click Cancel to exit."), QMessageBox::Cancel);
    }
}


void DialogContrat::on_pushButton_39_clicked()
{
    int num = ui->lineEdit_39->text().toInt();
        QString numeroString = ui->lineEdit_39->text();
        const Contrat& C = Con.rechercher(num);
        if (numeroString.isEmpty()) {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Veuillez entrer un numéro de contrat."),
                                  QMessageBox::Cancel);
            return;
        }



        if (C.getnum() != -1) {
            ui->lineEdit_34->setText(QString::number(C.getnum()));

            ui->lineEdit_48->setText(QString::number(C.getid_a()));
            ui->comboBox_5->setCurrentText(C.gettype());
            ui->dateEdit_8->setDate(C.getdated());
            ui->dateEdit_9->setDate(C.getdatef());
            ui->lineEdit_50->setText(C.getremise());
        } else {
            QMessageBox::warning(nullptr, QObject::tr("Attention"),
                                 QObject::tr("Aucun constat trouvé avec ce numéro."), QMessageBox::Ok);
        }
}

void DialogContrat::on_asc_clicked()
{

}

void DialogContrat::on_pushButton_40_clicked()
{
    if (ui->asc->isChecked()) // Check if radioButton_5 is clicked
    {
        if (ui->triebox->currentText() != "Choisir")
            ui->tableView_4->setModel(Con.trie("ASC", ui->triebox->currentText()));
        else
            ui->tableView_4->setModel(Con.afficher());
    }
    else if (ui->deso->isChecked()) // Check if radioButton_6 is clicked
    {
        if (ui->triebox->currentText() != "Choisir")
            ui->tableView_4->setModel(Con.trie("DESC", ui->triebox->currentText()));
        else
            ui->tableView_4->setModel(Con.afficher());
    }
}

void DialogContrat::on_pdf_clicked()
{
    Con.on_pdf_clicked();

}

void DialogContrat::on_tableView_4_activated(const QModelIndex &index)
{ using namespace qrcodegen;

    QString val = ui->tableView_4->model()->data(index).toString();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM contrat WHERE id_a=:id_a");
    qry.bindValue(":id_a", val);

    if (qry.exec()) {
        while (qry.next()) {
            // Extracting data from the query result
            int numero = qry.value("numero").toInt();
            int id_a = qry.value("id_a").toInt();

            QString date_debut = qry.value("date_debut").toString();
            QString date_fin = qry.value("date_fin").toString();
            QString type = qry.value("type").toString();
            QString remise = qry.value("remise").toString();

            // Constructing the text for QR code
            QString text = "Numero: " + QString::number(numero) + "\n"
                           "ID Adhérent: " + QString::number(id_a) + "\n"

                           "Date Debut: " + date_debut + "\n"
                           "Date Fin: " + date_fin + "\n"
                           "Type: " + type + "\n"
                           "Remise: " + remise + "\n";

            // Using QrCodegen to create a QR code from patient attributes
            QrCode qr = QrCode::encodeText(text.toUtf8().constData(), QrCode::Ecc::MEDIUM);

            qint32 sz = qr.getSize();
            QImage im(sz, sz, QImage::Format_RGB32);
            QRgb black = qRgb(0, 0, 0);
            QRgb white = qRgb(255, 255, 255);

            for (int y = 0; y < sz; y++) {
                for (int x = 0; x < sz; x++) {
                    im.setPixel(x, y, qr.getModule(x, y) ? black : white);
                }
            }
            ui->qrcode->setPixmap(QPixmap::fromImage(im.scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation)));
        }
    } else {
     }

}

void DialogContrat::on_lineEdit_40_textChanged(const QString &arg1)
{

}

void DialogContrat::on_lineEdit_40_cursorPositionChanged(int arg1, int arg2)
{

    int numero = ui->lineEdit_40->text().toInt();

       QSqlQuery query = Con.recherchernum(numero);

       if (query.exec() && query.next())
       {
           QStandardItemModel *model = new QStandardItemModel();

           model->setColumnCount(query.record().count());
           ui->tableView_4->setModel(model);

           model->removeRows(0, model->rowCount());

           for (int i = 0; i < query.record().count(); i++)
           {
               model->setHorizontalHeaderItem(i, new QStandardItem(query.record().fieldName(i)));
           }

           do
           {
               QList<QStandardItem*> rowItems;
               for (int col = 0; col < query.record().count(); col++)
               {
                   rowItems << new QStandardItem(query.value(col).toString());
               }
               model->appendRow(rowItems);
           } while (query.next());
       }
       else
       {
           qDebug() << "Query failed or returned no results.";
       }
       qDebug() << "Query executed.";
}

void DialogContrat::on_lineEdit_41_cursorPositionChanged(int arg1, int arg2)
{

}

void DialogContrat::on_lineEdit_41_textChanged(const QString &arg1)
{
    QString name = ui->lineEdit_41->text();

       QSqlQuery query = Con.recherchername(name);

       if (query.exec() && query.next())
       {
           QStandardItemModel *model = new QStandardItemModel();

           model->setColumnCount(query.record().count());
           ui->tableView_4->setModel(model);

           model->removeRows(0, model->rowCount());

           for (int i = 0; i < query.record().count(); i++)
           {
               model->setHorizontalHeaderItem(i, new QStandardItem(query.record().fieldName(i)));
           }

           do
           {
               QList<QStandardItem*> rowItems;
               for (int col = 0; col < query.record().count(); col++)
               {
                   rowItems << new QStandardItem(query.value(col).toString());
               }
               model->appendRow(rowItems);
           } while (query.next());
       }
       else
       {
           qDebug() << "Query failed or returned no results.";
       }
       qDebug() << "Query executed.";
}

void DialogContrat::on_lineEdit_42_textChanged(const QString &arg1)
{
    //lineEdit_42
    int id_a = ui->lineEdit_42->text().toInt();

       QSqlQuery query = Con.rechercherncin(id_a);

       if (query.exec() && query.next())
       {
           QStandardItemModel *model = new QStandardItemModel();

           model->setColumnCount(query.record().count());
           ui->tableView_4->setModel(model);

           model->removeRows(0, model->rowCount());

           for (int i = 0; i < query.record().count(); i++)
           {
               model->setHorizontalHeaderItem(i, new QStandardItem(query.record().fieldName(i)));
           }

           do
           {
               QList<QStandardItem*> rowItems;
               for (int col = 0; col < query.record().count(); col++)
               {
                   rowItems << new QStandardItem(query.value(col).toString());
               }
               model->appendRow(rowItems);
           } while (query.next());
       }
       else
       {
           qDebug() << "Query failed or returned no results.";
       }
       qDebug() << "Query executed.";
}
void DialogContrat::on_pushButton_38_clicked()
{


            QtCharts::QPieSeries *series = new QtCharts::QPieSeries();


            QHash<QString, int> Distribution = Contrat::statistiques();

            int total = 0;
            for (const QString &Group : Distribution.keys()) {
                total += Distribution.value(Group);
            }

            for (const QString &Group : Distribution.keys()) {
                int value = Distribution.value(Group);
                double percentage = (value * 100.0) / total;
                QString label = QString("%1 (%2%)").arg(Group).arg(percentage, 0, 'f', 1);
                // Ajouter la tranche (slice) avec le pourcentage comme étiquette
                series->append(label, value);
                // Vous pouvez personnaliser l'apparence de chaque tranche si vous le souhaitez
            }

            // Créer un graphique circulaire et configurer-le
            QtCharts::QChart *chart = new QtCharts::QChart();
            chart->addSeries(series);
            chart->setTitle("Répartition par Type");

            // Afficher les pourcentages directement sur les tranches
            series->setLabelsVisible();

            // Créer une vue de graphique pour afficher le graphique circulaire
            QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            // Afficher la vue du graphique dans une nouvelle fenêtre
            QMainWindow *chartWindow = new QMainWindow(this);
            chartWindow->setCentralWidget(chartView);
            chartWindow->resize(800, 600);
            chartWindow->show();
    }






#include "mainwindow.h"
#include "dialoglogin.h"
void DialogContrat::on_pushButton_retour_clicked()
{
    int nombreEntier = ui->label_id_employe->text().toInt(); // Convertir en entier directement
    MainWindow *M = new MainWindow();
    M->setResultRCont(nombreEntier); // Appeler une fonction de la deuxième interface pour définir le résultat
    M->show();
    //this->close();
    hide();

}

void DialogContrat::on_pushButton_logout_Adherent_clicked()
{
    DialogLogin *dLogin = new DialogLogin();
    dLogin->show();
    hide();

}
void DialogContrat::setResultCont(int result)
{
    ui->label_id_employe->setText(QString::number(result)); // Afficher le résultat dans le Label
}
