/* OknoBot.cpp */
/*
 * Okno które przechowuje rzędy boxów z przyciskami do wysłania.
 * Zawiera kontener dla wielu GroupBoxControl.
 * Obsługuje dodawanie, usuwanie i sterowanie wierszami.
 * Przechwytuje globalne klawisze i reaguje na hotkeye.
 */
#include "OknoBot.h"
#include "ui_OknoBot.h"

#include "GroupBoxControl.h"
#include "KeyMap.h"
#include "QTimer"

const short ROW_HEIGHT = 70;

OknoBot::OknoBot(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::OknoBot>())
{
    ui->setupUi(this);
    connect(ui->pushButton_DodajRzad, &QPushButton::clicked, this, &OknoBot::dodajRzad);
    connect(ui->pushButton_StartWszystkie, &QPushButton::clicked, this, &OknoBot::startWszystkie);
    connect(ui->pushButton_StopWszystkie, &QPushButton::clicked, this, &OknoBot::stopWszystkie);

    globalKeyListener = new GlobalKeyListener(this);
    connect(globalKeyListener, &GlobalKeyListener::keyPressed, this, &OknoBot::onKeyPressed);
    QTimer::singleShot(0, this, [this]() { globalKeyListener->start(); });

    ui->comboBox_StartALL->addItems({""});
    // Dodaj wszystkie klawisze z mapy KeyMap
    for (const auto &pair : KeyMap::getOrderedList()) {
        ui->comboBox_StartALL->addItem(pair.first);
    }
    ui->comboBox_StartALL->setMinimumWidth(90);

    ui->comboBox_StopALL->addItems({""});
    // Dodaj wszystkie klawisze z mapy KeyMap
    for (const auto &pair : KeyMap::getOrderedList()) {
        ui->comboBox_StopALL->addItem(pair.first);
    }
    ui->comboBox_StopALL->setMinimumWidth(90);
}

OknoBot::~OknoBot() = default;

void OknoBot::dodajRzad()
{
    emit rozszerzOkno(ROW_HEIGHT);
    qDebug() << "[OknoBot] Przycisk dodajRzadPrzyciskow został naciśnięty.";

    GroupBoxControl *newGroupBox = new GroupBoxControl(this);
    ui->verticalLayout->insertWidget(ui->verticalLayout->count() - 1, newGroupBox);
    connect(newGroupBox, &GroupBoxControl::zadajUsuniecie, this, &OknoBot::usunKonkretnegoGroupBoxa);

    groupBoxes.push_back(newGroupBox); // Dodajemy nowy rząd do wektora
}

void OknoBot::usunKonkretnegoGroupBoxa(GroupBoxControl *kto)
{
    qDebug() << "[OknoBot] Przycisk usunKonkretnegoGroupBoxa został naciśnięty.";
    auto it = std::find(groupBoxes.begin(), groupBoxes.end(), kto);
    if (it != groupBoxes.end()) {
        groupBoxes.erase(it);
        ui->verticalLayout->removeWidget(kto);
        delete kto;
        emit rozszerzOkno(-ROW_HEIGHT);
    }
}

void OknoBot::usunWszystkieRzedy()
{
    qDebug() << "[OknoBot] usunWszystkieRzedy()";
    for (auto *box : groupBoxes) {
        ui->verticalLayout->removeWidget(box);
        delete box;
    }
    emit rozszerzOkno(-ROW_HEIGHT * groupBoxes.size());
    groupBoxes.clear();
}

void OknoBot::startWszystkie()
{
    for (auto &box : groupBoxes) {
        qDebug() << "[OknoBot] startWszystkie() " << box->wysylanieStart();
    }
}

void OknoBot::stopWszystkie()
{
    for (auto &box : groupBoxes) {
        qDebug() << "[OknoBot] stopWszystkie() " << box->wysylanieStop();
    }
}

QString OknoBot::getAllDataFromGroupBox()
{
    QString AllData;
    for (auto &box : groupBoxes) {
        qDebug() << "[OknoBot] getAllDataFromGroupBox() " << box->getAllData();
        AllData += box->getAllData() + "\n";
    }
    return AllData;
}

void OknoBot::setAllDataToGroupBox(QString zawartoscPliku)
{
    qDebug() << "[OknoBot] setAllDataToGroupBox() " << zawartoscPliku;
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

void OknoBot::onKeyPressed(WPARAM vkCode)
{
    //qDebug() << "[OknoBot][Hotkey] Odebrano klawisz VK:" << vkCode << " WParam: " << KeyMap::getKeyText(vkCode);
    //qDebug() << "[OknoBot][Hotkey] Liczba aktywnych groupBoxes:" << groupBoxes.size();

    auto mapa = KeyMap::getMap();

    QString hotkey = ui->comboBox_StopALL->currentText().toUpper();
    if (mapa.contains(hotkey)) {
        if (mapa[hotkey] == vkCode) {
            qDebug() << "[OknoBot][onKeyPressed] wciśnięty klawisz: " << mapa[hotkey]
                     << "PASUJE do hotkey'a: " << hotkey << ". Wywołuję handleStartStop()";
            stopWszystkie();
            return;
        }
    }

    hotkey = ui->comboBox_StartALL->currentText().toUpper();
    if (mapa.contains(hotkey)) {
        if (mapa[hotkey] == vkCode) {
            qDebug() << "[OknoBot][onKeyPressed] wciśnięty klawisz: " << mapa[hotkey]
                     << "PASUJE do hotkey'a: " << hotkey << ". Wywołuję handleStartStop()";
            startWszystkie();
            return;
        }
    }

    for (auto *box : groupBoxes) {
        QString hotkey = box->pobierzHotkey().toUpper();
        if (hotkey == "") {
            continue;
        }

        if (mapa.contains(hotkey)) {
            if (mapa[hotkey] == vkCode) {
                qDebug() << "[OknoBot][onKeyPressed] wciśnięty klawisz: " << mapa[hotkey]
                         << "PASUJE do hotkey'a: " << hotkey << ". Wywołuję handleStartStop()";
                box->handleStartStop();
            }
        } else {
            qDebug() << "[OknoBot][onKeyPressed] Nie znaleziono '" << hotkey << "' w mapie.";
        }
    }
}
