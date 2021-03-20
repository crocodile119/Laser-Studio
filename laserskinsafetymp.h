#ifndef LASERSKINSAFETYMP_H
#define LASERSKINSAFETYMP_H
#include "laserskinsafety.h"

using namespace std;
/*! \brief 	Classe per il calcolo dell'Eposizione Massima Permessa, <b>EMP</b> per la cute e della Distanza Nominale di Sicurezza per la cute,
 * <b>DNRC</b>, di un laser ad impulsi multipli con il metodo previsto della parte 2 dell'Allegato XXXVII del D.Lgs 81/2008.
 *
 * La Classe è derivata dalla classe LaserSkinSafety ed aggiunge un consistente numero di funzioni membro che consentono di:
 <ul>
 <li>Applicare la valutazione del criterio della potenza media;</li>
  <li>Valutare il più piccolo valore di <b>EMP</b> applicabile(quello più gravoso) e il valore massimo della DNRO.</li>
 </ul>
 La Classe prevede l'impiego di tre istanze della classe ComputeEMP():
  <ul>
 <li>una dedicata all'impulso singolo per l'applicazione del criterio dell'impulso e alla valutazione degli effetti termici ;</li>
 <li>una dedicata agli effetti medi per l'applicazione degli effetti medi;</li>
 <li>una dedicata all'impulso singolo per l'applicazione del criterio dell'impulso e alla valutazione degli effetti termici
 nel caso di funzionamento ad alta frequenza.</li>
 </ul>
 Per procedere al calcolo dell'<b>EMP</b> sarà necessario invocare la funzione membro LaserUpdate(). */
