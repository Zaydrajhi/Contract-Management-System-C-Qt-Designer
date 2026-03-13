#ifndef DIALOGFORGOTPASSWORD_H
#define DIALOGFORGOTPASSWORD_H

#include <QDialog>

namespace Ui {
class Dialogforgotpassword;
}

class Dialogforgotpassword : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogforgotpassword(QWidget *parent = nullptr);
    ~Dialogforgotpassword();

private slots:
    void on_valider_clicked();

    void on_modifier_clicked();

    void on_Retour_clicked();

private:
    Ui::Dialogforgotpassword *ui;
};

#endif // DIALOGFORGOTPASSWORD_H
