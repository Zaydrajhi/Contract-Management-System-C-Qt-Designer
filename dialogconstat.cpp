
//************************************************************
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QPointF>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
//*************************************************************
#include "dialogconstat.h"
#include "ui_dialogconstat.h"
#include <QDate>
#include "constat.h"
#include <QString>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

#include <QTextDocument>
#include <QTextCursor>
#include <QPrinter>

#include <QTextDocumentFragment>

#include <QTableView>
#include <QQuickItem>
#include <QUrl>
#include "smtp.h"

#include <QTcpSocket>
#include <QTextStream>

DialogConstat::DialogConstat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConstat)
{

    ui->setupUi(this);



    ui->lineEdit_degat->setValidator(new QIntValidator(0,100,this));
    ui->lineEdit_degatm->setValidator(new QIntValidator(0,100,this));
    ui->tableView->setModel(Cons.afficher());
    proxyModel = new QSortFilterProxyModel(this);
       proxyModel->setSourceModel(Cons.afficher());


       ui->tableView->setModel(proxyModel);


       connect(ui->lineEdit_chercher, &QLineEdit::textChanged, this, &DialogConstat::chercherConstat);
       connect(ui->lineEdit_chercher_num, &QLineEdit::textChanged, this, &DialogConstat::chercherConstatNum);
       connect(ui->lineEdit_chercherCon, &QLineEdit::textChanged, this, &DialogConstat::chercherConstatNumCon);

       connect(ui->tableView, &QTableView::clicked, this, &DialogConstat::on_tableView_clicked);

       connect(ui->pushButton_35, &QPushButton::clicked, this, &DialogConstat::on_pushButton_35_clicked);



}


DialogConstat::~DialogConstat()
{
    delete ui;
}



void DialogConstat::on_pushButton_3_clicked()
{
        int num = ui->lineEdit_numero->text().toInt();
         int numero = ui->lineEdit_numeroC->text().toInt();
        QDateTime date_constat = ui->dateEdit_date->dateTime();
        QString adresse = ui->lineEdit_adresse->text();

        QString description = ui->textEdit_desc->toPlainText();
        QString degat = ui->lineEdit_degat->text();
       Constat C(num,numero,date_constat,adresse,description,degat);

                       //controle de saisir

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

     /*  if (!degat.contains("%") && degat.toInt() < 0 && degat.toInt() > 100 ) {
           QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Degat non effectué\n"
                                                                           "Click Cancel to exit."),QMessageBox::Cancel);
           return;
       }*/

       bool test=C.ajouter();




       if(test)
       {
           QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Ajouter effectué\n"
                                                                           "Click Cancel to exit."),QMessageBox::Cancel);

           smtp* ssmtp = new smtp("chihebmejri03@gmail.com", "fqda vhch voej sloc", "smtp.gmail.com", 465);

                             connect(ssmtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
                             QString email="aymentayari191@gmail.com";

                             QString b = "Constat ajouté";

                             QString ob="Le constat a été ajouté avec succès :\n\n\n\n\n" +  QString("                        COORDONNÉES D'UN CONSTAT: \n") + QString("\n-Numero de constat: ")
                                     +ui->lineEdit_numero->text()+ QString("\n-Numero de contrat: ")+ui->lineEdit_numeroC->text()+ QString("\n-Date dépôt: ")+ui->dateEdit_date->text()
                                    + QString("\n-Adresse : ")+ui->lineEdit_adresse->text()+ QString("\n-Déscription : ")+ui->textEdit_desc->toPlainText() + QString("\n-Dégat : ")+ui->lineEdit_degat->text() + QString(" % ") ;
                             ssmtp->sendMail("chihebmejri03@gmail.com", email , b,ob);
                  ui->lineEdit_numero->clear();
                  ui->lineEdit_numeroC->clear();
                  ui->lineEdit_adresse->clear();
                  ui->textEdit_desc->clear();
                  ui->lineEdit_degat->clear();





       }
       else
           QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                 QObject::tr("Ajout non effectué.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);

       ui->tableView->setModel(Cons.afficher());
       proxyModel->setSourceModel(Cons.afficher());


}


void DialogConstat::mailSent(const QString &status) {
    // Implémentez le traitement à effectuer lorsque le mail est envoyé avec succès
    qDebug() << "Mail envoyé avec succès. Statut : " << status;
}



