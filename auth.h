#ifndef AUTH_H
#define AUTH_H

#include <QDialog>
#include <QCryptographicHash>

namespace Ui {
class auth;
}

class auth : public QDialog {
    Q_OBJECT

public:
    explicit auth(QWidget *parent = nullptr);
    ~auth();

    bool isAuthenticated() const;

private slots:
    void on_pushButton_clicked();

private:
    Ui::auth *ui;
    bool authenticated;
    QString hashPin(const QString &pin) const;

    QString loadPinHash();
};

#endif // AUTH_H
