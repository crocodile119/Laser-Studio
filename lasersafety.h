#ifndef LASERSAFETY_H
#define LASERSAFETY_H
#include "computeemp.h"

using namespace std;

/*! \brief 	Classe per il calcolo dell'Eposizione Massima Permessa, <b>EMP</b> e della Distanza Nominale di Sicurezza Ottica, <b>DNRO</b>, di un laser
 * ad impulso singolo con il metodo previsto della parte 2 dell'Allegato XXXVII del D.Lgs 81/2008.
 *
 * La valutazione si svolge instanziando la Classe LaserSafetySP.  Impostati i valori della lunghezza d'onda <b>&lambda;</b>,
 * della potenza di uscita <b>P</b>, della divergenza <b>&phi;</b>, del diametro del fascio <b>a</b>, dell'angolo sotteso dalla sorgente apparente
 * <b>&alpha;</b> consente di ricavare l'<b>EMP</b>, la <b>DNRO</b> e altre grandezze utili per valutazioni successive.
 * La classe prevede l'impiego della classe ComputeEMP per il calcolo dell'<b>EMP</b> del dispositivo. Istanziata la Classe e impostati i parametri:
 <ul>
<li>divergenza <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
<li>durata dell'impulso <b>t</b> in secondi;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.</li>
</ul>
Per procedere al calcolo dell'<b>EMP</b> sarà necessario invocare la funzione membro LaserUpdate().*/