void DialogConstat::on_pushButton_34_clicked()
{
    QString numeroString = ui->lineEdit_numeros->text();

    if (numeroString.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                              QObject::tr("Veuillez entrer un numéro de constat."),
                              QMessageBox::Cancel);
        return;
    }

    int num = numeroString.toInt();

    if (!Cons.numeroExiste(num)) {
        QMessageBox::critical(nullptr, QObject::tr("Constat non trouvé"),
                              QObject::tr("Le numéro de constat spécifié n'existe pas."),
                              QMessageBox::Cancel);
        ui->lineEdit_numeros->clear();
        ui->lineEdit_numerom->clear();
        return;
    }

    bool test = Cons.supprimer(num);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
        smtp* ssmtp = new smtp("chihebmejri03@gmail.com", "fqda vhch voej sloc", "smtp.gmail.com", 465);

                          connect(ssmtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
                          QString email="aymentayari191@gmail.com";

                          QString b = "Constat supprimé";

                          QString ob="Le numéro de constat "+ui->lineEdit_numeros->text()+QString(" a été supprimé avec succès ")  ;
                          ssmtp->sendMail("chihebmejri03@gmail.com", email , b,ob);
        ui->lineEdit_numeros->clear();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);

    }
    ui->tableView->setModel(Cons.afficher());
    proxyModel->setSourceModel(Cons.afficher());


}



void DialogConstat::on_pushButton_supprimer_clicked(){}





void DialogConstat::on_pushButton_ok_clicked()
{
    int num = ui->lineEdit_numerom->text().toInt();
    QString numeroString = ui->lineEdit_numerom->text();
    const Constat& C = Cons.rechercher(num);
    if (numeroString.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                              QObject::tr("Veuillez entrer un numéro de constat."),
                              QMessageBox::Cancel);
        return;
    }

    if (!Cons.numeroExiste(num)) {
        QMessageBox::critical(nullptr, QObject::tr("Constat non trouvé"),
                              QObject::tr("Le numéro de constat spécifié n'existe pas."),
                              QMessageBox::Cancel);
        return;
    }

    if (C.getnum() != -1) {

        ui->lineEdit_numeroCM->setText(QString::number(C.getnumero()));
        ui->lineEdit_adressem->setText(C.getadresse());
        ui->dateEdit_datem->setDateTime(C.getdate());

        ui->textEdit_descm->setText(C.getdesc());
        ui->lineEdit_degatm->setText(C.getdegat());
    } else {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Aucun constat trouvé avec ce numéro."), QMessageBox::Ok);
    }

}




void DialogConstat::on_pushButton_modifier_clicked()
{
    int num = ui->lineEdit_numerom->text().toInt();
    const Constat& C = Cons.rechercher(num);

    if (C.getnum() != -1) {
        Constat copie_constat = C;
        int nouveau_numero = ui->lineEdit_numeroCM->text().toInt();
        QString nouvelle_adresse = ui->lineEdit_adressem->text();
        QDateTime nouvelle_date = ui->dateEdit_datem->dateTime();
        QString nouvelle_description = ui->textEdit_descm->toPlainText();
        QString nouveau_degat = ui->lineEdit_degatm->text();

        copie_constat.setnumero(nouveau_numero);
        copie_constat.setadresse(nouvelle_adresse);
        copie_constat.setdate(nouvelle_date);
        copie_constat.setdesc(nouvelle_description);
        copie_constat.setdegat(nouveau_degat);
        if (nouvelle_adresse.isEmpty() || nouvelle_description.isEmpty() || nouveau_degat.isEmpty()) {
                QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                                      QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
                return;
         }


        bool modification_reussie = copie_constat.modifier();
        if (modification_reussie) {
            QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Modification effectuée\n"
                                                                             "Click Cancel to exit."), QMessageBox::Cancel);
            smtp* ssmtp = new smtp("chihebmejri03@gmail.com", "fqda vhch voej sloc", "smtp.gmail.com", 465);

                              connect(ssmtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
                              QString email="aymentayari191@gmail.com";

                              QString b = "Constat modifié";

                              QString ob="Le constat a été modifié avec succès :\n\n\n\n\n" +  QString("                   NOUVELLES COORDONNÉES D'UN CONSTAT: \n") + QString("\n-Numero de constat: ")
                                      +ui->lineEdit_numerom->text()+ QString("\n-Numero de contrat: ")+ui->lineEdit_numeroCM->text()+ QString("\n-Date dépôt: ")+ui->dateEdit_datem->text()
                                     + QString("\n-Adresse : ")+ui->lineEdit_adressem->text()+ QString("\n-Déscription : ")+ui->textEdit_descm->toPlainText() + QString("\n-Dégat : ")+ui->lineEdit_degatm->text() + QString(" % ") ;
                              ssmtp->sendMail("chihebmejri03@gmail.com", email , b,ob);

            ui->tableView->setModel(Cons.afficher());
            ui->lineEdit_numerom->clear();
            ui->lineEdit_numeroCM->clear();
            ui->lineEdit_numeros->clear();
            ui->lineEdit_adressem->clear();
            ui->textEdit_descm->clear();
            ui->lineEdit_degatm->clear();
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Modification non effectuée.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Aucun constat trouvé avec ce numéro."), QMessageBox::Ok);

    }

proxyModel->setSourceModel(Cons.afficher());
}



