/* GroupBoxControl.h */
/*
 * Groupbox który zawiera pola do wprowadzania danych, wyświetla informacje oraz
 * odpowiada za start i stop poszczególnych przycisków. 
 */
#ifndef GROUPBOXCONTROL_H
#define GROUPBOXCONTROL_H

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

class AutoKeyPresser;

class GroupBoxControl : public QWidget
{
    Q_OBJECT

public:
    explicit GroupBoxControl(QWidget *parent = nullptr);
    ~GroupBoxControl();

private slots:
    void handleStartStop();
    void ZlapIdOkna();
    void zaktualizujNazwe();
    void aktualizujStanPrzyciskuStartNaPodstawieComboBox(const QString &text);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupGroupBox();
    void startSending();
    void stopSending();
    void sendKey();
    void getHandle();

    std::unique_ptr<QTimer> keyTimer;
    std::unique_ptr<QTimer> countdownTimer;
    std::unique_ptr<AutoKeyPresser> autoKeyPresser;

    HWND handle = NULL;
    HWND parentHandle = NULL;
    QString windowText = "";
    QString parentHandleWindowText = "";
    bool isSending = false;
    bool isButtonPressed = false;
    int remainingTime = 0;

    QPushButton *buttonStartStop;
    QPushButton *buttonPobierzID;
    QComboBox *comboBox_Klawisz;
    QComboBox *comboBox_Hotkey;
    QSpinBox *spinBox_Sekund;
    QSpinBox *spinBox_Milisekund;
    QSpinBox *spinBox_WysleZa;
    QGroupBox *groupBox = nullptr;
    QHBoxLayout *layout;

    void wyslijKlawisz();
    void aktualizujCountdown();
    void aktualizujStanPrzyciskuStart(bool isSending);
};

#endif // GROUPBOXCONTROL_H
