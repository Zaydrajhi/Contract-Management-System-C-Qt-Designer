#include "dialogadherent.h"
#include "dialoglogin.h"
#include "mainwindow.h"
#include "ui_dialogadherent.h"
#include "adherent.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDebug>
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
#include <QFileDialog>
#include <QPdfWriter>
#include <QTextDocument>
#include <QTableWidget>
#include <QFileDialog> // Ajout de cette ligne pour pouvoir utiliser QFileDialog
#include <QBuffer> // Ajout de cette ligne pour pouvoir utiliser QBuffer
#include <QPrinter>


DialogAdherent::DialogAdherent(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::DialogAdherent)
{
   ui->setupUi(this);
   ui->lineEdit_cin->setValidator(new QIntValidator(10000000,99999999,this));
   ui->lineEdit_num_tel->setValidator(new QIntValidator(10000000,99999999,this));
   ui->lineEdit_fidelite->setValidator(new QIntValidator(1,99999999,this));

   //ui->tableView_Adherent->setModel(Atmp.afficher());
   /*********************************************AFFICHAGE**************************************************/
   QSqlQueryModel *model = Atmp.afficher();
   ui->tableView_Adherent->setModel(model);

   ImageDelegate *imageDelegate = new ImageDelegate(this);
   ui->tableView_Adherent->setItemDelegateForColumn(8, imageDelegate); // Assuming the image column is at index 8
   /*******************************************************************************************************/


   connect(ui->tableView_Adherent, &QTableView::clicked, this, &DialogAdherent::on_tableView_Adherent_clicked);

   connect(ui->lineEdit_id_a_chercher, &QLineEdit::textChanged, this, &DialogAdherent::rechercheAutomatique);
   // Connecter le signal textChanged de ui->lineEdit_prenom_chercher
   connect(ui->lineEdit_prenom_chercher, &QLineEdit::textChanged, this, &DialogAdherent::rechercheAutomatique);

   // Connecter le signal finished à la fonction onDialogShown
   connect(this, &QDialog::finished, this, &DialogAdherent::onDialogShown);



   //CAMERA
   for(const QCameraInfo &infor : QCameraInfo::availableCameras())
   {
       qDebug() << infor.description();
   }
   M_Camera.reset(new QCamera(QCameraInfo::defaultCamera()));

   M_Camera->setViewfinder(ui->widget_Camera_View);

   M_Camera_Image.reset(new QCameraImageCapture(M_Camera.data()));
   M_Camera_Image->setCaptureDestination(QCameraImageCapture::CaptureToFile);
   connect(M_Camera_Image.data(), &QCameraImageCapture::imageCaptured,this, &DialogAdherent::imageCaptured);



   //CAMERA MODIF
   for(const QCameraInfo &info : QCameraInfo::availableCameras())
   {
       qDebug() << info.description();
   }

   M_Camera_modif.reset(new QCamera(QCameraInfo::defaultCamera()));

   M_Camera_modif->setViewfinder(ui->widget_Camera_View_modif);

   M_Camera_Image_modif.reset(new QCameraImageCapture(M_Camera_modif.data()));
   M_Camera_Image_modif->setCaptureDestination(QCameraImageCapture::CaptureToFile);
   connect(M_Camera_Image_modif.data(), &QCameraImageCapture::imageCaptured, this, &DialogAdherent::imageCaptured_modif);


}
void DialogAdherent::on_pushButton_chercher_clicked(){}
void DialogAdherent::on_pushButton_31_clicked(){}
void DialogAdherent::on_pushButton_pdf_2_clicked(){}

void DialogAdherent::showEvent(QShowEvent *event) {
    historiqueAutomatique();
    QDialog::showEvent(event);
}



void DialogAdherent::onDialogShown() {
    historiqueAutomatique();
}

void DialogAdherent::historiqueAutomatique()
{
    QFile file("his.txt");
          if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
               QMessageBox::information(0,"info",file.errorString());
                   QTextStream lecture(&file);
                   ui->textBrowser->setText(lecture.readAll());
                   QString montext = lecture.readAll();
                   file.close();
}
/*
void DialogAdherent::on_pushButton_supprimerHistorique_clicked()
{
    QFile file("his.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        file.close();
        ui->textBrowser->clear(); // Clear text browser if needed
        QMessageBox::information(0, "Info", "Historique supprimé avec succès.");
    } else {
        QMessageBox::warning(0, "Erreur", "Impossible de supprimer l'historique.");
    }
}
*/
void DialogAdherent::on_pushButton_supprimerHistorique_clicked()
{
    QMessageBox confirmationBox(QMessageBox::Question,
                                 tr("Confirmation"),
                                 tr("Êtes-vous sûr de vouloir supprimer l'historique ?"),
                                 QMessageBox::Yes | QMessageBox::No);
    confirmationBox.setButtonText(QMessageBox::Yes, tr("Confirmer"));
    confirmationBox.setButtonText(QMessageBox::No, tr("Annuler"));

    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        QFile file("his.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            file.close();
            ui->textBrowser->clear(); // Clear text browser if needed
            QMessageBox::information(this, "Info", "Historique supprimé avec succès.");
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de supprimer l'historique.");
        }
    } else {
        // User clicked No, do nothing
    }
}




void DialogAdherent::rechercheAutomatique() {
    QString id_a = ui->lineEdit_id_a_chercher->text();
    QString prenom = ui->lineEdit_prenom_chercher->text(); // Nouveau

    // Vérifier si l'ID ou le prénom est vide
    if(id_a.isEmpty() && prenom.isEmpty()) {
        Adherent adherentObj;
        ui->tableView_Adherent->setModel(adherentObj.afficher());
        return; // Sortir de la fonction car aucun filtre n'est appliqué
    }

    Adherent A;
    QSqlQueryModel* searchResults1;

    // Si l'ID n'est pas vide, chercher par ID
    if(!id_a.isEmpty()) {
        searchResults1 = A.chercherParId(id_a);
    }
    // Sinon, chercher par prénom
    else {
        searchResults1 = A.chercherParPrenom(prenom); // À définir dans votre classe Adherent
    }

    if (searchResults1) {
        ui->tableView_Adherent->setModel(searchResults1);

    } else {
        // Gérer le cas où aucun résultat n'est trouvé
    }
}



DialogAdherent::~DialogAdherent()
{
   delete ui;
}
#include <QLabel>

