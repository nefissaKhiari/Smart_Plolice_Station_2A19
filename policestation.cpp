#include "policestation.h"
#include "ui_policestation.h"
#include <QQuickItem>
using namespace qrcodegen;
PoliceStation::PoliceStation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PoliceStation)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);


    ui->LE_ACinIntervenant->setValidator(new QIntValidator(1, 99999999, this));
    ui->LE_ADureeService->setValidator(new QIntValidator(1, 999, this));
    ui->mdp->setEchoMode(QLineEdit::Password);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(myFunction()));
    timer->start(1000);
    QDate date = QDate ::currentDate();
    QString datee=date.toString();
    ui->date_3->setText(datee);
    player = new QMediaPlayer(this);
    connect(player, &QMediaPlayer::positionChanged , this ,&PoliceStation::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged , this ,&PoliceStation::on_durationChanged);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();
    auto obj= ui->quickWidget->rootObject();
    connect(this , SIGNAL(setCenter(QVariant,QVariant)), obj,SLOT(setCenter(QVariant,QVariant)));
    emit setCenter(25.000, 50.000);
    son=new QSound(":/ressource/image/cassette-playerbutton-3.wav");
}

PoliceStation::~PoliceStation()
{
    delete ui;
}

void PoliceStation::INFORMER(QLabel *label, QString message, int duration){
    label->setVisible(true);
    label->setText(message);
    QTimer::singleShot(duration, ui->L_AlerteIntervenant, &QLabel::hide);
    QTimer::singleShot(duration, ui->L_AlerteAffaire, &QLabel::hide);
    QTimer::singleShot(duration, ui->labelmessage, &QLabel::hide);
    QTimer::singleShot(duration, ui->ALERT_M, &QLabel::hide);
}

void PoliceStation::on_B_Connecter_clicked()
{ son->play();
qDebug()<< log->hash(ui->lineEdit_MDP->text());
    if (log->sign_in(ui->lineEdit_Mail->text(),ui->lineEdit_MDP->text()))
            {
                ui->stackedWidget->setCurrentIndex(1);
                ui->lineEdit_Mail->clear();
                ui->lineEdit_MDP->clear();
                N.notifications_connection();
   }else{

       N.notifications_prob();
    }
}

void PoliceStation::on_B_Deconnecte_clicked()
{ son->play();
    ui->stackedWidget->setCurrentIndex(0);
    N.notifications_deconnection();
}

void PoliceStation::on_B_AffaireJuri_clicked()
{ son->play();
    ui->stackedWidget->setCurrentIndex(2);
}

void PoliceStation::on_B_BackToMenu_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(1);
}

void PoliceStation::on_B_Vehic_clicked()
{ son->play();
    ui->stackedWidget->setCurrentIndex(19);
}

void PoliceStation::on_B_Equip_clicked()
{ son->play();
    ui->stackedWidget->setCurrentIndex(10);
}

void PoliceStation::on_B_BackToMenu_2_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(1);
}

void PoliceStation::on_B_BackToMenu_4_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(1);
}

void PoliceStation::on_B_GestionIntervenant_clicked()
{son->play();
    ui->T_Intervenants->setModel(intervenant.afficher());
    ui->CB_IDIntervenant->setModel(intervenant.listCin());
    ui->stackedWidget->setCurrentIndex(3);
}

void PoliceStation::on_B_GestioAffaire_clicked()
{son->play();
    ui->T_Affaire->setModel(affaire.afficher());
    ui->CB_IDAffaire->setModel(affaire.listId());
    ui->stackedWidget->setCurrentIndex(6);
}

void PoliceStation::on_B_BackToGestions_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(2);
}

void PoliceStation::on_B_Statistics_clicked()
{son->play();
    /********************* BEGIN : Donut->Nationalite *********************/
    QSqlQuery qry;
    int Autres=intervenant.NatA();
    int Tn=intervenant.NatTn();
    int Ag=intervenant.NatAg();
    int Fr=intervenant.NatFr();
    int Lb=intervenant.NatLb();

    QPieSeries *series = new QPieSeries();
    series->append("Tunisienne", Tn);
    series->append("Francaise", Fr);
    series->append("Algerienne", Ag);
    series->append("Libanaise", Lb);
    series->append("Autres...", Autres);
    series->setHoleSize(0.5);
    series->setPieSize(0.8);

    QPieSlice *tn = series->slices().at(0);
    QPieSlice *fr = series->slices().at(1);
    QPieSlice *ag = series->slices().at(2);
    QPieSlice *lb = series->slices().at(3);
    //QPieSlice *autres = series->slices().at(4);
    /*********************** Labels
    tn->setLabelVisible(true);
    fr->setLabelVisible(true);
    ag->setLabelVisible(true);
    lb->setLabelVisible(true);
    autres->setLabelVisible(true);
    ******************************/
    tn->setBrush(Qt::red);
    fr->setBrush(Qt::blue);
    ag->setBrush(Qt::green);
    lb->setPen(QPen(Qt::green, 1));
    lb->setBrush(Qt::white);
    fr->setBrush(Qt::darkBlue);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nationalite des Intervenants");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->F_Statistic);
    /********************* END : Donut->Nationalite *********************/

    /********************* BEGIN : Bars->Localisation *********************/
    int NE=intervenant.NE();
    int NO=intervenant.NO();
    int CE=intervenant.CE();
    int CO=intervenant.CO();
    int SE=intervenant.SE();
    int SO=intervenant.SO();

    QBarSet *set0 = new QBarSet("Tunis, Bizerte..");
    QBarSet *set1 = new QBarSet("Beja, Le Kef..");
    QBarSet *set2 = new QBarSet("Sousse, Mehdia..");
    QBarSet *set3 = new QBarSet("Kairouan..");
    QBarSet *set4 = new QBarSet("Sfax, Gabes..");
    QBarSet *set5 = new QBarSet("Gafsa, Tozeur..");

    *set0 << NE << 0 << 0 << 0 << 0 << 0;
    *set1 << 0 << NO << 0 << 0 << 0 << 0;
    *set2 << 0 << 0 << CE << 0 << 0 << 0;
    *set3 << 0 << 0 << 0 << CO << 0 << 0;
    *set4 << 0 << 0 << 0 << 0 << SE << 0;
    *set5 << 0 << 0 << 0 << 0 << 0 << SO;

    QBarSeries *seriess = new QBarSeries();
    seriess->append(set0);
    seriess->append(set1);
    seriess->append(set2);
    seriess->append(set3);
    seriess->append(set4);
    seriess->append(set5);

    QChart *charts = new QChart();
    charts->addSeries(seriess);
    charts->setTitle("Localisation des Intervenants");
    charts->setAnimationOptions(QChart::AllAnimations);
    QStringList categories;
    categories << "Nord-Est" << "Nord-Ouest" << "Centre-Est" << "Centre-Ouest" << "Sud-Est" << "Sud-Ouest";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    charts->createDefaultAxes();
    charts->setAxisX(axis, seriess);
    charts->legend()->setVisible(true);
    charts->legend()->setAlignment(Qt::AlignBottom);
    QChartView *chartsView = new QChartView(charts);
    chartsView->setRenderHint(QPainter::Antialiasing);
    chartsView->setParent(ui->F_StatisticLocal);
    /********************* END : Bars->Localisation *********************/

    /********************* BEGIN : Curve->Localisation *********************/
    QLineSeries *seriesss = new QLineSeries();
    seriesss->append(0, 12);
    seriesss->append(1, 8);
    seriesss->append(2, 10);
    seriesss->append(3, 8);
    seriesss->append(4, 4);
    QChart *charttt = new QChart();
    charttt->legend()->hide();
    charttt->addSeries(seriesss);
    charttt->createDefaultAxes();
    QFont font;
    font.setPixelSize(18);
    charttt->setTitleFont(font);
    charttt->setTitleBrush(QBrush(Qt::black));
    charttt->setTitle("Salaire des Intervenants");
    QPen pen(QRgb(0x0D74FF));
    pen.setWidth(5);
    seriesss->setPen(pen);
    charttt->setAnimationOptions(QChart::AllAnimations);
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->append("500DT",0);
    axisX->append("1000DT",1);
    axisX->append("1500DT",2);
    axisX->append("2000DT",3);
    axisX->append("2500DT",4);
    charttt->setAxisX(axisX, seriesss);
    QChartView *chartViewww = new QChartView(charttt);
    chartViewww->setRenderHint(QPainter::Antialiasing);

    chartViewww->setParent(ui->F_StatisticAge);
    /********************* END : Curve->Localisation *********************/

    ui->stackedWidget->setCurrentIndex(9);
}

void PoliceStation::on_B_BackToGestions_3_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(3);
}

