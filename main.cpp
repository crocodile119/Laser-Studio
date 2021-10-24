#include <QObject>
#include <QSplashScreen>
#include <exception>
#include <fstream>
#include <iostream>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "mainwindow.h"
#include "laserapplication.h"

int main(int argc, char *argv[])
{
    int ret;
    try
    {
        LaserApplication app(argc, argv);

        QSystemSemaphore semaphore("<uniq id>", 1);  // create semaphore
        semaphore.acquire(); // Raise the semaphore, barring other instances to work with shared memory

    #ifndef Q_OS_WIN32
        // in linux / unix shared memory is not freed when the application terminates abnormally,
        // so you need to get rid of the garbage
        QSharedMemory nix_fix_shared_memory("<uniq id 2>");
        if(nix_fix_shared_memory.attach()){
            nix_fix_shared_memory.detach();
        }
    #endif
//per inibire istanze multiple
        QSharedMemory sharedMemory("<uniq id 2>");  // Create a copy of the shared memory
        bool is_running;            // variable to test the already running application
        if (sharedMemory.attach()){ // We are trying to attach a copy of the shared memory
                                    // To an existing segment
            is_running = true;      // If successful, it determines that there is already a running instance
        }else{
            sharedMemory.create(1); // Otherwise allocate 1 byte of memory
            is_running = false;     // And determines that another instance is not running
        }
        semaphore.release();

        // If you already run one instance of the application, then we inform the user about it
        // and complete the current instance of the application
        if(is_running){
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Laser Studio già in esecuzione.");
            msgBox.exec();
            return 1;
        }

        app.setStyle("fusion");
        Q_INIT_RESOURCE(LaserStudio);

        QSplashScreen *splash = new QSplashScreen;
        splash->setPixmap(QPixmap(":/images/splash.png"));
        splash->show();
        Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
        splash->showMessage(QObject::tr("Preparo l'applicazione..."), topRight, Qt::black);

        MainWindow *mainWin= new MainWindow;
        //la risoluzione minima richiesta è 1360x768px

        splash->showMessage(QObject::tr("Carico i moduli..."), topRight, Qt::black);
        //loadModules();
        splash->showMessage(QObject::tr("Stabilisco le connessioni..."), topRight, Qt::black);
        // establishConnections();
        mainWin->showMaximized();
        splash->finish(mainWin);

        delete splash;
        ret = app.exec();
    }
    catch (...)
    {
        qDebug()<<"error";
        return EXIT_FAILURE; // exit the application
    }

    return ret;
}
