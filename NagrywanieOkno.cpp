/* NagrywanieOkno.cpp */
#include "NagrywanieOkno.h"
#include <QMouseEvent>
#include <QTimer>
#include "ui_NagrywanieOkno.h"
#include <memory>
#include <windows.h>

NagrywanieOkno::NagrywanieOkno(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::NagrywanieOkno>())
    , autoKeyPresser(std::make_unique<AutoKeyPresser>())
    , isButtonPressed(false)
{
    ui->setupUi(this);
    connect(ui->pushButton_Start, &QPushButton::clicked, this, &NagrywanieOkno::rozpocznijWysylanie);
    connect(ui->pushButton_PobierzID, &QPushButton::pressed, this, &NagrywanieOkno::ZlapIdOkna);
}

NagrywanieOkno::~NagrywanieOkno() = default;

void NagrywanieOkno::ZlapIdOkna()
{
    qDebug() << "\n"
             << "Metoda NagrywanieOkno::ZlapIdOkna() został wywołana.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void NagrywanieOkno::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressed) {
        qDebug() << "NagrywanieOkno::mouseReleaseEvent.";
        isButtonPressed = false;
        autoKeyPresser->WindowHandleFromPoint(handle.value(), parentHandle.value());
        releaseMouse();
        unsetCursor();
        ui->pushButton_PobierzID->setDown(false);
        windowText = autoKeyPresser->GetWindowTextFromHandle(handle.value());
        ui->lineEdit_NazwaProgramu->setText(windowText);
    }
    QWidget::mouseReleaseEvent(event);
}

void NagrywanieOkno::rozpocznijWysylanie()
{
    int robaki = ui->lineEdit_Robaki->text().toInt();
    qDebug() << "Robaki to:" << robaki;

    // Użyj QTimer do wysyłania klawiszy, aby GUI pozostało responsywne
    int interval = 700; // Przykładowy interwał (700 ms) między wysyłaniem klawiszy
    QTimer *keyTimer = new QTimer(this);

    // Użycie std::shared_ptr do zarządzania licznikiem klawiszy
    auto currentIndex = std::make_shared<int>(0);

    connect(keyTimer, &QTimer::timeout, this, [=]() mutable {
        if (*currentIndex >= robaki) {
            keyTimer->stop();
            keyTimer->deleteLater(); // Zwalnia pamięć związaną z keyTimer
            qDebug() << "Wysłano wszystkie klawisze.";
            return;
        }

        // Wysyłanie klawisza "Z"
        PostMessage(handle.value(), WM_KEYDOWN, 'Z', 0);
        PostMessage(handle.value(), WM_KEYUP, 'Z', 0);
        qDebug() << "Wysłano klawisz Z";

        // Symulacja kliknięcia myszki
        PostMessage(handle.value(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        PostMessage(handle.value(), WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        qDebug() << "Wysłano kliknięcie myszki";

        // Wysyłanie klawisza "Spacja"
        PostMessage(handle.value(), WM_KEYDOWN, VK_SPACE, 0);
        PostMessage(handle.value(), WM_KEYUP, VK_SPACE, 0);
        qDebug() << "Wysłano klawisz Spacja";

        (*currentIndex)++;
    });

    keyTimer->start(interval);
}
