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

void OknoBot::stopWszystkie()
{
    for (auto &box : groupBoxes) {
        qDebug() << box->getTitle();
    }
}