// Fonction pour vérifier si une QLabel est vide (ne contient ni texte ni image)
bool isLabelEmpty(QLabel *label) {
    // Vérifie si la QLabel ne contient pas de texte et n'a pas de pixmap
    return label->text().isEmpty() && label->pixmap() == nullptr;
}
void DialogAdherent::on_pushButton_ajouterAdherent_clicked()
{


        // Réinitialisez les étiquettes d'erreur à vide
    ui->label_erreur_nom->setText("");
    ui->label_erreur_prenom->setText("");
    ui->label_erreur_cin->setText("");
    ui->label_erreur_num_tel->setText("");
    ui->label_erreur_email->setText("");
    ui->label_erreur_fidelite->setText("");
    ui->label_erreur_image_a->setText("");
    ui->label_erreur_ville->setText("");

    // Récupérer les données des champs de l'interface utilisateur
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString email = ui->lineEdit_email->text();
    QString ville = ui->comboBox_ville->currentText();
    QString str_num_tel = ui->lineEdit_num_tel->text();
    int num_tel = str_num_tel.toInt();
    QString str_fidelite = ui->lineEdit_fidelite->text();
    int fidelite = str_fidelite.toInt();
    QString str_cin = ui->lineEdit_cin->text();
    int cin = str_cin.toInt();
    QString cheminImage = ui->lineEdit_cheminImage->text(); // Récupérer le chemin de l'image depuis le LineEdit
    //int id=str_id.toInt();


        // Validez les champs d'entrée
        bool isValid = true;

        if (nom.isEmpty()) {
            ui->label_erreur_nom->setText("<font color='red'>*Le champ du nom est vide !</font>");
            isValid = false;
        }

        // Validation pour le titre (exemplaire, assurez-vous qu'il n'est pas vide)
        if (prenom.isEmpty()) {
            ui->label_erreur_prenom->setText("<font color='red'>*Le champ du prenom est vide !</font>");
            isValid = false;
        }
        if (!email.contains("@")) {
            ui->label_erreur_email->setText("<font color='red'>*Adresse mail non valide !</font>");
            isValid = false;
        }
        if (email.isEmpty()) {
            ui->label_erreur_email->setText("<font color='red'>*Le champ d'adresse mail est vide !</font>");
            isValid = false;
        }
        if (str_num_tel.length()!=8) {
            ui->label_erreur_num_tel->setText("<font color='red'>*Le numéro de téléphone doit contenir 8 chiffres !</font>");
            isValid = false;
        }
        if (str_num_tel.isEmpty()) {
            ui->label_erreur_num_tel->setText("<font color='red'>*Le champ du téléphone est vide !</font>");
            isValid = false;
        }
        if (fidelite <= 0) {
            ui->label_erreur_fidelite->setText("<font color='red'>*Fidelité doit contenir un nombre positive !</font>");
            isValid = false;
        }
        if (str_fidelite.isEmpty()) {
            ui->label_erreur_fidelite->setText("<font color='red'>*Le champ du fidelité est vide !</font>");
            isValid = false;
        }
        if (str_cin.length() != 8) {
            ui->label_erreur_cin->setText("<font color='red'>*Le CIN doit contenir 8 chiffres !</font>");
            isValid = false;
        }

        if (str_cin.isEmpty()) {
            ui->label_erreur_cin->setText("<font color='red'>*Le champ du CIN est vide !</font>");
            isValid = false;
        }
        if (ville == "                 Choisir") {
            ui->label_erreur_ville->setText("<font color='red'>*Le champ du ville est vide !</font>");
            isValid = false;
        }
        //if (cheminImage.isEmpty() || isLabelEmpty(ui->imageLabel)) {
        if (isLabelEmpty(ui->imageLabel)) {
            ui->label_erreur_image_a->setText("<font color='red'>*Le champ d'image est vide !</font>");
            isValid = false;
        }

        // Ajouter une vérification de l'existence de l'ID dans la base de données
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT id FROM GS_CLIENT WHERE id = :id");
        //checkQuery.bindValue(":id", id);
        if (checkQuery.exec() && checkQuery.next()) {
           // ui->lblErreurId->setText("<font color='red'>ID Client existe déjà!!!!</font>");
            isValid = false;
        }

        // Continuez avec les autres validations d'entrée...

        if (isValid) {
            // Si l'entrée est valide et que l'ID n'existe pas, procédez à l'ajout de l'activité
            Adherent A(nom, prenom, email, ville, num_tel, fidelite, cin, cheminImage);
            A.setid_e(ui->label_id_employe_join->text().toInt());
            QSqlQuery query;
            bool test = A.ajouter();
            if (test) {
                //*****************************************************************************************************************************
                            // Récupérer l'ID de l'employé
                            int idEmploye = ui->label_id_employe_join->text().toInt();

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

                Adherent::historique(1, nom + " " + prenom, "",nomEmploye);

                historiqueAutomatique();
                ui->tableView_Adherent->setModel(Atmp.afficher());
                QMessageBox::information(nullptr, QObject::tr("OK"),
                                         QObject::tr("Ajout effectué \n"
                                                     "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
                // Effacer les champs après l'ajout
                ui->lineEdit_nom->clear();
                ui->lineEdit_prenom->clear();
                ui->lineEdit_email->clear();
                ui->comboBox_ville->setCurrentIndex(0); // Sélectionner l'élément par défaut
                ui->lineEdit_num_tel->clear();
                ui->lineEdit_fidelite->clear();
                ui->lineEdit_cin->clear();
                ui->lineEdit_cheminImage->clear(); // Effacer le chemin de l'image
                ui->imageLabel->clear();
                QPixmap imagePixmap("C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg");
                if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
                    // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
                    QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    ui->imageLabel_2->setPixmap(scaledPixmap);
                } else {
                    qDebug() << "Failed to load image from:" << "C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg";
                }

            } else {
                QMessageBox::critical(nullptr, QObject::tr("Échec de l'ajout"),
                                      QObject::tr("L'ajout n'a pas été effectué. \n"
                                                  "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            }

        } else {
            // Gérez l'erreur d'entrée invalide ou d'ID existant
        }
    }

/*
void DialogAdherent::on_pushButton_ajouterAdherent_clicked()
{
    // Récupérer les données des champs de l'interface utilisateur
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString email = ui->lineEdit_email->text();
    QString ville = ui->comboBox_ville->currentText();
    int num_tel = ui->lineEdit_num_tel->text().toInt();
    int fidelite = ui->lineEdit_fidelite->text().toInt();
    int cin = ui->lineEdit_cin->text().toInt();

    QString cheminImage = ui->lineEdit_cheminImage->text(); // Récupérer le chemin de l'image depuis le LineEdit

    // Vérifier si une image a été ajoutée
    if (cheminImage.isEmpty()) {
        QMessageBox::critical(this, "Image manquante", "Veuillez ajouter une image.");
        return;
    }

    // Créer l'objet Adherent avec les données et le chemin de l'image
    Adherent A(nom, prenom, email, ville, num_tel, fidelite, cin, cheminImage);

    // Vérifier les champs et ajouter l'adhérent
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || ville.isEmpty() || num_tel == 0 || fidelite == 0 || cin == 0 || cheminImage.isEmpty() || isLabelEmpty(ui->imageLabel)) {
        QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                              QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
        return;
    }
    if (ville == "Choisir") {
        QMessageBox::critical(nullptr, QObject::tr("Champ manquant"),
                              QObject::tr("Veuillez choisir la ville."),
                              QMessageBox::Cancel);
        return;
    }
        QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                               QObject::tr("Le CIN doit contenir exactement 8 chiffres."), QMessageBox::Cancel);
        return;
    }
    if (ui->lineEdit_fidelite->text().toInt() <= 0) {
        QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                               QObject::tr("Fidélité incorrecte."), QMessageBox::Cancel);
        return;
    }
    if (ui->lineEdit_num_tel->text().length() != 8) {
        QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                               QObject::tr("Le numéro de téléphone doit contenir exactement 8 chiffres."), QMessageBox::Cancel);
        return;
    }
    if (!email.contains("@")) {
        QMessageBox::critical(nullptr, QObject::tr("Champ manquant ou invalide"),
                              QObject::tr("Veuillez entrer une adresse e-mail valide."),
                              QMessageBox::Cancel);
        return;
    }

    // Ajouter l'adhérent à la base de données
    bool test = A.ajouter();

    if (test) {
        historiqueAutomatique();
        ui->tableView_Adherent->setModel(Atmp.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué \n"
                                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        // Effacer les champs après l'ajout
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->lineEdit_email->clear();
        ui->comboBox_ville->setCurrentIndex(0); // Sélectionner l'élément par défaut
        ui->lineEdit_num_tel->clear();
        ui->lineEdit_fidelite->clear();
        ui->lineEdit_cin->clear();
        ui->lineEdit_cheminImage->clear(); // Effacer le chemin de l'image
        ui->imageLabel->clear();
        QPixmap imagePixmap("C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg");
        if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
            // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
            QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->imageLabel_2->setPixmap(scaledPixmap);
        } else {
            qDebug() << "Failed to load image from:" << "C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg";
        }

    } else {
        QMessageBox::critical(nullptr, QObject::tr("Échec de l'ajout"),
                              QObject::tr("L'ajout n'a pas été effectué. \n"
                                          "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}
*/
void DialogAdherent::on_pushButton_ajouterImage_clicked()
{
    // Logique pour récupérer et afficher l'image de l'adhérent
    QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }

    QString imagePath = fileName;
    qDebug() << "Image Path:" << imagePath;

    QPixmap imagePixmap(imagePath);
    if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
        // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
        QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(scaledPixmap);
        ui->imageLabel_2->clear();
    } else {
        qDebug() << "Failed to load image from:" << imagePath;
    }

    // Mettre à jour le chemin de l'image dans l'objet Adherent
    ui->lineEdit_cheminImage->setText(fileName); // Assurez-vous d'avoir un LineEdit nommé "lineEdit_cheminImage" pour stocker le chemin de l'image


    ui->imageLabel_2->clear();
}


void DialogAdherent::on_pushButton_supprimerAdherent_clicked()
{
    QString id_a = ui->lineEdit_id_a_supp->text();
    if (id_a.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                              QObject::tr("Veuillez entrer un ID."),
                              QMessageBox::Cancel);
        return;
    }

    Adherent adh;
    if (!adh.idExiste(id_a)) {
        QMessageBox::critical(nullptr, QObject::tr("ID inexistant"),
                              QObject::tr("ID inexistant."),
                              QMessageBox::Cancel);
        ui->lineEdit_id_a_supp->clear();
        return;
    }

    QMessageBox confirmationBox(QMessageBox::Question,
                                 tr("Confirmation"),
                                 tr("Êtes-vous sûr de vouloir supprimer cet adherent ?"),
                                 QMessageBox::Yes | QMessageBox::No);
    confirmationBox.setButtonText(QMessageBox::Yes, tr("Confirmer"));
    confirmationBox.setButtonText(QMessageBox::No, tr("Annuler"));

    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        bool test = Atmp.supprimer(id_a.toInt(),ui->label_id_employe_join->text().toInt());
        if (test) {

            historiqueAutomatique();
            ui->tableView_Adherent->setModel(Atmp.afficher());
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Suppression effectuée \n"
                                                 "Click Cancel to exit."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Suppression non effectuée \n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
        }
        ui->lineEdit_id_a_supp->clear();
    } else {
        // User clicked No, do nothing
    }
}






