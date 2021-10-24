#ifndef LASERCLASSCW_H
#define LASERCLASSCW_H
#include "computelea.h"
#include <sstream>

using namespace std;

/*! \brief 	Classe per il calcolo della Classe di un laser ad onda conitnua con il metodo semplificato (CEI EN 60825-1 2017)
 *
 * La valutazione si svolge instanziando la Classe ComputeLEA.  Impostati i valori della lunghezza d'onda <b>&lambda;</b>,
 * della potenza di uscita <b>P</b>, della divergenza <b>&phi;</b> e del diametro del fascio <b>a</b> consente di ricavare la classe del dispositivo
 * invocando la funzione membro updateAll().
 * Quest'ultima funzione membro prevede inizialmente al calcolo dei valori di LEA dell'oggetto myLaserClass
 * invocando la funzione membro ad accesso protetto calculate() della Classe ComputeLEA. Dopo aver calcolato il diametro del fascio con computeBeamDiameter(),
 * si invoca la funzione membro classUpdate(laserOperation, const double&, const double &) che è la funzione membro che gestisce le varie
 * chiamate con le quali si ricava la CLasse del dispositivo.
 *
 *
 *
 *
 */

class LaserClassCW
{
public:
LaserClassCW(double=PUPIL_DIAMETER, double=POWER, double=DIVERGENCE, double=EmpLeaTables::HE_NE_WAVELENGTH,
             double=TIME_BASE, double=ALPHA);  /*!< L'istanza della classe prevede l'inserimento dei parametri:
<ul>
<li>diametro del fascio <b>a</b> in mm;</li>
<li>potenza <b>P</b> in Watt;</li>
<li>divergenza <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm.</li>
</ul>

La classe prevede inoltre un costruttore di default con i seguenti parametri:
<ul>
<li>diametro del fascio <b>a</b>= 7 mm;</li>
<li>Potenza <b>P</b>= 10 W;</li>
<li>divergenza <b>&phi;</b> = 1,5 mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> = 633 nm;</li>
<li>tempo base <b>t</b>= 0,25 s;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b>=1,5 mrad.</li>
</ul>
 */

const static size_t N_CLASS{7};
const static double PUPIL_DIAMETER;/*!< Parametro di default del diametro del fascio in mm. */
const static double POWER;/*!< Parametro di default della potenza ottica del dispositivo in Watt. */
const static double DIVERGENCE;/*!< Parametro di default della divergenza del fascio in mrad. */
const static double TIME_BASE;/*!< Parametro di default della base dei tempi in secondi. */
const static double ALPHA;/*!< Parametro di default dell'angolo sotteso dalla sorgente apparente in mrad. */

enum class laserOperation{CW,
    /**< Funzionamento ad onda continua. */
    PULSED,
    /**< Funzionamento ad impulso singolo. */
    MULTIPULSED
    /**< Funzionamento ad impulsi multipli. */
    };

enum class laserClass{CLASSE_1,
    /**< Laser di Classe 1 */
     CLASSE_1M,
    /**< Laser di Classe 1M */
     CLASSE_2,
    /**< Laser di Classe 2 */
     CLASSE_2M,
    /**< Laser di Classe 2M */
     CLASSE_3R,
    /**< Laser di Classe 3R */
     CLASSE_3B,
    /**< Laser di Classe 3B */
     CLASSE_4,
    /**< Laser di Classe 4 */
     NON_CLASSIFICATO
     /**< Laser non classificato. */
     };

    const static double PI; /*!<  Constante statica con valore pari a 3.141592653589793 */
    double getWavelength()const;/*!< Restituisce il valore della lunghezza d'onda <b>&lambda;</b> in nm.*/
    void setWavelength(const double&);/*!< Imposta il valore della lunghezza d'onda <b>&lambda;</b> in nm. */
    double getAlpha() const;/*!< Restituisce il valore dell'angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.*/
    void setAlpha(const double&);/*!< Imposta il valore dell'angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.
    * Non ha effetto alcuno sul calcolo della Classe avendo adottato la procedura semplificata. Si è deciso di considerare l'angolo per
    * futuri sviluppi.*/
    double getDivergence() const;/*!< Restituisce il valore della divergenza <b>&phi;</b> in mrad.*/
    void setDivergence(const double&);/*!< Imposta il valore della divergenza <b>&varphi;</b> in mrad. */
    double getBeamDiameter() const;/*!< Restituisce il valore del diametro del fascio a in mm.*/
    void setBeamDiameter(const double&);/*!< Imposta il valore del diametro del fascio <b>a</b> in mm. */
    void setTimeBase();/*!< Imposta la durata in secondi della base dei tempi secondo lo schema riportato di seguito:

    * <b>&lambda;</b> [nm]          | T<sub>b</sub> [s]
    * :--------------------- ------:|:------------------:
    *    400-700                    | 0,25
    *    180-400                    | 30000
    *    700-10<sup>6</sup>         | 100
    */

