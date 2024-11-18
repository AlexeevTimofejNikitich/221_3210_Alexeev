#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QInputDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    QString filePath = "../../../../../transactions.csv";
    loadTransactions(filePath);
    displayTransactions();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadTransactions(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString previousHash;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() != 4) continue;

        Transaction transaction = { fields[0], fields[1], fields[2], fields[3] };
        QString computedHash = computeHash(transaction, previousHash);

        if (transaction.hash != computedHash) {
            continue;
        }

        transactions.append(transaction);
        previousHash = computedHash;
    }

    file.close();
}

QString MainWindow::computeHash(const Transaction &transaction, const QString &previousHash) {
    QString data = transaction.cardNumber + transaction.route + transaction.time + previousHash;
    return QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md5).toHex();
}

void MainWindow::displayTransactions() {
    QString content;
    for (const auto &transaction : transactions) {
        content += QString("Карта: %1\nМаршрут: %2\nВремя: %3\nХеш: %4\n\n")
                       .arg(transaction.cardNumber)
                       .arg(transaction.route)
                       .arg(transaction.time)
                       .arg(transaction.hash);
    }

    ui->textEdit->setPlainText(content);
}

void MainWindow::on_pushButtonPin_clicked() {
    bool ok;
    QString newPin = QInputDialog::getText(this, "Смена пин-кода",
                                           "Введите новый пин-код:", QLineEdit::Password,
                                           "", &ok);
    if (ok && !newPin.isEmpty()) {
        QByteArray newPinHash = QCryptographicHash::hash(newPin.toUtf8(), QCryptographicHash::Sha256);
        if (savePinHash(newPinHash)) {
            QDialog successDialog(this);
            QLabel *label = new QLabel("Пин-код успешно обновлён!", &successDialog);
            QVBoxLayout *layout = new QVBoxLayout(&successDialog);
            layout->addWidget(label);
            successDialog.setLayout(layout);
            successDialog.exec();
        } else {
            QDialog successDialog(this);
            QLabel *label = new QLabel("Ошибка: Не удалось сохранить новый пин-код.", &successDialog);
            QVBoxLayout *layout = new QVBoxLayout(&successDialog);
            layout->addWidget(label);
            successDialog.setLayout(layout);
            successDialog.exec();
        }
    }
}

bool MainWindow::savePinHash(const QByteArray &hash) {
    QFile file("../../../../../pin.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << hash.toHex();
    file.close();
    return true;
}

