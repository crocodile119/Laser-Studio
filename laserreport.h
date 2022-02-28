#ifndef LASERREPORT_H
#define LASERREPORT_H
#include "centralwidget.h"
#include "laserpoint.h"
#include "reflector.h"
#include "binocular.h"
#include "footprintobject.h"
#include "beaminspector.h"
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
    void inspectorsValuation();
    void safetySignsValuation();

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
    QString htmlInspectors(const int &number);
    void buidInspectorsDocumentPart();
    QString htmlFootprints();
    QString htmlBinoculars();
    QString htmlSafetySigns();
    QString htmlClassifier();
    QString htmlMeteo();
    QString htmlSymbols();

    QString printSpecularReflectorCoefficients( vector< pair <double,double> > myVector);
    QString printReflectorTable( vector< pair <double,double> > myVector);
    void adjustWidth();

    void setIndoor(bool);
    bool isIndoor();

    void setReflectorsList(const QList<Reflector *> &);
    void setFootprintsList(const QList<FootprintObject *> &);
    void setBinocularsList(const QList<Binocular*> &);
    void setBeamInspectorsList(const QList<BeamInspector*> &);
    void setSafetySignsList(const QList<SafetySignItem*> &);
    void setReflectorsFilenameList(const QStringList &);
    void setReflectorsGraphImageList(const QList<QImage> &);
    void setInspectorsFilenameList(const QStringList &);
    void setInspectorsGraphImageList(const QList<QImage> &);

private:
    CentralWidget *laserWindow;

    QTextDocument *textDocument;
    QTextCursor myCursor;

    LaserPoint *laserpoint;
    Reflector *reflector;
    Binocular *binocular;
    FootprintObject *footprint;
    BeamInspector *beamInspector;
    SafetySignItem *safetySign;
    QString correction;

    QStringList effects;
    QStringList input;
    QStringList output;
    QStringList goggle;
    QStringList reflectors;
    QStringList binoculars;
    QStringList inspectors;
    QStringList footprints;
    QStringList safetySigns;
    QStringList laser;
    QStringList skin;
    QStringList classifierResultsOutput;
    QStringList classifierDetailsOutput;
    QStringList *firstPage;
    QStringList *entries;
    QStringList reflectorsImageName;
    QList<QImage> reflectorsGraphImage;
    QStringList inspectorsImageName;
    QList<QImage> inspectorsGraphImage;
    QRect previewRect;

    QList<Reflector*> myReflectors;
    QList<Binocular*> myBinoculars;
    QList<BeamInspector*> myBeamInspectors;
    QList<FootprintObject*> myFootprints;
    QList<SafetySignItem*> mySafetySigns;

    bool indoor;
    typeOfReport myTypeOfReport;
};

#endif // LASERREPORT_H