void DialogAdherent::on_pushButton_ok_modif_clicked()
{
    QString numeroString = ui->lineEdit_id_a_mod->text();

    Adherent adh;

    if (numeroString.isEmpty()) {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Veuillez entrer un ID."),
                                  QMessageBox::Cancel);
            return;
        }

         else if (adh.idExiste(ui->lineEdit_id_a_mod->text())==false) {
                QMessageBox::critical(nullptr, QObject::tr("ID inexistant"),
                                       QObject::tr("ID inexistant."),
                                       QMessageBox::Cancel);
                ui->lineEdit_id_a_mod->clear();

                return;
           }

    else
    {
        int id_a = ui->lineEdit_id_a_mod->text().toInt();
        const Adherent& A = Atmp.rechercher(id_a);


        if (A.Getid_a() != -1) {

            ui->lineEdit_nom_modif->setText(A.Getnom());
            ui->lineEdit_prenom_modiff->setText(A.Getprenom());
            ui->lineEdit_email_modif->setText(A.Getemail());

            ui->comboBox_ville_modif->setCurrentText(A.Getville());
            ui->lineEdit_num_tel_modif->setText(QString::number(A.Getnum_tel()));
            ui->lineEdit_fidelite_modif->setText(QString::number(A.Getfidelite()));
            ui->lineEdit_cin_modif->setText(QString::number(A.Getcin()));
            ui->lineEdit_cheminImage_modif->setText(A.Getimage_a());
/*
            QImage image(A.Getimage_a());
            if (image.isNull()) {
                QMessageBox::critical(this, "Erreur", "Impossible de charger l'image.");
                return;
            }

            // Afficher l'image dans imageLabel
            ui->imageLabel_modif->setPixmap(QPixmap::fromImage(image).scaledToWidth(ui->imageLabel_modif->width(), Qt::SmoothTransformation));
            */

            // Mettre à jour le chemin de l'image dans l'objet Adherent
            ui->lineEdit_cheminImage_modif->setText(A.Getimage_a()); // Assurez-vous d'avoir un LineEdit nommé "lineEdit_cheminImage" pour stocker le chemin de l'image



            QString imagePath = A.Getimage_a();
            qDebug() << "Image Path:" << imagePath;

            QPixmap imagePixmap(imagePath);
            if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
                // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
                QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_modif->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->imageLabel_modif->setPixmap(scaledPixmap);
                ui->imageLabel_modif_2->clear();
            } else {
                qDebug() << "Failed to load image from:" << imagePath;
            }





        } else {
            QMessageBox::warning(nullptr, QObject::tr("Attention"),
                                 QObject::tr("Aucun adhérent trouvé avec cet ID."), QMessageBox::Ok);
        }
    }


}

void DialogAdherent::on_pushButton_modifierAdherent_clicked()
{
    int clear=1;
    int id_a = ui->lineEdit_id_a_mod->text().toInt();
    const Adherent& A = Atmp.rechercher(id_a);
    QString numeroString = ui->lineEdit_id_a_mod->text();

    if (numeroString.isEmpty()) {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Veuillez entrer un ID."),
                                  QMessageBox::Cancel);
            return;
        }
    else if (A.Getid_a() != -1) {
        Adherent copie_adherent = A;
        QString nouveau_nom = ui->lineEdit_nom_modif->text();
        QString nouveau_prenom = ui->lineEdit_prenom_modiff->text();
        QString nouveau_email = ui->lineEdit_email_modif->text();
        QString nouvelle_ville = ui->comboBox_ville_modif->currentText();
        int nouveau_num_tel = ui->lineEdit_num_tel_modif->text().toInt();
        int nouveau_fidelite = ui->lineEdit_fidelite_modif->text().toInt();
        int nouveau_cin = ui->lineEdit_cin_modif->text().toInt();
        QString nouveau_image_a = ui->lineEdit_cheminImage_modif->text();






        if (nouveau_nom.isEmpty() || nouveau_prenom.isEmpty() || nouveau_email.isEmpty() || nouvelle_ville.isEmpty() || nouveau_num_tel==0 || nouveau_fidelite==0 || nouveau_cin==0 || nouveau_image_a.isEmpty() || isLabelEmpty(ui->imageLabel_modif)) {
                QMessageBox::critical(nullptr, QObject::tr("Champs manquants"),
                                      QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
                clear=0;
                return;
         }

        if (nouvelle_ville == "Choisir") {
               QMessageBox::critical(nullptr, QObject::tr("Champ manquants"),
                                      QObject::tr("Veuillez choisir la ville."),
                                      QMessageBox::Cancel);
               clear=0;

               return;
           }

        if (ui->lineEdit_fidelite_modif->text().toInt() <= 0) {
                QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                                       QObject::tr("Fidélité incorrecte."), QMessageBox::Cancel);
                return;
                  }




        if (ui->lineEdit_cin_modif->text().length() != 8) {
                QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                                       QObject::tr("Le CIN doit contenir exactement 8 chiffres."), QMessageBox::Cancel);
                clear=0;

                return;
                  }


        if (ui->lineEdit_num_tel_modif->text().length() != 8) {
                QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                                       QObject::tr("Le numéro Téléphone doit contenir exactement 8 chiffres."), QMessageBox::Cancel);
                clear=0;

                return;
                  }
        if (!nouveau_email.contains("@")) {
            QMessageBox::critical(nullptr, QObject::tr("Champ manquant ou invalide"),
                                  QObject::tr("Veuillez entrer une adresse e-mail valide."),
                                  QMessageBox::Cancel);
            clear=0;

            return;
        }


        copie_adherent.Setnom(nouveau_nom);
        copie_adherent.Setprenom(nouveau_prenom);
        copie_adherent.Setemail(nouveau_email);
        copie_adherent.Setville(nouvelle_ville);
        copie_adherent.Setnum_tel(nouveau_num_tel);
        copie_adherent.Setfidelite(nouveau_fidelite);
        copie_adherent.Setcin(nouveau_cin);
        copie_adherent.Setimage_a(nouveau_image_a);
        copie_adherent.setid_e(ui->label_id_employe_join_modif->text().toInt());



        bool modification_reussie = copie_adherent.modifier(ui->label_id_employe_join->text().toInt());
        if (modification_reussie) {
            QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Modification effectuée\n"
                                                                        "Click Cancel to exit."), QMessageBox::Cancel);
            historiqueAutomatique();
            ui->tableView_Adherent->setModel(Atmp.afficher());
            ui->lineEdit_nom_modif->clear();
            ui->lineEdit_prenom_modiff->clear();
            ui->lineEdit_email_modif->clear();
            ui->comboBox_ville_modif->setCurrentIndex(0); // Sélectionner l'élément par défaut
            ui->lineEdit_num_tel_modif->clear();
            ui->lineEdit_fidelite_modif->clear();
            ui->lineEdit_cin_modif->clear();
            ui->lineEdit_cheminImage_modif->clear();
            ui->imageLabel_modif->clear();
            QPixmap imagePixmap("C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg");
            if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
                // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
                QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_modif_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->imageLabel_modif_2->setPixmap(scaledPixmap);
            } else {
                qDebug() << "Failed to load image from:" << "C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg";
            }

        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Modification non effectuée.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Aucun adhérent trouvé avec ce numéro."), QMessageBox::Ok);
    }
    if(clear==1){ui->lineEdit_id_a_mod->clear();}

}