void PoliceStation::on_B_ModifierIntervenant_clicked()
{son->play();
    QSqlQuery qry;
    QString cin_string = QString::number(ui->CB_IDIntervenant->currentText().toInt());
    qry.prepare("SELECT * FROM intervenant where cin=:cin");
    qry.bindValue(0, cin_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MCinIntervenant->setText(qry.value(0).toString());
            ui->LE_MNomIntervenant->setText(qry.value(1).toString());
            ui->LE_MPrenomIntervenant->setText(qry.value(2).toString());
            ui->CB_MNationaliteIntervenant->setCurrentText(qry.value(3).toString());
            ui->CB_MLocalIntervenant->setCurrentText(qry.value(4).toString());
            ui->LE_MMailIntervenant->setText(qry.value(5).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(5);
}

void PoliceStation::on_B_AjouterIntervenant_clicked()
{son->play();
    ui->L_ACinIntAlert->clear();
    ui->L_ANomIntAlert->clear();
    ui->L_APrenomIntAlert->clear();
    ui->L_ALocalIntAlert->clear();
    ui->L_AMailIntAlert->clear();
    ui->CB_ANationaliteIntervenant->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(4);
}

void PoliceStation::on_B_SupprimerIntervenant_clicked()
{son->play();
    Intervenant intervenantS;
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du l'intervenant?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        intervenantS.setCin(ui->CB_IDIntervenant->currentText().toInt());
        if(intervenantS.supprimer(intervenantS.getCin())) {
            qDebug() << "Suppression Complet";
            ui->T_Intervenants->setModel(intervenantS.afficher());
            ui->CB_IDIntervenant->setModel(intervenantS.listCin());

            ui->T_Affaire->setModel(affaire.afficher());
            ui->CB_IDAffaire->setModel(affaire.listId());
            INFORMER(ui->L_AlerteIntervenant,"Intervenat est Supprime",3000);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_AConfirmerIntervenant_clicked()
{son->play();
    /*************** BEGIN : Controle de Saisir L'ajout d'Intervenant ***************/
    bool overAll = false, cin_B, nom_B, prenom_B; //mail_B
    int cin = ui->LE_ACinIntervenant->text().toInt();
    QString cin_l = ui->LE_ACinIntervenant->text();
    QString nom = ui->LE_ANomIntervenant->text();
    QString prenom = ui->LE_APrenomIntervenant->text();
    QString nationalite = ui->CB_ANationaliteIntervenant->currentText();
    QString localisation = ui->CB_ALocalIntervenant->currentText();
    QString mail = ui->LE_AMailIntervenant->text();

    // BEGIN : cin
    if(cin_l.length() < 8) {
        cin_B = false;
        ui->L_ACinIntAlert->setText("Il faut 8 charactere de facon XXXXXXXXXX");
        ui->L_ACinIntAlert->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        cin_B = true;
        ui->L_ACinIntAlert->setText("Ok");
        ui->L_ACinIntAlert->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    // END : cin

    // BEGIN : Nom
    if(nom.length() < 3) {
        nom_B = false;
        ui->L_ANomIntAlert->setText("Il faut 3 charactere Minimum");
        ui->L_ANomIntAlert->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        nom_B = true;
        nom[0] = nom[0].toUpper();
        ui->L_ANomIntAlert->setText("Ok");
        ui->L_ANomIntAlert->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    // END : Nom

    // BEGIN : Prenom
    if(prenom.length() < 3) {
        prenom_B = false;
        ui->L_APrenomIntAlert->setText("Il faut 3 charactere Minimum");
        ui->L_APrenomIntAlert->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        prenom_B = true;
        prenom[0] = prenom[0].toUpper();
        ui->L_APrenomIntAlert->setText("Ok");
        ui->L_APrenomIntAlert->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    // END : Prenom

    // BEGIN : Mail
    // END : Mail

    (cin_B && nom_B && prenom_B)? overAll = true : overAll = false;
    /*************** END : Controle de Saisir L'ajout d'Intervenant ***************/

    /*************** BEGIN : Ajouter sur BaseDonnee ***************/
    if(overAll) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du l'intervenant?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            Intervenant intervenant(cin, nom, prenom, nationalite, localisation, mail);
            if(intervenant.ajouter()) {
                ui->CB_IDIntervenant->setModel(intervenant.listCin());
                ui->T_Intervenants->setModel(intervenant.afficher());
                INFORMER(ui->L_AlerteIntervenant,"Intervenat est Ajoute",3000);
                ui->stackedWidget->setCurrentIndex(3);
            }
            else {
                QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
            }
        }
    }
    /*************** END : Ajouter sur BaseDonnee ***************/
}

void PoliceStation::on_B_AAnnulerIntervenant_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(3);
}

void PoliceStation::on_B_MAnuulerIntervenant_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(3);
}

void PoliceStation::on_B_MConfirmerIntervenant_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du l'intervenant?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        Intervenant affaire;
        affaire.setCin(ui->CB_IDIntervenant->currentText().toInt());
        affaire.setNom(ui->LE_MNomIntervenant->text());
        affaire.setPrenom(ui->LE_MPrenomIntervenant->text());
        affaire.setNationalite(ui->CB_MNationaliteIntervenant->currentText());
        affaire.setLocalisation(ui->CB_MLocalIntervenant->currentText());
        affaire.setMail(ui->LE_MMailIntervenant->text());
        if(affaire.modifier()) {
            ui->CB_IDIntervenant->setModel(intervenant.listCin());
            ui->T_Intervenants->setModel(intervenant.afficher());
            INFORMER(ui->L_AlerteIntervenant,"Intervenat est Modifie",3000);
            ui->stackedWidget->setCurrentIndex(3);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_Trier_clicked()
{son->play();
    QString Tri = ui->CB_TriIntervenant->currentText();
    ui->T_Intervenants->setModel(intervenant.Trier(Tri));
}

void PoliceStation::on_LE_ChercherPrenom_textChanged(const QString &arg1)
{son->play();
    QString by = ui->CB_ChercherIntervenant->currentText();
    ui->T_Intervenants->setModel(intervenant.Chercher(arg1, by));
}

void PoliceStation::on_B_ResetTableIntervenant_clicked()
{son->play();
    ui->LE_ChercherPrenom->clear();
    ui->T_Intervenants->setModel(intervenant.afficher());
}

void PoliceStation::on_B_Chat_clicked()
{son->play();
    MessengerClient MS;
    MS.exec();
}

void PoliceStation::on_B_BackToGestions_2_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(2);
}

void PoliceStation::on_B_ModifierAffaire_clicked()
{son->play();
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDAffaire->currentText().toInt());
    qry.prepare("SELECT * FROM affaires where id=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MTypeAffaire->setText(qry.value(1).toString());
            ui->LE_MLocalAffaire->setText(qry.value(2).toString());
            ui->TE_MDescAffaire->setText(qry.value(3).toString());
            ui->LE_MCinIntAffaire->setText(qry.value(4).toString());
            ui->DE_MDateAffaire->setDate(qry.value(5).toDate());
        }
    }
    ui->stackedWidget->setCurrentIndex(8);
}

void PoliceStation::on_B_AjouterAffaire_clicked()
{son->play();
    ui->LE_ATypeAffaire->clear();
    ui->LE_ALocalAffaire->clear();
    ui->TE_ADescAffaire->clear();
    QDateTime curDataTime=QDateTime::currentDateTime();
    ui->DE_ADateAffaire->setDate(curDataTime.date());
    ui->CB_ACinIntAffaire->setModel(intervenant.listCin());
    ui->stackedWidget->setCurrentIndex(7);
}

void PoliceStation::on_B_SupprimerAffaire_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        affaire.setId(ui->CB_IDAffaire->currentText().toInt());
        if(affaire.supprimer(affaire.getId())) {
            qDebug() << "Suppression Complet";
            ui->T_Affaire->setModel(affaire.afficher());
            ui->CB_IDAffaire->setModel(affaire.listId());
            INFORMER(ui->L_AlerteAffaire,"Affaire est Supprime",3000);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_AAnnulerAffaire_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(6);
}

void PoliceStation::on_B_AConfirmerAffaire_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        int intervenant = ui->CB_ACinIntAffaire->currentText().toInt();
        QString type = ui->LE_ATypeAffaire->text();
        QString localisation = ui->LE_ALocalAffaire->text();
        QString description = ui->TE_ADescAffaire->toPlainText();
        QDate date = ui->DE_ADateAffaire->date();
        Affaire affaire(type, localisation, date, description, intervenant);
        if(affaire.ajouter()) {
            ui->T_Affaire->setModel(affaire.afficher());
            ui->CB_IDAffaire->setModel(affaire.listId());
            INFORMER(ui->L_AlerteAffaire,"Affaire est Ajoute",3000);
            ui->stackedWidget->setCurrentIndex(6);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_MAnnulerAffaire_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(6);
}

void PoliceStation::on_B_MConfirmerAffaire_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        affaire.setId(ui->CB_IDAffaire->currentText().toInt());
        affaire.setType(ui->LE_MTypeAffaire->text());
        affaire.setLocalisation(ui->LE_MLocalAffaire->text());
        affaire.setDate(ui->DE_MDateAffaire->date());
        affaire.setDescription(ui->TE_MDescAffaire->toPlainText());
        if(affaire.modifier()) {
            ui->T_Affaire->setModel(affaire.afficher());
            ui->CB_IDAffaire->setModel(affaire.listId());
            INFORMER(ui->L_AlerteAffaire,"Affaire est Modifie",3000);
            ui->stackedWidget->setCurrentIndex(6);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_GestionVehicule_2_clicked()
{son->play();
    ui->T_Vehicules_2->setModel(v_tmp.afficher());
    ui->CB_IDVehicule_2->setModel(v_tmp.afficherm());
    ui->cb_pv->setModel(policier.listId());
    ui->stackedWidget->setCurrentIndex(20);
}

void PoliceStation::on_B_GestionMission_3_clicked()
{son->play();
    ui->T_Mission_2->setModel(m_tmp.afficher());
    ui->CB_IDMission_2->setModel(m_tmp.affichern());
    ui->stackedWidget->setCurrentIndex(23);
}

void PoliceStation::on_B_GestionMission_4_clicked()
{son->play();
    ui->CB_IDAffVeh_2->setModel(v_tmp.afficherm());
    ui->CB_IDAffMiss_2->setModel(m_tmp.affichern());
    ui->T_Affectation_2->setModel(a_tmp.afficherA());
    ui->stackedWidget->setCurrentIndex(26);
}

void PoliceStation::on_B_BackToGestions_9_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(19);
}

void PoliceStation::on_B_BackToGestions_10_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(19);
}

void PoliceStation::on_B_AjouterVehicule_2_clicked()
{son->play();
    ui->LE_AMarqueVehicule_2->clear();
    ui->LE_AMatriculeVehicule_2->clear();
    ui->LE_ANbplacesVehicule_2->clear();
    ui->LE_ACouleurVehicule_2->clear();
    ui->LE_AQuantiteVehicule_2->clear();
    ui->cb_pv->clear();
    ui->stackedWidget->setCurrentIndex(21);
}

void PoliceStation::on_B_AAnnulerVehicule_2_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(20);
}

void PoliceStation::on_B_AConfirmerVehicule_2_clicked()
{son->play();
    bool overAll = false, matricule_B, marque_B, couleur_B, quantite_B,nb_places_B;
    int matricule = ui->LE_AMatriculeVehicule_2->text().toInt();
    QString matriculel = ui->LE_AMatriculeVehicule_2->text();
    QString marque = ui->LE_AMarqueVehicule_2->text();
    QString couleur = ui->LE_ACouleurVehicule_2->text();
    int quantite = ui->LE_AQuantiteVehicule_2->text().toInt();
    QString quantitel = ui->LE_AQuantiteVehicule_2->text();
    int nb_places = ui->LE_ANbplacesVehicule_2->text().toInt();
    QString nb_placesl = ui->LE_ANbplacesVehicule_2->text();
    int CIN_policier=ui->cb_pv->currentText().toInt();
    if(matriculel.length() < 3) {
        matricule_B = false;
        ui->matriculealert_2->setText("Il faut 3 characteres de facon XXX");
        ui->matriculealert_2->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        matricule_B = true;
        ui->matriculealert_2->setText("Ok");
        ui->matriculealert_2->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    if(marque.length() >10) {
        marque_B = false;
        ui->marquealert_2->setText("Il faut 10 charactere au maximum");
        ui->marquealert_2->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        marque_B = true;
        marque[0] = marque[0].toUpper();
        ui->marquealert_2->setText("Ok");
        ui->marquealert_2->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    if(couleur.length() < 3) {
        couleur_B = false;
        ui->couleuralert_2->setText("Il faut  au moins 3 charactere ");
        ui->couleuralert_2->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        couleur_B = true;
        couleur[0] = couleur[0].toUpper();
        ui->couleuralert_2->setText("Ok");
        ui->couleuralert_2->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    if(quantitel.length() >2) {
        quantite_B = false;
        ui->quantitealerte_2->setText("Il faut maximum 2 characteres ");
        ui->quantitealerte_2->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        quantite_B = true;
        ui->quantitealerte_2->setText("Ok");
        ui->quantitealerte_2->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    if(nb_placesl.length() >2) {
        nb_places_B = false;
        ui->nb_placesalert_2->setText("Il faut maximum 2 characteres ");
        ui->nb_placesalert_2->setStyleSheet("QLabel{color: red; font-size: 12px;}");
    }
    else {
        quantite_B = true;
        ui->nb_placesalert_2->setText("Ok");
        ui->nb_placesalert_2->setStyleSheet("QLabel{color: green; font-size: 12px;}");
    }
    /*************** END : Controle de Saisir L'ajout d'Intervenant ***************/

    /*************** BEGIN : Ajouter sur BaseDonnee */
    (matricule_B && marque_B && couleur_B && quantite_B && nb_places_B  )? overAll = true : overAll = false;
    if(overAll) {
        QMessageBox msgBox;
        vehicule E(matricule, marque, couleur, nb_places, quantite, CIN_policier);
        bool test=E.ajouter();
        if(test)
        {
            msgBox.setText("Ajout avec succés.");
            ui->T_Vehicules_2->setModel(v_tmp.afficher());
            QSqlQuery *query = new QSqlQuery();
            QSqlQueryModel * modal = new QSqlQueryModel();
            query->prepare("SELECT matricule from vehicules");
            query->exec();
            modal->setQuery(*query);
            ui->CB_IDVehicule_2->setModel(modal);
            ui->stackedWidget->setCurrentIndex(20);
        }
        else {
            msgBox.setText("Echec au niveau de l'ajout.");
        }
        msgBox.exec();
    }
}

void PoliceStation::on_B_ModifierVehicule_2_clicked()
{son->play();
    QString matricule = ui->CB_IDVehicule_2->currentText();
    ui->LE_MMatriculeVehicule_2->setText(matricule);
    QSqlQuery query;
    query.prepare("SELECT * from vehicules where matricule = :matricule");
    query.bindValue(":matricule" , matricule);
    if (query.exec())
    {
        while (query.next())
        {
            ui->LE_MMarqueVehicule_2->setText(query.value(1).toString());
            ui->LE_MQuantiteVehicule_2->setText(query.value(2).toString());
            ui->LE_MCouleurVehicule_2->setText(query.value(3).toString());
            ui->LE_MNbplacesVehicule_2->setText(query.value(4).toString());
            ui->LE_MCinVehicule_2->setText(query.value(5).toString());

        }

    }
    ui->stackedWidget->setCurrentIndex(22);
}

void PoliceStation::on_B_SupprimerVehicule_2_clicked()
{son->play();
    int matricule = ui->CB_IDVehicule_2->currentText().toInt();
    QMessageBox msgbox;
    bool test=v_tmp.supprimer(matricule);
    if (test) {
        msgbox.setText("Suppression avec succés.");
        ui->T_Vehicules_2->setModel(v_tmp.afficher());
        ui->T_Mission_2->setModel(m_tmp.afficher());
        QSqlQuery *query = new QSqlQuery();
        QSqlQueryModel * modal = new QSqlQueryModel();
        query->prepare("SELECT matricule from vehicules");
        query->exec();
        modal->setQuery(*query);
        ui->CB_IDVehicule_2->setModel(modal);
    }
    else
        msgbox.setText("Echec au niveau de la Suppression");
    msgbox.exec();
}

void PoliceStation::on_B_MAnuulerVehicule_2_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(20);
}

void PoliceStation::on_B_MConfirmerVehicule_2_clicked()
{son->play();
    int matricule = ui->CB_IDVehicule_2->currentText().toInt();
    QString marque = ui->LE_MMarqueVehicule_2->text();
    QString couleur = ui->LE_MCouleurVehicule_2->text();
    int quantite=ui->LE_MQuantiteVehicule_2->text().toInt();
    int nb_places=ui->LE_MNbplacesVehicule_2->text().toInt();
    int CIN_policier=ui->LE_MCinVehicule_2->text().toInt();
    QMessageBox msgBox;
    bool test = v_tmp.modifier(marque, couleur, quantite, nb_places, matricule, CIN_policier);
    if (test) {
        ui->T_Vehicules_2->setModel(v_tmp.afficher());
        msgBox.setText("modification avec succes");
        n_tmp.notifications_modifiervehicule();
        ui->stackedWidget->setCurrentIndex(20);
    }
    else
        msgBox.setText("echec au niveau de la modification");
    msgBox.exec();
}

void PoliceStation::on_B_Trier_6_clicked()
{son->play();
    QString Tri = ui->comboBox_4->currentText();
    ui->T_Vehicules_2->setModel(v_tmp.Trier(Tri));
}

void PoliceStation::on_molka_2_textChanged(const QString &arg1)
{son->play();
    QString by=ui->rr_2->currentText();
    ui->T_Vehicules_2->setModel(v_tmp.rechercherv(arg1,by));
}

void PoliceStation::on_annulerv_2_clicked()
{son->play();
    ui->molka_2->clear();
    ui->T_Vehicules_2->setModel(v_tmp.afficher());
}

void PoliceStation::on_annulerM_2_clicked()
{son->play();
    ui->lineEdit_2->clear();
    ui->T_Mission_2->setModel(m_tmp.afficher());
}

void PoliceStation::on_B_Statistics_3_clicked()
{son->play();
    QPrinter printer (QPrinter::PrinterResolution);
    QPrintDialog dlg(&printer,this);
    if (dlg.exec() == QDialog::Rejected) {
        return;
    }
    QString strStream;
    QString currentDate = QDateTime().currentDateTime().toString();
    QTextStream out(&strStream);
    const int rowCount = ui->T_Vehicules_2->model()->rowCount();
    const int columnCount = ui->T_Vehicules_2->model()->columnCount();
    out <<
    "<html>\n"
    "<head>\n"
    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
    <<QString("<title>%1</title>\n").arg("strTitle")
    <<"</head>\n"
    "<body bgcolor=#ffffff link=#5000A0>\n"
    <<QString(currentDate)
    <<//"<align='right'> " << datefich << "</align>"
    "<center> <img src=""C:/Users/lenovo/Desktop/Smart_Plolice_Station_2A19/images/logo.png"" width=""100"" height=""100"" > <br> <br><H1>EXTRAIT DES VEHICULES</H1> <br> <br><table border=1 cellspacing=0 cellpadding=2>\n";
    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    if (!ui->T_Vehicules_2->isColumnHidden(column))
    out << QString("<th>%1</th>").arg(ui->T_Vehicules_2->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";
    // data table
    for (int row = 0; row < rowCount; row++)
    {
    out << "<tr> <td bkcolor=0>" << row+1 <<"</td>";
    for (int column = 0; column < columnCount; column++)
    {
    if (!ui->T_Vehicules_2->isColumnHidden(column))
    {
    QString data = ui->T_Vehicules_2->model()->data(ui->T_Vehicules_2->model()->index(row, column)).toString().simplified();
    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
    }
    }
    out << "</tr>\n";
    }
    out <<  "</table> </center>\n"
    "<br> <br> <br> <br>"
    "</body>\n"
    "<footer>\n"
    "<div class = ""container"">"
    "<div class = ""row"">"
    "<div>"
    "<div><img src="":/resources/images/fb.png""> <span>Debug Entity</div>\n"
    "<br>"
    "<div><img src="":/resources/images/insta.png""> <span>Debug_Entity </div>\n"
    "</div>"
    "</div>"
    "</div>"
    "</footer>\n"
    "</html>\n";
    QString filter = "pdf (*.pdf) ";
    QString fileName = QFileDialog::getSaveFileName(this, "save in", QDir::homePath(),filter);
    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName.append(".pdf");
    }
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);
    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size());
    doc.print(&printer);
    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);
}

