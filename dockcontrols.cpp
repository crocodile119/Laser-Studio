#include "dockcontrols.h"
#include "ui_dockcontrols.h"
#include <QFile>
#include <QMessageBox>
#include <QtDebug>
#include <cmath>
#include <iostream>
#include <string>
#include <QAbstractItemModel>
#include <QListView>
#include "mypolarchartview.h"

const int DockControls::DOCKGOGGLEMINIMUN=405;
const int DockControls::DOCKGOGGLEMAXIMUN=550;

DockControls::DockControls(QWidget *parent, DockResults *_dockResults, DockEffects *_dockEffects,
                           DockSkin *_dockSkin, DockGoggle *_dockGoggle) : QDockWidget(parent), ui(new Ui::DockControls)
{
    ui->setupUi(this);

    dockResults=_dockResults;
    dockEffects=_dockEffects;
    dockSkin= _dockSkin;
    dockGoggle= _dockGoggle;

     /**********************************************************************************************
     * LaserGoggle ha due costruttori quello a 4 parametri che prevede il funzionamento            *
     * a frequenza nulla e il secondo con cinque parametri per il quale è specificata la           *
     * frequenza di ripetizione dgli impulsi.                                                      *
     * Inizializzo laser con un valore di frequenza nulla per uso in funzionamento CW o impulsato. *
     ***********************************************************************************************/


    //Impiego il costruttore con cinque parmetri wavelength, pulseWidth, powerErg, beamDiameter, frequenza

        myLaserGoggle=new LaserGoggle(633, 0, 1, 7, 0);
        string myNewGoggleMark = myLaserGoggle->goggleMark(633, 0, 1, 7, 0);

        vector<pair<int, double>> dataVector;
        dataVector=myLaserGoggle->getDataVector();
        laserOutput=myLaserGoggle->laserIrrRadCorrected(1);
        myModel=new ScaleNumbersModelView(this);
        myModel->setTableList(dataVector);
        dockGoggle->ui->tableView->setModel(myModel);

        chartView = new MyChartView(0, dataVector, laserOutput);
        chartView->setRenderHint(QPainter::Antialiasing);
        dockGoggle->ui->gridLayout_5->addWidget(chartView, 1, 0, Qt::AlignCenter);

        /************************************************************************************************
         * Essendo il funzionamento all'avvio di tipo Continuos Wave disabilito sia il controllo        *
         * dell'impulso che della frequenza di ripetizione degli impulsi.                               *
         ************************************************************************************************/

        ui->pulseControl->setEnabled(false);
        ui->prfControl->setEnabled(false);
        ui->peakControl->setEnabled(false);

        /************************************************************************************************
         * Imposto la base dei tempi nel funzionamento Continuos Wave all'avvio. La base dei tempi è    *
         * quella preveista dala norma EN207 e non ha relazione con la base dei tempi impiegata per il  *
         * calcolo dell'EMP dell'impulso che della frequenza di ripetizione degli impulsi.              *                *
         ************************************************************************************************/

        if((wavelength>=180)&&(wavelength<=315))
            myTimeBase=LaserGoggle::timeBaseLowWavelength;
                else
            if((wavelength>315)&&(wavelength<=1.0e+06))
                    myTimeBase=LaserGoggle::timeBaseWavelength;

        /************************************************************************************************
         * Inizializzo laser frequenza nulla per uso calcolare gli effetti medi nel caso di             *
         * funzionamento ad impulsi ripetuti                                                            *
         ************************************************************************************************/

        myDLaserGoggle=new LaserGoggle(632, myTimeBase, 0, 7);
        string myNewDGoggleMark = myDLaserGoggle->goggleMark(632, myTimeBase, 0, 7, 0);

        vector<pair<int, double>> frequencyDataVector;
        myDLaserGoggle->printVector(frequencyDataVector);
        frequencyDataVector=myDLaserGoggle->getDataVector();
        dLaserOutput=myDLaserGoggle->laserIrrRadCorrected(0);
        myDModel=new ScaleNumbersModelView(this);
        myDModel->setTableList(frequencyDataVector);
        dockGoggle->ui->dTableView->setModel(myDModel);

        dChartView = new MyChartView(0, frequencyDataVector, dLaserOutput);
        dChartView->setRenderHint(QPainter::Antialiasing);
        dockGoggle->ui->dGridLayout->addWidget(dChartView, 1, 0, Qt::AlignCenter);

        dockGoggle->ui->scaleNumberDLabel->setVisible(false);
        dockGoggle->ui->tScaleNumberDLabel->setVisible(false);
        dockGoggle->ui->n_maxLabel->setVisible(false);
        dockGoggle->ui->tn_maxLabel->setVisible(false);
        dockGoggle->ui->coefficient_kLabel->setVisible(false);
        dockGoggle->ui->tCoefficient_kLabel->setVisible(false);
        dockGoggle->ui->coefficient_kiLabel->setVisible(false);
        dockGoggle->ui->tCoefficient_kiLabel->setVisible(false);
        dockGoggle->ui->dLaserOutputLabel->setVisible(false);
        dockGoggle->ui->tDLaserOutputLabel->setVisible(false);
        dockGoggle->ui->numberOfPulseLabel->setVisible(false);
        dockGoggle->ui->tNumberOfPulseLabel->setVisible(false);
        dockGoggle->ui->meanPowerLabel->setVisible(false);
        dockGoggle->ui->tMeanPowerLabel->setVisible(false);

        dispayScaleNumber();
        displayLaserOutput();

        //inizializzo il laser per il calcolo dei livelli di emissione e la NOHD occhi
        n_laser=0;

        showControls(false);

        dockGoggle->setFixedWidth(DOCKGOGGLEMINIMUN);

        MyLaserCW_Pr=nullptr;
        MyLaserSP_Pr=nullptr;
        MyLaserMP_Pr=nullptr;
        exposureTime=0.25;
        gaussianBeam=true;
        beamCorrection=1.0;
        //Nel costruttore: PRF, BeamDiameter, PowerErg, Divergence, Wavelength, PulseWidth, Alpha
        MyLaserSafetyMP=new LaserSafetyMP(1, 7, 1, 1.5, 632, 0.25, 1.5);
        MyLaserCW_Pr=MyLaserSafetyMP;

        MyLaserSkinSP_Pr=nullptr;
        MyLaserSkinMP_Pr=nullptr;

        //Nel costruttore: PRF, ExposureTime, BeamDiameter, PowerErg, Divergence, Wavelength, PulseWidth, Alpha
        MyLaserSkinSafetyMP=new LaserSkinSafetyMP(1, 5.0, 7, 1, 1.5, 632, 10, 1.5);

        MyLaserSkinSP_Pr=MyLaserSkinSafetyMP;

        ui->comboBox->setCurrentIndex(1);

        /************************************************************************************************
         * Imposto i valori dei controlli con le relative variabili membro.                             *
         ************************************************************************************************/

        setDialControls();

        //in modo da aggiornare il valore di powerErgPeak alla prima variazione dei valore dei parametri
        powerErgPeak=0;

        connect(this, SIGNAL(EMP_Changed()), this, SLOT(setEMP()));
        connect(this, SIGNAL(powerErgForEMPChanged()), this, SLOT(setPowerErgForEMP()));
        connect(this, SIGNAL(beamDiameterChanged()), this, SLOT(setBeamDiameter()));
        connect(dockGoggle->ui->pushButton, SIGNAL(toggled(bool)), this, SLOT(on_pushButton_toggled(bool)));
}

void DockControls::on_pushButton_toggled(bool checked)
{
    double width;

    showControls(n_laser==2);

    if(checked){
        if(n_laser==2)
            dChartView->show();

        chartView->show();
        width=DOCKGOGGLEMINIMUN+450;

        dockGoggle->setFixedWidth(width);

        dockGoggle->ui->pushButton->setText("<<");
    }

    else{
        if(n_laser==2)
            dChartView->hide();

        chartView->hide();
        dockGoggle->setFixedWidth(DOCKGOGGLEMINIMUN);

        dockGoggle->ui->pushButton->setText(">>");
    }
}

