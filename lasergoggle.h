#ifndef LASERGOGGLE_H
#define LASERGOGGLE_H

#include <vector>
#include <utility>
#include <functional>
#include <string>

using namespace std;

/*! \brief 	Classe per il calcolo della marcatura dei protettori ottici con il metodo indicato dalla norma UNI EN 207
 *
 * La classe consente la determinazione della marcatura dei protettori ottici per laser in qualsiasi regime di funzionamento.
 * Nel caso di laser ad impulsi multipli sarà necessario instanziare due oggetti della classe, per la valutazione
 * del criterio degli impulsi, l'altro con l'applicazione del criterio della potenza media.
 * L'istanza della classe prevede due costruttori.
 * Più precisamente:
 * <ul>
 * <li>nel caso di funzionamento in onda continua si impiega il costruttore a 5 parametri con la durata dell'impulso impostata a CONTINUOS_OPERATION;</li>
 * <li>nel caso di funzionamento a singolo impulso si impiega il costruttore a 5 parametri con la durata dell'impulso impostata ala durata
 * dell'impulso;</li>
 * <li>nel caso di funzionamento ad impulsi multipli si impiega il costruttore a 5 parametri con la durata dell'impulso impostata ala durata
 * dell'impulso, si impiega inoltre il costruttore a 4 parametri con la durata dell'impulso pari a CONTINUOS_OPERATION.</li>
 * </ul>
 * E' importante tenere presente che:
 * <ul>
 * <li>il primo costruttore prevede l'impostazione della PRF che nel caso di funzionamento ad onda continua
 * ed impulsato dovrà essere posta a zero mentre nel caso dell'applicazione del criterio dell'impulso dovrà essere impostato al valore di
 * funzionamento;</li>
 * <li>il secondo costruttore non prevede l'impostazione della frequenza in quanto per l'applicazione del criterio della potenza media
 * l'effetto della frequenza è rilevante solo per la potenza media che può essere impostata con la funzione membro setPowerErg(const double &).</li>
 * </ul>
 */

class LaserGoggle
{
public:
    const static int TIMEBASE;/*!< Base dei tempi per le lunghezze d'onda da 315 a 10<sup>6</sup> nm pari a 5 secondi. */
    const static int TIMEBASE_LOW_WAVELENGTH;/*!< Base dei tempi per le lunghezze d'onda da 180 a 315 nm pari a 30000 secondi. */
    const static int TABLEROWS;/*!< Numero di righe del prospetto B1 della norma UNI EN 207. */
    const static double PI;/*!< Valore della costante <b>&pi;</b>. */
    const static int CONTINUOS_OPERATION;/*!< Funzionamento ad onda continua. */
    const static double GLASS_EXPONENT;/*!< Esponente per la correzione per filtro con materiale principale di vetro e pari a 1,1693
    * (correzione applicata anche quando il diametro del fascio risulta pari a 1 mm.*/
    const static double PLASTIC_EXPONENT;/*!< Esponente per la correzione per filtro con materiale principale di plastica e pari a 1,2233
    * (correzione applicata anche quando il diametro del fascio risulta pari a 1 mm.*/

    const static int HE_NE_WAVELENGTH; /*!< Lunghezza d'onda dell'emissione del laser Elio Neon pari a 633 nm.*/
    const static double PULSE_WIDTH; /*!< Parametro di default della durata dell'impulso in secondi. */
    const static double POWER_ENERGY;/*!< Parametro di default della potenza ottica del dispositivo in Watt o dell'energia dell'impulso in Joule
a seconda della modalità di funzionamento, CW o ad impulso. */
    const static double BEAM_DIAMETER;/*!< Parametro di default del diametro del fascio in mm. */

    enum material{ONLY_REFLECTOR,
    /**< Riflettori semplici. */
    GLASS,
    /**< Filtro con materiale principale di vetro. */
    PLASTIC
    /**< Filtro con materiale principale di plastica. */};

    enum laserOperation{CONTINUOS_WAVE,
    /**< Funzionamento ad onda continua. */
    IMPULSATO,
    /**< Funzionamento impulsato. */
    IMPULSI_MULTIPLI
    /**< Funzionamento ad impulsi multipli. */
    };