    double getTimeBase()const; /*!< Restituisce la base dei tempi in secondi */
    ComputeLEA getLEA_Data(); /*!< Restituisce l'oggetto ComputeLEA impiegato per il calcolo della Classe.*/

void computeBeamArea(); /*!< Calcola l'area del fascio laser in m<sup>2</sup> con
 * la relazione: \f[A_B=\frac{\pi \cdot \left(10^{-3} \cdot a \right)^2}{4}\f] */;
    double getBeamArea() const;/*!< Restituisce il valore dell'area del diametro del fascio a in m<sup>2</sup>.*/
    double getC1() const;/*!< Restituisce il valore numerico del parametro C<sub>1</sub>.*/
    double getC2() const;/*!< Restituisce il valore numerico del parametro C<sub>2</sub>.*/
    double getC3() const;/*!< Restituisce il valore numerico del parametro C<sub>3</sub>.*/
    double getC4() const;/*!< Restituisce il valore numerico del parametro C<sub>4</sub>.*/
    double getC5() const;/*!< Restituisce il valore numerico del parametro C<sub>5</sub>.*/
    double getC6() const;/*!< Restituisce il valore numerico del parametro C<sub>6</sub>.*/
    double getC7() const;/*!< Restituisce il valore numerico del parametro C<sub>7</sub>.*/
    double getT1() const;/*!< Restituisce il valore numerico del parametro T<sub>1</sub> in secondi.*/
    double getT2() const;/*!< Restituisce il valore numerico del parametro T<sub>2</sub> in secondi.*/
    double getGamma() const;/*!< Restituisce il valore dell'angolo <b>&gamma;</b> in mrad. L'angolo <b>&gamma;</b> ha significato solo per esposizioni
    * con effetti fotochimici. */

    string getRadiation() const;/*!< Restituisce il tipo di radiazione emessa dal dispositivo laser. */
    string getSkinDamage() const;/*!< Restituisce il tipo dei possibili danni alla pelle. */
    string getEyeDamage() const;/*!< Restituisce il tipo dei possibili danni all'apparato visivo. */

    array <double, ComputeLEA::N_LEA> getLEA()const;/*!< Restituisce i valori dei LEA delle Classi previste corrispondente ai valori impostati di <b>&lambda;</b> e t. */
    array <string, ComputeLEA::N_LEA> getLEA_Formula()const;/*!< Restituisce l'espressione della formula dei LEA delle Classi previste corrispondente ai valori impostati di <b>&lambda;</b> e t. */
    array <string, ComputeLEA::N_LEA> getLEA_FormulaUnit()const;/*!< Restituisce l'unità di misura dei LEA delle Classi previste corrispondente ai valori impostati di <b>&lambda;</b> e <b>t</b>. */
    array <string, ComputeLEA::N_LEA> getLEA_FormulaTipo()const;/*!< Restituisce le grandezze fisiche dei LEA delle Classi previste corrispondente ai valori impostati di <b>&lambda;</b> e <b>t</b>. */
    array <int, ComputeLEA::N_LEA> getLEA_FormulaSort()const;/*!< Restituisce il tipo di LEA delle Classi previste corrispondente ai valori impostati di <b>&lambda;</b> e <b>t</b>. */

    array <string, ComputeLEA::N_LEA> getLEA_Expressions()const;
    void setPowerErg(const double&);/*!< Imposta il valore di powerErg se diverso da quello già impostato. */
    double getPowerErg() const;/*!< Restituisce il valore di powerErg in Watt. */
    double valuateBeamDiameterAtStop(const double&, const double&);/*!< Calcola il diametro del fascio in mm alla distanza del diaframma impostata secondo
    * la relazione approssimata:
    * \f[a_B = a + \varphi \cdot d\f]
    */
    double valuateCouplingFactor(const double&, const double&, bool);/*!< Calcola il valore del fattore di accoppiamento assegnati il diametro del diaframma
    * ed il diametro del fascio per la distanza prevista.
    * Nel caso in cui 302,5 < <b>&lambda;</b> < 4000 nm si imipega la formula, valida per fasci gaussiani: \f[\eta= 1-e^{-\left(\frac{d_a}{db}\right)^2}\f]
    * in caso contrario il valore restitutito è pari a 1 (accoppiamento perfettto). */
    void classUpdate(laserOperation, const double&, const double &);/*!< Effettua l'aggiornamento dei parametri che vengono successivamente
    * impiegati per la valutazione della classe.
    * Nel caso in cui il punto di riferimento della sorgente non sia noto la valutazione viene effettuata nel punto più vicino di accesso umano quindi
    * la distanza per la 3 <sup>a</sup> condizione va impostata a 0. Per fare ciò prima dell'aggiornamento sarà necessario impostare la variabile internalWaist
    * al valore vero usando la funzione membro setInternalWaist(bool) (a tale proposito si consulti anche getDistCond_3()).*/

