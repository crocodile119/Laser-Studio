#ifndef LASERSAFETYMP_H
#define LASERSAFETYMP_H
#include "lasersafetycw.h"
using namespace std;

/*! \brief 	Classe per il calcolo dell'Eposizione Massima Permessa, <b>EMP</b> e della Distanza Nominale di Sicurezza Ottica, <b>DNRO</b>, di un laser
 * ad impulsi multipli con il metodo previsto della parte 2 dell'Allegato XXXVII del D.Lgs 81/2008.
 *
 * La Classe è derivata dalla classe LaserSafetyCW ed aggiunge un consistente numero di funzioni membro che consentono di:
 <ul>
 <li>Applicare la valutazione del criterio della potenza media;</li>
 <li>Considerare gli effetti termici nella valutazione del'EMP nella banda da 400<&lambda;< 10^6 nm;</li>
 <li>Valutare l'<b>EMP</b> nel caso di impulsi ad alta frequenza di ripetizione;</li>
  <li>Valutare il più piccolo valore di <b>EMP</b> applicabile (quellopiù gravoso) e il valore massimo della DNRO.</li>
 </ul>
 La Classe prevede l'impiego di tre istanze della classe ComputeEMP():
  <ul>
 <li>una dedicata all'impulso singolo per l'applicazione del criterio dell'impulso e alla valutazione degli effetti termici ;</li>
 <li>una dedicata agli effetti medi per l'applicazione degli effetti medi;</li>
 <li>una dedicata all'impulso singolo per l'applicazione del criterio dell'impulso e alla valutazione degli effetti termici
 nel caso di funzionamento ad alta frequenza.</li>
 </ul>
 Per procedere al calcolo dell'<b>EMP</b> sarà necessario invocare la funzione membro LaserUpdate(). */