void DockControls::on_wavelengthScrollBar_valueChanged(int value)
{
    /*******************************************************************
     * Assegno il valore associato al controllo alla variabile membro  *
     *******************************************************************/

    /*********************************
    * casting della variabile value. *
    **********************************/

    wavelength=(double)value;

    /*******************************************************************************
     * Imposto il valore per la visualizzazione nella label associata al controllo *
     *******************************************************************************/

    ui->wavelengthLabel->setText(QString::number(wavelength));

    /*****************
    * CONTINUOS WAVE *
    * ****************/

    if(n_laser==0)
    {
    /*****************************************
     * Imposto il valore negli oggetti Laser *
     *****************************************/

    MyLaserCW_Pr->setWavelength(wavelength);
    MyLaserSkinSP_Pr->setWavelength(wavelength);

    /**********************************************************************
     * Imposto il valore nell'oggetto occhiali protettori.                *
     * Il valore della base dei tempi dipende dalla lunghezza d'onda e va *
     * pertanto impostato in funzione di questa (EN 207 B.4).             *
     **********************************************************************/

    myLaserGoggle->setWavelength(wavelength);

    if(((myLaserGoggle->getWavelength()>315) && (myLaserGoggle->getWavelength()<=1e+06)))//base dei tempi 5 s
         {
             myLaserGoggle->setPulseWidth(LaserGoggle::timeBaseWavelength);
          }
        else
            if(((myLaserGoggle->getWavelength()>=180) && (myLaserGoggle->getWavelength()<=315)))//base dei tempi 30000 s
              {
                 myLaserGoggle->setPulseWidth(LaserGoggle::timeBaseLowWavelength);
              }

    /**************************************************************
     * La funzione fetchDataVector() va invocata quando cambia    *
     * la frequenza o la durata dell'impulso.                     *                                 *
     * Seleziona il campo corrispondente della Tabella B.2 EN207  *
     * per l'oggetto myLaserGoggle.                               *
     * Memorizza il vettore corrispondente in dataVector ed       *
     * aggiorna il grafico                                        *
     **************************************************************/

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();

    fetchLaserOutput();
    fetchDataVector();

    /*****************************************************
    * Visualizzazione dati relativi ai protettori ottici *
    ******************************************************/

    dispayScaleNumber();
    displayLaserOutput();
    displayTimeBase();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();
    }

    /*************
     * IMPULSATO *
     * ***********/

    if(n_laser==1)
    {

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/

    MyLaserSP_Pr->setWavelength(wavelength);
    MyLaserSkinSP_Pr->setWavelength(wavelength);

    /**********************************************************************
     * Imposto il valore nell'oggetto occhiali protettori.                *
     * Il valore della base dei tempi dipende dalla lunghezza d'onda e va *
     * pertanto impostato in funzione di questa (EN 207 B.4).             *
     * In questo caso si assume come tempo base la durata dell'impulso.   *
     **********************************************************************/

    myLaserGoggle->setWavelength(wavelength);
    myLaserGoggle->setPulseWidth(pulseWidth);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();

    fetchLaserOutput();

    /**************************************************************
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

    dispayScaleNumber();
    displayLaserOutput();
    displayTimeBase();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();
    }

    /********************
     * IMPULSI RIPETUTI *
     ********************/

    if(n_laser==2)
    {

    /*****************************************
     * Imposto il valore negli oggetti Laser *
     *****************************************/

     MyLaserMP_Pr->setWavelength(wavelength);
     MyLaserSkinMP_Pr->setWavelength(wavelength);


     /***********************************************************************
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
         {
             myDLaserGoggle->setPulseWidth(LaserGoggle::timeBaseWavelength);
          }
        else{
            if(((myDLaserGoggle->getWavelength()>=180) && (myDLaserGoggle->getWavelength()<=315)))//base dei tempi 30000 5
              {
                 myDLaserGoggle->setPulseWidth(LaserGoggle::timeBaseLowWavelength);
              }
            }
    /**************************************************************
     * La funzione fetchDataVector() va invocata quando cambia    *
     * la frequenza o la durata dell'impulso.                     *
     * Seleziona il campo corrispondente della Tabella B.2 EN207  *
     * per entrambi gli oggetti relativi ai protettori ottici.    *
     * Memorizza il vettore corrispondente in dataVector ed       *
     * aggiorna il grafico                                        *
     **************************************************************/

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();

    fetchLaserOutput();
    fetchDataVector();

    /*****************************************************
    * Visualizzazione dati relativi ai protettori ottici *
    ******************************************************/

    //Oggetto myLaserGoggle
    dispayScaleNumber();
    displayLaserOutput();
    displayTimeBase();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();

    //Oggetto myLaserGoggle
    fetchDDataVector();
    dispayDScaleNumber();
    displayDLaserOutput();
    displayDTimeBase();
    }

    /*******************************************
     * Imposto i widget per la visualizzazione *
     *******************************************/

    setWidgets();

    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/

    setOpticalDistance();
    setLambertianMax();
    setSkinDistances();

    emit EMP_Changed();//Cambia l'EMP
    emit wavelengthChanged();//la lunghezza d'onda è cambiata
    emit modified();//Per salvataggio file
}

void DockControls::on_teControl_valueChanged()
{
    if(isTeEdtitingEnabled())
    {
        exposureTime=ui->teControl->getDialNumber();
        /*****************
        * CONTINUOS WAVE *
        * ****************/

          if(n_laser==0)
            {
            /********************************************************
             * Imposto il valore del tempo di esposizione impostato *
             ********************************************************/
             MyLaserCW_Pr->setExposureTimeEditable(true);
             MyLaserCW_Pr->setEditedExposureTime(exposureTime);
             }
          else
          if(n_laser==2)
        /********************
         * IMPULSI RIPETUTI *
         ********************/
            {
            /********************************************************
             * Imposto il valore del tempo di esposizione impostato *
             ********************************************************/
             MyLaserMP_Pr->setExposureTimeEditable(true);
             MyLaserMP_Pr->setEditedExposureTime(exposureTime);
            }
    setWidgets();
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
    }
}

void DockControls::on_T_SkinControl_valueChanged()
{
    /********************************************
     * Ricavo il valore impostato dal controllo *
     ********************************************/

     int exponent=ui->T_SkinControl->getExponent();
     qDebug()<< "Esponente di T_SkinControl: "<<exponent;
     double mantissa=ui->T_SkinControl->getMantissa();

    /********************************************************************************+****
     * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
     *************************************************************************************/

     T_Skin=mantissa*powf(10, exponent);

    /*****************
    * CONTINUOS WAVE *
    * ****************/
    if(n_laser==0){

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSkinSP_Pr->setPulseWidth(T_Skin);
    }
    else

    /*************
     * IMPULSATO *
     *************/
     if(n_laser==1){
		 
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
     MyLaserSkinSP_Pr->setPulseWidth(pulseWidth);
     }
     else

   /********************
    * IMPULSI RIPETUTI *
    ********************/
    if (n_laser==2){

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSkinSP_Pr->setPulseWidth(pulseWidth);
    MyLaserSkinMP_Pr->setExposureTime(T_Skin);
    }

    /*******************************************
     * Imposto i widget per la visualizzazione *
     *******************************************/

    qDebug()<<"Tempo esposizione pelle: " << T_Skin;
    setWidgets();

    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/

    setSkinDistances();

    /******************
     * Emetto segnali *
     ******************/
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
}

void DockControls::on_peakControl_valueChanged()
{
    int exponent=ui->peakControl->getExponent();
    qDebug()<< "Esponente di peakControl: "<<exponent;
    double mantissa=ui->peakControl->getMantissa();
    powerErgPeak=mantissa*powf(10, exponent);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();

    fetchDataVector();
    fetchLaserOutput();

        /**************************************************************
         * Prelevo i dati relativi al laser per tracciare il grafico  *
         * relativo ai protettori ottici per l'oggetto myLaserGoggle *                          *
         **************************************************************/

        /**************************************************************
         * La funzione fetchDataVector() va invocata quando cambia    *
         * la frequenza o la durata dell'impulso.                     *                                 *
         * Seleziona il campo corrispondente della Tabella B.2 EN207  *
         * per l'oggetto myLaserGoggle.                               *
         * Memorizza il vettore corrispondente in dataVector ed       *
         * aggiorna il grafico                                        *
         **************************************************************/

        /*************************************************
         * Visualizzazione dati relativi a myLaserGoggle *
         *************************************************/
        dispayScaleNumber();
        displayLaserOutput();
        displayNumberOfPulse();
        displayCoefficient_k();
        displayCoefficient_ki();
        display_ni_max();

        /******************
         * Emetto segnali *
         ******************/
        emit modified();//Per salvataggio file
}

void DockControls::on_powerErgControl_valueChanged()
{
   /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/

    int exponent=ui->powerErgControl->getExponent();
    qDebug()<< "Esponente di powerErgControl: "<<exponent;
    double mantissa=ui->powerErgControl->getMantissa();

   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/

    powerErg=mantissa*powf(10, exponent);

   /******************
    * CONTINUOS WAVE *
    * ****************/
    if(n_laser==0){

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/

    MyLaserCW_Pr->setPowerErg(powerErg*beamCorrection);
    MyLaserSkinSP_Pr->setPowerErg(powerErg);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
    }
    else

    /************
    * IMPULSATO *
    * ***********/
    if (n_laser==1){

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSP_Pr->setPowerErg(powerErg*beamCorrection);
    MyLaserSkinSP_Pr->setPowerErg(powerErg);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
    }
    else

    /*******************
    * IMPULSI RIPETUTI *
    * ******************/
    if (n_laser==2){
		
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
	
        MyLaserMP_Pr->setPowerErg(beamCorrection*powerErg);
        MyLaserSkinMP_Pr->setPowerErg(powerErg);

        enablePeakControl();
        effectivePowerErgPeak();
        modeLockingPeak();

    /********************************************************************
    * Imposto il valore della potenza media nell'oggetto myDLaserGoggle *
    * data dal prodotto dell'energia dell'impulso per la frequenza      *
    *********************************************************************/

        myDLaserGoggle->setPowerErg(powerErg*myLaserGoggle->getFrequency());

    /*************************************************************
     * Seleziono il campo corrispondente della Tabella B.2 EN207 *
     * Memorizzo il vettore corrispondente in dataVector per     *
     * per l'oggetto myDLaserGoggle                              *
     *************************************************************/
        fetchDLaserOutput();

    /**************************************************************
     * Prelevo i dati relativi al laser per tracciare il grafico  *
     * relativo ai protettori ottici per l'oggetto myDLaserGoggle *                          *
     **************************************************************/
        fetchDDataVector();

    /**************************************************
     * Visualizzazione dati relativi a myDLaserGoggle *
     **************************************************/

        dispayDScaleNumber();
        displayDLaserOutput();
    }

    /**************************************************************
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

    /*************************************************
     * Visualizzazione dati relativi a myLaserGoggle *
     *************************************************/
    dispayScaleNumber();
    displayLaserOutput();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();

    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/
    setOpticalDistance();
    setSkinDistances();
    setLambertianMax();
    setPowerErgForEMP();

    /******************
     * Emetto segnali *
     ******************/
    emit powerErgChanged();//Cambia la potenza/Energia impulso del laser
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
}

void DockControls::fetchDataVector()
{
    /**************************************************************
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

void DockControls::fetchDDataVector()
{
    /**************************************************************
     * La funzione fetchDataVector() va invocata quando cambia    *
     * la frequenza o la durata dell'impulso.                     *                                 *
     * Seleziona il campo corrispondente della Tabella B.2 EN207  *
     * per l'oggetto myLaserGoggle.                               *
     * Memorizza il vettore corrispondente in dataVector ed       *
     * aggiorna il grafico                                        *
     **************************************************************/

    dLaserOutput=myDLaserGoggle->laserIrrRadCorrected(myLaserGoggle->getPowerErg()*myLaserGoggle->getFrequency());
    //Calcola e restituisce la marcatura memorizzandola in myNewGoggleMark
    string myNewGoggleMark = myDLaserGoggle->goggleMark();
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
   /**************************************************************
    * La funzione fetchLaserOutput() va invocata quando cambia   *
    * l'uscita dal laser (potenza/energia impulso, alpha,        *
    * frequenza, diametro del fascio, divergenza).               *
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
    /**************************************************************
     * La funzione fetchLaserOutput() va invocata quando cambia   *
     * l'uscita dal laser (potenza/energia impulso, alpha,        *
     * frequenza, diametro del fascio, divergenza).               *
     * Memorizza il vettore corrispondente in dataVector ed       *
     * aggiorna il grafico per myDLaserGoggle.                     *
     * IMPORTANTE: Non aggiorna il modello per la visualizzazione *
     * dei numeri scala ma solo il grafico                        *
     **************************************************************/
    QString meanPowerString = QString::number(myLaserGoggle->getPowerErg()*myLaserGoggle->getFrequency(), 'e', 2);
    dockGoggle->ui->tMeanPowerLabel->setText("Pm [W]");
    dockGoggle->ui->meanPowerLabel->setText(meanPowerString);
    dLaserOutput= myDLaserGoggle->laserIrrRadCorrected(myLaserGoggle->getPowerErg()*myLaserGoggle->getFrequency());
    dChartView->setLaserOutput(dLaserOutput);
    //Calcola e restituisce la marcatura memorizzandola in myNewGoggleMark
    string myNewGoggleMark = myDLaserGoggle->goggleMark();
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

    /********************************************************************************+****
     * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
     *************************************************************************************/
     pulseWidth=mantissa*powf(10, exponent);

	/**************************************************************************************
    * La modalità di funzionamento CONTINUOS WAVE è indipendente dalla durata dell'impuso *
    ***************************************************************************************/
	 
	/************
    * IMPULSATO *
    * ***********/
	//
      if(n_laser==1)
        {
	
	/*****************************************
     * Imposto il valore negli oggetti Laser *
     *****************************************/
         
			 MyLaserSP_Pr->setPulseWidth(pulseWidth);
			 MyLaserSkinSP_Pr->setPulseWidth(pulseWidth);
			 myLaserGoggle->setPulseWidth(pulseWidth);

             enablePeakControl();
             effectivePowerErgPeak();
             modeLockingPeak();

             /**************************************************************
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

                      dispayScaleNumber();
                      displayLaserOutput();
                      displayNumberOfPulse();
                      displayCoefficient_k();
                      displayCoefficient_ki();
                      display_ni_max();
                      displayTimeBase();
         }
	
	/********************
     * IMPULSI RIPETUTI *
     ********************/
	 
         else if(n_laser==2){
	/*****************************************
     * Imposto il valore negli oggetti Laser *
     *****************************************/
			 MyLaserMP_Pr->setPulseWidth(pulseWidth);
             MyLaserSkinMP_Pr->setPulseWidth(pulseWidth);
             myLaserGoggle->setPulseWidth(pulseWidth);
			 QString scaleNumberString = QString::fromStdString(myLaserGoggle->goggleMark());

             enablePeakControl();
             effectivePowerErgPeak();
             modeLockingPeak();

	/**************************************************************
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

             dispayScaleNumber();
             displayLaserOutput();
             displayNumberOfPulse();
             displayCoefficient_k();
             displayCoefficient_ki();
             display_ni_max();
             displayTimeBase();
		 }
		 
	/*******************************************
     * Imposto i widget per la visualizzazione *
     *******************************************/
		 setWidgets();
		 
	/*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/
         setOpticalDistance();
         setSkinDistances();
         setLambertianMax();

    /******************
     * Emetto segnali *
     ******************/
        emit EMP_Changed();//Cambia l'EMP
        emit modified();//Per salvataggio file
}

void DockControls::dispayScaleNumber()
{
    QString scaleNumberString = QString::fromStdString(myLaserGoggle->goggleMark());  
    dockGoggle->ui->tScaleNumberLabel->setText("LB");
    dockGoggle->ui->scaleNumberLabel->setText(scaleNumberString);
}

void DockControls::displayTimeBase()
{
    if(n_laser==0)
    {
    QString timeBaseString =QString::number(myLaserGoggle->getPulseWidth());
    dockGoggle->ui->tTimeBaseLabel->setText("T<sub>b</sub> [s]");
    dockGoggle->ui->timeBaseLabel->setText(timeBaseString);
    }
    if((n_laser==1)or(n_laser==2))
    {
        if((wavelength>=180)&&(wavelength<=315))
        {
            dockGoggle->ui->tTimeBaseLabel->setText("T<sub>b</sub> [s]");
            dockGoggle->ui->timeBaseLabel->setText(QString::number(LaserGoggle::timeBaseLowWavelength));
        }
                else
        {
            dockGoggle->ui->tTimeBaseLabel->setText("T<sub>b</sub> [s]");
            dockGoggle->ui->timeBaseLabel->setText(QString::number(LaserGoggle::timeBaseWavelength));
        }
    }
}

void DockControls::displayDTimeBase()
{
    if(n_laser==2)
    {
    QString dTimeBaseString = QString::number(myDLaserGoggle->getPulseWidth());
    dockGoggle->ui->tDTimeBaseLabel->setText("T<sub>b</sub> [s]");
    dockGoggle->ui->dTimeBaseLabel->setText(dTimeBaseString);
    }
}

void DockControls::displayLaserOutput()
{
    QString laserOutputString= QString::number(laserOutput,'e',2);
    QString myUnitCodeString = QString::fromStdString(myLaserGoggle->getCodeUnit());
    QString myOutput=QString::fromStdString(myLaserGoggle->outputSort());
    dockGoggle->ui->tLaserOutputLabel->setText(myOutput+" " + myUnitCodeString);
    dockGoggle->ui->laserOutputLabel->setText(laserOutputString);
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

void DockControls::displayDLaserOutput()
{
    dLaserOutput= myDLaserGoggle->laserIrrRadCorrected(myLaserGoggle->getPowerErg()*myLaserGoggle->getFrequency());
    QString dLaserOutputString= QString::number(dLaserOutput,'e',2);
    QString myDUnitCodeString = QString::fromStdString(myDLaserGoggle->getCodeUnit());
    QString myDOutput=QString::fromStdString(myDLaserGoggle->outputSort());
    dockGoggle->ui->tDLaserOutputLabel->setText(myDOutput+"m " + myDUnitCodeString);
    dockGoggle->ui->dLaserOutputLabel->setText(dLaserOutputString);
}

void DockControls::display_ni_max()
{
    QString ni ="ν";
    QString ni_maxString = QString::number(myLaserGoggle->get_ni_max(), 'e', 2);
    dockGoggle->ui->tn_maxLabel->setText(ni.toHtmlEscaped()+"<sub>max</sub> [Hz]");
    dockGoggle->ui->n_maxLabel->setText(ni_maxString);
}

void DockControls::dispayDScaleNumber()
{
    QString scaleNumberDString = QString::fromStdString(myDLaserGoggle->goggleMark());
    dockGoggle->ui->tScaleNumberDLabel->setText("LB");
    dockGoggle->ui->scaleNumberDLabel->setText(scaleNumberDString);
}


void DockControls::displayOpticalDensity()
{

}

void DockControls::displayDOpticalDensity()
{

}

void DockControls::on_prfControl_valueChanged()
{
   /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
	
    int exponent=ui->prfControl->getExponent();
    qDebug()<< "Esponente di prfControl: "<<exponent;
    double mantissa=ui->prfControl->getMantissa();

   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/

    prf=mantissa*powf(10, exponent);

    if(n_laser==2)
    {
		
    /*******************
    * IMPULSI RIPETUTI *
    * ******************/
	
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
	
    MyLaserMP_Pr->setPRF(prf);
    MyLaserSkinMP_Pr->setPRF(prf);
    myLaserGoggle->setFrequency(prf);

    /********************************************************************
    * Imposto il valore della potenza media nell'oggetto myDLaserGoggle *
    * data dal prodotto dell'energia dell'impulso per la frequenza      *
    *********************************************************************/

    myDLaserGoggle->setPowerErg(myLaserGoggle->getPowerErg()*prf);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();

    /*************************************************************
     * Seleziono il campo corrispondente della Tabella B.2 EN207 *
     * Memorizzo il vettore corrispondente in dataVector per     *
     * per l'oggetto myDLaserGoggle                              *
     *************************************************************/
    fetchDLaserOutput();

    /**************************************************************
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
	
    /**************************************************************
     * Prelevo i dati relativi al laser per tracciare il grafico  *
     * relativo ai protettori ottici per l'oggetto myDLaserGoggle *                          *
     **************************************************************/
    fetchDataVector();
	

    /**************************************************
     * Visualizzazione dati relativi a myLaserGoggle *
     **************************************************/
    dispayScaleNumber();
    displayDLaserOutput();
    displayLaserOutput();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();

    /**************************************************
     * Visualizzazione dati relativi a myDLaserGoggle *
     **************************************************/
    dispayDScaleNumber();
    displayDLaserOutput();

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();

    /*******************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/	
    setOpticalDistance();
    setSkinDistances();
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
   /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
    int exponent=ui->beamDiameterControl->getExponent();
    qDebug()<< "Esponente di beamDiameterControl: "<<exponent;
    double mantissa=ui->beamDiameterControl->getMantissa();

   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/
    beamDiameter=mantissa*powf(10, exponent);

   /******************
    * CONTINUOS WAVE *
    * ****************/	
    if(n_laser==0){
		
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserCW_Pr->setBeamDiameter(beamDiameter);
    MyLaserSkinSP_Pr->setBeamDiameter(beamDiameter);
	myLaserGoggle->setBeamDiameter(beamDiameter);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
    }
    else
		
    /************
    * IMPULSATO *
    * ***********/
    if (n_laser==1){
	
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
	MyLaserSP_Pr->setBeamDiameter(beamDiameter);
	MyLaserSkinSP_Pr->setBeamDiameter(beamDiameter);
	myLaserGoggle->setBeamDiameter(beamDiameter);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
	}
		
    /*******************
    * IMPULSI RIPETUTI *
    * ******************/
    else if (n_laser==2){
	
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserMP_Pr->setBeamDiameter(beamDiameter);
    MyLaserSkinMP_Pr->setBeamDiameter(beamDiameter);
	myLaserGoggle->setBeamDiameter(beamDiameter);
    myDLaserGoggle->setBeamDiameter(beamDiameter);

    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
	
    /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
	* myDLaserGoggle                                                                          *
	******************************************************************************************/
    fetchDLaserOutput();
    fetchDDataVector();
    dispayDScaleNumber();
    displayDLaserOutput();
    }
	
    /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
	* myLaserGoggle                                                                          *
	******************************************************************************************/
    fetchLaserOutput();
    fetchDataVector();
    dispayScaleNumber();
    displayLaserOutput();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();


   /********************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/
    setOpticalDistance();
    setSkinDistances();
    setLambertianMax();

   /******************
    * Emetto segnali *
    ******************/
    emit beamDiameterChanged();//Cambia il diametro del fascio
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
}

void DockControls::showControls(bool _show)
{
	/****************************************************
	* Visualizza le label relative ai protettori ottici *
	*****************************************************/
    bool details=dockGoggle->ui->pushButton->isChecked();

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
                                    if (_show)
                                    {
                                        if(details)
                                            dChartView->show();

                                        dockGoggle->ui->page_2->show();
                                     }
                                    else
                                    {
                                        if(details)
                                            dChartView->hide();

                                        dockGoggle->ui->page_2->hide();
                                    }
}

void DockControls::on_alphaControl_valueChanged()
{
   /********************************************
    * Ricavo il valore impostato dal controllo *
    ********************************************/
    int exponent=ui->alphaControl->getExponent();
    qDebug()<< "Esponente di alphaControl: "<<exponent;
    double mantissa=ui->alphaControl->getMantissa();
	
   /********************************************************************************+****
    * Memorizzo il valore impostato dal controllo nella variabile membro corrispondente *
    *************************************************************************************/
    alpha=mantissa*powf(10, exponent);
	
   /******************
    * CONTINUOS WAVE *
    * ****************/	
    if(n_laser==0)
    {   

	/*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserCW_Pr->setAlpha(alpha);
    MyLaserSkinSP_Pr->setAlpha(alpha);
    }
    else
   /*************
    * IMPULSATO *
    * ***********/
    if(n_laser==1)
    {
	/*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSP_Pr->setAlpha(alpha);
    MyLaserSkinSP_Pr->setAlpha(alpha);
    }
    else
    /*******************
    * IMPULSI RIPETUTI *
    * ******************/
    if(n_laser==2)
    {
	/*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserMP_Pr->setAlpha(alpha);
    MyLaserSkinMP_Pr->setAlpha(alpha);
    }

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/	
    setWidgets();
	
   /********************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/	
    setOpticalDistance();
    setSkinDistances();
    setLambertianMax();
	
   /******************
    * Emetto segnali *
    ******************/
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file
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
    divergence=mantissa*powf(10, exponent);
	
   /******************	
    * CONTINUOS WAVE *
    * ****************/	
    if(n_laser==0)
    {
	/*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserCW_Pr->setDivergence(divergence);
    MyLaserSkinSP_Pr->setDivergence(divergence);
    }
    else
	/*************
    * IMPULSATO *
    * ***********/
    if(n_laser==1)
    {
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSP_Pr->setDivergence(divergence);
    MyLaserSkinSP_Pr->setDivergence(divergence);
    }
    else
   /********************
    * IMPULSI RIPETUTI *
    * ******************/
    if(n_laser==2)
    {
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserMP_Pr->setDivergence(divergence);
    MyLaserSkinMP_Pr->setDivergence(divergence);
    }
	
   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();
	
   /********************************************************
    * Imposto altri valori necessarie per la parte grafica *
    ********************************************************/	
    setOpticalDistance();
    setSkinDistances();
    setLambertianMax();
	
   /******************
    * Emetto segnali *
    ******************/
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
    QString minEMP;
    QString empSkinUnitMP;
    QString minEMPSkin;  
    QString skinPowerErgUnit="Energia [J]";

    if(n_laser==0)
        {
        ui->powerErgControl->setTitle("Potenza [W]");
        MyLaserCW_Pr->laserUpdate();
        MyLaserSkinSP_Pr->laserSkinUpdate();

            if(MyLaserCW_Pr->getFormulaSort()=="E")
            {
                powerErgUnit="Potenza [W]= ";
                    empUnit="[W/m<sup>2</sup>]";
            }
            else
                if(MyLaserCW_Pr->getFormulaSort()=="H")
                {
                    powerErgUnit="Energia [J]= ";
                    empUnit="[J/m<sup>2</sup>]";
                }
        /********************************************************
         * Results widgets                                      *
         ********************************************************/

        //Label non visibili
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

        dockResults->ui->tExposureTimeLabel->setVisible(true);
        dockResults->ui->ExposureTimeLabel->setVisible(true);

        dockResults->ui->tminEMP_Label->setVisible(false);
        dockResults->ui->minEMP_Label->setVisible(false);

        dockResults->ui->tEMP_Label->setText((QString)MyLaserCW_Pr->getFormulaSort().c_str() + " " + empUnit);
        dockResults->ui->EMP_Label->setText(QString::number(MyLaserCW_Pr->getEMP(),'e', 2));

        dockResults->ui->tExposureTimeLabel->setText("T<sub>e</sub> [s]");
        dockResults->ui->ExposureTimeLabel->setText(QString::number(MyLaserCW_Pr->getExposureTime(),'e', 2));

        dockResults->ui->tPowerErgLabel->setText("Potenza [W]");
        dockResults->ui->PowerErgLabel->setText(QString::number(MyLaserCW_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tFormulaLabel->setText((QString)"Formula");
        dockResults->ui->FormulaLabel->setText(MyLaserCW_Pr->getFormulaEMP().c_str());

        dockResults->ui->tNOHDLabel->setText((QString)"DNRO [m]");

        bool DNRO_scientNot;
        DNRO_scientNot=MyLaserCW_Pr->getNOHD()>1.0e+03;
        if(DNRO_scientNot)
            dockResults->ui->NOHDLabel->setText(QString::number(MyLaserCW_Pr->getNOHD(),'e', 2));
            else
            dockResults->ui->NOHDLabel->setText(QString::number(MyLaserCW_Pr->getNOHD(),'f', 1));

        dockResults->ui->conditions_Label->setText("EMP \nper esposizioni\nda onda continua");
        dockResults->ui->tEMP_1st_Label->setVisible(false);
        dockResults->ui->tEMP_2nd_Label->setVisible(false);
        dockResults->ui->tEMP_3rd_Label->setVisible(false);

        QString EMP_CW_Pulse=QString(" %1 = %2 %3")
                                        .arg(MyLaserCW_Pr->getFormulaEMP().c_str())
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

        /********************************************************
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
        dockEffects->ui->RadiationLabel->setText(MyLaserCW_Pr->getRadiation().c_str());

        dockEffects->ui->tSkinDamageLabel->setText("Danno alla cute");
        dockEffects->ui->SkinDamageLabel->setText(MyLaserCW_Pr->getSkinDamage().c_str());

        dockEffects->ui->tEyeDamageLabel->setText("Danno all'occhio");
        dockEffects->ui->EyeDamageLabel->setText(MyLaserCW_Pr->getEyeDamage().c_str());

        dockEffects->ui->tNoteLabel->setText("Note");
        dockEffects->ui->NoteLabel->setText(MyLaserCW_Pr->getPhotochemicalNote().c_str());

        /*******************
         * reset controlli *
         *******************/

        ui->prfControl->setBackgroundColor("#e0e0e0");
        ui->pulseControl->setBackgroundColor("#e0e0e0");
        ui->prfControl->setBackgroundColor("#e0e0e0");
        ui->pulseControl->setBackgroundColor("#e0e0e0");
        ui->prfControl->setStatusTipHelp("");
        ui->pulseControl->setStatusTipHelp("");

        //Skin widgets
        setSkinWidgetsSingle();
        }

else
     if(n_laser==1)
        {
        ui->powerErgControl->setTitle("Energia [J]");
        MyLaserSP_Pr->laserUpdate();
        MyLaserSkinSP_Pr->laserSkinUpdate();

             if(MyLaserSP_Pr->getFormulaSort()=="E")
             {
                 powerErgUnit="Potenza [W]= ";
                     empUnit="[W/m<sup>2</sup>]";
             }
             else
                 if(MyLaserSP_Pr->getFormulaSort()=="H")
                 {
                     powerErgUnit="Energia [J]= ";
                     empUnit="[J/m<sup>2</sup>]";
                 }

       /********************************************************
        * Results widgets                                      *
        ********************************************************/

        //Label non visibili
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

        dockResults->ui->tExposureTimeLabel->setVisible(false);
        dockResults->ui->ExposureTimeLabel->setVisible(false);

        dockResults->ui->tminEMP_Label->setVisible(false);
        dockResults->ui->minEMP_Label->setVisible(false);

        dockResults->ui->tEMP_MP_Label->setVisible(false);
        dockResults->ui->EMP_MP_Label->setVisible(false);

        dockResults->ui->tEMP_Label->setText((QString)MyLaserSP_Pr->getFormulaSort().c_str() + " " + empUnit);
        dockResults->ui->EMP_Label->setText(QString::number(MyLaserSP_Pr->getEMP(),'e', 2));

        dockResults->ui->tExposureTimeLabel->setText("T<sub>e</sub> [s]");
        dockResults->ui->ExposureTimeLabel->setText(QString::number(MyLaserSP_Pr->getPulseWidth(),'e', 2));

        dockResults->ui->tPowerErgLabel->setText("Energia [J]");
        dockResults->ui->PowerErgLabel->setText(QString::number(MyLaserSP_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tFormulaLabel->setText((QString)"Formula");
        dockResults->ui->FormulaLabel->setText(MyLaserSP_Pr->getFormulaEMP().c_str());

        dockResults->ui->tNOHDLabel->setText((QString)"DNRO [m]");

        bool DNRO_scientNot;
        DNRO_scientNot=MyLaserSP_Pr->getNOHD()>1.0e+03;
        if(DNRO_scientNot)
            dockResults->ui->NOHDLabel->setText(QString::number(MyLaserSP_Pr->getNOHD(),'e', 2));
            else
            dockResults->ui->NOHDLabel->setText(QString::number(MyLaserSP_Pr->getNOHD(),'f', 1));

        dockResults->ui->conditions_Label->setText("EMP \nper esposizioni\nda impulso singolo");
        dockResults->ui->tEMP_1st_Label->setVisible(false);
        dockResults->ui->tEMP_2nd_Label->setVisible(false);
        dockResults->ui->tEMP_3rd_Label->setVisible(false);

        QString EMP_ONE_Pulse=QString(" %1 = %2 %3")
                                        .arg(MyLaserSP_Pr->getFormulaEMP().c_str())
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

        dockEffects->ui->RadiationLabel->setText(MyLaserSP_Pr->getRadiation().c_str());
        dockEffects->ui->SkinDamageLabel->setText(MyLaserSP_Pr->getSkinDamage().c_str());
        dockEffects->ui->EyeDamageLabel->setText(MyLaserSP_Pr->getEyeDamage().c_str());
        dockEffects->ui->NoteLabel->setText(MyLaserSP_Pr->getPhotochemicalNote().c_str());

        //Skin widgets
        setSkinWidgetsSingle();

        /*******************
         * reset controlli *
         *******************/

        ui->prfControl->setBackgroundColor("#e0e0e0");
        ui->pulseControl->setBackgroundColor("#e0e0e0");
        ui->prfControl->setBackgroundColor("#e0e0e0");
        ui->pulseControl->setBackgroundColor("#e0e0e0");
        ui->prfControl->setStatusTipHelp("");
        ui->pulseControl->setStatusTipHelp("");
    }
else
if(n_laser==2)
    {
        ui->powerErgControl->setTitle("Energia [J]");
        MyLaserMP_Pr->laserUpdate();
        MyLaserSkinMP_Pr->laserSkinUpdate();

             if(MyLaserMP_Pr->getFormulaSort()=="E")
             {
                 powerErgUnit="Potenza [W]= ";
                 empUnit="[W/m<sup>2</sup>]";
                 minEMP="E<sub>min</sub> [W/m<sup>2</sup>]";
                 thermalEMP="Eth [W/m<sup>2</sup>]";
             }
             else
                 if(MyLaserMP_Pr->getFormulaSort()=="H")
                 {
                     powerErgUnit="Energia [J]= ";
                     empUnit="[J/m<sup>2</sup>]";
                     minEMP="H<sub>min</sub> [J/m<sup>2</sup>]";                   
                     thermalEMP="H<sub>th</sub> [J/m<sup>2</sup>]";
                 }


             if(MyLaserMP_Pr->getMeanPowerFormulaSort()=="E")
             {
                     empUnitMP="[W/m<sup>2</sup>]";
             }
             else
                 if(MyLaserMP_Pr->getMeanPowerFormulaSort()=="H")
                 {
                     empUnitMP="[J/m<sup>2</sup>]";
                 }

        /********************************************************
        * Results widgets                                      *
        ********************************************************/

        //Label non visibili
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

        dockResults->ui->tThermalEMP_Label->setVisible(true);
        dockResults->ui->ThermalEMP_Label->setVisible(true);

        dockResults->ui->tCountingLabel->setVisible(true);
        dockResults->ui->CountingLabel->setVisible(true);

        dockResults->ui->tCP_Label->setVisible(true);
        dockResults->ui->CP_Label->setVisible(true);

        dockResults->ui->tPulseNumberLabel->setVisible(true);
        dockResults->ui->PulseNumberLabel->setVisible(true);

        dockResults->ui->tminEMP_Label->setVisible(true);
        dockResults->ui->minEMP_Label->setVisible(true);

        dockResults->ui->conditionsFrame->setVisible(true);

        dockSkin->ui->tPowerErgSkinLabel->setText(skinPowerErgUnit);
        dockSkin->ui->powerErgSkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tEMP_MP_Label->setText((QString)MyLaserMP_Pr->getMeanPowerFormulaSort().c_str() + "<sub>Te</sub> " + empUnitMP);
        dockResults->ui->EMP_MP_Label->setText(QString::number(MyLaserMP_Pr->getEMP_MP(),'e', 2));

        dockResults->ui->tEMP_Label->setText((QString)MyLaserMP_Pr->getFormulaSort().c_str() + " " + empUnit);
        dockResults->ui->EMP_Label->setText(QString::number(MyLaserMP_Pr->getEMP(),'e', 2));

        dockResults->ui->tExposureTimeLabel->setText("T<sub>e</sub> [s]");
        dockResults->ui->ExposureTimeLabel->setText(QString::number(MyLaserMP_Pr->getExposureTime(),'e', 2));

        dockResults->ui->tPowerErgLabel->setText("Energia [J]");
        dockResults->ui->PowerErgLabel->setText(QString::number(MyLaserMP_Pr->getPowerErg(),'e', 2));

        dockResults->ui->tFormulaLabel->setText("Formula");
        dockResults->ui->FormulaLabel->setText(MyLaserMP_Pr->getFormulaEMP().c_str());

        dockResults->ui->tFormulaMP_Label->setText("Formula T<sub>e</sub>");
        dockResults->ui->FormulaMP_Label->setText(MyLaserMP_Pr->getMeanPowerFormulaEMP().c_str());

        dockResults->ui->tMeanPowerLabel->setText("P<sub>m</sub> [W]");
        dockResults->ui->MeanPowerLabel->setText(QString::number(MyLaserMP_Pr->getMeanPower(),'e', 2));

        dockResults->ui->tMeanIrradianceLabel->setText("E<sub>m</sub> [W/m<sup>2</sup>]");
        dockResults->ui->MeanIrradianceLabel->setText(QString::number(MyLaserMP_Pr->getMeanIrradiance(),'e', 2));

        dockResults->ui->tThermalEMP_Label->setText(thermalEMP);

        if((wavelength>=400)and(wavelength<=1.0e+06))
        {
        dockResults->ui->ThermalEMP_Label->setText(QString::number(MyLaserMP_Pr->getThermalEMP(),'e', 2));
        }
        else{
        dockResults->ui->ThermalEMP_Label->setText("Non applicabile");
        }

        dockResults->ui->tCountingLabel->setText("Conteggio");
        dockResults->ui->CountingLabel->setText(MyLaserMP_Pr->valutateCounting().c_str());

        dockResults->ui->tCP_Label->setText("C<sub>p</sub>");

        if((wavelength>=400)and(wavelength<=1.0e+06)){
            dockResults->ui->CP_Label->setText(QString::number(MyLaserMP_Pr->getCPCoefficient(),'e', 2));}
            else{
            dockResults->ui->CP_Label->setText("Non applicabile");}


        dockResults->ui->tPulseNumberLabel->setText("Numero impulsi");
        dockResults->ui->PulseNumberLabel->setText(QString::number(MyLaserMP_Pr->getPulseNumber()));

        dockResults->ui->tminEMP_Label->setText(minEMP);
        dockResults->ui->minEMP_Label->setText(QString::number(MyLaserMP_Pr->returnMultiPulse_EMP(),'e', 2));

        dockResults->ui->tNOHDLabel->setText("DNRO [m]");

        bool DNRO_scientNot;
        DNRO_scientNot=MyLaserMP_Pr->getNOHD()>1.0e+03;
        if(DNRO_scientNot)
            dockResults->ui->NOHDLabel->setText(QString::number(MyLaserMP_Pr->getNOHD(),'e', 2));
            else
            dockResults->ui->NOHDLabel->setText(QString::number(MyLaserMP_Pr->getNOHD(),'f', 1));


        dockResults->ui->conditions_Label->setText("EMP \nper esposizioni\nripetute");

        dockResults->ui->tEMP_1st_Label->setVisible(true);
        dockResults->ui->tEMP_2nd_Label->setVisible(true);
        dockResults->ui->tEMP_3rd_Label->setVisible(true);

        dockResults->ui->tEMP_1st_Label->setText("1<sup>a</sup>");      
        dockResults->ui->tEMP_2nd_Label->setText("2<sup>a</sup>");
        dockResults->ui->tEMP_3rd_Label->setText("3<sup>a</sup>");

        QString firstCondition=QString(" %1 = %2 %3")
                                        .arg(MyLaserMP_Pr->getFormulaEMP().c_str())
                                        .arg(QString::number(MyLaserMP_Pr->getEMP_1stCondition(),'e', 2))
                                        .arg(empUnit);

        dockResults->ui->EMP_1st_Label->setVisible(true);
        dockResults->ui->EMP_1st_Label->setText(firstCondition);

        QString formulaEMP;
            formulaEMP=QString::fromStdString(MyLaserMP_Pr->getMeanPowerFormulaEMP()+"/N");

        QString secondCondition=QString(" %1 = %2 %3")
                                        .arg(formulaEMP)
                                        .arg(QString::number(MyLaserMP_Pr->getEMP_2ndCondition(),'e', 2))
                                        .arg(empUnitMP);

        dockResults->ui->EMP_2nd_Label->setVisible(true);
        dockResults->ui->EMP_2nd_Label->setText(secondCondition);

        QString thirdCondition;

        if((wavelength>=400)and(wavelength<=1.0e+06)){
            QString  empUnitTmin;

            if(MyLaserMP_Pr->getFormulaSort_3rdCondition()=="E")
                empUnitTmin="[W/m<sup>2</sup>]";
                        else
            if(MyLaserMP_Pr->getFormulaSort_3rdCondition()=="H")
                 empUnitTmin="[J/m<sup>2</sup>]";

            thirdCondition=QString(" %1 C<sub>p</sub> = %2  %3")
                                            .arg(MyLaserMP_Pr->getFormula_3rdCondition().c_str())
                                            .arg(QString::number(MyLaserMP_Pr->getEMP_3rdCondition(),'e', 2))
                                            .arg(empUnitTmin);
        }
        else
        {
            thirdCondition="Non applicabile";
        }


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

        dockEffects->ui->RadiationLabel->setText(MyLaserMP_Pr->getRadiation_MP().c_str());
        dockEffects->ui->SkinDamageLabel->setText(MyLaserMP_Pr->getSkinDamage_MP().c_str());
        dockEffects->ui->EyeDamageLabel->setText(MyLaserMP_Pr->getEyeDamage_MP().c_str());

        if(MyLaserMP_Pr->getGamma_MP()==0)
          dockEffects->ui->GammaLabel->setText("Non applicabile");
            else
          dockEffects->ui->GammaLabel->setText(QString::number(MyLaserMP_Pr->getGamma_MP(),'e', 2));

        dockEffects->ui->NoteLabel->setText(MyLaserMP_Pr->getPhotochemicalNote_MP().c_str());


        /********************************************************************************************
         * Anomalia nell'input dei parametri di ingresso.                                           *
         * Quando il prodotto della durata dell'impulso per la frequenza dell'impulso è maggiore    *
         * o uguale di 1 l'ingresso non è fisicamente realizzabile                                  *
         ********************************************************************************************/

        bool noFeasible=(prf*pulseWidth>=1);

        if(noFeasible)
        {
            ui->prfControl->setBackgroundColor("#202020");
            ui->pulseControl->setBackgroundColor("#202020");

            ui->prfControl->setStatusTipHelp("Hai inserito un ingresso "
                                            " non fisicamente realizzabile");
            ui->pulseControl->setStatusTipHelp("Hai inserito un ingresso "
                                            " non fisicamente realizzabile");
            emit noFeasibleInput();
        }
        else
        {
            ui->prfControl->setBackgroundColor("#e0e0e0");
            ui->pulseControl->setBackgroundColor("#e0e0e0");
            ui->prfControl->setStatusTipHelp("");
            ui->pulseControl->setStatusTipHelp("");
        }

        /********************************************************
        * Widget pelle                                          *
        *********************************************************/

                 if(MyLaserSkinMP_Pr->getFormulaSort()=="E")
                 {
                         empSkinUnit="[W/m<sup>2</sup>]";
                         minEMPSkin="E<sub>min</sub> [W/m<sup>2</sup>]";
                 }
                 else
                     if(MyLaserSkinMP_Pr->getFormulaSort()=="H")
                     {
                         empSkinUnit="[J/m<sup>2</sup>]";
                         minEMPSkin="H<sub>min</sub> [W/m<sup>2</sup>]";
                     }

                 if(MyLaserSkinMP_Pr->getMeanPowerFormulaSort()=="E")
                 {
                         empSkinUnitMP="[W/m<sup>2</sup>]";
                 }
                 else
                     if(MyLaserMP_Pr->getMeanPowerFormulaSort()=="H")
                     {
                         empSkinUnitMP="[J/m<sup>2</sup>]";
                     }

            dockSkin->ui->tFormulaSkinLabel->setText("Formula");
            dockSkin->ui->FormulaSkinLabel->setText(MyLaserSkinMP_Pr->getFormulaEMP().c_str());

            dockSkin->ui->tEMP_SkinLabel->setText(MyLaserSkinMP_Pr->getFormulaSort().c_str() + empSkinUnit);
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

            dockSkin->ui->tEMP_MP_SkinLabel->setText((QString)MyLaserSkinMP_Pr->getMeanPowerFormulaSort().c_str() + "<sub>Te</sub> " + empSkinUnitMP);
            dockSkin->ui->EMP_MP_SkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getEMP_MP(),'e', 2));

            dockSkin->ui->tFormulaSkinMP_Label->setText("Formula T<sub>e</sub>");
            dockSkin->ui->FormulaSkinMP_Label->setText(MyLaserSkinMP_Pr->getMeanPowerFormulaEMP().c_str());

            dockSkin->ui->tMeanPowerSkinLabel->setText("P<sub>m</sub> [W]");
            dockSkin->ui->MeanPowerSkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getMeanPower(),'e', 2));

            dockSkin->ui->tMeanIrradianceSkinLabel->setText("E<sub>m</sub> [W/m<sup>2</sup>]");
            dockSkin->ui->MeanIrradianceSkinLabel->setText(QString::number(MyLaserSkinMP_Pr->getMeanIrradiance(),'e', 2));

            dockSkin->ui->tPulseNumberSkinLabel->setText("Numero impulsi");
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
                                            .arg(MyLaserSkinMP_Pr->getFormulaEMP().c_str())
                                            .arg(QString::number(MyLaserSkinMP_Pr->getEMP_1stCondition(),'e', 2))
                                            .arg(empSkinUnit);

            QString formulaSkinEMP;
            formulaSkinEMP=QString::fromStdString(MyLaserSkinMP_Pr->getMeanPowerFormulaEMP()+"/N");

            QString EMP_MP_Pulse=QString(" %1 = %2 %3")
                                            .arg(formulaSkinEMP)
                                            .arg(QString::number(MyLaserSkinMP_Pr->getEMP_2ndCondition(),'e', 2))
                                            .arg(empSkinUnitMP);

            dockSkin->ui->EMP_1st_Label->setText(EMP_CWSP_Pulse);
            dockSkin->ui->EMP_2nd_Label->setText(EMP_MP_Pulse);

            dockSkin->ui->EMP_1st_Label->setVisible(true);
            dockSkin->ui->EMP_2nd_Label->setVisible(true);


            DNRO_scientNot=MyLaserSkinMP_Pr->getNSHD()>1.0e+03;
            if(DNRO_scientNot)
                dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinMP_Pr->getNSHD(),'e', 2));
                else
                dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinMP_Pr->getNSHD(),'f', 1));
    }
}

void DockControls::on_operationCombo_currentIndexChanged(int index)
{
	/******************************************
	* L'indice scelto della casella combinata *
	*******************************************/
    n_laser=index;

   /******************
    * CONTINUOS WAVE *
    * ****************/	
    if(n_laser==0)
    {
		
	/***********************************************************************************
	* Range di variazione della potenza nella modalità di funzionamento CONTINUOS WAVE *
	************************************************************************************/

    ui->powerErgControl->setMinimumExponent(-1);
    ui->powerErgControl->setMaximumExponent(6);
		
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserCW_Pr=MyLaserSafetyMP;
    MyLaserSkinSP_Pr=MyLaserSkinSafetyMP;

    MyLaserCW_Pr->setExposureTime();
    ui->pulseControl->setEnabled(false);

	/*****************************************************************************
	* Ogni volta che passoa alla modalità di funzionamento CONTINUOS WAVE        *
	* Imposto il valore della potenza a 1.0e+00, il valore della frequenza a 0,  *
 	* il tempo di esposizione della cute a 10                                    *
	******************************************************************************/
   
    prf=0.0;//frequenza
    ui->prfControl->setValue(prf);
    ui->prfControl->setEnabled(false);
    //Non è possibile impostare il valore della prf essendo non definita per l'oggetto MyLaserCW_Pr
    myLaserGoggle->setFrequency(prf);

    powerErg=1.0e+00;//potenza
    MyLaserCW_Pr->setPowerErg(beamCorrection*powerErg);
    ui->powerErgControl->setValue(powerErg);
    myLaserGoggle->setPowerErg(powerErg);

    T_Skin=5.0;//tempo esposizione pelle
    MyLaserSkinSP_Pr->setPulseWidth(T_Skin);
    ui->T_SkinControl->setValue(T_Skin);
    ui->T_SkinControl->setEnabled(true);

    //abilito il checkbox per l'impotazione manuale del tempo di esposizione
    ui->enableTeCheckBox->setEnabled(true);

    /****************************************************************************************
    * se l'impostazione del tempo di esposizione è abilitata aggiorno il laser con          *
    * il valore corrisposndente altrimenti aggiorno con il tempo di esposizione predefinito *
    *****************************************************************************************/

    if(isTeEdtitingEnabled()){
        MyLaserCW_Pr->setEditedExposureTime(ui->teControl->getDialNumber());
        ui->teControl->setEnabled(true);}
    else
        MyLaserCW_Pr->setExposureTime();

   /**************************************************************************************
	* Nascondo la scheda relativa al laser effetti medi in modalità ad IMPULSI RIPETUTI. *
	**************************************************************************************/
    dockGoggle->ui->page_2->hide();

    if(((myLaserGoggle->getWavelength()>315) && (myLaserGoggle->getWavelength()<=1e+06)))
         {
             myLaserGoggle->setPulseWidth(LaserGoggle::timeBaseWavelength);
          }
        else
            if(((myLaserGoggle->getWavelength()>=180) && (myLaserGoggle->getWavelength()<=315)))
              {
                 myLaserGoggle->setPulseWidth(LaserGoggle::timeBaseLowWavelength);
              }

   /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
	* myLaserGoggle                                                                          *
	******************************************************************************************/
    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
    fetchDataVector();
    fetchLaserOutput();
    dispayScaleNumber();
    displayLaserOutput();
    displayTimeBase();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();
	//Nascondo i controlli riguardanti i protettori ottici non previsti
    showControls(false);

    }
    else
    if (n_laser==1)
    {

   /************
    * IMPULSATO *
    * ***********/
	
   /****************************************************************
	* Range di variazione dell'energia dell'impulso nella modalità *
	* di funzionamento IMPULSATO                                   *
	****************************************************************/

    ui->powerErgControl->setMinimumExponent(-12);
    ui->powerErgControl->setMaximumExponent(1);
	
   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserSP_Pr=MyLaserSafetyMP;
    MyLaserSkinSP_Pr=MyLaserSkinSafetyMP;
	
   /******************************************************************
	* Ogni volta che passo alla modalità di funzionamento IMPULSATO  *
	* imposto il valore dell'energia dell'impuslo a 1.0e+03, il      *
	* valore della frequenza a 0, il tempo di esposizione degli      *
	* occhi a 1.0e-06, il tempo di esposizione della cute a 1.0e-06. *
	******************************************************************/

    prf=0;
    ui->prfControl->setValue(prf);
    ui->prfControl->setEnabled(false);

    pulseWidth=1.0e-06;
    ui->pulseControl->setEnabled(true);
    ui->pulseControl->setValue(pulseWidth);
    MyLaserSkinSP_Pr->setPulseWidth(pulseWidth);

    powerErg=1.0e-03;
    ui->powerErgControl->setEnabled(true);
    ui->powerErgControl->setValue(powerErg);
    MyLaserSkinSP_Pr->setPowerErg(powerErg);

    T_Skin=1.0e-06;
    MyLaserSkinSP_Pr->setPulseWidth(T_Skin);
    ui->T_SkinControl->setEnabled(false);

    myLaserGoggle->setPulseWidth(pulseWidth);
    myLaserGoggle->setPowerErg(powerErg);
    myLaserGoggle->setFrequency(prf);

    //disabilito il controllo per l'impotazione manuale del tempo di esposizione e
    //il checkbox associato
    ui->enableTeCheckBox->setEnabled(false);
    ui->teControl->setEnabled(false);

	/**************************************************************************************
	* Nascondo la scheda relativa al laser effetti medi in modalità ad IMPULSI RIPETUTI. *
	**************************************************************************************/
    dockGoggle->ui->page_2->hide();

   /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
	* myLaserGoggle                                                                          *
	******************************************************************************************/
    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
    fetchDataVector();
    fetchLaserOutput();
    dispayScaleNumber();
    displayLaserOutput();
    displayTimeBase();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();
	//Nascondo i controlli riguardanti i protettori ottici non previsti
	showControls(false);
    }
    else
   /********************
    * IMPULSI RIPETUTI *
    * ******************/	
    if (n_laser==2)
    {
		
   /****************************************************************
	* Range di variazione dell'energia dell'impulso nella modalità *
	* di funzionamento IMPULSI RIPETUTI.                            *
	****************************************************************/

    ui->powerErgControl->setMinimumExponent(-12);
    ui->powerErgControl->setMaximumExponent(1);

   /*****************************************
    * Imposto il valore negli oggetti Laser *
    *****************************************/
    MyLaserMP_Pr=MyLaserSafetyMP;
    MyLaserSkinMP_Pr=MyLaserSkinSafetyMP;

   /******************************************************************
	* Ogni volta che passo alla modalità di funzionamento AD IMPULSI *
	* RIPETUTI imposto il valore dell'energia dell'impusl a 1.0e+03, il valore della     *
	* frequenza a 10.0, il tempo di esposizione degli occhi a 1.0e-06,  *
	* il tempo di esposizione della cute a 10. 				 *
	******************************************************************/
    powerErg=1.0e-03;
    MyLaserCW_Pr->setPowerErg(beamCorrection*powerErg);
    ui->powerErgControl->setValue(powerErg);
    myLaserGoggle->setPowerErg(powerErg);

    //ogni volta cha passo al funzionamento ad impulsi ripetuti imposto la durata degli impulsi a 1.0e-06 s
    pulseWidth=1.0e-06;
    ui->pulseControl->setEnabled(true);
    ui->pulseControl->setValue(pulseWidth);
    MyLaserSkinMP_Pr->setPulseWidth(pulseWidth);
    myLaserGoggle->setPulseWidth(pulseWidth);

    //ogni volta cha passo al funzionamento ad impulsi ripetuti imposto la frequenza a 10 Hz
    prf=10;
    ui->prfControl->setEnabled(true);
    ui->prfControl->setValue(prf);
    MyLaserMP_Pr->setPRF(prf);
    MyLaserSkinMP_Pr->setPRF(prf);
    myLaserGoggle->setFrequency(prf);
	
    T_Skin=5.0;
    MyLaserSkinMP_Pr->setExposureTime(T_Skin);
    ui->T_SkinControl->setValue(T_Skin);
    MyLaserSkinMP_Pr->setPulseWidth(pulseWidth);
    ui->T_SkinControl->setEnabled(true);

	//Imposto il tempo di esposizione alla radiazione laser.
    MyLaserMP_Pr->setExposureTime();

    /****************************************************************************************
    * se l'impostazione del tempo di esposizione è abilitata aggiorno il laser con          *
    * il valore corrisposndente altrimenti aggiorno con il tempo di esposizione predefinito *
    *****************************************************************************************/

    //abilito il checkbox per l'impotazione manuale del tempo di esposizione
    ui->enableTeCheckBox->setEnabled(true);

    if(isTeEdtitingEnabled()){
        MyLaserCW_Pr->setEditedExposureTime(ui->teControl->getDialNumber());
        ui->teControl->setEnabled(true);}
    else
        MyLaserMP_Pr->setExposureTime();

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
         myDLaserGoggle->setPulseWidth(LaserGoggle::timeBaseWavelength);
      }
      else
    if(((myDLaserGoggle->getWavelength()>=180) && (myDLaserGoggle->getWavelength()<=315)))
      {
         myDLaserGoggle->setPulseWidth(LaserGoggle::timeBaseLowWavelength);
      }
   
   /******************************************************************************************	
	* Invoco le funzioni per il prelievo dei dati e il tracciamento dei grafici per l'oggetto *
	* myLaserGoggle                                                                          *
	******************************************************************************************/
    enablePeakControl();
    effectivePowerErgPeak();
    modeLockingPeak();
    fetchLaserOutput();
    fetchDataVector();
    dispayScaleNumber();
    displayLaserOutput();
    displayNumberOfPulse();
    displayCoefficient_k();
    displayCoefficient_ki();
    display_ni_max();
    displayTimeBase();

    fetchDLaserOutput();
    fetchDDataVector();
    dispayDScaleNumber();
    displayDLaserOutput();
    displayDTimeBase();

    //Mostro i tutti controlli riguardanti i protettori
		 showControls(true);
      }

   /*******************************************
    * Imposto i widget per la visualizzazione *
    *******************************************/
    setWidgets();

   /*******************************************************
    * Imposto altri valori necessari per la parte grafica *
    *******************************************************/
    setPowerErgForEMP();
    setOpticalDistance();
    setSkinDistances();
    setLambertianMax();
	
   /******************
    * Emetto segnali *
    ******************/
    emit EMP_Changed();//Cambia il diametro del fascio
    emit operationChanged();//cambia la modalità di funzionamento
    emit modified();//Per salvataggio fil
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

if(n_laser==0)
            skinPowerErgUnit="Potenza [W]";
else
    if(n_laser==1)
            skinPowerErgUnit="Energia [J]";

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

dockSkin->ui->tEMP_SkinLabel->setText(MyLaserSkinSP_Pr->getFormulaSort().c_str() + empSkinUnit);
dockSkin->ui->EMP_SkinLabel->setText(QString::number(MyLaserSkinSP_Pr->getEMP(),'e', 2));

dockSkin->ui->tFormulaSkinLabel->setText("Formula");
dockSkin->ui->FormulaSkinLabel->setText(MyLaserSkinSP_Pr->getFormulaEMP().c_str());

dockSkin->ui->tNSHDLabel->setText((QString)"DNRP [m]");

bool DNRO_scientNot;


    if(n_laser==0)
    {
        DNRO_scientNot=MyLaserSkinSP_Pr->getPowerNSHD()>1.0e+03;
        if(DNRO_scientNot)
            dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinSP_Pr->getPowerNSHD(),'e', 2));
            else
            dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinSP_Pr->getPowerNSHD(),'f', 1));

         dockSkin->ui->conditions_Label->setText("EMP \nper esposizioni\nda onda continua");

    }
        else
    if(n_laser==1)
    {
        DNRO_scientNot=MyLaserSkinSP_Pr->getErgNSHD()>1.0e+03;
        if(DNRO_scientNot)
            dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinSP_Pr->getErgNSHD(),'e', 2));
            else
            dockSkin->ui->NSHDLabel->setText(QString::number(MyLaserSkinSP_Pr->getErgNSHD(),'f', 1));

         dockSkin->ui->conditions_Label->setText("EMP \nper esposizioni\nda impulso singolo");
    }

    dockSkin->ui->tEMP_1st_Label->setVisible(false);
    dockSkin->ui->tEMP_2nd_Label->setVisible(false);

    QString EMP_CW_Pulse=QString(" %1 = %2 %3")
                                    .arg(MyLaserSkinSP_Pr->getFormulaEMP().c_str())
                                    .arg(QString::number(MyLaserSkinSP_Pr->getEMP(),'e', 2))
                                    .arg(empSkinUnit);

    dockSkin->ui->EMP_1st_Label->setText(EMP_CW_Pulse);

    dockSkin->ui->EMP_1st_Label->setVisible(true);
    dockSkin->ui->EMP_2nd_Label->setVisible(false);
}

