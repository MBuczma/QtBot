#include "GroupBoxControl.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <windows.h>

GroupBoxControl::GroupBoxControl(QWidget *parent)
    : QWidget(parent)
    , keyTimer(new QTimer(this))
    , isSending(false)
    , windowHandle(nullptr)
{
    setupGroupBox();

    connect(keyTimer, &QTimer::timeout, this, &GroupBoxControl::sendKey);
}

GroupBoxControl::~GroupBoxControl() = default;

void GroupBoxControl::setupGroupBox()
{
    groupBox = new QGroupBox("Nowy GroupBox", this);

    QHBoxLayout *layout = new QHBoxLayout(groupBox);

    buttonStartStop = new QPushButton("Start", this);
    layout->addWidget(buttonStartStop);
    connect(buttonStartStop, &QPushButton::clicked, this, &GroupBoxControl::handleStartStop);

    buttonPobierzID = new QPushButton("Pobierz ID", this);
    layout->addWidget(buttonPobierzID);
    connect(buttonPobierzID, &QPushButton::clicked, this, &GroupBoxControl::ZlapIdOkna);

    comboBox_Klawisz = new QComboBox(this);
    comboBox_Klawisz->addItems({"Space", "F1", "F2", "Enter"});
    layout->addWidget(comboBox_Klawisz);

    spinBox_Sekund = new QSpinBox(this);
    spinBox_Sekund->setSuffix("s");
    spinBox_Sekund->setMaximum(9999);
    layout->addWidget(spinBox_Sekund);

    spinBox_Milisekund = new QSpinBox(this);
    spinBox_Milisekund->setSuffix("ms");
    spinBox_Milisekund->setMaximum(999);
    layout->addWidget(spinBox_Milisekund);

    groupBox->setLayout(layout);
}

void GroupBoxControl::ZlapIdOkna()
{
    qDebug() << "\n"
             << "Metoda OknoBot::ZlapIdOkna() została wywołana.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void GroupBoxControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressed) {
        qDebug() << "OknoBot::mouseReleaseEvent.";
        isButtonPressed = false;
        //autoKeyPresser->WindowHandleFromPoint(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        //zaktualizujNazwe();
        //ui->pushButton_PobierzID->setDown(false);
    }
    QWidget::mouseReleaseEvent(event); // Dodano wywołanie funkcji bazowej
}

void GroupBoxControl::handleStartStop()
{
    if (isSending) {
        keyTimer->stop();
        buttonStartStop->setText("Start");
        isSending = false;
    } else {
        int interval = spinBox_Sekund->value() * 1000 + spinBox_Milisekund->value();
        if (interval > 0 && windowHandle != nullptr) {
            keyTimer->start(interval);
            buttonStartStop->setText("Stop");
            isSending = true;
        } else {
            qDebug() << "Nieprawidłowy czas lub brak uchwytu okna.";
        }
    }
}

void GroupBoxControl::sendKey()
{
    QString key = comboBox_Klawisz->currentText();
    if (!key.isEmpty() && windowHandle != nullptr) {
        // Tu dodaj funkcję do wysyłania klawisza do windowHandle
        qDebug() << "Wysyłanie klawisza: " << key << " do okna: " << windowHandle;
        // Przykład: autoKeyPresser->SentKey(windowHandle, key);
    }
}

void GroupBoxControl::getWindowHandle()
{
    // Zastąp tym, co odpowiada za pobranie uchwytu okna
    POINT p;
    GetCursorPos(&p);
    windowHandle = WindowFromPoint(p);
    if (windowHandle) {
        qDebug() << "Pobrano uchwyt okna: " << windowHandle;
    } else {
        qDebug() << "Nie udało się pobrać uchwytu okna.";
    }
}
