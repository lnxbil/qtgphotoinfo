
#include <QApplication>

#include "infodialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    InfoDialog dialog;
    dialog.show();
    return dialog.exec();
}
