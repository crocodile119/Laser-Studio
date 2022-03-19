#include "dockcontrols.h"
#include "ui_dockcontrols.h"
#include <QFile>
#include <QMessageBox>
#include <QtDebug>
#include <cmath>
#include <iostream>
#include <string>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QListView>
#include "docklea.h"
#include "mypolarchartview.h"
#include "htmldelegate.h"
#include <exception>
#include "beaminspector.h"

const int DockControls::DOCKGOGGLEMINIMUN=405;
const int DockControls::DOCKGOGGLEMAXIMUN=550;
const double DockControls::MODELOCKED_LIMIT=std::pow(10,-9);

DockControls::DockControls(QWidget *parent, DockResults *_dockResults, DockEffects *_dockEffects,
                           DockSkin *_dockSkin, DockGoggle *_dockGoggle, DockLea* _dockLea)
                           : QDockWidget(parent), ui(new Ui::DockControls),
                             powerErg(LaserSafetyCW::POWER), wavelength(EmpLeaTables::HE_NE_WAVELENGTH), pulseWidth(EmpLeaTables::NATURAL_AVERSION_TIME),
                             alpha(EmpLeaTables::ALPHA_MIN),  divergence(LaserSafety::DIVERGENCE), beamDiameter(LaserSafety::PUPIL_DIAMETER),
                             prf(LaserSafetyMP::PULSE_REPETITION_FREQUENCY), dockResults(_dockResults), dockEffects(_dockEffects),
                             dockSkin(_dockSkin), dockGoggle(_dockGoggle), dockLea(_dockLea), MyLaserSafetyMP(new LaserSafetyMP),
                             MyLaserSkinSafetyMP(new LaserSkinSafetyMP), MyLaserClassMP(new LaserClassMP)
{
    ui->setupUi(this);
    n_laser=operation::CONTINUOS_WAVE;

    setUpGoggle();

    showControls(false);
    dockGoggle->setFixedWidth(DOCKGOGGLEMINIMUN);
    exposureTimeControl=10;
    gaussianBeam=true;
    beamCorrection=1.0;
    lambertianMax=0.109557;
    internalWaist=false;
    isThermalHazard=true;

    MyLaserCW_Pr=nullptr;
    MyLaserSP_Pr=nullptr;
    MyLaserMP_Pr=nullptr;
    MyLaserCW_Pr=MyLaserSafetyMP;

    MyLaserSkinSP_Pr=nullptr;
    MyLaserSkinMP_Pr=nullptr;
    MyLaserSkinSP_Pr=MyLaserSkinSafetyMP;

    /************************************************************************************************
    * Imposto i valori dei controlli con le relative variabili membro.                             *
    ************************************************************************************************/
    MyLaserClassCW_Pr=nullptr;
    MyLaserClassSP_Pr=nullptr;
    MyLaserClassMP_Pr=nullptr;
    MyLaserClassCW_Pr=MyLaserClassMP;

    setDataForCW_Operation();
    setCW_SP_LEAModel();

    setDialControls();

    displayScaleNumber();
    displayLaserOutput();

    setObjectName(tr("Controlli"));

    enableTeEditing=false;
    setEMP();
    setEMP_Sort();
    setPowerErgForEMP();

    connect(this, SIGNAL(EMP_Changed()), this, SLOT(setEMP()));
    connect(this, SIGNAL(EMP_Changed()), this, SLOT(setEMP_Sort()));
    connect(this, SIGNAL(powerErgForEMPChanged()), this, SLOT(setPowerErgForEMP()));
    connect(dockGoggle->ui->showGoggleCharts, SIGNAL(toggled(bool)), this, SLOT(showGoggleCharts(bool)));
    connect(ui->wavelengthScrollBar, SIGNAL(valueChanged(int)), this, SLOT(setLEAModelForWavelength()));
}

void DockControls::showGoggleCharts(bool checked)
{
    double width;

    showControls(n_laser==operation::MULTI_PULSE);

    if(checked)
    {
        if(n_laser==operation::MULTI_PULSE)
            dChartView->show();

        chartView->show();
        width=DOCKGOGGLEMINIMUN+450;

        dockGoggle->setFixedWidth(width);
        dockGoggle->ui->showGoggleCharts->setText("<<");
    }

    else
    {
        if(n_laser==operation::MULTI_PULSE)
            dChartView->hide();

        chartView->hide();
        dockGoggle->setFixedWidth(DOCKGOGGLEMINIMUN);
        dockGoggle->ui->showGoggleCharts->setText(">>");
    }
}

void DockControls::setUpGoggle()
{

    /**********************************************************************************************
    * LaserGoggle ha due costruttori quello a 4 parametri che prevede il funzionamento            *
    * a frequenza nulla e il secondo con cinque parametri per il quale è specificata la           *
    * frequenza di ripetizione dgli impulsi.                                                      *
    * Inizializzo laser con un valore di frequenza nulla per uso in funzionamento CW o impulsato. *
    ***********************************************************************************************/


    //Impiego il costruttore con cinque parmetri wavelength, pulseWidth, powerErg, beamDiameter, frequenza

    myLaserGoggle=new LaserGoggle(wavelength, LaserGoggle::CONTINUOS_OPERATION, powerErg, beamDiameter, LaserGoggle::CONTINUOS_OPERATION);
    string myNewGoggleMark = myLaserGoggle->goggleMark(wavelength, LaserGoggle::CONTINUOS_OPERATION, powerErg, beamDiameter, LaserGoggle::CONTINUOS_OPERATION);

    //Ricavo il calori riguardanti la tabella EN 207 corrispondente alla lunghezza d'onda e alla base dei tempi
    //necassaria sia per costruire la il Widget della tabella dei numeri di scala e il grafico.
    vector<pair<int, double>> dataVector;
    dataVector=myLaserGoggle->getDataVector();

    //il valore di laser output serve per tracciare la retta orizzontale relativa all'uscita del laser.
    laserOutput=myLaserGoggle->laserIrrRadCorrected(powerErg);

    myModel=new ScaleNumbersModelView(this);
    myModel->setTableList(dataVector);
    dockGoggle->ui->tableView->setModel(myModel);

    chartView = new MyChartView(0, dataVector, laserOutput);
    chartView->setRenderHint(QPainter::Antialiasing);

    dockGoggle->ui->gridLayout_5->addWidget(chartView, 1, 0, Qt::AlignCenter);

    /************************************************************************************************
    * Essendo il funzionamento all'avvio di tipo Continuos Wave disabilito sia il controllo        *
    * dell'impulso che della frequenza di ripetizione degli impulsi che il valore di picco della   *
    * corrente (riguarda il funzionamento del tipo mode locking).                                  *
    ************************************************************************************************/

    ui->pulseControl->setEnabled(false);
    ui->prfControl->setEnabled(false);

    /************************************************************************************************
    * Imposto la base dei tempi nel funzionamento Continuos Wave all'avvio. La base dei tempi è    *
    * quella preveista dala norma EN207 e non ha relazione con la base dei tempi impiegata per il  *
    * calcolo dell'EMP dell'impulso che della frequenza di ripetizione degli impulsi.              *                *
    ************************************************************************************************/
    double myGoggleTimeBase;
    if((wavelength>=180)&&(wavelength<=315))
        myGoggleTimeBase=LaserGoggle::TIMEBASE;
    else
        myGoggleTimeBase=LaserGoggle::TIMEBASE_LOW_WAVELENGTH;

    /***********************************************************************************************
    * Inizializzo laser frequenza nulla per uso calcolare gli effetti medi nel caso di             *
    * funzionamento ad impulsi multipli. Valgono e considerazioni fatte in precedenza.             *                                                          *
    ************************************************************************************************/

    const double ZERO_POWER_ERG=0;
    myDLaserGoggle=new LaserGoggle(LaserGoggle::HE_NE_WAVELENGTH, myGoggleTimeBase, ZERO_POWER_ERG, LaserGoggle::PUPIL_DIAMETER);
    string myNewDGoggleMark = myDLaserGoggle->goggleMark(LaserGoggle::HE_NE_WAVELENGTH, myGoggleTimeBase, 0, LaserGoggle::PUPIL_DIAMETER,
                                                         LaserGoggle::CONTINUOS_OPERATION);

    const double NO_ZERO_POWER_ERG=1;
    frequencyDataVector=myDLaserGoggle->getDataVector();
    dLaserOutput=myDLaserGoggle->laserIrrRadCorrected(NO_ZERO_POWER_ERG);
    myDModel=new ScaleNumbersModelView(this);
    myDModel->setTableList(frequencyDataVector);
    dockGoggle->ui->dTableView->setModel(myDModel);

    dChartView = new MyChartView(0, frequencyDataVector, dLaserOutput);
    dChartView->setRenderHint(QPainter::Antialiasing);
    dockGoggle->ui->dGridLayout->addWidget(dChartView, 1, 0, Qt::AlignCenter);
    //inizializzo il laser per il calcolo dei livelli di emissione e la NOHD occhi

}
void DockControls::on_wavelengthScrollBar_valueChanged(int value)
{
    /******************************************************************
    * Assegno il valore associato al controllo alla variabile membro  *
    *******************************************************************/

    /*********************************
    * casting della variabile value. *
    **********************************/

    wavelength=static_cast<double>(value);

    /*****************
    * CONTINUOS WAVE *
    ******************/
					
    if(n_laser==operation::CONTINUOS_WAVE)
    {
        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/

        MyLaserCW_Pr->setWavelength(wavelength);
        MyLaserCW_Pr->setExposureTime();
        MyLaserCW_Pr->setPulseWidth(MyLaserCW_Pr->getExposureTime());
        MyLaserSkinSP_Pr->setWavelength(wavelength);

        MyLaserClassCW_Pr->setWavelength(wavelength);
        MyLaserClassCW_Pr->setTimeBase();

        /*********************************************************************
        * Imposto il valore nell'oggetto occhiali protettori.                *
        * Il valore della base dei tempi dipende dalla lunghezza d'onda e va *
        * pertanto impostato in funzione di questa (EN 207 B.4).             *
        **********************************************************************/

        myLaserGoggle->setWavelength(wavelength);

        if(((wavelength>315) && (wavelength<=1e+06)))//base dei tempi 5 s
            myLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE);
        else
        if(((myLaserGoggle->getWavelength()>=180) && (myLaserGoggle->getWavelength()<=315)))//base dei tempi 30000 s
            myLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE_LOW_WAVELENGTH);

        enableTeEditing=ui->teControl->isEnabled();
        /*************************************************************
        * La funzione fetchDataVector() va invocata quando cambia    *
        * la frequenza o la durata dell'impulso.                     *                                 *
        * Seleziona il campo corrispondente della Tabella B.2 EN207  *
        * per l'oggetto myLaserGoggle.                               *
        * Memorizza il vettore corrispondente in dataVector ed       *
        * aggiorna il grafico                                        *
        **************************************************************/

        modeLockedPeak();
        fetchDataVector();
        fetchLaserOutput();

        /************
        * IMPULSATO *
        *************/
    }
    else if(n_laser==operation::PULSE)
    {  
        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/

        MyLaserSP_Pr->setWavelength(wavelength);
        MyLaserSkinSP_Pr->setWavelength(wavelength);
        MyLaserClassSP_Pr->setWavelength(wavelength);

        /*********************************************************************
        * Imposto il valore nell'oggetto occhiali protettori.                *
        * Il valore della base dei tempi dipende dalla lunghezza d'onda e va *
        * pertanto impostato in funzione di questa (EN 207 B.4).             *
        * In questo caso si assume come tempo base la durata dell'impulso.   *
        **********************************************************************/

        myLaserGoggle->setWavelength(wavelength);
        myLaserGoggle->setPulseWidth(pulseWidth);

        modeLockedPeak();

        fetchLaserOutput();

        /*************************************************************
        * La funzione fetchDataVector() va invocata quando cambia    *
        * la frequenza o la durata dell'impulso.                     *                                 *
        * Seleziona il campo corrispondente della Tabella B.2 EN207  *
        * per l'oggetto myLaserGoggle.                               *
        * Memorizza il vettore corrispondente in dataVector ed       *
        * aggiorna il grafico                                        *
        **************************************************************/
        fetchDataVector();

        /*****************************************************
        * Visualizzazione dati relativi ai protettori ottici *
        ******************************************************/
    }

    /*******************
    * IMPULSI MULTIPLI *
    ********************/

    else if(n_laser==operation::MULTI_PULSE)
    {

        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserMP_Pr->setWavelength(wavelength);
        MyLaserMP_Pr->setExposureTime();
        MyLaserMP_Pr->setPulseWidth(pulseWidth);

        MyLaserSkinMP_Pr->setWavelength(wavelength);
        MyLaserSkinMP_Pr->setPulseWidth(pulseWidth);

        MyLaserMP_Pr->computeTmin();

        MyLaserClassMP_Pr->setWavelength(wavelength);
        MyLaserClassMP_Pr->setTimeBase();

        /**********************************************************************
        * Imposto il valore negli oggetti occhiali protettori.                *
        * Il valore della base dei tempi dipende dalla lunghezza d'onda e va  *
        * pertanto impostato in funzione di questa (EN 207 B.4).              *
        * Per l'oggetto myLaserGoggle si procede impostando il valore della   *
        * durata dell'impulso.                                                *
        * Per l'oggetto myDLaserGoggle il valore della base dei tempi dipende *
        * dalla lunghezza d'onda. Imposto in modo esplicito la base dei tempi *
        **********************************************************************/

        myLaserGoggle->setWavelength(wavelength);
        myDLaserGoggle->setWavelength(wavelength);
        myLaserGoggle->setPulseWidth(pulseWidth);

        if(((myDLaserGoggle->getWavelength()>315) && (myDLaserGoggle->getWavelength()<=1e+06)))//base dei tempi 5 s
            myDLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE);
        else
        {
            if(((myDLaserGoggle->getWavelength()>=180) && (myDLaserGoggle->getWavelength()<=315)))//base dei tempi 30000 5
                myDLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE_LOW_WAVELENGTH);
        }
        /*************************************************************
        * La funzione fetchDataVector() va invocata quando cambia    *
        * la frequenza o la durata dell'impulso.                     *
        * Seleziona il campo corrispondente della Tabella B.2 EN207  *
        * per entrambi gli oggetti relativi ai protettori ottici.    *
        * Memorizza il vettore corrispondente in dataVector ed       *
        * aggiorna il grafico                                        *
        **************************************************************/
        modeLockedPeak();
        fetchLaserOutput();
        fetchDataVector();

        /*****************************************************
        * Visualizzazione dati relativi ai protettori ottici *
        ******************************************************/
        //Oggetto myLaserGoggle
        fetchDDataVector();
    }

    /******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
    set_LEA_Widgets();
    kindOfHazardChanged();
    setLaserGoggleWidgets();

    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/

    setLambertianMax();

    emit NOHD_Changed();
    emit EMP_Changed();//Cambia l'EMP
    emit wavelengthChanged();//la lunghezza d'onda è cambiata
    emit modified();//Per salvataggio file
}

void DockControls::on_teControl_valueChanged()
{
    if(isTeEdtitingEnabled())
    {
        exposureTimeControl=ui->teControl->getDialNumber();
        /*****************
        * CONTINUOS WAVE *
        ******************/

    if(n_laser==operation::CONTINUOS_WAVE)
    {
        /********************************************************
        * Imposto il valore del tempo di esposizione impostato *
        ********************************************************/
        MyLaserCW_Pr->setExposureTimeEditable(true);
        MyLaserCW_Pr->setEditedExposureTime(exposureTimeControl);
        MyLaserCW_Pr->setPulseWidth(exposureTimeControl);
    }
    else
    if(n_laser==operation::MULTI_PULSE)
        /*******************
        * IMPULSI MULTIPLI *
        ********************/
        {
        /*******************************************************
        * Imposto il valore del tempo di esposizione impostato *
        ********************************************************/
        MyLaserMP_Pr->setExposureTimeEditable(true);
        MyLaserMP_Pr->setEditedExposureTime(exposureTimeControl);
        MyLaserMP_Pr->setPulseWidth(pulseWidth);
        }
        setWidgets();
        setPowerErgForEMP();
        setLambertianMax();
        emit EMP_Changed();//Cambia l'EMP
        emit modified();//Per salvataggio file
    }
}

void DockControls::on_T_SkinControl_valueChanged()
{
    /*******************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/

    int exponent=ui->T_SkinControl->getExponent();
    qDebug()<< "Esponente di T_SkinControl: "<<exponent;
    double mantissa=ui->T_SkinControl->getMantissa();

    /*******************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/

    T_Skin=mantissa*std::pow(10, exponent);

    /*****************
    * CONTINUOS WAVE *
    * ****************/
    if(n_laser==operation::CONTINUOS_WAVE){

        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserSkinSP_Pr->setPulseWidth(T_Skin);
    }
    else

    /************
    * IMPULSATO *
    *************/
    if(n_laser==operation::PULSE)
    {
        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserSkinSP_Pr->setPulseWidth(pulseWidth);
    }
    else

    /*******************
    * IMPULSI MULTIPLI *
    ********************/
    if (n_laser==operation::MULTI_PULSE){

        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserSkinSP_Pr->setPulseWidth(pulseWidth);
        MyLaserSkinMP_Pr->setExposureTime(T_Skin);
    }

    /******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/

    qDebug()<<"Tempo esposizione pelle: " << T_Skin;
    setWidgets();

    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/

    /******************
    * Emetto segnali *
    ******************/
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
}