void DialogAdherent::on_pushButton_refresh_insertion_clicked()
{
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_email->clear();
    ui->comboBox_ville->setCurrentIndex(0); // Sélectionner l'élément par défaut
    ui->lineEdit_num_tel->clear();
    ui->lineEdit_fidelite->clear();
    ui->lineEdit_cin->clear();
    ui->imageLabel->clear();
    ui->lineEdit_cheminImage->clear();
    // Réinitialisez les étiquettes d'erreur à vide
    ui->label_erreur_nom->setText("");
    ui->label_erreur_prenom->setText("");
    ui->label_erreur_cin->setText("");
    ui->label_erreur_num_tel->setText("");
    ui->label_erreur_email->setText("");
    ui->label_erreur_fidelite->setText("");
    ui->label_erreur_image_a->setText("");
    ui->label_erreur_ville->setText("");

    QPixmap imagePixmap("C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg");
    if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
        // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
        QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel_2->setPixmap(scaledPixmap);
    } else {
        qDebug() << "Failed to load image from:" << "C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg";
    }


}

void DialogAdherent::on_pushButton_refresh_liste_clicked()
{
    Adherent adherentObj;
    ui->tableView_Adherent->setModel(adherentObj.afficher());

    // Désélectionner le radioButton_ordre_croissant
    ui->radioButton_ordre_croissant->setAutoExclusive(false); // Désactiver l'exclusivité automatique des boutons radio
    ui->radioButton_ordre_croissant->setChecked(false); // Désélectionner le radioButton_ordre_croissant
    ui->radioButton_ordre_croissant->setAutoExclusive(true); // Réactiver l'exclusivité automatique des boutons radio

    // Désélectionner le radioButton_ordre_decroissant
    ui->radioButton_ordre_decroissant->setAutoExclusive(false); // Désactiver l'exclusivité automatique des boutons radio
    ui->radioButton_ordre_decroissant->setChecked(false); // Désélectionner le radioButton_ordre_croissant
    ui->radioButton_ordre_decroissant->setAutoExclusive(true); // Réactiver l'exclusivité automatique des boutons radio


    // Réinitialiser comboBox_trie à "Choisir"
    ui->comboBox_trie->setCurrentIndex(0); // 0 est l'index de l'élément "Choisir"

    ui->label_67->clear();
    ui->label_67->setText("Vous pouvez visualiser les  <br>images des adhérents en<br>cliquantsur celles de la liste");
    ui->label_67->setAlignment(Qt::AlignCenter);

    ui->lineEdit_prenom_chercher->clear();
    ui->lineEdit_id_a_chercher->clear();


}


void DialogAdherent::on_pushButton_refresh_chercher_clicked()
{
    ui->lineEdit_id_a_chercher->clear();
    ui->lineEdit_prenom_chercher->clear();
}



void DialogAdherent::on_radioButton_ordre_croissant_clicked()
{
    Adherent A;
    if(ui->comboBox_trie->currentText() !="             Choisir")
    {
        QString ch;
        if(ui->comboBox_trie->currentText() =="ID"){ch="id_a";}
        if(ui->comboBox_trie->currentText() =="Nom"){ch="nom";}
        if(ui->comboBox_trie->currentText() =="Prénom"){ch="prenom";}
        if(ui->comboBox_trie->currentText() =="Email"){ch="email";}
        if(ui->comboBox_trie->currentText() =="Téléphone"){ch="num_tel";}
        if(ui->comboBox_trie->currentText() =="Ville"){ch="ville";}
        if(ui->comboBox_trie->currentText() =="Fidélité"){ch="fidelite";}
        if(ui->comboBox_trie->currentText() =="CIN"){ch="cin";}
        ui->tableView_Adherent->setModel(Adherent::trie("ASC",ch,ui->label_id_employe_join->text().toInt()));
        historiqueAutomatique();

    }
    else  ui->tableView_Adherent->setModel(A.afficher());
    if(ui->comboBox_trie->currentText() =="             Choisir")
    {
        // Désélectionner le radioButton_ordre_croissant
        ui->radioButton_ordre_croissant->setAutoExclusive(false); // Désactiver l'exclusivité automatique des boutons radio
        ui->radioButton_ordre_croissant->setChecked(false); // Désélectionner le radioButton_ordre_croissant
        ui->radioButton_ordre_croissant->setAutoExclusive(true); // Réactiver l'exclusivité automatique des boutons radio

    }
}

void DialogAdherent::on_radioButton_ordre_decroissant_clicked()
{
    Adherent A;
    if(ui->comboBox_trie->currentText() !="             Choisir")
    {


        QString ch;
        if(ui->comboBox_trie->currentText() =="ID"){ch="id_a";}
        if(ui->comboBox_trie->currentText() =="Nom"){ch="nom";}
        if(ui->comboBox_trie->currentText() =="Prénom"){ch="prenom";}
        if(ui->comboBox_trie->currentText() =="Email"){ch="email";}
        if(ui->comboBox_trie->currentText() =="Téléphone"){ch="num_tel";}
        if(ui->comboBox_trie->currentText() =="Ville"){ch="ville";}
        if(ui->comboBox_trie->currentText() =="Fidélité"){ch="fidelite";}
        if(ui->comboBox_trie->currentText() =="CIN"){ch="cin";}
        ui->tableView_Adherent->setModel(Adherent::trie("DESC",ch,ui->label_id_employe_join->text().toInt()));
        historiqueAutomatique();

    }
    else  ui->tableView_Adherent->setModel(A.afficher());
    if(ui->comboBox_trie->currentText() =="             Choisir")
    {
        // Désélectionner le radioButton_ordre_decroissant
        ui->radioButton_ordre_decroissant->setAutoExclusive(false); // Désactiver l'exclusivité automatique des boutons radio
        ui->radioButton_ordre_decroissant->setChecked(false); // Désélectionner le radioButton_ordre_croissant
        ui->radioButton_ordre_decroissant->setAutoExclusive(true); // Réactiver l'exclusivité automatique des boutons radio

    }
}





