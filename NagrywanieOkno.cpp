#include "NagrywanieOkno.h"
#include "ui_NagrywanieOkno.h"
#include <QMouseEvent>
#include <synchapi.h>
#include <windows.h>

NagrywanieOkno::NagrywanieOkno(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NagrywanieOkno)
    , autoKeyPresser(new AutoKeyPresser(this))
    , handle(0)
    , parentHandle(0)
    , windowText("windowText")
    , parentHandleWindowText("parentHandleWindowText")
    , isButtonPressed(false)
{
    ui->setupUi(this);
    connect(ui->pushButton_Start, &QPushButton::clicked, this, &NagrywanieOkno::rozpocznijWysylanie);
    connect(ui->pushButton_PobierzID, &QPushButton::pressed, this, &NagrywanieOkno::ZlapIdOkna);
}

NagrywanieOkno::~NagrywanieOkno()
{
    delete autoKeyPresser;
    delete ui;
}

void NagrywanieOkno::ZlapIdOkna()
{
    qDebug() << "\n" << "Metoda NagrywanieOkno::ZlapIdOkna() został wywołana.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void NagrywanieOkno::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressed == true){
        qDebug() << "NagrywanieOkno::mouseReleaseEvent.";
        isButtonPressed = false;
        autoKeyPresser->WindowHandleFromPoint(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        ui->pushButton_PobierzID->setDown(false);
        windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
        ui->lineEdit_NazwaProgramu->setText(windowText);
    }
}

void NagrywanieOkno::rozpocznijWysylanie()
{
    int robaki = ui->lineEdit_Robaki->text().toInt();
    qDebug() << "Robaki to." << robaki;
    for(int i=0; i < robaki; ++i)
    {
        PostMessage(handle, WM_KEYDOWN, 'Z', 0);
        PostMessage(handle, WM_KEYUP, 'Z', 0);
        Sleep(100);
        PostMessage(handle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
        PostMessage(handle, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        Sleep(100);
        PostMessage(handle, WM_KEYDOWN, VK_SPACE, 0);
        PostMessage(handle, WM_KEYUP,   VK_SPACE, 0);
        Sleep(500);
    }
}
