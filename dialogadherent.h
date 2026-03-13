#ifndef DIALOGADHERENT_H
#define DIALOGADHERENT_H

#include <QDialog>
#include "adherent.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include "mainwindow.h"
namespace Ui {
class DialogAdherent;
}

class DialogAdherent : public QDialog
{
   Q_OBJECT

public:
   explicit DialogAdherent(QWidget *parent = nullptr);
   ~DialogAdherent();
    void rechercheAutomatique();
    void setResult2(int result); //2----------------------------------------------


protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void onDialogShown();

private slots:
   void on_pushButton_ajouterAdherent_clicked();

   void on_pushButton_supprimerAdherent_clicked();

   void on_pushButton_ok_modif_clicked();

   void on_pushButton_modifierAdherent_clicked();

   void on_pushButton_refresh_insertion_clicked();

   void on_pushButton_refresh_liste_clicked();

   //void on_pushButton_32_clicked();

   void on_pushButton_refresh_chercher_clicked();

   //void on_pushButton_25_clicked();

   void on_pushButton_chercher_clicked();

   void on_radioButton_ordre_croissant_clicked();

   void on_radioButton_ordre_decroissant_clicked();

   void on_pushButton_31_clicked();

   void on_pushButton_pdf_clicked();

   void on_pushButton_statistiques_clicked();

   void on_pushButton_retour_clicked();


   void on_pushButton_logout_Adherent_clicked();

   //void on_pb_historique_adherent_clicked();

   void historiqueAutomatique();
   void on_pushButton_ajouterImage_clicked();

   void on_tableView_Adherent_clicked(const QModelIndex &index);


   void on_pushButton_ajouterImage_modif_clicked();

   void on_pushButton_refresh_modification_clicked();

   void imageCaptured(int reqid, const QImage &img );
   void imageCaptured_modif(int reqid, const QImage &img );


   void on_pushButton_Start_Camera_clicked();
   void on_pushButton_Image_Capture_clicked();
   void on_pushButton_Stop_Camera_clicked();

   void on_pushButton_Start_Camera_modif_clicked();
   void on_pushButton_Image_Capture_modif_clicked();
   void on_pushButton_Stop_Camera_modif_clicked();

   void on_pushButton_pdf_2_clicked();

   void on_pushButton_refresh_pdf_clicked();

   void on_pushButton_supprimerHistorique_clicked();

   void on_pushButton_refresh_id_a_supp_clicked();

private:

   Ui::DialogAdherent *ui;
   Adherent Atmp;

   //CAMERA
   void start_camera();
   void stop_camera();
   QScopedPointer<QCamera> M_Camera;
   QScopedPointer<QCameraImageCapture> M_Camera_Image;
   int Counter=0;
   QImage ImageFromCamera;

   //CAMERA MODIF
   void start_camera_modif();
   void stop_camera_modif();
   QScopedPointer<QCamera> M_Camera_modif;
   QScopedPointer<QCameraImageCapture> M_Camera_Image_modif;
   int Counter_modif=0;
   QImage ImageFromCamera_modif;
public:
     static int gett();



};

#endif // DIALOGADHERENT_H
