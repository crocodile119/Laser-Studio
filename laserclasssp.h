#ifndef LASERCLASSSP_H
#define LASERCLASSSP_H
#include "laserclasscw.h"

using namespace std;

/*! \brief 	Classe per il calcolo della Classe di un laser impulsato con il metodo semplificato (CEI EN 60825-1 2017)
 *
 * La Classe è derivata dalla classe LaserClassCW e si differenzia per laa presenza di getter e di un setter per la durata dell'impulso
 * dell'impulso durata. Sono presenti inoltre gli overload delle funzioni membro leaPowerErgUnit(int*, const double &, const double &)
 * ed updateAll(). */

class LaserClassSP : public LaserClassCW
{
public:
    LaserClassSP(double=PUPIL_DIAMETER, double=PULSE_ENERGY, double=DIVERGENCE, double=EmpLeaTables::HE_NE_WAVELENGTH,
                 double=PULSE_WIDTH, double=ALPHA); /*!< L'istanza della classe prevede l'inserimento dei parametri:
<ul>
<li>diametro del fascio <b>a</b> in mm;</li>
<li>energia dell'impulso <b>Q</b> in Joule;</li>
<li>divergenza <b>&phi;</b> in mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> in nm.</li>
<li>la durata dell'impulso <b>t</b> in secondi<li>
</ul>

La classe prevede inoltre un costruttore di default con i seguenti parametri:
<ul>
<li>diametro del fascio <b>a</b>= 7 mm;</li>
<li>energia dell'impulso <b>Q</b>= 10<sup>-3</sup> J;</li>
<li>divergenza <b>&phi;</b> = 1.5 mrad;</li>
<li>lunghezza d'onda <b>&lambda;</b> = 633 nm;</li>
<li>durata dell'impulso <b>t</b>= 1.0e-06 s;</li>
<li>angolo sotteso dalla sorgente apparente <b>&alpha;</b>=1,5 mrad.</li>
</ul>
 */

const static double PULSE_ENERGY; /*!< Parametro di default della base dei tempi in secondi. */
const static double PULSE_WIDTH; /*!< Parametro di default della dura dell'impulso in secondi. */

void setPulseWidth(const double);/*!< Imposta il valore della durata dell'impulso <b>t</b> in secondi. */
double getPulseWidth() const;/*!< Restituisce il valore della durata dell'impulso <b>t</b> in secondi. */

void updateAll();  /*!< Versione overloaded della funzione membro LaserClassCW::updateAll() per il calcolo e l'aggiornamento della Classe. L'overload in questo caso
genererebbe confuzione si è pertanto deciso di considerare un'altra funzione membro (nel funzionamento impulsato è necessario calcolare la Classe del dispositivo
con classUpdate(laserData, double&, double&) della classe genitore impostando l'enumerale al valore LaserClassCW::laserOperation::PULSED) . */

protected:
double pulseWidth;
};
#endif // LASERCLASSSP_H