void DockControls::on_powerErgControl_valueChanged()
{
    /*******************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/

    int exponent=ui->powerErgControl->getExponent();
    qDebug()<< "Esponente di powerErgControl: "<<exponent;
    double mantissa=ui->powerErgControl->getMantissa();

    /*******************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/

    powerErg=mantissa*std::pow(10, exponent);

    /*****************
    * CONTINUOS WAVE *
    * ****************/
    if(n_laser==operation::CONTINUOS_WAVE){

       /*****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/

        MyLaserCW_Pr->setPowerErg(beamCorrection*powerErg);
        MyLaserSkinSP_Pr->setPowerErg(powerErg);
        MyLaserClassCW_Pr->setPowerErg(beamCorrection*powerErg);

        modeLockedPeak();
    }
    else

    /************
    * IMPULSATO *
    * ***********/
    if (n_laser==operation::PULSE){

       /*****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserSP_Pr->setPowerErg(beamCorrection*powerErg);
        MyLaserSkinSP_Pr->setPowerErg(powerErg);

        MyLaserClassSP_Pr->setPowerErg(beamCorrection*powerErg);

        modeLockedPeak();
    }
    else

    /*******************
    * IMPULSI MULTIPLI *
    * ******************/
    if (n_laser==operation::MULTI_PULSE){

        /*****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/

        MyLaserMP_Pr->setPowerErg(beamCorrection*powerErg);
        MyLaserSkinMP_Pr->setPowerErg(powerErg);
        MyLaserClassMP_Pr->setPowerErg(beamCorrection*powerErg);

        modeLockedPeak();

        /********************************************************************
        * Imposto il valore della potenza media nell'oggetto myDLaserGoggle *
        * data dal prodotto dell'energia dell'impulso per la frequenza      *
        *********************************************************************/

        if(myLaserGoggle->getFrequency()!=0)
        {
            myDLaserGoggle->setPowerErg(powerErg*myLaserGoggle->getFrequency());

            /************************************************************
            * Seleziono il campo corrispondente della Tabella B.2 EN207 *
            * Memorizzo il vettore corrispondente in dataVector per     *
            * per l'oggetto myDLaserGoggle                              *
            *************************************************************/
            fetchDLaserOutput();

            /**************************************************************
            * Prelevo i dati rlativi al laser per tracciare il grafico  *
            * relativo ai protettori ottici per l'oggetto myDLaserGoggle *                          *
            **************************************************************/
            fetchDDataVector();
        }
    }

    /*************************************************************
    * Prelevo i dati relativi al laser per tracciare il grafico  *
    * relativo ai protettori ottici per l'oggetto myDLaserGoggle *                          *
    **************************************************************/

    fetchDataVector();
    fetchLaserOutput();

    /**************************************************************
    * La funzione fetchDataVector() va invocata quando cambia    *
    * la frequenza o la durata dell'impulso.                     *                                 *
    * Seleziona il campo corrispondente della Tabella B.2 EN207  *
    * per l'oggetto myLaserGoggle.                               *
    * Memorizza il vettore corrispondente in dataVector ed       *
    * aggiorna il grafico                                        *
    **************************************************************/

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
    set_LEA_Widgets();
    setLaserGoggleWidgets();
    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/
    setPowerErgForEMP();
    setLambertianMax();

    /******************
     * Emetto segnali *
     ******************/
    emit NOHD_Changed();
    emit powerErgChanged();//Cambia la potenza/Energia impulso del laser
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
}

void DockControls::fetchDataVector()
{
    /*************************************************************
    * La funzione fetchDataVector() va invocata quando cambia    *
    * la frequenza o la durata dell'impulso.                     *                                 *
    * Seleziona il campo corrispondente della Tabella B.2 EN207  *
    * per l'oggetto myLaserGoggle.                               *
    * Memorizza il vettore corrispondente in dataVector ed       *
    * aggiorna il grafico                                        *
    **************************************************************/
    myLaserGoggle->setPowerErg(effectivePowerErg);
    //calcolo laserIrrRadCorrected solo per visualizzare
    laserOutput=myLaserGoggle->laserIrrRadCorrected(effectivePowerErg);
    //Calcola e restituisce la marcatura memorizzandola in myNewGoggleMark
    string myNewGoggleMark = myLaserGoggle->goggleMark();
    myModel->setScaleNumber(myLaserGoggle->getScaleNumber());
    dataVector=myLaserGoggle->getDataVector();
    myModel->setTableList(dataVector);

    chartView->setTableSeries(dataVector); //imposta il vettore dei dati
    chartView->buildDataSerie(); //costruisce la serie
    chartView->updateChart(myLaserGoggle->getCodeUnit()); //imposta l'unità di misura sugli assi
    chartView->repaint();
}

void DockControls::updateAllCompositeControlsFunctions()
{
    on_T_SkinControl_valueChanged();
    on_beamDiameterControl_valueChanged();
    on_divergenceControl_valueChanged();
    on_powerErgControl_valueChanged();
    on_prfControl_valueChanged();
    on_pulseControl_valueChanged();
}
void DockControls::fetchDDataVector()
{
    /*************************************************************
    * La funzione fetchDataVector() va invocata quando cambia    *
    * la frequenza o la durata dell'impulso.                     *                                 *
    * Seleziona il campo corrispondente della Tabella B.2 EN207  *
    * per l'oggetto myLaserGoggle.                               *
    * Memorizza il vettore corrispondente in dataVector ed       *
    * aggiorna il grafico                                        *
    **************************************************************/

    dLaserOutput=myDLaserGoggle->laserIrrRadCorrected(powerErg*myLaserGoggle->getFrequency());
    //Calcola e restituisce la marcatura memorizzandola in myNewGoggleMark
    string myNewGoggleMark = myDLaserGoggle->goggleMark(wavelength, LaserGoggle::CONTINUOS_OPERATION, powerErg*prf,
                                                        beamDiameter, LaserGoggle::CONTINUOS_OPERATION);
    myDModel->setScaleNumber(myDLaserGoggle->getScaleNumber());
    frequencyDataVector=myDLaserGoggle->getDataVector();
    myDModel->setTableList(frequencyDataVector);
    dChartView->setTableSeries(frequencyDataVector);
    dChartView->buildDataSerie();
    dChartView->updateChart(myDLaserGoggle->getCodeUnit());
    dChartView->repaint();
}
void DockControls::fetchLaserOutput()
{
    /*************************************************************
    * La funzione fetchLaserOutput() va invocata quando cambia   *
    * l'uscita dal laser (potenza/energia impulso,frequenza,     *
    * diametro del fascio.                                       *
    * Memorizza il vettore corrispondente in dataVector ed       *
    * aggiorna il grafico per myLaserGoggle.                     *
    * IMPORTANTE: Non aggiorna il modello per la visualizzazione *
    * dei numeri scala ma solo il grafico                        *
    **************************************************************/
    myLaserGoggle->setPowerErg(effectivePowerErg);
    //calcolo laserIrrRadCorrected solo per visualizzare
    laserOutput=myLaserGoggle->laserIrrRadCorrected(effectivePowerErg);
    qDebug()<<"Laser Output: "<< laserOutput;
    chartView->setLaserOutput(laserOutput);
    //Calcola e restituisce la marcatura memorizzandola in myNewGoggleMark
    string myNewGoggleMark = myLaserGoggle->goggleMark();
    dataVector=myLaserGoggle->getDataVector();
    chartView->buildDataLaserOutput();
}

void DockControls::fetchDLaserOutput()
{
    /*************************************************************
    * La funzione fetchLaserOutput() va invocata quando cambia   *
    * l'uscita dal laser (potenza/energia impulso,frequenza,     *
    * diametro del fascio.                                       *
    * Memorizza il vettore corrispondente in dataVector ed       *
    * aggiorna il grafico per myLaserGoggle.                     *
    * IMPORTANTE: Non aggiorna il modello per la visualizzazione *
    * dei numeri scala ma solo il grafico                        *
    **************************************************************/
    QString meanPowerString = QString::number(powerErg*myLaserGoggle->getFrequency(), 'e', 2);
    dockGoggle->ui->tMeanPowerLabel->setText("Pm [W]");
    dockGoggle->ui->meanPowerLabel->setText(meanPowerString);
    dLaserOutput= myDLaserGoggle->laserIrrRadCorrected(powerErg*myLaserGoggle->getFrequency());
    dChartView->setLaserOutput(dLaserOutput);
    //Calcola e restituisce la marcatura memorizzandola in myNewGoggleMark
    string myNewGoggleMark = myDLaserGoggle->goggleMark(wavelength, LaserGoggle::CONTINUOS_OPERATION, powerErg*prf,
                                                        beamDiameter, LaserGoggle::CONTINUOS_OPERATION);
    dChartView->buildDataLaserOutput();
}

void DockControls::on_pulseControl_valueChanged()
{
    /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
    int exponent=ui->pulseControl->getExponent();
    qDebug()<< "Esponente di pulseControl: "<<exponent;
    double mantissa=ui->pulseControl->getMantissa();

    /*******************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/
    pulseWidth=mantissa*std::pow(10, exponent);
    if(pulseWidth>=30000)
    {
        pulseWidth=29999;
        ui->pulseControl->setValue(pulseWidth);
    }

	/**************************************************************************************
    * La modalità di funzionamento CONTINUOS WAVE è indipendente dalla durata dell'impuso *
    ***************************************************************************************/
	 
	/************
    * IMPULSATO *
    * ***********/

    if(n_laser==operation::PULSE)
    {

        /*****************************************
         * Imposto il valore negli oggetti Laser *
         *****************************************/

        MyLaserSP_Pr->setPulseWidth(pulseWidth);
        MyLaserSkinSP_Pr->setPulseWidth(pulseWidth);
        myLaserGoggle->setPulseWidth(pulseWidth);
        MyLaserClassSP_Pr->setPulseWidth(pulseWidth);

        modeLockedPeak();

        /*************************************************************
        * La funzione fetchDataVector() va invocata quando cambia    *
        * la frequenza o la durata dell'impulso.                     *
        * Seleziona il campo corrispondente della Tabella B.2 EN207  *
        * per entrambi gli oggetti relativi ai protettori ottici.    *
        * Memorizza il vettore corrispondente in dataVector ed       *
        * aggiorna il grafico                                        *
        **************************************************************/
        fetchDataVector();
        fetchLaserOutput();
    }
	
	/********************
     * IMPULSI MULTIPLI *
     ********************/
	 
    else if(n_laser==operation::MULTI_PULSE)
    {
        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserMP_Pr->setPulseWidth(pulseWidth);
        MyLaserSkinMP_Pr->setPulseWidth(pulseWidth);
        myLaserGoggle->setPulseWidth(pulseWidth);
        MyLaserClassMP_Pr->setPulseWidth(pulseWidth);

        modeLockedPeak();
        QString scaleNumberString = QString::fromStdString(myLaserGoggle->goggleMark());

        /*************************************************************
        * La funzione fetchDataVector() va invocata quando cambia    *
        * la frequenza o la durata dell'impulso.                     *
        * Seleziona il campo corrispondente della Tabella B.2 EN207  *
        * per entrambi gli oggetti relativi ai protettori ottici.    *
        * Memorizza il vettore corrispondente in dataVector ed       *
        * aggiorna il grafico                                        *
        **************************************************************/
        fetchDataVector();
        fetchLaserOutput();
        /*****************************************************
        * Visualizzazione dati relativi ai protettori ottici *
        ******************************************************/
    }
		 
    /******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
    set_LEA_Widgets();
    setLaserGoggleWidgets();
		 
	/*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/
    setLambertianMax();

    /*****************
    * Emetto segnali *
    ******************/
    emit NOHD_Changed();
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
}

void DockControls::setLaserGoggleWidgets()
{
    displayScaleNumber();
    displayTimeBase();
    displayLaserOutput();
    displayNumberOfPulse();
    displayCoefficient_ki();
    displayCoefficient_k();
    display_ni_max();

    if(n_laser==operation::MULTI_PULSE)
    {
        displayDLaserOutput();
        displayDTimeBase();
        displayDScaleNumber();
    }
}

void DockControls::displayLaserOutput()
{
    QString laserOutputString= QString::number(laserOutput,'e',2);
    QString myUnitCodeString = QString::fromStdString(myLaserGoggle->getCodeUnit());
    QString myOutput=QString::fromStdString(myLaserGoggle->outputSort());

    if(n_laser==operation::MULTI_PULSE)
        myOutput=myOutput+"<sub>c</sub>";

    dockGoggle->ui->tLaserOutputLabel->setText(myOutput+" " + myUnitCodeString);
    dockGoggle->ui->laserOutputLabel->setText(laserOutputString);
}

void DockControls::displayTimeBase()
{
    if(n_laser==operation::CONTINUOS_WAVE)
    {
    QString timeBaseString =QString::number(myLaserGoggle->getPulseWidth());
    dockGoggle->ui->tTimeBaseLabel->setText("T<sub>b</sub> [s]");
    dockGoggle->ui->timeBaseLabel->setText(timeBaseString);
    }
    else if((n_laser==operation::PULSE)or(n_laser==operation::MULTI_PULSE))
    {
        if((wavelength>=180)&&(wavelength<=315))
        {
            dockGoggle->ui->tTimeBaseLabel->setText("T<sub>b</sub> [s]");
            dockGoggle->ui->timeBaseLabel->setText(QString::number(LaserGoggle::TIMEBASE_LOW_WAVELENGTH));
        }
                else
        {
            dockGoggle->ui->tTimeBaseLabel->setText("T<sub>b</sub> [s]");
            dockGoggle->ui->timeBaseLabel->setText(QString::number(LaserGoggle::TIMEBASE));
        }
    }
    dockGoggle->ui->kindOfLaserLabel->setText(QString::fromStdString(myLaserGoggle->getLaserPulseOperation()));
}

void DockControls::displayScaleNumber()
{
    computeOpticalDensity();
    if(opticalDensity>0)
    {
        QString scaleNumberString = QString::fromStdString(myLaserGoggle->goggleMark());
        dockGoggle->ui->tScaleNumberLabel->setText("LB");
        dockGoggle->ui->scaleNumberLabel->setText(scaleNumberString);
    }
    else
    {
        dockGoggle->ui->tScaleNumberLabel->setText("----");
        dockGoggle->ui->scaleNumberLabel->setText("non supera l'EMP");
    }
}

void DockControls::displayNumberOfPulse()
{
    QString numberOfPulsesString= QString::number(myLaserGoggle->getNumberOfPulse());
    dockGoggle->ui->tNumberOfPulseLabel->setText("N");
    dockGoggle->ui->numberOfPulseLabel->setText(numberOfPulsesString);
}

void DockControls::displayCoefficient_ki()
{
    QString Coefficient_kiString= QString::number(myLaserGoggle->getCoefficient_ki(),'f', 2);
    dockGoggle->ui->tCoefficient_kiLabel->setText("k<sub>Ti</sub>");
    dockGoggle->ui->coefficient_kiLabel->setText(Coefficient_kiString);
}

void DockControls::displayCoefficient_k()
{
    QString Coefficient_kString= QString::number(myLaserGoggle->getCoefficient_k(), 'f', 2);
    dockGoggle->ui->tCoefficient_kLabel->setText("k");
    dockGoggle->ui->coefficient_kLabel->setText(Coefficient_kString);
}

void DockControls::display_ni_max()
{
    QString ni ="ν";
    QString ni_maxString = QString::number(myLaserGoggle->get_ni_max(), 'e', 2);
    dockGoggle->ui->tn_maxLabel->setText(ni.toHtmlEscaped()+"<sub>max</sub> [Hz]");
    dockGoggle->ui->n_maxLabel->setText(ni_maxString);
}

void DockControls::displayDLaserOutput()
{
    dLaserOutput= myDLaserGoggle->laserIrrRadCorrected(powerErg*myLaserGoggle->getFrequency());
    QString dLaserOutputString= QString::number(dLaserOutput,'e',2);
    QString myDUnitCodeString = QString::fromStdString(myDLaserGoggle->getCodeUnit());
    QString myDOutput=QString::fromStdString(myDLaserGoggle->outputSort());
    dockGoggle->ui->tDLaserOutputLabel->setText(myDOutput+"m " + myDUnitCodeString);
    dockGoggle->ui->dLaserOutputLabel->setText(dLaserOutputString);
}

void DockControls::displayDScaleNumber()
{
    dComputeOpticalDensity();

    QString scaleNumberDString = QString::fromStdString(myDLaserGoggle->goggleMark(wavelength, LaserGoggle::CONTINUOS_OPERATION, powerErg*prf,
                                                                                   beamDiameter, LaserGoggle::CONTINUOS_OPERATION));
    if(dOpticalDensity>0)
    {
        dockGoggle->ui->tScaleNumberDLabel->setText("LB");
        dockGoggle->ui->scaleNumberDLabel->setText(scaleNumberDString);
    }
    else
    {
        dockGoggle->ui->tScaleNumberDLabel->setText("----");
        dockGoggle->ui->scaleNumberDLabel->setText("non supera l'EMP");
    }
}

void DockControls::displayDTimeBase()
{
    if(n_laser==operation::MULTI_PULSE)
    {
        QString dTimeBaseString = QString::number(myDLaserGoggle->getPulseWidth());
        dockGoggle->ui->tDTimeBaseLabel->setText("T<sub>b</sub> [s]");
        dockGoggle->ui->dTimeBaseLabel->setText(dTimeBaseString);
    }
}

void DockControls::on_prfControl_valueChanged()
{
    /*******************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
	
    int exponent=ui->prfControl->getExponent();
    qDebug()<< "Esponente di prfControl: "<<exponent;
    double mantissa=ui->prfControl->getMantissa();

    /*******************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/

    prf=mantissa*std::pow(10.0, exponent);

    if(n_laser==operation::MULTI_PULSE)
    {

        /*******************
        * IMPULSI MULTIPLI *
        * ******************/

        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/

        MyLaserMP_Pr->setPRF(prf);
        MyLaserSkinMP_Pr->setPRF(prf);
        MyLaserClassMP_Pr->setPRF(prf);
        myLaserGoggle->setFrequency(prf);

        /********************************************************************
        * Imposto il valore della potenza media nell'oggetto myDLaserGoggle *
        * data dal prodotto dell'energia dell'impulso per la frequenza      *
        *********************************************************************/

        myDLaserGoggle->setPowerErg(myLaserGoggle->getPowerErg()*prf);

        modeLockedPeak();

        /************************************************************
        * Seleziono il campo corrispondente della Tabella B.2 EN207 *
        * Memorizzo il vettore corrispondente in dataVector per     *
        * per l'oggetto myDLaserGoggle                              *
        *************************************************************/
        fetchDLaserOutput();

        /*************************************************************
        * Prelevo i dati relativi al laser per tracciare il grafico  *
        * relativo ai protettori ottici per l'oggetto myDLaserGoggle *                          *
        **************************************************************/
        fetchDDataVector();

        /*************************************************************
        * Seleziono il campo corrispondente della Tabella B.2 EN207 *
        * Memorizzo il vettore corrispondente in dataVector per     *
        * per l'oggetto myLaserGoggle                              *
        *************************************************************/
        fetchLaserOutput();

        /*************************************************************
        * Prelevo i dati relativi al laser per tracciare il grafico  *
        * relativo ai protettori ottici per l'oggetto myDLaserGoggle *                          *
        **************************************************************/
        fetchDataVector();

       /*******************************************
        * Imposto i widget per la visualizzazione *
        *******************************************/
        setWidgets();
        set_LEA_Widgets();
        setLaserGoggleWidgets();

        /*******************************************************
        * Imposto altri valori necessarie per la parte grafica *
        ********************************************************/
        setLambertianMax();

        /******************
         * Emetto segnali *
         ******************/
        emit EMP_Changed();//Cambia l'EMP
        emit modified();//Per salvataggio file
    }
}


void DockControls::on_beamDiameterControl_valueChanged()
{
    /*******************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
    int exponent=ui->beamDiameterControl->getExponent();
    qDebug()<< "Esponente di beamDiameterControl: "<<exponent;
    double mantissa=ui->beamDiameterControl->getMantissa();

    /*******************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/
    beamDiameter=mantissa*std::pow(10, exponent);

    /*****************
    * CONTINUOS WAVE *
    * ****************/	
    if(n_laser==operation::CONTINUOS_WAVE){

        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserCW_Pr->setBeamDiameter(beamDiameter);
        MyLaserSkinSP_Pr->setBeamDiameter(beamDiameter);
        MyLaserClassCW_Pr->setBeamDiameter(beamDiameter);
        myLaserGoggle->setBeamDiameter(beamDiameter);

        modeLockedPeak();
    }
    else
		
    /************
    * IMPULSATO *
    * ***********/
    if (n_laser==operation::PULSE)
    {
        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserSP_Pr->setBeamDiameter(beamDiameter);
        MyLaserSkinSP_Pr->setBeamDiameter(beamDiameter);
        MyLaserClassSP_Pr->setBeamDiameter(beamDiameter);
        myLaserGoggle->setBeamDiameter(beamDiameter);

        modeLockedPeak();
    }
		
    /*******************
    * IMPULSI MULTIPLI *
    * ******************/
    else if (n_laser==operation::MULTI_PULSE)
    {
       /*****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserMP_Pr->setBeamDiameter(beamDiameter);
        MyLaserSkinMP_Pr->setBeamDiameter(beamDiameter);
        MyLaserClassMP_Pr->setBeamDiameter(beamDiameter);
        myLaserGoggle->setBeamDiameter(beamDiameter);
        myDLaserGoggle->setBeamDiameter(beamDiameter);

        modeLockedPeak();

        /******************************************************************************************
        * Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
        * myDLaserGoggle                                                                          *
        ******************************************************************************************/
        fetchDLaserOutput();
        fetchDDataVector();
    }
	
    /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
    * myLaserGoggle                                                                           *
	******************************************************************************************/
    fetchLaserOutput();
    fetchDataVector();

    /******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
    set_LEA_Widgets();
    setLaserGoggleWidgets();

    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/
    setLambertianMax();

   /******************
    * Emetto segnali *
    ******************/
    emit NOHD_Changed();
    emit beamDiameterChanged();//Cambia il diametro del fascio
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
}

void DockControls::showControls(bool _show)
{
	/****************************************************
	* Visualizza le label relative ai protettori ottici *
	*****************************************************/
    bool details=dockGoggle->ui->showGoggleCharts->isChecked();

    if(details)
        chartView->show();
    else
        chartView->hide();

    dockGoggle->ui->scaleNumberDLabel->setVisible(_show);
    dockGoggle->ui->tScaleNumberDLabel->setVisible(_show);

    dockGoggle->ui->tLaserOutputLabel->setVisible(details);
    dockGoggle->ui->laserOutputLabel->setVisible(details);

    dockGoggle->ui->tTimeBaseLabel->setVisible(details);
    dockGoggle->ui->timeBaseLabel->setVisible(details);

    dockGoggle->ui->coefficient_kLabel->setVisible(_show and details);
    dockGoggle->ui->tCoefficient_kLabel->setVisible(_show and details);
    dockGoggle->ui->coefficient_kiLabel->setVisible(_show and details);
    dockGoggle->ui->tCoefficient_kiLabel->setVisible(_show and details);
    dockGoggle->ui->numberOfPulseLabel->setVisible(_show and details);
    dockGoggle->ui->tNumberOfPulseLabel->setVisible(_show and details);
    dockGoggle->ui->n_maxLabel->setVisible(_show and details);
    dockGoggle->ui->tn_maxLabel->setVisible(_show and details);
    dockGoggle->ui->tDLaserOutputLabel->setVisible(_show and details);
    dockGoggle->ui->dLaserOutputLabel->setVisible(_show and details);
    dockGoggle->ui->tDTimeBaseLabel->setVisible(_show and details);
    dockGoggle->ui->dTimeBaseLabel->setVisible(_show and details);
    dockGoggle->ui->tMeanPowerLabel->setVisible(_show and details);
    dockGoggle->ui->meanPowerLabel->setVisible(_show and details);

    dockGoggle->ui->tabWidget->setTabVisible(1, _show);
    if(_show)
    {
        if(details)
            dChartView->show();

        dockGoggle->ui->tabWidget->setTabText(0,"Criterio dell'impulso");
        dockGoggle->ui->tabWidget->setTabText(1,"Criterio della potenza media");
    }
    else
    {
        if(details)
            dChartView->hide();

        if(n_laser==operation::CONTINUOS_WAVE)
            dockGoggle->ui->tabWidget->setTabText(0,"Criterio della potenza");
        else if(n_laser==operation::PULSE)
            dockGoggle->ui->tabWidget->setTabText(0,"Criterio dell'impulso");
    }
}

void DockControls::on_divergenceControl_valueChanged()
{
	/********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
    int exponent=ui->divergenceControl->getExponent();
    qDebug()<< "Esponente di divergenceControl: "<<exponent;
    double mantissa=ui->divergenceControl->getMantissa();
	
    /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/
    divergence=mantissa*std::pow(10, exponent);
	
    /*****************
    * CONTINUOS WAVE *
    * ****************/	
    if(n_laser==operation::CONTINUOS_WAVE)
    {
    /****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
        MyLaserCW_Pr->setDivergence(divergence);
        MyLaserSkinSP_Pr->setDivergence(divergence);
        MyLaserClassCW_Pr->setDivergence(divergence);
    }
    else
    /************
    * IMPULSATO *
    * ***********/
    if(n_laser==operation::PULSE)
    {
        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserSP_Pr->setDivergence(divergence);
        MyLaserSkinSP_Pr->setDivergence(divergence);
        MyLaserClassSP_Pr->setDivergence(divergence);
    }
    else
    /*******************
    * IMPULSI MULTIPLI *
    * ******************/
    if(n_laser==operation::MULTI_PULSE)
    {
        /****************************************
        * Imposto il valore negli oggetti Laser *
        *****************************************/
        MyLaserMP_Pr->setDivergence(divergence);
        MyLaserSkinMP_Pr->setDivergence(divergence);
        MyLaserClassMP_Pr->setDivergence(divergence);
    }
	
    /******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
    set_LEA_Widgets();
	
    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/	
    setLambertianMax();
	
    /*****************
    * Emetto segnali *
    ******************/
    emit NOHD_Changed();
    emit EMP_Changed();//Cambia l'EMP
    emit divergenceChanged();//Cambia la divergenza
    emit modified();//Per salvataggio file
}

void DockControls::setWidgets()
{
    QString powerErgUnit;
    QString empUnit;
    QString empSkinUnit;
    QString empUnitMP;
    QString thermalEMP;
    QString meanEMP;
    QString minEMP;
    QString empSkinUnitMP;
    QString minEMPSkin;  
    QString skinPowerErgUnit="Q [J]";

    verifyFeasibleRayleigh();

    if(n_laser==operation::CONTINUOS_WAVE)
    {
        ui->powerErgControl->setTitle("P [W]");
        MyLaserCW_Pr->laserUpdate();
        MyLaserSkinSP_Pr->laserSkinUpdate();
        displayScaleNumber();

            if(MyLaserCW_Pr->getFormulaSort()=="E")
            {
                powerErgUnit="P [W]= ";
                empUnit="[W/m<sup>2</sup>]";
            }
            else if(MyLaserCW_Pr->getFormulaSort()=="H")
            {
                powerErgUnit="Q [J]= ";
                empUnit="[J/m<sup>2</sup>]";
            }
        /********************************************************
         * Results widgets                                      *
         ********************************************************/

        //Label non visibili
        dockEffects->ui->deltaLabel->setVisible(false);
        dockEffects->ui->tDeltaLabel->setVisible(false);

        dockResults->ui->tEMP_MP_Label->setVisible(false);
        dockResults->ui->EMP_MP_Label->setVisible(false);

        dockResults->ui->tFormulaMP_Label->setVisible(false);
        dockResults->ui->FormulaMP_Label->setVisible(false);

        dockResults->ui->tMeanPowerLabel->setVisible(false);
        dockResults->ui->MeanPowerLabel->setVisible(false);

        dockResults->ui->tMeanIrradianceLabel->setVisible(false);
        dockResults->ui->MeanIrradianceLabel->setVisible(false);

        dockResults->ui->tThermalEMP_Label->setVisible(false);
        dockResults->ui->ThermalEMP_Label->setVisible(false);

        dockResults->ui->tCountingLabel->setVisible(false);
        dockResults->ui->CountingLabel->setVisible(false);

        dockResults->ui->tCP_Label->setVisible(false);
        dockResults->ui->CP_Label->setVisible(false);

        dockResults->ui->tPulseNumberLabel->setVisible(false);
        dockResults->ui->PulseNumberLabel->setVisible(false);

        dockResults->ui->tPulseNumberThLabel->setVisible(false);
        dockResults->ui->PulseNumberThLabel->setVisible(false);

        dockResults->ui->tExposureTimeLabel->setVisible(true);
        dockResults->ui->ExposureTimeLabel->setVisible(true);

        dockResults->ui->tminEMP_Label->setVisible(false);
        dockResults->ui->minEMP_Label->setVisible(false);

        dockResults->ui->tEMP_mean_Label->setVisible(false);
        dockResults->ui->EMP_mean_Label->setVisible(false);

        dockResults->ui->tEMP_Label->setText(QString::fromStdString(MyLaserCW_Pr->getFormulaSort()) + " " + empUnit);
        dockResults->ui->EMP_Label->setText(QString::number(MyLaserCW_Pr->getEMP(),'e', 2));

        dockResults->ui->tExposureTimeLabel->setText("T<sub>e</sub> [s]");
        dockResults->ui->ExposureTimeLabel->setText(QString::number(MyLaserCW_Pr->getExposureTime(),'e', 2));

        dockResults->ui->tPowerErgLabel->setText("P [W]");
        dockResults->ui->PowerErgLabel->setText(QString::number(MyLaserCW_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tFormulaLabel->setText((QString)"Formula");
        dockResults->ui->FormulaLabel->setText(QString::fromStdString(MyLaserCW_Pr->getFormulaEMP()));

        dockResults->ui->tNOHDLabel->setText((QString)"DNRO [m]");

        bool DNRO_scientNot;
        NOHD=MyLaserCW_Pr->getNOHD();
        emit NOHD_Changed();

        DNRO_scientNot=NOHD>1.0e+03;

        if(DNRO_scientNot)
            dockResults->ui->NOHDLabel->setText(QString::number(NOHD,'e', 2));
        else
            dockResults->ui->NOHDLabel->setText(QString::number(NOHD,'f', 1));

        dockResults->ui->conditions_Label->setText("EMP \nper esposizioni\nda onda continua");
        dockResults->ui->tEMP_1st_Label->setVisible(false);
        dockResults->ui->tEMP_2nd_Label->setVisible(false);
        dockResults->ui->tEMP_3rd_Label->setVisible(false);

        QString EMP_CW_Pulse=QString(" %1 = %2 %3")
                                        .arg(QString::fromStdString(MyLaserCW_Pr->getFormulaEMP()))
                                        .arg(QString::number(MyLaserCW_Pr->getEMP(),'e', 2))
                                        .arg(empUnit);

        dockResults->ui->EMP_1st_Label->setText(EMP_CW_Pulse);
        dockResults->ui->EMP_2nd_Label->setVisible(false);
        dockResults->ui->EMP_3rd_Label->setVisible(false);

        computeOpticalDensity();

        dockResults->ui->tOD_FilterLabel->setVisible(true);
        dockResults->ui->OD_FilterLabel->setVisible(true);
        dockResults->ui->tOD_MeanFilterLabel->setVisible(false);
        dockResults->ui->OD_MeanFilterLabel->setVisible(false);

        dockResults->ui->tOD_FilterLabel->setText("OD Filtro");
        dockResults->ui->OD_FilterLabel->setText(QString::number(opticalDensity, 'e', 2));

        /*******************************************************
        * Effects widgets                                      *
        ********************************************************/

        //Label non visibili
        dockEffects->ui->tTminLabel->setVisible(false);
        dockEffects->ui->TminLabel->setVisible(false);

        dockEffects->ui->tCA_Label->setText("C<sub>A</sub>");
        dockEffects->ui->CA_Label->setText(QString::number(MyLaserCW_Pr->getCA(),'e', 2));

        dockEffects->ui->tCB_Label->setText("C<sub>B</sub>");
        dockEffects->ui->CB_Label->setText(QString::number(MyLaserCW_Pr->getCB(),'e', 2));

        dockEffects->ui->tCC_Label->setText("C<sub>C</sub>");
        dockEffects->ui->CC_Label->setText(QString::number(MyLaserCW_Pr->getCC(),'e', 2));

        dockEffects->ui->tCE_Label->setText("C<sub>E</sub>");
        dockEffects->ui->CE_Label->setText(QString::number(MyLaserCW_Pr->getCE(),'e', 2));

        dockEffects->ui->tT1_Label->setText("T<sub>1</sub> [s]");
        dockEffects->ui->T1_Label->setText(QString::number(MyLaserCW_Pr->getT1(),'e', 2));

        dockEffects->ui->tT2_Label->setText("T<sub>2</sub> [s]");
        dockEffects->ui->T2_Label->setText(QString::number(MyLaserCW_Pr->getT2(),'e', 2));

        QString gamma="γ";
        dockEffects->ui->tGammaLabel->setText(gamma.toHtmlEscaped() + " [mrad]");

        if(MyLaserCW_Pr->getGamma()==0)
            dockEffects->ui->GammaLabel->setText("Non applicabile");
        else
            dockEffects->ui->GammaLabel->setText(QString::number(MyLaserCW_Pr->getGamma(),'e', 2));

        dockEffects->ui->tRadiationLabel->setText("Tipo di radiazione");
        dockEffects->ui->RadiationLabel->setText(QString::fromStdString(MyLaserCW_Pr->getRadiation()));

        dockEffects->ui->tSkinDamageLabel->setText("Danno alla cute");
        dockEffects->ui->SkinDamageLabel->setText(QString::fromStdString(MyLaserCW_Pr->getSkinDamage()));

        dockEffects->ui->tEyeDamageLabel->setText("Danno all'occhio");
        dockEffects->ui->EyeDamageLabel->setText(QString::fromStdString(MyLaserCW_Pr->getEyeDamage()));

        dockEffects->ui->tNoteLabel->setText("Note");
        dockEffects->ui->NoteLabel->setText(QString::fromStdString(MyLaserCW_Pr->getPhotochemicalNote()));

        /******************
        * reset controlli *
        *******************/
        ui->prfControl->setStatusTipHelp("");
        ui->pulseControl->setStatusTipHelp("");

        //Skin widgets
        setSkinWidgetsSingle();
    }
    else if(n_laser==operation::PULSE)
    {
        ui->powerErgControl->setTitle("Q [J]");
        MyLaserSP_Pr->laserUpdate();
        MyLaserSkinSP_Pr->laserSkinUpdate();
        displayScaleNumber();

        if(MyLaserSP_Pr->getFormulaSort()=="E")
        {
            powerErgUnit="P [W]= ";
            empUnit="[W/m<sup>2</sup>]";
        }
        else if(MyLaserSP_Pr->getFormulaSort()=="H")
        {
            powerErgUnit="Q [J]= ";
            empUnit="[J/m<sup>2</sup>]";
        }

       /********************************************************
        * Results widgets                                      *
        ********************************************************/

        //Label non visibili
        dockEffects->ui->deltaLabel->setVisible(false);
        dockEffects->ui->tDeltaLabel->setVisible(false);

        dockResults->ui->tFormulaMP_Label->setVisible(false);
        dockResults->ui->FormulaMP_Label->setVisible(false);

        dockResults->ui->tMeanPowerLabel->setVisible(false);
        dockResults->ui->MeanPowerLabel->setVisible(false);

        dockResults->ui->tMeanIrradianceLabel->setVisible(false);
        dockResults->ui->MeanIrradianceLabel->setVisible(false);

        dockResults->ui->tThermalEMP_Label->setVisible(false);
        dockResults->ui->ThermalEMP_Label->setVisible(false);

        dockResults->ui->tCountingLabel->setVisible(false);
        dockResults->ui->CountingLabel->setVisible(false);

        dockResults->ui->tCP_Label->setVisible(false);
        dockResults->ui->CP_Label->setVisible(false);

        dockResults->ui->tPulseNumberLabel->setVisible(false);
        dockResults->ui->PulseNumberLabel->setVisible(false);

        dockResults->ui->tPulseNumberThLabel->setVisible(false);
        dockResults->ui->PulseNumberThLabel->setVisible(false);

        dockResults->ui->tExposureTimeLabel->setVisible(false);
        dockResults->ui->ExposureTimeLabel->setVisible(false);

        dockResults->ui->tEMP_mean_Label->setVisible(false);
        dockResults->ui->EMP_mean_Label->setVisible(false);

        dockResults->ui->tminEMP_Label->setVisible(false);
        dockResults->ui->minEMP_Label->setVisible(false);

        dockResults->ui->tEMP_MP_Label->setVisible(false);
        dockResults->ui->EMP_MP_Label->setVisible(false);

        dockResults->ui->tEMP_Label->setText(QString::fromStdString(MyLaserSP_Pr->getFormulaSort()) + " " + empUnit);
        dockResults->ui->EMP_Label->setText(QString::number(MyLaserSP_Pr->getEMP(),'e', 2));

        dockResults->ui->tExposureTimeLabel->setText("T<sub>e</sub> [s]");
        dockResults->ui->ExposureTimeLabel->setText(QString::number(MyLaserSP_Pr->getPulseWidth(),'e', 2));

        dockResults->ui->tPowerErgLabel->setText("Q [J]");
        dockResults->ui->PowerErgLabel->setText(QString::number(MyLaserSP_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tFormulaLabel->setText((QString)"Formula");
        dockResults->ui->FormulaLabel->setText(QString::fromStdString(MyLaserSP_Pr->getFormulaEMP()));

        dockResults->ui->tNOHDLabel->setText((QString)"DNRO [m]");

        bool DNRO_scientNot;
        NOHD=MyLaserSP_Pr->getNOHD();
        emit NOHD_Changed();

        DNRO_scientNot=NOHD>1.0e+03;

        if(DNRO_scientNot)
            dockResults->ui->NOHDLabel->setText(QString::number(NOHD,'e', 2));
        else
            dockResults->ui->NOHDLabel->setText(QString::number(NOHD,'f', 1));

        dockResults->ui->conditions_Label->setText("EMP \nper esposizioni\nda impulso singolo");
        dockResults->ui->tEMP_1st_Label->setVisible(false);
        dockResults->ui->tEMP_2nd_Label->setVisible(false);
        dockResults->ui->tEMP_3rd_Label->setVisible(false);

        QString EMP_ONE_Pulse=QString(" %1 = %2 %3")
                                        .arg(QString::fromStdString(MyLaserSP_Pr->getFormulaEMP()))
                                        .arg(QString::number(MyLaserSP_Pr->getEMP(),'e', 2))
                                        .arg(empUnit);

        dockResults->ui->EMP_1st_Label->setText(EMP_ONE_Pulse);
        dockResults->ui->EMP_2nd_Label->setVisible(false);
        dockResults->ui->EMP_3rd_Label->setVisible(false);

        computeOpticalDensity();

        dockResults->ui->tOD_FilterLabel->setVisible(true);
        dockResults->ui->OD_FilterLabel->setVisible(true);
        dockResults->ui->tOD_MeanFilterLabel->setVisible(false);
        dockResults->ui->OD_MeanFilterLabel->setVisible(false);

        dockResults->ui->tOD_FilterLabel->setText("OD Filtro");
        dockResults->ui->OD_FilterLabel->setText(QString::number(opticalDensity, 'e', 2));

        /********************************************************
         * Results effects                                      *
         ********************************************************/

        //Label non visibili
        dockEffects->ui->tTminLabel->setVisible(false);
        dockEffects->ui->TminLabel->setVisible(false);

        dockEffects->ui->CA_Label->setText(QString::number(MyLaserSP_Pr->getCA(),'e', 2));
        dockEffects->ui->CB_Label->setText(QString::number(MyLaserSP_Pr->getCB(),'e', 2));
        dockEffects->ui->CC_Label->setText(QString::number(MyLaserSP_Pr->getCC(),'e', 2));
        dockEffects->ui->CE_Label->setText(QString::number(MyLaserSP_Pr->getCE(),'e', 2));
        dockEffects->ui->T1_Label->setText(QString::number(MyLaserSP_Pr->getT1(),'e', 2));
        dockEffects->ui->T2_Label->setText(QString::number(MyLaserSP_Pr->getT2(),'e', 2));

        if(MyLaserSP_Pr->getGamma()==0)
            dockEffects->ui->GammaLabel->setText("Non applicabile");
        else
            dockEffects->ui->GammaLabel->setText(QString::number(MyLaserSP_Pr->getGamma(),'e', 2));

        dockEffects->ui->RadiationLabel->setText(QString::fromStdString(MyLaserSP_Pr->getRadiation()));
        dockEffects->ui->SkinDamageLabel->setText(QString::fromStdString(MyLaserSP_Pr->getSkinDamage()));
        dockEffects->ui->EyeDamageLabel->setText(QString::fromStdString(MyLaserSP_Pr->getEyeDamage()));
        dockEffects->ui->NoteLabel->setText(QString::fromStdString(MyLaserSP_Pr->getPhotochemicalNote()));

        //Skin widgets
        setSkinWidgetsSingle();


        /******************************************************************************************
         * Imposto il colore del controllo della durata dell'impuso ed il tip che potrebbero      *
         * essere cambiati se precedentemente nella modalità di funzionamento ad impulsi multipli *
         * sono stati immessi valori non fisicamente realizzabili                                 *
         ******************************************************************************************/

        ui->pulseControl->setBackgroundColor("");
        ui->pulseControl->setStatusTipHelp("");
    }
    else if(n_laser==operation::MULTI_PULSE)
    {
        ui->powerErgControl->setTitle("Q [J]");
        MyLaserMP_Pr->laserUpdate();
        MyLaserSkinMP_Pr->laserSkinUpdate();
        displayScaleNumber();
        displayDScaleNumber();

            if(MyLaserMP_Pr->getFormulaSort()=="E")
            {
                powerErgUnit="P [W]= ";
                empUnit="[W/m<sup>2</sup>]";
                minEMP="E<sub>min</sub> [W/m<sup>2</sup>]";
                thermalEMP="E<sub>th</sub> [W/m<sup>2</sup>]";
                meanEMP="E<sub>mean</sub> [W/m<sup>2</sup>]";
            }
            else if(MyLaserMP_Pr->getFormulaSort()=="H")
            {
                powerErgUnit="Q [J]= ";
                empUnit="[J/m<sup>2</sup>]";
                minEMP="H<sub>min</sub> [J/m<sup>2</sup>]";
                thermalEMP="H<sub>th</sub> [J/m<sup>2</sup>]";
                meanEMP="H<sub>mean</sub> [J/m<sup>2</sup>]";
            }


            if(MyLaserMP_Pr->getMeanPowerFormulaSort()=="E")
            {
                empUnitMP="[W/m<sup>2</sup>]";
            }
            else if(MyLaserMP_Pr->getMeanPowerFormulaSort()=="H")
            {
                empUnitMP="[J/m<sup>2</sup>]";
            }

        /********************************************************
        * Results widgets                                      *
        ********************************************************/

        //Label non visibili
        dockEffects->ui->deltaLabel->setVisible(true);
        dockEffects->ui->tDeltaLabel->setVisible(true);

        dockResults->ui->tEMP_MP_Label->setVisible(true);
        dockResults->ui->EMP_MP_Label->setVisible(true);

        dockResults->ui->tFormulaMP_Label->setVisible(true);
        dockResults->ui->FormulaMP_Label->setVisible(true);

        dockResults->ui->tMeanPowerLabel->setVisible(true);
        dockResults->ui->MeanPowerLabel->setVisible(true);

        dockResults->ui->tMeanIrradianceLabel->setVisible(true);
        dockResults->ui->MeanIrradianceLabel->setVisible(true);

        dockResults->ui->tExposureTimeLabel->setVisible(true);
        dockResults->ui->ExposureTimeLabel->setVisible(true);

        dockResults->ui->tEMP_mean_Label->setVisible(true);
        dockResults->ui->EMP_mean_Label->setVisible(true);

        dockResults->ui->tThermalEMP_Label->setVisible(true);
        dockResults->ui->ThermalEMP_Label->setVisible(true);

        dockResults->ui->tCountingLabel->setVisible(true);
        dockResults->ui->CountingLabel->setVisible(true);

        dockResults->ui->tCP_Label->setVisible(true);
        dockResults->ui->CP_Label->setVisible(true);

        dockResults->ui->tPulseNumberLabel->setVisible(true);
        dockResults->ui->PulseNumberLabel->setVisible(true);

        dockResults->ui->tPulseNumberThLabel->setVisible(true);
        dockResults->ui->PulseNumberThLabel->setVisible(true);

        dockResults->ui->tminEMP_Label->setVisible(true);
        dockResults->ui->minEMP_Label->setVisible(true);

        dockResults->ui->conditionsFrame->setVisible(true);

        double deltaPercent=pulseWidth*prf*100;
        QString deltaString;

        if(deltaPercent>1)
            deltaString=QString::number(deltaPercent, 'f', 2);
        else
            deltaString=QString::number(deltaPercent, 'e', 2);

        dockEffects->ui->deltaLabel->setText(QString("%1%").arg(deltaString));

        dockSkin->ui->tPowerErgSkinLabel->setText(skinPowerErgUnit);
        dockSkin->ui->powerErgSkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tEMP_MP_Label->setText(QString::fromStdString(MyLaserMP_Pr->getMeanPowerFormulaSort()) + "<sub>Te</sub> " + empUnitMP);
        dockResults->ui->EMP_MP_Label->setText(QString::number(MyLaserMP_Pr->getEMP_MP(),'e', 2));

        dockResults->ui->tEMP_Label->setText(QString::fromStdString(MyLaserMP_Pr->getFormulaSort()) + " " + empUnit);
        dockResults->ui->EMP_Label->setText(QString::number(MyLaserMP_Pr->getEMP(),'e', 2));

        dockResults->ui->tExposureTimeLabel->setText("T<sub>e</sub> [s]");
        dockResults->ui->ExposureTimeLabel->setText(QString::number(MyLaserMP_Pr->getExposureTime(),'e', 2));

        dockResults->ui->tEMP_mean_Label->setText(meanEMP);
        dockResults->ui->EMP_mean_Label->setText(QString::number(MyLaserMP_Pr->getMeanPower_EMP_Equate(),'e', 2));

        dockResults->ui->tPowerErgLabel->setText("Q [J]");
        dockResults->ui->PowerErgLabel->setText(QString::number(MyLaserMP_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tFormulaLabel->setText("Formula");
        dockResults->ui->FormulaLabel->setText(QString::fromStdString(MyLaserMP_Pr->getFormulaEMP()));

        dockResults->ui->tFormulaMP_Label->setText("Formula T<sub>e</sub>");
        dockResults->ui->FormulaMP_Label->setText(QString::fromStdString(MyLaserMP_Pr->getMeanPowerFormulaEMP()));

        dockResults->ui->tMeanPowerLabel->setText("P<sub>m</sub> [W]");
        dockResults->ui->MeanPowerLabel->setText(QString::number(MyLaserMP_Pr->getMeanPower(),'e', 2));

        dockResults->ui->tMeanIrradianceLabel->setText("E<sub>m</sub> [W/m<sup>2</sup>]");
        dockResults->ui->MeanIrradianceLabel->setText(QString::number(MyLaserMP_Pr->getMeanIrradiance(),'e', 2));

        dockResults->ui->tThermalEMP_Label->setText(thermalEMP);

        if((wavelength>=400)and(wavelength<=1.0e+06))
            dockResults->ui->ThermalEMP_Label->setText(QString::number(MyLaserMP_Pr->getThermalEMP(),'e', 2));
        else
            dockResults->ui->ThermalEMP_Label->setText("Non applicabile");

        dockResults->ui->tCountingLabel->setText("Conteggio");
        dockResults->ui->CountingLabel->setText(QString::fromStdString(MyLaserMP_Pr->valutateCounting()));

        dockResults->ui->tCP_Label->setText("C<sub>p</sub>");

        if((wavelength>=400)and(wavelength<=1.0e+06))
            dockResults->ui->CP_Label->setText(QString::number(MyLaserMP_Pr->getCPCoefficient(),'e', 2));
        else
            dockResults->ui->CP_Label->setText("Non applicabile");

        dockResults->ui->tPulseNumberThLabel->setText("N<sub>th</sub>");
        dockResults->ui->PulseNumberThLabel->setText(QString::number(MyLaserMP_Pr->getPulseNumber()));

        dockResults->ui->tPulseNumberLabel->setText("N");

        double prf=round(MyLaserMP_Pr->getPRF());
        double te=MyLaserMP_Pr->getExposureTime();
        int pulseNumber=ceil(prf*te);
        qDebug()<<"prf: "<<prf;
        qDebug()<<"Tempo di esposizione: "<<te;
        qDebug()<<"Numero di impulsi: "<<pulseNumber;
        dockResults->ui->PulseNumberLabel->setText(QString::number(pulseNumber));

        dockResults->ui->tminEMP_Label->setText(minEMP);
        dockResults->ui->minEMP_Label->setText(QString::number(MyLaserMP_Pr->returnMultiPulse_EMP(),'e', 2));

        dockResults->ui->tNOHDLabel->setText("DNRO [m]");

        bool DNRO_scientNot;
        NOHD=MyLaserMP_Pr->getNOHD();
        emit NOHD_Changed();

        DNRO_scientNot=NOHD>1.0e+03;

        if(DNRO_scientNot)
            dockResults->ui->NOHDLabel->setText(QString::number(NOHD,'e', 2));
        else
            dockResults->ui->NOHDLabel->setText(QString::number(NOHD,'f', 1));

        dockResults->ui->conditions_Label->setText("EMP \nper esposizioni\nripetute");

        dockResults->ui->tEMP_1st_Label->setVisible(true);
        dockResults->ui->tEMP_2nd_Label->setVisible(true);
        dockResults->ui->tEMP_3rd_Label->setVisible(true);

        dockResults->ui->tEMP_1st_Label->setText("1<sup>a</sup>");      
        dockResults->ui->tEMP_2nd_Label->setText("2<sup>a</sup>");
        dockResults->ui->tEMP_3rd_Label->setText("3<sup>a</sup>");

        QString firstCondition=QString(" %1 = %2 %3")
                                        .arg(QString::fromStdString(MyLaserMP_Pr->getFormulaEMP()))
                                        .arg(QString::number(MyLaserMP_Pr->getEMP_1stCondition(),'e', 2))
                                        .arg(empUnit);

        dockResults->ui->EMP_1st_Label->setVisible(true);
        dockResults->ui->EMP_1st_Label->setText(firstCondition);

        QString formulaEMP;

        if(MyLaserMP_Pr->getMeanPowerFormulaSort()=="H")
            formulaEMP=QString::fromStdString(MyLaserMP_Pr->getMeanPowerFormulaEMP()+"/N");
        else
            formulaEMP=QString::fromStdString(MyLaserMP_Pr->getMeanPowerFormulaEMP());

        QString secondCondition=QString(" %1 = %2 %3")
                                        .arg(formulaEMP)
                                        .arg(QString::number(MyLaserMP_Pr->getEMP_2ndCondition(),'e', 2))
                                        .arg(empUnitMP);

        dockResults->ui->EMP_2nd_Label->setVisible(true);
        dockResults->ui->EMP_2nd_Label->setText(secondCondition);

        QString thirdCondition;

        if((wavelength>=400)and(wavelength<=1.0e+06))
        {
            QString  empUnitTmin;

            if(MyLaserMP_Pr->getFormulaSort_3rdCondition()=="E")
                empUnitTmin="[W/m<sup>2</sup>]";
            else if(MyLaserMP_Pr->getFormulaSort_3rdCondition()=="H")
                empUnitTmin="[J/m<sup>2</sup>]";

        thirdCondition=QString(" %1 C<sub>p</sub> = %2  %3")
                                .arg(QString::fromStdString(MyLaserMP_Pr->getFormula_3rdCondition()))
                                .arg(QString::number(MyLaserMP_Pr->getEMP_3rdCondition(),'e', 2))
                                .arg(empUnitTmin);
        }
        else
            thirdCondition="Non applicabile";

        dockResults->ui->EMP_3rd_Label->setText(thirdCondition);
        dockResults->ui->EMP_1st_Label->setVisible(true);
        dockResults->ui->EMP_2nd_Label->setVisible(true);
        dockResults->ui->EMP_3rd_Label->setVisible(true);

        computeOpticalDensity();
        dComputeOpticalDensity();

        dockResults->ui->tOD_FilterLabel->setVisible(true);
        dockResults->ui->OD_FilterLabel->setVisible(true);
        dockResults->ui->tOD_MeanFilterLabel->setVisible(true);
        dockResults->ui->OD_MeanFilterLabel->setVisible(true);

        dockResults->ui->tOD_FilterLabel->setText("OD Filtro");
        dockResults->ui->OD_FilterLabel->setText(QString::number(opticalDensity, 'e', 2));
        dockResults->ui->tOD_MeanFilterLabel->setText("OD Filtro medio");
        dockResults->ui->OD_MeanFilterLabel->setText(QString::number(dOpticalDensity, 'e', 2));

        /********************************************************
        * Results effects                                       *
        *********************************************************/

        dockEffects->ui->tTminLabel->setVisible(true);
        dockEffects->ui->TminLabel->setVisible(true);

        dockEffects->ui->CA_Label->setText(QString::number(MyLaserMP_Pr->getCA(),'e', 2));
        dockEffects->ui->CB_Label->setText(QString::number(MyLaserMP_Pr->getCB(),'e', 2));
        dockEffects->ui->CC_Label->setText(QString::number(MyLaserMP_Pr->getCC(),'e', 2));
        dockEffects->ui->CE_Label->setText(QString::number(MyLaserMP_Pr->getCE(),'e', 2));
        dockEffects->ui->T1_Label->setText(QString::number(MyLaserMP_Pr->getT1(),'e', 2));
        dockEffects->ui->T2_Label->setText(QString::number(MyLaserMP_Pr->getT2(),'e', 2));


        dockEffects->ui->tTminLabel->setText("T<sub>min</sub> [s]");
        dockEffects->ui->TminLabel->setText(QString::number(MyLaserMP_Pr->getTmin(),'e', 2));

        dockEffects->ui->RadiationLabel->setText(QString::fromStdString(MyLaserMP_Pr->getRadiation_MP()));
        dockEffects->ui->SkinDamageLabel->setText(QString::fromStdString(MyLaserMP_Pr->getSkinDamage_MP()));
        dockEffects->ui->EyeDamageLabel->setText(QString::fromStdString(MyLaserMP_Pr->getEyeDamage_MP()));

        if(MyLaserMP_Pr->getGamma_MP()==0)
            dockEffects->ui->GammaLabel->setText("Non applicabile");
        else
            dockEffects->ui->GammaLabel->setText(QString::number(MyLaserMP_Pr->getGamma_MP(),'e', 2));

        dockEffects->ui->NoteLabel->setText(QString::fromStdString(MyLaserMP_Pr->getPhotochemicalNote_MP()));

        verifyFeasibleDutyCycle();
        /********************************************************
        * Widget pelle                                          *
        *********************************************************/

        if(MyLaserSkinMP_Pr->getFormulaSort()=="E")
        {
            empSkinUnit="[W/m<sup>2</sup>]";
            minEMPSkin="E<sub>min</sub> [W/m<sup>2</sup>]";
        }
        else if(MyLaserSkinMP_Pr->getFormulaSort()=="H")
        {
            empSkinUnit="[J/m<sup>2</sup>]";
            minEMPSkin="H<sub>min</sub> [W/m<sup>2</sup>]";
        }

        if(MyLaserSkinMP_Pr->getMeanPowerFormulaSort()=="E")
            empSkinUnitMP="[W/m<sup>2</sup>]";
        else if(MyLaserMP_Pr->getMeanPowerFormulaSort()=="H")
            empSkinUnitMP="[J/m<sup>2</sup>]";

        dockSkin->ui->tFormulaSkinLabel->setText("Formula");
        dockSkin->ui->FormulaSkinLabel->setText(QString::fromStdString(MyLaserSkinMP_Pr->getFormulaEMP()));

        dockSkin->ui->tEMP_SkinLabel->setText(QString::fromStdString(MyLaserSkinMP_Pr->getFormulaSort()) + empSkinUnit);
        dockSkin->ui->EMP_SkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getEMP(),'e', 2));

        dockSkin->ui->tFormulaSkinMP_Label->setVisible(true);
        dockSkin->ui->FormulaSkinMP_Label->setVisible(true);

        dockSkin->ui->tEMP_MP_SkinLabel->setVisible(true);
        dockSkin->ui->EMP_MP_SkinLabel->setVisible(true);

        dockSkin->ui->tMeanPowerSkinLabel->setVisible(true);
        dockSkin->ui->MeanPowerSkinLabel->setVisible(true);

        dockSkin->ui->tMeanIrradianceSkinLabel->setVisible(true);
        dockSkin->ui->MeanIrradianceSkinLabel->setVisible(true);

        dockSkin->ui->tPulseNumberSkinLabel->setVisible(true);
        dockSkin->ui->PulseNumberSkinLabel->setVisible(true);

        dockSkin->ui->tminEMP_SkinLabel->setVisible(true);
        dockSkin->ui->minEMP_SkinLabel->setVisible(true);

        dockSkin->ui->tEMP_MP_SkinLabel->setText(QString::fromStdString(MyLaserSkinMP_Pr->getMeanPowerFormulaSort()) + "<sub>Te</sub> " + empSkinUnitMP);
        dockSkin->ui->EMP_MP_SkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getEMP_MP(),'e', 2));

        dockSkin->ui->tFormulaSkinMP_Label->setText("Formula T<sub>e</sub>");
        dockSkin->ui->FormulaSkinMP_Label->setText(QString::fromStdString(MyLaserSkinMP_Pr->getMeanPowerFormulaEMP()));

        dockSkin->ui->tMeanPowerSkinLabel->setText("P<sub>m</sub> [W]");
        dockSkin->ui->MeanPowerSkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getMeanPower(),'e', 2));

        dockSkin->ui->tMeanIrradianceSkinLabel->setText("E<sub>m</sub> [W/m<sup>2</sup>]");
        dockSkin->ui->MeanIrradianceSkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getMeanIrradiance(),'e', 2));

        dockSkin->ui->tPulseNumberSkinLabel->setText("N<sub>cute</sub>");
        dockSkin->ui->PulseNumberSkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getPulseNumber(),'e', 2));

        dockSkin->ui->tminEMP_SkinLabel->setText(minEMPSkin);
        dockSkin->ui->minEMP_SkinLabel->setText(QString::number(MyLaserSkinMP_Pr->returnMultiPulse_EMP(),'e', 2));

        dockSkin->ui->tNSHDLabel->setText("DNRP [m]");

        dockSkin->ui->conditions_Label->setText("EMP \nper esposizioni\nripetute");
        dockSkin->ui->tEMP_1st_Label->setVisible(true);
        dockSkin->ui->tEMP_2nd_Label->setVisible(true);

        dockSkin->ui->tEMP_1st_Label->setText("1<sup>a</sup>");
        dockSkin->ui->tEMP_2nd_Label->setText("2<sup>a</sup>");

        QString EMP_CWSP_Pulse=QString(" %1 = %2 %3")
                                            .arg(QString::fromStdString(MyLaserSkinMP_Pr->getFormulaEMP()))
                                            .arg(QString::number(MyLaserSkinMP_Pr->getEMP_1stCondition(),'e', 2))
                                            .arg(empSkinUnit);

        QString formulaSkinEMP;

        if(MyLaserSkinMP_Pr->getMeanPowerFormulaSort()=="H")
            formulaSkinEMP=QString::fromStdString(MyLaserSkinMP_Pr->getMeanPowerFormulaEMP()+"/N");
        else
            formulaSkinEMP=QString::fromStdString(MyLaserSkinMP_Pr->getMeanPowerFormulaEMP());

        QString EMP_MP_Pulse=QString(" %1 = %2 %3")
                                            .arg(formulaSkinEMP)
                                            .arg(QString::number(MyLaserSkinMP_Pr->getEMP_2ndCondition(),'e', 2))
                                            .arg(empSkinUnitMP);

        dockSkin->ui->EMP_1st_Label->setText(EMP_CWSP_Pulse);
        dockSkin->ui->EMP_2nd_Label->setText(EMP_MP_Pulse);

        dockSkin->ui->EMP_1st_Label->setVisible(true);
        dockSkin->ui->EMP_2nd_Label->setVisible(true);

        NSHD=MyLaserSkinMP_Pr->getNSHD();
        emit NSHD_Changed();

        bool DNRC_scientNot=NSHD>1.0e+03;
        if(DNRC_scientNot)
            dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinMP_Pr->getNSHD(),'e', 2));
        else
            dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinMP_Pr->getNSHD(),'f', 1));
    }
}

