#ifndef LASERCLASSMP_H
#define LASERCLASSMP_H
#include "laserclasssp.h"

using namespace std;

/*! \brief 	Classe per il calcolo della Classe di un laser ad impulsi multipli con il metodo semplificato (CEI EN 60825-1 2017)
 *
 * La Classe è derivata dalla classe LaserClassSP ed aggiunge un consistente numero di funzioni membro che consentono di:
 <ul>
 <li>Applicare la valutazione del criterio della potenza media;</li>
 <li>Considerare gli effetti termici nella valutazione del LEA nella banda da 400<&lambda;< 1400 nm;</li>
 <li>Valutare la classe nel caso di impulsi ad alta frequenza di ripetizione;</li>
 <li>Individuare la classe del dispositivo dal confronto con le Classi parziali.</li>
 </ul>
 */

class LaserClassMP: public LaserClassSP
{
public:
    LaserClassMP(double=PULSE_REPETITION_FREQUENCY, double=PUPIL_DIAMETER, double=PULSE_ENERGY, double=DIVERGENCE,
                 double=EmpLeaTables::HE_NE_WAVELENGTH, double=PULSE_WIDTH, double=ALPHA);/*!< L'istanza della classe prevede l'inserimento dei parametri:

<ul>
<li>Frequenza di ripetizione dell'impulso <b>PRF</b> in Hz;</li>
<li>diametro del fascio <b>a</b> in mm;</li>
<li>energia dell'impulso <b>Q</b> in Joule;</li>
<li>divergenza <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm.</li>
</ul>

La classe prevede inoltre un costruttore di default con i seguenti parametri:
<ul>
<li>Frequenza di ripetizione dell'impulso <b>PRF</b>=10 Hz;</li>
<li>diametro del fascio <b>a</b>= 7 mm;</li>
<li>energia dell'impulso <b>Q</b>= 10<sup>-3</sup> J;</li>
<li>divergenza <b>&phi;</b> = 1.5 mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> = 633 nm;</li>
<li>durata dell'impulso <b>t</b>= 1.0e-06 s;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b>=1,5 mrad.</li>
</ul>

*/
    const static double PULSE_REPETITION_FREQUENCY; /*!< Parametro di default della frequenza di ripetizione degli impulsi i Hz. */

    void meanClassUpdate(const double&, const double&);/*!< Effettua l'aggiornamento dei parametri che vengono successivamente
    * impiegati per la valutazione della classe riguardante il criterio della potenza media. Ha il medesimo funzionamento della classe
    * LaserClassCW::classUpdate(laserOperation, const double&, const double &) con la differenza che non viene riciesto il parametro time.	*/
    void tiClassUpdate(const double&, const double&);/*!< Effettua l'aggiornamento dei parametri che vengono successivamente
    * impiegati per la valutazione della classe riguardante il criterio dell'impulso per funzionamento ad alta frequenza. Ha il medesimo
    * funzionamento della classe LaserClassCW::classUpdate(laserOperation, const double&, const double &) con la differenza che non viene
    * riciesto il parametro time.	*/
    void c5ClassUpdate();/*!< Effettua l'aggiornamento dei parametri che vengono successivamente
    * impiegati per la valutazione della classe riguardante il criterio dell'impulso. Ha il medesimo
    * funzionamento della classe LaserClassCW::classUpdate(laserOperation, const double&, const double &) con la differenza che non viene
    * riciesto alcun parametro. */
    void computeTi();/*!< Calcola il parametro <b>T<sub>i</sub></b> secondo quanto riportato nella tabella seguente:
    *
    * Lunghezze d'onda [nm]                | T<sub>i</sub>
    * :-----------------------------------:|:-------------------------:
    * 400 < &lambda; &le; 1050             | 5 &sdot; 10<sup>-6</sup>
    * 1050 < &lambda; &le; 1400            | 13 &sdot; 10<sup>-6</sup>
    * 1400 < &lambda; &le; 1500            | 0,001
    * 1500 < &lambda; &le; 1800            | 10
    * 1800 < &lambda; &le; 2600            | 0,001
    * 2600 < &lambda; &le; 10<sup>6</sup>  | 10<sup>-7</sup>
*/