DockControls::~DockControls()
{
    delete ui;
}



/************************************************************************************************
 * Parametri necessari per il calcolo dell'energia diffusa estesa di un riflettore lambertiano. *
 * I segnali vengono impiegati in queste funzioni e non nelle slot dei controllj per evitare di *
 * dover considerare anche le variazioni della modalità di funzionamento del laser.             *
 * **********************************************************************************************/

void DockControls::setPowerErgForEMP()
{
    double _powerErgForEMP;

    if(n_laser==0)
        _powerErgForEMP=MyLaserCW_Pr->getPowerErgForEMP();
        else
    if(n_laser==1)
        _powerErgForEMP=MyLaserSP_Pr->getPowerErgForEMP();
        else
        _powerErgForEMP=MyLaserMP_Pr->getPowerErgForEMP();

        if(_powerErgForEMP==powerErgForEMP)
        return;

    powerErgForEMP=_powerErgForEMP;
    emit powerErgForEMPChanged();
}


double DockControls::getPowerErgForEMP()const
{
    return powerErgForEMP;
}

void DockControls::setEMP()
{
    double _myEMP;

    if(n_laser==0)
        _myEMP=MyLaserCW_Pr->getEMP();
        else
    if(n_laser==1)
        _myEMP=MyLaserSP_Pr->getEMP();
    else
        _myEMP=MyLaserMP_Pr->returnMultiPulse_EMP();

    if(_myEMP==myEMP)
    return;

    myEMP=_myEMP;
}