DockControls::operation DockControls::get_n_laser()const
{
    return n_laser;
}

void DockControls::on_operationCombo_currentIndexChanged(int index)
{
    undoStack->clear();
    resetHistory();
	/******************************************
	* L'indice scelto della casella combinata *
	*******************************************/
    n_laser=static_cast<operation>(index);
    QString whatThis;

   /******************
    * CONTINUOS WAVE *
    * ****************/	
    if(n_laser==operation::CONTINUOS_WAVE)
    {
		
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserCW_Pr=MyLaserSafetyMP;
    MyLaserSkinSP_Pr=MyLaserSkinSafetyMP;
    MyLaserClassCW_Pr=MyLaserClassMP;

	/*****************************************************************************
	* Ogni volta che passoa alla modalità di funzionamento CONTINUOS WAVE        *
	* Imposto il valore della potenza a 1.0e+00, il valore della frequenza a 0,  *
 	* il tempo di esposizione della cute a 10                                    *
	******************************************************************************/
   
    prf=LaserGoggle::CONTINUOS_OPERATION;//frequenza
    ui->prfControl->setValue(prf);
    on_prfControl_valueChanged();
    ui->prfControl->setEnabled(false);
    //Non è possibile impostare il valore della prf essendo non definita per l'oggetto MyLaserCW_Pr
    myLaserGoggle->setFrequency(LaserGoggle::CONTINUOS_OPERATION);

    powerErg=LaserSafetyCW::POWER;
    /***********************************************************************************
    * Range di variazione della potenza nella modalità di funzionamento CONTINUOS WAVE *
    ************************************************************************************/

    ui->powerErgControl->setValue(powerErg);
    ui->powerErgControl->setMinimumExponent(-4);
    ui->powerErgControl->setMaximumExponent(6);

    on_powerErgControl_valueChanged();

    T_Skin=LaserSkinSafety::EXPOSURE_TIME;
    //tempo esposizione pelle
    ui->T_SkinControl->setValue(T_Skin);
    on_T_SkinControl_valueChanged();
    ui->T_SkinControl->setEnabled(true);

    //abilito il checkbox per l'impotazione manuale del tempo di esposizione
    ui->enableTeCheckBox->setEnabled(true);

    /****************************************************************************************
    * se l'impostazione del tempo di esposizione è abilitata aggiorno il laser con          *
    * il valore corrisposndente altrimenti aggiorno con il tempo di esposizione predefinito *
    *****************************************************************************************/

    ui->pulseControl->setEnabled(false);

    MyLaserCW_Pr->setExposureTimeEditable(isTeEdtitingEnabled());

    if(isTeEdtitingEnabled()){
        MyLaserCW_Pr->setEditedExposureTime(exposureTimeControl);
        ui->teControl->setEnabled(true);}
    else{
        MyLaserCW_Pr->setExposureTime();
        ui->teControl->setEnabled(false);
        }

    MyLaserCW_Pr->setPulseWidth(MyLaserCW_Pr->getExposureTime());

    if(((wavelength>315) && (wavelength<=1e+06)))
    {
         myLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE);
    }
    else if(((myLaserGoggle->getWavelength()>=180) && (myLaserGoggle->getWavelength()<=315)))
    {
        myLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE_LOW_WAVELENGTH);
    }

    enableTeEditing=ui->teControl->isEnabled();

    MyLaserClassCW_Pr->setTimeBase();

   /*******************************************************************************************
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
    * myLaserGoggle                                                                           *
	******************************************************************************************/

    setDataForCW_Operation();
    setCW_SP_LEAModel();

    modeLockedPeak();
    fetchDataVector();
    fetchLaserOutput();

	//Nascondo i controlli riguardanti i protettori ottici non previsti
    showControls(false);
    whatThis="Continuos Wave";
    }
    else
    if (n_laser==operation::PULSE)
    {

   /************
    * IMPULSATO *
    * ***********/
	
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSP_Pr=MyLaserSafetyMP;
    MyLaserSkinSP_Pr=MyLaserSkinSafetyMP;
    MyLaserClassSP_Pr=MyLaserClassMP;
	
   /******************************************************************
	* Ogni volta che passo alla modalità di funzionamento IMPULSATO  *
	* imposto il valore dell'energia dell'impuslo a 1.0e+03, il      *
	* valore della frequenza a 0, il tempo di esposizione degli      *
	* occhi a 1.0e-06, il tempo di esposizione della cute a 1.0e-06. *
	******************************************************************/
    prf=LaserGoggle::CONTINUOS_OPERATION;
    myLaserGoggle->setFrequency(prf);

    ui->prfControl->setValue(prf);
    on_prfControl_valueChanged();
    ui->prfControl->setEnabled(false);

    qDebug()<< "Larghezza dell'impulso: " << pulseWidth;

    pulseWidth=1.0e-06;
    ui->pulseControl->setValue(pulseWidth);
    on_pulseControl_valueChanged();

    ui->pulseControl->setEnabled(true);

    powerErg=1.0e-03;

    /****************************************************************
     * Range di variazione dell'energia dell'impulso nella modalità *
     * di funzionamento IMPULSATO                                   *
     ****************************************************************/
    ui->powerErgControl->setMinimumExponent(-12);
    ui->powerErgControl->setMaximumExponent(1);
    ui->powerErgControl->setValue(powerErg);

    on_powerErgControl_valueChanged();
    ui->powerErgControl->setEnabled(true);

    T_Skin=1.0e-06;
    MyLaserSkinSP_Pr->setPulseWidth(T_Skin);
    on_T_SkinControl_valueChanged();
    ui->T_SkinControl->setEnabled(false);

    //disabilito il controllo per l'impotazione manuale del tempo di esposizione e
    //il checkbox associato
    ui->enableTeCheckBox->setEnabled(false);
    ui->teControl->setEnabled(false);

   /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
	* myLaserGoggle                                                                          *
	******************************************************************************************/

    setDataForSP_Operation();
    setCW_SP_LEAModel();

    modeLockedPeak();
    fetchDataVector();
    fetchLaserOutput();

	//Nascondo i controlli riguardanti i protettori ottici non previsti
    showControls(false);
    whatThis="Impulsato";
    }
    else
   /********************
    * IMPULSI MULTIPLI *
    * ******************/	
    if (n_laser==operation::MULTI_PULSE)
    {

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/

    MyLaserMP_Pr=MyLaserSafetyMP;
    MyLaserSkinMP_Pr=MyLaserSkinSafetyMP;
    MyLaserClassMP_Pr=MyLaserClassMP;

    MyLaserMP_Pr->setWavelength(wavelength);
    MyLaserSkinMP_Pr->setWavelength(wavelength);
    MyLaserClassMP_Pr->setWavelength(wavelength);

    // list is safe to use - the exception did not affect it.

   /******************************************************************
    * Ogni volta che passo alla modalità di funzionamento AD IMPULSI *
    * multipli imposto il valore dell'energia dell'impusl a 1.0e+03, il valore della     *
    * frequenza a 10.0, il tempo di esposizione degli occhi a 1.0e-06,  *
    * il tempo di esposizione della cute a 10. 				 *
    ******************************************************************/

    /****************************************************************
     * Range di variazione dell'energia dell'impulso nella modalità *
     * di funzionamento IMPULSI MULTIPLI.                            *
     ****************************************************************/

    ui->powerErgControl->setMinimumExponent(-12);
    ui->powerErgControl->setMaximumExponent(1);
    powerErg=1.0e-03;
    ui->powerErgControl->setValue(powerErg);
    on_powerErgControl_valueChanged();

    //ogni volta che passo al funzionamento ad impulsi multipli imposto la durata degli impulsi a 1.0e-06 s
    pulseWidth=1.0e-06;

    //abilito il checkbox per l'impotazione manuale del tempo di esposizione

    ui->enableTeCheckBox->setEnabled(true);

    //Imposto il tempo di esposizione alla radiazione laser.
    /****************************************************************************************
    * se l'impostazione del tempo di esposizione è abilitata aggiorno il laser con          *
    * il valore corrisposndente altrimenti aggiorno con il tempo di esposizione predefinito *
    *****************************************************************************************/

    if(isTeEdtitingEnabled())
    {
        MyLaserMP_Pr->setEditedExposureTime(exposureTimeControl);
        ui->teControl->setEnabled(true);
    }
    else
        MyLaserMP_Pr->setExposureTime();

    qDebug()<< "Tempo di esposizione: "<< MyLaserMP_Pr->getExposureTime();
    MyLaserMP_Pr->computeTmin();
    MyLaserClassMP_Pr->setTimeBase();

    ui->pulseControl->setValue(pulseWidth);
    on_pulseControl_valueChanged();
    ui->pulseControl->setEnabled(true);


    //ogni volta cha passo al funzionamento ad impulsi multipli imposto la frequenza a 10 Hz
    prf=LaserSafetyMP::PULSE_REPETITION_FREQUENCY;
    ui->prfControl->setEnabled(true);
    ui->prfControl->setValue(prf);
    on_prfControl_valueChanged();

    MyLaserMP_Pr->setPRF(prf);
    MyLaserSkinMP_Pr->setPRF(prf);
    MyLaserClassMP_Pr->setPRF(prf);
    myLaserGoggle->setFrequency(prf);
	
    T_Skin=LaserSkinSafety::EXPOSURE_TIME;
    MyLaserSkinMP_Pr->setExposureTime(T_Skin);
    on_T_SkinControl_valueChanged();
    ui->T_SkinControl->setValue(T_Skin);
    ui->T_SkinControl->setEnabled(true);

     /***********************************************************************
      * Imposto il valore negli oggetti occhiali protettori.                *
      * Il valore della base dei tempi dipende dalla lunghezza d'onda e va  *
      * pertanto impostato in funzione di questa (EN 207 B.4).              *
      * Per l'oggetto myLaserGoggle si procede impostando il valore della   *
      * durata dell'impulso.                                                *
      * Per l'oggetto myDLaserGoggle il valore della base dei tempi dipende *
      * dalla lunghezza d'onda. Imposto in modo esplicito la base dei tempi *
      **********************************************************************/   

    myDLaserGoggle->setWavelength(wavelength);
    myDLaserGoggle->setPowerErg(powerErg*myLaserGoggle->getFrequency());

    if(((myDLaserGoggle->getWavelength()>315) && (myDLaserGoggle->getWavelength()<=1e+06)))
      {
         myDLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE);
      }
      else
    if(((myDLaserGoggle->getWavelength()>=180) && (myDLaserGoggle->getWavelength()<=315)))
      {
         myDLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE);
      }

   /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
	* myLaserGoggle                                                                          *
	******************************************************************************************/
    setDataForMP_Operation();
    setMP_LEAModel();
    selectMP_Thermal_Model();

    modeLockedPeak();
    fetchLaserOutput();
    fetchDataVector();

    fetchDLaserOutput();
    fetchDDataVector();

    //Mostro i tutti controlli riguardanti i protettori
    showControls(true);
    whatThis="Impulsi multipli";
    }

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
    set_LEA_Widgets();
    setLaserGoggleWidgets();

   /**************************************************************************
    * Imposto altri valori necessari per la parte grafica, emissione segnali *
    **************************************************************************/
    setPowerErgForEMP();
    setLambertianMax();
	
   /*****************************
    * Emetto i segnali restanti *
    *****************************/
    emit NOHD_Changed();
    emit EMP_Changed();//Cambia il diametro del fascio
    emit operationChanged();//cambia la modalità di funzionamento
    emit modified();//Per salvataggio file
    emit statusBarSignal(whatThis);
}

