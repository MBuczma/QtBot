/* OknoBot.cpp */
/*
 * Okno które przechowuje rzędy boxów z przyciskami do wysłania.
 */
#include "OknoBot.h"
#include "ui_OknoBot.h"

#include "GroupBoxControl.h"

OknoBot::OknoBot(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::OknoBot>())
{
    ui->setupUi(this);
    connect(ui->pushButton_DodajRzad, &QPushButton::clicked, this, &OknoBot::dodajRzad);
    connect(ui->pushButton_StartWszystkie, &QPushButton::clicked, this, &OknoBot::startWszystkie);
    connect(ui->pushButton_StopWszystkie, &QPushButton::clicked, this, &OknoBot::stopWszystkie);
}

OknoBot::~OknoBot() = default;

void OknoBot::dodajRzad()
{
    emit rozszerzOkno(70);
    qDebug() << "Przycisk dodajRzadPrzyciskow został naciśnięty.";

    GroupBoxControl *newGroupBox = new GroupBoxControl(this);
    ui->verticalLayout->insertWidget(ui->verticalLayout->count() - 1, newGroupBox);

    groupBoxes.push_back(newGroupBox); // Dodajemy nowy rząd do wektora
    stopWszystkie();
}

void OknoBot::startWszystkie()
{
    for (auto &box : groupBoxes) {
        qDebug() << box->wysylanieStart();
    }
}

void OknoBot::stopWszystkie()
{
    for (auto &box : groupBoxes) {
        qDebug() << box->wysylanieStop();
    }
}

QString OknoBot::getAllDataFromGroupBox()
{
    QString AllData = "";
    if (groupBoxes.empty() != true) {
        for (auto &box : groupBoxes) {
            qDebug() << box->getAllData();
            AllData += box->getAllData() + "\n";
        }
        return AllData;
    }
    return QString();
}

void OknoBot::setAllDataToGroupBox(QString zawartoscPliku)
{
    QStringList lines = zawartoscPliku.split("\n", Qt::SkipEmptyParts);

    for (int i = groupBoxes.size(); i < lines.count(); ++i) {
        dodajRzad();
    }

    for (int i = 0; i < lines.size(); ++i) {
        if (i < int(groupBoxes.size())) {
            groupBoxes[i]->setAllData(lines[i]);
        }
    }
}