/* avec segments
void DialogAdherent::on_pushButton_statistiques_clicked()
{
    // Créer un modèle de données pour le graphique à barres
    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();

    // Obtenir les données de répartition par niveau à partir de votre base de données
    QHash<QString, int> distribution = Adherent::statistiques();

    // Calculer le nombre total d'adhérents
    int total = 0;
    for (int count : distribution) {
        total += count;
    }

    // Ajouter les barres pour chaque niveau avec le pourcentage correspondant
    for (const QString &level : distribution.keys()) {
        qreal percentage = (qreal)distribution.value(level) / total * 100.0;
        QtCharts::QBarSet *barSet = new QtCharts::QBarSet(level);
        *barSet << percentage;
        series->append(barSet);
    }

    // Créer un graphique à barres et configurer les options
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par Niveau");

    // Créer un axe des Y pour afficher les pourcentages
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setLabelFormat("%.2f%%"); // Format pour afficher les pourcentages avec deux décimales
    chart->setAxisY(axisY, series);

    // Créer une vue de graphique pour afficher le graphique à barres
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Afficher la vue du graphique dans une nouvelle fenêtre
    QMainWindow *chartWindow = new QMainWindow(this);
    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(800, 600);
    chartWindow->show();
}
*/
void DialogAdherent::on_pushButton_statistiques_clicked()
{
    // Créer un modèle de données pour le graphique circulaire
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

    // Ajouter des tranches (slices) pour chaque groupe d'âge
    // Vous devez obtenir les données de votre base de données ici
    QHash<QString, int> Distribution = Adherent::statistiques(ui->label_id_employe_join->text().toInt());

    int total = 0;
    for (const QString &Group : Distribution.keys()) {
        total += Distribution.value(Group);
    }


    // Définir une palette de couleurs pour les tranches
    // Assurez-vous d'avoir suffisamment de couleurs pour chaque tranche
    QStringList colors = {"#FF5733", "#FFBD33", "#33FF57", "#337AFF", "#AB33FF", "#FF33A8"};

    int colorIndex = 0;
    for (const QString &Group : Distribution.keys()) {
        QString label1 = Group;
        int value = Distribution.value(Group);
        double percentage = (value * 100.0) / total;
        QString label = QString("%1 (%2%)").arg(Group).arg(percentage, 0, 'f', 1);
        //QString label1 = Group;
        QString label2 = QString("(%1%)").arg(percentage, 0, 'f', 1);
        // Ajouter la tranche (slice) avec le pourcentage comme étiquette et une couleur différente
        QtCharts::QPieSlice *slice = series->append(label, value);
        slice->setLabelVisible();
        slice->setLabelColor(QColor(Qt::black)); // Couleur du texte à l'intérieur du cercle
        slice->setLabelFont(QFont("Arial", 10)); // Police du texte à l'intérieur du cercle
        slice->setLabelPosition(QtCharts::QPieSlice::LabelInsideHorizontal); // Position du texte à l'intérieur du cercle
        slice->setColor(QColor(colors[colorIndex])); // Couleur de la tranche
        slice->setExploded(); // Séparer légèrement la tranche
        slice->setExplodeDistanceFactor(0.01); // Définir la distance de séparation

        colorIndex = (colorIndex + 1) % colors.size();
    }

    // Créer un graphique circulaire et configurer-le
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);

    // Définir le titre en gras et changer sa taille
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(12); // Changer la taille du titre selon vos besoins
    chart->setTitleFont(titleFont);

    chart->setTitle("Répartition par Niveau");

    // Créer une vue de graphique pour afficher le graphique circulaire
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Afficher la vue du graphique dans une nouvelle fenêtre
    QMainWindow *chartWindow = new QMainWindow(this);
    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(800, 600);
    chartWindow->show();
    historiqueAutomatique();

}

void DialogAdherent::on_pushButton_retour_clicked()
{
    int nombreEntier = ui->label_id_employe_join->text().toInt(); // Convertir en entier directement
    MainWindow *M = new MainWindow();
    M->setResultRA(nombreEntier); // Appeler une fonction de la deuxième interface pour définir le résultat

    M->show();
    //this->close();
    hide();
}

void DialogAdherent::on_pushButton_logout_Adherent_clicked()
{
    DialogLogin *dLogin = new DialogLogin();
    dLogin->show();
    hide();

}
                       //[EN MODE PORTRAIT]
