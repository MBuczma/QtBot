/* GroupBoxControl.cpp */
#include "GroupBoxControl.h"
#include "AutoKeyPresser.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QTimer>
#include <memory>
#include <windows.h>

GroupBoxControl::GroupBoxControl(QWidget *parent)
    : QWidget(parent)
    , keyTimer(std::make_unique<QTimer>(this))
    , countdownTimer(std::make_unique<QTimer>(this))
    , autoKeyPresser(std::make_unique<AutoKeyPresser>())
    , isSending(false)
    , isButtonPressed(false)
    , remainingTime(0)
{
    setupGroupBox();
    connect(keyTimer.get(), &QTimer::timeout, this, &GroupBoxControl::sendKey);
    connect(countdownTimer.get(), &QTimer::timeout, this, &GroupBoxControl::aktualizujCountdown);
}

GroupBoxControl::~GroupBoxControl() = default;

void GroupBoxControl::setupGroupBox()
{
    groupBox = new QGroupBox("Nowy GroupBox", this);
    //groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout = new QHBoxLayout(groupBox);

    buttonStartStop = new QPushButton("Start", this);
    buttonStartStop->setMinimumWidth(80);
    //buttonStartStop->setEnabled(false);
    layout->addWidget(buttonStartStop);
    connect(buttonStartStop, &QPushButton::clicked, this, &GroupBoxControl::handleStartStop);

    buttonPobierzID = new QPushButton("Pobierz ID", this);
    buttonPobierzID->setMinimumWidth(80);
    layout->addWidget(buttonPobierzID);
    connect(buttonPobierzID, &QPushButton::pressed, this, &GroupBoxControl::ZlapIdOkna);

    comboBox_Klawisz = new QComboBox(this);
    comboBox_Klawisz->addItems({"Space",
                                "1",
                                "2",
                                "F1",
                                "F2",
                                "F3",
                                "F4",
                                "F5",
                                "F6",
                                "F7",
                                "F8",
                                "F9",
                                "F10",
                                "F11",
                                "F12",
                                "Enter"});
    comboBox_Klawisz->setMinimumWidth(80);
    layout->addWidget(comboBox_Klawisz);

    comboBox_Hotkey = new QComboBox(this);
    comboBox_Hotkey->addItems({"Space",
                               "1",
                               "2",
                               "F1",
                               "F2",
                               "F3",
                               "F4",
                               "F5",
                               "F6",
                               "F7",
                               "F8",
                               "F9",
                               "F10",
                               "F11",
                               "F12",
                               "Enter"});
    comboBox_Hotkey->setMinimumWidth(80);
    layout->addWidget(comboBox_Hotkey);

    spinBox_Sekund = new QSpinBox(this);
    spinBox_Sekund->setSuffix("s");
    spinBox_Sekund->setMaximum(9999);
    spinBox_Sekund->setMinimumWidth(70);
    layout->addWidget(spinBox_Sekund);

    spinBox_Milisekund = new QSpinBox(this);
    spinBox_Milisekund->setSuffix("ms");
    spinBox_Milisekund->setMaximum(999);
    spinBox_Milisekund->setMinimumWidth(70);
    layout->addWidget(spinBox_Milisekund);

    spinBox_WysleZa = new QSpinBox(this);
    spinBox_WysleZa->setSuffix("ms");
    spinBox_WysleZa->setMaximum(999999);
    spinBox_WysleZa->setMinimumWidth(80);
    spinBox_WysleZa->setButtonSymbols(QAbstractSpinBox::NoButtons);
    layout->addWidget(spinBox_WysleZa);
}