void DialogConstat::on_pushButton_refreche_clicked()
{

     ui->tableView->setModel(Cons.afficher());
     proxyModel->setSourceModel(Cons.afficher());
     ui->comboBox->setCurrentIndex(0);


}

void DialogConstat::chercherConstat(const QString &text)
{


    proxyModel->setFilterKeyColumn(3);
       proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive));
       ui->tableView->setModel(proxyModel);

}

void DialogConstat::chercherConstatNum(const QString &text){
    proxyModel->setFilterKeyColumn(0); // Supposons que le numéro de constat est dans la colonne 2 (index 1)
    proxyModel->setFilterFixedString(text);
       ui->tableView->setModel(proxyModel);

}

void DialogConstat::chercherConstatNumCon(const QString &text){
    proxyModel->setFilterKeyColumn(1); // Supposons que le numéro de constat est dans la colonne 2 (index 1)
    proxyModel->setFilterFixedString(text);
       ui->tableView->setModel(proxyModel);

}


/*void DialogConstat::chercherConstat(const QString &text) {
    bool ok;
    int numeroConstat = text.toInt(&ok);

    if (ok) {
        // Si la conversion en entier est réussie, cela signifie que nous avons un numéro de constat valide
        proxyModel->setFilterKeyColumn(0); // Supposons que le numéro de constat est dans la colonne 2 (index 1)
        proxyModel->setFilterFixedString(text); // Filtrer par le numéro de constat directement
    } else {
        // Si la conversion en entier échoue, cela signifie que nous avons une adresse (chaîne de caractères)
        proxyModel->setFilterKeyColumn(3); // Supposons que l'adresse est dans la colonne 3 (index 2)
        proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive)); // Filtrer par l'adresse
    }

    ui->tableView->setModel(proxyModel);
}*/

void DialogConstat::on_pushButton_trier_clicked()
{
    QString type = ui->comboBox->currentText();
    if(type=="Date"){
    proxyModel->sort(2, Qt::DescendingOrder);
    ui->tableView->setModel(proxyModel);
    }
    else if(type == "Degat"){
        proxyModel->sort(5, Qt::DescendingOrder);
        ui->tableView->setModel(proxyModel);
    }
    else if(type == "Numero constat"){
        proxyModel->sort(0, Qt::DescendingOrder);
        ui->tableView->setModel(proxyModel);
    }
    else if(type == "Numero contrat"){
        proxyModel->sort(1, Qt::DescendingOrder);
        ui->tableView->setModel(proxyModel);
    }

    else if(type == "Choisir un choix"){
        QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                              QObject::tr("Veuillez choisir un choix."), QMessageBox::Cancel);
    }

}




void DialogConstat::on_pushButton_trier_croi_clicked()
{

    QString type = ui->comboBox->currentText();
    if(type=="Date"){
        proxyModel->sort(2, Qt::AscendingOrder);
        ui->tableView->setModel(proxyModel);
    }
    else if(type == "Degat"){
        proxyModel->sort(5, Qt::AscendingOrder);
        ui->tableView->setModel(proxyModel);
    }
    else if(type == "Numero constat"){
        proxyModel->sort(0, Qt::AscendingOrder);
        ui->tableView->setModel(proxyModel);
    }
    else if(type == "Numero contrat"){
        proxyModel->sort(1, Qt::AscendingOrder);
        ui->tableView->setModel(proxyModel);
    }
    else if(type == "Choisir un choix"){
        QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                              QObject::tr("Veuillez choisir un choix."), QMessageBox::Cancel);
    }

}