void DialogAdherent::on_pushButton_pdf_clicked()
{
    QString cinString = ui->lineEdit_cin_pdf->text();
        int cin = cinString.toInt();

        // Si le champ CIN est vide, afficher le PDF de toute la liste des adhérents
        if (cinString.isEmpty()) {
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
            QString htmlContent = "<html><head><style>"
                                  "h2 { text-align: center; color: #333; font-family: Arial, sans-serif; font-size: 30px; }"
                                  "table { border-collapse: collapse; }"
                                  "th, td {  text-align: center; border: 1px solid #333; }"
                                  "th { background-color: #f2f2f2; color: #333; font-family: Arial, sans-serif; font-weight: bold; }"
                                  "td { font-family: Arial, sans-serif; font-size: 16px; }"
                                  "</style></head><body>";

            // Titre centré avec l'image
            htmlContent += "<h2><img src='C:\\Users\\Admin\\Desktop\\Agence_d_assurance\\img\\IMG_4507 (1).jpg' width='100' height='100'>"
                           "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                           "Liste des adhérents</h2>";

            // Tableau centré
            htmlContent += "<div ><table border='1'>";

            // En-têtes de colonnes
            htmlContent += "<tr><th>ID</th><th>Nom</th><th>Prénom</th><th>Email</th><th>Ville</th><th>Téléphone</th><th>Fidélité</th><th>CIN</th><th>Image</th></tr>";

            // Récupérer les données de la table view
            QTableView *tableView_Adherent = ui->tableView_Adherent;
            QAbstractItemModel *model = tableView_Adherent->model();
            int rows = model->rowCount();
            int columns = model->columnCount();

            // Insérer les données de la table view dans le tableau HTML
            for (int row = 0; row < rows; ++row) {
                htmlContent += "<tr>";
                for (int column = 0; column < columns; ++column) {
                    QModelIndex index = model->index(row, column);
                    QString data = model->data(index).toString();

                    // Si la colonne est la colonne de fidélité
                    if (column == 6) {
                        int fidelite = data.toInt();
                        QString fideliteText;

                        // Déterminer le statut de fidélité en fonction de la valeur
                        if (fidelite < 2)
                            fideliteText = "Silver";
                        else if (fidelite >= 2 && fidelite < 4)
                            fideliteText = "Premium";
                        else if (fidelite >= 4 && fidelite < 6)
                            fideliteText = "Gold";
                        else
                            fideliteText = "VIP";

                        htmlContent += "<td>" + fideliteText + "</td>";
                    }
                    // Si la colonne est la colonne de l'image
                    else if (column == 8) {
                        QString imagePath = data; // Chemin d'accès de l'image
                        QImage image(imagePath);
                        int width = image.width();
                        int height = image.height();

                        if (width >= height) {
                            width = (width * 50) / height;
                            height = 50;
                        } else {
                            height = (height * 50) / width;
                            width = 50;
                        }

                        htmlContent += "<td><img src='" + imagePath + "' width='" + QString::number(width) + "' height='" + QString::number(height) + "'></td>";
                    }
                    else {
                        htmlContent += "<td>" + data + "</td>";
                    }
                }
                htmlContent += "</tr>";
            }

            htmlContent += "</table></div></body></html>"; // Fermeture du tableau et centrage

            doc.setHtml(htmlContent);

            // Imprimer le document dans le fichier PDF
            QPainter painter;
            if (!painter.begin(&printer)) {
                qDebug() << "Failed to open printer";
                return;
            }

            doc.drawContents(&painter);
            painter.end();

            QMessageBox::information(this, "PDF Exporté", "La liste des adhérents a été exportée dans un fichier PDF.");
            //*****************************************************************************************************************************
                        // Récupérer l'ID de l'employé
                       int idEmploye = ui->label_id_employe_join->text().toInt();

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
            Adherent::historique(8,"","",nomEmploye);
            historiqueAutomatique();
            // Effacer le contenu du champ CIN
            ui->lineEdit_cin_pdf->clear();

            // Terminer la fonction
            return;
        }

    // Récupérer les détails du adhérent à partir de la classe Adherent
    Adherent adherent = Adherent::getAdherentDetails(cin, ui->label_id_employe_join->text().toInt());

    // Si le CIN n'existe pas, afficher un message d'avertissement
    if (!Adherent::idExiste(QString::number(adherent.Getid_a()))) {
        QMessageBox::warning(this, "Adhérent non trouvé", "Aucun adhérent trouvé avec ce CIN.");
        ui->lineEdit_cin_pdf->clear();
        return;
    }

    // Générer le contenu du PDF avec les détails du adhérent spécifié
    QString pdfContent = "                                        FICHE D'UN ADHERENT \n\n\n\n\n\n\n\n";

    QString fid;
    // Logique pour déterminer le statut fidélité
    if (adherent.Getfidelite() == 1) {
        fid = "Silver";
    } else if (adherent.Getfidelite() == 2) {
        fid = "Premium";
    } else if (adherent.Getfidelite() == 3) {
        fid = "Gold";
    } else {
        fid = "VIP";
    }

    pdfContent = " \n\n\n\n                                                        Fiche d'un adhérent\n\n\n\n"
                     " \tCIN :                  " + QString::number(cin) + "\n\n"
                     " \tNom :                  " + adherent.Getnom() + "\n\n"
                     " \tPrénom :               " + adherent.Getprenom() + "\n\n"
                     " \tEmail :                " + adherent.Getemail() + "\n\n"
                     " \tVille :                " + adherent.Getville() + "\n\n"
                     " \tTéléphone :            " + QString::number(adherent.Getnum_tel()) + "\n\n"
                     " \tStatut :               " + fid + "\n\n"
                     " \tPhoto :                "
                     "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
                     "\n\n                         ASSURITE - El Aouina, 36 Av. Khaled Ibn El Walid, Tunis 2045 - 78 450 200\n"
                     "                                        RC B2411262004, Mat. Fiscal: 0868024D/A/M/0000 "   ;


    // Génération du reste du PDF et sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", QDir::homePath(), "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return;

    QPdfWriter pdfWriter(filePath);
    QPainter painter(&pdfWriter);
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);
    QTextDocument textDocument;
    textDocument.setDefaultFont(font);
    textDocument.setPlainText(pdfContent);
    textDocument.drawContents(&painter);
    QImage image("C:/Users/Admin/Desktop/Agence_d_assurance/img/IMG_4507 (1).jpg"); // Chemin vers votre image
    QSize newSize(1500, 1500); // Nouvelle taille de l'image
    QImage scaledImage = image.scaled(newSize, Qt::KeepAspectRatio); // Redimensionner l'image en conservant le ratio

    QRectF targetRect(0, 0, newSize.width(), newSize.height()); // Rectangle cible pour l'image
    QRectF sourceRect = QRectF(0, 0, scaledImage.width(), scaledImage.height());
    painter.drawImage(targetRect, scaledImage, sourceRect);
   /* QImage imageA(adherent.Getimage_a()); // Chemin vers votre image
    QSize newSizeA(5334, 3000); // Nouvelle taille de l'image
    QImage scaledImageA = imageA.scaled(newSizeA, Qt::KeepAspectRatio); // Redimensionner l'image en conservant le ratio

    QRectF targetRectA(2100, 5500, newSizeA.width(), newSizeA.height()); // Rectangle cible pour l'image
    QRectF sourceRectA = QRectF(0, 0, scaledImageA.width(), scaledImageA.height());
    painter.drawImage(targetRectA, scaledImageA, sourceRectA);*/
    QImage imageA(adherent.Getimage_a()); // Chemin vers votre image
    QSize newSizeA(4000, 4000); // Nouvelle taille de l'image
    QImage scaledImageA = imageA.scaled(newSizeA, Qt::KeepAspectRatio); // Redimensionner l'image en conservant le ratio

    // Calculer la position pour centrer l'image dans l'espace de dimension 4000x4000
    int posX = (3000);
    int posY = (6000 );

    QRectF targetRectA(posX, posY, scaledImageA.width(), scaledImageA.height()); // Rectangle cible pour l'image
    QRectF sourceRectA = QRectF(0, 0, scaledImageA.width(), scaledImageA.height());
    painter.drawImage(targetRectA, scaledImageA, sourceRectA);


    QMessageBox::information(this, "PDF Exporté", "La fiche de l'adhérent a été exportée dans un fichier PDF.");

    // Effacer le contenu du champ CIN
    ui->lineEdit_cin_pdf->clear();
    historiqueAutomatique();

}
/*                        //[EN MODE PAYSAGE]

void DialogAdherent::on_pushButton_pdf_clicked()
{
    QString cinString = ui->lineEdit_cin_pdf->text();
    int cin = cinString.toInt();

    // Si le champ CIN est vide, afficher le PDF de toute la liste des adhérents
    if (cinString.isEmpty()) {
        // Sélectionner le fichier de destination
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", "*.pdf");
        if (fileName.isEmpty())
            return;

        // Créer un document PDF
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);

        // Définir l'orientation du papier en mode paysage
        printer.setOrientation(QPrinter::Landscape);

        // Définir une taille de papier personnalisée
        QSizeF customPageSize(800, 1100); // Largeur x Hauteur (en points)
        printer.setPageSize(QPageSize(customPageSize, QPageSize::Point));

        printer.setOutputFileName(fileName);

        // Créer un document texte pour le PDF
        QTextDocument doc;

        // Ajouter le contenu HTML
        QString htmlContent = "<html><head><style>"
                              "h2 { text-align: center; color: #333; font-family: Arial, sans-serif; font-size: 30px; }"
                              "table { border-collapse: collapse; }"
                              "th, td {  text-align: center; border: 1px solid #333; }"
                              "th { background-color: #f2f2f2; color: #333; font-family: Arial, sans-serif; font-weight: bold; }"
                              "td { font-family: Arial, sans-serif; font-size: 16px; }"
                              "</style></head><body>";

        // Titre centré avec l'image
        htmlContent += "<h2><img src='C:\\Users\\Admin\\Desktop\\Agence_d_assurance\\img\\IMG_4507 (1).jpg' width='100' height='100'>"
                       "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                       "Liste des adhérents</h2>";

        // Tableau centré
        htmlContent += "<div ><table border='1'>";

        // En-têtes de colonnes
        htmlContent += "<tr><th>ID</th><th>Nom</th><th>Prénom</th><th>Email</th><th>Ville</th><th>Téléphone</th><th>Fidélité</th><th>CIN</th><th>Image</th></tr>";

        // Récupérer les données de la table view
        QTableView *tableView_Adherent = ui->tableView_Adherent;
        QAbstractItemModel *model = tableView_Adherent->model();
        int rows = model->rowCount();
        int columns = model->columnCount();

        // Insérer les données de la table view dans le tableau HTML
        for (int row = 0; row < rows; ++row) {
            htmlContent += "<tr>";
            for (int column = 0; column < columns; ++column) {
                QModelIndex index = model->index(row, column);
                QString data = model->data(index).toString();

                // Si la colonne est la colonne de fidélité
                if (column == 6) {
                    int fidelite = data.toInt();
                    QString fideliteText;

                    // Déterminer le statut de fidélité en fonction de la valeur
                    if (fidelite < 2)
                        fideliteText = "Silver";
                    else if (fidelite >= 2 && fidelite < 4)
                        fideliteText = "Premium";
                    else if (fidelite >= 4 && fidelite < 6)
                        fideliteText = "Gold";
                    else
                        fideliteText = "VIP";

                    htmlContent += "<td>" + fideliteText + "</td>";
                }
                // Si la colonne est la colonne de l'image
                else if (column == 8) {
                    QString imagePath = data; // Chemin d'accès de l'image
                    QImage image(imagePath);
                    int width = image.width();
                    int height = image.height();

                    if (width >= height) {
                        width = (width * 50) / height;
                        height = 50;
                    } else {
                        height = (height * 50) / width;
                        width = 50;
                    }

                    htmlContent += "<td><img src='" + imagePath + "' width='" + QString::number(width) + "' height='" + QString::number(height) + "'></td>";
                }
                else {
                    htmlContent += "<td>" + data + "</td>";
                }
            }
            htmlContent += "</tr>";
        }

        htmlContent += "</table></div></body></html>"; // Fermeture du tableau et centrage

        doc.setHtml(htmlContent);

        // Imprimer le document dans le fichier PDF
        QPainter painter;
        if (!painter.begin(&printer)) {
            qDebug() << "Failed to open printer";
            return;
        }

        doc.drawContents(&painter);
        painter.end();

        QMessageBox::information(this, "PDF Exporté", "La liste des adhérents a été exportée dans un fichier PDF.");

        Adherent::historique(8,"","");
        historiqueAutomatique();
        // Effacer le contenu du champ CIN
        ui->lineEdit_cin_pdf->clear();

        // Terminer la fonction
        return;
    }

    // Récupérer les détails du adhérent à partir de la classe Adherent
    Adherent adherent = Adherent::getAdherentDetails(cin);

    // Si le CIN n'existe pas, afficher un message d'avertissement
    if (!Adherent::idExiste(QString::number(adherent.Getid_a()))) {
        QMessageBox::warning(this, "Adhérent non trouvé", "Aucun adhérent trouvé avec ce CIN.");
        ui->lineEdit_cin_pdf->clear();
        return;
    }

    // Générer le contenu du PDF avec les détails du adhérent spécifié
    QString pdfContent = "                                        FICHE D'UN ADHERENT \n\n\n\n\n\n\n\n";

    QString fid;
    // Logique pour déterminer le statut fidélité
    if (adherent.Getfidelite() == 1) {
        fid = "Silver";
    } else if (adherent.Getfidelite() == 2) {
        fid = "Premium";
    } else if (adherent.Getfidelite() == 3) {
        fid = "Gold";
    } else {
        fid = "VIP";
    }

    pdfContent = " \n\n\n\n                                                        Fiche d'un adhérent\n\n\n\n"
                     " \tCIN :                  " + QString::number(cin) + "\n\n"
                     " \tNom :                  " + adherent.Getnom() + "\n\n"
                     " \tPrénom :               " + adherent.Getprenom() + "\n\n"
                     " \tEmail :                " + adherent.Getemail() + "\n\n"
                     " \tVille :                " + adherent.Getville() + "\n\n"
                     " \tTéléphone :            " + QString::number(adherent.Getnum_tel()) + "\n\n"
                     " \tStatut :               " + fid + "\n\n"
                     " \tPhoto :                "
                     "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
                     "\n\n                         ASSURITE - El Aouina, 36 Av. Khaled Ibn El Walid, Tunis 2045 - 78 450 200\n"
                     "                                        RC B2411262004, Mat. Fiscal: 0868024D/A/M/0000 "   ;


    // Génération du reste du PDF et sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", QDir::homePath(), "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return;

    QPdfWriter pdfWriter(filePath);
    QPainter painter(&pdfWriter);
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);
    QTextDocument textDocument;
    textDocument.setDefaultFont(font);
    textDocument.setPlainText(pdfContent);
    textDocument.drawContents(&painter);
    QImage image("C:/Users/Admin/Desktop/Agence_d_assurance/img/IMG_4507 (1).jpg"); // Chemin vers votre image
    QSize newSize(1500, 1500); // Nouvelle taille de l'image
    QImage scaledImage = image.scaled(newSize, Qt::KeepAspectRatio); // Redimensionner l'image en conservant le ratio

    QRectF targetRect(0, 0, newSize.width(), newSize.height()); // Rectangle cible pour l'image
    QRectF sourceRect = QRectF(0, 0, scaledImage.width(), scaledImage.height());
    painter.drawImage(targetRect, scaledImage, sourceRect);
    QImage imageA(adherent.Getimage_a()); // Chemin vers votre image
    QSize newSizeA(5334, 3000); // Nouvelle taille de l'image
    QImage scaledImageA = imageA.scaled(newSizeA, Qt::KeepAspectRatio); // Redimensionner l'image en conservant le ratio

    QRectF targetRectA(2100, 5500, newSizeA.width(), newSizeA.height()); // Rectangle cible pour l'image
    QRectF sourceRectA = QRectF(0, 0, scaledImageA.width(), scaledImageA.height());
    painter.drawImage(targetRectA, scaledImageA, sourceRectA);

    QMessageBox::information(this, "PDF Exporté", "La fiche de l'adhérent a été exportée dans un fichier PDF.");

    // Effacer le contenu du champ CIN
    ui->lineEdit_cin_pdf->clear();
    historiqueAutomatique();

}
*/
#include <QTimer>