void PoliceStation::on_B_GestionEquipement_clicked()
{son->play();
    ui->CB_IDEquipement->setModel(equipement.listRef());
    ui->T_Equipement->setModel(equipement.afficher());
    ui->stackedWidget->setCurrentIndex(11);
}

void PoliceStation::on_B_GestionMaintenance_clicked()
{son->play();
    ui->CB_IDMaintenance->setModel(maintenance.listId());
    ui->T_Maintenance->setModel(maintenance.afficher());
    ui->stackedWidget->setCurrentIndex(14);
}

void PoliceStation::on_B_BackToGestions_4_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(10);
}

void PoliceStation::on_B_BackToGestions_5_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(10);
}

void PoliceStation::on_B_AjouterEquipement_clicked()
{son->play();
    ui->CB_CIN_Policier->setModel(equipement.listCIN());
    ui->LE_AReferenceEquipement->clear();
    ui->LE_AQuantiteEquipement->clear();
    ui->LE_ATailleEquipement->clear();
    ui->LE_AEtatEquipement->clear();
    ui->LE_APoidEquipement->clear();
    ui->LE_ANomEquipement->clear();
    ui->stackedWidget->setCurrentIndex(12);
}

void PoliceStation::on_B_BackToGestionEquipement_3_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(11);
}

void PoliceStation::on_B_AConfirmerEquipement_clicked()
{son->play();
   bool overAll = false, ref_b, quantite_b, taille_B, etat_B,poid_B ,nom_B ;
   int reference = ui->LE_AReferenceEquipement->text().toInt();
   QString ref_l=ui->LE_AReferenceEquipement->text();
   int quantite = ui->LE_AQuantiteEquipement->text().toInt();
   QString quantite_l=ui->LE_AQuantiteEquipement->text();
   QString taille = ui->LE_ATailleEquipement->text();
   QString etat = ui->LE_AEtatEquipement->text();
   int poid = ui->LE_APoidEquipement->text().toInt();
   QString poid_l=ui->LE_APoidEquipement->text();
   QString nom = ui->LE_ANomEquipement->text();
   int CIN_policier=ui->CB_CIN_Policier->currentText().toInt();
   if(ref_l.length()!=9) {
       ref_b = false;
       ui->AAlerteRef->setText("Il faut 9 charactere de facon XXXXXXXXXX");
       ui->AAlerteRef->setStyleSheet("QLabel{color: red; font-size: 12px;}");
   }
   else {
       ref_b = true;
       ui->AAlerteRef->setText("Ok");
       ui->AAlerteRef->setStyleSheet("QLabel{color: green; font-size: 12px;}");
   }
   if(quantite<5) {
       quantite_b = false;
       ui->AAlerteQuantite->setText("Il faut une quantite de 5 Minimum");
       ui->AAlerteQuantite->setStyleSheet("QLabel{color: red; font-size: 12px;}");
   }
   else {
       quantite_b = true;
       ui->AAlerteQuantite->setText("Ok");
       ui->AAlerteQuantite->setStyleSheet("QLabel{color: green; font-size: 12px;}");
   }
   if(taille.length() <2) {
       taille_B = false;
       ui->AAlerteTaille->setText("Il faut 3 charactere Minimum");
       ui->AAlerteTaille->setStyleSheet("QLabel{color: red; font-size: 12px;}");
   }
   else {
       taille_B = true;
       taille[0] = taille[0].toUpper();
       ui->AAlerteTaille->setText("Ok");
       ui->AAlerteTaille->setStyleSheet("QLabel{color: green; font-size: 12px;}");
   }
   if(etat.length() < 5) {
       etat_B = false;
       ui->AAlerteEtat->setText("Il faut 5 charactere Minimum");
       ui->AAlerteEtat->setStyleSheet("QLabel{color: red; font-size: 12px;}");
   }
   else {
       etat_B = true;
       taille[0] = taille[0].toUpper();
       ui->AAlerteEtat->setText("Ok");
       ui->AAlerteEtat->setStyleSheet("QLabel{color: green; font-size: 12px;}");
   }
   if(poid<1) {
       poid_B = false;
       ui->AAlertePoid->setText("Il faut un poid de 1kg Minimum");
       ui->AAlertePoid->setStyleSheet("QLabel{color: red; font-size: 12px;}");
   }
   else {
       poid_B = true;
       ui->AAlertePoid->setText("Ok");
       ui->AAlertePoid->setStyleSheet("QLabel{color: green; font-size: 12px;}");
   }
   if(nom.length() < 5) {
       nom_B = false;
       ui->AAlerteNom->setText("Il faut 5 charactere Minimum");
       ui->AAlerteNom->setStyleSheet("QLabel{color: red; font-size: 12px;}");
   }
   else {
       nom_B  = true;
       taille[0] = taille[0].toUpper();
       ui->AAlerteNom->setText("Ok");
       ui->AAlerteNom->setStyleSheet("QLabel{color: green; font-size: 12px;}");
   }
   (ref_b && quantite_b && taille_B && etat_B && poid_B && nom_B )? overAll = true : overAll = false;

   /*************** BEGIN : Ajouter sur BaseDonnee ***************/
   if(overAll) {
       QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout de l'equipement?", QMessageBox::Yes | QMessageBox::No);
       if(reply == QMessageBox::Yes) {
           Equipement equipement(reference, quantite, taille, etat, poid, nom, CIN_policier);
           if(equipement.ajouter()) {
               ui->CB_IDEquipement->setModel(equipement.listRef());
               ui->T_Equipement->setModel(equipement.afficher());
               ui->stackedWidget->setCurrentIndex(11);
               INFORMER(ui->labelmessage,"Equipement ajouter",3000);
           }
           else {
               QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
           }
       }
   }
}

