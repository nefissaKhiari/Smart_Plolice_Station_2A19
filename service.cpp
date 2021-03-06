#include "service.h"
#include<QSqlQuery>
#include<QtDebug>
#include<QObject>

Service::Service()
{
    id=0;  libelle=""; duree=0;
    papiers_necess="";description="";
}

Service::Service(QString libelle, int duree, QString papiers_necess, QString description,int id_citoyen,int id_policier)
{
    this->libelle=libelle; this->duree=duree;
    this->papiers_necess=papiers_necess; this->description=description;
    this->id_citoyen=id_citoyen;
    this->id_policier=id_policier;
}
int Service::getid(){return id;}
QString Service::getlibelle(){return libelle;}
int Service::getduree(){return duree;}
QString Service::getpapiersnecess(){return papiers_necess;}
QString Service::getdescription(){return description;}
int Service::getid_citoyen(){return id_citoyen;}
int Service::getidpolicier(){return id_citoyen;}
void Service::setid(int id){this->id=id;}
void Service::setlibelle(QString libelle){this->libelle=libelle;}
void Service::setduree(int duree){this->duree=duree;}
void Service::setpapiersnecess(QString papiers_necess){this->papiers_necess=papiers_necess;}
void Service::setdescription(QString description){this->description=description;}
void Service::setid_citoyen(int id_citoyen){this->id_citoyen=id_citoyen;}
void Service::setidpolicier(int id_policier){this->id_policier=id_policier;}
bool Service::ajouter()
{
    QSqlQuery query;
QString duree_string = QString::number(duree);
        query.prepare("INSERT INTO services (libelle, duree, papiers_necess,description,id_citoyen,id_policier) "
                      "VALUES (:libelle, :duree, :papiers_necess,:description,:id_citoyen,:id_policier)");
        query.bindValue(0, libelle);
        query.bindValue(1, duree_string);
        query.bindValue(2,papiers_necess);
        query.bindValue(3,description);
        query.bindValue(4,id_citoyen);
        query.bindValue(5,id_policier);

    return query.exec();

}
bool Service::supprimer(int id)
{
    QSqlQuery query;

        query.prepare("Delete from services where id=:id");
        query.bindValue(0, id);

    return query.exec();
}
QSqlQueryModel* Service::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();
          model->setQuery("SELECT * FROM services ");
          model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
          model->setHeaderData(1, Qt::Horizontal, QObject::tr("LIBELLE"));
          model->setHeaderData(2, Qt::Horizontal, QObject::tr("DUREE"));
          model->setHeaderData(3, Qt::Horizontal, QObject::tr("PAPIERS_NESCESSAIRES"));
          model->setHeaderData(4, Qt::Horizontal, QObject::tr("DESCRIPTION"));
          model->setHeaderData(5, Qt::Horizontal, QObject::tr("ID_CITOYEN"));
           model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID_POLICIER"));
    return model;
}
QSqlQueryModel* Service::listId() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id FROM services");
    return model;
}
QSqlQueryModel* Service::listNom() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT libelle FROM services");
    return model;
}
bool Service::modifier() {
    QSqlQuery query;
    QString id_string = QString::number(id);
    QString duree_string = QString::number(duree);
    query.prepare("UPDATE services set libelle=:libelle, duree=:duree, papiers_necess=:papiers_necess, description=:description where id=:id");
    query.bindValue(4, id_string);
    query.bindValue(0, libelle);
    query.bindValue(1, duree_string);
    query.bindValue(2, papiers_necess);
    query.bindValue(3, description);

    return query.exec();
}
QSqlQueryModel* Service::rechercher(QString rechercher,QString by) {
    QSqlQueryModel* model = new QSqlQueryModel();
    if(by=="id") {
        model->setQuery("SELECT * FROM services WHERE id LIKE '"+rechercher+"%'");
    }
    else if(by=="libelle") {
       model->setQuery("SELECT * FROM services WHERE libelle LIKE '"+rechercher+"%'");
    }



    return model;
}
QSqlQueryModel* Service::Trier(QString tri) {
    QSqlQueryModel* model = new QSqlQueryModel();
    if(tri=="libelle") {
        model->setQuery("SELECT * FROM services ORDER BY libelle ");
    }

    return model;
}
QSqlQueryModel * Service::affectjoint(){
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT p.cin_policier, s.libelle,c.nom FROM citoyens c INNER JOIN services s ON c.id = s.id_citoyen INNER JOIN policier p ON p.cin_policier = s.cin_policier");
    return model;
}