void DialogAdherent::on_tableView_Adherent_clicked(const QModelIndex &index)
{
    if (index.isValid() && index.column() == 0) {
        QString id_a = index.data().toString();
        ui->lineEdit_id_a_supp->setText(id_a);
    }

    if (index.isValid() && index.column() == 7) {
        QString cin = index.data().toString();
        ui->lineEdit_cin_pdf->setText(cin);
    }

    if (index.isValid()) {
        qDebug() << "Row:" << index.row() << "Column:" << index.column() << "Data:" << index.data().toString();

        if (index.column() == 8 || index.column() == 1 || index.column() == 2 || index.column() == 0 || index.column() == 3 || index.column() == 4 || index.column() == 5 || index.column() == 6 || index.column() == 7) { // Vérifiez si la colonne est 8, 1 ou 2
            QString imagePath = index.sibling(index.row(), 8).data().toString(); // Obtenez le chemin de la colonne 8 pour la même ligne
            qDebug() << "Image Path:" << imagePath;
            ui->label_67->setAlignment(Qt::AlignCenter);

            QPixmap imagePixmap(imagePath);
            if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
                // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
                QPixmap scaledPixmap = imagePixmap.scaled(ui->label_67->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->label_67->setPixmap(scaledPixmap);

                // Définir un timer pour masquer l'image après 5 secondes
                QTimer::singleShot(3000, [=]() {
                    ui->label_67->clear();
                    ui->label_67->setText("Vous pouvez visualiser les  <br>images des adhérents en<br>cliquantsur celles de la liste");
                    ui->label_67->setAlignment(Qt::AlignCenter);
                });
            } else {
                qDebug() << "Failed to load image from:" << imagePath;
            }
        }
    }
}

/*
void DialogAdherent::on_tableView_Adherent_clicked(const QModelIndex &index)
{
    if (index.isValid() && index.column() == 0) {

        QString id_a = index.data().toString();
        ui->lineEdit_id_a_supp->setText(id_a);

    }
    if (index.isValid() && index.column() == 7) {

        QString cin = index.data().toString();
        ui->lineEdit_cin_pdf->setText(cin);

    }

    if (index.isValid()) {
        qDebug() << "Row:" << index.row() << "Column:" << index.column() << "Data:" << index.data().toString();

        if (index.column() == 8) { // Assurez-vous que l'index de la colonne est correct
            QString imagePath = index.data().toString();
            qDebug() << "Image Path:" << imagePath;
            ui->label_67->setAlignment(Qt::AlignCenter);

            QPixmap imagePixmap(imagePath);
            if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
                // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
                QPixmap scaledPixmap = imagePixmap.scaled(ui->label_67->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->label_67->setPixmap(scaledPixmap);
            } else {
                qDebug() << "Failed to load image from:" << imagePath;
            }
        }
    }

}*/

void DialogAdherent::on_pushButton_ajouterImage_modif_clicked()
{
    // Logique pour récupérer et afficher l'image de l'adhérent
    QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }

    QString imagePath = fileName;
    qDebug() << "Image Path:" << imagePath;

    QPixmap imagePixmap(imagePath);
    if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
        // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
        QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_modif->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel_modif->setPixmap(scaledPixmap);
        ui->imageLabel_modif_2->clear();
    } else {
        qDebug() << "Failed to load image from:" << imagePath;
    }

    // Mettre à jour le chemin de l'image dans l'objet Adherent
    ui->lineEdit_cheminImage_modif->setText(fileName); // Assurez-vous d'avoir un LineEdit nommé "lineEdit_cheminImage" pour stocker le chemin de l'image

}

void DialogAdherent::on_pushButton_refresh_modification_clicked()
{
    ui->lineEdit_nom_modif->clear();
    ui->lineEdit_prenom_modiff->clear();
    ui->lineEdit_email_modif->clear();
    ui->comboBox_ville_modif->setCurrentIndex(0); // Sélectionner l'élément par défaut
    ui->lineEdit_num_tel_modif->clear();
    ui->lineEdit_fidelite_modif->clear();
    ui->lineEdit_cin_modif->clear();
    ui->imageLabel_modif->clear();
    ui->lineEdit_cheminImage_modif->clear();
    ui->lineEdit_id_a_mod->clear();
    QPixmap imagePixmap("C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg");
    if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
        // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
        QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_modif_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel_modif_2->setPixmap(scaledPixmap);
    } else {
        qDebug() << "Failed to load image from:" << "C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg";
    }

}
void DialogAdherent::start_camera()
{
    if (ui->lineEdit_nom->text().isEmpty() )
    {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Tu doix saisir le nom d'abord "),
                                  QMessageBox::Cancel);
            return;
    }
    if (ui->lineEdit_prenom->text().isEmpty() )
    {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Tu doix saisir le prénom d'abord "),
                                  QMessageBox::Cancel);
            return;
    }
    if (ui->lineEdit_cin->text().isEmpty() )
    {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Tu doix saisir le CIN d'abord "),
                                  QMessageBox::Cancel);
            return;
    }
    if (ui->lineEdit_cin->text().length()!=8 )
    {
        QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                               QObject::tr("Le CIN doit contenir exactement 8 chiffres."), QMessageBox::Cancel);
        return;
            return;
    }

    M_Camera->start();

    ui->widget_Camera_View->setVisible(true);


}