void PoliceStation::on_B_SupprimerEquipement_clicked()
{son->play();
    Equipement equipement;
    int reference;
    int quantite;
    QString taille;
    QString etat;
    int poid;
    QString nom;
    int CIN_policier;
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression de l'equipement?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        equipement.setReference(ui->CB_IDEquipement->currentText().toInt());
        QSqlQuery qry;
        QString ref_string = QString::number(ui->CB_IDEquipement->currentText().toInt());
        qry.prepare("SELECT * FROM equipement where reference= :reference");
        qry.bindValue(0, ref_string);
        if(qry.exec()) {
            while(qry.next()) {
                reference=qry.value(0).toInt();
                quantite=qry.value(1).toInt();
                taille=qry.value(2).toString();
                etat=qry.value(3).toString();
                poid=qry.value(4).toInt();
                nom=qry.value(5).toString();
                CIN_policier=qry.value(6).toInt();

            }
        }
        qDebug()<<reference <<CIN_policier;
        Poubelle poubelle (reference,quantite,taille,etat,poid,nom,CIN_policier );

        if(equipement.supprimer(equipement.getReference())) {
            qDebug() << "Suppression Complet";
            poubelle.ajouter();
            ui->T_Equipement->setModel(equipement.afficher());
            ui->CB_IDEquipement->setModel(equipement.listRef());
            INFORMER(ui->labelmessage,"Equipement suprimer",3000);
        }

        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_ModifierEquipement_clicked()
{son->play();
    QSqlQuery qry;
    QString ref_string = QString::number(ui->CB_IDEquipement->currentText().toInt());
    qry.prepare("SELECT * FROM equipement where reference= :reference");
    qry.bindValue(0, ref_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->CB_CIN_Policier_2->setModel(equipement.listCIN());
            ui->LE_MReference->setText(qry.value(0).toString());
            ui->LE_MQuantiteEquipement->setText(qry.value(1).toString());
            ui->LE_MTailleEquipement->setText(qry.value(2).toString());
            ui->LE_MEtatEquipement->setText(qry.value(3).toString());
            ui->LE_MPoidEquipement->setText(qry.value(4).toString());
            ui->LE_MNomEquipement->setText(qry.value(5).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(13);
}

void PoliceStation::on_B_BackToGestionsEquipement_4_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(11);
}

void PoliceStation::on_B_MConfirmerEquipement_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer modification de l'equipement?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        int reference = ui->LE_MReference->text().toInt();
        int quantite = ui->LE_MQuantiteEquipement->text().toInt();
        QString taille = ui->LE_MTailleEquipement->text();
        QString etat = ui->LE_MEtatEquipement->text();
        int poid = ui->LE_MPoidEquipement->text().toInt();
        QString nom = ui->LE_MNomEquipement->text();
        int CIN_policier=ui->CB_CIN_Policier_2->currentText().toInt();
        Equipement equipement(reference, quantite, taille, etat, poid, nom, CIN_policier);
        if(equipement.modifier()) {
            ui->CB_IDEquipement->setModel(equipement.listRef());
            ui->T_Equipement->setModel(equipement.afficher());
            ui->stackedWidget->setCurrentIndex(11);
            INFORMER(ui->labelmessage,"Equipement modifier",3000);

        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("Modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_Trier_2_clicked()
{son->play();
    QString Tri = ui->CB_TriEquipement->currentText();
    ui->T_Equipement->setModel(equipement.Trier(Tri));
}

void PoliceStation::on_LE_ChercherReference_textChanged(const QString &arg1)
{son->play();
    QString by = ui->CB_RechercheEquipement->currentText();
    ui->T_Equipement->setModel(equipement.Chercher(arg1, by));
}

void PoliceStation::on_B_ResetTableEquipement_clicked()
{son->play();
    ui->LE_ChercherReference->clear();
    ui->T_Equipement->setModel(equipement.afficher());
}

void PoliceStation::on_B_AjouterMaintenance_clicked()
{son->play();
    ui->LE_ACout->clear();
    ui->Cb_AMaintenance->setModel(equipement.listRef());
    ui->stackedWidget->setCurrentIndex(15);
}

void PoliceStation::on_B_BackToGestionMaintenance_5_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(14);
}

void PoliceStation::on_B_AConfirmerMaintenance_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout de l'equipement au maintenance?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        QDate date = ui->dateEditDebut_2->date();
        int cout = ui->LE_ACout->text().toInt();
        QDate date2 = ui->dateEdit_2Fin_2->date();
        int reference = ui->Cb_AMaintenance->currentText().toInt();
        Maintenance maintenance(date, cout , date2, reference);
        if(maintenance.ajouter()) {
            ui->CB_IDMaintenance->setModel(maintenance.listId());
            ui->T_Maintenance->setModel(maintenance.afficher());
            ui->stackedWidget->setCurrentIndex(14);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_SupprimerMaintenance_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la fixation de l'equipement?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        maintenance.setIdmaintenance(ui->CB_IDMaintenance->currentText().toInt());
        if(maintenance.supprimer(maintenance.getIdmaintenance())) {
            qDebug() << "Suppression Complet";
            ui->T_Maintenance->setModel(maintenance.afficher());
            ui->CB_IDMaintenance->setModel(maintenance.listId());
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_ModifierMaintenance_clicked()
{son->play();
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDMaintenance->currentText().toInt());
    qry.prepare("SELECT * FROM maintenance where idmaintenance=:idmaintenance");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->dateEditDebut->setDate(qry.value(1).toDate());
            ui->LE_MCout->setText(qry.value(2).toString());
            ui->dateEdit_2Fin->setDate(qry.value(3).toDate());
            ui->LE_MIDEquipement->setText(qry.value(4).toString());
            ui->CB_MEtat->setCurrentText(qry.value(5).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(16);
}

void PoliceStation::on_B_BackToGestionmaintenance_6_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(14);
}

void PoliceStation::on_B_MConfirmerMaintenance_clicked()
{son->play();
    QSqlQuery qry;
    QString etatp;
    QString id_string = QString::number(ui->CB_IDMaintenance->currentText().toInt());
    qry.prepare("SELECT * FROM maintenance where idmaintenance=:idmaintenance");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            etatp=qry.value(5).toString();
        }
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du maintenance?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        maintenance.setIdmaintenance(ui->CB_IDMaintenance->currentText().toInt());
        maintenance.setCout(ui->LE_MCout->text().toInt());
        maintenance.setDatedebut(ui->dateEditDebut_2->date());
        maintenance.setDatefin(ui->dateEdit_2Fin_2->date());
        maintenance.setReference(ui->LE_MIDEquipement->text().toInt());
        maintenance.setEtat(ui->CB_MEtat->currentText());
        if(maintenance.modifier()) {
            if(etatp!=maintenance.getEtat())
            {
                notification.notifications_supprimermaintenance();
            }
            ui->T_Maintenance->setModel(maintenance.afficher());
            ui->CB_IDMaintenance->setModel(maintenance.listId());
            ui->stackedWidget->setCurrentIndex(14);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_statestique_3_clicked()
{son->play();
    ui->text->clear();
    ui->stackedWidget->setCurrentIndex(18);
}

void PoliceStation::on_New_File_clicked()
{son->play();
    file_path="";
    ui->text->setText("");
}

void PoliceStation::on_New_Folder_clicked()
{son->play();
    QString file_name=QFileDialog::getOpenFileName(this,"choisir le fichier");
    QFile file(file_name);
    file_path=file_name;
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
       QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("impossible d'ouvrir le fichier.\n" "Cliquer Ok."), QMessageBox::Ok);
       return;
    }
    QTextStream in(&file);
    QString text=in.readAll();
    ui->text->setText(text);
    file.close();
}

void PoliceStation::on_Edit_clicked()
{son->play();
    QFile file(file_path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("impossible d'ouvrir le fichier.\n" "Cliquer Ok."), QMessageBox::Ok);
       return;
    }
    QTextStream out(&file);
    QString text=ui->text->toPlainText();
    ui->text->setText(text);
    out << text;
    file.flush();
    file.close();
}

void PoliceStation::on_save_clicked()
{son->play();
    QString file_name=QFileDialog::getSaveFileName(this,"choisir le fichier");
    QFile file(file_name);
    file_path=file_name;
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("impossible d'ouvrir le fichier.\n" "Cliquer Ok."), QMessageBox::Ok);
       return;
    }
    QTextStream out(&file);
    QString text=ui->text->toPlainText();
    ui->text->setText(text);
    out << text;
    file.flush();
    file.close();
}

void PoliceStation::on_Cut_clicked()
{son->play();
    ui->text->cut();
}

void PoliceStation::on_copy_clicked()
{son->play();
    ui->text->copy();
}

void PoliceStation::on_paste_clicked()
{son->play();
    ui->text->paste();
}

void PoliceStation::on_refresh_clicked()
{son->play();
    ui->text->redo();
}

void PoliceStation::on_controlZ_clicked()
{son->play();
    ui->text->undo();
}

void PoliceStation::on_FBack_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(14);
}

void PoliceStation::on_statestique_clicked()
{son->play();
    int fixer=maintenance.sfixer();
    int nonfixer=maintenance.snfixer();
    qDebug()<<fixer;
    qDebug()<<nonfixer;

    QPieSeries *series = new QPieSeries();
    series->append("Fixer",fixer);
    series->append("Non Fixer",nonfixer);
    series->setHoleSize(0.5);
    series->setPieSize(0.8);

    QPieSlice *f = series->slices().at(0);
    QPieSlice *nf = series->slices().at(1);
    /*********************** Labels
    tn->setLabelVisible(true);
    fr->setLabelVisible(true);
    ag->setLabelVisible(true);
    lb->setLabelVisible(true);
    autres->setLabelVisible(true);
    ******************************/
    f->setBrush(Qt::red);
    nf->setBrush(Qt::blue);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Etat des equipements");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->F_Statisticc);
    ui->stackedWidget->setCurrentIndex(17);
}

void PoliceStation::on_pushButton_9_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(14);
}

void PoliceStation::on_B_GestionCitoyen_clicked()
{son->play();
    ui->T_Citoyens ->setModel(C.afficher());
    ui->CB_IDCitoyen ->setModel(C.listId());
    ui->stackedWidget->setCurrentIndex(28);
}

void PoliceStation::on_B_GestioService_clicked()
{son->play();
    ui->T_Service ->setModel(S.afficher());
    ui->CB_IDService->setModel(S.listId());
    ui->CB_NomCitoyen->setModel(C.listId());
    ui->CB_Idpolicier->setModel(policier.listId());

    ui->stackedWidget->setCurrentIndex(32);
}

void PoliceStation::on_B_BackToGestions_6_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(27);
}

void PoliceStation::on_B_BackToGestions_7_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(27);
}

void PoliceStation::on_B_AjouterCitoyen_clicked()
{son->play();
    //ui->LE_ANomCitoyen ->clear();
    //ui->LE_APrenomCitoyen ->clear();
    //ui->LE_ALieuNaissCit->clear();
    //ui->LE_AMailCitoyen->clear();
    //ui->LE_AAdresseCit ->clear();
    //ui->LE_ANomPereCit ->clear();
    //ui->LE_AProfessionCit->clear();
    //ui->LE_AEtatCitoyen ->clear();
    ui->stackedWidget->setCurrentIndex(29);
}

void PoliceStation::on_B_AAnnulerCitoyen_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(28);
}

void PoliceStation::on_B_AConfirmerCitoyen_clicked()
{son->play();
    bool overAll = false, nom_B,prenom_B ;

    QString nom = ui->LE_ANomCitoyen->text();
    QString prenom = ui->LE_APrenomCitoyen->text();
    QDate date_Naiss= ui->DE_ADateNaissCit->date();
    QString lieu_Naiss = ui->LE_ALieuNaissCit->text();
    QString mail = ui->LE_AMailCitoyen->text();
    QString adresse = ui->LE_AAdresseCit->text();
    QString nom_pere = ui->LE_ANomPereCit->text();
    QString profession = ui->CB_Aprofession ->currentText();
    QString etat_civil = ui->CB_Aetatciv->currentText();
    QString region = ui->CB_Aregion->currentText();
        if(nom.length() < 3) {
                nom_B = false;
                ui->L_ANomCitoyenA->setText(" 3 characteres minimum pour le Nom");
                ui->L_ANomCitoyenA->setStyleSheet("QLabel{color: red; font-size: 12px;}");
            }
            else {
                nom_B = true;
                nom[0] = nom[0].toUpper();
                ui->L_ANomCitoyenA->setText("Validé");
                ui->L_ANomCitoyenA->setStyleSheet("QLabel{color: green; font-size: 12px;}");
            }
        if(prenom.length() < 3) {
               prenom_B = false;
                ui->L_APrenomCitoyenA->setText(" 3 characteres minimum pour le Prenom");
                ui->L_APrenomCitoyenA->setStyleSheet("QLabel{color: red; font-size: 12px;}");
            }
            else {
                prenom_B = true;
                prenom[0] = prenom[0].toUpper();
                ui->L_APrenomCitoyenA->setText("Validé");
                ui->L_APrenomCitoyenA->setStyleSheet("QLabel{color: green; font-size: 12px;}");
            }
(nom_B&& prenom_B )? overAll = true : overAll = false;
 if(overAll){


  QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du citoyen?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {

       Citoyen C(nom,prenom,date_Naiss,lieu_Naiss,mail, adresse,nom_pere,profession,etat_civil,region);
       if(C.ajouter()) {
                ui->T_Citoyens ->setModel(C.afficher());
                ui->CB_IDCitoyen ->setModel(C.listId());
                ui->stackedWidget->setCurrentIndex(28);

                ui->LE_ANomCitoyen ->setText("");
                ui->LE_APrenomCitoyen ->setText("");
                //ui->DE_ADateNaissCit->setDate();
                ui->LE_ALieuNaissCit->setText("");
                ui->LE_AMailCitoyen->setText("");
                ui->LE_AAdresseCit ->setText("");
                ui->LE_ANomPereCit ->setText("");
                ui->CB_Aprofession->setCurrentText("");
                ui->CB_Aetatciv ->setCurrentText("");
                ui->CB_Aregion ->setCurrentText("");
                N.notifications_ajoutercitoyen();
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
    }
    }
 }
}

void PoliceStation::on_B_AccederProfil_clicked()
{son->play();
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
            ui->LE_Pregion->setText(qry.value(10).toString());
            ui->stackedWidget->setCurrentIndex(31);
        }
    }
}

void PoliceStation::on_B_SupprimerCitoyen_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du lcitoyen?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        C.setid(ui->CB_IDCitoyen ->currentText().toInt());
        if(C.supprimer(C.getid())) {
            qDebug() << "Suppression Complet";
            ui->T_Citoyens ->setModel(C.afficher());
            ui->CB_IDCitoyen ->setModel(C.listId());
            ui->stackedWidget->setCurrentIndex(28);
            N.notifications_supprimercitoyen();
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_MConfirmerCitoyen_2_clicked()
{son->play();
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
            ui->CB_Mprofession->setCurrentText(qry.value(8).toString());
            ui->CB_Metatciv->setCurrentText(qry.value(9).toString());
            ui->CB_Mregion->setCurrentText(qry.value(10).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(30);
}

void PoliceStation::on_B_MAnnulerCitoyen_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(31);
}

void PoliceStation::on_B_MConfirmerCitoyen_clicked()
{son->play();
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
        C.setprofession(ui->CB_Mprofession ->currentText());
        C.setetatcivil(ui->CB_Metatciv ->currentText());
        C.setregion(ui->CB_Mregion ->currentText());
qDebug() << C.getdatenaiss();
        if(C.modifier()) {
            ui->T_Citoyens->setModel(C.afficher());
            ui->CB_IDCitoyen->setModel(C.listId());
            ui->stackedWidget->setCurrentIndex(28);
            ui->LE_ANomCitoyen->setText("");
            ui->LE_APrenomCitoyen->setText("");
            //QDate date_Naiss = ui->DE_MDateNaissCit->date();
          //  ui->DE_MDateNaissCit->setDate();
            ui->LE_ALieuNaissCit->setText("");
            ui->LE_AMailCitoyen->setText("");
            ui->LE_AAdresseCit->setText("");
            ui->LE_ANomPereCit->setText("");
            ui->CB_Aprofession->setCurrentText("");
            ui->CB_Aetatciv->setCurrentText("");
             ui->CB_Aregion->setCurrentText("");
            N.notifications_modifiercitoyen();
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }

}

void PoliceStation::on_B_EnvoyeMail_clicked()
{son->play();
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDCitoyen->currentText().toInt());
    qry.prepare("SELECT * FROM citoyens where id=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->lineEdit->setText(qry.value(5).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(35);
}

void PoliceStation::on_B_returnCitoyen_3_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(31);
}