class LaserSafety
{
public:
LaserSafety(double=BEAM_DIAMETER, double=PULSE_ENERGY, double=DIVERGENCE, double=EmpLeaTables::HE_NE_WAVELENGTH, double=PULSE_WIDTH,
double=EmpLeaTables::ALPHA_MIN);/*!< L'istanza della classe prevede l'inserimento dei parametri:
<ul>
<li>diametro del fascio <b>a</b> in mm;</li>
<li>potenza <b>P</b> in Watt;</li>
<li>divergenza <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
<li>durata dell'impulso <b>t</b> in secondi;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.</li>
</ul>

La classe prevede inoltre un costruttore di default con i seguenti parametri:
<ul>
<li>diametro del fascio <b>a</b>= 7 mm;</li>
<li>energia dell'impuls <b>Q</b>= 10<sup>-3</sup> J;</li>
<li>divergenza <b>&phi;</b> = 1 mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> = 633 nm;</li>
<li>durata dell'impulso <b>t</b>= 0,25 s;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b>=1,5 mrad.</li>
</ul>
 */


const static double PI;/*!< Valore della costante <b>&pi;</b>. */

const static double PULSE_ENERGY;/*!< Parametro di default dell'energia dell'impulso in J. */
const static double PULSE_WIDTH;/*!< Parametro di default della durata dell'impulso in secondi. */
const static double BEAM_DIAMETER;/*!< Parametro di default del diametro del fascio in mrad. */
const static double DIVERGENCE;/*!< Parametro di default della divergenza del fascio in mrad. */

double getWavelength()const;/*!< Restituisce il valore della lunghezza d'onda <b>&lambda;</b> in nm.*/
void setWavelength(const double &);/*!< Imposta il valore della lunghezza d'onda <b>&lambda;</b> in nm. */
double getAlpha() const;/*!< Restituisce il valore dell'angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.*/
void setAlpha(const double &);/*!< Imposta il valore dell'angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad. */
double getDivergence() const;/*!< Restituisce il valore della divergenza <b>&phi;</b> in mrad.*/
void setDivergence(const double &);/*!< Imposta il valore della divergenza <b>&varphi;</b> in mrad. */
double getBeamDiameter() const;/*!< Restituisce il valore del diametro del fascio <b>a</b> in mm.*/
void setBeamDiameter(const double &);/*!< Imposta il valore del diametro del fascio <b>a</b> in mm. */
void computeNOHD();/*!< Calcola la <b>DNRO</b> in metri applicando la nota formula:
\f[DNRO=\frac{\sqrt{\frac{4 \cdot Q}{\pi \cdot H_{EMP}}}-a}{\varphi}\f]
dove <b>H<sub>EMP</sub></b> è l'<b>EMP</b> ricavato della tabella dell'Allegato XXXVII espresso in esposizione energetica misurata in J/m<sup>2</sup>,
<b>Q</b> è l'energia dell'impuslo in J, <b>a</b> il diametro del fascio e <b>&phi;</b> la divergenza.*/
double getPowerErgForEMP();
void computeLambertianMax();/*!< Calcola il valore massimo della distanza di sicurezza espressa in metri di una eventuale riflessione puntiforme
* su di una superficie diffondente con coefficiente di riflessione <b>&rho;</b> unitario:
\f[DRO=\sqrt{\frac{Q}{\pi \cdot H_{EMP}}}\f]
Il valore risulta utile qualora si voglia successivamente calcolare la distanza di sicurezza per una riflessione puntiforme su superficie diffondente
definita dalla nota formula:
\f[DRO=\sqrt{\frac{\rho \cdot Q \cdot \theta}{\pi \cdot H_{EMP}}}\f]

*/
void computeBeamArea();/*!< Calcola l'area della sezione trasversale del fascio in prossimità dell'apertura <b>A<sub>b</sub></b> in m<sup>2</sup> con la nota formula:
\f[A_b=\frac{\pi a^2}{4}\f]
*/
double getNOHD() const;/*!< Restituisce il valore della <b>DNRO</b> calcolata in metri. */
double getLambertianMax() const;/*!< Restituisce il valore calcolato del valore massimo della distanza di sicurezza espressa in metri di una eventuale riflessione puntiforme
* su di una superficie diffondente con coefficiente di riflessione <b>&rho;</b> unitario:
\f[DRO=\sqrt{\frac{Q}{\pi \cdot H_{EMP}}}\f]*/
double getBeamArea() const;/*!< Restituisce il valore dell'area della sezione trasversale del fascio <b>A<sub>b</sub></b> misurata in m<sup>2</sup>.*/
double getCA() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>A</sub></b>.*/
double getCB() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>B</sub></b>.*/
double getCC() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>C</sub></b>.*/
double getCE() const;/*!< Restituisce il valore numerico del parametro <b>C<sub>E</sub></b>.*/
double getT1() const;/*!< Restituisce il valore numerico del parametro <b>T<sub>1</sub></b> misurato in secondi.*/
double getT2() const;/*!< Restituisce il valore numerico del parametro <b>T<sub>2</sub></b> misurato in secondi.*/
void EMP();/*!< Calcola l'EMP del dispositivo dall'oggetto istanza della Classe ComputeEMP. */
string getRadiation() const;/*!< Restituisce il tipo di radiazione emessa dal dispositivo.*/
string getSkinDamage() const;/*!< Restituisce il tipo di danno che il dispositivo può produrre alla cute.*/
string getEyeDamage() const;/*!< Restituisce il tipo di danno che il dispositivo può produrre all'occhio.*/
string getPhotochemicalNote() const;/*!< Fornisce commenti nel caso di valutazione sia fotochimica che termica. */
double getGamma() const;/*!< Restituisce il valore dell'angolo <b>&gamma;</b> in mrad. */
double getEMP();/*!< Restituisce il valore numerico dell'<b>EMP</b> del dispositivo. */
string getFormulaEMP() const;/*!< Restituisce la formula impiegata per il calcolo dell'EMP del dispositivo. */
string getFormulaSort() const;/*!< Restituisce il tipo di grandezza usata per epreimere l'EMP del dispositivo:
<ul>
<li>Esposizione radiante <b>H</b> in J/m<sup>2</sup>;</li>
<li>Irradianza <b>E</b> in W/m<sup>2</sup>.</li>
</ul>
 */
void laserUpdate();/*!< Impostati i parametri necessari effettua il calcolo dell'<b>EMP</b> e della <b>DNRO</> del dispositivo */
void setPulseWidth(const double &);/*!< Imposta il valore della durata dell'impulso <b>t</b> in secondi. */
double getPulseWidth() const;/*!< Restituisce il valore della durata dell'impulso <b>t</b> in secondi.*/
void setPowerErg(const double &);/*!< Imposta il valore dell'energia dell'impulso <b>Q</b> in Joule. */
double getPowerErg() const;/*!< Restituisce il valore dell'energia dell'impulso impostata <b>Q</b> in Joule.*/
void computePowerForExended();
double getPowerForExended() const;

protected:
double wavelength;
double EMP_Result;
ComputeEMP myLaser;
double alpha;
double pulseWidth;
double powerErg;
double NOHD;
double lambertianMax;
double divergence;
double beamDiameter;
double laserEMP;
double beamArea;
string formula;
double CA;
double CB;
double CC;
double CE;
double T1;
double T2;
double t_exp;
double gamma;
string radiation;
string skinDamage;
string eyeDamage;
string notes;
string formulaSort;
double powerForExended;
};

#endif // LASERSAFETY_H