void DockControls::setBeamDiameter()
{
    double _beamDiameter=ui->beamDiameterControl->getScientificNumber();

        if(_beamDiameter==beamDiameter)
        return;

    beamDiameter=_beamDiameter;
}

double DockControls::getPowerErg()const
{
    return powerErg;
}

double DockControls::getExposureTime()const
{
    return exposureTime;
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


/***********************************************************************************************
 * Salvattagio degli argomenti delle slot dei controlli necessari                              *
 * *********************************************************************************************/

/*
void DockControls::setPowerErgDial(const int _powerErgDial)
{
    powerErgDial=_powerErgDial;
}

void DockControls::setBeamDiameterDial(const int _beamDiameterDial)
{
    beamDiameterDial=_beamDiameterDial;
}
*/
void DockControls::setOpticalDistance()
{
    double _NOHD;

    if(n_laser==0)
        {
         _NOHD=MyLaserCW_Pr->getNOHD();
         }
         else
            if(n_laser==1)
                {
                _NOHD=MyLaserSP_Pr->getNOHD();
                }
                else
                    {
                    _NOHD=MyLaserMP_Pr->getNOHD();
                    }
    if(_NOHD==NOHD)
        return;

     NOHD=_NOHD;
     emit NOHD_Changed();
}

void DockControls::setLambertianMax()
{
    double _lambertianMax;

    if(n_laser==0)
        {
         _lambertianMax=MyLaserCW_Pr->getLambertianMax();
         }
         else
            if(n_laser==1)
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
     emit lambertianMaxChanged();
}

void DockControls::setSkinDistances()
{
    double _NSHD;

        if(n_laser==0)
            _NSHD=MyLaserSkinSP_Pr->getPowerNSHD();
            else
        if(n_laser==1)
            _NSHD=MyLaserSkinSP_Pr->getErgNSHD();
        else
            _NSHD=MyLaserSkinMP_Pr->getNSHD();

    if(_NSHD==NSHD)
        return;

     NSHD=_NSHD;
     emit NSHD_Changed();
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
    ui->powerErgControl->setTitle(tr("potenza [W]"));
    ui->powerErgControl->setMinimumExponent(-3);
    ui->powerErgControl->setMaximumExponent(6);
    ui->powerErgControl->setValue(1.00e+00);

    QString alpha="α";
    ui->alphaControl->setTitle(alpha.toHtmlEscaped() + " [mrad]");
    ui->alphaControl->setMinimumExponent(-3);
    ui->alphaControl->setMaximumExponent(1);
    ui->alphaControl->setValue(1.50e+00);

    ui->pulseControl->setTitle(tr("t [s]"));
    ui->pulseControl->setMinimumExponent(-11);
    ui->pulseControl->setMaximumExponent(3);
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

    ui->enableTeCheckBox->setEnabled(true);
    ui->enableTeCheckBox->setChecked(false);

    ui->checkGaussianBeam->setChecked(true);

    ui->peakControl->setTitle(tr("P<sub>picco</sub> [W]"));
    ui->peakControl->setMinimumExponent(1);
    ui->peakControl->setMaximumExponent(15);
    ui->peakControl->setValue(8.0e+00);
}


void DockControls::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1=="UVA")
        setUVA();
    else
    if(arg1=="VIS")
        setVIS();
    else
    if(arg1=="IRA")
        setIRA();
    else
    if(arg1=="IRB")
        setIRB();
    else
    if(arg1=="IRC")
        setIRC();

    if(n_laser==0)
    MyLaserCW_Pr->setExposureTime();
    else
    if(n_laser==2)
    MyLaserMP_Pr->setExposureTime();


    setWidgets();
    emit modified();
}

