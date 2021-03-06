#include "policier.h"
#include <QSqlQuery>
#include <QDebug>
#include <QObject>
#include <QString>
#include "ui_policestation.h"

Policier::Policier() {
    CIN_policier=0;  nom_policier="";  prenom_policier="";  grade_policier="";   mail_policier="";   mdp_policier=0;  secteur_policier="";
}

Policier::Policier(int CIN_policier, QString nom_policier, QString prenom_policier, QString grade_policier , QString mail_policier, int mdp_policier,  QString secteur_policier)
{
     this->CIN_policier=CIN_policier;
     this->nom_policier=nom_policier;
     this->prenom_policier=prenom_policier;
     this->grade_policier=grade_policier;
     this->mail_policier=mail_policier;
     this->mdp_policier=mdp_policier;

     this->secteur_policier=secteur_policier;
}

void Policier::setCin_policier(int CIN_policier) { this->CIN_policier=CIN_policier; }
int Policier::getCin_policier() { return CIN_policier; }

void Policier::setNom_policier(QString nom_policier) { this->nom_policier=nom_policier; }
QString Policier::getNom_policier() { return nom_policier; }

void Policier::setPrenom_policier(QString prenom_policier) { this->prenom_policier=prenom_policier; }
QString Policier::getPrenom_policier() { return prenom_policier; }

void Policier::setGrade_policier(QString grade_policier) { this->grade_policier=grade_policier; }
QString Policier::getGrade_policier() { return grade_policier; }

void Policier::setMail_policier(QString mail_policier) { this->mail_policier=mail_policier; }
QString Policier::getMail_policier() { return mail_policier; }

void Policier::setMdp_policier(int mdp_policier) { this->mdp_policier=mdp_policier; }
int Policier::getMdp_policier() { return mdp_policier; }


void Policier::setSecteur_policier(QString secteur_policier) { this->secteur_policier=secteur_policier; }
QString Policier::getSecteur_policier() { return secteur_policier; }





/*bool Policier::ajouter_policier() {
    QSqlQuery query;
    QString cin_policier_string = QString::number(CIN_policier);
    query.prepare("INSERT INTO policier (cin_policier, nom_policier, prenom_policier, grade_policier, mail_policier, mdp_policier, secteur_policier)" "VALUES (:cin_policier, :nom_policier, :prenom_policier, :grade_policier, :mail_policier,  :mdp_policier, :secteur_policier )");
    query.bindValue(0, cin_policier_string);
    query.bindValue(1, nom_policier);
    query.bindValue(2, prenom_policier);
    query.bindValue(3, grade_policier);
    query.bindValue(4, mail_policier);
    query.bindValue(5, mdp_policier);
    query.bindValue(7, secteur_policier);
    return query.exec();
}*/

bool Policier::ajouter_policier() {
    QSqlQuery query;
    QString cin_policier_string = QString::number(CIN_policier);
    QString mdp_policier_string = QString::number(mdp_policier);
    query.prepare("INSERT INTO policier ( cin_policier, nom_policier , prenom_policier, grade_policier, mail_policier, mdp_policier,  secteur_policier )"
 "VALUES ( :cin_policier, :nom_policier,:prenom_policier,:grade_policier, :mail_policier, :mdp_policier, :secteur_policier)");

    query.bindValue(0, cin_policier_string);
    query.bindValue(1, nom_policier);
    query.bindValue(2, prenom_policier);
    query.bindValue(3, grade_policier);
    query.bindValue(4, mail_policier);
    query.bindValue(5, mdp_policier_string);
    query.bindValue(7, secteur_policier);
    return query.exec();
}


bool Policier::modifier_policier() {
    QSqlQuery query;
    QString cin_policier_string = QString::number(CIN_policier);
    QString mdp_policier_string = QString::number(mdp_policier);
    query.prepare("UPDATE policier set nom_policier=:nom_policier, prenom_policier=:prenom_policier, grade_policier=:grade_policier, mail_policier=:mail_policier, mdp_policier=:mdp_policier, secteur_policier=:secteur_policier where cin_policier=:cin_policier");
    query.bindValue(6, cin_policier_string);
    query.bindValue(0, nom_policier);
    query.bindValue(1, prenom_policier);
    query.bindValue(2, grade_policier);
    query.bindValue(3, mail_policier);
    query.bindValue(4, mdp_policier_string);
    query.bindValue(5, secteur_policier);
    return query.exec();
}


