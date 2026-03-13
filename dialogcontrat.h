#ifndef DIALOGCONTRAT_H
#define DIALOGCONTRAT_H
#include "contrat.h"

#include <QDialog>

namespace Ui {
class DialogContrat;
}

class DialogContrat : public QDialog
{
    Q_OBJECT

public:
    explicit DialogContrat(QWidget *parent = nullptr);
    ~DialogContrat();
    void setResultCont(int result); //2----------------------------------------------

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_41_clicked();
    void on_pushButton_39_clicked();
    void on_pushButton_50_clicked();

    void on_asc_clicked();

    void on_pushButton_40_clicked();

    void on_pdf_clicked();

    void on_tableView_4_activated(const QModelIndex &index);

    void on_lineEdit_40_textChanged(const QString &arg1);

    void on_lineEdit_40_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_41_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_41_textChanged(const QString &arg1);

    void on_lineEdit_42_textChanged(const QString &arg1);
void  showNotification(const QString& title, const QString& message);
void on_pushButton_38_clicked();
void on_pushButton_retour_clicked();
void on_pushButton_logout_Adherent_clicked();

private:
    Ui::DialogContrat *ui;
    Contrat Con;
};

#endif // DIALOGCONTRAT_H
