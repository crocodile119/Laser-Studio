#ifndef DOCKCONTROLS_H
#define DOCKCONTROLS_H

#include <QDockWidget>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPolarChart>
#include "lasersafety.h"
#include "lasersafetycw.h"
#include "lasersafetymp.h"
#include "laserclasscw.h"
#include "laserclasssp.h"
#include "laserclassmp.h"
#include "laserskinsafety.h"
#include "laserskinsafetymp.h"
#include "lasergoggle.h"
#include "scalenumbersmodelview.h"
#include "mychartview.h"
#include "mypolarchartview.h"
#include "dockeffects.h"
#include "dockresults.h"
#include "dockskin.h"
#include "dockgoggle.h"
#include "docklea.h"
#include "reflectorsqlist.h"
#include "ui_dockeffects.h"
#include "ui_dockresults.h"
#include "ui_dockskin.h"
#include "ui_dockgoggle.h"
#include "ui_docklea.h"
#include "ui_reflectorsqlist.h"


namespace Ui {
class DockControls;
}

class DockControls : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockControls(QWidget *parent, DockResults *dockResults, DockEffects *dockEffects,
                          DockSkin *dockSkin, DockGoggle *dockGoggle, DockLea *dockLea);
    ~DockControls();
    Ui::DockControls *ui;

    static const int DOCKGOGGLEMINIMUN;
    static const int DOCKGOGGLEMAXIMUN;

    void fetchDataVector();
    void fetchDDataVector();
    void fetchLaserOutput();
    void fetchDLaserOutput();
    void dispayScaleNumber();
    void dispayDScaleNumber();
    void displayLaserOutput();
    void displayDLaserOutput();
    void displayNumberOfPulse();
    void displayCoefficient_ki();
    void displayCoefficient_k();
    void display_ni_max();
    void displayTimeBase();
    void displayDTimeBase();
    void showControls(bool);
    void setWidgets();
    void setSkinWidgetsSingle();
    void setOpticalDistance();
    void setSkinDistances();   
    void setLambertianMax();
    //void setBeamDiameterDial(const int);
    //void setPowerErgDial(const int);
    void setDialControls();
    void updateGoggle();
    void setGoggleMaterial(LaserGoggle::material);
    LaserGoggle::material getGoggleMaterial()const;

    void setUVA();
    void setVIS();
    void setIRA();
    void setIRB();
    void setIRC();

    double getEMP()const;
    double getBeamDiameter()const;
    double getPowerErg()const;

    double getWavelength()const;
    double getPowerErgForEMP()const;

    double getOpticalDistance()const;
    double getSkinDistances()const;
    double getDivergence()const;
    double getLambertianMax()const;
    double getExposureTime()const;

    bool isTeEdtitingEnabled();

    MyChartView* getChartView()const;
    MyChartView* getdChartView()const;

    void computeOpticalDensity();
    void dComputeOpticalDensity();
    double getOpticalDensity();
    double getDOpticalDensity();
    int get_n_laser()const;
    bool isModeLocking();
    void effectivePowerErgPeak();
    void modeLockingPeak();
    void enablePeakControl();
	
	//funzioni membro lea
    void set_LEA_Widgets();
    void setWidgetsForOperation(LaserClassMP *);
    void setWidgetsForCW_Operation();
    void setWidgetsForSinglePulse_Operation();
    void setWidgetsForMultiPulse_Operation();
    void setWidgetsForThermal();
    void setWidgetsForThermalTi();    
    QString getLaserClassString(const LaserClassCW::laserClass &);


private slots:
    void on_operationCombo_currentIndexChanged(int index);
    void on_powerErgControl_valueChanged();
    void on_alphaControl_valueChanged();
    void on_pulseControl_valueChanged();
    void on_divergenceControl_valueChanged();
    void on_beamDiameterControl_valueChanged();
    void on_prfControl_valueChanged();
    void on_T_SkinControl_valueChanged();
    void on_teControl_valueChanged();
    void on_wavelengthScrollBar_valueChanged(int value);
    void on_peakControl_valueChanged();
    void setBeamDiameter();
    void setEMP();
    void setPowerErgForEMP();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_enableTeCheckBox_toggled(bool checked);   
    void on_pushButton_toggled(bool checked);
    void on_checkGaussianBeam_clicked(bool checked);
    void on_internalWaist_checkBox_toggled(bool checked);

signals:
    void modified();
    void wavelengthChanged();
    void NOHD_Changed();
    void NSHD_Changed();
    void lambertianMaxChanged();
    void divergenceChanged();
    void beamDiameterChanged();
    void powerErgChanged();
    void powerErgForEMPChanged();
    void EMP_Changed();
    void noFeasibleInput();
    void operationChanged();

private:
    //Variabili membro riguardanti i controlli
    int operationCombo;
    double powerErg;
    bool gaussianBeam;
    bool internalWaist;
    double beamCorrection;
    double alpha;
    double pulseWidth;
    double divergence;
    double beamDiameter;
    double prf;
    double T_Skin;
    double wavelength;
    double powerErgPeak;

    double effectivePowerErg;
    double myEMP;
    double powerErgForEMP;
    double myTimeBase;
    double NOHD;
    double NSHD;
    double lambertianMax;
    bool enableTeEditing;
    double exposureTimeControl;
    double opticalDensity;
    double dOpticalDensity;

    DockResults *dockResults;
    DockEffects *dockEffects;
    DockSkin *dockSkin;
    DockGoggle *dockGoggle;
    DockLea *dockLea;
    ReflectorsQList *dockReflectorsList;

    int n_laser;
    MyPolarChartView *polarChartView;
    enum {MagicNumber = 0x7E51D683, RowCount = 10, RowCountMPE=11, ColumnCountMPE=6, RowCountData=17, ColumnCountData=1};
    ScaleNumbersModelView *myModel;
    ScaleNumbersModelView *myDModel;
    LaserGoggle *myLaserGoggle;
    LaserGoggle *myDLaserGoggle;
    string myNewGoggleMark;
    MyChartView *chartView;
    MyChartView *dChartView;
    string myPulseCode;
    string myDPulseCode;
    vector<pair<int, double>> dataVector;
    vector<pair<int, double>> frequencyDataVector;
    double laserOutput;
    double dLaserOutput;
    LaserSafetyMP* MyLaserSafetyMP;
    LaserSafetyCW* MyLaserCW_Pr;
    LaserSafety* MyLaserSP_Pr;
    LaserSafetyMP* MyLaserMP_Pr;
    //Laser danni pelle
    LaserSkinSafetyMP* MyLaserSkinSafetyMP;
    LaserSkinSafety* MyLaserSkinSP_Pr;
    LaserSkinSafetyMP* MyLaserSkinMP_Pr;
	
    LaserClassMP* MyLaserClassMP;
    LaserClassCW* MyLaserClassCW_Pr;
    LaserClassSP* MyLaserClassSP_Pr;
    LaserClassMP* MyLaserClassMP_Pr;

    LaserGoggle::material goggleMaterial;
};

#endif // DOCKCONTROLS_H
