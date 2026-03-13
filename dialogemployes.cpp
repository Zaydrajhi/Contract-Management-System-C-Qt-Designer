#include "dialogemployes.h"
#include "ui_dialogemployes.h"
#include "employe.h"
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDate>
#include <QValidator>
#include <QSqlError>
#include <QDoubleValidator>
#include <QSortFilterProxyModel>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QDir>
//#include <QtCharts/QPieSeries>
//#include <QtCharts/QPieSlice>
//#include <QtCharts/QChartView>
//#include <QtCharts/QChart>
#include <QtCharts>
#include<QChartView>
#include<QPieSlice>
#include<QChart>
#include <QSqlQueryModel>
#include <QDialog>
#include <QSqlRecord>
#include <QPrinter>
DialogEmployes::DialogEmployes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEmployes)
{
    ui->setupUi(this);
    ui->tableView->setModel(emp.afficher());


}
/*
void DialogEmployes::update_label()
{
    data=A.read_from_arduino();

    if(data=="1"){}

        //ui->label_3->setText("ON"); // si les données reçues de arduino via la liaison série sont égales à 1
    // alors afficher ON

    else if (data=="0"){}

        //ui->label_3->setText("OFF");   // si les données reçues de arduino via la liaison série sont égales à 0
     //alors afficher ON
}
*/
DialogEmployes::~DialogEmployes()
{
    delete ui;
}

void DialogEmployes::on_pushButton_4_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString departement = ui->comboBox_departement->currentText();
    QString sexe = ui->comboBox_sexe->currentText();
    int salaire = ui->lineEdit_salaire->text().toInt();
    int num_tel = ui->lineEdit_telephone->text().toInt();
    QString adresse_email = ui->lineEdit_email->text();
    QString password = ui->lineEdit_password->text();
    int code = ui->lineEdit_code->text().toInt();
    Employe e(id, nom, prenom, departement, sexe, salaire, num_tel, adresse_email, password,code);
    if (nom.isEmpty() || adresse_email.isEmpty() || departement.isEmpty() || prenom.isEmpty() || password.isEmpty()) {
                   QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                                         QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
                   return;
            }
           if (sexe == "Choisir") {
                  QMessageBox::critical(nullptr, QObject::tr("Champ manquants"),
                                         QObject::tr("Veuillez choisir un sexe."),
                                         QMessageBox::Cancel);
                  return;
            }
    bool test = e.ajouter();
   if(test)
   {
       ui->tableView->setModel(emp.afficher());
       QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Ajouter effectué\n"
                                                                       "Click Cancel to exit."),QMessageBox::Cancel);
e.notification("votre nouveau employe a ete ajouter");
   }
   else {
       QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                             QObject::tr("Ajout non effectué.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);
   }

}

void DialogEmployes::on_pushButton_34_clicked()
{
    int id =ui->lineEdit_idsupp->text().toInt();
    bool test=emp.supprimer(id);
    if(test)
    {
        ui->tableView->setModel(emp.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Suppression effectué\n"
                                                                        "Click Cancel to exit."),QMessageBox::Cancel);

    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
}


void DialogEmployes::on_pushButton_41_clicked()
{

    int id = ui->lineEdit_idm->text().toInt();
    QString numeroString = ui->lineEdit_idm->text();
     const Employe& e = emp.rechercher(id);
    if (numeroString.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                              QObject::tr("Veuillez entrer un id d'employé."),
                              QMessageBox::Cancel);
        return;
    }

    Employe resultat = emp.rechercher(id);

    if (resultat.getid() == -1) {
        QMessageBox::critical(nullptr, QObject::tr("employé non trouvé"),
                              QObject::tr("L'id de l'employé spécifié n'existe pas."),
                              QMessageBox::Cancel);
        return;
    }

    if (e.getid() != -1) {
        ui->lineEdit_ID2->setText(QString::number(e.getid()));
        ui->lineEdit_nom1->setText(e.getnom());
        ui->lineEdit_prenom1->setText(e.getprenom());
        ui->comboBox_departement_1->setCurrentText(e.getdepartement());
        ui->comboBox_sexe1->setCurrentText(e.getsexe());
        ui->lineEdit_salaire1->setText(QString::number(e.getsalaire()));
        ui->lineEdit_telephone1->setText(QString::number(e.getnum_tel()));
        ui->lineEdit_email1->setText(e.getadresse_email());
        ui->lineEdit_password1->setText(e.getpassword());
        ui->lineEdit_code1->setText(QString::number(e.getcode()));
    } else {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Aucun employe trouvé avec cette id."), QMessageBox::Ok);
    }
}