void DockControls::setUVA()
{
    ui->wavelengthScrollBar->setMinimum(180);
    ui->wavelengthScrollBar->setMaximum(399);
    ui->wavelengthScrollBar->setValue(380);
}

void DockControls::setVIS()
{
    ui->wavelengthScrollBar->setMinimum(400);
    ui->wavelengthScrollBar->setMaximum(700);
    ui->wavelengthScrollBar->setValue(633);
}

void DockControls::setIRA()
{
    ui->wavelengthScrollBar->setMinimum(701);
    ui->wavelengthScrollBar->setMaximum(1400);
    ui->wavelengthScrollBar->setValue(1064);
}

void DockControls::setIRB()
{
    ui->wavelengthScrollBar->setMinimum(1401);
    ui->wavelengthScrollBar->setMaximum(3000);
    ui->wavelengthScrollBar->setValue(1583);
}

void DockControls::setIRC()
{
    ui->wavelengthScrollBar->setMinimum(3001);
    ui->wavelengthScrollBar->setMaximum(20000);
    ui->wavelengthScrollBar->setValue(10600);
}

void DockControls::on_enableTeCheckBox_toggled(bool checked)
{
    enableTeEditing=checked;
    ui->teControl->setEnabled(checked);
    if(checked)
        {
            on_teControl_valueChanged();
            /*****************
            * CONTINUOS WAVE *
            * ****************/
              if(n_laser==0)
                {
                /*******************************************************
                 * Imposto il valore del tempo di esposizione impostato*
                 *******************************************************/
                 MyLaserCW_Pr->setExposureTimeEditable(true);
                 MyLaserCW_Pr->setEditedExposureTime(exposureTime);
                 }
              else
              if(n_laser==2)
            /********************
             * IMPULSI RIPETUTI *
             ********************/
                {
                /********************************************************
                 * Imposto il valore del tempo di esposizione impostato *
                 ********************************************************/
                 MyLaserMP_Pr->setExposureTimeEditable(true);
                 MyLaserMP_Pr->setEditedExposureTime(exposureTime);
                }
    }
              else
    {
              /*****************
              * CONTINUOS WAVE *
              * ****************/

                if(n_laser==0)
                  {
                  /********************************************************
                   * Imposto il valore del tempo di esposizione impostato *
                   ********************************************************/
                   MyLaserCW_Pr->setExposureTimeEditable(false);
                   MyLaserCW_Pr->setExposureTime();
                   exposureTime=MyLaserCW_Pr->getExposureTime();
                   }
                else
              if(n_laser==2)
              /********************
               * IMPULSI RIPETUTI *
               ********************/
                  {
                  /********************************************************
                   * Imposto il valore del tempo di esposizione impostato *
                   ********************************************************/
                   MyLaserMP_Pr->setExposureTimeEditable(false);
                   MyLaserMP_Pr->setExposureTime();
                   exposureTime=MyLaserMP_Pr->getExposureTime();
                  }
    }

    setWidgets();
    emit EMP_Changed();//Cambia l'EMP
    emit modified();//Per salvataggio file

}