class LaserSkinSafetyMP : public LaserSkinSafety
{
public:
LaserSkinSafetyMP(double=PULSE_REPETITION_FREQEUNCY, double=EXPOSURE_TIME, double=BEAM_DIAMETER, double=POWER_ENERGY, double=DIVERGENCE, double=EmpLeaTables::HE_NE_WAVELENGTH,
                  double=PULSE_WIDTH, double=EmpLeaTables::ALPHA_MIN);/*!< L'istanza della classe prevede l'inserimento dei parametri:
<ul>
<li>frequenza di ripetizione degli impulsi <b>PRF</b> in Hz;</li>
<li>tempo di esposizione <b>T<sub>e</sub></b> in secondi;</li>
<li>diametro del fascio <b>a</b> in mm;</li>
<li>potenza <b>P</b> in Watt;</li>
<li>divergenza del fascio <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
<li>durata dell'impulso <b>t</b> in secondi;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.</li>
</ul>

La classe prevede inoltre un costruttore di default con i seguenti parametri:
<ul>
<li>Frequenza di ripetizione dell'impulso <b>PRF</b>=10 Hz;</li>
<li>tempo di esposizione <b>T<sub>e</sub></b>=5 s;</li>
<li>diametro del fascio <b>a</b>= 7 mm;</li>
<li>energia dell'impulso <b>Q</b>= 10<sup>-3</sup> J;</li>
<li>divergenza del fascio <b>&phi;</b> = 1.5 mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> = 633 nm;</li>
<li>durata dell'impulso <b>t</b>= 10<sup>-6</sup> s;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b>=1,5 mrad.</li>
</ul>
 */

const static double PULSE_REPETITION_FREQEUNCY; /*!< Parametro di default della frequenza di ripetizione degli impulsi i Hz. */
const static double PULSE_WIDTH; /*!< Parametro di default della durata dell'impulso in secondi. */

void setPulseWidth(const double &);/*!< &Egrave; un overload di funzione. Imposta il valore del tempo di esposizione <b>T<sub>e</sub></b> in secondi per
* l'applicazione del criterio della potenza media, imposta il valore della durata minima dell'impulso <b>T<sub>min</sub></b> nel caso
* di impulsi ad alta frequenza, aggiorna se necessario il dato membro riguardante la durata del singolo impulso <b>t</b> per la parte
* relativa alla valutazione del criterio dell'impulso. */
void setWavelength(const double &);/*!< &Egrave; un overload di funzione. Imposta il valore della lunghezza d'onda <b>&lambda;</b> in nm per l'applicazione
* del criterio della potenza media e degli impulsi ad alta frequenza, aggiorna se necessario il dato membro e la lunghezza d'onda per
* la parte relativa alla valutazione del criterio dell'impulso.*/
void setAlpha(const double &);/*!< &Egrave; un overload di funzione. Imposta il valore dell'angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad per l'applicazione
* del criterio della potenza media e degli impulsi ad alta frequenza, aggiorna se necessario il dato membro e la lunghezza d'onda per
* la parte relativa alla valutazione del criterio dell'impulso. */
void computeNSHD();
void laserSkinUpdate();/*!< &Egrave; un overload di funzione. Impostati i parametri necessari effettua il calcolo dell'<b>EMP</b> del dispositivo, della <b>DNRC</b>. */
void computeMeanPower();/*!< Calcola la potenza ottica media espressa in Watt del dispositivo secondo la nota relazione:
    \f[P_{media}=PRF \cdot Q\f] */
void computeMeanPowerIrradiance();/*!< Calcola l'irradianza della potenza ottica media del dispositivo espressa in W/m<sup>2</sup>
    secondo la nota relazione:
    \f[E_{media}=\frac{4 \ \ P_{media}}{\pi \ \ a^2}\f] */
void computeMeanPowerEMP();/*!< Calcola il valore dell'<b>EMP</b> della pelle con il criterio della potenza riferito all'impulso medio. La modalità con
    * cui cui valore dell'<b>EMP</b> della pelle è ricavato dipende dall'unità di misura del valore dell'<b>EMP</b> relativo al tempo di esposizione
    * <b>T<sub>e</sub></b>.
     In particolare:
                  EMP in T<sub>e</sub> | Formula impiegata
                :---------------------:|:-------------------------------------------------------------------------:
                   H<sub>Tmedio</sub>  | H<sub>Impulso medio</sub> =H<sub>Tmedio</sub> / (PRF &sdot; T<sub>e</sub>)
                   E<sub>Tmedio</sub>  | H<sub>Impulso medio</sub> E<sub>Tmedio</sub> / (PRF &sdot; t)
     */
void computeSP_EMP();
double returnMultiPulse_EMP();
string getMeanPowerFormulaSort();
string getMeanPowerFormulaEMP();
void computePulseNumber();
double getEMP_MP();
void setPRF(const int);
double getPRF() const;
double getMeanPower() const;
double getMeanIrradiance() const;
double getPulseNumber() const;
void computeMeanPower_NSHD();/*!< Calcola la <b>DNRC</b> in metri per l'<b>EMP</b> ottenuto con il criterio della potenza applicando la nota formula:
\f[DNRC=\frac{\sqrt{\frac{4 \cdot P_{media}}{\pi \cdot E_{Te}}}-a}{\varphi}\f]
dove <b>E<sub>EMP</sub></b> è l'<b>EMP<sub>Te</sub></b> della cute ricavato della tabella dell'Allegato XXXVII espresso in irradianza misurata in W/m<sup>2</sup>,
relativo alla durata <b>T<sub>e</sub></b>, <b>P<sub>media</sub></b> è la potenza ottica media in W, <b>a</b> il diametro del fascio in metri e
<b>&phi;</b> la divergenza in mrad.*/
void computeSinglePulse_NSHD();/*!< Calcola la <b>DNRO</b> in metri per l'<b>EMP</b> della cute ottenuto con il criterio dell'impulso applicando la nota formula:
\f[DNRC=\frac{\sqrt{\frac{4 \cdot Q}{\pi \cdot H_{impulso}}}-a}{\varphi}\f]
dove <b>H<sub>t</sub></b> è l'<b>EMP</b> della cute ricavato della tabella dell'Allegato XXXVII espresso in esposizione radiante misurata in J/m<sup>2</sup>
relativo alla durata dell'impulso <b>t</b>, <b>Q</b> è l'energia dell'impulso in J, <b>a</b> il diametro del fascio in metri e
<b>&phi;</b> la divergenza in mrad.*/
void equateMeanPowerEMP();/*!< Rende omogeneo all'<b>EMP</b> della cute del singolo impulso l'<b>EMP</b> relativo al criterio della potenza ciò al fine di consentire
il confronto di tale valore con l'<b>EMP</b> riguardante il criterio dell'impulso e determinare il valore minimo. */
double getMeanPower_EMP_Equate() const;/*!< Restituisce il valore medio dell'<b>EMP</b> del dispositivo ricavato per l'applicazione del
criterio della potenza. */
double getExposureTime() const;/*!< Restituisce il valore del parametro <b>T<sub>e</sub></b> in secondi*/
void setExposureTime(const double &);
double getNSHD() const;
double getEMP_1stCondition()const;
double getEMP_2ndCondition()const;

private:
double PRF;
double meanPower;
double meanPower_NSHD;
double singlePulse_NSHD;
double meanPowerIrradiance;
int pulseNumber;
ComputeEMP myMeanPower_SkinLaser; //oggetto relativo all'esposizione al treno di impulsi
double meanPow_EMP_Result;
double powerSkinEMP;
double SP_EMP_Result;
string powerFormulaSort;
string powerFormulaEMP;
double meanPow_EMP_Equate;
double NSHD;

};

#endif // LASERSKINSAFETYMP_H