    void computeMeanPower();/*!< Calcola il valore medio della potenza del dispositivo come  \f[ P_{media} = Q &sdot; PRF \f] */
    void computePulseNumber();/*!< Calcola il numero degli impulsi. Nel caso di rischio termico 400 &le; &lambda; &le; 1400 nm
    * per basse frequenze di ripetizione vale:
    * \f[ N=T_e &sdot; PRF \f]
    * nel caso di alte frequenze di ripetizione \f$ PRF > \frac{1}{T_i} \f$ :
    * \f[ N=T_e &sdot; \frac{1}{T_i} \f]
    * negli altri casi:
    * \f[ N=T_b &sdot; PRF \f]
    *dove <b>T<sub>i</sub></b> è il tempo minimo dell'impulso, <b>T<sub>e</sub></b> è il tempo di esposizione, <b>T<sub>b</sub></b> è
    * la base dei tempi.
    */
    string valutateCounting();/*!< Nel caso di rischio termico 400 &le; &lambda; &le; 1400 nm e se \f$ PRF > \frac{1}{T_i} \f$
    restituisce la stringa <i>non regolare</i> altrimenti restituisce <i>regolare</i>". */

    double getTi()const; /*!< Restituisce il valore di <b>T<sub>i</sub></b> in secondi. */
    void computeC5(); /*!< Calcola il valore del coefficiente C<sub>5</sub> secondo la procedura indicata nella norma CEI EN 60825-1 2017. */
    void setPRF(const double &_prf); /*!< Imposta il valore della Frequenza di Ripetizione degli Impulsi in Hz. */
    double getPRF()const;/*!< Restituisce il valore della Frequenza di Ripetizione degli Impulsi in Hz. */
    void setTimeBase();/*!< Imposta la base dei tempi <b>T<sub>b</sub></b> secondo il criterio indicato nella tabella seguente:
    *
    * Lunghezze d'onda [nm]               | T<sub>b</sub> [s]
    * :----------------------------------:|:----------------:
    * 400 < &lambda; &le; 700             | 0.25
    * 180 < &lambda; &le; 400             | 30000
    * 700 < &lambda; &le; 10<sup>6</sup>  | 100
    *
    * contestualmente imposta il valore della durata dell'impulso dell'oggetto myMeanLaserClass a T<sub>i</sub>.
    */

    array<double, ComputeLEA::N_LEA> powerErgUnit(array<int, ComputeLEA::N_LEA>, const double &, const double &);/*!< Restituisce un puntatore ad un array di double lungo 4 nel quale
    * sono memorizzati i valori dell'uscita media del dispositivo espressi nell'unità di misura omogenee al LEA corrispondente.
    *     Unità di Misura LEA  | Uscita
    * :-----------------------:|:------------------:
    *    W/m<sup>2</sup>       | P<sub>media</sub> /A<sub>b</sub>
    *    J/m<sup>2</sup>       | P &sdot; T<sub>b</sub>/A<sub>b</sub>
    *    W                     | P
    *    J                     | P &sdot; T<sub>b</sub>
    */
    array<double, ComputeLEA::N_LEA> computeMeanLEA_Corrected(array<int, ComputeLEA::N_LEA>);/*!< Costruisce un puntatore ad un array di double lungo 4 nel quale
    * sono memorizzati i valori LEA riguardanti il funzionamento medio. Si ricorda infatti che nel caso di funzionamento medio è necessario che
    * il LEA medio venga calcolato come valore medio del LEA corrispondente alla base dei tempi per una durata corrispondente a quella della
    * ripetizione singola.
    *     Unità di Misura LEA  | Correzione
    * :-----------------------:|:------------------:
    *    W/m<sup>2</sup>       | 1/(t &sdot; PRF)
    *    J/m<sup>2</sup>       | 1/(PRF &sdot; T<sub>b</sub>)
    *    W                     | 1
    *    J                     | t
    */

    array<double, ComputeLEA::N_LEA> computeLEA_ThermalCorrection(array<double, ComputeLEA::N_LEA>);/*!< Restituisce un array di tipo double ottenuto dall'array del LEA dell'impulso singolo
    * corretto con il coefficiente C<sub>5</sub>. */

    array<string, ComputeLEA::N_LEA> getThermalLEA_Expressions()const;