void DialogConstat::on_tableView_clicked(const QModelIndex &index)
{

    if (index.isValid() && index.column() == 0) {

        QString numero = index.data().toString();
        ui->lineEdit_numeros->setText(numero);
        ui->lineEdit_numerom->setText(numero);

    }
}


void DialogConstat::on_pushButton_statistique_clicked()
{
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

    QHash<QString, int> Distribution = Constat::statistiques();

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




void DialogConstat::on_pushButton_35_clicked() {
    // Sélectionner le fichier de destination
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", "*.pdf");
    if (fileName.isEmpty())
        return;

    // Créer un document PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);

    // Définir une taille de papier personnalisée
    QSizeF customPageSize(800, 1100); // Largeur x Hauteur (en points)
    printer.setPageSize(QPageSize(customPageSize, QPageSize::Point));

    printer.setOutputFileName(fileName);

    // Créer un document texte pour le PDF
    QTextDocument doc;

    // Ajouter le contenu HTML
    QString htmlContent = "<html><body><h2>Liste des constats :</h2><br><br>";
    htmlContent += "<table border='1'><tr><th>numero constat</th><th>numero contrat</th><th>date constat</th><th>adresse</th><th>description</th><th>degat (%)</th></tr>";

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











void DialogConstat::on_pushButton_pdfS_clicked()
{
    // Récupérer le numéro de constat
          QString numeroString = ui->lineEdit_numeros->text();
          if (numeroString.isEmpty()) {
              //QMessageBox::warning(this, "Champ vide", "Veuillez entrer un numéro de constat.");
              return;
          }

          int num = numeroString.toInt();

          // Récupérer les détails du constat à partir de la classe Constat
          Constat constat = Constat::getConstatDetails(num); // Assurez-vous d'avoir une fonction pour récupérer les détails du constat

          // Générer le contenu du PDF avec les coordonnées du constat
          QString pdfContent = "\n\n\n\n\n\n                                      Informations du constat :\n\n\n"
                               " -Numéro de constat : " + QString::number(num) + "\n\n"
                               " -Numero constat : " + QString::number(constat.getnumero()) + "\n\n"
                               " -Date de dépôt : " + constat.getdate().toString() + "\n\n"
                               " -Adresse : " + constat.getadresse() + "\n\n"

                               " -Description : " + constat.getdesc() + "\n\n"
                               " -Dégât : " + constat.getdegat() + "\n\n"
                               "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
                               "\n\n       ASSURITE - El Aouina, 36 Av. Khaled Ibn El Walid, Tunis 2045 - 78 450 200\n"
                               "                      RC B2411262004, Mat. Fiscal: 0868024D/A/M/0000 "   ;


          // Demander à l'utilisateur l'emplacement où enregistrer le PDF
          QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", QDir::homePath(), "Fichiers PDF (*.pdf)");
          if (filePath.isEmpty())
              return;

          // Créer un document PDF
          QPdfWriter pdfWriter(filePath);
          QPainter painter(&pdfWriter);

              QFont font = painter.font();
             font.setPointSize(14); // Changer la taille de la police selon vos besoins
             painter.setFont(font);

          // Écrire le contenu dans le PDF
          QTextDocument textDocument;
          textDocument.setDefaultFont(font);
          textDocument.setPlainText(pdfContent);
          textDocument.drawContents(&painter);

          QImage image("C:/Users/ASUS/Desktop/Projet c++/Agence_d_assurance/image.jpg"); // Chemin vers votre image
          int newWidth = 1500; // Nouvelle largeur de l'image (augmentée)
          int newHeight = 1500; // Nouvelle hauteur de l'image (augmentée)
          QImage scaledImage = image.scaled(newWidth, newHeight); // Redimensionner l'image sans conserver le ratio

          QRectF targetRect(0, 0, newWidth, newHeight); // Rectangle cible pour l'image
          painter.drawImage(targetRect, scaledImage);


                 /* QImage image("C:/Users/ASUS/Desktop/Projet c++/Agence_d_assurance/image.jpg"); // Chemin vers votre image
                  QSize newSize(1000, 1000); // Nouvelle taille de l'image
                      QImage scaledImage = image.scaled(newSize, Qt::KeepAspectRatio); // Redimensionner l'image en conservant le ratio

                      QRectF targetRect(0, 0, newSize.width(), newSize.height()); // Rectangle cible pour l'image
                      QRectF sourceRect = QRectF(0, 0, scaledImage.width(), scaledImage.height());
                      painter.drawImage(targetRect, image, sourceRect);*/


         ui->lineEdit_numeros->clear();
         ui->lineEdit_numerom->clear();

          // Informer l'utilisateur que le PDF a été exporté avec succès
          //QMessageBox::information(this, "PDF Exporté", "Les coordonnées du constat ont été exportées dans un fichier PDF.");*/

}

void DialogConstat::on_sendButton_clicked()
{
    QString message = ui->messageLineEdit->text();
             if (!message.isEmpty()) {
                 QDateTime currentDateTime = QDateTime::currentDateTime();
                 QString currentDateTimeString = currentDateTime.toString("dd/MM/yyyy hh:mm:ss");
                 QString formattedMessage = QString("[%1] You: %2\n").arg(currentDateTimeString, message);
                 ui->conversationPlainTextEdit->append(formattedMessage);

                 // Placeholder for bot response - replace this with your own logic
                 QString botResponse;
                        if ( message.contains("merci", Qt::CaseInsensitive) || message.contains("mrc", Qt::CaseInsensitive)) {
                            botResponse = "Bot: De rien ! Si vous avez d'autres questions ou besoin d'aide supplémentaire.";
                        }
                        else if ( message.contains("bonjour", Qt::CaseInsensitive) || message.contains("bnj", Qt::CaseInsensitive)) {
                            botResponse = "Bot: Bonjour ! Comment puis-je vous aider aujourd'hui ?";
                        }
                        else if ( message.contains("salut", Qt::CaseInsensitive) || message.contains("slt", Qt::CaseInsensitive) ) {
                            botResponse = "Bot: Salut ! Comment puis-je vous aider aujourd'hui ?";
                        }
                        else if ( message.contains("ajou", Qt::CaseInsensitive)  || message =="Comment ajouter un constat ?") {
                              botResponse = "Bot: Pour ajouter un constat, cliquez sur 'Insertion' et remplissez les coordonnées du constat.";

                        }
                        else if (message.contains("modif ", Qt::CaseInsensitive)|| message =="Comment modifier un constat ?") {
                              botResponse = "Bot: Pour modifier un constat, cliquez sur 'modifier', entrez le numéro à modifier, puis cliquez sur 'OK' et modifiez les coordonnées du constat.";
                        }
                        else if ( message.contains("suppr", Qt::CaseInsensitive) || message =="Comment supprimer un constat ?") {
                              botResponse = "Bot: Pour supprimer un constat, cliquez sur 'Liste', sélectionnez le numéro à supprimer, puis cliquez sur 'Supprimer'.";
                        }
                        else if (message.contains("degat" , Qt::CaseInsensitive) ) {
                              botResponse = "Bot: Degat de constat exprime en (%), représentant les dommages subis par la personne concernée et doit être compris entre 0 et 100 .";
                        }
                        else if (message.contains("desc" , Qt::CaseInsensitive) ) {
                              botResponse = "Bot: Description de constat représentant les détails et circonstances entourant l'événement, afin de fournir une compréhension complète de la situation.";
                        }
                        else if (message.contains("statis", Qt::CaseInsensitive) || message =="Comment voir les statistiques des constats ?") {
                              botResponse = "Bot: Pour voir les statistiques, cliquez sur 'Liste', puis sur le bouton 'Statistiques'.";
                        }
                        else if (message.contains("pdf", Qt::CaseInsensitive) || message =="Comment exporter un PDF ?") {
                              botResponse = "Bot: Pour exporeter un pdf, cliquez sur 'Liste', puis sur le bouton 'PDF'.";
                        }

                        else if (message.contains( "cherch", Qt::CaseInsensitive) || message == "Comment chercher un constat ?") {
                              botResponse = "Bot: Pour rechercher un constat, cliquez sur 'Liste', puis entrez le numéro ou l'adresse du constat à rechercher.";
                        }
                        else if (message.contains("tri", Qt::CaseInsensitive) || message == "Comment trier les listes de constats ?") {
                              botResponse = "Bot: Pour trier un constat, cliquez sur 'Liste', puis sur le bouton 'trier croissante' ou 'trier décroissante'.";

                        }
                        else if (message.contains("type", Qt::CaseInsensitive)) {
                              botResponse = "Bot: Il existe trois types d'assurances :\n-Maison\n-Santé\n-Voiture\nTout risque";

                        }
                        else if (message.contains("cons" , Qt::CaseInsensitive) || message == "Que signifie constat ?") {
                              botResponse ="Bot: un constat est un document utilisé pour enregistrer les détails d'un accident ou d'un événement assuré, fournissant une base pour évaluer les réclamations et déterminer les responsabilités\nConstat comprend:\n -Numéro de constat\n -Numéro de contrat\n -Date de dépôt\n -Adresse\n -dégât\n -Description ";
                        }
                        else if (message.contains("ca va" , Qt::CaseInsensitive) || message.contains("cv" , Qt::CaseInsensitive)) {
                              botResponse ="Bot: Je vais bien, merci! Et toi, comment ça va ?";
                        }
                        else if (message.contains("type", Qt::CaseInsensitive)) {
                              botResponse = "Bot: Il existe trois types d'assurances :\n-Maison\n-Santé\n-Voiture\n-Tout risque";

                        }
                        else if (message.contains("app", Qt::CaseInsensitive)) {
                              botResponse = "Bot: Il existe cinq gestions dans notre application  :\n-Gestion d'employés\n-Gestion d'adhérents\n-Gestion de contarts \n-Gestion de constats\n-Gestion de partenaires";

                        }
                        else if (message.contains("assuri" , Qt::CaseInsensitive) || message.contains("assurance" , Qt::CaseInsensitive)) {
                              botResponse ="Bot: L'agence d'assurance 'ASSURITé' est dédiée à la protection financière et à la tranquillité d'esprit de ses clients. Spécialisée dans les services d'assurance, elle propose des solutions adaptées pour couvrir les risques liés à la santé, aux biens, et à la responsabilité civile.' ASSURITé' vise à offrir des garanties fiables et un service client de qualité pour répondre aux besoins individuels et professionnels.";
                        }



                        else {
                            // Placeholder for other responses
                            botResponse = "Bot: Désolé, je ne comprends pas. Pouvez-vous reformuler votre question ?";
                        }
                 QString formattedBotResponse = QString("[%1] %2\n").arg(currentDateTimeString, botResponse);
                 ui->conversationPlainTextEdit->append(formattedBotResponse);

                 ui->messageLineEdit->clear();
             }

}



void DialogConstat::on_pushButtonq8_clicked()
{
     ui->messageLineEdit->setText("Que signifie constat ?");
}

void DialogConstat::on_pushButtonq1_clicked()
{
     ui->messageLineEdit->setText("Comment ajouter un constat ?");
}

void DialogConstat::on_pushButtonq2_clicked()
{
     ui->messageLineEdit->setText("Comment modifier un constat ?");
}

void DialogConstat::on_pushButtonq3_clicked()
{
     ui->messageLineEdit->setText("Comment supprimer un constat ?");
}

void DialogConstat::on_pushButtonq4_clicked()
{
     ui->messageLineEdit->setText("Comment voir les statistiques des constats ?");
}

void DialogConstat::on_pushButtonq5_clicked()
{
     ui->messageLineEdit->setText("Comment chercher un constat ?");
}

void DialogConstat::on_pushButtonq6_clicked()
{
     ui->messageLineEdit->setText("Comment trier les listes de constats ?");
}

void DialogConstat::on_pushButtonq7_clicked()
{
     ui->messageLineEdit->setText("Comment exporter un PDF ?");
}

#include "mainwindow.h"
void DialogConstat::on_pushButton_retour_clicked()
{
    int nombreEntier = ui->label_id_employe->text().toInt(); // Convertir en entier directement
    MainWindow *M = new MainWindow();
    M->setResultRCons(nombreEntier); // Appeler une fonction de la deuxième interface pour définir le résultat
    M->show();
    //this->close();
    hide();

}
#include "dialoglogin.h"
void DialogConstat::on_pushButton_logout_Adherent_clicked()
{
    DialogLogin *dLogin = new DialogLogin();
    dLogin->show();
    hide();

}
void DialogConstat::setResultCons(int result)
{
    ui->label_id_employe->setText(QString::number(result)); // Afficher le résultat dans le Label
}
