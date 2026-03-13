#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "connection.h"
#include <QMessageBox>
#include "dialogconstat.h"
#include "dialogemployes.h"
#include "dialoglogin.h"
#include "dialogforgotpassword.h"


/*int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "interface_client_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    DialogConstat dc;
    DialogEmployes de;
    //MainWindow w;
    DialogLogin w;
    Connection c;
    bool test=c.createconnect();
    if(test)
    {w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);


    return a.exec();
}
*/


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "interface_client_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Connection c;
    Dialogforgotpassword D;
    bool test = c.createconnect();
    DialogLogin w;
    DialogConstat dc;
    DialogEmployes de;
    Employe e;
    w.show();

    // Connexion entre le signal loginSuccessful de la fenêtre de connexion et la création de la fenêtre principale
    QObject::connect(&w, & DialogLogin::loginSuccessful, [&]() {
        QString role = e.getdepartement();

        if (role == "informatique") {
           MainWindow l;
           l.show();
            QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                     QObject::tr("Connection successful.\n"
                                                 "Click Cancel to exit."), QMessageBox::Cancel);
        }


}

   ); if (!test) {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }

    return a.exec();
}