void DialogAdherent::stop_camera()
{
    M_Camera->stop();
    ui->widget_Camera_View->setVisible(false);
}

void DialogAdherent::imageCaptured(int reqid, const QImage &img)
{


    Q_UNUSED(reqid);
    ImageFromCamera=img.scaled(ui->widget_Camera_View->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    Counter=Counter+1;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Chemin du bureau
    //QString ImagePath = desktopPath + "/Agence_d_assurance/Images_adherents/Image_" + QString::number(Counter) + ".bmp"; // Chemin complet avec le nom du fichier
    QString ImagePath = desktopPath + "/Agence_d_assurance/Images_adherents/Image_" +ui->lineEdit_prenom->text()+"_"+ui->lineEdit_nom->text()+"_"+ui->lineEdit_cin->text() + ".bmp"; // Chemin complet avec le nom du fichier
    QPixmap::fromImage(ImageFromCamera).save(ImagePath);

    QSound::play("C:/Users/Admin/Desktop/TesterTaswira/camera-shutter-click-01.wav"); // Ajout de cette ligne pour jouer le son



}

void DialogAdherent::on_pushButton_Start_Camera_clicked()
{
    start_camera();
    //ui->imageLabel_2->clear();
}

void DialogAdherent::on_pushButton_Image_Capture_clicked()
{
    M_Camera_Image->capture();

}

void DialogAdherent::on_pushButton_Stop_Camera_clicked()
{
    stop_camera();
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Chemin du bureau
    QString fileName = desktopPath + "/Agence_d_assurance/Images_adherents/Image_" +ui->lineEdit_prenom->text()+"_"+ui->lineEdit_nom->text()+"_"+ui->lineEdit_cin->text() + ".bmp"; // Chemin complet avec le nom du fichier

    // Logique pour récupérer et afficher l'image de l'adhérent

    QString imagePath = fileName;
    qDebug() << "Image Path:" << imagePath;

    QPixmap imagePixmap(imagePath);
    if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
        // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
        QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(scaledPixmap);
        ui->imageLabel_2->clear();
    } else {
        qDebug() << "Failed to load image from:" << imagePath;
    }

    // Mettre à jour le chemin de l'image dans l'objet Adherent
    if(!isLabelEmpty(ui->imageLabel))
    {
        ui->lineEdit_cheminImage->setText(fileName); // Assurez-vous d'avoir un LineEdit nommé "lineEdit_cheminImage" pour stocker le chemin de l'image
    }

    ui->imageLabel_2->clear();
    if(ui->lineEdit_cheminImage->text()=="")
    {
        QPixmap imagePixmap("C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg");
        if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
            // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
            QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->imageLabel_2->setPixmap(scaledPixmap);
        } else {
            qDebug() << "Failed to load image from:" << "C:/Users/Admin/Desktop/Agence_d_assurance/img/prof.jpg";
        }
    }

}
void DialogAdherent::start_camera_modif()
{
    if (ui->lineEdit_nom_modif->text().isEmpty() )
    {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Tu doix saisir le nom d'abord "),
                                  QMessageBox::Cancel);
            return;
    }
    if (ui->lineEdit_prenom_modiff->text().isEmpty() )
    {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Tu doix saisir le prénom d'abord "),
                                  QMessageBox::Cancel);
            return;
    }
    if (ui->lineEdit_cin_modif->text().isEmpty() )
    {
            QMessageBox::critical(nullptr, QObject::tr("Champ vide"),
                                  QObject::tr("Tu doix saisir le CIN d'abord "),
                                  QMessageBox::Cancel);
            return;
    }
    if (ui->lineEdit_cin_modif->text().length()!=8 )
    {
        QMessageBox::critical(nullptr, QObject::tr("Format invalide"),
                               QObject::tr("Le CIN doit contenir exactement 8 chiffres."), QMessageBox::Cancel);
        return;
    }

    M_Camera_modif->start();

    ui->widget_Camera_View_modif->setVisible(true);


}
void DialogAdherent::stop_camera_modif()
{
    M_Camera_modif->stop();
    ui->widget_Camera_View_modif->setVisible(false);

}
void DialogAdherent::on_pushButton_Start_Camera_modif_clicked()
{
    start_camera_modif();
}
void DialogAdherent::on_pushButton_Image_Capture_modif_clicked()
{
    M_Camera_Image_modif->capture();
}
void DialogAdherent::on_pushButton_Stop_Camera_modif_clicked()
{
    stop_camera_modif();
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Chemin du bureau
    QString fileName = desktopPath + "/Agence_d_assurance/Images_adherents/Image_" +ui->lineEdit_prenom_modiff->text()+"_"+ui->lineEdit_nom_modif->text()+"_"+ui->lineEdit_cin_modif->text() + ".bmp"; // Chemin complet avec le nom du fichier

    // Logique pour récupérer et afficher l'image de l'adhérent

    QString imagePath = fileName;
    qDebug() << "Image Path:" << imagePath;

    QPixmap imagePixmap(imagePath);
    if (!imagePixmap.isNull()) { // Vérifiez si l'image est valide
        // Redimensionner l'image pour qu'elle s'adapte aux dimensions de label_67
        QPixmap scaledPixmap = imagePixmap.scaled(ui->imageLabel_modif->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel_modif->setPixmap(scaledPixmap);
        ui->imageLabel_modif_2->clear();
    } else {
        qDebug() << "Failed to load image from:" << imagePath;
    }

    // Mettre à jour le chemin de l'image dans l'objet Adherent
    if(!isLabelEmpty(ui->imageLabel_modif))
    {
        ui->lineEdit_cheminImage_modif->setText(fileName); // Assurez-vous d'avoir un LineEdit nommé "lineEdit_cheminImage" pour stocker le chemin de l'image
    }


    ui->imageLabel_modif_2->clear();
}
void DialogAdherent::imageCaptured_modif(int reqid, const QImage &img )
{
    Q_UNUSED(reqid);
    ImageFromCamera_modif=img.scaled(ui->widget_Camera_View_modif->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    Counter_modif=Counter_modif+1;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Chemin du bureau
    //QString ImagePath = desktopPath + "/Agence_d_assurance/Images_adherents/Image_" + QString::number(Counter) + ".bmp"; // Chemin complet avec le nom du fichier
    QString ImagePath = desktopPath + "/Agence_d_assurance/Images_adherents/Image_" +ui->lineEdit_prenom_modiff->text()+"_"+ui->lineEdit_nom_modif->text()+"_"+ui->lineEdit_cin_modif->text() + ".bmp"; // Chemin complet avec le nom du fichier
    QPixmap::fromImage(ImageFromCamera_modif).save(ImagePath);

    QSound::play("C:/Users/Admin/Desktop/TesterTaswira/camera-shutter-click-01.wav"); // Ajout de cette ligne pour jouer le son

}

void DialogAdherent::on_pushButton_refresh_pdf_clicked()
{
    ui->lineEdit_cin_pdf->clear();
}


void DialogAdherent::on_pushButton_refresh_id_a_supp_clicked()
{
    ui->lineEdit_id_a_supp->clear();
}
void DialogAdherent::setResult2(int result)
{
    ui->label_id_employe->setText(QString::number(result)); // Afficher le résultat dans le Label
    ui->label_id_employe_join->setText(QString::number(result)); // Afficher le résultat dans le Label
    ui->label_id_employe_join_modif->setText(QString::number(result)); // Afficher le résultat dans le Label
}
