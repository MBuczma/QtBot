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

signals:
    void zadajUsuniecie(GroupBoxControl *kto);

public:
    explicit GroupBoxControl(QWidget *parent = nullptr);
    ~GroupBoxControl();
    QString getAllData() const;
    bool wysylanieStart();
    bool wysylanieStop();
    void setAllData(const QString &);
    void czyPotwierdzicUsuniecie();

private slots:
    void handleStartStop();
    void ZlapIdOkna();
    void zaktualizujNazwe();
    void aktualizujStanPrzyciskuStart();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupGroupBox();
    void startSending();
    void stopSending();
    void wyslijKlawisz();

    std::unique_ptr<QTimer> keyTimer;
    std::unique_ptr<QTimer> countdownTimer;
    std::unique_ptr<AutoKeyPresser> autoKeyPresser;

    HWND handle = NULL;
    HWND parentHandle = NULL;
    QString windowText = "";
    QString parentWindowText = "";
    bool isSending = false;
    bool isButtonPressed = false;
    int remainingTime = 0;

    QPushButton *buttonStartStop;
    QPushButton *buttonPobierzID;
    QPushButton *buttonUsun;
    QComboBox *comboBox_Klawisz;
    QComboBox *comboBox_Hotkey;
    QSpinBox *spinBox_Sekund;
    QSpinBox *spinBox_Milisekund;
    QSpinBox *spinBox_WysleZa;
    QGroupBox *groupBox = nullptr;
    QHBoxLayout *layout;

    void aktualizujCountdown();
};

#endif // GROUPBOXCONTROL_H