bool DockControls::isTeEdtitingEnabled()
{
    return enableTeEditing;
}

void DockControls::computeOpticalDensity()
{
double CW_EMP;
double SP_EMP;
double powerErg;
double firstAndThirdEMP;
double opticalDensityRatio=0.0;
double beamArea;
QString formulaSort;

if(n_laser==0)
{
    double meanIrradiance;
    CW_EMP=MyLaserCW_Pr->getEMP();
    formulaSort=MyLaserCW_Pr->getFormulaSort().c_str();
    MyLaserCW_Pr->computeBeamArea();
    beamArea=MyLaserCW_Pr->getBeamArea();
    powerErg=MyLaserCW_Pr->getPowerErg();
    meanIrradiance=4*powerErg/beamArea;

    if(formulaSort=='E')
        opticalDensityRatio=meanIrradiance/CW_EMP;
    else{
     if(formulaSort=='H')
         opticalDensityRatio=meanIrradiance*exposureTime/CW_EMP;
    }

    opticalDensity=std::log10(opticalDensityRatio);
}
else
    if(n_laser==1)
    {
    double meanExposure;

    SP_EMP= MyLaserSP_Pr->getEMP();
    powerErg=MyLaserSP_Pr->getPowerErg();
    formulaSort=MyLaserSP_Pr->getFormulaSort().c_str();
    double pulseWidth=MyLaserSP_Pr->getPulseWidth();
    MyLaserSP_Pr->computeBeamArea();
    beamArea=MyLaserSP_Pr->getBeamArea();
    meanExposure=4*powerErg/beamArea;

    if(formulaSort=='E'){
        opticalDensityRatio=meanExposure/(SP_EMP*pulseWidth);
    }
    else{
     if(formulaSort=='H')
         opticalDensityRatio=meanExposure/SP_EMP;
    }

    opticalDensity=std::log10(opticalDensityRatio);
    }

else
   if(n_laser==2)
   {
   double energyExposure;

   double TimeForPulse;

   firstAndThirdEMP=MyLaserMP_Pr->getEMP_ForOD();
   MyLaserMP_Pr->getThermalEMP();


   if(MyLaserMP_Pr->getPRF()>1/MyLaserMP_Pr->getTmin())
   {
       if(firstAndThirdEMP==MyLaserMP_Pr->getThermalEMP())
           TimeForPulse=MyLaserMP_Pr->getTmin();
       else
            TimeForPulse=MyLaserMP_Pr->getPulseWidth();
   }
   else
          TimeForPulse=MyLaserMP_Pr->getPulseWidth();

   MyLaserMP_Pr->getPulseWidth();
   powerErg=MyLaserMP_Pr->getPowerErg();

   MyLaserMP_Pr->computeBeamArea();
   beamArea=MyLaserMP_Pr->getBeamArea();
   energyExposure=powerErg/beamArea;

   MyLaserMP_Pr->computeEMP_ForOD();
   firstAndThirdEMP=MyLaserMP_Pr->getEMP_ForOD();
   qDebug() << "Primo e terzo emp: " << firstAndThirdEMP;

    if(MyLaserMP_Pr->getFormulaSort()=="E")
        opticalDensityRatio=energyExposure/(firstAndThirdEMP*TimeForPulse);
    else if(MyLaserMP_Pr->getFormulaSort()=="H")
            opticalDensityRatio=energyExposure/firstAndThirdEMP;

        opticalDensity=std::log10(opticalDensityRatio);
    }
}

