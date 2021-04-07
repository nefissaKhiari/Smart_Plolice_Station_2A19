#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "citoyen.h"
#include "service.h"
#include "notification.h"
#include "smtp.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QSqlQuery>
#include <QDebug>
#include <QProcess>
#include <QDialog>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     son=new QSound(":/ressources/cassette-player-button-3.wav");
    ui->stackedWidget->setCurrentIndex(0);
    ui->mdp->setEchoMode(QLineEdit::Password);
    //for email tab
   // connect(ui->pushButton_2, SIGNAL(clicked()),this, SLOT(sendMail()));
    //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(browse()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_B_GestionCitoyen_clicked()
{
    ui->T_Citoyens ->setModel(C.afficher());
       ui->CB_IDCitoyen ->setModel(C.listId());
       ui->stackedWidget->setCurrentIndex(1);
         son->play();
}

void MainWindow::on_B_GestioService_clicked()
{
    ui->T_Service ->setModel(S.afficher());
    ui->CB_IDService->setModel(S.listId());
    ui->CB_NomCitoyen->setModel(C.listId());
    ui->stackedWidget->setCurrentIndex(5);
      son->play();
}

void MainWindow::on_B_BackToGestions_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
      son->play();
}

void MainWindow::on_B_AccederProfil_clicked()
{
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDCitoyen->currentText().toInt());
    qry.prepare("SELECT * FROM citoyens where id=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_PNomCitoyen ->setText(qry.value(1).toString());
            ui->LE_PPrenomCitoyen->setText(qry.value(2).toString());
            ui->LE_PDateNaissCit->setText(qry.value(3).toString());
            ui->LE_PLieuNaissCit->setText(qry.value(4).toString());
            ui->LE_PMailCitoyen->setText(qry.value(5).toString());
            ui->LE_PAdresseCit->setText(qry.value(6).toString());
            ui->LE_PNomPereCit->setText(qry.value(7).toString());
            ui->LE_PProfessionCit->setText(qry.value(8).toString());
            ui->LE_PEtatCitoyen->setText(qry.value(9).toString());
            ui->stackedWidget->setCurrentIndex(4);
        }
    }
  son->play();
}

void MainWindow::on_B_AjouterCitoyen_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
  son->play();
}

void MainWindow::on_B_AAnnulerCitoyen_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
      son->play();
}

void MainWindow::on_B_AConfirmerCitoyen_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du citoyen?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {

        QString nom = ui->LE_ANomCitoyen->text();
        QString prenom = ui->LE_APrenomCitoyen->text();
        QDate date_Naiss= ui->DE_ADateNaissCit->date();
        QString lieu_Naiss = ui->LE_ALieuNaissCit->text();
        QString mail = ui->LE_AMailCitoyen->text();
        QString adresse = ui->LE_AAdresseCit->text();
        QString nom_pere = ui->LE_ANomPereCit->text();
        QString profession = ui->LE_AProfessionCit ->text();
        QString etat_civil = ui->LE_AEtatCitoyen->text();
    Citoyen C(nom,prenom,date_Naiss,lieu_Naiss,mail, adresse,nom_pere,profession,etat_civil);
    if(C.ajouter()) {
        ui->T_Citoyens ->setModel(C.afficher());
        ui->CB_IDCitoyen ->setModel(C.listId());
        ui->stackedWidget->setCurrentIndex(1);

        ui->LE_ANomCitoyen ->setText("");
        ui->LE_APrenomCitoyen ->setText("");
        //ui->DE_ADateNaissCit->setDate();
        ui->LE_ALieuNaissCit->setText("");
        ui->LE_AMailCitoyen->setText("");
        ui->LE_AAdresseCit ->setText("");
        ui->LE_ANomPereCit ->setText("");
        ui->LE_AProfessionCit->setText("");
        ui->LE_AEtatCitoyen ->setText("");
        N.notifications_ajoutercitoyen();
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
    }
    }
  son->play();
}

void MainWindow::on_B_MAnnulerCitoyen_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
      son->play();
}

