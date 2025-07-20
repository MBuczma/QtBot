/* GroupBoxControl.cpp */
/*
 * Groupbox który zawiera pola do wprowadzania danych, wyświetla informacje oraz
 * odpowiada za start i stop poszczególnych przycisków. 
 */
#include "GroupBoxControl.h"
#include "AutoKeyPresser.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QTimer>
#include "KeyMap.h"
#include <memory>
#include <windows.h>

GroupBoxControl::GroupBoxControl(QWidget *parent) //konstrukor
    : QWidget(parent)
    , keyTimer(std::make_unique<QTimer>(this))
    , countdownTimer(std::make_unique<QTimer>(this))
    , autoKeyPresser(std::make_unique<AutoKeyPresser>())
    , isSending(false)
    , isButtonPressed(false)
    , remainingTime(0)
{
    setupGroupBox();
    connect(keyTimer.get(), &QTimer::timeout, this, &GroupBoxControl::wyslijKlawisz);
    connect(countdownTimer.get(), &QTimer::timeout, this, &GroupBoxControl::aktualizujCountdown);
}

GroupBoxControl::~GroupBoxControl() = default; //destruktor

void GroupBoxControl::setupGroupBox()
{
    groupBox = new QGroupBox("Nowy GroupBox", this);
    layout = new QHBoxLayout(groupBox);

    buttonStartStop = new QPushButton("Start", this);
    buttonStartStop->setMinimumWidth(60);
    buttonStartStop->setEnabled(false);
    layout->addWidget(buttonStartStop);
    connect(buttonStartStop, &QPushButton::clicked, this, &GroupBoxControl::handleStartStop);

    buttonPobierzID = new QPushButton("Pobierz ID", this);
    buttonPobierzID->setMinimumWidth(60);
    layout->addWidget(buttonPobierzID);
    connect(buttonPobierzID, &QPushButton::pressed, this, &GroupBoxControl::ZlapIdOkna);

    comboBox_Klawisz = new QComboBox(this);
    comboBox_Klawisz->addItem("");
    // Dodaj wszystkie klawisze z mapy KeyMap
    for (const auto &pair : KeyMap::getOrderedList()) {
        comboBox_Klawisz->addItem(pair.first);
    }

    comboBox_Klawisz->setMinimumWidth(60);
    connect(comboBox_Klawisz,
            &QComboBox::currentTextChanged,
            this,
            &GroupBoxControl::aktualizujStanPrzyciskuStart);
    layout->addWidget(comboBox_Klawisz);

    comboBox_Hotkey = new QComboBox(this);
    comboBox_Hotkey->addItems({""});
    for (const auto &pair : KeyMap::getOrderedList()) {
        comboBox_Hotkey->addItem(pair.first);
    }
    comboBox_Hotkey->setMinimumWidth(60);
    connect(comboBox_Hotkey,
            &QComboBox::currentTextChanged,
            this,
            &GroupBoxControl::aktualizujStanPrzyciskuStart);
    layout->addWidget(comboBox_Hotkey);

    spinBox_Sekund = new QSpinBox(this);
    spinBox_Sekund->setSuffix("s");
    spinBox_Sekund->setMaximum(999);
    spinBox_Sekund->setMinimumWidth(60);
    layout->addWidget(spinBox_Sekund);
    connect(spinBox_Sekund,
            &QSpinBox::textChanged,
            this,
            &GroupBoxControl::aktualizujStanPrzyciskuStart);

    spinBox_Milisekund = new QSpinBox(this);
    spinBox_Milisekund->setSuffix("ms");
    spinBox_Milisekund->setSingleStep(100);
    spinBox_Milisekund->setMaximum(999);
    spinBox_Milisekund->setMinimumWidth(60);
    layout->addWidget(spinBox_Milisekund);
    connect(spinBox_Milisekund,
            &QSpinBox::textChanged,
            this,
            &GroupBoxControl::aktualizujStanPrzyciskuStart);

    spinBox_WysleZa = new QSpinBox(this);
    spinBox_WysleZa->setSuffix("ms");
    spinBox_WysleZa->setMaximum(999999);
    spinBox_WysleZa->setMinimumWidth(60);
    spinBox_WysleZa->setButtonSymbols(QAbstractSpinBox::NoButtons);
    layout->addWidget(spinBox_WysleZa);

    buttonUsun = new QPushButton("Usuń rząd", this);
    buttonUsun->setMinimumWidth(60);
    layout->addWidget(buttonUsun);
    connect(buttonUsun, &QPushButton::clicked, this, &GroupBoxControl::czyPotwierdzicUsuniecie);
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
    if (event->button() == Qt::LeftButton && isButtonPressed == true) {
        qDebug() << "GroupBoxControl::mouseReleaseEvent. W IFie";
        isButtonPressed = false;
        autoKeyPresser->WindowHandleFromPoint(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        zaktualizujNazwe();
        buttonPobierzID->setDown(false);
        aktualizujStanPrzyciskuStart();
    }
    QWidget::mouseReleaseEvent(event); // Dodano wywołanie funkcji bazowej
}

void GroupBoxControl::zaktualizujNazwe()
{
    windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
    parentWindowText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);

    if (groupBox != nullptr) {
        if (windowText == parentWindowText) {
            groupBox->setTitle(parentWindowText);
        } else {
            groupBox->setTitle(parentWindowText + " " + windowText);
        }
        //qDebug() << "Tytuł groupBox został zaktualizowany.";
    } else {
        qDebug() << "Błąd: wskaźnik groupBox jest nullptr!";
    }
}