void PoliceStation::on_pushButton_2_clicked()
{son->play();
    Smtp* smtp = new Smtp("myriam.brahmi@esprit.tn",ui->mdp->text(), "smtp.gmail.com");
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    if( !files.isEmpty() )
        smtp->sendMail("myriam.brahmi@esprit.tn", ui->lineEdit->text() , ui->lineEdit_3->text(),ui->msg->toPlainText(),files);
    else
        smtp->sendMail("myriam.brahmi@esprit.tn", ui->lineEdit->text() , ui->lineEdit_3->text(),ui->msg->toPlainText());
    ui->stackedWidget->setCurrentIndex(31);
    N.notifications_envoyermail();
}

void PoliceStation::on_browseBtn_clicked()
{son->play();
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

void PoliceStation::on_B_Trier_3_clicked()
{son->play();
    QString Tri = ui->CB_Tricitoyen->currentText();
    ui->T_Citoyens->setModel(C.Trier(Tri));
}

void PoliceStation::on_LE_recherche_textChanged(const QString &arg1)
{
    QString by=ui->CB_recherche->currentText();
    ui->T_Citoyens->setModel(C.rechercher(arg1,by));
}

void PoliceStation::on_B_ResetTableIntervenant_2_clicked()
{son->play();
    ui->LE_recherche->clear();
    ui->T_Citoyens ->setModel(C.afficher());
}

void PoliceStation::on_B_ServiceAdminis_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(27);
}

void PoliceStation::on_B_returnCitoyen_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(28);
}

void PoliceStation::on_B_AjouterService_clicked()
{son->play();
    //ui->LE_ATypeService ->clear();
    //ui->LE_ADureeService ->clear();
    //ui->LE_APapierService->clear();
    //ui->TE_ADescService->clear();
    ui->stackedWidget->setCurrentIndex(33);
}

void PoliceStation::on_B_AAnnulerService_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(32);
}

void PoliceStation::on_B_ModifierService_clicked()
{son->play();
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDService->currentText().toInt());
    QString duree_string = QString::number(ui->LE_MDureeService->text().toInt());
    qry.prepare("SELECT * FROM services where id=:id");
    qry.bindValue(0, id_string);
    qry.bindValue(2, duree_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MTypeService->setText(qry.value(1).toString());
            ui->LE_MDureeService->setText(qry.value(2).toString());
            ui->LE_MPapierService->setText(qry.value(3).toString());
            ui->TE_MDescService->setText(qry.value(4).toString());
            ui->LE_Nomci->setText(qry.value(5).toString());
             ui->LE_MIdPolicier->setText(qry.value(6).toString());

        }
    }
    ui->stackedWidget->setCurrentIndex(34);
}

void PoliceStation::on_B_AConfirmerService_clicked()
{son->play();
    bool overAll = false, descrip_B;
       QString libelle = ui->LE_ATypeService->text();
       int duree = ui->LE_ADureeService->text().toInt();
       QString duree_l = ui->LE_ADureeService->text();
       QString papiers_necess = ui->LE_APapierService->text();
       QString description = ui->TE_ADescService->toPlainText();
       int id_citoyen=ui->CB_NomCitoyen->currentText().toInt();
        int id_policier=ui->CB_Idpolicier->currentText().toInt();

        if(description.length() < 8) {
               descrip_B = false;
               ui->L_ADescripServiceA->setText("Il faut au minimum 20 characteres ");
               ui->L_ADescripServiceA->setStyleSheet("QLabel{color: red; font-size: 12px;}");
           }
           else {
               descrip_B = true;
               ui->L_ADescripServiceA->setText("Validé");
               ui->L_ADescripServiceA->setStyleSheet("QLabel{color: green; font-size: 12px;}");
           }
         (descrip_B )? overAll = true : overAll = false;
        if(overAll) {
QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du service?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
   Service S(libelle,duree,papiers_necess,description,id_citoyen,id_policier);
   if(S.ajouter()) {
       ui->T_Service ->setModel(S.afficher());
       ui->CB_IDService ->setModel(S.listId());
       ui->CB_Idpolicier ->setModel(S.listId());
       ui->stackedWidget->setCurrentIndex(32);

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
        }
}

void PoliceStation::on_B_MAnnulerService_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(32);
}

void PoliceStation::on_B_MConfirmerService_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du service?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        Service S;
        S.setid(ui->CB_IDService ->currentText().toInt());
        S.setlibelle(ui->LE_MTypeService ->text());
        S.setduree(ui->LE_MDureeService ->text().toInt());
        S.setpapiersnecess(ui->LE_MPapierService ->text());
        S.setdescription(ui->TE_MDescService->toPlainText());
       // S.setid_citoyen(ui->LE_Nomci->text().toInt());


qDebug()<<S.getdescription();

        if(S.modifier()) {
            ui->T_Service->setModel(S.afficher());
            ui->CB_IDService->setModel(S.listId());
            ui->stackedWidget->setCurrentIndex(32);
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

}

void PoliceStation::on_B_SupprimerService_clicked()
{son->play();
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
}

void PoliceStation::on_B_TrierS_clicked()
{son->play();
    QString Tri = ui->CB_TriService->currentText();
    ui->T_Service->setModel(S.Trier(Tri));
}

void PoliceStation::on_LE_rechercheS_textChanged(const QString &arg1)
{son->play();
    QString by=ui->CB_rechercheS->currentText();
    ui->T_Service->setModel(S.rechercher(arg1,by));
}

void PoliceStation::on_B_ResetTableIntervenant_3_clicked()
{son->play();
    ui->LE_rechercheS->clear();
    ui->T_Service ->setModel(S.afficher());
}

void PoliceStation::on_pdf_clicked()
{son->play();
    QPrinter printer (QPrinter::PrinterResolution);
    QPrintDialog dlg(&printer,this);
    if (dlg.exec() == QDialog::Rejected)
    {
        return;
    }
    QString strStream;
    QString currentDate = QDateTime().currentDateTime().toString();
    QTextStream out(&strStream);
    const int rowCount = ui->T_Service->model()->rowCount();
    const int columnCount = ui->T_Service->model()->columnCount();
    out <<
     "<html>\n"
    "<head>\n"
    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
    <<QString("<title>%1</title>\n").arg("strTitle")
    <<"</head>\n"
    "<body bgcolor=#ffffff link=#5000A0>\n"
     <<QString(currentDate)
    <<//"<align='right'> " << datefich << "</align>"
    "<center> <img src=""lien logo mtaa application"" width=""100"" height=""100"" > <br> <br><H1>EXTRAIT DE NAISSANCE</H1> <br> <br><table border=1 cellspacing=0 cellpadding=2>\n";
    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    if (!ui->T_Service->isColumnHidden(column))
    out << QString("<th>%1</th>").arg(ui->T_Service->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";
    for (int row = 0; row < rowCount; row++)
    {
    out << "<tr> <td bkcolor=0>" << row+1 <<"</td>";
    for (int column = 0; column < columnCount; column++)
    {
    if (!ui->T_Service->isColumnHidden(column))
    {
    QString data = ui->T_Service->model()->data(ui->T_Service->model()->index(row, column)).toString().simplified();
    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
    }
    }
    out << "</tr>\n";
    }
    out <<  "</table> </center>\n"
    "<br> <br> <br> <br>"
    "</body>\n"
    "<footer>\n"
    "<div class = ""container"">"
    "<div class = ""row"">"
    "<div>"
    "<div><img src="":/ressources/image/logoo.png"" width=""60"" height=""60""> <span>DEBUG_ENTITY</div>\n"
    "<br>"
    "</div>"
    "</div>"
    "</div>"
    "</footer>\n"
    "</html>\n";
    QString filter = "pdf (*.pdf) ";
    QString fileName = QFileDialog::getSaveFileName(this, "save in", QDir::homePath(),filter);
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);
    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size());
    doc.print(&printer);
    N.notifications_pdfservice();
}

void PoliceStation::on_B_AjouterMission_2_clicked()
{son->play();
    ui->LE_ANomMission_2->clear();
    ui->LE_ALocalMission_2->clear();
    ui->TE_ADescMission_2->clear();
    ui->stackedWidget->setCurrentIndex(24);
}

void PoliceStation::on_B_AAnnulerMission_2_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(23);
}

void PoliceStation::on_B_AConfirmerMission_2_clicked()
{son->play();
    int id=0;
    QString nom = ui->LE_ANomMission_2->text();
    QDate datem = ui->dateEdit_3->date();
    QString localisation = ui->LE_ALocalMission_2->text();
    QString description = ui->TE_ADescMission_2->toPlainText();
    QMessageBox msgBox;
    missions E( id, nom, datem, localisation, description);
    bool test=E.ajouter();
    if(test)
    {
        msgBox.setText("Ajout avec succés.");
        ui->T_Mission_2->setModel(m_tmp.afficher());
        ui->CB_IDAffMiss_2->setModel(m_tmp.affichern());
        QSqlQuery *query = new QSqlQuery();
        QSqlQueryModel * modal = new QSqlQueryModel();
        query->prepare("SELECT id from missions");
        query->exec();
        modal->setQuery(*query);
        ui->CB_IDMission_2->setModel(modal);
        ui->stackedWidget->setCurrentIndex(23);
    }
}

void PoliceStation::on_B_ModifierMission_2_clicked()
{son->play();
    int id = ui->CB_IDMission_2->currentText().toInt();
    QSqlQuery query;
    query.prepare("SELECT * from missions where id = :id");
    query.bindValue(":id" , id);
    if (query.exec())
    {
        while (query.next())
        {
            ui->LE_MNomMission_2->setText(query.value(0).toString());
            ui->dateEdit_4->setDate(query.value(1).toDate());
            ui->LE_MLocalMission_2->setText(query.value(2).toString());
            ui->TE_MDescMission_2->setText(query.value(3).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(25);
}

void PoliceStation::on_B_MAnnulerMission_2_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(23);
}

void PoliceStation::on_B_MConfirmerMission_2_clicked()
{son->play();
    int id = ui->CB_IDMission_2->currentText().toInt();
    QString nom = ui->LE_MNomMission_2->text();
    QDate datem = ui->dateEdit_4->date();
    QString localisation = ui->LE_MLocalMission_2->text();
    QString description = ui->TE_MDescMission_2->toPlainText();
    QMessageBox msgBox;
    bool test = m_tmp.modifier(id, nom, datem, localisation, description);
    if (test)
    {
        ui->T_Mission_2->setModel(m_tmp.afficher());
        msgBox.setText("modification avec succes");
        n_tmp.notifications_modifiermission();
        ui->stackedWidget->setCurrentIndex(23);
    }
    else
        msgBox.setText("echec au niveau de la modification");
    msgBox.exec();
}

void PoliceStation::on_B_SupprimerMission_2_clicked()
{son->play();
    int id = ui->CB_IDMission_2->currentText().toInt();
    QMessageBox msgbox;
    bool test=m_tmp.supprimer(id);
    if (test)
    {
        msgbox.setText("Suppression avec succés.");
        ui->T_Mission_2->setModel(m_tmp.afficher());
        QSqlQuery *query = new QSqlQuery();
        QSqlQueryModel * modal = new QSqlQueryModel();
        query->prepare("SELECT id from missions");
        query->exec();
        modal->setQuery(*query);
        ui->CB_IDMission_2->setModel(modal);
    }
    else
        msgbox.setText("Echec au niveau de la Suppression");
    msgbox.exec();
}

void PoliceStation::on_B_Trier_7_clicked()
{son->play();
    QString Tri = ui->comboBox_6->currentText();
    ui->T_Mission_2->setModel(m_tmp.Trierm(Tri));
}

void PoliceStation::on_lineEdit_2_textChanged(const QString &arg1)
{son->play();
    QString by=ui->comboBox_5->currentText();
     ui->T_Mission_2->setModel(m_tmp.rechercher(arg1,by));
}

void PoliceStation::on_export_excel_2_clicked()
{son->play();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                        tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
        return;
    ExportExcelObject obj(fileName, "Sheet1", ui->T_Mission_2);
    obj.addField(0, "nom", "char(20)");
    obj.addField(1, "datem", "char(20)");
    obj.addField(2, "localisation", "char(20)");
    obj.addField(3, "description", "char(20)");
    obj.addField(4, "id", "char(20)");
    int retVal = obj.export2Excel();
    if( retVal > 0)
    {
        QMessageBox::information(this, tr("Done"), QString(tr("exported!")).arg(retVal));
    }
}

void PoliceStation::on_B_Trier_8_clicked()
{son->play();
    QString Tri = ui->cc_2->currentText();
    ui->T_Affectation_2->setModel(a_tmp.Triera(Tri));
}

void PoliceStation::on_annulera_2_clicked()
{son->play();
    ui->cc_2->clear();
    ui->T_Affectation_2->setModel(a_tmp.afficherA());
}

void PoliceStation::on_recherchera_2_textChanged(const QString &arg1)
{son->play();
    QString by=ui->lineedit_2->currentText();
    ui->T_Affectation_2->setModel(a_tmp.recherchera(arg1,by));
}

void PoliceStation::on_B_BackToGestions_11_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(19);
}

void PoliceStation::on_B_ConfirmerAffectation_2_clicked()
{son->play();
    int id_M = ui->CB_IDAffMiss_2->currentText().toInt();
    int matricule_V = ui->CB_IDAffVeh_2->currentText().toInt();
    QMessageBox msgBox;
    affectations A( id_M, matricule_V);
    bool test=A.ajouterA();
    if(test)
    {
        msgBox.setText("Ajout avec succés.");
        ui->T_Affectation_2->setModel(a_tmp.afficherA());
    }
}

void PoliceStation::on_supprimera_2_clicked()
{son->play();
    int id_M = ui->CB_IDAffMiss_2->currentText().toInt();
    int matricule_V = ui->CB_IDAffVeh_2->currentText().toInt();
    QMessageBox msgbox;
    bool test=a_tmp.supprimerA(id_M, matricule_V);
    if (test)
    {
        msgbox.setText("Suppression avec succés.");
        ui->T_Affectation_2->setModel(a_tmp.afficherA());
        QSqlQuery *query = new QSqlQuery();
        QSqlQuery *qury = new QSqlQuery();
        QSqlQueryModel * modal = new QSqlQueryModel();
        QSqlQueryModel * modale = new QSqlQueryModel();
        query->prepare("SELECT id_mission from affectations");
        query->exec();
        modal->setQuery(*query);
        ui->CB_IDAffMiss_2->setModel(modal);
        qury->prepare("SELECT id_vehicule from affectations");
        qury->exec();
        modale->setQuery(*qury);
        ui->CB_IDAffVeh_2->setModel(modale);
    }
    else
        msgbox.setText("Echec au niveau de la Suppression");
    msgbox.exec();
}

void PoliceStation::on_B_BackToMenu_3_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(1);
}

