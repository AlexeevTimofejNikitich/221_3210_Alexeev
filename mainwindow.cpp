#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QInputDialog>
#include <QLabel>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    this->setWindowTitle("221_3210_Alexeev");

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
    bool flag = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() != 4) continue;

        Transaction transaction = { fields[0], fields[1], fields[2], fields[3] };
        QString computedHash = computeHash(transaction, previousHash);

        if (transaction.hash != computedHash) {
            flag = true;
        }
        transactionErrors.append(flag);
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
    for (int i = 0; i < transactions.size(); ++i) {
        const auto &transaction = transactions[i];
        bool hasError = transactionErrors[i];

        if (hasError) {
            content += QString("<p style='color: red;'>Карта: %1<br>Маршрут: %2<br>Время: %3<br>Хеш: %4</p>")
                           .arg(transaction.cardNumber)
                           .arg(transaction.route)
                           .arg(transaction.time)
                           .arg(transaction.hash);
        } else {
            content += QString("<p>Карта: %1<br>Маршрут: %2<br>Время: %3<br>Хеш: %4</p>")
                           .arg(transaction.cardNumber)
                           .arg(transaction.route)
                           .arg(transaction.time)
                           .arg(transaction.hash);
        }
    }

    ui->textEdit->setHtml(content);
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


void MainWindow::on_pushButtonFile_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "CSV Files (*.csv);;All Files (*)");
    if (!filePath.isEmpty()) {
        transactions.clear();
        transactionErrors.clear();
        loadTransactions(filePath);
        displayTransactions();
    }
}
