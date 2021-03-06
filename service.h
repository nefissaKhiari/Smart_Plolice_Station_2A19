#ifndef SERVICE_H
#define SERVICE_H


#include<QString>
#include <QSqlQueryModel>
class Service
{
public:
    Service();

    Service(QString, int, QString, QString,int,int);
    int getid();
    QString getlibelle();
    int getduree();
    QString getpapiersnecess();
    QString getdescription();
    int getidpolicier();
     int getid_citoyen();
    void setid(int);
    void setlibelle(QString);
    void setduree(int);
    void setpapiersnecess(QString);
    void setdescription(QString);
    void setid_citoyen(int);
    void setidpolicier(int);
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
     bool modifier();
    QSqlQueryModel* listId();
     QSqlQueryModel* listNom();
       QSqlQueryModel* rechercher(QString,QString);
        QSqlQueryModel* Trier(QString);
        QSqlQueryModel *affectjoint();
private:
    int id , id_citoyen,id_policier,duree;
    QString libelle  , papiers_necess, description;

};


#endif // SERVICE_H