bool Policier::supprimer_policier(int CIN_policier) {
    QSqlQuery query;
    query.prepare("delete from policier where cin_policier=:cin_policier");
    query.bindValue(0, CIN_policier);
    return query.exec();
}


QSqlQueryModel* Policier::afficher_policier() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM policier");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN_policier"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom_policier"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom_policier"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Grade_policier"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mail_policier"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mdp_policier"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Secteur_policier"));
    return model;
}


QSqlQueryModel* Policier::listCin_policier() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT cin_policier FROM policier");
    return model;
}


QSqlQueryModel* Policier::Trier_policier(QString tri) {
    QSqlQueryModel* model = new QSqlQueryModel();
    if(tri=="Nom") {
        model->setQuery("SELECT * FROM policier ORDER BY nom_policier ASC");
    }
    else if(tri=="Prenom") {
        model->setQuery("SELECT * FROM policier ORDER BY prenom_policier ASC");
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN_policier"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom_policier"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom_policier"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Grade_policier"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mail_policier"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mdp_policier"));

    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Secteur_policier"));
    return model;
}






 QSqlQueryModel * Policier::Chercher_policier1(QString nom_policier)
 {
     QSqlQueryModel *model = new QSqlQueryModel;
         model->setQuery("SELECT * FROM policier WHERE nom_policier='"+nom_policier+"' ");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN_policier"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom_policier"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom_policier"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("Grade_policier"));
         model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mail_policier"));
         model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mdp_policier"));
         model->setHeaderData(7, Qt::Horizontal, QObject::tr("Secteur_policier"));
         return model;
 }


 QSqlQueryModel * Policier::Chercher_policier2(QString prenom_policier)
 {
     QSqlQueryModel *model = new QSqlQueryModel;
         model->setQuery("SELECT * FROM policier WHERE prenom_policier='"+prenom_policier+"' ");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN_policier"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom_policier"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom_policier"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("Grade_policier"));
         model->setHeaderData(4, Qt::Horizontal, QObject::tr("Mail_policier"));
         model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mdp_policier"));
         model->setHeaderData(7, Qt::Horizontal, QObject::tr("Secteur_policier"));
         return model;
 }
 QSqlQueryModel* Policier::listId() {
     QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("SELECT CIN_policier FROM policier");
     return model;
 }

 void Policier::addToHistory(QString action, QString type, QString CIN_policier)
 {



     QSqlQuery query;
     QString date=QDateTime::currentDateTime().toString("dddd, dd MMMM yyyy");
     QString date1=QDateTime::currentDateTime().toString("dd/MM/yy");
     QString time=QDateTime::currentDateTime().toString("hh:mm");
     QString activity;

     activity="\n    "+date1+"   -   "+time+" \t    "+ action +" "+type+"  cin_policier:  "+CIN_policier+" \n";

           query.prepare("INSERT INTO hiistory (activity) VALUES (:activity)");
           query.bindValue(":activity", activity);

     query.exec();
 }

 QSqlQueryModel* Policier::afficherHistorique(Ui::PoliceStation*ui)
 {
     QSqlQueryModel * model=new QSqlQueryModel();
     QSqlQuery query;
     QString historyType="";
     switch (ui->combo_action->currentIndex())
     {
     case 0:  historyType="";break;
     case 1:  historyType="Ajout";break;
     case 2:  historyType="Modification";break;
     case 3:  historyType="Suppression";break;
     }
     QString historymp="";
     switch (ui->comboBox_mp->currentIndex())
     {
     case 0:  historymp="";break;
     case 1:  historymp="policier";break;
     case 2:  historymp="amende";break;

     }
     query.prepare("SELECT activity FROM hiistory where activity like '%"+historyType+"%' and  activity like '%"+historymp+"%'");
          query.exec();
          model->setQuery(query);
     return model;
 }

 bool Policier::supprimerhistory(QString date,QString type,QString mp)
 {
   QSqlQuery query ;
   query.prepare("DELETE FROM HIISTORY WHERE ACTIVITY like '%"+date+"%' and ACTIVITY like '%"+type+"%' and ACTIVITY like '%"+mp+"%'");

   if(query.exec())
   {
               return query.exec();
   }
   else return false;
 }
