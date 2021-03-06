#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H
#include<QString>
#include <QSqlQueryModel>
class Equipement
{
private:
    int reference;
    int quantite;
    QString taille;
    QString etat;
    int poid;
    QString nom;
    int CIN_policier;
public:
    Equipement();
    Equipement(int,int,QString,QString,int,QString,int);

    void setReference(int);
    int getReference();

    void setQuantite(int);
    int getQuantite();

    void setTaille(QString);
    QString getTaille();

    void setEtat(QString);
    QString getEtat();

    void setPoid(int);
    int getPoid();

    void setNom(QString);
    QString getNom();

    bool ajouter();
    bool modifier();
    bool supprimer(int);

    QSqlQueryModel* afficher();
    QSqlQueryModel* listRef();
    QSqlQueryModel* listCIN();
    QSqlQueryModel* afficherAffectation();

    QSqlQueryModel* Trier(QString);
    QSqlQueryModel* Chercher(QString,QString);
};

#endif // EQUIPEMENT_H