    array <double, ComputeLEA::N_LEA> leaPowerErgUnit(laserOperation myLaserOperation, array <int, ComputeLEA::N_LEA>, const double & time, const double& _powerErg);/*!< E' la prima chiamata della funzione membro
    * classUpdate e restituisce un puntatore ad un array di double lungo 4 nel quale sono memorizzati i valori dell'uscita del dispositivo espressi
    * nell'unità di misura omogenee al LEA corrispondente. Il parametro time è la base dei tempi in secondi.
    *     Unità di Misura LEA  | Uscita
    * :-----------------------:|:------------------:
    *    W/m<sup>2</sup>       | potenza/area fascio
    *    J/m<sup>2</sup>       | potenza &sdot; base dei tempi /area fascio
    *    W                     | potenza
    *    J                     | potenza &sdot; base dei tempi
    * L'array ricavato con questa logica consente di confrontare l'uscita del dispositivo con i LEA.
    * La funzione membro è ad accesso protetto e non è necessario impiegarla, ma è necessaria all'implementazione dell classi derivate.
    */
    array<bool, N_CLASS> valuateLEA_forClass(array <double, ComputeLEA::N_LEA>, array <double, ComputeLEA::N_LEA>, array <double, ComputeLEA::N_LEA>);/*!< Opera il confronto di ciascun LEA con le uscite corrette per la 1<sup>a</sup>
    * e la 3<sup>a</sup> condizione e fornisce un array di tipo booleano i cui elementi risultano veri se le condizioni per la classificazione
    * sono verificate.
    * Il significato dell'array fornito è specificato dalla tabella seguente.
    *
    *  elemento |Classe 1|Classe 1M|Classe 2|Classe 2M|Classe 3R|Classe 3B|Classe 4
    * :--------:|:------:|:-------:|:------:|:-------:|:-------:|:-------:|:--------:
    *  [0]      |true    |false    |false   |false    |false    |false    |false
    *  [1]      |false   |true     |false   |false    |false    |false    |false
    *  [2]      |false   |false    |true    |false    |false    |false    |false
    *  [3]      |false   |false    |false   |true     |false    |false    |false
    *  [4]      |false   |false    |false   |false    |true     |false    |false
    *  [5]      |false   |false    |false   |false    |false    |true     |false
    *  [6]      |false   |false    |false   |false    |false    |false    |true
    */

    laserClass valuateClass(array <bool, N_CLASS>);/*!< Restituisce il risultato della classificazione come enumerale LaserClassCW::laserClass.
    * Se il formato non è quello precisato in valuateLEA_forClass(double*, double*, double*) restituisce LaserClassCW::NON_CLASSIFICATO. */

    array <double, ComputeLEA::N_LEA> computePowerErgCond_1(array <double, ComputeLEA::N_LEA> , const double &); /*!< Calcola il valore dell'uscita del laser corretta con il fattore di accoppiamento.
    * Ad esempio nel caso in cui il LEA è espresso in Watt fornisce: \f[P_{ACC 1} = \eta_1 \cdot P\f] */
    array <double, ComputeLEA::N_LEA> computePowerErgCond_3(array <double, ComputeLEA::N_LEA> , const double &);/*!< Calcola il valore dell'uscita del laser corretta con il fattore di accoppiamento.
    * Ad esempio nel caso in cui il LEA è espresso in Watt fornisce: \f[P_{ACC 3} = \eta_3 \cdot P\f] */

    array <double, ComputeLEA::N_LEA> getPowerErg_Cond_1()const;/*!< Restituisce l'uscita del dispositivo corretta con il fattore di accoppiamento della 1<sup>a</sup> condizione. */
    array <double, ComputeLEA::N_LEA> getPowerErg_Cond_3()const;/*!< Restituisce l'uscita del dispositivo corretta con il fattore di accoppiamento della 3<sup>a</sup> condizione. */