void DialogEmployes::on_pushButton_39_clicked()
{


            int id = ui->lineEdit_ID2->text().toInt();
            QString nom = ui->lineEdit_nom1->text();
            QString prenom = ui->lineEdit_prenom1->text();
            QString departement = ui->comboBox_departement_1->currentText();
            QString sexe = ui->comboBox_sexe1->currentText();
            int salaire=ui->lineEdit_salaire1->text().toInt();
            int num_tel = ui->lineEdit_telephone1->text().toInt();
            QString adresse_email = ui->lineEdit_email1->text();
            QString password = ui->lineEdit_password1->text();
            int code = ui->lineEdit_code1->text().toInt();

             Employe EmployeToUpdate(id,nom,prenom,departement,sexe,salaire,num_tel,adresse_email,password,code);
                     bool test = EmployeToUpdate.modifier();
                     QMessageBox msgBox;

                     if (test)

                     {

                         QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("modification effectué\n"
                                                                                         "Click Cancel to exit."),QMessageBox::Cancel);

                                                ui->tableView->setModel(EmployeToUpdate.afficher());

                     }
                     else
                     {
                         QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("modification failed\n"
                                                                                         "Click Cancel to exit."),QMessageBox::Cancel);
                     }




}

void DialogEmployes::on_lineEdit_id3_textChanged(const QString &searchText)

{
    QSqlQueryModel* model = new QSqlQueryModel();

    if (model == nullptr) {
        qDebug() << "Failed to create QSqlQueryModel.";
        return;
    }

    QSqlQuery query;
    // Utilisez une requête LIKE pour rechercher par ID ou par nom
    query.prepare("SELECT * FROM EMPLOYE WHERE ID LIKE :searchText OR NOM LIKE :searchText");
    query.bindValue(":searchText", "%" + searchText + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("SEXE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("DEPARTEMENT"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("SALAIRE"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("NUM_TEL"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("ADRESSE_EMAIL"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("PASSWORD"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("CODE"));

        ui->tableView->setModel(model);
    } else {
        qDebug() << "Error in SQL query: " << query.lastError().text();
        return;
    }
}


void DialogEmployes::on_pushButton_44_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);

       QSqlQuery query;
       query.prepare("SELECT * FROM EMPLOYE ORDER BY NOM ASC, PRENOM ASC");


       if (query.exec()) {
           model->setQuery(query);
           ui->tableView->setModel(model);
           ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

           return;
       }
}

void DialogEmployes::on_pushButton_40_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE ORDER BY NOM DESC, PRENOM DESC");

    if (query.exec()) {
        model->setQuery(query);
        ui->tableView->setModel(model);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}

