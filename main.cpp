#include "mainwindow.h"
#include "auth.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auth authDialog;
    if (authDialog.exec() != QDialog::Accepted || !authDialog.isAuthenticated()) {
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
