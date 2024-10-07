/* AutoKeyPresser.h */
#ifndef AUTOKEYPRESSER_H
#define AUTOKEYPRESSER_H

#include <QWidget>

class AutoKeyPresser : public QWidget
{
    Q_OBJECT
public:
    explicit AutoKeyPresser(QWidget *parent = nullptr);
    ~AutoKeyPresser();
    void WindowHandleFromPoint(HWND &handle, HWND &parentHandle);
    QString GetWindowTextFromHandle(HWND hwnd);
    void SentKey(HWND &handle, QString key);

signals:

private slots:

private:

};
#endif // AUTOKEYPRESSER_H
