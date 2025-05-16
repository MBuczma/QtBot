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

    globalKeyListener = new GlobalKeyListener(this);
    connect(globalKeyListener, &GlobalKeyListener::keyPressed, this, &OknoBot::onKeyPressed);
    globalKeyListener->start();
}

OknoBot::~OknoBot() = default;

void OknoBot::dodajRzad()
{
    emit rozszerzOkno(70);
    qDebug() << "Przycisk dodajRzadPrzyciskow został naciśnięty.";

    GroupBoxControl *newGroupBox = new GroupBoxControl(this);
    ui->verticalLayout->insertWidget(ui->verticalLayout->count() - 1, newGroupBox);
    connect(newGroupBox, &GroupBoxControl::zadajUsuniecie, this, &OknoBot::usunKonkretnegoGroupBoxa);

    groupBoxes.push_back(newGroupBox); // Dodajemy nowy rząd do wektora
}

void OknoBot::usunKonkretnegoGroupBoxa(GroupBoxControl *kto)
{
    auto it = std::find(groupBoxes.begin(), groupBoxes.end(), kto);
    if (it != groupBoxes.end()) {
        groupBoxes.erase(it);
        ui->verticalLayout->removeWidget(kto);
        delete kto;
        emit rozszerzOkno(-70);
    }
}

void OknoBot::usunWszystkieRzedy()
{
    for (auto *box : groupBoxes) {
        ui->verticalLayout->removeWidget(box);
        delete box;
    }
    emit rozszerzOkno(-70 * groupBoxes.size());
    groupBoxes.clear();
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

void OknoBot::onKeyPressed(int vkCode)
{
    qDebug() << "[Hotkey] Odebrano klawisz VK:" << vkCode;
    qDebug() << "[Hotkey] Liczba aktywnych groupBoxes:" << groupBoxes.size();

    static const QMap<QString, int> mapa = {
        {"SPACE", VK_SPACE},
        {"ENTER", VK_RETURN},
        {"F1", VK_F1},
        {"F2", VK_F2},
        {"F3", VK_F3},
        {"F4", VK_F4},
        {"F5", VK_F5},
        {"F6", VK_F6},
        {"F7", VK_F7},
        {"F8", VK_F8},
        {"F9", VK_F9},
        {"F10", VK_F10},
        {"F11", VK_F11},
        {"F12", VK_F12},
        {"1", '1'},
        {"2", '2'},
        {"3", '3'},
        {"4", '4'},
        {"Numpad 1", VK_NUMPAD1},
        {"Numpad 2", VK_NUMPAD2},
        {"Numpad 0", VK_NUMPAD0},
        // rozszerz w razie potrzeby
    };

    for (auto *box : groupBoxes) {
        QString hotkey = box->pobierzHotkey().trimmed().toUpper();
        qDebug() << "[Hotkey] VK:" << vkCode << "vs:" << hotkey;

        if (mapa.contains(hotkey)) {
            qDebug() << "  Mapa[" << hotkey << "] = " << mapa[hotkey];
            if (mapa[hotkey] == vkCode) {
                qDebug() << " PASUJE: wywołuję wysylanieStart()";
                box->handleStartStop();
            }
        } else {
            qDebug() << " Nie znaleziono '" << hotkey << "' w mapie.";
        }
    }
}