void PoliceStation::on_B_Amende_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(37);
}

void PoliceStation::on_B_BackToMenu_5_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(1);
}

void PoliceStation::on_B_GestionPolicier_clicked()
{son->play();
    ui->T_Policier->setModel(policier.afficher_policier());
    ui->comboBoxCin->setModel(policier.listCin_policier());
    ui->stackedWidget->setCurrentIndex(38);
}

void PoliceStation::on_B_GestionAmende_2_clicked()
{son->play();
    ui->T_Amende->setModel(amende.afficher_amende());
    ui->CB_IDAmende->setModel(amende.listId_amende());
    ui->stackedWidget->setCurrentIndex(41);
}

void PoliceStation::on_B_BackToGestionAmende_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(37);
}

void PoliceStation::on_B_BackToGestionPolicier_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(37);
}

void PoliceStation::on_B_AjouterPolicier_clicked()
{son->play();
    ui->LE_ACin->clear();
    ui->LE_ANomPolicier->clear();
    ui->LE_APrenomPolicier->clear();
    ui->LE_AGradePolicier->clear();
    ui->LE_AMailPolicier->clear();
    ui->LE_AMdpPolicier->clear();
    ui->LE_ASecteurPolicier->clear();
    ui->stackedWidget->setCurrentIndex(39);
}

void PoliceStation::on_B_AAnnulerPolicier_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(38);
}

void PoliceStation::on_B_AConfirmerPolicier_clicked()
{son->play();
    QString msg;

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du Policier?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            int cin_policier= ui->LE_ACin->text().toInt();
            QString nom_policier = ui->LE_ANomPolicier->text();
            QString prenom_policier = ui->LE_APrenomPolicier->text();
            QString grade_policier = ui->LE_AGradePolicier->text();
            QString mail_policier = ui->LE_AMailPolicier->text();
            int mdp_policier = ui->LE_AMdpPolicier->text().toInt();
            QString secteur_policier = ui->LE_ASecteurPolicier->text();
            Policier policier(cin_policier, nom_policier, prenom_policier, grade_policier, mail_policier, mdp_policier, secteur_policier);
            msg="monsieur "+policier.getNom_policier()+" , Nous sommes heureux de vous avoir en tant que nouveau policier parmis nous    ";
           if(log->email_exist(mail_policier)){
               QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("cette adresse mail existe.\n" "Cliquer Ok."), QMessageBox::Ok);}
        else{
            if(policier.ajouter_policier()) {

                Smtp* smtp = new Smtp("policestaion2021@gmail.com", "Mokki3211", "smtp.gmail.com", 465);
                             connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));


                          smtp->sendMail("policestaion2021@gmail.com", ui->LE_AMailPolicier->text() , "confirmation d'ajout",msg);


                ui->comboBoxCin->setModel(policier.listCin_policier());
                ui->T_Policier->setModel(policier.afficher_policier());
                INFORMER(ui->ALERT_P,"AJOUT AVEC SUCCEES",3000);
                policier.addToHistory("Ajout du " ,"policier",  QString::number(cin_policier));

                ui->stackedWidget->setCurrentIndex(38);}

            else {
                QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
            }
        }
}
}

void PoliceStation::on_B_SupprimerPolicier_clicked()
{son->play();

     QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du Policier?", QMessageBox::Yes | QMessageBox::No);
     if(reply == QMessageBox::Yes) {
         int cin_policier = ui->comboBoxCin->currentText().toInt();
         policier.setCin_policier(ui->comboBoxCin->currentText().toInt());
         if(policier.supprimer_policier(policier.getCin_policier())) {
             qDebug() << "Suppression Complet";
             ui->T_Policier->setModel(policier.afficher_policier());
             ui->comboBoxCin->setModel(policier. listCin_policier());
             INFORMER(ui->ALERT_P,"SUPPRESSION AVEC SUCCEES",3000);
             policier.addToHistory("Suppression du " ,"policier",  QString::number(cin_policier));
         }
         else {
             QMessageBox::critical(nullptr, QObject::tr("Nope"),
                         QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
         }
     }
}

void PoliceStation::on_B_ModifierPolicier_clicked()
{son->play();
    QSqlQuery qry;
    QString cin_string = QString::number(ui->comboBoxCin->currentText().toInt());
    QString mdp_policier_string = QString::number(ui->LE_MMdpPolicier->text().toInt());
    qry.prepare("SELECT * FROM policier where cin_policier=:cin");
    qry.bindValue(0, cin_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MCinPolicier->setText(qry.value(0).toString());
            ui->LE_MNomPolicier->setText(qry.value(1).toString());
            ui->LE_MPrenomPolicier->setText(qry.value(2).toString());
            ui->LE_MGradePolicier->setText(qry.value(3).toString());
            ui->LE_MMailPolicier->setText(qry.value(4).toString());
            ui->LE_MMdpPolicier->setText(qry.value(5).toString());
            ui->LE_MSecteurPolicier->setText(qry.value(7).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(40);
}

void PoliceStation::on_B_MAnuulerPolicier_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(38);
}

void PoliceStation::on_B_MConfirmerPolicier_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du Policier?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        Policier amende;
        int cin_policier = ui->comboBoxCin->currentText().toInt();
        amende.setCin_policier(ui->comboBoxCin->currentText().toInt());
        amende.setNom_policier(ui->LE_MNomPolicier->text());
        amende.setPrenom_policier(ui->LE_MPrenomPolicier->text());
        amende.setGrade_policier(ui->LE_MGradePolicier->text());
        amende.setMail_policier(ui->LE_MMailPolicier->text());
        amende.setMdp_policier(ui->LE_MMdpPolicier->text().toInt());
        amende.setSecteur_policier(ui->LE_MSecteurPolicier->text());
        if(amende.modifier_policier()) {
            ui->comboBoxCin->setModel(policier.listCin_policier());
            ui->T_Policier->setModel(policier.afficher_policier());
            INFORMER(ui->ALERT_P,"MODIFICATION AVEC SUCCEES",3000);
            policier.addToHistory("Modification du " ,"policier",  QString::number(cin_policier));
            ui->stackedWidget->setCurrentIndex(38);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_Trier_4_clicked()
{son->play();
    QString Tri = ui->CB_TriPolicier->currentText();
    ui->T_Policier->setModel(policier.Trier_policier(Tri));
}

void PoliceStation::on_B_Recherche_clicked()
{son->play();
    QString nom_policier =ui->LE_ChercherNom->text();
    QString prenom_policier = ui->LE_ChercherPrenom->text();
    if (nom_policier != ""){
        ui->T_Policier->setModel(policier.Chercher_policier1( nom_policier)) ;
    }
    if (prenom_policier != ""){
        ui->T_Policier->setModel(policier.Chercher_policier2(prenom_policier)) ;
    }
}

void PoliceStation::on_B_ResetTablePolicier_clicked()
{son->play();
    ui->LE_ChercherNom->clear();
    ui->LE_ChercherPrenom->clear();
    ui->T_Policier->setModel(policier.afficher_policier());
}

void PoliceStation::on_B_imprimer_clicked()
{son->play();
    QString strStream;
    QTextStream out(&strStream);
    const int rowCount = ui->T_Policier->model()->rowCount();
    const int columnCount =ui->T_Policier->model()->columnCount();
    out <<  "<html>\n"
    "<head>\n"
    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
    <<  QString("<title>%1</title>\n").arg("strTitle")
    <<  "</head>\n"
    "<body bgcolor=#ffffff link=#5000A0>\n"
    "<table border=1 cellspacing=0 cellpadding=2>\n";
    // headers
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
      //  if (ui->table_journaliste->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(ui->T_Policier->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";
    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->T_Policier->isColumnHidden(column)) {
                QString data = ui->T_Policier->model()->data(ui->T_Policier->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
    "</body>\n"
    "</html>\n";
    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }
    delete document;
}

void PoliceStation::on_B_Excel_clicked()
{son->play();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                        tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
        return;
    EXCEL obj(fileName, "mydata", ui->T_Policier);
    //colums to export
    obj.addField(0, "cin_policier", "number");
    obj.addField(1, "nom_policier", "char(20)");
    obj.addField(2, "prenom_policier", "char(20)");
    obj.addField(3, "grade_policier", "char(20)");
    obj.addField(4, "mdp_policier", "char(20)");
    obj.addField(6, "secteur_policier", "char(20)");
    int retVal = obj.export2Excel();
    if( retVal > 0){
        QMessageBox::information(this, tr("Done"), QString(tr("%1 records exported!")).arg(retVal));
    }
}

void PoliceStation::on_B_AjouterAmende_clicked()
{son->play();
    ui->LE_AIDAmende->clear();
    ui->LE_ATypeAmende->clear();
    ui->LE_APrixAmende->clear();
    ui->LE_ADescAmende->clear();
    ui->LE_ALieuAmende->clear();
    ui->LE_ADateAmende->clear();
    ui->LE_ACINpolicier->clear();
    ui->CB_IDAmende->setModel(policier.listCin_policier());
    ui->stackedWidget->setCurrentIndex(42);
}

void PoliceStation::on_B_AConfirmerAmende_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout de l'Amende?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        int id_amende = ui->LE_AIDAmende->text().toInt();
        QString type_amende = ui->LE_ATypeAmende->text();
        float prix_amende = ui->LE_APrixAmende->text().toFloat() ;
        QString description_amende = ui->LE_ADescAmende->toPlainText();
        QString lieu_amende = ui->LE_ALieuAmende->text();
        QString date_amende = ui->LE_ADateAmende->text();
        int CIN_policier = ui->LE_ACINpolicier->text().toInt();
        Amende amende (id_amende, type_amende, prix_amende, description_amende, lieu_amende, date_amende,CIN_policier);
        if(amende.ajouter_amende()) {
            ui->T_Amende->setModel(amende.afficher_amende());
            ui->CB_IDAmende->setModel(amende.listId_amende());
            INFORMER(ui->ALERT_M,"AJOUT AVEC SUCCEES",3000);
            policier.addToHistory("Ajout d' " ,"amende",  QString::number(id_amende));
            ui->stackedWidget->setCurrentIndex(41);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_AAnnulerAmende_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(41);
}

void PoliceStation::on_B_SupprimerAmende_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression de l'Amende?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        amende.setId_amende(ui->CB_IDAmende->currentText().toInt());
        if(amende.supprimer_amende(amende.getId_amende())) {
            qDebug() << "Suppression Complet";
            ui->T_Amende->setModel(amende.afficher_amende());
            ui->CB_IDAmende->setModel(amende.listId_amende());
            INFORMER(ui->ALERT_M,"SUPPRESSION AVEC SUCCEES",3000);
            policier.addToHistory("Suppression d' " ,"amende",  QString::number(ui->CB_IDAmende->currentText().toInt()));
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_ModifierAmende_clicked()
{son->play();
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDAmende->currentText().toInt());
    qry.prepare("SELECT * FROM amende where id_amende=:id_amende");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MIDAmende->setText(qry.value(0).toString());
            ui->LE_MTypeAmende->setText(qry.value(1).toString());
            ui->LE_MPrixAmende->setText(qry.value(2).toString());
            ui->LE_MDescAmende->setText(qry.value(3).toString());
            ui->lineEdit_25->setText(qry.value(4).toString());
            ui->LE_MDateAmende->setText(qry.value(5).toString());
            ui->LE_MCINpolicier->setText(qry.value(6).toString());
        }
    }
    ui->CB_IDAmende->setModel(amende.listId_amende());
    ui->stackedWidget->setCurrentIndex(43);
}

void PoliceStation::on_B_MAnnulerAmende_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(41);
}

