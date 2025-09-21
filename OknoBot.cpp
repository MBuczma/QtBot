/* OknoBot.cpp */
/*
 * Okno które przechowuje rzędy boxów z przyciskami do wysłania.
 * Zawiera kontener dla wielu GroupBoxControl.
 * Obsługuje dodawanie, usuwanie i sterowanie wierszami.
 * Przechwytuje globalne klawisze i reaguje na hotkeye.
 */
#include "OknoBot.h"
#include "ui_OknoBot.h"

#include <QMouseEvent>
#include "AutoKeyPresser.h"
#include "GroupBoxControl.h"
#include "KeyMap.h"
#include "QTimer"

const short ROW_HEIGHT = 70;
static constexpr WPARAM VK_MOUSE_X1_SYN = 0x1001;
static constexpr WPARAM VK_MOUSE_X2_SYN = 0x1002;

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

    // Mysz – mapujemy XBUTTON -> syntetyczny „vk” i ponownie używamy onKeyPressed
    globalMouseListener = std::make_unique<GlobalMouseListener>(this);
    connect(globalMouseListener.get(),
            &GlobalMouseListener::xButton,
            this,
            [this](int btn, bool down) {
                if (!down)
                    return; // tylko DOWN
                const WPARAM code = (btn == 1) ? VK_MOUSE_X1_SYN : VK_MOUSE_X2_SYN;
                this->onKeyPressed(code);
            });
    QTimer::singleShot(0, this, [this] { globalMouseListener->start(); });
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
        qDebug() << "[OknoBot] startWszystkie() ";
        box->wysylanieStart();
    }
}

void OknoBot::stopWszystkie()
{
    for (auto &box : groupBoxes) {
        qDebug() << "[OknoBot] stopWszystkie() ";
        box->wysylanieStop();
    }
}

QString OknoBot::getAllDataFromGroupBox()
{
    QString AllData;
    QString startHotkey = ui->comboBox_StartALL->currentText();
    QString stopHotkey = ui->comboBox_StopALL->currentText();
    AllData = startHotkey + ";" + stopHotkey + "\n";

    for (auto &box : groupBoxes) {
        qDebug() << "[OknoBot] getAllDataFromGroupBox() " << box->getAllData();
        AllData += box->getAllData() + "\n";
    }
    return AllData;
}

void OknoBot::setAllDataToGroupBox(QString zawartoscPliku)
{
    qDebug() << "[OknoBot] setAllDataToGroupBox() " << zawartoscPliku;
    QStringList lines = zawartoscPliku.split("\n");
    QStringList hotkeys = lines[0].split(";");
    lines.removeFirst();
    ui->comboBox_StartALL->setCurrentText(hotkeys[0].trimmed());
    ui->comboBox_StopALL->setCurrentText(hotkeys[1].trimmed());

    for (int i = groupBoxes.size(); i < lines.count() - 1; ++i) {
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
                     << "PASUJE do hotkey'a: " << hotkey << ". Wywołuję stopWszystkie()";
            stopWszystkie();
            return;
        }
    }

    hotkey = ui->comboBox_StartALL->currentText().toUpper();
    if (mapa.contains(hotkey)) {
        if (mapa[hotkey] == vkCode) {
            qDebug() << "[OknoBot][onKeyPressed] wciśnięty klawisz: " << mapa[hotkey]
                     << "PASUJE do hotkey'a: " << hotkey << ". Wywołuję startWszystkie()";
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

void OknoBot::pobierzIdWszystkie()
{
    qDebug() << "[OknoBot] pobierzIdWszystkie() start";
    isButtonPressedGlobal = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void OknoBot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressedGlobal) {
        isButtonPressedGlobal = false;

        // pobierz uchwyt spod kursora
        AutoKeyPresser presser;
        presser.WindowHandleFromPoint(handleGlobal, parentHandleGlobal);

        releaseMouse();
        unsetCursor();

        if (handleGlobal != nullptr) {
            qDebug() << "[OknoBot] Złapano uchwyt globalny:" << handleGlobal;

            // rozesłanie do wszystkich rzędów
            for (auto *box : groupBoxes) {
                if (box) {
                    box->ustawHandle(handleGlobal, parentHandleGlobal);
                }
            }
        }
    }

    QWidget::mouseReleaseEvent(event);
}