    enum laserPulseOperation{CONTINUOS_EMISSION,
    /**< D, Laser ad onda continua (CEI EN 207 Prospetto 4). */
    SIMPLE_PULSED,
    /**< I, Laser ad impulsi (CEI EN 207 Prospetto 4). */
    Q_SWITCHED,
    /**< R, Laser ad impulsi Q switch (CEI EN 207 Prospetto 4). */
    MODE_LOCKED
    /**< M, Laser ad impulsi a modo accoppiato (CEI EN 207 Prospetto 4).. */
    };
    LaserGoggle(int, double, double, double, double);/*!< Costruttore utile nel caso di laser CW, impulsato e ad impulsi multipli per
    l'applicazione del criterio dell'impulso.
    <ul>
    <li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
    <li>durata dell'impulso <b>t</b> in secondi;</li>
    <li>potenza <b>P</b> in W o energia dell'impulso <b>Q</b> in <b>J</b>;</li>
    <li>diametro del fascio <b>a</b> in m<sup>2</sup>;</li>
    <li>frequenza di ripetizione degli impulsi <b>PRF</b> in Hz.<li>
    </ul>
    Se il parametro relativo alla durata dell'impulso viene passato con valore pari a CONTINUOS_OPERATION (uguale a 0) allora la durata
    dell'impulso viene impostata alla base dei tempi corrispondente.
 */

    LaserGoggle(int, double, double, double);/*!< Versione overload del costruttore utile nel caso di laser ad impulsi multipli per
    l'applicazione del criterio della potenza media.
    <ul>
    <li>la lunghezza d'onda <b>&lambda;</b> in nm;</li>
    <li>la durata dell'impulso <b>t</b> in secondi;</li>
    <li>la potenza <b>P</b> in W o l'energia dell'impulso <b>Q</b> in <b>J</b>;</li>
    <li>il diametro del fascio <b>a</b> in m<sup>2</sup>;</li>
    <li>la frequenza di ripetizione degli impulsi <b>PRF</b> in Hz.<li>
    </ul>
    Se il parametro relativo alla durata dell'impulso viene passato con valore pari a CONTINUOS_OPERATION (uguale a 0) allora la durata
    dell'impulso viene impostata alla base dei tempi corrispondente.*/

    ~LaserGoggle();/*!< Distruttore */

    string goggleMark(int, double, double, double, double);/*!< E' la funzione membro che consente il calcolo diretto della della marcatura
    una volta instanziato correttamente la classe. Prevede il passagio dei parametri:
    <ul>
    <li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
    <li>durata dell'impulso <b>t</b> in secondi;</li>
    <li>la potenza <b>P</b> in Watt o l'energia dell'impulso <b>Q</b> in J a seconda dei casi;</li>
    <li>il diametro del fascio <b>a</b> in mm;</li>
    <li>la frequenza di ripetizione degli impulsi <b>PRF</b> in hz.</li>
    </ul>
 */
    string goggleMark();/*!< Consente il calcolo diretto della della marcatura dopo l'istanza della classe e l'impostazione dei parametri:
    <ul>
    <li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
    <li>durata dell'impulso <b>t</b> in secondi;</li>
    <li>la potenza <b>P</b> in Watt o l'energia dell'impulso <b>Q</b> in J a seconda dei casi;</li>
    <li>il diametro del fascio <b>a</b> in mm;</li>
    <li>la frequenza di ripetizione degli impulsi <b>PRF</b> in hz.</li>
    </ul>
 */
    int getWavelength();/*!< Restituisce la lunghezza d'onda <b>&lambda;</b> impostata in nm. */
    void setWavelength(const double &);/*!< Imposta il valore della lunghezza d'onda <b>&lambda;</b> in nm. */
    double getPulseWidth();/*!< Restituisce la durata dell'impulso <b>t</b> o la base dei tempi impostata in secondi. */
    void setPulseWidth(const double &);/*!< Imposta la durata dell'impulso <b>t</b> o la base dei tempi a seconda del modo di funzionamento
secondo i criteri illustrati di seguito:

<table>
<tr><th>onda continua o criterio della potenza </th><th>impulsato</th></tr>
<tr><td>Imposto il valore 0.</td><td>Imposto la durata dell'impulso.</td></tr>
<tr><td>Il tempo impostato è: <br>
<ul>
<li>5 s se 180 < <b>&lambda;</b> <= 315 nm <br></li>
<li>30000 s se 180 < <b>&lambda;</b> <= 315 nm </li>
</ul>
</td><td>Il tempo impostato è la durata dell'impulso </td></tr>
</table> */