void PoliceStation::on_B_MConfirmerAmende_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification de l'Amende?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        amende.setId_amende(ui->LE_MIDAmende->text().toInt());
        amende.setType_amende(ui->LE_MTypeAmende->text());
        amende.setPrix_amende(ui->LE_MPrixAmende->text().toFloat());
        amende.setDescription_amende(ui->LE_MDescAmende->toPlainText());
        amende.setLieu_amende(ui->lineEdit_25->text());
        amende.setDate_amende(ui->LE_MDateAmende->text());
        amende.setCIN_policier(ui->LE_MCINpolicier->text().toInt());
        if(amende.modifier_amende()) {
            ui->T_Amende->setModel(amende.afficher_amende());
            ui->CB_IDAmende->setModel(amende.listId_amende());
            INFORMER(ui->ALERT_M,"MODIFICATION AVEC SUCCEES",3000);
            policier.addToHistory("Modification d' " ,"amende",  QString::number(ui->LE_MIDAmende->text().toInt()));
            ui->stackedWidget->setCurrentIndex(41);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_calculator_clicked()
{son->play();
    Calc cl;
    cl.exec();
}

void PoliceStation::on_pushButton_4_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(28);
}

void PoliceStation::on_statistics_clicked()
{son->play();
    QSqlQuery qry;
    int Autres=C.ProfA();
    int Em=C.ProfEm();
    int Pr=C.ProfPr();
    int Med=C.ProfMed();
    int Dir=C.ProfDir();

    QPieSeries *series = new QPieSeries();
    series->append("Employe", Em);
    series->append("Directeur",Dir);
    series->append("Medecin", Med);
    series->append("Professeur", Pr);
    series->append("Autres...", Autres);
    series->setHoleSize(0.5);
    series->setPieSize(0.8);

    QPieSlice *em = series->slices().at(0);
    QPieSlice *dir = series->slices().at(1);
    QPieSlice *med= series->slices().at(2);
    QPieSlice *pr= series->slices().at(3);
    em->setBrush(Qt::red);
    med->setBrush(Qt::blue);
    pr->setBrush(Qt::green);
    dir->setPen(QPen(Qt::green, 1));
    dir->setBrush(Qt::white);
    med->setBrush(Qt::darkBlue);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Profession des Citoyens");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->F_Statistic_2);
    QLineSeries *seriesss = new QLineSeries();
    seriesss->append(0, 12);
    seriesss->append(1, 8);
    seriesss->append(2, 10);
    seriesss->append(3, 8);
    seriesss->append(4, 4);
    QChart *charttt = new QChart();
    charttt->legend()->hide();
    charttt->addSeries(seriesss);
    charttt->createDefaultAxes();
    QFont font;
    font.setPixelSize(18);
    charttt->setTitleFont(font);
    charttt->setTitleBrush(QBrush(Qt::black));
    charttt->setTitle("Ages des Citoyens");
    QPen pen(QRgb(0x000000));
    pen.setWidth(5);
    seriesss->setPen(pen);
    charttt->setAnimationOptions(QChart::AllAnimations);
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->append("18-25ans",0);
    axisX->append("26-35ans",1);
    axisX->append("36-45ans",2);
    axisX->append("46-60ans",3);
    axisX->append("+60ans",4);
    charttt->setAxisX(axisX, seriesss);
    QChartView *chartViewww = new QChartView(charttt);
    chartViewww->setRenderHint(QPainter::Antialiasing);

    chartViewww->setParent(ui->F_StatisticAge_2);
    /********************* BEGIN : Bars->Localisation *********************/
    int NE=C.NE();
    int NO=C.NO();
    int CE=C.CE();
    int CO=C.CO();
    int SE=C.SE();
    int SO=C.SO();

    QBarSet *set0 = new QBarSet("Tunis, Bizerte..");
    QBarSet *set1 = new QBarSet("Beja, Le Kef..");
    QBarSet *set2 = new QBarSet("Sousse, Mehdia..");
    QBarSet *set3 = new QBarSet("Kairouan..");
    QBarSet *set4 = new QBarSet("Sfax, Gabes..");
    QBarSet *set5 = new QBarSet("Gafsa, Tozeur..");

    *set0 << NE << 0 << 0 << 0 << 0 << 0;
    *set1 << 0 << NO << 0 << 0 << 0 << 0;
    *set2 << 0 << 0 << CE << 0 << 0 << 0;
    *set3 << 0 << 0 << 0 << CO << 0 << 0;
    *set4 << 0 << 0 << 0 << 0 << SE << 0;
    *set5 << 0 << 0 << 0 << 0 << 0 << SO;

    QBarSeries *seriess = new QBarSeries();
    seriess->append(set0);
    seriess->append(set1);
    seriess->append(set2);
    seriess->append(set3);
    seriess->append(set4);
    seriess->append(set5);

    QChart *charts = new QChart();
    charts->addSeries(seriess);
    charts->setTitle("Localisation des Citoyens");
    charts->setAnimationOptions(QChart::AllAnimations);
    QStringList categories;
    categories << "Nord-Est" << "Nord-Ouest" << "Centre-Est" << "Centre-Ouest" << "Sud-Est" << "Sud-Ouest";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    charts->createDefaultAxes();
    charts->setAxisX(axis, seriess);
    charts->legend()->setVisible(true);
    charts->legend()->setAlignment(Qt::AlignBottom);
    QChartView *chartsView = new QChartView(charts);
    chartsView->setRenderHint(QPainter::Antialiasing);
    chartsView->setParent(ui->F_StatisticLocal_2);
    ui->stackedWidget->setCurrentIndex(36);
}

void PoliceStation::on_excel_clicked()
{son->play();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                        tr("Excel Files (*.xls)"));
        if (fileName.isEmpty())
            return;

        EXCEL obj(fileName, "mydata", ui->T_Citoyens);

        //colums to export

        obj.addField(0, "Nom", "char(50)");
        obj.addField(1, "Prenom", "char(50)");
        obj.addField(2, "Date_Naiss", "date");
         obj.addField(3, "Lieu_Naiss", "char(20)");
         obj.addField(4, "Mail", "char(50)");
         obj.addField(5, "Adresse", "char(50)");
         obj.addField(6, "Nom_pere", "char(20)");
         obj.addField(6, "Profession", "char(50)");
         obj.addField(6, "Etat_civil", "char(50)");
         obj.addField(6, "région", "char(20)");
}

void PoliceStation::myFunction()
{son->play();
    QTime time = QTime::currentTime();
    QString time_text =time.toString("hh : mm : ss");
    ui->time->setText(time_text);
    ui->time_7->setText(time_text);
    ui->time_8->setText(time_text);
    ui->time_9->setText(time_text);
    ui->time_10->setText(time_text);
    ui->time_4->setText(time_text);
}

void PoliceStation::on_export_excel_3_clicked()
{son->play();
    Smtp* smtp = new Smtp("molka.elabed@esprit.tn", "123mokki321", "smtp.gmail.com", 465);
        smtp->sendMail("molka.elabed@esprit.tn", "molka.elabed@esprit.tn" , "Signalisation Problème" ,ui->plainTextEdit->toPlainText());
        ui->plainTextEdit->clear();
}

void PoliceStation::on_statistics_2_clicked()
{son->play();
    QSqlQuery qry;
    int Autres=intervenant.NatA();
    int Tn=intervenant.NatTn();
    int Ag=intervenant.NatAg();
    int Fr=intervenant.NatFr();
    int Lb=intervenant.NatLb();

    QPieSeries *series = new QPieSeries();
    series->append("Tunisienne", Tn);
    series->append("Francaise", Fr);
    series->append("Algerienne", Ag);
    series->append("Libanaise", Lb);
    series->append("Autres...", Autres);
    series->setHoleSize(0.5);
    series->setPieSize(0.8);

    QPieSlice *tn = series->slices().at(0);
    QPieSlice *fr = series->slices().at(1);
    QPieSlice *ag = series->slices().at(2);
    QPieSlice *lb = series->slices().at(3);
   
    tn->setBrush(Qt::red);
    fr->setBrush(Qt::blue);
    ag->setBrush(Qt::green);
    lb->setPen(QPen(Qt::green, 1));
    lb->setBrush(Qt::white);
    fr->setBrush(Qt::darkBlue);
//a
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nationalite des Intervenants");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->F_Statistic);
}

void PoliceStation::on_qrcodeb_clicked()
{son->play();
    int tabequipement=ui->T_Equipement->currentIndex().row();
    QVariant idd=ui->T_Equipement->model()->data(ui->T_Equipement->model()->index(tabequipement,0));
    int id= idd.toInt();
    QSqlQuery qry;
    qry.prepare("select * from equipement where REFERENCE=:ref");
    qry.bindValue(":ref",id);
    qry.exec();
    QString QUANTITE,TAILLE,ETAT,POID,NOM,CIN_POLICIER,REFERENCE;
    while(qry.next()){
        QUANTITE=qry.value(1).toString();
        TAILLE=qry.value(2).toString();
        ETAT=qry.value(3).toString();
        POID=qry.value(4).toString();
        NOM=qry.value(5).toString();
        CIN_POLICIER=qry.value(6).toString();
    }
    REFERENCE=QString::number(id);
    REFERENCE="REFERENCE: "+REFERENCE+" QUANTITE: "+QUANTITE+" TAILLE: "+TAILLE+" ETAT: "+ETAT+ "POID: "+POID+ "NOM: "+NOM+ "CIN_POLICIER: "+CIN_POLICIER;
    QrCode qr = QrCode::encodeText(REFERENCE.toUtf8().constData(), QrCode::Ecc::HIGH);
    QImage im(qr.getSize(),qr.getSize(), QImage::Format_RGB888);
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            int color = qr.getModule(x, y);
            if(color==0)
                im.setPixel(x, y,qRgb(254, 254, 254));
            else
                im.setPixel(x, y,qRgb(0, 0, 0));
        }
    }
    im=im.scaled(200,200);
    ui->QRcode->setPixmap(QPixmap::fromImage(im));
}

void PoliceStation::on_B_Google_clicked()
{son->play();
    QString link="http://www.google.com";
    QDesktopServices::openUrl(QUrl(link));
}

void PoliceStation::on_B_Detenu_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(44);
}

void PoliceStation::on_B_BackToMenu_6_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(1);
}

void PoliceStation::on_B_BackToMenu_7_clicked()
{son->play();
    Cam xcam;
    xcam.exec();
}

void PoliceStation::on_signup_clicked()
{

}

void PoliceStation::on_sliderprogress_sliderMoved(int position)
{son->play();
   player->setPosition(position);
}

void PoliceStation::on_slidervolume_sliderMoved(int position)
{son->play();
  player->setVolume(position);
}

void PoliceStation::on_pushButton_3_clicked()
{son->play();
    player->setMedia(QUrl::fromLocalFile("/Users/myria/Desktop/new/Smart_Plolice_Station_2A19/cassette-player-button-3.wav"));
    player->play();
    qDebug()<<player->errorString();
}

void PoliceStation::on_pushButton_5_clicked()
{son->play();
    player->stop();
}

void PoliceStation::on_positionChanged(qint64 position)
{son->play();
    ui->sliderprogress->setValue(position);
}

void PoliceStation::on_durationChanged(qint64 position)
{son->play();
    ui->sliderprogress->setMaximum(position);
}

void PoliceStation::on_pushButton_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(46);
}

void PoliceStation::on_B_PolicierParticipants_clicked()
{son->play();
    ui->T_AffFourT->setModel(m_tmp.affectAll());
    ui->stackedWidget->setCurrentIndex(47);
}

void PoliceStation::on_B_RetourMiss_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(23);
}

