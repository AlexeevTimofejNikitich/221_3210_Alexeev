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

    const QString storedPinHash = "03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4"; //SHA-256
};

#endif // AUTH_H
