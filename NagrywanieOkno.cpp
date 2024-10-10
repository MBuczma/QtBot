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

void NagrywanieOkno::zaktualizujNazwe()
{
    windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
    parentHandleWindowText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);
    if (windowText == parentHandleWindowText) {
        ui->groupBoxMouseTracker->setTitle(parentHandleWindowText);
    } else {
        ui->groupBoxMouseTracker->setTitle(parentHandleWindowText + " " + windowText);
    }
}

void NagrywanieOkno::rozpocznijWysylanie()
{
    int robaki = ui->lineEdit_Robaki->text().toInt();
    qDebug() << "Robaki to:" << robaki;

    for (int i = 0; i < robaki; ++i) {
        qDebug() << i;
        // Wysyłanie klawisza "Z"
        PostMessage(handle, WM_KEYDOWN, 'Z', 0);
        PostMessage(handle, WM_KEYUP, 'Z', 0);
        Sleep(500);
        // Symulacja kliknięcia myszki
        PostMessage(handle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        PostMessage(handle, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        Sleep(500);
        // Wysyłanie klawisza "Spacja"
        PostMessage(handle, WM_KEYDOWN, VK_SPACE, 0);
        PostMessage(handle, WM_KEYUP, VK_SPACE, 0);

        Sleep(200);
    };
}