void GroupBoxControl::aktualizujStanPrzyciskuStart()
{
    const QString klawisz = comboBox_Klawisz->currentText();

    if (isSending == true) {
        buttonPobierzID->setEnabled(false);
        comboBox_Klawisz->setEnabled(false);
        comboBox_Hotkey->setEnabled(false);
        spinBox_Sekund->setEnabled(false);
        spinBox_Milisekund->setEnabled(false);
        buttonStartStop->setText("Stop");
        buttonStartStop->setStyleSheet(
            "QPushButton:enabled { background-color: red; color: white; }");
    } else {
        buttonPobierzID->setEnabled(true);
        comboBox_Klawisz->setEnabled(true);
        comboBox_Hotkey->setEnabled(true);
        spinBox_Sekund->setEnabled(true);
        spinBox_Milisekund->setEnabled(true);
        buttonStartStop->setText("Start");
        buttonStartStop->setStyleSheet(
            "QPushButton:enabled { background-color: green; color: white; }");
    }

    if (klawisz.isEmpty() || handle == nullptr
        || (spinBox_Sekund->value() == 0 && spinBox_Milisekund->value() == 0)) {
        buttonStartStop->setEnabled(false);
        qDebug() << "klawisz.isEmpty() || !handle = " << klawisz << " " << handle;

    } else {
        // Ustaw zielony kolor i aktywuj przycisk
        qDebug() << "Ustaw zielony kolor i aktywuj przycisk = " << klawisz << " " << handle;
        buttonStartStop->setEnabled(true);
    }
}

void GroupBoxControl::handleStartStop()
{
    qDebug() << "Przycisk start z OknoBot został naciśnięty.";

    if (isSending == false) {
        Beep(1000, 100); // 500 Hz przez 100 ms
        wysylanieStart();
    } else {
        Beep(500, 50); // 500 Hz przez 100 ms
        wysylanieStop();
    }
}

bool GroupBoxControl::wysylanieStop()
{
    if (isSending == true) {
        isSending = false;
        aktualizujStanPrzyciskuStart();
        keyTimer->stop();
        countdownTimer->stop();
        return true;
    } else {
        qDebug() << "Zatrzymanie się nie powiodło";
        return false;
    }
}

bool GroupBoxControl::wysylanieStart()
{
    int czasSekund = spinBox_Sekund->value();
    int czasMilisekund = spinBox_Milisekund->value();
    QString wybranyKlawisz = comboBox_Klawisz->currentText();
    if (isSending == false) {
        if ((czasSekund != 0 || czasMilisekund != 0) && wybranyKlawisz.isEmpty() == false
            && handle != nullptr) {
            isSending = true;
            aktualizujStanPrzyciskuStart();

            int interval = czasSekund * 1000 + czasMilisekund;
            remainingTime = interval;

            keyTimer->start(interval);
            countdownTimer->start(100);
            return true;
        } else {
            qDebug() << "Czas musi być większy od 0 i klawisz musi być wybrany.";
        }
    } else {
        qDebug() << "Juz wysyła";
    }
    return false;
}

void GroupBoxControl::wyslijKlawisz()
{
    QString key = comboBox_Klawisz->currentText();
    if (key.isEmpty() == false && handle != nullptr) {
        autoKeyPresser->SentKey(handle, key);
    }
}

void GroupBoxControl::aktualizujCountdown()
{
    remainingTime -= 100;

    if (remainingTime <= 0) {
        remainingTime = spinBox_Sekund->value() * 1000 + spinBox_Milisekund->value();
    }
    spinBox_WysleZa->setValue(remainingTime);
}

QString GroupBoxControl::getAllData() const
{
    //return "aktualizujCountdown()  ";
    if (groupBox != NULL) {
        QString data = groupBox->title() + ";";
        data += comboBox_Klawisz->currentText() + ";";
        data += comboBox_Hotkey->currentText() + ";";
        data += spinBox_Sekund->text() + ";";
        data += spinBox_Milisekund->text() + ";";
        return data;
    }
    return QString(); // zwraca pusty QString, jeśli groupBox == nullptr
}

void GroupBoxControl::setAllData(const QString &line)
{
    //GlowneOkno::wczytajPlik() -> OknoBot::setAllDataToGroupBox(QString zawartoscPliku) -> GroupBoxControl::setAllData(QString zawartoscPliku)

    QStringList values = line.split(";", Qt::SkipEmptyParts);

    for (int i = 0; i < values.size(); ++i) {
        QString value = values[i].trimmed();
        qDebug() << "Wartość [" << i << "] :" << value;

        switch (i) {
        case 0:
            if (groupBox)
                groupBox->setTitle(value);
            break;
        case 1:
            if (comboBox_Klawisz)
                comboBox_Klawisz->setCurrentText(value);
            break;
        case 2:
            if (comboBox_Hotkey)
                comboBox_Hotkey->setCurrentText(value);
            break;
        case 3:
            if (spinBox_Sekund) {
                value.remove("s", Qt::CaseInsensitive);
                spinBox_Sekund->setValue(value.toInt());
            }
            break;
        case 4:
            if (spinBox_Milisekund) {
                value.remove("ms", Qt::CaseInsensitive);
                spinBox_Milisekund->setValue(value.toInt());
            }
            break;
        default:
            break;
        }
    }
}

void GroupBoxControl::czyPotwierdzicUsuniecie()
{
    QMessageBox::StandardButton odpowiedz
        = QMessageBox::question(this,
                                "Usuń rząd",
                                "Czy na pewno chcesz usunąć ten rząd?",
                                QMessageBox::Yes | QMessageBox::No);

    if (odpowiedz == QMessageBox::Yes) {
        emit zadajUsuniecie(this);
    }
}

QString GroupBoxControl::pobierzHotkey() const
{
    return comboBox_Hotkey->currentText();
}