void DockControls::dComputeOpticalDensity()
{
    double secondEMP=0.0;
    double powerErg;
    double meanPower;
    double beamArea;
    int PRF;
    double opticalDensityRatio;
    double meanIrradiance;
    QString formulaSort;

    MyLaserMP_Pr->computeBeamArea();
    beamArea=MyLaserMP_Pr->getBeamArea();

    if(n_laser==2){
        powerErg=MyLaserMP_Pr->getPowerErg();
        formulaSort=MyLaserMP_Pr->getMeanPowerFormulaSort().c_str();
        PRF=MyLaserMP_Pr->getPRF();
        meanPower=powerErg*PRF;
        meanIrradiance=meanPower/beamArea;

        if(formulaSort=='E'){
             secondEMP=MyLaserMP_Pr->getEMP_MP();
                }
                else{
                    if(formulaSort=='H')
                        secondEMP=MyLaserMP_Pr->getEMP_MP()/MyLaserMP_Pr->getExposureTime();
                }

        opticalDensityRatio=meanIrradiance/secondEMP;

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

bool DockControls::isModeLocking()
{
    bool modeLocking=false;

    if(n_laser!=0){
        if((wavelength<400)or(wavelength>1400)){
           if(pulseWidth<1.0e-09)
            {
            modeLocking=true;
            }
        }
    }

    return modeLocking;
}

void DockControls::effectivePowerErgPeak()
{
    if(isModeLocking())
    {
        double peak=ui->peakControl->getScientificNumber();

        if((wavelength>315)and(wavelength<400))
                powerErgPeak=peak*pulseWidth;
            else
                powerErgPeak=peak;
    }
}

void DockControls::modeLockingPeak()
{
    if(isModeLocking()){
           effectivePowerErg=powerErgPeak;
           }
       else
           {
            effectivePowerErg=powerErg;
           }
}

void DockControls::enablePeakControl()
{
    ui->peakControl->setEnabled(isModeLocking());
}

void DockControls::on_checkGaussianBeam_clicked(bool checked)
{
    gaussianBeam=checked;
    if(checked)
        beamCorrection=1.0;
          else
        beamCorrection=2.5;

    on_powerErgControl_valueChanged();
}
