#ifndef ADHERENT_H
#define ADHERENT_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
//#include <QByteArray> // Ajout de cette ligne pour utiliser QByteArray

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include <QPainter>
#include <QStyledItemDelegate>

class Adherent
{
private:

   int id_a;
   QString nom;
   QString prenom;
   QString email;
   QString ville;
   int num_tel;
   int fidelite;
   int cin;
   QString image_a; // Ajout de cette ligne pour stocker l'image de l'adhérent
   int id_e;

public:
   Adherent();
   Adherent(int,QString,QString,QString,QString,int,int,int,QString); // Modification du constructeur pour inclure l'image
   Adherent(QString,QString,QString,QString,int,int,int,QString); // Modification du constructeur pour inclure l'image


   int Getid_a() const{ return id_a; }
   void Setid_a(int val) { id_a = val; }

   QString Getemail() const{ return email; }
   void Setemail(QString val) { email = val; }

   QString Getnom() const{ return nom; }
   void Setnom(QString val) { nom = val; }

   QString Getprenom() const{ return prenom; }
   void Setprenom(QString val) { prenom = val; }

   QString Getville() const{ return ville; }
   void Setville(QString val) { ville = val; }

   int Getnum_tel() const{ return num_tel; }
   void Setnum_tel(int val) { num_tel = val; }

   int Getcin() const{ return cin; }
   void Setcin(int val) { cin =val; }

   int Getfidelite() const{ return fidelite; }
   void Setfidelite(int val) { fidelite = val; }

   // Fonctions pour manipuler l'image
   QString Getimage_a() const { return image_a; }
   void Setimage_a(QString val) { image_a = val; }

   int getid_e() const{ return id_e; }
   void setid_e(int val) { id_e = val; }


   //Les fonctionnalités
   bool ajouter();
   static QSqlQueryModel * afficher();
   bool supprimer(int id, int idEmploye);

   static Adherent rechercher(int id_a);
   bool modifier(int idEmploye);
   static bool idExiste(QString);

   //Recherche
   QSqlQueryModel* chercherParId(const QString& id_a);
   static QSqlQueryModel* chercherParPrenom(const QString& prenom) ;


   //tri
   static QSqlQueryModel* trie(QString croissance, QString critere, int idEmploye);

   //Statistiques
   static QHash<QString, int> statistiques(int idEmploye);

   //PDF

   static Adherent getAdherentDetails(int cin, int idEmploye);
   static QList<Adherent> getAdherentList(int idEmploye);

   //historique
   static void historique(int a, QString name, QString type, QString nomEmplye);
};

// Custom delegate to display images in a table view
// Custom delegate to display square images in a table view
class ImageDelegate : public QStyledItemDelegate {
public:
    ImageDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.column() == 8) { // Assuming the image column is at index 8
            QString imagePath = index.data().toString();
            QImage image(imagePath);
            if (!image.isNull()) {
                QRect rect = option.rect;
                painter->drawImage(rect, image);
            }
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

};


#endif // ADHERENT_H
