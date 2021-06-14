#ifndef LASERSAFETYCW_H
#define LASERSAFETYCW_H
#include "lasersafety.h"

using namespace std;
/*! \brief 	Classe per il calcolo per il calcolo dell'Eposizione Massima Permessa, <b>EMP</b> e della Distanza Nominale di
 * Sicurezza Ottica, <b>DNRO</b>, di un laser ad Onda Continua con il metodo previsto della parte 2 dell'Allegato XXXVII del D.Lgs 81/2008.
 *
 * La Classe è derivata dalla classe LaserSafety per adattarne il funzionamento nel caso di laser ad Onda Continua con le seguenti modifiche:
 <ul>
 <li>Overload delle funzioni che calcolano le disanze di sicurezza;</li>
  <li>Valutazione programmatica del tempo di esposizione;</li>
 <li>Possibilità di impostare manualmente il tempo di esposizione.</li>
 </ul>
Per procedere al calcolo dell'<b>EMP</b> sarà necessario invocare la funzione membro LaserUpdate().
 */

class LaserSafetyCW : public LaserSafety
{
public:
LaserSafetyCW(double=PUPIL_DIAMETER, double=POWER, double=DIVERGENCE, double=EmpLeaTables::HE_NE_WAVELENGTH,
              double=EmpLeaTables::NATURAL_AVERSION_TIME, double=EmpLeaTables::ALPHA_MIN);/*!< L'istanza della classe prevede l'inserimento dei parametri:
<ul>
<li>diametro del fascio <b>a</b> in mm;</li>
<li>potenza <b>P</b> in Watt;</li>
<li>divergenza <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm;</li>
<li>durata dell'impulso <b>t</b> in secondi;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b> in mrad.</li>
</ul>
La durata dell'impulso dovrà essere fisicamente plausibile per un'esposizione da Onda Continua.

In alternativa la durata di esposizione è impostata in
modo programmatico secondo il criterio mostrato in seguito:
 Lunghezza d'onda [nm]           | t [s]
:-------------------------------:|:----------:
 400 &le; &lambda; &le; 700 nm   | 0,25
 &lambda; > 700 nm               | 10
 180 < &lambda; < 700 nm         | 30000
con la funzione membro setExposureTime(). Perchè l'impulso sia impostata in modo efficace è necessario
impostare la durata dell'impulso con con la durata getExposureTime().

La classe prevede inoltre un costruttore di default con i seguenti parametri:
<ul>
<li>diametro del fascio <b>a</b>= 7 mm;</li>
<li>potenza <b>P</b>= 1 W;</li>
<li>divergenza <b>&phi;</b> = 1 mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> = 633 nm;</li>
<li>durata dell'impulso <b>t</b>= 0,25 s;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b>=1,5 mrad.</li>
</ul>
 */

const static double POWER;/*!< Parametro di default della potenza ottica del dispositivo in W. */

void computeNOHD();/*!< &Egrave; un overload di funzione. Calcola la <b>DNRO</b> in metri applicando la nota formula:
\f[DNRO=\frac{\sqrt{\frac{4 \cdot P}{\pi \cdot E_{EMP}}}-a}{\varphi}\f]
dove <b>E<sub>EMP</sub></b> è l'<b>EMP</b> ricavato della tabella dell'Allegato XXXVII espresso in irradianza misurata in W/m<sup>2</sup>,
<b>P</b> è la potenza ottica del dispositivo espressa in W, <b>a</b> il diametro del fascio e <b>&phi;</b> la divergenza.*/
double getPowerErgForEMP();
void computeLambertianMax();/*!< &Egrave; un overload di funzione. Calcola il valore massimo della distanza di sicurezza espressa in metri di una eventuale riflessione puntiforme
* su di una superficie diffondente con coefficiente di riflessione <b>&rho;</b> unitario:
\f[DRO=\sqrt{\frac{P}{\pi \cdot E_{EMP}}}\f]
Il valore risulta utile qualora si voglia successivamente calcolare la distanza di sicurezza per una riflessione puntiforme su superficie diffondente
definita dalla nota formula:
\f[DRO=\sqrt{\frac{\rho \cdot P \cdot \theta}{\pi \cdot E_{EMP}}}\f]*/
void setExposureTime();/*!< Imposta il valore del tempo di esposizione in secondi secondo la logica seguente:
 Lunghezza d'onda [nm]           | t [s]
:-------------------------------:|:----------:
 400 &le; &lambda; &le; 700 nm   | 0,25
 &lambda; > 700 nm               | 10
 180 < &lambda; < 700 nm         | 30000
Funziona se isTimeEditable() restitusce il valore falso booleano.
 */
double getExposureTime() const;/*!< Restituisce il valore del tempo di esposizione <b>T<sub>e</sub></b> impostato in secondi.*/
void laserUpdate();/*!< &Egrave; un overload di funzione. < Impostati i parametri necessari consente il calcolo dell'<b>EMP</b> del dispositivo */
void setExposureTimeEditable(bool);/*!< Consente di impostare il tempo di esposizione <b>T<sub>e</sub></b> se con il valore vero booleano.*/
bool isTimeEditable();/*!< Restituisce il valore vero booleano se il tempo di esposizione <b>T<sub>e</sub></b> è editabile. */
void setEditedExposureTime(const double &);/*!< Imposta il tempo di esposizione <b>T<sub>e</sub></b> se isTimeEditable() restitusce
il valore vero booleano (da verificare).*/

protected:
double exposureTime;
bool timeEditable;


private:

};

#endif // LASERSAFETYCW_H