void DialogEmployes::on_pushButton_38_clicked()
{
    /*QSqlQueryModel *model = emp.afficher();

    if (!model) {
        qDebug() << "Error: QSqlQueryModel is null.";
        return;
    }

    int totalEmployes = model->rowCount();
    int Informatique = 0;
    int Ressource_humaine = 0;
    int Sinistre = 0;
    int Batiment = 0;

    // Assuming 'departement' is an attribute in the employe class
    int departementColumnIndex = model->record().indexOf("departement");

    // Count employees in each department
    for (int row = 0; row < totalEmployes; ++row) {
        QString departement = model->data(model->index(row, departementColumnIndex)).toString().toLower();
        if (departement == "informatique") {
            Informatique++;
        } else if (departement == "ressource_humaine") {
            Ressource_humaine++;
        } else if (departement == "sinistre") {
            Sinistre++;
        } else if (departement == "batiment") {
            Batiment++;
        }
    }

    // Create QPieSeries
    QPieSeries *series = new QPieSeries();

    // Add slices
    if (Informatique > 0) {
        QPieSlice *sliceInformatique = new QPieSlice(QString("Informatique (%1)").arg(Informatique), Informatique);
        series->append(sliceInformatique);
    }

    if (Ressource_humaine > 0) {
        QPieSlice *sliceRessource_humaine = new QPieSlice(QString("Ressource humaine (%1)").arg(Ressource_humaine), Ressource_humaine);
        series->append(sliceRessource_humaine);
    }

    if (Sinistre > 0) {
        QPieSlice *sliceSinistre = new QPieSlice(QString("Sinistre (%1)").arg(Sinistre), Sinistre);
        series->append(sliceSinistre);
    }

    if (Batiment > 0) {
        QPieSlice *sliceBatiment = new QPieSlice(QString("Batiment (%1)").arg(Batiment), Batiment);
        series->append(sliceBatiment);
    }

    // Create QChart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistique par Département");

    // Customize legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create QChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set chart animation
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView->setChart(chart);

    // Create QDialog
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setFixedSize(1080, 720);
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);
    layout->addWidget(chartView);

    // Close button
    QPushButton *closeButton = new QPushButton("Fermer", chartDialog);
    connect(closeButton, &QPushButton::clicked, chartDialog, &QDialog::accept);
    layout->addWidget(closeButton);

    // Display the window
    chartDialog->exec();
    */
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

        QHash<QString, int> Distribution = Employe::statistiques();

        int total = 0;
        for (const QString &categorie : Distribution.keys()) {
            total += Distribution.value(categorie);
        }

        for (const QString &categorie : Distribution.keys()) {
            int value = Distribution.value(categorie);
            double percentage = (value * 100.0) / total;
            QString label = QString("%1 (%2%)").arg(categorie).arg(percentage, 0, 'f', 1);

            series->append(label, value);
        }

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition par Catégorie de Dommages");

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


void DialogEmployes::on_pushButton_43_clicked()
{
/*
    // Créez un dialogue d'impression pour choisir les options d'impression
    QPrinter printer;
    QPrintDialog dialog(&printer, this);

    if (dialog.exec() == QDialog::Accepted) {
        // Configurez le format de la page et l'orientation
        printer.setPageSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Landscape);

        // Initialisez le peintre avec l'imprimante
        QPainter painter(&printer);
        painter.begin(&printer);

        // Définissez la taille de la page et la zone d'impression
        QRect pageRect = printer.pageRect();
        painter.setViewport(pageRect.x(), pageRect.y(), pageRect.width(), pageRect.height());
        painter.setWindow(pageRect);

        // Insérer une image (utilisez le chemin complet vers votre image sur le bureau)
        QString imagePath = QDir::homePath() + "/OneDrive/Bureau/qt image/imgpri.png";  // Assurez-vous de l'extension correcte
        QImage image(imagePath);
        if (!image.isNull()) {
            // Ajustez la taille et la position de l'image
            QSize imageSize(200, 200);  // Ajustez la taille de l'image selon vos besoins
            QPoint imagePos(50, 50);    // Ajustez la position de l'image selon vos besoins
            QRect imageRect(imagePos, imageSize);
            painter.drawImage(imageRect, image);
        }

        // Créez un modèle de données pour stocker les informations des employeurs
        QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->tableView->model());

        // Définir les paramètres de la mise en page
        const int margin = 100;  // Ajustez la marge selon vos besoins
        const int headerHeight = 150;
        int rowHeight = 20;
        int columnWidth = 150;
        int y = margin + headerHeight;

        // Dessiner l'en-tête avec les noms de colonnes
        for (int col = 0; col < model->columnCount(); ++col) {
            QString headerText = model->headerData(col, Qt::Horizontal).toString();
            painter.drawText(margin + col * columnWidth, y, columnWidth, headerHeight, Qt::AlignCenter, headerText);
        }
        y += headerHeight;

        // Dessiner les données
        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QString cellValue = model->data(model->index(row, col)).toString();
                painter.drawText(margin + col * columnWidth, y, columnWidth, rowHeight, Qt::AlignLeft | Qt::AlignVCenter, cellValue);
            }
            y += rowHeight;
        }


        painter.end();
    }
    */
    // Sélectionner le fichier de destination
       QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", "*.pdf");
       if (fileName.isEmpty())
           return;

       // Créer un document PDF
       QPrinter printer(QPrinter::PrinterResolution);
       printer.setOutputFormat(QPrinter::PdfFormat);

       // Définir une taille de papier personnalisée
       QSizeF customPageSize(1000, 1100); // Largeur x Hauteur (en points)
       printer.setPageSize(QPageSize(customPageSize, QPageSize::Point));

       printer.setOutputFileName(fileName);

       // Créer un document texte pour le PDF
       QTextDocument doc;

       // Ajouter le contenu HTML
       QString htmlContent = "<html><body><h2>Liste des Employés :</h2><br><br>";
       htmlContent += "<table border='1'><tr><th>ID</th><th>NOM</th><th>PRENOM</th><th>SEXE</th><th>DEPARTEMENT</th><th>SALAIRE</th><th>NUM_TEL</th><th>ADRESSE_EMAIL</th><th>PASSWORD</th><th>CODE</th></tr>";

       // Récupérer les données de la table view
       QTableView *tableView = ui->tableView;
       QAbstractItemModel *model = tableView->model();
       int rows = model->rowCount();
       int columns = model->columnCount();

       // Insérer les données de la table view dans le tableau HTML
       for (int row = 0; row < rows; ++row) {
           htmlContent += "<tr>";
           for (int column = 0; column < columns; ++column) {
               QModelIndex index = model->index(row, column);
               QString data = model->data(index).toString();
               htmlContent += "<td>" + data + "</td>";
           }
           htmlContent += "</tr>";
       }

       htmlContent += "</table></body></html>";

       doc.setHtml(htmlContent);

       // Imprimer le document dans le fichier PDF
       QPainter painter;
       if (!painter.begin(&printer)) {
           qDebug() << "Failed to open printer";
           return;
       }

       doc.drawContents(&painter);
       painter.end();
}
#include "mainwindow.h"
#include "dialoglogin.h"
void DialogEmployes::on_pushButton_logout_Employe_clicked()
{
    DialogLogin *dLogin = new DialogLogin();
    dLogin->show();
    hide();

}

void DialogEmployes::on_pushButton_retour_clicked()
{
    int nombreEntier = ui->label_id_employe->text().toInt(); // Convertir en entier directement
    MainWindow *M = new MainWindow();
    M->setResultRE(nombreEntier); // Appeler une fonction de la deuxième interface pour définir le résultat
    M->show();
    //this->close();
    hide();
}
void DialogEmployes::setResultE(int result)
{
    ui->label_id_employe->setText(QString::number(result)); // Afficher le résultat dans le Label
}
