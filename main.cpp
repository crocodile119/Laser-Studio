#include <QApplication>
#include <QObject>
#include <QSplashScreen>
#include <QMessageBox>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("fusion");
    Q_INIT_RESOURCE(LaserStudio);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.png"));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Preparo l'applicazione..."),
                        topRight, Qt::black);
    MainWindow mainWin;

    //la risoluzione minima richiesta è 1360x768px

    splash->showMessage(QObject::tr("Carico i moduli..."),
                        topRight, Qt::black);
    //loadModules();
    splash->showMessage(QObject::tr("Stabilisco le connessioni..."),
                        topRight, Qt::black);
   // establishConnections();
    mainWin.showMaximized();
    splash->finish(&mainWin);

    delete splash;

    return app.exec();
}