    void setWavelength(const double&);/*!< E' un overload. Imposta il valore della lunghezza d'onda <b>&lambda;</b> in nm oltre ad
    * impostare gli oggetti myLaserClass, myMeanLaserClass, myTiLaserClass. */
    void setPulseWidth(const double&);/*!< E' un overload. Imposta il valore della durata dell'impulso <b>t</b> in secondi oltre
    * ad impostare gli oggetti myLaserClass, myMeanLaserClass, myTiLaserClass. */
    void setAlpha(const double&);/*!< E' un overload. Imposta il valore dell'angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad
    * oltre ad impostare gli oggetti myLaserClass, myMeanLaserClass, myTiLaserClass. */
    void setAlpha_max(const double &);/*!< Imposta il valore di &alpha;<sub>max</sub> secondo la procedura indicata dalla CEI EN 60825-1 2017.
    *	   Durata  t                | &alpha;<sub>max</sub> [mrad]
    * :----------------------------:|:-----------------------------:
    * < 625 &mu; s                  | 5
    * 625 &mu; s &le; t &le; 0,25 s | 200 t<sup>0,5</sup>
    * t> 0,25 s	                    | 100
    */

    double getMeanPower() const; /*!< Restituisce il valore della potenza media in Watt. */
    double getC5Coefficient() const;/*!< Restituisce il coefficiente C<sub>5</sub> della valutazione termica dell'impulso. */
    double getPulseNumber() const;/*!< Restituisce il numero di impulsi. */


/***************************************************************
 * funzioni membro per il calcolo relativo all'emissione media *
 * **************************************************************/
    array<double, ComputeLEA::N_LEA> getMeanLEA()const;/*!< Restituisce i valori dei LEA delle Classi previste, relativamente agli effetti medi, corrispondente
    * ai valori impostati di <b>&lambda;</b> e t. */
    array<string, ComputeLEA::N_LEA> getMeanLEA_Formula()const;/*!< Restituisce l'espressione della formula dei LEA delle Classi previste, relativamente agli
    * effetti medi, corrispondente ai valori impostati di <b>&lambda;</b> e t. */
    array<string, ComputeLEA::N_LEA> getMeanLEA_FormulaUnit()const;/*!< Restituisce l'unità di misura dei LEA delle Classi previste, relativamente agli effetti medi,
    * corrispondente ai valori impostati di <b>&lambda;</b> e <b>t</b>. */
    array<string, ComputeLEA::N_LEA> getMeanLEA_FormulaTipo()const;/*!< Restituisce le grandezze fisiche dei LEA delle Classi previste, relativamente agli effetti medi,
    corrispondente ai valori impostati di <b>&lambda;</b> e <b>t</b>. */
    array<double, ComputeLEA::N_LEA> getMeanPowerErgEq()const; /*!<  Restituisce un puntatore ad un array di double lungo 4 nel quale sono memorizzati i valori medi
    * dell'uscita del dispositivo espressi nell'unità di misura omogenee al LEA corrispondente.*/
    array<bool, LaserClassCW::N_CLASS> getMeanClassValutation()const;/*!< Restituisce la Classe del dispositivo per le emissioni medie. */
    double getMeanDistCond_1()const;/*!< Restituisce il valore in mm della distanza dal diaframma della 1<sup>a</sup> condizione per la valutazione
    * degli effetti medi. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta
    * meanClassUpdate(const double&, const double&). */
    double getMeanDistCond_3()const;/*!< Restituisce il valore in mm del diametro dal diaframma della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta meanClassUpdate(const double&, const double&). */
    double getMeanApCond_1()const;/*!< Restituisce il valore in mm del diametro dal diaframma della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta meanClassUpdate(const double&, const double&). */
    double getMeanApCond_3()const;/*!< Restituisce il valore in mm del diametro dal diaframma della 3<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta meanClassUpdate(const double&, const double&). */
    double getMeanBeamAtStop_Cond_1()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 1<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta
    * meanClassUpdate(const double&, const double&). */
    double getMeanBeamAtStop_Cond_3()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 3<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta
    * meanClassUpdate(const double&, const double&). */
    double getMeanCouplingFactor_Cond_1()const;/*!< Restituisce il fattore di accoppiamento della 1<sup>a</sup> condizione riguardanle la valutazione
    * nel caso di effetti medi. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta meanClassUpdate(const double&,
    * const double&). */
    double getMeanCouplingFactor_Cond_3()const;/*!< Restituisce il fattore di accoppiamento della 3<sup>a</sup> condizione riguardanle la valutazione
    * nel caso di effetti medi. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta
    * meanClassUpdate(const double&, const double&). */
    array<double, ComputeLEA::N_LEA> getMeanPowerErg_Cond_1()const;/*!< Restituisce l'uscita media del dispositivo corretta con il fattore di accoppiamento
    * della 1<sup>a</sup> condizione. */
    array<double, ComputeLEA::N_LEA> getMeanPowerErg_Cond_3()const;/*!< Restituisce l'uscita del dispositivo media corretta con il fattore di accoppiamento
    * della 3<sup>a</sup> condizione. */
    laserClass getMeanLaserClassAssigned()const;/*!< Opera il confronto di ciascun con le uscite corrette LEA relative  agli effetti medi per
    * la 1<sup>a</sup> e la 3<sup>a</sup> condizione e fornisce un array di tipo booleano cong gli estessi criteri espositi in
    * LaserClassCW::valuateLEA_forClass(double*, double*, double*).*/
    array<double, ComputeLEA::N_LEA> getMeanLEA_Corrected()const; /*!< Restituisce i valori del LEA per la valutazione degli effetti medi corretti da
    * computeMeanLEA_Corrected(int*). */

