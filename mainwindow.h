#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVector>

struct Transaction {
    QString cardNumber;
    QString route;
    QString time;
    QString hash;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonPin_clicked();
    void on_pushButtonFile_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Transaction> transactions;
    QVector<bool> transactionErrors; // Ошибки для каждой транзакции

    void loadTransactions(const QString &filePath);
    QString computeHash(const Transaction &transaction, const QString &previousHash = "");
    void displayTransactions();

    bool savePinHash(const QByteArray &hash);
};

#endif // MAINWINDOW_H