class LaserSafetyMP : public LaserSafetyCW
{
public:
LaserSafetyMP(double=PULSE_REPETITION_FREQUENCY, double=BEAM_DIAMETER, double=POWER, double=DIVERGENCE, double=EmpLeaTables::HE_NE_WAVELENGTH,
              double=EmpLeaTables::NATURAL_AVERSION_TIME, double=EmpLeaTables::ALPHA_MIN);/*!< L'istanza della classe prevede l'inserimento dei parametri:
<ul>
<li>frequenza di ripetizione degli impulsi <b>PRF</b> in Hz;</li>
<li>diametro del fascio <b>a</b> in mm;</li>
<li>potenza <b>P</b> in Watt;</li>
<li>divergenza <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
<li>durata dell'impulso <b>t</b> in secondi;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.</li>
</ul>
La durata dell'impulso riguarda la durata del singolo impulso laser, mentre la durata di esposizione è impostata in
modo programmatico secondo il criterio mostrato in seguito:
 Lunghezza d'onda [nm]           | t [s]
:-------------------------------:|:----------:
 400 &le; &lambda; &le; 700 nm   | 0,25
 &lambda; > 700 nm               | 10
 180 < &lambda; < 700 nm         | 30000

La classe prevede inoltre un costruttore di default con i seguenti parametri:
<ul>
<li>frequenza di ripetizione degli impulsi <b>PRF</b>= 10 Hz;</li>
<li>diametro del fascio <b>a</b>= 7 mm;</li>
<li>energia dell'impulso <b>Q</b>= 10<sup>-3</sup> J;</li>
<li>divergenza <b>&phi;</b> = 1 mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> = 633 nm;</li>
<li>durata dell'impulso <b>t</b>= 0,25 s;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b>=1,5 mrad.</li>
</ul>
*/

const static double PULSE_REPETITION_FREQUENCY; /*!< Parametro di default della frequenza di ripetizione degli impulsi in Hz. */

void setWavelength(const double& _wavelength);/*!< &Egrave; un overload di funzione. Imposta il valore della lunghezza d'onda <b>&lambda;</b> in nm per l'applicazione
* del criterio della potenza media e degli impulsi ad alta frequenza, aggiorna se necessario il dato membro e la lunghezza d'onda per
* la parte relativa alla valutazione del criterio dell'impulso.*/
void setAlpha(const double& _alpha);/*!< &Egrave; un overload di funzione. Imposta il valore dell'angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad per l'applicazione
* del criterio della potenza media e degli impulsi ad alta frequenza, aggiorna se necessario il dato membro e la lunghezza d'onda per
* la parte relativa alla valutazione del criterio dell'impulso. */

void setPulseWidth(const double& _pulseWidth);/*!< &Egrave; un overload di funzione. Imposta il valore del tempo di esposizione <b>T<sub>e</sub></b> in secondi per
* l'applicazione del criterio della potenza media, imposta il valore della durata minima dell'impulso <b>T<sub>min</sub></b> nel caso
* di impulsi ad alta frequenza, aggiorna se necessario il dato membro riguardante la durata del singolo impulso <b>t</b> per la parte
* relativa alla valutazione del criterio dell'impulso. */
void computeNOHD();/*!< &Egrave; un overload di funzione.  Calcola la <b>DNRO</b> in metri considerando la massima distanza di sicurezza tra quelle dovuta all'applicazione
del criterio dell'impulso, quella ottenuta dal criterio della potenza e quella ottenuta, nel caso di 400 &le; <b>&lambda;</b> &le; 10<sup>6</sup>
\f[DNRO=max \left(DNRO_{singolo}, DNRO_{medio}, DNRO_{treno} \right)\f]
per l'applicazione della correzione termica. La <b>DNRO</b> sarebbe potuta essere calcolata dal minimo valore dell'<b>EMP</b> con la nota formula per il calcolo della <b>DNRO</b>*/

void computeLambertianMax();/*!< &Egrave; un overload di funzione. Calcola il valore massimo della distanza di sicurezza espressa in metri di una eventuale riflessione puntiforme
* su di una superficie diffondente con coefficiente di riflessione <b>&rho;</b> unitario e ciò valutando il minimo tra le medesime distanze calcolate
* con l'applicazione del criterio dell'impulso , del criterio della potenza e del criterio dell'impulso con correzione termica.
    \f[Lamb_{max}=max \left(Lamb_{max singolo}, Lamb_{max medio}, Lamb_{max treno} \right)\f]
*/
void laserUpdate();/*!< &Egrave; un overload di funzione. Impostati i parametri necessari effettua il calcolo dell'<b>EMP</b> del dispositivo, della <b>DNRO</b> e delle distanza
massima da riflessione diffondente
*/
void computeMeanPower();/*!< Calcola la potenza ottica media espressa in Watt del dispositivo secondo la nota relazione:
    \f[P_{media}=PRF \cdot Q\f] */
void computeMeanPowerIrradiance();/*!< Calcola l'irradianza della potenza ottica media del dispositivo espressa in W/m<sup>2</sup>
    secondo la nota relazione:
    \f[E_{media}=\frac{4 \ \ P_{media}}{\pi \ \ a^2}\f] */
void computeCP();/*!< Calcola il valore del fattore <b>C<sub>P</sub></b> per la correzione termica necessario prevista per
     400 \le &lambda; 10<sup>6</sup> nell'applicazione del criterio dell'impuso.
     Il fattore  <b>C<sub>P</sub></b> è dato dalla formula:
     *\f[C_P=N^{-0,25}\f] */

void computePulseNumber();/*!< La funzione calcola il numero degli impulsi <b>N</b> del treno.
    * Nel caso in cui 400 &le; &lambda &le; 10<sup>6</sup> se PRF &le; 1/T<sub>min</sub> il conteggio è regolare ed il numero di impulsi
    * è pari al prodotto della <b>PRF</b> del minimo tra <b>T<sub>2</sub></b> ed il tempo di esposizione <b>T<sub>e</sub></b>:
    *           \f[N=PRF \cdot min \left( T_e, T_2 \right)\f]
    *
    * se PRF > 1/T<sub>min</sub> il conteggio non è regolare ed il numero di impulsi è pari al prodotto di <b>1/T<sub>min</sub></b>
    * per il minimo  tra <b>T<sub>2</sub></b> ed il tempo di esposizione <b>T<sub>e</sub></b>:
    *            \f[N=\frac{1}{T_{min}} \cdot min \left( T_e, T_2 \right)\f]
    *
    * Se infine la lunghezza d'onda è tale che &lambda &le; 400 il numero di impulsi <b>N</b> è fornito dalla relazione:
    *           \f[N=PRF \cdot T_e\f]
    *In tutti i casi illustrati il valore di <b>N</b> viene arrotondato all'intero immediatamente più grande.
*/
void computeMeanPowerEMP();/*!< Calcola il valore dell'<b>EMP</b> con il criterio della potenza riferito all'impulso medio. La modalità con
    * cui cui valore dell'<b>EMP</b> è ricavato dipende dall'unità di misura del valore dell'<b>EMP</b> relativo al tempo di esposizione
    * <b>T<sub>e</sub></b>.
     In particolare:
                  EMP in T<sub>e</sub> | Formula impiegata
                :---------------------:|:-------------------------------------------------------------------------:
                   H<sub>Tmedio</sub>  | H<sub>Impulso medio</sub> =H<sub>Tmedio</sub> / (PRF &sdot; T<sub>e</sub>)
                   E<sub>Tmedio</sub>  | H<sub>Impulso medio</sub> E<sub>Tmedio</sub> / (PRF &sdot; t)
     */
void computeSP_CP_EMP();/*!< Calcola i valori dell'<b>EMP</b> con il criterio dell'impulso.
    Nel caso di 400 &le; <b>&lambda;</b> &le; 10<sup>6</sup> apporta le correzioni termiche secondo il punto 3
    del paragrafo <i>correzioni ripetute</i> dell'allegato XXXVII parte 2 del cià citato D. Lgs 81/2008.
    Più precisamente se <b>PRF</b> >(1/<b>T<sub>min</sub></b>) allora il valore dell'<b>EMP</b> viene ricavato
    con la formula:

    \f[H_{treno}=C_P \cdot H_{impulso}\f]
    oppure
    \f[E_{treno}=C_P \cdot E_{impulso}\f]

    dove <b>H<sub>impulso</sub></b> / <b>E<sub>impulso</sub></b> è il valore dell'<b>EMP</b> relativo alla durata <b>t</b>.
    Vicevesa nel caso in cui <b>PRF</b> >(1/<b>T<sub>min</sub></b>) la durata del singolo impulso sarà pari a <b>T<sub>min</sub></b>
    quindi dove <b>H<sub>Tmin</sub></b> è il valore dell'<b>EMP</b> relativo alla durata <b>T<sub>min</sub></b>, inoltre poichè
     il valore dell'EMP così calcolato si riferisce ad un impulso di energia/potenza <b>PRF</b> &sdot; <b>T<sub>min</sub></b> volte più grande
     il risultato va diviso proprio per <b>PRF</b> &sdot; <b>T<sub>min</sub></b>, pertanto:

    \f[H_{treno}=C_P \cdot; \frac{H_{Tmin}}{PRF \cdot T_{min}}\f]
    oppure
    \f[E_{treno}=C_P \frac{E_{Tmin}}{PRF \cdot T_{min}}\f]

    dove <b>H<sub>Tmin</sub></b> / <b>E<sub>Tmin</sub></b> è il valore dell'<b>EMP</b> relativo alla durata <b>T<sub>min</sub></b>.
    Nel caso in cui <b>&lambda;</b> < 400 nm il valore dell'<b>EMP</b> relativo alla correzione termica non va valutato, il suo valore
    è pertanto posto uguale a zero.
*/
double returnMultiPulse_EMP();/*!< Calcola e restituisce il valore numerico del minimo valore dell'<b>EMP</b> ottenuto nel caso di
    400 &le; <b>&lambda;</b> &le; 10<sup>6</sup> da:
    \f[EMP=min \left(EMP_{singolo}, EMP_{medio}, EMP_{treno} \right)\f]
    negli altri casi da:
    \f[EMP=min \left(EMP_{singolo}, EMP_{medio}\right)\f]

*/
double getPowerErgForEMP();/*!< La funzione individua la potenza/energia dell'impulso del laser corrispondente all'<b>EMP</b> minimo.
    Se l'<b>EMP</b> corrisponde al valore di potenza media si considera questo valore come quello
    significativo altrimenti si considera l'<b>EMP</b> che corrisponde al prodotto <b>EPM</b> &sdot; <b>C<sub>P</sub></b>
    con <b>C<sub>P</sub></b>=1 nel caso di impulso singolo. In ogni caso se l'<b>EMP</b> del singolo impulso è
    espresso in esposizione radiante il parametro del laser corrispondente sarà espresso in energia dell'impulso o energia
    nell'intervallo <b>T<sub>e</sub></b> mentre se l'<b>EMP</b> è espresso in irradianza il parametro del laser corrispondente
    sarà espresso in potenza.

    In sintesi:

      EMP minimo            |  Valore fornito
    :----------------------:|:---------------------------------:
      E<sub>singolo</sub>   | Potenza di picco dell'impulso
      H<sub>singolo</sub>   | Energia dell'impulso
      E<sub>medio</sub>     | Potenza ottica media
      H<sub>medio</sub>     | Energia ottica emessa in T<sub>e</sub>
      EMP<sub>Treno</sub>   | Potenza di picco dell'impulso
      EMP<sub>Treno</sub>   | Energia dell'impulso

    Tale valore potrà risultare utile allorquando sarà necessario fornire l'uscita del laser espressa in modo omogeneo al valore di
    <b>EMP</b> calcolato come nel caso in cui si vuole calcolare la distanza di sicurezza da assumere per una riflessione diffondente puntiforme.
    */
string getMeanPowerFormulaSort();/*!<Restituisce il tipo di grandezza, <b>H</b> o <b>E</b>, impiegata per il calcolo dell'EMP del
dispositivo per l'applicazione del criterio della potenza. */
string getMeanPowerFormulaEMP();/*!< Restituisce la formula dell'<b>EMP</b> impiegata per il calcolo dell'EMP del
dispositivo per l'applicazione del criterio della potenza. */
double getCA_MP() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>A</sub></b> riguardante il criterio della potenza.*/
double getCB_MP() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>B</sub></b> riguardante il criterio della potenza.*/
double getCC_MP() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>C</sub></b> riguardante il criterio della potenza.*/
double getCE_MP() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>E</sub></b> riguardante il criterio della potenza.*/
double getT1_MP() const;/*!< Restituisce il valore numerico del parametro <b>T<sub>1</sub></b> riguardante il criterio della potenza.*/
double getT2_MP() const;/*!< Restituisce il valore numerico del parametro <b>T<sub>2</sub></b> riguardante il criterio della potenza.*/
double getGamma_MP() const;/*!< Restituisce il valore dell'angolo <b>&gamma;</b> in mrad relativo allapplicazione del criterio della potenza. */
string getRadiation_MP() const;/*!< Restituisce il tipo di radiazione emessa dal dispositivo relativo al criterio della potenza.*/
string getSkinDamage_MP() const;/*!< Restituisce il tipo di danno che il dispositivo può produrre alla cute riferito al criterio della potenza.*/
string getEyeDamage_MP() const;/*!< Restituisce il tipo di danno che il dispositivo può produrre all'occhio riferito al criterio della potenza.*/
string getPhotochemicalNote_MP() const;/*!< Fornisce commenti nel caso di valutazione sia fotochimica che termica ed è riferito al criterio della potenza.*/
double getEMP_MP();/*!< Restituisce il valore numerico dell'<b>EMP</b> del dispositivo utile per l'applicazione del criterio della potenza. */
void setPRF(const double &);/*!< Restituisce il valore della Pulse Repetition Frquency <b>PRF</b> impostata misurata in Hz.*/
double getPRF() const;/*!< Imposta il valore della Pulse Repetition Frquency <b>PRF</b> in Hz.*/
double getMeanPower() const;/*!< Restituisce il valore della Potenza media <b>P</b> impostata misurata in Watt.*/
double getMeanIrradiance() const;/*!< Restituisce il valore dell'irradianza della potenza ottica media del dispositivo impostata
espressa in W/m<sup>2</sup> */
double getThermalEMP()const;
double getCPCoefficient()const;
double getPulseNumber()const;
double getEMP_ForOD()const;/*!< Restituisce il valore numerico minimo dell'<b>EMP</b> tra quello relativo all'impulso singolo e
quello relativo agli effetti termici.
 Tale valore risulta utile allorquando si voglia determinare il massimo valore della trasmittanza <b>&tau;</b> del filtro necessario ad abbattre
 la radiazioni a livelli inferiori al massimo permesso.
 */
void computeMeanPower_NOHD();/*!< Calcola la <b>DNRO</b> in metri per l'<b>EMP</b> ottenuto con il criterio della potenza applicando la nota formula:
\f[DNRO=\frac{\sqrt{\frac{4 \cdot P_{media}}{\pi \cdot E_{Te}}}-a}{\varphi}\f]
dove <b>E<sub>EMP</sub></b> è l'<b>EMP<sub>Te</sub></b> ricavato della tabella dell'Allegato XXXVII espresso in irradianza misurata in W/m<sup>2</sup>,
relativo alla durata <b>T<sub>e</sub></b>, <b>P<sub>media</sub></b> è la potenza ottica media in W, <b>a</b> il diametro del fascio in metri e
<b>&phi;</b> la divergenza in mrad.*/
void computeSinglePulse_NOHD();/*!< Calcola la <b>DNRO</b> in metri per l'<b>EMP</b> ottenuto con il criterio dell'impulso applicando la nota formula:
\f[DNRO=\frac{\sqrt{\frac{4 \cdot Q}{\pi \cdot H_{impulso}}}-a}{\varphi}\f]
dove <b>H<sub>t</sub></b> è l'<b>EMP</b> ricavato della tabella dell'Allegato XXXVII espresso in esposizione radiante misurata in J/m<sup>2</sup>
relativo alla durata dell'impulso <b>t</b>, <b>Q</b> è l'energia dell'impulso in J, <b>a</b> il diametro del fascio in metri e
<b>&phi;</b> la divergenza in mrad.*/
void computePulseTrain_NOHD();/*!< Calcola la <b>DNRO</b> in metri per l'<b>EMP</b> ottenuto con il criterio dell'impulso con correzione termica,
valida nel caso di 400 &le; <b>&lambda;</b> &le; 10<sup>6</sup>, applicando la nota formula:
\f[DNRO=\frac{\sqrt{\frac{4 \cdot Q}{\pi \cdot H_{impulso}}}-a}{\varphi}\f]
dove <b>H<sub>T</sub></b> è l'<b>EMP</b> ricavato della tabella dell'Allegato XXXVII espresso in esposizione radiante misurata in J/m<sup>2</sup>
relativo alla durata dell'impulso <b>Teff</b>, <b>Q</b> è l'energia dell'impulso in J, <b>a</b> il diametro del fascio in metri, <b>C<sub>B</sub></b>
il fattore di correzione termica, <b>&phi;</b> la divergenza in mrad.*/
void computeEMP_ForOD();/*!< Calcola il valore numerico minimo dell'<b>EMP</b> tra quello relativo all'impulso singolo e
quello relativo agli effetti termici.
In sintesi:
 \f[EMP=min \left(EMP_{singolo},EMP_{treno} \right)\f]
 Tale valore risulta utile allorquando si voglia determinare il massimo valore della trasmittanza <b>&tau;</b> del filtro necessario ad abbattre
 la radiazioni a livelli inferiori al massimo permesso.*/

void computeMeanPowerLambertianMax();/*!< Calcola il valore massimo della distanza di sicurezza espressa in metri di una eventuale riflessione puntiforme
* su di una superficie diffondente con coefficiente di riflessione <b>&rho;</b> unitario nel caso in cui l'<b>EMP</b> sia quello ottenuto con il criterio
  della potenza:
\f[DRO=\sqrt{\frac{P_{media}}{\pi \cdot E_{media}}}\f]
*/
void computeSinglePulseLambertianMax();/*!< Calcola il valore massimo della distanza di sicurezza espressa in metri di una eventuale riflessione puntiforme
* su di una superficie diffondente con coefficiente di riflessione <b>&rho;</b> unitario nel caso in cui l'<b>EMP</b> sia quello ottenuto con il criterio
  dell'impulso con correzione termica:
\f[DRO=\sqrt{\frac{Q}{\pi \cdot H_{impulso}}}\f]*/
void computePulseTrainLambertianMax();/*!< Calcola il valore massimo della distanza di sicurezza espressa in metri di una eventuale riflessione puntiforme
* su di una superficie diffondente con coefficiente di riflessione <b>&rho;</b> unitario nel caso in cui l'<b>EMP</b> sia quello ottenuto con il criterio
  dell'impulso con correzione termica nel caso in cui 400 &le; <b>&lambda;</b> &le; 10<sup>6</sup>:
\f[DRO=\sqrt{\frac{Q}{\pi \cdot E_{treno}}}\f]*/

double getEMP_1stCondition()const;/*!< Restituisce il valore dell'<b>EMP</b> del dispositivo ricavato per l'applicazione del criterio dell'impulso
espresso nella grandezza indicata dalla Tabella dell'Allegato XXXVII del D.Lgs 81/2008 a cui corrisponde. */
double getEMP_2ndCondition()const;/*!< Restituisce il valore dell'<b>EMP</b> del dispositivo ricavato per l'applicazione del criterio della potenza
nella grandezza indicata dalla Tabella dell'Allegato XXXVII del D.Lgs 81/2008 a cui corrisponde. */
double getEMP_3rdCondition()const;/*!< Restituisce il valore dell'<b>EMP</b> del dispositivo ricavato per l'applicazione del criterio dell'impulso
 con correzione termica nel caso di 400 &le; <b>&lambda;</b> &le; 10<sup>6</sup> nella grandezza indicata dalla Tabella dell'Allegato XXXVII del D.Lgs 81/2008 a cui corrisponde. */

string getFormula_3rdCondition();/*!< Restituisce la formula dell'<b>EMP</b> impiegata per il calcolo dell'<b>EMP</b> del
dispositivo in applicazione del criterio dell'impulso con correzione termica. Si ricorda che la formula dell'<b>EMP</b> per la terza condizione può essere diversa da quella
dell'impulso singolo nel caso di funzionamento in alta frquenza.*/
string getFormulaSort_3rdCondition();/*!< Restituisce la formula dell'<b>EMP</b> impiegata per il calcolo dell'EMP del
dispositivo in applicazione del criterio dell'impulso con correzione termica. Si ricorda che la formula dell'<b>EMP</b> per la terza condizione può essere diversa da quella
dell'impulso singolo nel caso di funzionamento in alta frquenza.*/

void equateMeanPowerEMP(); /*!< Rende omogeneo all'<b>EMP</b> del singolo impulso l'<b>EMP</b> relativo al criterio della potenza ciò al fine di consentire
il confronto di tale valore con gli <b>EMP</b> riguardante il criterio dell'impulso e determinare il valore minimo. */
void computeTmin(); /*!< *Si imposta il valore di <b>T<sub>min</sub></b> in funzione della lunghezza d'onda &lambda; del dispositivo
* secondo quanto riportato in tabella:
    Lunghezza d'onda &lambda; [nm]	         | Tmin [s]
   :----------------------------------------:|:---------------------------:
    315 < &lambda; &le; 400) 		         | 1,0 &sdot; 10<sup>-09</sup>
    400 < &lambda; &le; 1050) 		         | 18 &sdot; 10<sup>-06</sup>
    1050 < &lambda; &le; 1400) 		         | 0.00005
    1400 < &lambda; &le; 1500) 		         | 0,001
    1500 < &lambda; &le; 1800) 		         | 10
    1800 < &lambda; &le; 2600) 		         | 0,001
    2600 < &lambda; &le; 10<sup>-7</sup>     | 0,001
*/
double getTmin() const;/*!< Restituisce il valore del parametro <b>T<sub>min</sub></b> in secondi*/
string valutateCounting(); /*!< Se 400 &le; <b>&lambda;</b> &le; 10<sup>6</sup> restituisce la modalità di conteggio in particolare:
<ul>
<li>se <b>PRF</b> < 1/<b>T<sub>min</sub></b> restituisce <b>"regolare"</b>;</li>
<li>se <b>PRF</b> > 1/<b>T<sub>min</sub></b> restituisce <b>"non regolare"</b>.</li>
</ul>

  */
double getMeanPower_EMP_Equate() const;/*!< Restituisce il valore medio dell'<b>EMP</b> del dispositivo ricavato per l'applicazione del
criterio della potenza. */
void setTe();/*!< Imposta il valore del tempo effettivo di esposizione in secondi come il minimo tra il valore del tempo <b>T<sub>2</sub></b>
 * ed il tempo di eposizione <b>T<sub>e</sub></b>. Il valore del tempo <b>T<sub>2</sub></b> può cambiare nel caso di funzionamento
 * ad alta frequenza. */
double getTe()const;/*!< Restituisce il valore del parametro <b>T<sub>eff</sub></b> in secondi*/


private:
double PRF;
double Te;
double timeBase;
double meanPower;
double meanPower_NOHD;
double singlePulse_NOHD;
double pulseTrain_NOHD;

double meanPowerLambertianMax;
double singlePulseLambertianMax;
double pulseTrainLambertianMax;

double meanPowerIrradiance;
int pulseNumber;
double CP;
ComputeEMP myMeanPower_Laser;
ComputeEMP myTmin_Laser;
//oggetto relativo all'esposizione al treno di impulsi
double meanPow_EMP_Result;
double powerEMP;
double CP_EMP_Result;
double SP_EMP_Result;
double EMP_ForOD;
string powerFormulaSort;
string powerFormulaEMP;
double meanPow_EMP_Equate;
double Tmin;
string formula_Tmin;
string formulaSort_Tmin;
};

#endif // LASERSAFETYMP_H