    void setMaterial(material typeOfMaterial);/*!<Imposta il materiale principale del filtro.
<table>
<tr><th>Valore impostato</th><th>Materiale</th>
<tr><td>ONLY_REFLECTOR</td><td>Riflettori semplici</td>
<tr><td>GLASS </td><td>Vetro</td>
<tr><td>PLASTIC</td><td>Riflettori semplici</td>
</table>

    */
    double getPowerErg();/*!< Restituisce la potenza <b>P</b> in Watt o l'energia dell'impulso <b>Q</b> in J. */
    void setPowerErg(const double &);/*!< Imposta la potenza <b>P</b> in W o la durata dell'impulso <b>Q</b> in J.*/
    double getBeamDiameter();/*!< Restituisce il diametro del fascio <b>a</b> in m<sup>2</sup>. */
    void setBeamDiameter(const double &);/*!< Imposta il diametro del fascio <b>a</b> in m<sup>2</sup>. */
    double getFrequency();/*!< Restituisce la frequenza di ripetizione degli impulsi <b>PRF</b> in Hz. */
    void setFrequency(const double &);/*!< Imposta la frequenza di ripetizione degli impulsi <b>PRF</b> in Hz. */
    int getScaleNumber()const;/*!< Restituisce il numero di scala dei protettori. */
    int getNumberOfPulse()const;/*!< Restituisce il numero di impulsi che si verseicano nella base dei tempi. */
    double getCoefficient_k()const;/*!< Restituisce il coefficiente di correzione per gli effetti termici nel caso di applicazione del criterio
    * degli impulsi per laser ad impulsi multipli. */
    double getCoefficient_ki()const;/*!< Restituisce il coefficiente di correzione per funzionamento ad alta frequenza per laser ad impulsi multipli. */
    double get_ni_max()const;/*!< Restituisce il massimo valore di frequenza oltre il quale è necessario correggere gli effetti dovuti al funzionamento
    * ad alta frequenza.*/
    string getCodeUnit();/*!< Restituisce l'unità di misura in cui è espresso il limite del Prospetto B1 in funzione delle lettere (D, I, R, M) che
    * che definiscono le condizioni di prova dei protettori. */
    void setLaserOperation();/*!< Imposta la modlità di funzionamento del dispositivo laser così come codificato dalla norma UNI EN 207.

     Condizioni di prova per tipo di laser | Tipo di laser                      | Durata dell'impulso [s]                | Numero minimo di impulsi
    :-------------------------------------:|:----------------------------------:|:--------------------------------------:|:--------------------------:
    D                                      | Laser a onda continua              |  5                                     |  1
    I                                      | Laser a impulsi                    |  Da 10<sup>-6</sup> a 0,25             |  50
    R                                      | Laser Q Switch                     |  Da 10<sup>-9</sup> a 10<sup>-6</sup>  |  50
    M                                      | Laser a impulsi a modo accoppiato  |  < 10<sup>-9</sup>                     |  50
    */
    laserOperation getLaserOperation();/*!< Restituisce la modlità di funzionamento del dispositivo laser così come codificato dalla norma UNI EN 207 */
    void setPulseCode();/*!< Imposta il codice di prova dell'occhiale ed la descrizione in funzione del risultato della valutazione effettuatail codice */
    string getLaserPulseOperation();/*!< Restituisce la descrizione del tipo di protettori calcolato."*/