void DockControls::setSkinWidgetsSingle()
{
//Skin widgets

    QString empSkinUnit;
    QString skinPowerErgUnit;

    if(MyLaserSkinSP_Pr->getFormulaSort()=="E")
    {
            empSkinUnit="[W/m<sup>2</sup>]";
    }
    else
        if(MyLaserSkinSP_Pr->getFormulaSort()=="H")
        {
            empSkinUnit="[J/m<sup>2</sup>]";
        }

    if(n_laser==operation::CONTINUOS_WAVE)
                skinPowerErgUnit="P [W]";
    else
        if(n_laser==operation::PULSE)
                skinPowerErgUnit="Q [J]";

    dockSkin->ui->tFormulaSkinMP_Label->setVisible(false);
    dockSkin->ui->FormulaSkinMP_Label->setVisible(false);

    dockSkin->ui->tMeanPowerSkinLabel->setVisible(false);
    dockSkin->ui->MeanPowerSkinLabel->setVisible(false);

    dockSkin->ui->tMeanIrradianceSkinLabel->setVisible(false);
    dockSkin->ui->MeanIrradianceSkinLabel->setVisible(false);

    dockSkin->ui->tEMP_MP_SkinLabel->setVisible(false);
    dockSkin->ui->EMP_MP_SkinLabel->setVisible(false);

    dockSkin->ui->tPulseNumberSkinLabel->setVisible(false);
    dockSkin->ui->PulseNumberSkinLabel->setVisible(false);

    dockSkin->ui->tminEMP_SkinLabel->setVisible(false);
    dockSkin->ui->minEMP_SkinLabel->setVisible(false);

    dockSkin->ui->tPowerErgSkinLabel->setText(skinPowerErgUnit);
    dockSkin->ui->powerErgSkinLabel->setText(QString::number(MyLaserSkinSP_Pr->getPowerErg(),'e', 2));

    dockSkin->ui->tEMP_SkinLabel->setText(QString::fromStdString(MyLaserSkinSP_Pr->getFormulaSort()) + empSkinUnit);
    dockSkin->ui->EMP_SkinLabel->setText(QString::number(MyLaserSkinSP_Pr->getEMP(),'e', 2));

    dockSkin->ui->tFormulaSkinLabel->setText("Formula");
    dockSkin->ui->FormulaSkinLabel->setText(QString::fromStdString(MyLaserSkinSP_Pr->getFormulaEMP()));

    dockSkin->ui->tNSHDLabel->setText((QString)"DNRP [m]");

    bool DNRC_scientNot;


    if(n_laser==operation::CONTINUOS_WAVE)
    {
        NSHD=MyLaserSkinSP_Pr->getPowerNSHD();
        emit NSHD_Changed();

        DNRC_scientNot=NSHD>1.0e+03;
        if(DNRC_scientNot)
            dockSkin->ui->NSHDLabel->setText(QString::number(NSHD,'e', 2));
            else
            dockSkin->ui->NSHDLabel->setText(QString::number(NSHD,'f', 1));

         dockSkin->ui->conditions_Label->setText("EMP \nper esposizioni\nda onda continua");

    }
        else
    if(n_laser==operation::PULSE)
    {
        NSHD=MyLaserSkinSP_Pr->getErgNSHD();
        emit NSHD_Changed();

        DNRC_scientNot=NSHD>1.0e+03;
        if(DNRC_scientNot)
            dockSkin->ui->NSHDLabel->setText(QString::number(NSHD,'e', 2));
            else
            dockSkin->ui->NSHDLabel->setText(QString::number(NSHD,'f', 1));

         dockSkin->ui->conditions_Label->setText("EMP \nper esposizioni\nda impulso singolo");
    }

    dockSkin->ui->tEMP_1st_Label->setVisible(false);
    dockSkin->ui->tEMP_2nd_Label->setVisible(false);

    QString EMP_CW_Pulse=QString(" %1 = %2 %3")
                                    .arg(QString::fromStdString(MyLaserSkinSP_Pr->getFormulaEMP()))
                                    .arg(QString::number(MyLaserSkinSP_Pr->getEMP(),'e', 2))
                                    .arg(empSkinUnit);

    dockSkin->ui->EMP_1st_Label->setText(EMP_CW_Pulse);

    dockSkin->ui->EMP_1st_Label->setVisible(true);
    dockSkin->ui->EMP_2nd_Label->setVisible(false);
}

/************************************************************************************************
 * Parametri necessari per il calcolo dell'energia diffusa estesa di un riflettore lambertiano. *
 * I segnali vengono impiegati in queste funzioni e non nelle slot dei controllj per evitare di *
 * dover considerare anche le variazioni della modalità di funzionamento del laser.             *
 * **********************************************************************************************/

void DockControls::setPowerErgForEMP()
{
    double _powerErgForEMP;

    if(n_laser==operation::CONTINUOS_WAVE)
        _powerErgForEMP=MyLaserCW_Pr->getPowerErgForEMP();
    else if(n_laser==operation::PULSE)
        _powerErgForEMP=MyLaserSP_Pr->getPowerErgForEMP();
    else
        _powerErgForEMP=MyLaserMP_Pr->getPowerErgForEMP();

    if(_powerErgForEMP==powerErgForEMP)
        return;

    powerErgForEMP=_powerErgForEMP;
    emit powerErgForEMPChanged();
}

void DockControls::setEMP_Sort()
{
    if(n_laser==operation::CONTINUOS_WAVE)
        EMP_Sort=MyLaserCW_Pr->getFormulaSort();
    else if(n_laser==operation::PULSE)
        EMP_Sort=MyLaserSP_Pr->getFormulaSort();
    else if(n_laser==operation::MULTI_PULSE)
        EMP_Sort=MyLaserMP_Pr->getFormulaSort();
    else
        EMP_Sort="";
}

double DockControls::getPowerErgForEMP()const
{
    return powerErgForEMP;
}

std::string DockControls::getEMP_Sort()const
{
    return EMP_Sort;
}

void DockControls::setEMP()
{
    double _myEMP;

    if(n_laser==operation::CONTINUOS_WAVE)
        _myEMP=MyLaserCW_Pr->getEMP();
        else
    if(n_laser==operation::PULSE)
        _myEMP=MyLaserSP_Pr->getEMP();
    else
        if((getExposureTime()>10)&&(getExposureTime()<3.0e+04))
            _myEMP=MyLaserMP_Pr->EMP_ForLongRetinalExposure();
        else
            _myEMP=MyLaserMP_Pr->returnMultiPulse_EMP();

    if(_myEMP==myEMP)
    return;

    myEMP=_myEMP;
    qDebug()<<"myEMP: "<<myEMP;
}

double DockControls::getPowerErg()const
{
    return powerErg;
}

double DockControls::getExposureTime()const
{
    return exposureTimeControl;
}

double DockControls::getWavelength()const
{
    return wavelength;
}

double DockControls::getEMP()const
{
    return myEMP;
}


double DockControls::getBeamDiameter()const
{
    return beamDiameter;
}

double DockControls::getPulseWidth()const
{
    return pulseWidth;
}

double DockControls::getPRF()const
{
    return prf;
}

/***********************************************************************************************
 * Salvattagio degli argomenti delle slot dei controlli necessari                              *
 * *********************************************************************************************/


void DockControls::setLambertianMax()
{
    double _lambertianMax;

    if(n_laser==operation::CONTINUOS_WAVE)
    {
        _lambertianMax=MyLaserCW_Pr->getLambertianMax();
    }
    else if(n_laser==operation::PULSE)
    {
        _lambertianMax=MyLaserSP_Pr->getLambertianMax();
    }
    else
    {
        _lambertianMax=MyLaserMP_Pr->getLambertianMax();
    }

    if(_lambertianMax==lambertianMax)
        return;

    lambertianMax=_lambertianMax;
    qDebug()<<"Massimo valore della riflessione su superficie lambertiana: "<<lambertianMax;
    emit lambertianMaxChanged();
}

double DockControls::getOpticalDistance() const
{
    return NOHD;
}

double DockControls::getSkinDistances() const
{
    return NSHD;
}

double DockControls::getDivergence()const
{
    return divergence;
}

double DockControls::getLambertianMax()const
{
    return lambertianMax;
}

MyChartView* DockControls::getChartView()const
{
    return chartView;
}

MyChartView* DockControls::getdChartView()const
{
    return dChartView;
}

void DockControls::setDialControls()
{
    ui->powerErgControl->setTitle(tr("P [W]"));
    ui->powerErgControl->setMinimumExponent(-4);
    ui->powerErgControl->setMaximumExponent(6);
    ui->powerErgControl->setValue(1.00e+00);

    ui->pulseControl->setTitle(tr("t [s]"));
    ui->pulseControl->setMinimumExponent(-11);
    ui->pulseControl->setMaximumExponent(4);
    ui->pulseControl->setValue(2.50e-01);

    QString phi="φ";
    ui->divergenceControl->setTitle(phi.toHtmlEscaped() + " [mrad]");
    ui->divergenceControl->setMinimumExponent(-3);
    ui->divergenceControl->setMaximumExponent(2);
    ui->divergenceControl->setValue(1.50e+00);

    ui->beamDiameterControl->setTitle(tr("a [mm]"));
    ui->beamDiameterControl->setMinimumExponent(-3);
    ui->beamDiameterControl->setMaximumExponent(2);
    ui->beamDiameterControl->setValue(7.00e+00);

    ui->prfControl->setTitle(tr("PRF [Hz]"));
    ui->prfControl->setMinimumExponent(0);
    ui->prfControl->setMaximumExponent(10);
    ui->prfControl->setValue(1.00e+01);

    ui->T_SkinControl->setTitle(tr("t<sub>cute</sub> [s]"));
    ui->T_SkinControl->setMinimumExponent(-4);
    ui->T_SkinControl->setMaximumExponent(3);
    ui->T_SkinControl->setValue(5.0e+00);

    ui->teControl->setTitle(tr("T<sub>e</sub> [s]"));
    ui->teControl->setDialNumber(10);
    ui->teControl->setEnabled(false);

    ui->wavelengthScrollBar->setMinimum(400);
    ui->wavelengthScrollBar->setMaximum(700);
    ui->wavelengthScrollBar->setValue(633);

    ui->enableTeCheckBox->setEnabled(true);
    ui->enableTeCheckBox->setChecked(false);

    ui->checkGaussianBeam->setChecked(true);

    ui->internalWaist_checkBox->setChecked(false);
}