void MainWindow::on_B_MConfirmerCitoyen_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du citoyen?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        Citoyen C;
        C.setid(ui->CB_IDCitoyen ->currentText().toInt());
        C.setnom(ui->LE_MNomCitoyen ->text());
        C.setprenom(ui->LE_MPrenomCitoyen ->text());
        C.setdatenaiss(ui->DE_MDateNaissCit ->date());
        C.setlieunaiss(ui->LE_MLieuNaissCit ->text());
        C.setmail(ui->LE_MMailCitoyen ->text());
        C.setadresse(ui->LE_MAdresseCit ->text());
        C.setnompere(ui->LE_MNomPereCit->text());
        C.setprofession(ui->LE_MProfessionCit ->text());
        C.setetatcivil(ui->LE_MEtatCitoyen ->text());
qDebug() << C.getdatenaiss();
        if(C.modifier()) {
            ui->T_Citoyens->setModel(C.afficher());
            ui->CB_IDCitoyen->setModel(C.listId());
            ui->stackedWidget->setCurrentIndex(1);
            ui->LE_ANomCitoyen->setText("");
            ui->LE_APrenomCitoyen->setText("");
            //QDate date_Naiss = ui->DE_MDateNaissCit->date();
          //  ui->DE_MDateNaissCit->setDate();
            ui->LE_ALieuNaissCit->setText("");
            ui->LE_AMailCitoyen->setText("");
            ui->LE_AAdresseCit->setText("");
            ui->LE_ANomPereCit->setText("");
            ui->LE_AProfessionCit->setText("");
            ui->LE_AEtatCitoyen->setText("");
            N.notifications_modifiercitoyen();
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
      son->play();
}

void MainWindow::on_B_SupprimerCitoyen_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du lcitoyen?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        C.setid(ui->CB_IDCitoyen ->currentText().toInt());
        if(C.supprimer(C.getid())) {
            qDebug() << "Suppression Complet";
            ui->T_Citoyens ->setModel(C.afficher());
            ui->CB_IDCitoyen ->setModel(C.listId());
             ui->stackedWidget->setCurrentIndex(1);
              N.notifications_supprimercitoyen();
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
  son->play();
}

void MainWindow::on_B_MConfirmerCitoyen_2_clicked()
{
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDCitoyen->currentText().toInt());
    qry.prepare("SELECT * FROM citoyens where id=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MNomCitoyen ->setText(qry.value(1).toString());
            ui->LE_MPrenomCitoyen->setText(qry.value(2).toString());
            ui->DE_MDateNaissCit->setDate(qry.value(3).toDate());
            ui->LE_MLieuNaissCit->setText(qry.value(4).toString());
            ui->LE_MMailCitoyen->setText(qry.value(5).toString());
            ui->LE_MAdresseCit->setText(qry.value(6).toString());
            ui->LE_MNomPereCit->setText(qry.value(7).toString());
            ui->LE_MProfessionCit->setText(qry.value(8).toString());
            ui->LE_MEtatCitoyen->setText(qry.value(9).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(3);
      son->play();
}

void MainWindow::on_B_returnCitoyen_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
      son->play();
}

void MainWindow::on_B_AjouterService_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
      son->play();
}

void MainWindow::on_B_ModifierService_clicked()
{
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDService->currentText().toInt());
    qry.prepare("SELECT * FROM services where id=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MTypeService->setText(qry.value(1).toString());
            ui->LE_MDureeService->setText(qry.value(2).toString());
            ui->LE_MPapierService->setText(qry.value(3).toString());
            ui->TE_MDescService->setText(qry.value(4).toString());
            ui->LE_Nomci->setText(qry.value(5).toString());

        }
    }
    ui->stackedWidget->setCurrentIndex(8);
      son->play();
}

void MainWindow::on_B_AAnnulerService_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
      son->play();
}

void MainWindow::on_B_AConfirmerService_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du service?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        QString libelle = ui->LE_ATypeService->text();
        QString duree = ui->LE_ADureeService->text();
        QString papiers_necess = ui->LE_APapierService->text();
        QString description = ui->TE_ADescService->toPlainText();
        int id_citoyen=ui->CB_NomCitoyen->currentText().toInt();
         int id_policier=ui->CB_Idpolicier->currentText().toInt();

    Service S(libelle,duree,papiers_necess,description,id_citoyen,id_policier);
    if(S.ajouter()) {
        ui->T_Service ->setModel(S.afficher());
        ui->CB_IDService ->setModel(S.listId());
        ui->CB_Idpolicier ->setModel(S.listId());
        ui->stackedWidget->setCurrentIndex(5);

        ui->LE_ATypeService ->setText("");
        ui->LE_ADureeService ->setText("");
        ui->LE_APapierService->setText("");
        ui->TE_ADescService->setText("");
        N.notifications_ajouterservice();

    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
    }
    }
      son->play();
}

