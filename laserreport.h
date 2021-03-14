#ifndef LASERREPORT_H
#define LASERREPORT_H
#include "centralwidget.h"
#include "laserpoint.h"
#include "reflector.h"
#include "binocular.h"
#include "footprintobject.h"
#include "dockcontrols.h"

class LaserReport
{

public:
    enum typeOfReport{ODF, PDF};
    LaserReport(CentralWidget*, LaserPoint*, typeOfReport);

    QTextDocument *buildReportDocument();
    void laserAssessmentResults();

    void classifierDetails();
    void classifierResults();
    void setSelectionRect(const QRect & _rect);
    void firstPageReport();

    void reflectorsValuation();
    void binocularsValuation();
    void footprintsValuation();

    QString htmlInstallationDescription();
    QString htmlLaserInstallation();
    QString kindOfLaser();
    QString htmlInputData(const QString & kindOfLaser);
    QString htmlEyeExposition();
    QString htmlSkinExposition();
    QString htmlEffects();
    QString htmlGoggleAssessment();
    QString htmlDGoggleDetails();
    QString htmlGoggleDetails();

    QString htmlReflectors(const int &number);
    void buidReflectorsDocumentPart();
    QString htmlFootprints();
    QString htmlBinoculars();
    QString htmlClassifier();
    QString htmlMeteo();

    QString printSpecularReflectorCoefficients( vector< pair <double,double> > myVector);
    QString printReflectorTable( vector< pair <double,double> > myVector);
    void adjustWidth();

    void setIndoor(bool);
    bool isIndoor();

    void setReflectorsList(const QList<pair<Reflector *, int> > &);
    void setFootprintsList(const QList<pair<FootprintObject*, int>> &);
    void setBinocularsList(const QList<pair<Binocular*, int>> &);
    void setReflectorsFilenameList(const QStringList &);
    void setReflectorsGraphImageList(const QList<QImage> &);

private:
    CentralWidget *laserWindow;

    QTextDocument *textDocument;
    QTextCursor myCursor;

    LaserPoint *laserpoint;
    Reflector *reflector;
    Binocular *binocular;
    FootprintObject *footprint;
    QString correction;

    QStringList effects;
    QStringList input;
    QStringList output;
    QStringList goggle;
    QStringList reflectors;
    QStringList binoculars;
    QStringList footprints;
    QStringList laser;
    QStringList skin;
    QStringList classifierResultsOutput;
    QStringList classifierDetailsOutput;
    QStringList *firstPage;
    QStringList *entries;
    QStringList reflectorsImageName;
    QList<QImage> reflectorsGraphImage;
    QRect previewRect;

    QList<pair<Reflector*, int>> myReflectors;
    QList<pair<Binocular*, int>> myBinoculars;
    QList<pair<FootprintObject*, int>> myFootprints;

    bool indoor;
    typeOfReport myTypeOfReport;
};

#endif // LASERREPORT_H
