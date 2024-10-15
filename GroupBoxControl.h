#ifndef GROUPBOXCONTROL_H
#define GROUPBOXCONTROL_H

#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>
#include "AutoKeyPresser.h"

class GroupBoxControl : public QWidget
{
    Q_OBJECT

public:
    explicit GroupBoxControl(QWidget *parent = nullptr);
    ~GroupBoxControl();

private slots:
    void ZlapIdOkna();
    void handleStartStop();
    void sendKey();
    void getWindowHandle();
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupGroupBox();

    QGroupBox *groupBox;
    QPushButton *buttonStartStop;
    QPushButton *buttonPobierzID;
    QComboBox *comboBox_Klawisz;
    QComboBox *comboBox_HotKey;
    QSpinBox *spinBox_Sekund;
    QSpinBox *spinBox_Milisekund;

    QTimer *keyTimer;
    bool isSending;
    bool isButtonPressed;
    HWND windowHandle; // Uchwyt do okna
    HWND handle;       // Uchwyt do okna
    HWND parentHandle; // Uchwyt do okna
};

#endif // GROUPBOXCONTROL_H