void MainWindow::on_B_MAnnulerService_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
      son->play();
}

void MainWindow::on_B_MConfirmerService_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du service?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        Service S;
        S.setid(ui->CB_IDService ->currentText().toInt());
        S.setlibelle(ui->LE_MTypeService ->text());
        S.setduree(ui->LE_MDureeService ->text());
        S.setpapiersnecess(ui->LE_MPapierService ->text());
        S.setdescription(ui->TE_MDescService->toPlainText());
       // S.setid_citoyen(ui->LE_Nomci->text().toInt());


qDebug()<<S.getdescription();

        if(S.modifier()) {
            ui->T_Service->setModel(S.afficher());
            ui->CB_IDService->setModel(S.listId());
            ui->stackedWidget->setCurrentIndex(5);
            ui->LE_MTypeService->setText("");
            ui->LE_MDureeService->setText("");
            ui->LE_MPapierService->setText("");
            ui->TE_MDescService->setText("");
             //ui->LE_Nomci->setText("");
            N.notifications_modifierservice();
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
      son->play();
}

void MainWindow::on_B_BackToGestions_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
      son->play();
}

void MainWindow::on_B_SupprimerService_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du service?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        S.setid(ui->CB_IDService->currentText().toInt());
        if(S.supprimer(S.getid())) {
            qDebug() << "Suppression Complet";
            ui->T_Service->setModel(S.afficher());
            ui->CB_IDService->setModel(S.listId());
             N.notifications_supprimerservice();
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
      son->play();
}

void MainWindow::on_LE_recherche_textChanged(const QString &arg1)
{
    QString by=ui->CB_recherche->currentText();
            ui->T_Citoyens->setModel(C.rechercher(arg1,by));

}

void MainWindow::on_LE_rechercheS_textChanged(const QString &arg1)
{
    QString by=ui->CB_rechercheS->currentText();
            ui->T_Service->setModel(S.rechercher(arg1,by));
}

void MainWindow::on_B_Trier_clicked()
{
    son->play();
    QString Tri = ui->CB_Tricitoyen->currentText();
            ui->T_Citoyens->setModel(C.Trier(Tri));

}






void MainWindow::on_B_TrierS_clicked()
{
    QString Tri = ui->CB_TriService->currentText();
            ui->T_Service->setModel(S.Trier(Tri));

}

void MainWindow::on_B_ResetTableIntervenant_2_clicked()
{
    ui->T_Citoyens ->setModel(C.afficher());
}

void MainWindow::on_B_ResetTableIntervenant_clicked()
{
    ui->T_Service ->setModel(S.afficher());
}

void MainWindow::on_B_EnvoyeMail_clicked()
{
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDCitoyen->currentText().toInt());
    qry.prepare("SELECT * FROM citoyens where id=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->lineEdit->setText(qry.value(5).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_pushButton_2_clicked()

    {
        Smtp* smtp = new Smtp("myriam.brahmi@esprit.tn",ui->mdp->text(), "smtp.gmail.com");
        connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
 if( !files.isEmpty() )
          smtp->sendMail("myriam.brahmi@esprit.tn", ui->lineEdit->text() , ui->lineEdit_3->text(),ui->msg->toPlainText(),files);
 else
     smtp->sendMail("myriam.brahmi@esprit.tn", ui->lineEdit->text() , ui->lineEdit_3->text(),ui->msg->toPlainText());
          ui->stackedWidget->setCurrentIndex(4);
          N.notifications_envoyermail();
    }


void MainWindow::on_B_returnCitoyen_3_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_browseBtn_clicked()
{
    files.clear();

    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec())
        files = dialog.selectedFiles();

    QString fileListString;
    foreach(QString file, files)
        fileListString.append( "\"" + QFileInfo(file).fileName() + "\" " );

    ui->file->setText( fileListString );
}