    double* selectData(const double &, const double &);/*!< Restituisce un array contenente la riga del prospetto B1 UNI EN 207
    corrispondente  ai valori di lunghezza d'onda <b>&lambda;</b> e di tempo applicabile in seondi impostati. */
    vector< pair <int,double> > buildDataVector(const double[], const int[]);/*!< Costruisce e restituisce un vettore STD il cui elemento è
    la coppia LB valore limite dell'array ottenuta da double* selectData(const double &, const double &). */
    vector< pair <int,double> > getDataVector();/*!< Restituisce un vettore STD il cui elemento è la coppia LB, valore limite. */
    int scaleNumber(vector< pair <int,double> > dataVector, double);/*!< Calcola e restituisce il numero di gradazione dei protettori assegnato
    il vettore colonna del prospetto B1 ed il valore della grandezza relativa all'esposizione alla radiazione.*/
    double pulseTrainCorrectionK();/*!< Applica la correzione per gli effetti termici nel caso di impulsi multipli con la formula:
    * \f[k = N^{0,25}\f]
    * dove N è il numeo di impulsi che si verseicano nel tempo base.
    */
    double get_Ki();/*!< Restituisce la correzione nel caso di impulsi multipli ad alta frequenza. */
    double frequencyCorrection();/*!< Applica e restituisce la correzione nel caso di impulsi multipli ad alta frequenza usando la formula:
    * \f[k' = \frac{PRF}{T_i}\f]
    * dove N è il numeo di impulsi che si verseicano nel tempo base. */
    double laserIrrRad(double powerErg);/*!< Applica e restituisce il valore dell'irradianza o dell'esposizione radiante assegnato la potenza
    * <b>P</b> o l'energia dell'impulso <b>Q</b>. Considera nel calcolo il coefficiente di correzione relativo al materiale del filtro. */
    double meanPower();/*!< Calcola e restituisce il valore della potenza media per l'applicazione del criterio della potenza media:
    * \f[P_m=PRF \cdot Q\f] */
    int numberOfPulse();/*!< Imposta e restituisce il numero degli impulsi che si verseicano nel tempo base corrispondente. Se il funzionamento è
    ad impulsi multipli il numero di impulsi minimo è pari a 50*/
    int* buildScaleNumbers();/*!< Costruisce e restituisce un array di interi con i valori 1, 2, 3, 4, 5, 6, 7, 8, 9 e 10 corrispondenti ai
    * numeri di gradazione*/
    double laserIrrRadCorrected(double);/*!< Imposta e restitusce la radianza corretta del dispositivo nel caso di applicazione del criterio dell'impulso
    * moltiplicando il valore ottenuto della radianza con laserIrrRad(_laserOutput) per i fattori K e K<sub>i</sub>.*/

    void prepareGoggleMark();/*!< Esegue le seguenti operazioni:
    <ul>
    <li>memorizza il campo del prospetto B1 pertinente ai parametri impostati;</li>
    <li>costruisce il vettore STD delle gradazioni;</li>
    <li>calcola la radianza/irradianza a seconda dei casi tenendo in conto del materiale principale del filtro e nel caso di impulsi multipli effettua
    la correzione termica;</li>
    <li>calcola e memorizza il numero di scala dei protettori.</li>
    </ul>*/

    string outputSort();/*!< Restituisce la grandezza fisica in cui è espresso il limite del Prospetto B1 in funzione delle lettere (D, I, R, M) che
    * che definiscono le condizioni di prova dei protettori. */

    void printData(); /*!<Restituisce nello stream di output gli elementi della colonna del prospetto B1 selezionata. */
    void printScaleNumbers(int[]);/*!<Restituisce nello stream di output gli elementi del vettore dei numeri di scala. */
    void printVector( vector< pair <int,double> >);/*!<Restituisce nello stream di output gli elementi del vettore del prospetto B1 di interesse. */

private:
    string printGoggleCode();/*!< Funzione membro impiegata per il calcolo della marcatura.*/
    double *expositionData;
    vector<pair<int, double>> dataVector;
    double myExpositionValue;
    double thermicExpositionValue;
    bool pulseTrain;
    double powerErg;
    double wavelength;
    double pulseWidth;
    double beamDiameter;
    double materialCorrection;

    double irrRad;
    double myMeanPower;
    int numberOfPulses;
    string myGoggleCode;
    double frequency;
    double ki;
    double k;
    int myScaleNumber;
    int *LB_Scale;
    double Ti;
    double ni_max;
    string codeUnit;
    laserPulseOperation pulseCode;
    string pulseCodeString;
    char pulseCodeLetter;
    string laserOutputSort;
    string goggleCode;
    laserOperation myLaserOperation;
};

#endif // LASERGOGGLE_H
