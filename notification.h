#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <QSystemTrayIcon>
#include<QString>

class notification
{
public:
    notification();

        void notifications_modifiervehicule();
        void notifications_modifiermission();
        void notifications_supprimermaintenance();
        void notifications_ajoutercitoyen();
        void notifications_ajouterservice();
        void notifications_supprimercitoyen();
        void notifications_supprimerservice();
        void notifications_modifiercitoyen();
        void notifications_modifierservice();
        void notifications_envoyermail();
        void notifications_pdfservice();
        void notifications_connection();
        void notifications_deconnection();
        void notifications_prob();
        void notifications_mdpoubl();

};
#endif // NOTIFICATION_H