void PoliceStation::on_pushButton_6_clicked()
{son->play();
    N.notifications_mdpoubl();
    QString code=log->code_generator();
if(log->update_mpd_reset(ui->lineEdit_Mail->text(),code)){
    Smtp* smtp = new Smtp("policestaion2021@gmail.com","Mokki3211", "smtp.gmail.com");
        smtp->sendMail("policestaion2021@gmail.com", ui->lineEdit_Mail->text() , "Récupération de votre compte","Veuillez saisir le code dans le champs du mot de passe.\nVotre Code est :"+code);
}
}

void PoliceStation::on_pushButton_8_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(1);
}

void PoliceStation::on_pushButton_10_clicked()
{son->play();
    ui->T_Detenu_2->setModel(detenu.affectAll());
    ui->stackedWidget->setCurrentIndex(60);
}

void PoliceStation::on_B_AjouterEquipement_2_clicked()
{son->play();
    ui->T_AFF_Equipement->setModel(equipement.afficherAffectation());
    ui->stackedWidget->setCurrentIndex(48);
}

void PoliceStation::on_B_AjouterEquipement_3_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(11);
}

void PoliceStation::on_pushButton_11_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(28);
}

void PoliceStation::on_map_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(49);
}

void PoliceStation::on_ret_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(28);
}

void PoliceStation::on_repart_clicked()
{son->play();
    ui->T_AffePS->setModel(S.affectjoint());
    ui->stackedWidget->setCurrentIndex(50);
}

void PoliceStation::on_B_historique_clicked()
{son->play();
    ui->listhistorique->setModel( policier.afficherHistorique(ui));
    ui->stackedWidget->setCurrentIndex(51);
}

void PoliceStation::on_combo_action_currentIndexChanged(const QString &arg1)
{son->play();
    ui->listhistorique->setModel( policier.afficherHistorique(ui));
}

void PoliceStation::on_comboBox_mp_currentIndexChanged(const QString &arg1)
{son->play();
    ui->listhistorique->setModel( policier.afficherHistorique(ui));
}

void PoliceStation::on_B_Retouuuur_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(37);
}

void PoliceStation::on_B_GestionInfraction_clicked()
{son->play();
    ui->T_Infraction->setModel(infraction.afficher());
    ui->CB_IDIntervenant_2->setModel(infraction.listId());
    ui->stackedWidget->setCurrentIndex(52);
}

void PoliceStation::on_B_GestioDetenus_clicked()
{son->play();
    ui->T_Detenu->setModel(detenu.afficher());
    ui->CB_IDDetenus->setModel(detenu.listId());
    ui->stackedWidget->setCurrentIndex(55);
}

void PoliceStation::on_B_GestionMission_2_clicked()
{son->play();
    ui->CB_CinDetAffectation->setModel(detenu.listId());
    ui->CB_IDInfAffectation->setModel(infraction.listId());
    ui->T_Affectation->setModel(affectation.afficherA());
    ui->stackedWidget->setCurrentIndex(59);
}

void PoliceStation::on_B_BackToGestions_12_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(44);
}

void PoliceStation::on_B_BackToGestions_8_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(44);
}

void PoliceStation::on_B_BackToGestions_14_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(44);
}

void PoliceStation::on_B_SupprimerDetenus_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        detenu.setId(ui->CB_IDDetenus->currentText().toInt());
        if(detenu.supprimer(detenu.getId())) {
            ui->T_Detenu->setModel(detenu.afficher());
            ui->CB_IDDetenus->setModel(detenu.listId());
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_AjouterDetenus_clicked()
{son->play();
    ui->LE_ACinDetenus->clear();
    ui->LE_ANomDetenus->clear();
    ui->LE_APrenomDetenus->clear();
    ui->LE_ANationalitelDetenus->clear();
    ui->DE_AHistoriqueDetenus->clear();
    ui->CB_APolicierResponsable->setModel(policier.listCin_policier());
    ui->stackedWidget->setCurrentIndex(56);
}

void PoliceStation::on_B_AAnnulerDetenus_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(55);
}

void PoliceStation::on_B_AConfirmerDetenus_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        int cin = ui->LE_ACinDetenus->text().toInt();
        QString nom = ui->LE_ANomDetenus->text();
        QString prenom = ui->LE_APrenomDetenus->text();
        QString nationalite = ui->LE_ANationalitelDetenus->text();
        QString danger = ui->CB_ANivDangDetenus->currentText();
        QString historique = ui->DE_AHistoriqueDetenus->toPlainText();
        QDate dn = ui->DE_ADateNaissDetenus->date();
        QDate da = ui->DE_ADateArrestDetenus->date();
        int cin_policier = ui->CB_APolicierResponsable->currentText().toInt();
        Detenu detenu(cin, nom, prenom, nationalite, dn, da, historique, danger, cin_policier);
        if(detenu.ajouter()) {
            ui->T_Detenu->setModel(detenu.afficher());
            ui->CB_IDDetenus->setModel(detenu.listId());
            ui->stackedWidget->setCurrentIndex(55);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_ModifierDetenus_clicked()
{son->play();
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDDetenus->currentText().toInt());
    qry.prepare("SELECT * FROM detenus where cin=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MCinDetenus->setText(qry.value(0).toString());
            ui->LE_MNomDetenus->setText(qry.value(1).toString());
            ui->LE_MPrenomDetenus->setText(qry.value(2).toString());
            ui->DE_MDateNaissDetenus->setDate(qry.value(3).toDate());
            ui->LE_MNationalitelDetenus->setText(qry.value(4).toString());
            ui->DE_MDateArrestDetenus->setDate(qry.value(5).toDate());
            ui->DE_MHistoriqueDetenus->setText(qry.value(6).toString());
            ui->CB_MNivDangDetenus->setCurrentText(qry.value(7).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(57);
}

void PoliceStation::on_B_MAnnulerDetenus_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(55);
}

void PoliceStation::on_B_Trier_5_clicked()
{son->play();
    QString Tri = ui->CB_TriInfr_2->currentText();
    ui->T_Detenu->setModel(detenu.Trier(Tri));
}

void PoliceStation::on_B_ResetTableIntervenant_4_clicked()
{son->play();
    ui->LE_Chercher_2->clear();
    ui->T_Detenu->setModel(detenu.afficher());
}

void PoliceStation::on_LE_Chercher_2_textChanged(const QString &arg1)
{son->play();
    QString by = ui->CB_ChercherInfr_2->currentText();
    ui->T_Detenu->setModel(detenu.Chercher(arg1, by));
}

void PoliceStation::on_B_MConfirmerDetenus_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        detenu.setId(ui->CB_IDDetenus->currentText().toInt());
        detenu.setNom(ui->LE_MNomDetenus->text());
        detenu.setNationalite(ui->LE_MNationalitelDetenus->text());
        detenu.setPrenom(ui->LE_MPrenomDetenus->text());
        detenu.setDanger(ui->CB_MNivDangDetenus->currentText());
        detenu.setDn(ui->DE_MDateNaissDetenus->date());
        detenu.setDa(ui->DE_MDateArrestDetenus->date());
        detenu.setHistorique(ui->DE_MHistoriqueDetenus->toPlainText());
        if(detenu.modifier()) {
            ui->T_Detenu->setModel(detenu.afficher());
            ui->CB_IDDetenus->setModel(detenu.listId());
            ui->stackedWidget->setCurrentIndex(55);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_SupprimerIntervenant_2_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la suppression", "Confirmer la suppression du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        infraction.setId(ui->CB_IDIntervenant_2->currentText().toInt());
        if(infraction.supprimer(infraction.getId())) {
            ui->T_Infraction->setModel(infraction.afficher());
            ui->CB_IDIntervenant->setModel(infraction.listId());
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"),
                        QObject::tr("Suppression a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_AjouterInfraction_clicked()
{son->play();
    ui->LE_ANomInfraction->clear();
    ui->CB_ADetInfraction->setModel(detenu.listId());
    ui->stackedWidget->setCurrentIndex(53);
}

void PoliceStation::on_B_AConfirmerInfraction_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de l'ajout", "Confirmer l'ajout du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        QString nom = ui->LE_ANomInfraction->text();
        QString type = ui->LE_ATypeInfraction_2->currentText();
        QString niveau = ui->CB_ANivInfraction->currentText();
        QString detenu = ui->CB_ADetInfraction->currentText();
        QDate di = ui->DE_ADatInfraction->date();
        Infraction infraction(nom, type, niveau, di, detenu);
        if(infraction.ajouter()) {
            ui->T_Infraction->setModel(infraction.afficher());
            ui->CB_IDIntervenant_2->setModel(infraction.listId());
            ui->stackedWidget->setCurrentIndex(52);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("L'ajout a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}

void PoliceStation::on_B_AAnnulerInfraction_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(52);
}

void PoliceStation::on_B_ModifierIntervenant_2_clicked()
{son->play();
    QSqlQuery qry;
    QString id_string = QString::number(ui->CB_IDIntervenant_2->currentText().toInt());
    qry.prepare("SELECT * FROM infractions where id=:id");
    qry.bindValue(0, id_string);
    if(qry.exec()) {
        while(qry.next()) {
            ui->LE_MNomInfraction->setText(qry.value(1).toString());
            ui->LE_MTypeInfraction->setCurrentText(qry.value(2).toString());
            ui->CB_MNivInfraction->setCurrentText(qry.value(3).toString());
            ui->DE_MDatInfraction->setDate(qry.value(4).toDate());
            ui->LE_MDetInf->setText(qry.value(5).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(54);
}

void PoliceStation::on_B_Statistics_2_clicked()
{son->play();
    QSqlQuery qry;
    int dis=infraction.dis();
    int as=infraction.as();
    int hi=infraction.hi();
    int vol=infraction.vol();
    int meurtre=infraction.meurtre();

    QPieSeries *series = new QPieSeries();
    series->append("discrimination", dis);
    series->append("attouchements sexuels", as);
    series->append("homicide involontaire", hi);
    series->append("Vol", vol);
    series->append("Meurtre", meurtre);
    series->setPieSize(0.8);

    QPieSlice *tn = series->slices().at(0);
    QPieSlice *fr = series->slices().at(1);
    QPieSlice *ag = series->slices().at(2);
    QPieSlice *lb = series->slices().at(3);
    //QPieSlice *autres = series->slices().at(4);
    /*********************** Labels
    tn->setLabelVisible(true);
    fr->setLabelVisible(true);
    ag->setLabelVisible(true);
    lb->setLabelVisible(true);
    autres->setLabelVisible(true);
    ******************************/
    tn->setBrush(Qt::red);
    fr->setBrush(Qt::blue);
    ag->setBrush(Qt::green);
    lb->setBrush(Qt::yellow);
    fr->setBrush(Qt::darkBlue);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Type d'infraction");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->F_Statistic_3);

    ui->stackedWidget->setCurrentIndex(58);
}

void PoliceStation::on_B_BackToGestions_13_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(52);
}

void PoliceStation::on_B_MAnnulerInfraction_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(52);
}

void PoliceStation::on_B_MConfirmerInfraction_clicked()
{son->play();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de la modification", "Confirmer la modification du l'affaire?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        infraction.setId(ui->CB_IDIntervenant_2->currentText().toInt());
        infraction.setNom(ui->LE_MNomInfraction->text());
        infraction.setType(ui->LE_MTypeInfraction->currentText());
        infraction.setNiveau(ui->CB_MNivInfraction->currentText());
        infraction.setDate(ui->DE_MDatInfraction->date());
        if(infraction.modifier()) {
            ui->T_Infraction->setModel(infraction.afficher());
            ui->CB_IDIntervenant->setModel(infraction.listId());
            ui->stackedWidget->setCurrentIndex(52);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Nope"), QObject::tr("La modification a échoué.\n" "Cliquer Ok."), QMessageBox::Ok);
        }
    }
}


void PoliceStation::on_B_ConfirmerAffectation_clicked()
{son->play();
    int id = ui->CB_IDInfAffectation->currentText().toInt();
    int cin = ui->CB_CinDetAffectation->currentText().toInt();
    QMessageBox msgBox;
    Affectation A( cin, id);
    bool test=A.ajouterA();
    if(test)
    {
        msgBox.setText("Ajout avec succés.");
         ui->T_Affectation->setModel(affectation.afficherA());
    }
}

void PoliceStation::on_pushButton_12_clicked()
{son->play();
    ui->stackedWidget->setCurrentIndex(55);
}

void PoliceStation::on_Poubelle_clicked()
{son->play();
    ui->T_Equipemen_suprime->setModel(poubelle.afficherPoubelle());
    ui->stackedWidget->setCurrentIndex(61);
}

void PoliceStation::on_vider_corbeille_clicked()
{son->play();
    poubelle.supprimer();
  ui->T_Equipemen_suprime->setModel(poubelle.afficherPoubelle());
}

void PoliceStation::on_EQ_retour_clicked()
{son->play();
     ui->stackedWidget->setCurrentIndex(11);
}
