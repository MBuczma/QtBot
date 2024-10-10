/* OknoBot.cpp */
#include "OknoBot.h"
#include <QMouseEvent>
#include <QTimer>
#include "ui_OknoBot.h"
#include <memory>

OknoBot::OknoBot(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::OknoBot>())
    , autoKeyPresser(std::make_unique<AutoKeyPresser>())
    , keyTimer(std::make_unique<QTimer>(this))
    , countdownTimer(std::make_unique<QTimer>(this))
    , isButtonPressed(false)
    , isSendingKeys(false)
    , remainingTime(0)
{
    ui->setupUi(this);
    connect(ui->pushButton_Start, &QPushButton::clicked, this, &OknoBot::startStop);
    connect(ui->pushButton_DodajRzad, &QPushButton::clicked, this, &OknoBot::dodajRzad);
    connect(ui->pushButton_PobierzID, &QPushButton::pressed, this, &OknoBot::ZlapIdOkna);
    connect(ui->comboBox_Klawisz,
            &QComboBox::currentTextChanged,
            this,
            &OknoBot::aktualizujStanPrzyciskuStartNaPodstawieComboBox);
    connect(keyTimer.get(), &QTimer::timeout, this, &OknoBot::wyslijKlawisz);
    connect(countdownTimer.get(), &QTimer::timeout, this, &OknoBot::aktualizujCountdown);
}

OknoBot::~OknoBot() = default;

void OknoBot::aktualizujStanPrzyciskuStartNaPodstawieComboBox(const QString &text)
{
    if (text.isEmpty()) {
        // Ustaw kolor na szary i wyłącz przycisk, gdy nie wybrano żadnego klawisza
        ui->pushButton_Start->setStyleSheet("background-color: #808080; color: white;");
        ui->pushButton_Start->setEnabled(false);
    } else {
        // Przywróć pierwotny kolor i aktywuj przycisk
        ui->pushButton_Start->setStyleSheet("background-color: green; color: white;");
        ui->pushButton_Start->setEnabled(true);
    }
}

void OknoBot::startStop()
{
    qDebug() << "Przycisk start z OknoBot został naciśnięty.";

    int czasSekund = ui->spinBox_Sekund->value();
    int czasMilisekund = ui->spinBox_Milisekund->value();
    QString wybranyKlawisz = ui->comboBox_Klawisz->currentText();

    if (isSendingKeys == false) {
        if ((czasSekund != 0 || czasMilisekund != 0) && !wybranyKlawisz.isEmpty()) {
            isSendingKeys = true;
            aktualizujStanPrzyciskuStart(isSendingKeys);

            int interval = czasSekund * 1000 + czasMilisekund;
            remainingTime = interval;

            keyTimer->start(interval);
            countdownTimer->start(100);
        } else {
            qDebug() << "Czas musi być większy od 0 i klawisz musi być wybrany.";
        }
    } else {
        isSendingKeys = false;
        aktualizujStanPrzyciskuStart(isSendingKeys);
        keyTimer->stop();
        countdownTimer->stop();
    }
}

void OknoBot::aktualizujStanPrzyciskuStart(bool isSending)
{
    if (isSending) {
        ui->pushButton_Start->setText("Stop");
        ui->pushButton_Start->setStyleSheet("background-color: red; color: white;");
    } else {
        ui->pushButton_Start->setText("Start");
        ui->pushButton_Start->setStyleSheet("background-color: green; color: white;");
    }
}

void OknoBot::dodajRzad()
{
    qDebug() << "Przycisk dodajRzadPrzyciskow został naciśnięty.";
}

void OknoBot::ZlapIdOkna()
{
    qDebug() << "\n"
             << "Metoda OknoBot::ZlapIdOkna() została wywołana.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void OknoBot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressed) {
        qDebug() << "OknoBot::mouseReleaseEvent.";
        isButtonPressed = false;
        autoKeyPresser->WindowHandleFromPoint(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        zaktualizujNazwe();
        ui->pushButton_PobierzID->setDown(false);
    }
    QWidget::mouseReleaseEvent(event); // Dodano wywołanie funkcji bazowej
}

void OknoBot::zaktualizujNazwe()
{
    windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
    parentHandleWindowText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);
    if (windowText == parentHandleWindowText) {
        ui->groupBoxPrzyciski->setTitle(parentHandleWindowText);
    } else {
        ui->groupBoxPrzyciski->setTitle(parentHandleWindowText + " " + windowText);
    }
}

void OknoBot::wyslijKlawisz()
{
    QString klawisz = ui->comboBox_Klawisz->currentText();
    qDebug() << "Wysyłam klawisz:" << klawisz;
    autoKeyPresser->SentKey(handle, klawisz);
    remainingTime = ui->spinBox_Sekund->value() * 1000 + ui->spinBox_Milisekund->value();
}

void OknoBot::aktualizujCountdown()
{
    remainingTime -= 100;

    if (remainingTime <= 0) {
        remainingTime = 0;
    }

    ui->spinBox_WysleZa->setValue(remainingTime);

    if (remainingTime == 0) {
        remainingTime = ui->spinBox_Sekund->value() * 1000;
    }
}