void DockControls::setUV()
{
    ui->wavelengthScrollBar->setMinimum(181);
    ui->wavelengthScrollBar->setMaximum(399);
    ui->wavelengthScrollBar->setValue(380);
}

void DockControls::setVIS()
{
    ui->wavelengthScrollBar->setMinimum(400);
    ui->wavelengthScrollBar->setMaximum(700);
    ui->wavelengthScrollBar->setValue(633);
}

void DockControls::setIRA_NIR()
{
    ui->wavelengthScrollBar->setMinimum(701);
    ui->wavelengthScrollBar->setMaximum(1400);
    ui->wavelengthScrollBar->setValue(1064);
}

void DockControls::setIRB_SWIR()
{
    ui->wavelengthScrollBar->setMinimum(1401);
    ui->wavelengthScrollBar->setMaximum(3000);
    ui->wavelengthScrollBar->setValue(1583);
}

void DockControls::setIRC_MWIR()
{
    ui->wavelengthScrollBar->setMinimum(3001);
    ui->wavelengthScrollBar->setMaximum(8000);
    ui->wavelengthScrollBar->setValue(3391);
}

void DockControls::setIRC_LWIR()
{
    ui->wavelengthScrollBar->setMinimum(8001);
    ui->wavelengthScrollBar->setMaximum(15000);
    ui->wavelengthScrollBar->setValue(10600);
}

void DockControls::setIRC_FIR()
{
    ui->wavelengthScrollBar->setMinimum(15001);
    ui->wavelengthScrollBar->setMaximum(999999);
    ui->wavelengthScrollBar->setValue(571699);
}

void DockControls::on_enableTeCheckBox_toggled(bool checked)
{
    QString whatThis;
    undoStack->clear();
    resetHistory();

    enableTeEditing=checked;
    ui->teControl->setEnabled(checked);

    if(checked)
    {
        on_teControl_valueChanged();
        whatThis="Regolazione tempo esposizione oculare attivata";
    }
    else
    {
        /*****************
        * CONTINUOS WAVE *
        ******************/
        whatThis="Esposizione oculare al tempo base";
        if(n_laser==operation::CONTINUOS_WAVE)
        {
            /********************************************************
            * Imposto il valore del tempo di esposizione impostato *
            ********************************************************/
            MyLaserCW_Pr->setExposureTimeEditable(false);
            MyLaserCW_Pr->setExposureTime();
            exposureTimeControl=MyLaserCW_Pr->getExposureTime();
            MyLaserCW_Pr->setPulseWidth(exposureTimeControl);
        }
        else if(n_laser==operation::MULTI_PULSE)
        /********************
         * IMPULSI multipli *
         ********************/
         {
            /********************************************************
            * Imposto il valore del tempo di esposizione impostato *
            ********************************************************/
            MyLaserMP_Pr->setExposureTimeEditable(false);
            MyLaserMP_Pr->setExposureTime();
            exposureTimeControl=MyLaserMP_Pr->getExposureTime();
            MyLaserMP_Pr->setPulseWidth(pulseWidth);
        }
    }

    setWidgets();
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
    emit statusBarSignal(whatThis);
}

bool DockControls::isTeEdtitingEnabled()
{
    return enableTeEditing;
}

void DockControls::enableTeEdtiting(bool enabled)
{
    ui->teControl->setEnabled(enabled);
}


void DockControls::enablePulseControl(bool enabled)
{
    ui->pulseControl->setEnabled(enabled);
}


void DockControls::computeOpticalDensity()
{
    double CW_EMP;
    double SP_EMP;
    double powerErg;
    double firstAndThirdEMP;
    double opticalDensityRatio=0.0;
    double beamArea;
    double timeBase;
    QString formulaSort;

    if(n_laser==operation::CONTINUOS_WAVE)
    {
        double irradiance;
        CW_EMP=MyLaserCW_Pr->getEMP();
        formulaSort=QString::fromStdString(MyLaserCW_Pr->getFormulaSort());
        MyLaserCW_Pr->computeBeamArea();
        timeBase=MyLaserCW_Pr->getExposureTime();
        beamArea=MyLaserCW_Pr->getBeamArea();
        powerErg=MyLaserCW_Pr->getPowerErg();
        irradiance=powerErg/beamArea;

        if(formulaSort=='E')
            opticalDensityRatio=irradiance/CW_EMP;
        else{
         if(formulaSort=='H')
             opticalDensityRatio=irradiance*timeBase/CW_EMP;
    }

    opticalDensity=std::log10(opticalDensityRatio);
}
    else if(n_laser==operation::PULSE)
    {
        double energyExposure;

        SP_EMP= MyLaserSP_Pr->getEMP();
        powerErg=MyLaserSP_Pr->getPowerErg();
        formulaSort=QString::fromStdString(MyLaserSP_Pr->getFormulaSort());
        double pulseWidth=MyLaserSP_Pr->getPulseWidth();
        MyLaserSP_Pr->computeBeamArea();
        beamArea=MyLaserSP_Pr->getBeamArea();
        energyExposure=powerErg/beamArea;

            if(formulaSort=='E'){
                opticalDensityRatio=energyExposure/(SP_EMP*pulseWidth);
    }
    else
    {
        if(formulaSort=='H')
            opticalDensityRatio=energyExposure/SP_EMP;
    }
    opticalDensity=std::log10(opticalDensityRatio);
    }

    else if(n_laser==operation::MULTI_PULSE)
    {
        double exposure;
        double timeForPulse;

        MyLaserMP_Pr->computeEMP_ForOD();
        firstAndThirdEMP=MyLaserMP_Pr->getEMP_ForOD();
        formulaSort=QString::fromStdString(MyLaserMP_Pr->getFormulaSort());
        qDebug() << "Primo e terzo emp: " << firstAndThirdEMP;

            if(MyLaserMP_Pr->getPRF()>1/MyLaserMP_Pr->getTmin())
            {
                if(firstAndThirdEMP==MyLaserMP_Pr->getThermalEMP())
                    timeForPulse=MyLaserMP_Pr->getTmin();
                else
                    timeForPulse=MyLaserMP_Pr->getPulseWidth();
            }
            else
            timeForPulse=MyLaserMP_Pr->getPulseWidth();

        powerErg=MyLaserMP_Pr->getPowerErg();

        MyLaserMP_Pr->computeBeamArea();
        beamArea=MyLaserMP_Pr->getBeamArea();

        /****************************************************************************
        * L'EMP da considerare nel caso di laser MODELOCKED è riferito             *
        * alla potenza di picco pertanto nel caso in cui la lunghezza d'onda sia   *
        * > 1400 e < 400 nm e che t< 10^9 l'uscita del laser va calcolata come Q/t *
        ****************************************************************************/

            exposure=powerErg/beamArea;
            if(formulaSort=="E")
                opticalDensityRatio=exposure/(firstAndThirdEMP*timeForPulse);
            else if(formulaSort=="H")
                opticalDensityRatio=exposure/firstAndThirdEMP;
            qDebug()<<"opticalDensityRatio: "<<opticalDensityRatio;

        opticalDensity=std::log10(opticalDensityRatio);
    }
}

void DockControls::dComputeOpticalDensity()
{
    if(n_laser==operation::MULTI_PULSE)
    {
        double secondEMP=0.0;
        double powerErg;
        double meanPower;
        double beamArea;
        double PRF;
        double opticalDensityRatio;
        double radiance;
        double irradiance;
        QString formulaSort;
        int pulseNumber;

        MyLaserMP_Pr->computeBeamArea();
        beamArea=MyLaserMP_Pr->getBeamArea();

        powerErg=MyLaserMP_Pr->getPowerErg();
        formulaSort=QString::fromStdString(MyLaserMP_Pr->getMeanPowerFormulaSort());
        PRF=MyLaserMP_Pr->getPRF();
        pulseNumber=ceil(PRF*MyLaserMP_Pr->getExposureTime());
        meanPower=powerErg*PRF;
        irradiance=meanPower/beamArea;
        radiance=powerErg/beamArea;
        secondEMP=MyLaserMP_Pr->getEMP_MP();

        if(formulaSort=='E')
            opticalDensityRatio=irradiance/secondEMP;
        else
            opticalDensityRatio=radiance*double(pulseNumber)/secondEMP;

        dOpticalDensity=std::log10(opticalDensityRatio);
    }
    else
        dOpticalDensity=0;
}


double DockControls::getOpticalDensity()
{
    return opticalDensity;
}

double DockControls::getDOpticalDensity()
{
    return dOpticalDensity;
}

bool DockControls::isModeLocked()
{
    bool modeLocking=false;

    if(n_laser!=operation::CONTINUOS_WAVE)
    {
        if((wavelength<400)or(wavelength>1400))
        {
            if(pulseWidth<1.0e-09)
                modeLocking=true;
        }
    }

    return modeLocking;
}


void DockControls::modeLockedPeak()
{
    if(isModeLocked())
        effectivePowerErg=powerErg/pulseWidth;
    else
        effectivePowerErg=powerErg;
}

QVector<QString> DockControls::getLEA_DataVector()const
{
    return LEA_Data;
}

void DockControls::on_checkGaussianBeam_clicked(bool checked)
{
    QString whatThis;
    undoStack->clear();
    resetHistory();

    gaussianBeam=checked;
    if(checked)
    {
        beamCorrection=1.0;
        whatThis="Fascio gaussiano";
    }
    else
    {
        beamCorrection=2.5;
        whatThis="Fascio non gaussiano";
    }

    on_powerErgControl_valueChanged();
    emit statusBarSignal(whatThis);
}

void DockControls::set_LEA_Widgets()
{
    if(n_laser==operation::CONTINUOS_WAVE)
    {
        MyLaserClassCW_Pr->updateAll();
        setDataForCW_Operation();
        updateCW_SP_LEA_data();
    }
    else if(n_laser==operation::PULSE)
    {
        MyLaserClassSP_Pr->updateAll();
        setDataForSP_Operation();
        updateCW_SP_LEA_data();
    }
    else if(n_laser==operation::MULTI_PULSE)
    {
        MyLaserClassMP_Pr->updateAll();
        setDataForMP_Operation();
        updateMP_LEA_data();

        if((wavelength>=400)and(wavelength<=1400))
        {
            QString header;
            if(prf<1/MyLaserClassMP_Pr->getTi())
            {
                setDataForMP_ThermalOperation();
                header="Criterio dell'impulso per effetti termici";
            }
            else
            {
                setDataForMP_ThermalTiOperation();
                header="Criterio dell'impulso per effetti termici in alta frequenza";
            }
            updateMP_Thermal_LEA_data(header);
        }
    }
}

void  DockControls::selectMP_Thermal_Model()
{
    if((wavelength>=400)and(wavelength<=1400))
    {
        if(prf<1/MyLaserClassMP_Pr->getTi())
        {
            setDataForMP_ThermalOperation();
            setMP_Thermal_LEAModel();
        }
        else
        {
            setDataForMP_ThermalTiOperation();
            setMP_ThermalTi_LEAModel();
        }
    }
}

QString DockControls::getLaserClassString(const LaserClassCW::laserClass & myLaserClass)
{
    QString laserClassString;
    if(myLaserClass==LaserClassCW::laserClass::CLASSE_1)
        laserClassString="Classe 1";
    else if(myLaserClass==LaserClassCW::laserClass::CLASSE_1M)
        laserClassString="Classe 1M";
    else if(myLaserClass==LaserClassCW::laserClass::CLASSE_2)
        laserClassString="Classe 2";
    else if(myLaserClass==LaserClassCW::laserClass::CLASSE_2M)
        laserClassString="Classe 2M";
    else if(myLaserClass==LaserClassCW::laserClass::CLASSE_3R)
        laserClassString="Classe 3R";
    else if(myLaserClass==LaserClassCW::laserClass::CLASSE_3B)
        laserClassString="Classe 3B";
    else if(myLaserClass==LaserClassCW::laserClass::CLASSE_4)
        laserClassString="Classe 4";
    else
        laserClassString="Non classificato";

    return laserClassString;
}

void DockControls::setDataForCW_Operation()
{
    LEA_Data.clear();

    QString diameterCond1;
    QString distanceCond1;
    QString beamAtStopCond1;
    QString couplingFactor1;

    if(isnan(MyLaserClassCW_Pr->getCouplingFactor_Cond_1()))
        couplingFactor1="N.A.";
    else
        couplingFactor1=QString::number(MyLaserClassCW_Pr->getCouplingFactor_Cond_1(), 'e', 2);

    QString couplingFactor3=QString::number(MyLaserClassCW_Pr->getCouplingFactor_Cond_3(), 'e', 2);

    if(isnan(MyLaserClassCW_Pr->getApCond_1()))
        diameterCond1="N.A.";
    else
        diameterCond1=QString::number(MyLaserClassCW_Pr->getApCond_1(), 'e', 2)+" mm";

    QString diameterCond3=QString::number(MyLaserClassCW_Pr->getApCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassCW_Pr->getDistCond_1()))
        distanceCond1="N.A.";
    else
        distanceCond1=QString::number(MyLaserClassCW_Pr->getDistCond_1(), 'e', 2)+" mm";

    QString distanceCond3=QString::number(MyLaserClassCW_Pr->getDistCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassCW_Pr->getBeamAtStop_Cond_1()))
        beamAtStopCond1="N.A.";
    else
        beamAtStopCond1=QString::number(MyLaserClassCW_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm";

    QString beamAtStopCond3=QString::number(MyLaserClassCW_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm";

    ComputeLEA::ClassData myClassData;

    LaserClassCW::laserClass myLaserClass=MyLaserClassCW_Pr->getLaserClass();
    QString FormulaLEA;
    QString FormulaLEA_Tipo;
    QString FormulaLEA_Unit;
    QString LEA_Value;
    QString PowerErgCond_1;
    QString PowerErgCond_3;
    QString FormulaLEA_Label;
    QString LEA_Value_Label;
    QString tLEA_Label;
    QString PowerErgCond_1_Label;
    QString PowerErgCond_3_Label;

    if ((myLaserClass==LaserClassCW::laserClass::CLASSE_1)or(myLaserClass==LaserClassCW::laserClass::CLASSE_1M))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_1_1M;
        tLEA_Label="LEA 1 - 1M";
    }
    else if ((myLaserClass==LaserClassCW::laserClass::CLASSE_2)or(myLaserClass==LaserClassCW::laserClass::CLASSE_2M))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_2_2M;
        tLEA_Label="LEA 2 - 2M";
    }
    else if (myLaserClass==LaserClassCW::laserClass::CLASSE_3R)
    {
        myClassData=ComputeLEA::ClassData::CLASSE_3R;
        tLEA_Label="LEA 3R";
    }
    else if ((myLaserClass==LaserClassCW::laserClass::CLASSE_3B)or(myLaserClass==LaserClassCW::laserClass::CLASSE_4))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_3B;
        tLEA_Label="LEA 3B";
    }
    else
    {
        myClassData=ComputeLEA::ClassData::SENZA_CLASSIFICA;
        tLEA_Label="NC";
    }
    FormulaLEA_Tipo=QString::fromStdString(MyLaserClassCW_Pr->getLEA_FormulaTipo()[static_cast<int>(myClassData)]);
    FormulaLEA=QString::fromStdString(MyLaserClassCW_Pr->getLEA_Formula()[static_cast<int>(myClassData)]);
    FormulaLEA_Unit=QString::fromStdString(MyLaserClassCW_Pr->getLEA_FormulaUnit()[static_cast<int>(myClassData)]);
    LEA_Value=QString::number(MyLaserClassCW_Pr->getLEA()[static_cast<int>(myClassData)], 'e', 2);
    FormulaLEA_Label=QString("%1=%2 %3")
                             .arg(FormulaLEA_Tipo)
                             .arg(FormulaLEA)
                             .arg(FormulaLEA_Unit);

    LEA_Value_Label=QString("%1 %2")
                             .arg(LEA_Value)
                             .arg(FormulaLEA_Unit);

    PowerErgCond_1=QString::number(MyLaserClassCW_Pr->getPowerErg_Cond_1()[static_cast<int>(myClassData)], 'e', 2);
    PowerErgCond_3=QString::number(MyLaserClassCW_Pr->getPowerErg_Cond_3()[static_cast<int>(myClassData)], 'e', 2);

    QString tPowerErgCond_1_Label=QString("%1<sub>Acc 1</sub>").arg(FormulaLEA_Tipo);
    QString tPowerErgCond_3_Label=QString("%1<sub>Acc 3</sub>").arg(FormulaLEA_Tipo);


    if(isnan(MyLaserClassCW_Pr->getPowerErg_Cond_1()[static_cast<int>(myClassData)]))
        PowerErgCond_1_Label="N.A.";
    else
        PowerErgCond_1_Label=QString("%1 %2")
                                .arg(PowerErgCond_1)
                                .arg(FormulaLEA_Unit);

    PowerErgCond_3_Label=QString("%1 %2")
                             .arg(PowerErgCond_3)
                             .arg(FormulaLEA_Unit);

    QString TimeBase_Label=QString("%1 s")
                         .arg(QString::number(MyLaserClassCW_Pr->getTimeBase()));

    QString classLabel=getLaserClassString(myLaserClass);

    if(myLaserClass==LaserClassCW::laserClass::CLASSE_1)
         ui->warning_Label->setPixmap(QPixmap(":/images/ok_circle.png"));
    else
         ui->warning_Label->setPixmap(QPixmap(":/images/laser_warning.png"));

    ui->class_Label->setText(getLaserClassString(myLaserClass));

    LEA_Data.push_back(TimeBase_Label);         //0
    LEA_Data.push_back(tLEA_Label);             //1
    LEA_Data.push_back(FormulaLEA_Label);       //2
    LEA_Data.push_back(LEA_Value_Label);        //3
    LEA_Data.push_back(tPowerErgCond_1_Label);  //4
    LEA_Data.push_back(PowerErgCond_1_Label);   //5
    LEA_Data.push_back(tPowerErgCond_3_Label);  //6
    LEA_Data.push_back(PowerErgCond_3_Label);   //7

    LEA_Data.push_back(couplingFactor1);        //8
    LEA_Data.push_back(couplingFactor3);        //9
    LEA_Data.push_back(diameterCond1);          //10
    LEA_Data.push_back(diameterCond3);          //11
    LEA_Data.push_back(distanceCond1);          //12
    LEA_Data.push_back(distanceCond3);          //13
    LEA_Data.push_back(beamAtStopCond1);        //14
    LEA_Data.push_back(beamAtStopCond3);        //15
    LEA_Data.push_back(classLabel);             //16
}

void DockControls::setDataForSP_Operation()
{
    LEA_Data.clear();
    QString diameterCond1;
    QString distanceCond1;
    QString beamAtStopCond1;
    QString couplingFactor1;

    if(isnan(MyLaserClassSP_Pr->getCouplingFactor_Cond_1()))
        couplingFactor1="N.A.";
    else
        couplingFactor1=QString::number(MyLaserClassSP_Pr->getCouplingFactor_Cond_1(), 'e', 2);

    QString couplingFactor3=QString::number(MyLaserClassSP_Pr->getCouplingFactor_Cond_3(), 'e', 2);

    if(isnan(MyLaserClassSP_Pr->getApCond_1()))
        diameterCond1="N.A.";
    else
        diameterCond1=QString::number(MyLaserClassSP_Pr->getApCond_1(), 'e', 2)+" mm";

    QString diameterCond3=QString::number(MyLaserClassSP_Pr->getApCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassSP_Pr->getDistCond_1()))
        distanceCond1="N.A.";
    else
        distanceCond1=QString::number(MyLaserClassSP_Pr->getDistCond_1(), 'e', 2)+" mm";

    QString distanceCond3=QString::number(MyLaserClassSP_Pr->getDistCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassSP_Pr->getBeamAtStop_Cond_1()))
        beamAtStopCond1="N.A.";
    else
        beamAtStopCond1=QString::number(MyLaserClassSP_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm";

    QString beamAtStopCond3=QString::number(MyLaserClassSP_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm";

    ComputeLEA::ClassData myClassData;

    LaserClassCW::laserClass myLaserClass=MyLaserClassSP_Pr->getLaserClass();
    QString FormulaLEA;
    QString FormulaLEA_Tipo;
    QString FormulaLEA_Unit;
    QString LEA_Value;
    QString PowerErgCond_1;
    QString PowerErgCond_3;
    QString FormulaLEA_Label;
    QString LEA_Value_Label;
    QString tLEA_Label;
    QString PowerErgCond_1_Label;
    QString PowerErgCond_3_Label;

    if ((myLaserClass==LaserClassSP::laserClass::CLASSE_1)or(myLaserClass==LaserClassSP::laserClass::CLASSE_1M))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_1_1M;
        tLEA_Label="LEA 1 - 1M";
    }
    else if ((myLaserClass==LaserClassSP::laserClass::CLASSE_2)or(myLaserClass==LaserClassSP::laserClass::CLASSE_2M))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_2_2M;
        tLEA_Label="LEA 2 - 2M";
    }
    else if (myLaserClass==LaserClassSP::laserClass::CLASSE_3R)
    {
        myClassData=ComputeLEA::ClassData::CLASSE_3R;
        tLEA_Label="LEA 3R";
    }
    else if ((myLaserClass==LaserClassSP::laserClass::CLASSE_3B)or(myLaserClass==LaserClassSP::laserClass::CLASSE_4))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_3B;
        tLEA_Label="LEA 3B";
    }
    else
    {
        myClassData=ComputeLEA::ClassData::SENZA_CLASSIFICA;
        tLEA_Label="NC";
    }
    FormulaLEA_Tipo=QString::fromStdString(MyLaserClassSP_Pr->getLEA_FormulaTipo()[static_cast<int>(myClassData)]);
    FormulaLEA=QString::fromStdString(MyLaserClassSP_Pr->getLEA_Formula()[static_cast<int>(myClassData)]);
    FormulaLEA_Unit=QString::fromStdString(MyLaserClassSP_Pr->getLEA_FormulaUnit()[static_cast<int>(myClassData)]);
    LEA_Value=QString::number(MyLaserClassSP_Pr->getLEA()[static_cast<int>(myClassData)], 'e', 2);
    FormulaLEA_Label=QString("%1=%2 %3")
                             .arg(FormulaLEA_Tipo)
                             .arg(FormulaLEA)
                             .arg(FormulaLEA_Unit);

    LEA_Value_Label=QString("%1 %2")
                             .arg(LEA_Value)
                             .arg(FormulaLEA_Unit);

    PowerErgCond_1=QString::number(MyLaserClassSP_Pr->getPowerErg_Cond_1()[static_cast<int>(myClassData)], 'e', 2);
    PowerErgCond_3=QString::number(MyLaserClassSP_Pr->getPowerErg_Cond_3()[static_cast<int>(myClassData)], 'e', 2);

    QString tPowerErgCond_1_Label=QString("%1<sub>Acc 1</sub>").arg(FormulaLEA_Tipo);
    QString tPowerErgCond_3_Label=QString("%1<sub>Acc 3</sub>").arg(FormulaLEA_Tipo);

    if(isnan(MyLaserClassSP_Pr->getPowerErg_Cond_1()[static_cast<int>(myClassData)]))
        PowerErgCond_1_Label="N.A.";
    else
        PowerErgCond_1_Label=QString("%1 %2")
                                .arg(PowerErgCond_1)
                                .arg(FormulaLEA_Unit);

    PowerErgCond_3_Label=QString("%1 %2")
                             .arg(PowerErgCond_3)
                             .arg(FormulaLEA_Unit);

    QString TimeBase_Label=QString("no time base");

    QString classLabel=getLaserClassString(myLaserClass);

    if(myLaserClass==LaserClassCW::laserClass::CLASSE_1)
         ui->warning_Label->setPixmap(QPixmap(":/images/ok_circle.png"));
    else
         ui->warning_Label->setPixmap(QPixmap(":/images/laser_warning.png"));

    ui->class_Label->setText(getLaserClassString(myLaserClass));

    LEA_Data.push_back(TimeBase_Label);         //0
    LEA_Data.push_back(tLEA_Label);             //1
    LEA_Data.push_back(FormulaLEA_Label);       //2
    LEA_Data.push_back(LEA_Value_Label);        //3
    LEA_Data.push_back(tPowerErgCond_1_Label);	//4
    LEA_Data.push_back(PowerErgCond_1_Label);	//5
    LEA_Data.push_back(tPowerErgCond_3_Label);	//6
    LEA_Data.push_back(PowerErgCond_3_Label);	//7

    LEA_Data.push_back(couplingFactor1);        //8
    LEA_Data.push_back(couplingFactor3);        //9
    LEA_Data.push_back(diameterCond1);          //10
    LEA_Data.push_back(diameterCond3);          //11
    LEA_Data.push_back(distanceCond1);          //12
    LEA_Data.push_back(distanceCond3);          //13
    LEA_Data.push_back(beamAtStopCond1);        //14
    LEA_Data.push_back(beamAtStopCond3);        //15
    LEA_Data.push_back(classLabel);             //16
}