    array<int, ComputeLEA::N_LEA>getMeanLEA_FormulaSort()const;/*!< Restituisce il tipo di LEA delle Classi previste nel caso di effetti medi, corrispondente ai valori
    * impostati di <b>&lambda;</b> e <b>t</b>. */

    array<string, ComputeLEA::N_LEA> getMeanLEA_Expressions()const;

    array<double, ComputeLEA::N_LEA> getTiLEA()const;/*!< Restituisce i valori dei LEA delle Classi previste, relativamente al caso di alta frequenza, corrispondente
    * ai valori impostati di <b>&lambda;</b> e t. */
    array<string, ComputeLEA::N_LEA> getTiLEA_Formula()const;/*!< Restituisce l'espressione della formula dei LEA delle Classi previste, relativamente al caso di alta
    * frequenza, corrispondente ai valori impostati di <b>&lambda;</b> e t. */
    array<string, ComputeLEA::N_LEA> getTiLEA_FormulaUnit()const;/*!< Restituisce l'unità di misura dei LEA delle Classi previste, relativamente al caso di alta frequenza,
    * corrispondente ai valori impostati di <b>&lambda;</b> e <b>t</b>. */
    array<string, ComputeLEA::N_LEA> getTiLEA_FormulaTipo()const;/*!< Restituisce le grandezze fisiche dei LEA delle Classi previste, relativamente al caso di alta frequenza,
    corrispondente ai valori impostati di <b>&lambda;</b> e <b>t</b>. */
    array<double, ComputeLEA::N_LEA> getTiPowerErgEq()const;
    array<bool, LaserClassCW::N_CLASS> getTiClassValutation()const;
    double getTiDistCond_1()const;/*!< Restituisce il valore in mm del diametro dal diaframma della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&, const double&). */
    double getTiDistCond_3()const;/*!< Restituisce il valore in mm del diametro dal diaframma della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&, const double&). */
    double getTiApCond_1()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 1<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&,
    * const double&). */
    double getTiApCond_3()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 3<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&,
    * const double&). */
    double getTiBeamAtStop_Cond_1()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 1<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&,
    * const double&). */
    double getTiBeamAtStop_Cond_3()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 1<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&,
    * const double&). */
    double getTiCouplingFactor_Cond_1()const;/*!< Restituisce il fattore di accoppiamento della 1<sup>a</sup> condizione riguardanle la valutazione
    nel caso di alta frequenza. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&,
    * const double&). */
    double getTiCouplingFactor_Cond_3()const;/*!< Restituisce il fattore di accoppiamento della 3<sup>a</sup> condizione riguardanle la valutazione
    nel caso di alta frequenza. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta tiClassUpdate(const double&,
    * const double&). */
    array<double, ComputeLEA::N_LEA> getTiPowerErg_Cond_1()const;/*!< Restituisce l'uscita del dispositivo riguardante l'alta frequenza corretta con il fattore di accoppiamento
    * della 1<sup>a</sup> condizione. */
    array<double, ComputeLEA::N_LEA> getTiPowerErg_Cond_3()const;/*!< Restituisce l'uscita del dispositivo riguardante l'alta frequenza corretta con il fattore di accoppiamento
    * della 3<sup>a</sup> condizione. */
    laserClass getTiLaserClassAssigned()const;
    array<double, ComputeLEA::N_LEA>getTiLEA_Corrected()const;
    void setTe();/*!< Imposta il tempo di esposizione cda impiegare per la valutazione degli effetti termici cioè il valore minimo tra
    * <b>T<sub>2</sub></b> e <b>T<sub>b</sub></b> */

    double getTe()const; /*!< Restituisce il valore del tempo <b>T<sub>E</sub></b>*/
    array<int, ComputeLEA::N_LEA> getTiLEA_FormulaSort()const;/*!< Restituisce il tipo di LEA delle Classi previste nel caso di alta frequenza, corrispondente ai valori
    * impostati di <b>&lambda;</b> e <b>t</b>. */

    array<string, ComputeLEA::N_LEA> getTiLEA_Expressions()const;

    laserClass getC5LaserClassAssigned()const;
    array<double, ComputeLEA::N_LEA>getLEA_Corrected()const;

    array<bool, LaserClassCW::N_CLASS> valuateSystemClass(array<bool, LaserClassCW::N_CLASS>, array<bool, LaserClassCW::N_CLASS>, array<bool, LaserClassCW::N_CLASS> c5ClassValutation);

    void highFrequencyValuation();/*!< Calcola e aggiorna la valutazione nel caso di alta frequenza (\f$PRF > \frac{1}{T_i}\f$)  */
    laserClass getSystemClassValutation()const;
    void updateAll();/*!< Calcola i valori dei LEA degli oggetti myLaserClass, myMeanLaserClass, myTiLaserClass, calcola l'area della sezione trasversale del fascio,
    * aggiorna la valutazionde la classe dell'intero sistema. */