void GroupBoxControl::ZlapIdOkna()
{
    qDebug() << "\n"
             << "Metoda GroupBoxControl::ZlapIdOkna() została wywołana.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void GroupBoxControl::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "GroupBoxControl::mouseReleaseEvent. bez ifa" << isButtonPressed;
    if (event->button() == Qt::LeftButton && isButtonPressed) {
        qDebug() << "GroupBoxControl::mouseReleaseEvent.";
        isButtonPressed = false;
        autoKeyPresser->WindowHandleFromPoint(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        zaktualizujNazwe();
        buttonPobierzID->setDown(false);
    }
    QWidget::mouseReleaseEvent(event); // Dodano wywołanie funkcji bazowej
}

void GroupBoxControl::zaktualizujNazwe()
{
    windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
    parentHandleWindowText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);

    if (groupBox) {
        if (windowText == parentHandleWindowText) {
            groupBox->setTitle(parentHandleWindowText);
        } else {
            groupBox->setTitle(parentHandleWindowText + " " + windowText);
        }
        //qDebug() << "Tytuł groupBox został zaktualizowany.";
    } else {
        qDebug() << "Błąd: wskaźnik groupBox jest nullptr!";
    }
}

void GroupBoxControl::aktualizujStanPrzyciskuStartNaPodstawieComboBox(const QString &text)
{
    if (text.isEmpty()) {
        buttonStartStop->setEnabled(false);
    } else {
        // Ustaw zielony kolor i aktywuj przycisk
        buttonStartStop->setEnabled(true);
    }
}

void GroupBoxControl::handleStartStop()
{
    qDebug() << "Przycisk start z OknoBot został naciśnięty.";

    int czasSekund = spinBox_Sekund->value();
    int czasMilisekund = spinBox_Milisekund->value();
    QString wybranyKlawisz = comboBox_Klawisz->currentText();

    if (isSending == false) {
        if ((czasSekund != 0 || czasMilisekund != 0) && !wybranyKlawisz.isEmpty()) {
            isSending = true;
            aktualizujStanPrzyciskuStart(isSending);

            int interval = czasSekund * 1000 + czasMilisekund;
            remainingTime = interval;

            keyTimer->start(interval);
            countdownTimer->start(100);
        } else {
            qDebug() << "Czas musi być większy od 0 i klawisz musi być wybrany.";
        }
    } else {
        isSending = false;
        aktualizujStanPrzyciskuStart(isSending);
        keyTimer->stop();
        countdownTimer->stop();
    }
}

void GroupBoxControl::aktualizujStanPrzyciskuStart(bool isSending)
{
    if (isSending) {
        buttonPobierzID->setEnabled(false);
        comboBox_Klawisz->setEnabled(false);
        spinBox_Sekund->setEnabled(false);
        spinBox_Milisekund->setEnabled(false);
        buttonStartStop->setText("Stop");
        buttonStartStop->setStyleSheet(
            "QPushButton:enabled { background-color: red; color: white; }");
    } else {
        buttonPobierzID->setEnabled(true);
        comboBox_Klawisz->setEnabled(true);
        spinBox_Sekund->setEnabled(true);
        spinBox_Milisekund->setEnabled(true);
        buttonStartStop->setText("Start");
        buttonStartStop->setStyleSheet(
            "QPushButton:enabled { background-color: green; color: white; }");
    }
}

void GroupBoxControl::sendKey()
{
    QString key = comboBox_Klawisz->currentText();
    if (!key.isEmpty() && handle != nullptr) {
        // Tu dodaj funkcję do wysyłania klawisza do handle
        qDebug() << "Wysyłanie klawisza: " << key << " do okna: " << handle;
        autoKeyPresser->SentKey(handle, key);
    }
}

void GroupBoxControl::getHandle()
{
    POINT p;
    GetCursorPos(&p);
    handle = WindowFromPoint(p);
    if (handle) {
        qDebug() << "Pobrano uchwyt okna: " << handle;
    } else {
        qDebug() << "Nie udało się pobrać uchwytu okna.";
    }
}

void GroupBoxControl::aktualizujCountdown()
{
    remainingTime -= 100;

    if (remainingTime <= 0) {
        remainingTime = 0;
    }

    spinBox_WysleZa->setValue(remainingTime);

    if (remainingTime == 0) {
        remainingTime = spinBox_Sekund->value() * 1000;
    }
}
