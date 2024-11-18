#include "auth.h"
#include "ui_auth.h"
#include <QLabel>

auth::auth(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::auth),
    authenticated(false) {
    ui->setupUi(this);
}

auth::~auth() {
    delete ui;
}

bool auth::isAuthenticated() const {
    return authenticated;
}

QString auth::hashPin(const QString &pin) const {
    QByteArray hash = QCryptographicHash::hash(pin.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}


void auth::on_pushButton_clicked() {
    QString enteredPin = ui->lineEdit->text();
    if (hashPin(enteredPin) == storedPinHash) {
        authenticated = true;
        accept();
    } else {
        QDialog errorDialog(this);
        errorDialog.setWindowTitle("Ошибка");

        QVBoxLayout *layout = new QVBoxLayout(&errorDialog);
        QLabel *label = new QLabel("Попробуйте ещё раз.", &errorDialog);
        QPushButton *okButton = new QPushButton("OK", &errorDialog);

        layout->addWidget(label);
        layout->addWidget(okButton);

        connect(okButton, &QPushButton::clicked, &errorDialog, &QDialog::accept);

        errorDialog.exec();
        authenticated = false;
    }
}