    double getCouplingFactor_Cond_1()const;/*!< Restituisce il fattore di accoppiamento della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation, const double&, const double &). */
    double getCouplingFactor_Cond_3()const;/*!< Restituisce il fattore di accoppiamento della 3<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation, const double&, const double &). */

    double getDistCond_1()const;/*!< Restituisce il valore in mm della distanza dal diaframma della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation, const double&, const double &);. */
    double getDistCond_3()const;/*!< Restituisce il valore in mm della distanza dal diaframma della 3<sup>a</sup> condizione. La distanza è quella
    * dal punto di riferimento inteso nella modalità indicate di seguito (CEI EN 60825-1 2017):
    *         Tipo di apparecchio      |  Punto di riferimento
    * :-------------------------------:|:---------------------------------:
    *   Emettitori a semiconduttori    | Posizione fisica del chip che effettua l'emissione
    *   Emissioni a scansione          | Vertice della scansione
    *   Laser di allineamento          | Punto focale della linea
    *   Uscita della fibra             | Punta della fibra
    *   Sorgenti completamente diffuse | superficie del diffusore
    *   Altri                          | Punto di raggio minimo del fascio (waist)
    * Nel caso in cui la sorgente non sia inclusa nell'elenco (altri) la valutazione è effettuata nel punto più vicino di accesso umano quindi
    * la distanza va impostata a 0. Per fare ciò sarà necessario impostare la variabile internalWaist al valore vero usando la funzione membro
    * setInternalWaist(bool).
    * L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation, const double&, const double &). */

    double getApCond_1()const;/*!< Restituisce il valore in mm del diametro dal diaframma della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation, const double&, const double &). */
    double getApCond_3()const;/*!< Restituisce il valore in mm del diametro dal diaframma della 1<sup>a</sup> condizione. L'impiego della funzione
    * presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation, const double&, const double &);. */

    double getBeamAtStop_Cond_1()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 1<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation,
    * const double&, const double &);. */
    double getBeamAtStop_Cond_3()const;/*!< Restituisce il valore in mm del diametro del fascio del dispositivo laser relativamente alla 3<sup>a</sup>
    * condizione. L'impiego della funzione presupppone che in precedenza sia stata invocata la funzione membro protetta classUpdate(laserOperation,
    * const double&, const double &);. */

    bool isInternalWaist();/*!< Restituisce lo stato di internal waist ed è vero se il waist del fascio è interno e non accessibile, falso viceversa. */
    void setInternalWaist(bool);/*!< Imposta lo stato del waist nel senso della posizione che può essere nota oppure non accessibile. */
    laserClass getLaserClass()const;/*!< Restituisce la Classe del dispositivo. */
    void updateAll();/*!< Calcola i valori dei LEA dell'oggetto myLaserClass, ricava, calcola l'area della sezione trasversale del fascio,
    * aggiorna la valutazionde la classe. */

protected:
    double wavelength;
    ComputeLEA myLaserClass;
    double alpha;
    double powerErg;
    double divergence;
    double beamDiameter;
    double beamArea;
    bool internalWaist;

    array<double, ComputeLEA::N_LEA> LEA_Value;
    array<string, ComputeLEA::N_LEA> LEA_formula;
    array<string, ComputeLEA::N_LEA> LEA_formulaTipo;
    array<string, ComputeLEA::N_LEA> LEA_formulaUnit;
    array<int, ComputeLEA::N_LEA> LEA_formulaSort;
    array<double, ComputeLEA::N_LEA>powerErgEq;
    array<bool, N_CLASS>classValutation;

    double distanceCond_1;
    double distanceCond_3;
    double apertureStopCond_1;
    double apertureStopCond_3;

    double beamAtStop_Cond_1;
    double beamAtStop_Cond_3;

    double couplingFactor_Cond_1;
    double couplingFactor_Cond_3;

    double distCond_1;
    double distCond_3;

    double apCond_1;
    double apCond_3;

    array<double, ComputeLEA::N_LEA> powerErg_Cond_1;
    array<double, ComputeLEA::N_LEA> powerErg_Cond_3;

    laserClass laserClassAssigned;

    double C1;
    double C2;
    double C3;
    double C4;
    double C5;
    double C6;
    double C7;
    double T1;
    double T2;
    double t_exp;
    double Gamma;
    string Radiation;
    string SkinDamage;
    string EyeDamage;
    string Note;
    string FormulaSort;
    double timeBase;
};


#endif // LASERCLASSCW_H
