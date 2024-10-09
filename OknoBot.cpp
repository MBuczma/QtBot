/* OknoBot.cpp */
#include "OknoBot.h"
#include "ui_OknoBot.h"
#include <QMouseEvent>
#include <QTimer>

OknoBot::OknoBot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OknoBot)
    , autoKeyPresser(new AutoKeyPresser(this))
    , handle(0)
    , parentHandle(0)
    , windowText("windowText")
    , parentHandleWindowText("parentHandleWindowText")
    , keyTimer(new QTimer(this))
    , isButtonPressed(false) // Inicjalizacja timera
    , countdownTimer(new QTimer(this))  // Timer do odliczania
    , remainingTime(0)
    , isSendingKeys(false)  // Początkowy stan: klawisze nie są wysyłane
{
    ui->setupUi(this);
    connect(ui->pushButton_Start, &QPushButton::clicked, this, &OknoBot::start);
    connect(ui->pushButton_DodajNowyRzad, &QPushButton::clicked, this, &OknoBot::dodajRzadPrzyciskow);
    connect(ui->pushButton_PobierzID, &QPushButton::pressed, this, &OknoBot::ZlapIdOkna);
    connect(keyTimer, &QTimer::timeout, this, &OknoBot::wyslijKlawisz);
    connect(countdownTimer, &QTimer::timeout, this, &OknoBot::aktualizujCountdown);
}

OknoBot::~OknoBot()
{
    delete keyTimer;
    delete countdownTimer;
    delete autoKeyPresser;
    delete ui;
}

void OknoBot::start()
{
    qDebug() << "Przycisk start z oknobot został naciśnięty.";
    qDebug() << "lineEdit_HotKey" <<  ui->lineEdit_HotKey->displayText();
    qDebug() << "comboBox_Klawisz" << ui->comboBox_Klawisz->currentText();
    qDebug() << "spinBox_Sekund" << ui->spinBox_Sekund->text();
    qDebug() << "spinBox_Milisekund" << ui->spinBox_Milisekund->text();
    qDebug() << "spinBox_WysleZa" << ui->spinBox_WysleZa->text();
    //autoKeyPresser->SentKey(handle, ui->lineEdit_HotKey->displayText());
    int czasSekund = ui->spinBox_Sekund->value();
    int czasMilisekund = ui->spinBox_Milisekund->value();
    QString wybranyKlawisz = ui->comboBox_Klawisz->currentText();

    // Sprawdzenie, czy czas jest różny od 0 i klawisz został wybrany
    if ((czasSekund != 0 || czasMilisekund != 0) && !wybranyKlawisz.isEmpty()) {
        if (!isSendingKeys) {
            // Przycisk w stanie "Start"
            isSendingKeys = true;
            ui->pushButton_Start->setText("Stop");
            ui->pushButton_Start->setStyleSheet("background-color: red; color: white;");

            int interval = czasSekund * 1000 + czasMilisekund;  // Przelicz na milisekundy
            remainingTime = interval;

            keyTimer->start(interval);  // Uruchom timer
        } else {
            // Przycisk w stanie "Stop"
            isSendingKeys = false;
            ui->pushButton_Start->setText("Start");
            ui->pushButton_Start->setStyleSheet("background-color: green; color: white;");

            keyTimer->stop();  // Zatrzymaj timer
        }
    } else {
        qDebug() << "Czas musi być większy od 0 i klawisz musi być wybrany.";
    }
}

void OknoBot::dodajRzadPrzyciskow()
{
    qDebug() << "Przycisk dodajRzadPrzyciskow został naciśnięty.";
}

void OknoBot::ZlapIdOkna()
{
    qDebug() << "\n" << "Metoda OknoBot::ZlapIdOkna() został wywołana.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void OknoBot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressed == true){
        qDebug() << "OknoBot::mouseReleaseEvent.";
        isButtonPressed = false;
        autoKeyPresser->WindowHandleFromPoint(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        zaktualizujNazwe();
        ui->pushButton_PobierzID->setDown(false);
    }
}

void OknoBot::zaktualizujNazwe()
{
    windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
    parentHandleWindowText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);
    if(windowText == parentHandleWindowText)
    {
        ui->groupBoxPrzyciski->setTitle(parentHandleWindowText);
    }else
    {
        ui->groupBoxPrzyciski->setTitle(parentHandleWindowText + " " + windowText);
    }
}

void OknoBot::wyslijKlawisz()
{
    // Pobranie klawisza z comboBox_Klawisz
    QString klawisz = ui->comboBox_Klawisz->currentText();

    qDebug() << "Wysyłam klawisz:" << klawisz;

    // Wysyłanie klawisza (tutaj dodaj kod do wysyłania klawisza w zależności od platformy)
    autoKeyPresser->SentKey(handle, klawisz);
    remainingTime = ui->spinBox_Sekund->value() * 1000;
}

void OknoBot::aktualizujCountdown()
{
    // Zmniejsz pozostały czas
    remainingTime -= 100;

    if (remainingTime <= 0) {
        remainingTime = 0;  // Upewnij się, że czas nie jest mniejszy niż 0
    }

    // Zaktualizuj wartość w spinBox_WysleZa
    ui->spinBox_WysleZa->setValue(remainingTime);

    // Jeśli remainingTime wynosi 0, odśwież na nowo interwał
    if (remainingTime == 0) {
        remainingTime = ui->spinBox_Sekund->value() * 1000;
    }
}