void DockControls::setCW_SP_LEAModel()
{
    leaModel=new QStandardItemModel();    

    QStandardItem* criteriumGroup;
    QStandardItem* timeBaseChild;
    QStandardItem* timeBaseValueChild;

    if(n_laser==operation::CONTINUOS_WAVE)
    {
        criteriumGroup=new QStandardItem("Criterio della potenza");
        timeBaseChild=new QStandardItem("Base dei tempi");
        timeBaseValueChild=new QStandardItem(QString("T<sub>b</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TIMEBASE))));
        timeBaseChild->appendRow(timeBaseValueChild);
        criteriumGroup->appendRow(timeBaseChild);
    }
    else
        criteriumGroup=new QStandardItem("Criterio dell'impulso");

    QStandardItem* LEA_Child=new QStandardItem(QString("%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TLEA))));
    QStandardItem* LEA_formulaChild=new QStandardItem(QString("formula: %1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::FORMULALEA))));
    QStandardItem* LEA_valueChild=new QStandardItem(QString("valore: %1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::LEA_VALUE))));

    LEA_Child->appendRow(LEA_formulaChild);
    LEA_Child->appendRow(LEA_valueChild);

    criteriumGroup->appendRow(LEA_Child);

    QStandardItem* powerErgCondChild=new QStandardItem("Livello accessibile");

    QStandardItem* powerErgCondChild_1=new QStandardItem(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TPOWERERGCOND_1)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::POWERERGCOND_1))));

    QStandardItem* powerErgCondChild_3=new QStandardItem(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TPOWERERGCOND_3)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::POWERERGCOND_3))));

    powerErgCondChild->appendRow(powerErgCondChild_1);
    powerErgCondChild->appendRow(powerErgCondChild_3);
    criteriumGroup->appendRow(powerErgCondChild);

    QStandardItem* couplingFactorsChild=new QStandardItem("Fattori di accoppiamento");

    QStandardItem* couplingFactor1Child= new QStandardItem(QString("&eta;<sub>1</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::COUPLINGFACTOR1))));
    QStandardItem* couplingFactor3Child= new QStandardItem(QString("&eta;<sub>3</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::COUPLINGFACTOR3))));
    couplingFactorsChild->appendRow(couplingFactor1Child);
    couplingFactorsChild->appendRow(couplingFactor3Child);
    criteriumGroup->appendRow(couplingFactorsChild);

    QStandardItem* diametersCondChild=new QStandardItem("Diametri dei diaframmi");

    QStandardItem* diameterCond1Child= new QStandardItem(QString("a<sub>1</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DIAMETERCOND1))));
    QStandardItem* diameterCond3Child= new QStandardItem(QString("a<sub>3</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DIAMETERCOND3))));
    diametersCondChild->appendRow(diameterCond1Child);
    diametersCondChild->appendRow(diameterCond3Child);

    criteriumGroup->appendRow(diametersCondChild);

    QStandardItem* distancesCondChild=new QStandardItem("Distanze dei diaframmi");

    QStandardItem* distanceCond1Child= new QStandardItem(QString("d<sub>1</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DISTANCECOND1))));
    QStandardItem* distanceCond3Child= new QStandardItem(QString("d<sub>3</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DISTANCECOND3))));
    distancesCondChild->appendRow(distanceCond1Child);
    distancesCondChild->appendRow(distanceCond3Child);

    criteriumGroup->appendRow(distancesCondChild);

    QStandardItem* beamsAtStopCondChild=new QStandardItem("Diametri dei fasci ai diaframmi");

    QStandardItem* beamAtStopCond1Child= new QStandardItem(QString("a<sub>b1</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::BEAMATSTOPCOND1))));
    QStandardItem* beamAtStopCond3Child= new QStandardItem(QString("a<sub>b3</sub>=%1")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::BEAMATSTOPCOND3))));
    beamsAtStopCondChild->appendRow(beamAtStopCond1Child);
    beamsAtStopCondChild->appendRow(beamAtStopCond3Child);

    criteriumGroup->appendRow(beamsAtStopCondChild);

    QStandardItem* laserClassChild= new QStandardItem(QString(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::CLASS))));
    criteriumGroup->appendRow(laserClassChild);

    leaModel->appendRow(criteriumGroup);

    HtmlDelegate* treeHtmlDelegate = new HtmlDelegate();
    dockLea->ui->treeView->setModel(leaModel);
    dockLea->ui->treeView->setItemDelegate(treeHtmlDelegate);

    QModelIndex modelIndex;
    int rows=leaModel->rowCount();

    for(int i=0; i<rows; i++)
    {
        modelIndex=leaModel->index(i,0);
        dockLea->ui->treeView->setExpanded(modelIndex, true);
    }
}

void DockControls::updateCW_SP_LEA_data()
{
    int index=0;
    QModelIndex firstIndex=leaModel->index(0, 0);
    QModelIndex secondIndex=leaModel->index(index, 0, firstIndex);
    QModelIndex thirdIndex=leaModel->index(0, 0, secondIndex);

    if(n_laser==operation::CONTINUOS_WAVE)
    {
        leaModel->setData(thirdIndex, QVariant(QString("T<sub>b</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TIMEBASE)))));
        index++;
        secondIndex=leaModel->index(index, 0, firstIndex);
    }

    leaModel->setData(secondIndex, QVariant(QString("%1").arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TLEA)))));

    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("formula: %1").arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::FORMULALEA)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("valore: %1").arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::LEA_VALUE)))));

    index++;
    secondIndex=leaModel->index(index, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2").arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TPOWERERGCOND_1)))
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::POWERERGCOND_1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2").arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::TPOWERERGCOND_3)))
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::POWERERGCOND_3)))));

    index++;
    secondIndex=leaModel->index(index, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>1</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::COUPLINGFACTOR1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>3</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::COUPLINGFACTOR3)))));

    index++;
    secondIndex=leaModel->index(index, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>1</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DIAMETERCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>3</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DIAMETERCOND3)))));

    index++;
    secondIndex=leaModel->index(index, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>1</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DISTANCECOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>3</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::DISTANCECOND3)))));

    index++;
    secondIndex=leaModel->index(index, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b1</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::BEAMATSTOPCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b3</sub>=%1")
                                                    .arg(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::BEAMATSTOPCOND3)))));

    index++;
    secondIndex=leaModel->index(index, 0, firstIndex);
    leaModel->setData(secondIndex, QVariant(QString(LEA_Data.at(static_cast<int>(DockLea::CW_SP_Data::CLASS)))));
}

