#include "laserapplication.h"
#include <exception>
#include <QEvent>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <QMessageBox>
#include <ctime>

LaserApplication::LaserApplication(int& argc, char** argv) : QApplication(argc, argv)
{
}

bool LaserApplication::notify(QObject* receiver, QEvent* event)
{
    bool done = true;
    try
    {
        done = QApplication::notify(receiver, event);
    }
    catch (std::exception & e)
    {
        QString exceptionString ="E' stata lanciata un'eccezione.\n"
                                 "I dettagli sull'errore verranno salvati in error.dat. \n"
                                 "Eccezione: ";

        exceptionString.append(e.what());
        std::time_t result =std::time(nullptr);


        int ret = QMessageBox::critical(nullptr, tr("Errore"), exceptionString,  QMessageBox::Ok);
        // exit program if unable to create file
        if(ret)
        {
            std::cerr << "Exception thrown : "<< e.what() <<"\n";
            std::fstream outClientFile{"error.dat", std::ios::app};

            QString object =receiver->objectName();
            std::string objectName=object.toStdString();
            outClientFile<<"Eccezione: "<< objectName <<  " " << e.what() << " " << std::asctime(std::localtime(&result));
            if (!outClientFile)
            {
                int ret2 = QMessageBox::critical(nullptr, tr("Errore"), tr("Non è possibile """
                                                                            "salvare il file error.dat"),
                                                                           QMessageBox::Ok);
                if(ret2)
                    exit(EXIT_FAILURE);
            }
        }

    }
    catch(...)
    {
        QString exceptionString ="E' stata lanciata un eccezione.\n"
                                 "I dettagli sull'errore verranno salvati in error.dat. \n"
                                 "Eccezione: ";

        std::time_t result =std::time(nullptr);


        int ret = QMessageBox::critical(nullptr, tr("Errore"), exceptionString,  QMessageBox::Ok);
        // exit program if unable to create file
        if(ret)
        {
            std::cerr << "Exception thrown. <<\n";
            std::fstream outClientFile{"error.dat", std::ios::app};

            QString object =receiver->objectName();
            std::string objectName=object.toStdString();
            outClientFile<<"Eccezione: "<< objectName << " " << std::asctime(std::localtime(&result));
            if (!outClientFile)
            {
                int ret2 = QMessageBox::critical(nullptr, tr("Errore"), tr("Non è possibile """
                                                                            "salvare il file error.dat"),
                                                                           QMessageBox::Ok);
                if(ret2)
                    exit(EXIT_FAILURE);
            }
        }
    }
    return done;
}