private:
    int prf;
    double Ti;
    double meanPower;
    int pulseNumber;
    double alpha_max;

   /***********************************************************
    * Te è il tempo di esposizione da considerare nel calcolo *
    * del numero di impulsi (il minore tra la timeBase e T2). *
    ***********************************************************/
    double Te;
    ComputeLEA myMeanLaserClass;
    ComputeLEA myTiLaserClass;


    array<double, ComputeLEA::N_LEA> meanLEA_Value;
    array <string, ComputeLEA::N_LEA> meanLEA_formula;
    array <string, ComputeLEA::N_LEA> meanLEA_formulaTipo;
    array <string, ComputeLEA::N_LEA> meanLEA_formulaUnit;
    array<int, ComputeLEA::N_LEA> meanLEA_formulaSort;
    array<double, ComputeLEA::N_LEA> meanPowerErgEq;
    array<bool, LaserClassCW::N_CLASS> meanClassValutation;
    double C5;
    double meanDistCond_1;
    double meanDistCond_3;
    double meanApCond_1;
    double meanApCond_3;
    double meanBeamAtStop_Cond_1;
    double meanBeamAtStop_Cond_3;
    double meanCouplingFactor_Cond_1;
    double meanCouplingFactor_Cond_3;
    array<double, ComputeLEA::N_LEA> meanPowerErg_Cond_1;
    array<double, ComputeLEA::N_LEA> meanPowerErg_Cond_3;
    laserClass meanLaserClassAssigned;
    array<double, ComputeLEA::N_LEA> meanLEA_Corrected;

    laserClass c5LaserClassAssigned;
    array<bool, LaserClassCW::N_CLASS> c5ClassValutation;

    array<double, ComputeLEA::N_LEA> tiLEA_Value;
    array <string, ComputeLEA::N_LEA> tiLEA_formula;
    array <string, ComputeLEA::N_LEA> tiLEA_formulaTipo;
    array <string, ComputeLEA::N_LEA> tiLEA_formulaUnit;
    array <int, ComputeLEA::N_LEA> tiLEA_formulaSort;
    array<double, ComputeLEA::N_LEA> tiPowerErgEq;
    array<bool, LaserClassCW::N_CLASS> tiClassValutation;
    double tiDistCond_1;
    double tiDistCond_3;
    double tiApCond_1;
    double tiApCond_3;
    double tiBeamAtStop_Cond_1;
    double tiBeamAtStop_Cond_3;
    double tiCouplingFactor_Cond_1;
    double tiCouplingFactor_Cond_3;
    array<double, ComputeLEA::N_LEA> tiPowerErg_Cond_1;
    array<double, ComputeLEA::N_LEA>tiPowerErg_Cond_3;
    laserClass tiLaserClassAssigned;
    array<double, ComputeLEA::N_LEA>tiLEA_Corrected;
    array<double, ComputeLEA::N_LEA>LEA_Corrected;
    array <bool, LaserClassCW::N_CLASS> systemClassValutation;
    laserClass systemLaserClassAssigned;
};


#endif // LASERCLASSMP_H