void DockControls::updateMP_LEA_data()
{
    QModelIndex firstIndex=leaModel->index(0, 0);
    leaModel->setData(firstIndex, QVariant("Criterio dell'impulso"));
    QModelIndex secondIndex=leaModel->index(0, 0, firstIndex);
    QModelIndex thirdIndex=leaModel->index(0, 0, secondIndex);

    leaModel->setData(thirdIndex, QVariant(QString("T<sub>b</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TIMEBASE)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("T<sub>e</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TE)))));

    thirdIndex=leaModel->index(2, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("T<sub>i</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TI_PRF)))));

    thirdIndex=leaModel->index(3, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("T<sub>i</sub> &times; PRF=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::MEANPOWER)))));

    thirdIndex=leaModel->index(4, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("P<sub>m</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::MEANPOWER)))));

    thirdIndex=leaModel->index(5, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("N=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::PULSENUMBER)))));

    thirdIndex=leaModel->index(6, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("Conteggio=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::COUNTING)))));

    thirdIndex=leaModel->index(7, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("C<sub>5</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::C5)))));

    secondIndex=leaModel->index(1, 0, firstIndex);
    leaModel->setData(secondIndex, QVariant(QString("%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TLEA)))));

    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("formula: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::FORMULALEA)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("valore: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::LEA_VALUE)))));

    secondIndex=leaModel->index(2, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TPOWERERGCOND_1)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::POWERERGCOND_1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TPOWERERGCOND_3)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::POWERERGCOND_3)))));

    secondIndex=leaModel->index(3, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::COUPLINGFACTOR1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::COUPLINGFACTOR3)))));

    secondIndex=leaModel->index(4, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::DIAMETERCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::DIAMETERCOND3)))));

    secondIndex=leaModel->index(5, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::DISTANCECOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::DISTANCECOND3)))));

    secondIndex=leaModel->index(6, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::BEAMATSTOPCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::BEAMATSTOPCOND3)))));

    secondIndex=leaModel->index(7, 0, firstIndex);
    leaModel->setData(secondIndex, QVariant(QString("Classificazione parziale: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::CLASS)))));

    firstIndex=leaModel->index(1, 0);
    leaModel->setData(firstIndex, QVariant("Criterio della potenza media"));

    secondIndex=leaModel->index(0, 0, firstIndex);
    leaModel->setData(secondIndex, QVariant(QString("%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::TMEANLEA)))));

    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("formula: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANFORMULALEA)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("valore: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANLEA_VALUE)))));

    secondIndex=leaModel->index(1, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::TMEANPOWERERGCOND_1)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANPOWERERGCOND_1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::TMEANPOWERERGCOND_3)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANPOWERERGCOND_3)))));

    secondIndex=leaModel->index(2, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANCOUPLINGFACTOR1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANCOUPLINGFACTOR3)))));

    secondIndex=leaModel->index(3, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDIAMETERCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDIAMETERCOND3)))));

    secondIndex=leaModel->index(4, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDISTANCECOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDISTANCECOND3)))));

    secondIndex=leaModel->index(5, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANBEAMATSTOPCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANBEAMATSTOPCOND3)))));

    secondIndex=leaModel->index(6, 0, firstIndex);
    leaModel->setData(secondIndex, QVariant(QString("Classificazione parziale: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANCLASS)))));
}

void DockControls::updateMP_Thermal_LEA_data(const QString & header)
{
    QModelIndex firstIndex=leaModel->index(2, 0);
    leaModel->setData(firstIndex, QVariant(header));
    QModelIndex secondIndex=leaModel->index(0, 0, firstIndex);
    QModelIndex thirdIndex=leaModel->index(0, 0, secondIndex);

    secondIndex=leaModel->index(0, 0, firstIndex);
    leaModel->setData(secondIndex, QVariant(QString("%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALLEA_VALUE)))));

    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("formula: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALFORMULALEA)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("valore: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALLEA_VALUE)))));

    secondIndex=leaModel->index(1, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALPOWERERGCOND_1)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALPOWERERGCOND_1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("%1= %2")
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALPOWERERGCOND_3)))
                                        .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALPOWERERGCOND_3)))));

    secondIndex=leaModel->index(2, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCOUPLINGFACTOR1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("&eta;<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCOUPLINGFACTOR3)))));

    secondIndex=leaModel->index(3, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDIAMETERCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDIAMETERCOND3)))));

    secondIndex=leaModel->index(4, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDISTANCECOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("d<sub>3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDISTANCECOND3)))));

    secondIndex=leaModel->index(5, 0, firstIndex);
    thirdIndex=leaModel->index(0, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b1</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALBEAMATSTOPCOND1)))));

    thirdIndex=leaModel->index(1, 0, secondIndex);
    leaModel->setData(thirdIndex, QVariant(QString("a<sub>b3</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALBEAMATSTOPCOND3)))));

    secondIndex=leaModel->index(6, 0, firstIndex);
    leaModel->setData(secondIndex, QVariant(QString("Classificazione parziale: %1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCLASS)))));
}

void DockControls::setDataForMP_Operation()
{
    LEA_Data.clear();
    QString diameterCond1;
    QString distanceCond1;
    QString beamAtStopCond1;
    QString couplingFactor1;

    if(isnan(MyLaserClassMP_Pr->getCouplingFactor_Cond_1()))
        couplingFactor1="N.A.";
    else
        couplingFactor1=QString::number(MyLaserClassMP_Pr->getCouplingFactor_Cond_1(), 'e', 2);

    QString couplingFactor3=QString::number(MyLaserClassMP_Pr->getCouplingFactor_Cond_3(), 'e', 2);

    if(isnan(MyLaserClassMP_Pr->getApCond_1()))
        diameterCond1="N.A.";
    else
        diameterCond1=QString::number(MyLaserClassMP_Pr->getApCond_1(), 'e', 2)+" mm";

    QString diameterCond3=QString::number(MyLaserClassMP_Pr->getApCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassMP_Pr->getDistCond_1()))
        distanceCond1="N.A.";
    else
        distanceCond1=QString::number(MyLaserClassMP_Pr->getDistCond_1(), 'e', 2)+" mm";

    QString distanceCond3=QString::number(MyLaserClassMP_Pr->getDistCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassMP_Pr->getBeamAtStop_Cond_1()))
        beamAtStopCond1="N.A.";
    else
        beamAtStopCond1=QString::number(MyLaserClassMP_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm";

    QString beamAtStopCond3=QString::number(MyLaserClassMP_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm";

    QString Te_Label=QString::number(MyLaserClassMP_Pr->getTe(), 'e', 2)+" s";
    QString Ti_Label=QString::number(MyLaserClassMP_Pr->getTi(), 'e', 2)+" s";
    QString Ti_prf_Label=QString::number(MyLaserClassMP_Pr->getTi()*MyLaserMP_Pr->getPRF());

    QString TimeBase_Label=QString("%1 s")
                         .arg(QString::number(MyLaserClassMP_Pr->getTimeBase()));


    LaserClassCW::laserClass myLaserClass=MyLaserClassMP_Pr-> getLaserClass();
    QString FormulaLEA;
    QString FormulaLEA_Tipo;
    QString FormulaLEA_Unit;
    QString LEA_Value;
    QString PowerErgCond_1;
    QString PowerErgCond_3;
    QString FormulaLEA_Label;
    QString LEA_Value_Label;
    QString tFormulaLEA_Label;
    QString tLEA_Label;
    QString PowerErgCond_1_Label;
    QString PowerErgCond_3_Label;
    ComputeLEA::ClassData myClassData;

    if ((myLaserClass==LaserClassCW::laserClass::CLASSE_1)or(myLaserClass==LaserClassCW::laserClass::CLASSE_1M))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_1_1M;
        tFormulaLEA_Label="Formula 1 - 1M";
        tLEA_Label="LEA 1 - 1M";
    }
    else if ((myLaserClass==LaserClassCW::laserClass::CLASSE_2)or(myLaserClass==LaserClassCW::laserClass::CLASSE_2M))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_2_2M;
        tFormulaLEA_Label="Formula 2 - 2M";
        tLEA_Label="LEA 2 - 2M";
    }
    else if (myLaserClass==LaserClassCW::laserClass::CLASSE_3R)
    {
        myClassData=ComputeLEA::ClassData::CLASSE_3R;
        tFormulaLEA_Label="Formula 3R";
        tLEA_Label="LEA 3R";
    }
    else if ((myLaserClass==LaserClassCW::laserClass::CLASSE_3B)or(myLaserClass==LaserClassCW::laserClass::CLASSE_4))
    {
        myClassData=ComputeLEA::ClassData::CLASSE_3B;
        tFormulaLEA_Label="Formula 3B";
        tLEA_Label="LEA 3B";
    }
    else
    {
        myClassData=ComputeLEA::ClassData::SENZA_CLASSIFICA;
        tFormulaLEA_Label="NC";
        tLEA_Label="NC";
    }

    FormulaLEA_Tipo=QString::fromStdString(MyLaserClassMP_Pr->getLEA_FormulaTipo()[static_cast<int>(myClassData)]);
    FormulaLEA=QString::fromStdString(MyLaserClassMP_Pr->getLEA_Formula()[static_cast<int>(myClassData)]);
    FormulaLEA_Unit=QString::fromStdString(MyLaserClassMP_Pr->getLEA_FormulaUnit()[static_cast<int>(myClassData)]);
    LEA_Value=QString::number(MyLaserClassMP_Pr->getLEA()[static_cast<int>(myClassData)], 'e', 2);
    FormulaLEA_Label=QString("%1=%2 %3")
                             .arg(FormulaLEA_Tipo)
                             .arg(FormulaLEA)
                             .arg(FormulaLEA_Unit);

    LEA_Value_Label=QString("%1 %2")
                             .arg(LEA_Value)
                             .arg(FormulaLEA_Unit);


    PowerErgCond_1=QString::number(MyLaserClassMP_Pr->getPowerErg_Cond_1()[static_cast<int>(myClassData)], 'e', 2);
    PowerErgCond_3=QString::number(MyLaserClassMP_Pr->getPowerErg_Cond_3()[static_cast<int>(myClassData)], 'e', 2);

    if(isnan(MyLaserClassMP_Pr->getPowerErg_Cond_1()[static_cast<int>(myClassData)]))
        PowerErgCond_1_Label="N.A.";
    else
        PowerErgCond_1_Label=QString("%1 %2")
                                .arg(PowerErgCond_1)
                                .arg(FormulaLEA_Unit);

    PowerErgCond_3_Label=QString("%1 %2")
                             .arg(PowerErgCond_3)
                             .arg(FormulaLEA_Unit);

    QString tPowerErgCond_1_Label=QString("%1<sub>Acc 1</sub>").arg(FormulaLEA_Tipo);
    QString tPowerErgCond_3_Label=QString("%1<sub>Acc 3</sub>").arg(FormulaLEA_Tipo);

    QString classLabel=getLaserClassString(myLaserClass);

     /*****************************************************
      * valutazione relativa l'emissione media del laser  *
      * ***************************************************/

    QString meanCouplingFactor1=QString::number(MyLaserClassMP_Pr->getMeanCouplingFactor_Cond_1(), 'e', 2);
    QString meanCouplingFactor3=QString::number(MyLaserClassMP_Pr->getMeanCouplingFactor_Cond_3(), 'e', 2);
    QString meanDiameterCond1=QString::number(MyLaserClassMP_Pr->getMeanApCond_1(), 'e', 2)+" mm";
    QString meanDiameterCond3=QString::number(MyLaserClassMP_Pr->getMeanApCond_3(), 'e', 2)+" mm";
    QString meanDistanceCond1=QString::number(MyLaserClassMP_Pr->getMeanDistCond_1(), 'e', 2)+" mm";
    QString meanDistanceCond3=QString::number(MyLaserClassMP_Pr->getMeanDistCond_3(), 'e', 2)+" mm";
    QString meanBeamAtStopCond1=QString::number(MyLaserClassMP_Pr->getMeanBeamAtStop_Cond_1(), 'e', 2)+" mm";
    QString meanBeamAtStopCond3=QString::number(MyLaserClassMP_Pr->getMeanBeamAtStop_Cond_3(), 'e', 2)+" mm";

    LaserClassCW::laserClass myMeanLaserClass=MyLaserClassMP_Pr->getMeanLaserClassAssigned();
    LaserClassCW::laserClass mySystemLaserClass=MyLaserClassMP_Pr->getSystemClassValutation();
    QString meanFormulaLEA;
    QString meanFormulaLEA_Tipo;
    QString meanFormulaLEA_Unit;
    QString meanLEA_Value;
    QString meanPowerErgCond_1;
    QString meanPowerErgCond_3;
    QString meanFormulaLEA_Label;
    QString meanLEA_Value_Label;
    QString tMeanFormulaLEA_Label;
    QString tMeanLEA_Label;
    QString meanPowerErgCond_1_Label;
    QString meanPowerErgCond_3_Label;
    ComputeLEA::ClassData myMeanClassData;

    if ((myMeanLaserClass==LaserClassCW::laserClass::CLASSE_1)or(myMeanLaserClass==LaserClassCW::laserClass::CLASSE_1M))
    {
        myMeanClassData=ComputeLEA::ClassData::CLASSE_1_1M;
        tMeanFormulaLEA_Label="Formula 1 - 1M";
        tMeanLEA_Label="LEA 1 - 1M";
    }
    else if ((myMeanLaserClass==LaserClassCW::laserClass::CLASSE_2)or(myMeanLaserClass==LaserClassCW::laserClass::CLASSE_2M))
    {
        myMeanClassData=ComputeLEA::ClassData::CLASSE_2_2M;
        tMeanFormulaLEA_Label="Formula 2 - 2M";
        tMeanLEA_Label="LEA 2 - 2M";
    }
    else if (myMeanLaserClass==LaserClassCW::laserClass::CLASSE_3R)
    {
        myMeanClassData=ComputeLEA::ClassData::CLASSE_3R;
        tMeanFormulaLEA_Label="Formula 3R";
        tMeanLEA_Label="LEA 3R";
    }
    else if ((myMeanLaserClass==LaserClassCW::laserClass::CLASSE_3B)or(myMeanLaserClass==LaserClassCW::laserClass::CLASSE_4))
    {
        myMeanClassData=ComputeLEA::ClassData::CLASSE_3B;
        tMeanFormulaLEA_Label="Formula 3B";
        tMeanLEA_Label="LEA 3B";
    }
    else
    {
        myMeanClassData=ComputeLEA::ClassData::SENZA_CLASSIFICA;
        tMeanFormulaLEA_Label="NC";
        tMeanLEA_Label="NC";
    }

    meanFormulaLEA_Tipo=QString::fromStdString(MyLaserClassMP_Pr->getMeanLEA_FormulaTipo()[static_cast<int>(myMeanClassData)]);
    meanFormulaLEA=QString::fromStdString(MyLaserClassMP_Pr->getMeanLEA_Formula()[static_cast<int>(myMeanClassData)]);
    meanFormulaLEA_Unit=QString::fromStdString(MyLaserClassMP_Pr->getMeanLEA_FormulaUnit()[static_cast<int>(myMeanClassData)]);
    meanLEA_Value=QString::number(MyLaserClassMP_Pr->getMeanLEA()[static_cast<int>(myMeanClassData)], 'e', 2);
    meanFormulaLEA_Label=QString("%1=%2 %3")
                             .arg(meanFormulaLEA_Tipo)
                             .arg(meanFormulaLEA)
                             .arg(meanFormulaLEA_Unit);

    meanLEA_Value_Label=QString("%1 %2")
                             .arg(meanLEA_Value)
                             .arg(meanFormulaLEA_Unit);

    meanPowerErgCond_1=QString::number(MyLaserClassMP_Pr->getMeanPowerErg_Cond_1()[static_cast<int>(myMeanClassData)], 'e', 2);
    meanPowerErgCond_3=QString::number(MyLaserClassMP_Pr->getMeanPowerErg_Cond_3()[static_cast<int>(myMeanClassData)], 'e', 2);

    if(isnan(MyLaserClassMP_Pr->getMeanPowerErg_Cond_1()[static_cast<int>(myMeanClassData)]))
        meanPowerErgCond_1_Label="N.A.";
    else
        meanPowerErgCond_1_Label=QString("%1 %2")
                                .arg(meanPowerErgCond_1)
                                .arg(meanFormulaLEA_Unit);

    meanPowerErgCond_3_Label=QString("%1 %2")
                             .arg(meanPowerErgCond_3)
                             .arg(meanFormulaLEA_Unit);


    QString tMeanPowerErgCond_1_Label=QString("%1<sub>Acc 1</sub>").arg(meanFormulaLEA_Tipo);
    QString tMeanPowerErgCond_3_Label=QString("%1<sub>Acc 3</sub>").arg(meanFormulaLEA_Tipo);
    QString C5_Label;

    if((wavelength>=400)and(wavelength<=1400))
        C5_Label=QString::number(MyLaserClassMP_Pr->getC5Coefficient(),'e', 2);
    else
        C5_Label="Non applicabile";

    QString MeanPowerLabel=QString::number(MyLaserClassMP_Pr->getMeanPower(),'e', 2)+" W";
    QString PulseNumberLabel=QString::number(MyLaserClassMP_Pr->getPulseNumber());
    QString CountingLabel=QString::fromStdString(MyLaserClassMP_Pr->valutateCounting());

   QString meanClassLabel=getLaserClassString(myMeanLaserClass);
   QString systemClassString=getLaserClassString(mySystemLaserClass);

   if(mySystemLaserClass==LaserClassCW::laserClass::CLASSE_1)
        ui->warning_Label->setPixmap(QPixmap(":/images/ok_circle.png"));
   else
        ui->warning_Label->setPixmap(QPixmap(":/images/laser_warning.png"));

   ui->class_Label->setText(systemClassString);

    LEA_Data.push_back(TimeBase_Label);                //0
    LEA_Data.push_back(Te_Label);                      //1
    LEA_Data.push_back(Ti_Label);                      //2
    LEA_Data.push_back(Ti_prf_Label);                  //3
    LEA_Data.push_back(MeanPowerLabel);                //4
    LEA_Data.push_back(PulseNumberLabel);              //5
    LEA_Data.push_back(CountingLabel);                 //6
    LEA_Data.push_back(C5_Label);                      //7
    LEA_Data.push_back(tLEA_Label);                    //8
    LEA_Data.push_back(FormulaLEA_Label);              //9
    LEA_Data.push_back(LEA_Value_Label);               //10
    LEA_Data.push_back(tPowerErgCond_1_Label);         //11
    LEA_Data.push_back(PowerErgCond_1_Label);          //12
    LEA_Data.push_back(tPowerErgCond_3_Label);         //13
    LEA_Data.push_back(PowerErgCond_3_Label);          //14
    LEA_Data.push_back(couplingFactor1);               //15
    LEA_Data.push_back(couplingFactor3);               //16
    LEA_Data.push_back(diameterCond1);                 //17
    LEA_Data.push_back(diameterCond3);                 //18
    LEA_Data.push_back(distanceCond1);                 //19
    LEA_Data.push_back(distanceCond3);                 //20
    LEA_Data.push_back(beamAtStopCond1);               //21
    LEA_Data.push_back(beamAtStopCond3);               //22
    LEA_Data.push_back(classLabel);                    //23
    LEA_Data.push_back(tMeanLEA_Label);                //24
    LEA_Data.push_back(meanFormulaLEA_Label);          //25
    LEA_Data.push_back(meanLEA_Value_Label);           //26
    LEA_Data.push_back(tMeanPowerErgCond_1_Label);     //27
    LEA_Data.push_back(meanPowerErgCond_1_Label);      //28
    LEA_Data.push_back(tMeanPowerErgCond_3_Label);     //29
    LEA_Data.push_back(meanPowerErgCond_3_Label);      //30
    LEA_Data.push_back(meanCouplingFactor1);           //31
    LEA_Data.push_back(meanCouplingFactor3);           //32
    LEA_Data.push_back(meanDiameterCond1);             //33
    LEA_Data.push_back(meanDiameterCond3);             //34
    LEA_Data.push_back(meanDistanceCond1);             //35
    LEA_Data.push_back(meanDistanceCond3);             //36
    LEA_Data.push_back(meanBeamAtStopCond1);           //37
    LEA_Data.push_back(meanBeamAtStopCond3);           //38
    LEA_Data.push_back(meanClassLabel);                //39
    LEA_Data.push_back(systemClassString);             //40
}

void DockControls::setMP_LEAModel()
{
    leaModel=new QStandardItemModel();

    QStandardItem* criteriumGroup;
    QStandardItem* timeChild;
    QStandardItem* timeBaseValueChild;
    QStandardItem* TeValueChild;
    QStandardItem* TiValueChild;
    QStandardItem* Ti_prfValueChild;
    QStandardItem* MeanPowerChild;
    QStandardItem* PulseNumberChild;
    QStandardItem* CountingChild;
    QStandardItem* C5ValueChild;

    criteriumGroup=new QStandardItem("Criterio dell'impulso");
    timeChild=new QStandardItem("Paramentri");
    timeBaseValueChild=new QStandardItem(QString("T<sub>b</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TIMEBASE))));
    TeValueChild=new QStandardItem(QString("T<sub>e</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TE))));
    TiValueChild=new QStandardItem(QString("T<sub>i</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TI))));
    Ti_prfValueChild=new QStandardItem(QString("T<sub>i</sub>/PRF=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TI_PRF))));

    MeanPowerChild=new QStandardItem(QString("P<sub>m</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::MEANPOWER))));
    PulseNumberChild=new QStandardItem(QString("N=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::PULSENUMBER))));
    CountingChild=new QStandardItem(QString("Conteggio=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::COUNTING))));
    C5ValueChild=new QStandardItem(QString("C<sub>5</sub>=%1").arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::C5))));

    timeChild->appendRow(timeBaseValueChild);
    timeChild->appendRow(TeValueChild);
    timeChild->appendRow(TiValueChild);
    timeChild->appendRow(Ti_prfValueChild);
    timeChild->appendRow(MeanPowerChild);
    timeChild->appendRow(PulseNumberChild);
    timeChild->appendRow(CountingChild);
    timeChild->appendRow(C5ValueChild);
    criteriumGroup->appendRow(timeChild);

    QStandardItem* LEA_Child=new QStandardItem(QString("%1")
                                          .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TLEA))));
    QStandardItem* LEA_formulaChild=new QStandardItem(QString("formula: %1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::FORMULALEA))));
    QStandardItem* LEA_valueChild=new QStandardItem(QString("valore: %1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::LEA_VALUE))));

    LEA_Child->appendRow(LEA_formulaChild);
    LEA_Child->appendRow(LEA_valueChild);

    criteriumGroup->appendRow(LEA_Child);

    QStandardItem* powerErgCondChild=new QStandardItem("Livello accessibile");

    QStandardItem* powerErgCondChild_1=new QStandardItem(QString("%1= %2")
                                             .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TPOWERERGCOND_1)))
                                             .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::POWERERGCOND_1))));

    QStandardItem* powerErgCondChild_3=new QStandardItem(QString("%1= %2")
                                             .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::TPOWERERGCOND_3)))
                                             .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::POWERERGCOND_3))));

    powerErgCondChild->appendRow(powerErgCondChild_1);
    powerErgCondChild->appendRow(powerErgCondChild_3);
    criteriumGroup->appendRow(powerErgCondChild);

    QStandardItem* couplingFactorsChild=new QStandardItem("Fattori di accoppiamento");

    QStandardItem* couplingFactor1Child= new QStandardItem(QString("&eta;<sub>1</sub>=%1")
                                              .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::COUPLINGFACTOR1))));
    QStandardItem* couplingFactor3Child= new QStandardItem(QString("&eta;<sub>3</sub>=%1")                                                       
                                              .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::COUPLINGFACTOR3))));
    couplingFactorsChild->appendRow(couplingFactor1Child);
    couplingFactorsChild->appendRow(couplingFactor3Child);
    criteriumGroup->appendRow(couplingFactorsChild);

    QStandardItem* diametersCondChild=new QStandardItem("Diametri dei diaframmi");

    QStandardItem* diameterCond1Child= new QStandardItem(QString("a<sub>1</sub>=%1")
                                               .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::DIAMETERCOND1))));
    QStandardItem* diameterCond3Child= new QStandardItem(QString("a<sub>3</sub>=%1")
                                               .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::DIAMETERCOND3))));
    diametersCondChild->appendRow(diameterCond1Child);
    diametersCondChild->appendRow(diameterCond3Child);

    criteriumGroup->appendRow(diametersCondChild);

    QStandardItem* distancesCondChild=new QStandardItem("Distanze dei diaframmi");

    QStandardItem* distanceCond1Child= new QStandardItem(QString("d<sub>1</sub>=%1")
                                                 .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::DISTANCECOND1))));
    QStandardItem* distanceCond3Child= new QStandardItem(QString("d<sub>3</sub>=%1")
                                                 .arg(LEA_Data.at((static_cast<int>(DockLea::MP_SP_Data::DISTANCECOND3)))));
    distancesCondChild->appendRow(distanceCond1Child);
    distancesCondChild->appendRow(distanceCond3Child);

    criteriumGroup->appendRow(distancesCondChild);

    QStandardItem* beamsAtStopCondChild=new QStandardItem("Diametri dei fasci ai diaframmi");

    QStandardItem* beamAtStopCond1Child= new QStandardItem(QString("a<sub>b1</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::BEAMATSTOPCOND1))));
    QStandardItem* beamAtStopCond3Child= new QStandardItem(QString("a<sub>b3</sub>=%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::BEAMATSTOPCOND3))));
    beamsAtStopCondChild->appendRow(beamAtStopCond1Child);
    beamsAtStopCondChild->appendRow(beamAtStopCond3Child);

    criteriumGroup->appendRow(beamsAtStopCondChild);

    QStandardItem* laserClassChild= new QStandardItem(QString("Classificazione parziale: %1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_SP_Data::CLASS))));
    criteriumGroup->appendRow(laserClassChild);

    leaModel->appendRow(criteriumGroup);

    QStandardItem* meanCriteriumGroup=new QStandardItem(QString("Criterio della potenza media"));
    QStandardItem* meanLEA_Child=new QStandardItem(QString("%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::TMEANLEA))));
    QStandardItem* meanLEA_formulaChild=new QStandardItem(QString("formula: %1")                                                         
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANFORMULALEA))));
    QStandardItem* meanLEA_valueChild=new QStandardItem(QString("valore: %1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANLEA_VALUE))));

    meanLEA_Child->appendRow(meanLEA_formulaChild);
    meanLEA_Child->appendRow(meanLEA_valueChild);

    meanCriteriumGroup->appendRow(meanLEA_Child);

    QStandardItem* meanPowerErgCondChild=new QStandardItem("Livello medio accessibile");

    QStandardItem* meanPowerErgCondChild_1=new QStandardItem(QString("%1= %2")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::TMEANPOWERERGCOND_1)))
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANPOWERERGCOND_1))));

    QStandardItem* meanPowerErgCondChild_3=new QStandardItem(QString("%1= %2")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::TMEANPOWERERGCOND_3)))
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANPOWERERGCOND_3))));

    meanPowerErgCondChild->appendRow(meanPowerErgCondChild_1);
    meanPowerErgCondChild->appendRow(meanPowerErgCondChild_3);
    meanCriteriumGroup->appendRow(meanPowerErgCondChild);

    QStandardItem* meanCouplingFactorsChild=new QStandardItem("Fattori di accoppiamento");

    QStandardItem* meanCouplingFactor1Child= new QStandardItem(QString("&eta;<sub>1</sub>=%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANCOUPLINGFACTOR1))));
    QStandardItem* meanCouplingFactor3Child= new QStandardItem(QString("&eta;<sub>3</sub>=%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANCOUPLINGFACTOR3))));
    meanCouplingFactorsChild->appendRow(meanCouplingFactor1Child);
    meanCouplingFactorsChild->appendRow(meanCouplingFactor3Child);
    meanCriteriumGroup->appendRow(meanCouplingFactorsChild);

    QStandardItem* meanDiametersCondChild=new QStandardItem("Diametri dei diaframmi");

    QStandardItem* meanDiameterCond1Child= new QStandardItem(QString("a<sub>1</sub>=%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDIAMETERCOND1))));
    QStandardItem* meanDiameterCond3Child= new QStandardItem(QString("a<sub>3</sub>=%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDIAMETERCOND3))));
    meanDiametersCondChild->appendRow(meanDiameterCond1Child);
    meanDiametersCondChild->appendRow(meanDiameterCond3Child);

    meanCriteriumGroup->appendRow(meanDiametersCondChild);

    QStandardItem* meanDistancesCondChild=new QStandardItem("Distanze dei diaframmi");

    QStandardItem* meanDistanceCond1Child= new QStandardItem(QString("d<sub>1</sub>=%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDISTANCECOND1))));
    QStandardItem* meanDistanceCond3Child= new QStandardItem(QString("d<sub>3</sub>=%1")                                             
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANDISTANCECOND3))));
    meanDistancesCondChild->appendRow(meanDistanceCond1Child);
    meanDistancesCondChild->appendRow(meanDistanceCond3Child);

    meanCriteriumGroup->appendRow(meanDistancesCondChild);

    QStandardItem* meanBeamsAtStopCondChild=new QStandardItem("Diametri dei fasci ai diaframmi");

    QStandardItem* meanBeamAtStopCond1Child= new QStandardItem(QString("a<sub>b1</sub>=%1")                                                             
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANBEAMATSTOPCOND1))));
    QStandardItem* meanBeamAtStopCond3Child= new QStandardItem(QString("a<sub>b3</sub>=%1")
                                           .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANBEAMATSTOPCOND3))));
    meanBeamsAtStopCondChild->appendRow(meanBeamAtStopCond1Child);
    meanBeamsAtStopCondChild->appendRow(meanBeamAtStopCond3Child);

    meanCriteriumGroup->appendRow(meanBeamsAtStopCondChild);

    QStandardItem* meanLaserClassChild= new QStandardItem(QString("Classificazione parziale: %1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Mean_Data::MEANCLASS))));
    meanCriteriumGroup->appendRow(meanLaserClassChild);

    leaModel->appendRow(meanCriteriumGroup);

    HtmlDelegate* treeHtmlDelegate = new HtmlDelegate();
    dockLea->ui->treeView->setModel(leaModel);
    dockLea->ui->treeView->setItemDelegate(treeHtmlDelegate);

    QModelIndex modelIndex;
    int rows=leaModel->rowCount();

    for(int i=0; i<rows; i++)
    {
        modelIndex=leaModel->index(i,0);
        dockLea->ui->treeView->setExpanded(modelIndex, true);
    }
}

void DockControls::setMP_Thermal_LEAModel()
{
    QStandardItem* thermalCriteriumGroup=new QStandardItem(QString("Criterio dell'impulso per effetti termici"));
    QStandardItem* thermalLEA_Child=new QStandardItem(QString("%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALLEA_VALUE))));
    QStandardItem* thermalLEA_formulaChild=new QStandardItem(QString("formula: %1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALFORMULALEA))));
    QStandardItem* thermalLEA_valueChild=new QStandardItem(QString("valore: %1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALLEA_VALUE))));

    thermalLEA_Child->appendRow(thermalLEA_formulaChild);
    thermalLEA_Child->appendRow(thermalLEA_valueChild);

    thermalCriteriumGroup->appendRow(thermalLEA_Child);

    QStandardItem* thermalPowerErgCondChild=new QStandardItem("Livello termico accessibile");

    QStandardItem* thermalPowerErgCondChild_1=new QStandardItem(QString("%1= %2")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALPOWERERGCOND_1)))
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALPOWERERGCOND_1))));

    QStandardItem* thermalPowerErgCondChild_3=new QStandardItem(QString("%1= %2")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALPOWERERGCOND_3)))
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALPOWERERGCOND_3))));

    thermalPowerErgCondChild->appendRow(thermalPowerErgCondChild_1);
    thermalPowerErgCondChild->appendRow(thermalPowerErgCondChild_3);
    thermalCriteriumGroup->appendRow(thermalPowerErgCondChild);

    QStandardItem* thermalCouplingFactorsChild=new QStandardItem("Fattori di accoppiamento");

    QStandardItem* thermalCouplingFactor1Child= new QStandardItem(QString("&eta;<sub>1</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCOUPLINGFACTOR1))));
    QStandardItem* thermalCouplingFactor3Child= new QStandardItem(QString("&eta;<sub>3</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCOUPLINGFACTOR3))));
    thermalCouplingFactorsChild->appendRow(thermalCouplingFactor1Child);
    thermalCouplingFactorsChild->appendRow(thermalCouplingFactor3Child);
    thermalCriteriumGroup->appendRow(thermalCouplingFactorsChild);

    QStandardItem* thermalDiametersCondChild=new QStandardItem("Diametri dei diaframmi");

    QStandardItem* thermalDiameterCond1Child= new QStandardItem(QString("a<sub>1</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDIAMETERCOND1))));
    QStandardItem* thermalDiameterCond3Child= new QStandardItem(QString("a<sub>3</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDIAMETERCOND3))));
    thermalDiametersCondChild->appendRow(thermalDiameterCond1Child);
    thermalDiametersCondChild->appendRow(thermalDiameterCond3Child);

    thermalCriteriumGroup->appendRow(thermalDiametersCondChild);

    QStandardItem* thermalDistancesCondChild=new QStandardItem("Distanze dei diaframmi");

    QStandardItem* thermalDistanceCond1Child= new QStandardItem(QString("d<sub>1</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDISTANCECOND1))));
    QStandardItem* thermalDistanceCond3Child= new QStandardItem(QString("d<sub>3</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDISTANCECOND3))));
    thermalDistancesCondChild->appendRow(thermalDistanceCond1Child);
    thermalDistancesCondChild->appendRow(thermalDistanceCond3Child);

    thermalCriteriumGroup->appendRow(thermalDistancesCondChild);

    QStandardItem* thermalBeamsAtStopCondChild=new QStandardItem("Diametri dei fasci ai diaframmi");

    QStandardItem* thermalBeamAtStopCond1Child= new QStandardItem(QString("a<sub>b1</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALBEAMATSTOPCOND1))));
    QStandardItem* thermalBeamAtStopCond3Child= new QStandardItem(QString("a<sub>b3</sub>=%1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALBEAMATSTOPCOND3))));
    thermalBeamsAtStopCondChild->appendRow(thermalBeamAtStopCond1Child);
    thermalBeamsAtStopCondChild->appendRow(thermalBeamAtStopCond3Child);

    thermalCriteriumGroup->appendRow(thermalBeamsAtStopCondChild);

    QStandardItem* thermalLaserClassChild= new QStandardItem(QString("Classificazione parziale: %1")
                                            .arg(LEA_Data.at(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCLASS))));
    thermalCriteriumGroup->appendRow(thermalLaserClassChild);

    leaModel->appendRow(thermalCriteriumGroup);

    HtmlDelegate* treeHtmlDelegate = new HtmlDelegate();
    dockLea->ui->treeView->setModel(leaModel);
    dockLea->ui->treeView->setItemDelegate(treeHtmlDelegate);

    QModelIndex modelIndex;
    int rows=leaModel->rowCount();

    for(int i=0; i<rows; i++)
    {
        modelIndex=leaModel->index(i,0);
        dockLea->ui->treeView->setExpanded(modelIndex, true);
    }
}


void DockControls::setMP_ThermalTi_LEAModel()
{
    QStandardItem* tiCriteriumGroup=new QStandardItem(QString("Criterio dell'impulso per effetti termici"));
    QStandardItem* tiLEA_Child=new QStandardItem(QString("%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALLEA_VALUE)));
    QStandardItem* tiLEA_formulaChild=new QStandardItem(QString("formula: %1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALFORMULALEA)));
    QStandardItem* tiLEA_valueChild=new QStandardItem(QString("valore: %1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALLEA_VALUE)));

    tiLEA_Child->appendRow(tiLEA_formulaChild);
    tiLEA_Child->appendRow(tiLEA_valueChild);

    tiCriteriumGroup->appendRow(tiLEA_Child);

    QStandardItem* tiPowerErgCondChild=new QStandardItem("Livello termico accessibile");

    QStandardItem* tiPowerErgCondChild_1=new QStandardItem(QString("%1= %2")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALPOWERERGCOND_1))
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALPOWERERGCOND_1)));

    QStandardItem* tiPowerErgCondChild_3=new QStandardItem(QString("%1= %2")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::TTHERMALPOWERERGCOND_3))
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALPOWERERGCOND_3)));

    tiPowerErgCondChild->appendRow(tiPowerErgCondChild_1);
    tiPowerErgCondChild->appendRow(tiPowerErgCondChild_3);
    tiCriteriumGroup->appendRow(tiPowerErgCondChild);

    QStandardItem* tiCouplingFactorsChild=new QStandardItem("Fattori di accoppiamento");

    QStandardItem* tiCouplingFactor1Child= new QStandardItem(QString("&eta;<sub>1</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCOUPLINGFACTOR1)));
    QStandardItem* tiCouplingFactor3Child= new QStandardItem(QString("&eta;<sub>3</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCOUPLINGFACTOR3)));
    tiCouplingFactorsChild->appendRow(tiCouplingFactor1Child);
    tiCouplingFactorsChild->appendRow(tiCouplingFactor3Child);
    tiCriteriumGroup->appendRow(tiCouplingFactorsChild);

    QStandardItem* tiDiametersCondChild=new QStandardItem("Diametri dei diaframmi");

    QStandardItem* tiDiameterCond1Child= new QStandardItem(QString("a<sub>1</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDIAMETERCOND1)));
    QStandardItem* tiDiameterCond3Child= new QStandardItem(QString("a<sub>3</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDIAMETERCOND3)));
    tiDiametersCondChild->appendRow(tiDiameterCond1Child);
    tiDiametersCondChild->appendRow(tiDiameterCond3Child);

    tiCriteriumGroup->appendRow(tiDiametersCondChild);

    QStandardItem* tiDistancesCondChild=new QStandardItem("Distanze dei diaframmi");

    QStandardItem* tiDistanceCond1Child= new QStandardItem(QString("d<sub>1</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDISTANCECOND1)));
    QStandardItem* tiDistanceCond3Child= new QStandardItem(QString("d<sub>3</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALDISTANCECOND3)));
    tiDistancesCondChild->appendRow(tiDistanceCond1Child);
    tiDistancesCondChild->appendRow(tiDistanceCond3Child);

    tiCriteriumGroup->appendRow(tiDistancesCondChild);

    QStandardItem* tiBeamsAtStopCondChild=new QStandardItem("Diametri dei fasci ai diaframmi");

    QStandardItem* tiBeamAtStopCond1Child= new QStandardItem(QString("a<sub>b1</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALBEAMATSTOPCOND1)));
    QStandardItem* tiBeamAtStopCond3Child= new QStandardItem(QString("a<sub>b3</sub>=%1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALBEAMATSTOPCOND3)));
    tiBeamsAtStopCondChild->appendRow(tiBeamAtStopCond1Child);
    tiBeamsAtStopCondChild->appendRow(tiBeamAtStopCond3Child);

    tiCriteriumGroup->appendRow(tiBeamsAtStopCondChild);

    QStandardItem* tiLaserClassChild= new QStandardItem(QString("Classificazione parziale: %1")
                                            .arg(static_cast<int>(DockLea::MP_Thermal_Data::THERMALCLASS)));
    tiCriteriumGroup->appendRow(tiLaserClassChild);

    leaModel->appendRow(tiCriteriumGroup);

    HtmlDelegate* treeHtmlDelegate = new HtmlDelegate();
    dockLea->ui->treeView->setModel(leaModel);
    dockLea->ui->treeView->setItemDelegate(treeHtmlDelegate);

    QModelIndex modelIndex;
    int rows=leaModel->rowCount();

    for(int i=0; i<rows; i++)
    {
        modelIndex=leaModel->index(i,0);
        dockLea->ui->treeView->setExpanded(modelIndex, true);
    }
}

void DockControls::setDataForMP_ThermalOperation()
{

    /********************************************************************
     * valutazione relativa agli effetti termici del laser per prf<1/Ti *
     * ******************************************************************/

    QString thermalFormulaLEA;
    QString thermalFormulaLEA_Tipo;
    QString thermalFormulaLEA_Unit;
    QString thermalLEA_Value;
    QString thermalPowerErgCond_1;
    QString thermalPowerErgCond_3;
    QString thermalFormulaLEA_Label;
    QString thermalLEA_Value_Label;
    QString tThermalFormulaLEA_Label;
    QString tThermalLEA_Value_Label;
    QString thermalPowerErgCond_1_Label;
    QString thermalPowerErgCond_3_Label;
    ComputeLEA::ClassData myThermalClassData;

    QString thermalCouplingFactor1;
    QString thermalDiameterCond1;
    QString thermalDistanceCond1;
    QString thermalBeamAtStopCond1;

    if(isnan(MyLaserClassMP_Pr->getCouplingFactor_Cond_1()))
        thermalCouplingFactor1="N.A.";
    else
        thermalCouplingFactor1=QString::number(MyLaserClassMP_Pr->getCouplingFactor_Cond_1(), 'e', 2);

    QString thermalCouplingFactor3=QString::number(MyLaserClassMP_Pr->getCouplingFactor_Cond_3(), 'e', 2);

    if(isnan(MyLaserClassMP_Pr->getApCond_1()))
        thermalDiameterCond1="N.A.";
    else
        thermalDiameterCond1=QString::number(MyLaserClassMP_Pr->getApCond_1(), 'e', 2)+" mm";

    QString thermalDiameterCond3=QString::number(MyLaserClassMP_Pr->getApCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassMP_Pr->getDistCond_1()))
        thermalDistanceCond1="N.A.";
    else
        thermalDistanceCond1=QString::number(MyLaserClassMP_Pr->getDistCond_1(), 'e', 2)+" mm";

    QString thermalDistanceCond3=QString::number(MyLaserClassMP_Pr->getDistCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassMP_Pr->getBeamAtStop_Cond_1()))
        thermalBeamAtStopCond1="N.A.";
    else
        thermalBeamAtStopCond1=QString::number(MyLaserClassMP_Pr->getBeamAtStop_Cond_1(), 'e', 2)+" mm";

    QString thermalBeamAtStopCond3=QString::number(MyLaserClassMP_Pr->getBeamAtStop_Cond_3(), 'e', 2)+" mm";

    QString thermalTimeBase_Label=QString("%1 s")
                         .arg(QString::number(MyLaserClassMP_Pr->getTimeBase()));


    LaserClassCW::laserClass myThermalLaserClass=MyLaserClassMP_Pr->getC5LaserClassAssigned();

    if ((myThermalLaserClass==LaserClassCW::laserClass::CLASSE_1)or(myThermalLaserClass==LaserClassCW::laserClass::CLASSE_1M))
    {
        myThermalClassData=ComputeLEA::ClassData::CLASSE_1_1M;
        tThermalFormulaLEA_Label="Formula 1 - 1M";
        tThermalLEA_Value_Label="LEA 1 - 1M";
    }
    else if ((myThermalLaserClass==LaserClassCW::laserClass::CLASSE_2)or(myThermalLaserClass==LaserClassCW::laserClass::CLASSE_2M))
    {
        myThermalClassData=ComputeLEA::ClassData::CLASSE_2_2M;
        tThermalFormulaLEA_Label="Formula 2 - 2M";
        tThermalLEA_Value_Label="LEA 2 - 2M";
    }
    else if (myThermalLaserClass==LaserClassCW::laserClass::CLASSE_3R)
    {
        myThermalClassData=ComputeLEA::ClassData::CLASSE_3R;
        tThermalFormulaLEA_Label="Formula 3R";
        tThermalLEA_Value_Label="LEA 3R";
    }
    else if ((myThermalLaserClass==LaserClassCW::laserClass::CLASSE_3B)or(myThermalLaserClass==LaserClassCW::laserClass::CLASSE_4))
    {
        myThermalClassData=ComputeLEA::ClassData::CLASSE_3B;
        tThermalFormulaLEA_Label="Formula 3B";
        tThermalLEA_Value_Label="LEA 3B";
    }
    else
    {
        myThermalClassData=ComputeLEA::ClassData::SENZA_CLASSIFICA;
        tThermalFormulaLEA_Label="NC";
        tThermalLEA_Value_Label="NC";
    }

    thermalFormulaLEA_Tipo=QString::fromStdString(MyLaserClassMP_Pr->getLEA_FormulaTipo()[static_cast<int>(myThermalClassData)]);
    thermalFormulaLEA=QString::fromStdString(MyLaserClassMP_Pr->getLEA_Formula()[static_cast<int>(myThermalClassData)]);
    thermalFormulaLEA_Unit=QString::fromStdString(MyLaserClassMP_Pr->getLEA_FormulaUnit()[static_cast<int>(myThermalClassData)]);
    thermalLEA_Value=QString::number(MyLaserClassMP_Pr->getLEA_Corrected()[static_cast<int>(myThermalClassData)], 'e', 2);
    thermalFormulaLEA_Label=QString("%1=%2 %3")
                             .arg(thermalFormulaLEA_Tipo)
                             .arg(thermalFormulaLEA)
                             .arg(thermalFormulaLEA_Unit);

    thermalLEA_Value_Label=QString("%1 %2")
                             .arg(thermalLEA_Value)
                             .arg(thermalFormulaLEA_Unit);

    QString tThermalPowerErgCond_1_Label=QString("%1<sub>Acc 1</sub>").arg(thermalFormulaLEA_Tipo);
    QString tThermalPowerErgCond_3_Label=QString("%1<sub>Acc 3</sub>").arg(thermalFormulaLEA_Tipo);

    thermalPowerErgCond_1=QString::number(MyLaserClassMP_Pr->getPowerErg_Cond_1()[static_cast<int>(myThermalClassData)], 'e', 2);
    thermalPowerErgCond_3=QString::number(MyLaserClassMP_Pr->getPowerErg_Cond_3()[static_cast<int>(myThermalClassData)], 'e', 2);

    if(isnan(MyLaserClassMP_Pr->getPowerErg_Cond_1()[static_cast<int>(myThermalClassData)]))
        thermalPowerErgCond_1_Label="N.A.";
    else
        thermalPowerErgCond_1_Label=QString("%1 %2")
                                .arg(thermalPowerErgCond_1)
                                .arg(thermalFormulaLEA_Unit);

    thermalPowerErgCond_1_Label=QString("%1 %2")
                             .arg(thermalPowerErgCond_1)
                             .arg(thermalFormulaLEA_Unit);

    thermalPowerErgCond_3_Label=QString("%1 %2")
                             .arg(thermalPowerErgCond_3)
                             .arg(thermalFormulaLEA_Unit);

   QString thermalClassLabel=getLaserClassString(myThermalLaserClass);

    LEA_Data.push_back(tThermalLEA_Value_Label);       //41
    LEA_Data.push_back(thermalFormulaLEA_Label);       //42
    LEA_Data.push_back(thermalLEA_Value_Label);        //43
    LEA_Data.push_back(tThermalPowerErgCond_1_Label);  //44
    LEA_Data.push_back(thermalPowerErgCond_1_Label);   //45
    LEA_Data.push_back(tThermalPowerErgCond_3_Label);  //46
    LEA_Data.push_back(thermalPowerErgCond_3_Label);   //47
    LEA_Data.push_back(thermalCouplingFactor1);        //48
    LEA_Data.push_back(thermalCouplingFactor3);        //49
    LEA_Data.push_back(thermalDiameterCond1);          //50
    LEA_Data.push_back(thermalDiameterCond3);          //51
    LEA_Data.push_back(thermalDistanceCond1);          //52
    LEA_Data.push_back(thermalDistanceCond3);          //53
    LEA_Data.push_back(thermalBeamAtStopCond1);          //54
    LEA_Data.push_back(thermalBeamAtStopCond3);          //55
    LEA_Data.push_back(thermalClassLabel);             //56
}


void DockControls::setDataForMP_ThermalTiOperation()
{
        /********************************************************************
         * valutazione relativa agli effetti termici del laser per prf>1/Ti *
         * ******************************************************************/
    LaserClassCW::laserClass myTiLaserClass=MyLaserClassMP_Pr->getTiLaserClassAssigned();

    QString tiFormulaLEA;
    QString tiFormulaLEA_Tipo;
    QString tiFormulaLEA_Unit;
    QString tiLEA_Value;
    QString tiPowerErgCond_1;
    QString tiPowerErgCond_3;
    QString tiFormulaLEA_Label;
    QString tiLEA_Value_Label;
    QString tTiFormulaLEA_Label;
    QString tTiLEA_Value_Label;
    QString tiPowerErgCond_1_Label;
    QString tiPowerErgCond_3_Label;
    ComputeLEA::ClassData myTiClassData;

    QString tiDiameterCond1;
    QString tiDistanceCond1;
    QString tiBeamAtStopCond1;
    QString tiCouplingFactor1;

    if(MyLaserClassMP_Pr->getTiCouplingFactor_Cond_1())
        tiCouplingFactor1="N.A.";
    else
        tiCouplingFactor1=QString::number(MyLaserClassMP_Pr->getTiCouplingFactor_Cond_1(), 'e', 2);

    QString tiCouplingFactor3=QString::number(MyLaserClassMP_Pr->getTiCouplingFactor_Cond_3(), 'e', 2);

    if(isnan(MyLaserClassMP_Pr->getTiApCond_1()))
        tiDiameterCond1="N.A.";
    else
        tiDiameterCond1=QString::number(MyLaserClassMP_Pr->getTiApCond_1(), 'e', 2)+" mm";

    QString tiDiameterCond3=QString::number(MyLaserClassMP_Pr->getTiApCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassMP_Pr->getTiDistCond_1()))
        tiDistanceCond1="N.A.";
    else
        tiDistanceCond1=QString::number(MyLaserClassMP_Pr->getTiDistCond_1(), 'e', 2)+" mm";

    QString tiDistanceCond3=QString::number(MyLaserClassMP_Pr->getTiDistCond_3(), 'e', 2)+" mm";

    if(isnan(MyLaserClassMP_Pr->getTiBeamAtStop_Cond_1()))
        tiBeamAtStopCond1="N.A.";
    else
        tiBeamAtStopCond1=QString::number(MyLaserClassMP_Pr->getTiBeamAtStop_Cond_1(), 'e', 2)+" mm";

    QString tiBeamAtStopCond3=QString::number(MyLaserClassMP_Pr->getTiBeamAtStop_Cond_3(), 'e', 2)+" mm";

    QString tiTimeBase_Label=QString("%1 s")
                    .arg(QString::number(MyLaserClassMP_Pr->getTimeBase()));

    if ((myTiLaserClass==LaserClassCW::laserClass::CLASSE_1)or(myTiLaserClass==LaserClassCW::laserClass::CLASSE_1M))
    {
        myTiClassData=ComputeLEA::ClassData::CLASSE_1_1M;
        tTiFormulaLEA_Label="Formula 1 - 1M";
        tTiLEA_Value_Label="LEA 1 - 1M";
    }
    else if ((myTiLaserClass==LaserClassCW::laserClass::CLASSE_2)or(myTiLaserClass==LaserClassCW::laserClass::CLASSE_2M))
    {
        myTiClassData=ComputeLEA::ClassData::CLASSE_2_2M;
        tTiFormulaLEA_Label="Formula 2 - 2M";
        tTiLEA_Value_Label="LEA 2 - 2M";
    }
    else if (myTiLaserClass==LaserClassCW::laserClass::CLASSE_3R)
    {
        myTiClassData=ComputeLEA::ClassData::CLASSE_3R;
        tTiFormulaLEA_Label="Formula 3R";
        tTiLEA_Value_Label="LEA 3R";
    }
    else if ((myTiLaserClass==LaserClassCW::laserClass::CLASSE_3B)or(myTiLaserClass==LaserClassCW::laserClass::CLASSE_4))
    {
        myTiClassData=ComputeLEA::ClassData::CLASSE_3B;
        tTiFormulaLEA_Label="Formula 3B";
        tTiLEA_Value_Label="LEA 3B";
    }
    else
    {
        myTiClassData=ComputeLEA::ClassData::SENZA_CLASSIFICA;
        tTiFormulaLEA_Label="NC";
        tTiLEA_Value_Label="NC";
    }

    tiFormulaLEA_Tipo=QString::fromStdString(MyLaserClassMP_Pr->getTiLEA_FormulaTipo()[static_cast<int>(myTiClassData)]);
    tiFormulaLEA=QString::fromStdString(MyLaserClassMP_Pr->getTiLEA_Formula()[static_cast<int>(myTiClassData)]);
    tiFormulaLEA_Unit=QString::fromStdString(MyLaserClassMP_Pr->getTiLEA_FormulaUnit()[static_cast<int>(myTiClassData)]);
    tiLEA_Value=QString::number(MyLaserClassMP_Pr->getTiLEA_Corrected()[static_cast<int>(myTiClassData)], 'e', 2);
    tiFormulaLEA_Label=QString("%1=%2 %3")
                             .arg(tiFormulaLEA_Tipo)
                             .arg(tiFormulaLEA)
                             .arg(tiFormulaLEA_Unit);

    tiLEA_Value_Label=QString("%1 %2")
                             .arg(tiLEA_Value)
                             .arg(tiFormulaLEA_Unit);

    tiPowerErgCond_1=QString::number(MyLaserClassMP_Pr->getTiPowerErg_Cond_1()[static_cast<int>(myTiClassData)], 'e', 2);
    tiPowerErgCond_3=QString::number(MyLaserClassMP_Pr->getTiPowerErg_Cond_3()[static_cast<int>(myTiClassData)], 'e', 2);

    QString tTiPowerErgCond_1_Label=QString("%1<sub>Acc 1</sub>").arg(tiFormulaLEA_Tipo);
    QString tTiPowerErgCond_3_Label=QString("%1<sub>Acc 3</sub>").arg(tiFormulaLEA_Tipo);

    if(isnan(MyLaserClassMP_Pr->getTiPowerErg_Cond_1()[static_cast<int>(myTiClassData)]))
        tiPowerErgCond_1_Label="N.A.";
    else
        tiPowerErgCond_1_Label=QString("%1 %2")
                                .arg(tiPowerErgCond_1)
                                .arg(tiFormulaLEA_Unit);

    tiPowerErgCond_3_Label=QString("%1 %2")
                             .arg(tiPowerErgCond_3)
                             .arg(tiFormulaLEA_Unit);

    QString thermalTiClassLabel=getLaserClassString(myTiLaserClass);

    LEA_Data.push_back(tTiLEA_Value_Label);       //40
    LEA_Data.push_back(tiFormulaLEA_Label);       //41
    LEA_Data.push_back(tiLEA_Value_Label);        //42
    LEA_Data.push_back(tTiPowerErgCond_1_Label);  //43
    LEA_Data.push_back(tiPowerErgCond_1_Label);   //44
    LEA_Data.push_back(tTiPowerErgCond_3_Label);  //45
    LEA_Data.push_back(tiPowerErgCond_3_Label);   //46
    LEA_Data.push_back(tiCouplingFactor1);        //47
    LEA_Data.push_back(tiCouplingFactor3);        //48
    LEA_Data.push_back(tiDiameterCond1);          //49
    LEA_Data.push_back(tiDiameterCond3);          //50
    LEA_Data.push_back(tiDistanceCond1);          //51
    LEA_Data.push_back(tiDistanceCond3);          //52
    LEA_Data.push_back(tiBeamAtStopCond1);        //53
    LEA_Data.push_back(tiBeamAtStopCond3);        //54
    LEA_Data.push_back(thermalTiClassLabel);      //55
}

void DockControls::on_internalWaist_checkBox_toggled(bool checked)
{
    QString whatThis;
    undoStack->clear();
    resetHistory();

    if(n_laser==operation::CONTINUOS_WAVE)
        MyLaserClassCW_Pr->setInternalWaist(checked);
    if(n_laser==operation::PULSE)
        MyLaserClassSP_Pr->setInternalWaist(checked);
    if(n_laser==operation::MULTI_PULSE)
        MyLaserClassMP_Pr->setInternalWaist(checked);

    internalWaist=checked;

    if(checked)
        whatThis="Waist interno non accessibile";
    else
        whatThis="Waist accessibile";

    set_LEA_Widgets();
    emit statusBarSignal(whatThis);
}

void DockControls::setGoggleMaterial(LaserGoggle::material myMaterial)
{
    goggleMaterial=myMaterial;
    myLaserGoggle->setMaterial(myMaterial);
    myDLaserGoggle->setMaterial(myMaterial);

    fetchDataVector();
    fetchLaserOutput();
    displayLaserOutput();
    displayTimeBase();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();

    fetchDDataVector();
    fetchDLaserOutput();

    displayDLaserOutput();
    displayDTimeBase();
}

LaserGoggle::material DockControls::getGoggleMaterial()const
{
    return goggleMaterial;
}


void DockControls::updateGoggle()
{  
    if(n_laser==operation::PULSE)
    {
        myLaserGoggle->setPulseWidth(pulseWidth);
        fetchLaserOutput();
        fetchDataVector();
    }

    else if(n_laser==operation::MULTI_PULSE)
    {
        myLaserGoggle->setWavelength(wavelength);
        myDLaserGoggle->setWavelength(wavelength);
        myLaserGoggle->setPulseWidth(pulseWidth);

        if(((wavelength>315) && (wavelength<=1e+06)))//base dei tempi 5 s
            myDLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE);
        else
        {
            if(((wavelength>=180) && (wavelength<=315)))//base dei tempi 30000 5
                myDLaserGoggle->setPulseWidth(LaserGoggle::TIMEBASE_LOW_WAVELENGTH);
        }

        modeLockedPeak();
        fetchDLaserOutput();
        fetchDDataVector();
    }
    fetchLaserOutput();
    fetchDataVector();
    setLaserGoggleWidgets();
}

vector<pair<int, double>> DockControls::getGoggleDataVect()const
{
    return myLaserGoggle->getDataVector();
}
vector<pair<int, double>> DockControls::getDGoggleDataVect()const
{
    return myDLaserGoggle->getDataVector();
}

int DockControls::getGoggleScaleNumber()const
{
    return myLaserGoggle->getScaleNumber();
}

int DockControls::getDGoggleScaleNumber()const
{
    return myDLaserGoggle->getScaleNumber();
}

std::string DockControls::getGoggleLimitsUnit()const
{
    return myLaserGoggle->getCodeUnit();
}

std::string DockControls::getDGoggleLimitsUnit()const
{
    return myDLaserGoggle->getCodeUnit();
}

void DockControls::leaExpressions_SP()
{
    if(n_laser==operation::PULSE)
        LEA_SP=MyLaserClassSP_Pr->getLEA_Expressions();
}

void DockControls::leaExpressions_CW()
{
    if(n_laser==operation::CONTINUOS_WAVE)
        LEA_CW=MyLaserClassCW_Pr->getLEA_Expressions();
}

void DockControls::leaExpressions_MP()
{
    if(n_laser==operation::MULTI_PULSE){
        LEA_SP_MultiPulse=MyLaserClassMP_Pr->getLEA_Expressions();
        LEA_Mean=MyLaserClassMP_Pr->getMeanLEA_Expressions();

        if(isThermal_LaserCLass())
        {
            if(!isHF_LaserCLass())
            LEA_Thermal=MyLaserClassMP_Pr->getThermalLEA_Expressions();
            else
            LEA_Thermal=MyLaserClassMP_Pr->getTiLEA_Expressions();
        }
    }
}

array<string, 4> DockControls::getLeaExpressions_CW()const
{
    return LEA_CW;
}
array<string, 4> DockControls::getLeaExpressions_SP()const
{
    return LEA_SP;
}

array<string, 4> DockControls::getLeaExpressions_SP_MultiPulse()const
{
    return LEA_SP_MultiPulse;
}

array<string, 4> DockControls::getLeaExpressions_Mean()const
{
    return LEA_Mean;
}

array<string, 4> DockControls::getLeaExpressions_Thermal()const
{
    return LEA_Thermal;
}

bool DockControls::isHF_LaserCLass()
{
    bool highFrequency;
    highFrequency=false;

    if(n_laser==operation::MULTI_PULSE)
    {
        if((wavelength>=400)and(wavelength<=1400))
        {
            double Ti= MyLaserClassMP_Pr->getTi();
            double PRF=MyLaserClassMP_Pr->getPRF();

            if(Ti>(1/PRF))
                highFrequency=true;
        }
    }
    return highFrequency;
}

bool DockControls::isThermal_LaserCLass()
{
    bool isThermal;
    isThermal=false;

    if(n_laser==operation::MULTI_PULSE)
    {
        if((wavelength>=400)and(wavelength<=1400))
            isThermal=true;
    }
    return isThermal;
}

DockControls::operation DockControls::laserOperation() const
{
    operation myLaserOperation;
    switch(n_laser)
    {
    case operation::CONTINUOS_WAVE:
        myLaserOperation=operation::CONTINUOS_WAVE;
        break;
    case operation::PULSE:
        myLaserOperation=operation::PULSE;
        break;
    case operation::MULTI_PULSE:
        myLaserOperation=operation::MULTI_PULSE;
        break;
    default:
        myLaserOperation=operation::NOT_WORKING;
    }

    return myLaserOperation;
}

DockControls::~DockControls()
{
    delete ui;
    delete myDLaserGoggle;
    delete myLaserGoggle;
    delete MyLaserSafetyMP;
    delete MyLaserSkinSafetyMP;
    delete MyLaserClassMP;
}

void DockControls::on_comboBoxBands_currentIndexChanged(int index)
{
    QString whatThis;
    if(index==0)
    {
        setUV();
        whatThis="Banda ultravioletta";
    }
    else
    if(index==1)
    {
        setVIS();
        whatThis="Banda visibile";
    }
    else
    if(index==2)
    {
        setIRA_NIR();
        whatThis="Banda del vicino infrarosso";
    }
    else
    if(index==3)
    {
       setIRB_SWIR();
        whatThis="Banda infrarossa ad onde corte";
    }
    else
    if(index==4)
    {
       setIRC_MWIR();
        whatThis="Banda infrarossa ad onde medie";
    }
    else
    if(index==5)
    {
       setIRC_LWIR();
       whatThis="Banda infrarossa ad onde lunghe";
    }
    else
    if(index==6)
    {
       setIRC_FIR();
       whatThis="Banda lontano infrarosso";
    }

    setWidgets();
    emit modified();
    emit statusBarSignal(whatThis);
}

void DockControls::setUndoStack(QUndoStack* _undoStack)
{
    undoStack=_undoStack;
    ui->powerErgControl->setUndoStack(undoStack);
    ui->pulseControl->setUndoStack(undoStack);
    ui->divergenceControl->setUndoStack(undoStack);
    ui->beamDiameterControl->setUndoStack(undoStack);
    ui->prfControl->setUndoStack(undoStack);
    ui->T_SkinControl->setUndoStack(undoStack);
    ui->teControl->setUndoStack(undoStack);
    ui->wavelengthScrollBar->setUndoStack(undoStack);
    ui->comboBoxBands->setUndoStack(undoStack);
    resetHistory();
}

void DockControls::resetHistory()
{
    ui->powerErgControl->setDialInitialValue();
    ui->powerErgControl->setScrollBarInitialValue();

    ui->pulseControl->setDialInitialValue();
    ui->pulseControl->setScrollBarInitialValue();

    ui->divergenceControl->setDialInitialValue();
    ui->divergenceControl->setScrollBarInitialValue();

    ui->beamDiameterControl->setDialInitialValue();
    ui->beamDiameterControl->setScrollBarInitialValue();

    ui->prfControl->setDialInitialValue();
    ui->prfControl->setScrollBarInitialValue();

    ui->T_SkinControl->setSliderInitialValue();
    ui->T_SkinControl->setScrollBarInitialValue();

    ui->teControl->setDialInitialValue();

    ui->wavelengthScrollBar->setScrollBarInitialValue();

    ui->comboBoxBands->setComboBoxInitialValue();
}

double DockControls::ordinaryExposureTime(const double &wavelength)
{
    double ordinaryExposureTime;
    if((wavelength>180)&&(wavelength<400))
        ordinaryExposureTime=3000;
    else if((wavelength>=400)&&(wavelength<=700))
        ordinaryExposureTime=0.25;
    else
        ordinaryExposureTime=10;

 return ordinaryExposureTime;
}

void DockControls::kindOfHazardChanged()
{
    if(n_laser==operation::MULTI_PULSE)
    {
    bool thermalHazard=(wavelength>=400)&&(wavelength<=1400);
    if(thermalHazard==isThermalHazard)
        return;

    isThermalHazard=thermalHazard;
    emit hazardChanged();
    }
}

void DockControls::setLEAModelForWavelength()
{
    if(n_laser==operation::MULTI_PULSE)
    {
        if(!isThermalHazard)
        {
            setDataForMP_Operation();
            setMP_LEAModel();
        }
        else
        {
            setDataForMP_Operation();
            setMP_LEAModel();
            selectMP_Thermal_Model();
        }
    }
}

void DockControls::verifyFeasibleRayleigh()
{
    BeamInspector::computeRayleighDistance(wavelength,
                                           beamDiameter,
                                           divergence);
    double qualityFactor=BeamInspector::getQualityFactor();
    bool noFeasibleRayleigh=(qualityFactor<1);
    QString whatThis;

    if(noFeasibleRayleigh)
    {
        whatThis="Hai inserito un ingresso non fisicamente realizzabile";
        ui->divergenceControl->setBackgroundColor("#8844ff");
        ui->beamDiameterControl->setBackgroundColor("#8844ff");
    }
    else
    {
        whatThis.clear();
        ui->divergenceControl->setBackgroundColor("");
        ui->beamDiameterControl->setBackgroundColor("");
    }

    ui->divergenceControl->setStatusTipHelp(whatThis);
    ui->beamDiameterControl->setStatusTipHelp(whatThis);

    emit statusBarSignal(whatThis);
}

void DockControls::verifyFeasibleDutyCycle()
{
    /********************************************************************************************
     * Anomalia nell'input dei parametri di ingresso.                                           *
     * Quando il prodotto della durata dell'impulso per la frequenza dell'impulso è maggiore    *
     * o uguale di 1 l'ingresso non è fisicamente realizzabile                                  *
     ********************************************************************************************/

    bool noFeasible=(prf*pulseWidth>=1);
    QString whatThis;

    if(noFeasible)
    {
        whatThis="Hai inserito un ingresso non fisicamente realizzabile";
        ui->prfControl->setBackgroundColor("#8844ff");
        ui->pulseControl->setBackgroundColor("#8844ff");
    }
    else
    {
        whatThis.clear();
        ui->prfControl->setBackgroundColor("");
        ui->pulseControl->setBackgroundColor("");
    }

    ui->prfControl->setStatusTipHelp(whatThis);
    ui->pulseControl->setStatusTipHelp(whatThis);

    emit noFeasibleDutyCycle();
    emit statusBarSignal(whatThis);
}

void DockControls::setDarkChartTheme(bool dark)
{       
    chartView->setViewTheme(dark);
    dChartView->setViewTheme(dark);
}
