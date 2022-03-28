#ifndef COMPUTEEMP_H
#define COMPUTEEMP_H
#include "empdata.h"
#include "empleatables.h"
#include <string>
#include <array>

using namespace std;


/*! \brief 	Calcola il valore dell'Esposizione Massima Permessa assegnate la lunghezza d'onda &lambda;, la durata dell'impulso t e l'angolo
 *			sotteso dalla sorgente apparente &alpha;. Il calcolo viene eseguito leggendo i valori di EMP da una struttura dati.
 *
 * 	Quando instanziata invoca la funzione membro EMP() per la la scrittura della tabella delle Esposizioni Massime Permesse nell'array di struttura
 *  dati del tipo empdata,
 *  L'impiego della classe dovrà includere nelle direttive di preprocessore la struttura dati empdata.h, per la scrittura di una riga della tabella EMP  e l'array
 *  di strutture empdata empleatables.h, per la scrittura della tabella EMP.  Vengono inizialmente valutati i parametri di emissione e gli effetti
 *  biologici successivamente, nell'array con i valori di EMP, viene selezionata la riga corrispondente ai valori &lambda; e t impostati.
 *  Nel caso di valutazione semplice (valore singolo dell'EMP), a partire dalla riga si calcola e si memorizza il valore dell'EMP, la formula
 *  ed il tipo di esposizione.
 *  Nel caso di valutazione complessa (valutazione su più di un valore) il calcolo vieve svolto in modo dedicato in funzione della lunghezza
 *  d'onda e della durata dell'impulso. A esempio nella banda 400-600 nm per esposizioni maggiori o uguali di 10 secondi, si calcolano il limite
 *  per effetto termico ed il limite per fotochimico, considerando infine il valore più basso.
 * 	Qualora fosse necessario ottenere il valore di EMP per la pelle sarà possibile, una volta instanzaiata la classe, impostati i parametri
 *  di ingresso del laser ed invocato il metodo EMP(), invocare il metodo pubblico adaptForSkinEMP(). Il riferimento normativo è fornito
 *  dall'Allegato XXXVII del D.Lgs. 81/2008.
 *
 */

class ComputeEMP
{
public:
    ComputeEMP(double=EmpLeaTables::HE_NE_WAVELENGTH, double=EmpLeaTables::NATURAL_AVERSION_TIME, double=EmpLeaTables::ALPHA_MIN);
    const static double ALPHA_MIN;
    const static double ALPHA_MAX;
    void EMP();/*!< Segue l'istanza della classe e calcola sia il valore dell'EMP, che la formula per il calcolo e l'unità di misura e
    * quindi il tipo di grandezza (radianza o esposizione energetica). Calcola inoltre i parametri ed effettua le correzioni nel caso di
    * valutazioni multiple. E' la funzione membro da impiegare ogni volta che si desidera calcolare l'EMP di un dispositivo laser. */
    void setWavelength(const double&);/*!< Imposta il valore della lunghezza d'onda &lambda; in nm. */
    double getWavelength() const;/*!< Restituisce il valore della lunghezza d'onda &lambda; impostato in nm. */
    void setPulseWidth(const double&);/*!< Imposta il valore della durata dell'impulso t in secondi. */
    double getPulseWidth() const;/*!< Restituisce il valore della durata dell'impulso t impostato in secondi. */
    void setAlpha(const double&);/*!< Imposta il valore dell'angolo sotteso dalla sorgente apparente &alpha; in mrad. */
    double getAlpha() const;/*!< Restituisce il valore dell'angolo sotteso dalla sorgente apparente &alpha; impostato in mrad. */
    double getCA() const;/*!< Restituisce il valore del parametro C<sub>A</sub>. */
    double getCB() const;/*!< Restituisce il valore del parametro C<sub>B</sub>. */
    double getCC() const;/*!< Restituisce il valore del parametro C<sub>C</sub>. */
    double getCE() const;/*!< Restituisce il valore del parametro C<sub>E</sub>. */
    double getT1() const;/*!< Restituisce il valore del parametro T<sub>1</sub> in secondi. */
    double getT2() const;/*!< Restituisce il valore del parametro T<sub>1</sub> in secondi. */
    double getLimitingAperture() const;/*!< Restituisce il valore del diametro dell'apertura in mm. */
    double getEMP() const;/*!< Restituisce il valore numerico dell'EMP. */
    empdata getEMP_Data()const;
    string getFormulaEMP() const;/*!< Restituisce la formula usata per il calcolo dell'EMP. */
    string getFormulaSort() const;/*!< Restituisce 'E' se il valore dell'EMP è in irradianza 'H' se il valore è in esposizione energetica. */
    string getRadiation() const;/*!< Restituisce il tipo di radiazione emessa dal dispositivo laser. */
    string getSkinDamage() const;/*!< Restituisce il tipo dei possibili danni alla pelle. */
    string getEyeDamage() const;/*!< Restituisce il tipo dei possibili danni all'apparato visivo. */
    string getPhotochemicalNote() const;/*!< Fornisce commenti nel caso in cui la lunghezza d'onda del dispositivo sia compresa tra i 400
    * e i 600 nm in merito agli effetti termici e fotochimici in questo caso entrambi presenti. */
    double getGamma() const;/*!< Restituisce il valore dell'angolo &gamma; in mrad. L'angolo &gamma; ha significato solo per esposizioni
    * con effetti fotochimici. */
    void adaptForSkinEMP();/*!< Potrà essere invocato qualora fosse necessario ottenere il valore di EMP per la pelle. */
    std::array<empdata, EmpLeaTables::TABLEROW_EMP> getEMP_Table()const;
    bool isPhotochemical();


protected:
    void writeEmpInStructValues();/*!< Scrive la tabella EMP nella struttura dati del tipo empleatables.h. La funzione invoca un metodo statico
    * della classe TablesController perchè le tabelle siano scritte in un'unica struttura dati (singleton).*/
    void selectLeaRow();/*!< Nel caso di valutazione semplice, seleziona la riga dell'EMP corrispondente ai parametri di ingresso impostati.*/

    void valutateFormula();/*!< Costruisce la formula dell'EMP semplice.*/
    void EMP_Value();/*!< Calcola il valore dell'EMP semplice.*/
    void ComputeParameters();/*!< Calcola i valori dei parametri.*/
    void computeLimitingAperture();

    void PhotoEffects();/*!< Calcola l'EMP, costruisce la formula ed il tipo di esposizione, per effetti fotochimici nel caso di valutazione complessa.*/
    void ThermoEffects();/*!< Calcola l'EMP, costruisce la formula ed il tipo di esposizione, per effetti termici nel caso di valutazione complessa.*/
    void EMPPhotoThermo();/*!< Confronta l'EMP per effetti termici con l'EMP per effetti fotochimici e restituisce i risultati corrispondenti al minimo.*/
    void BioEffects();/*!< Ricava gli effetti biologici.*/

private:
    double wavelength;
    double pulseWidth;
    double alpha;
    string formula;
    string formulaSort;
    double EMP_Result;
    double CA;
    double CB;
    double CC;
    double CE;
    double T1;
    double T2;
    double t_exp;
    double limitingAperture;
    string radiation;
    string skinDamage;
    string eyeDamage;
    string notes;

    std::array<empdata, EmpLeaTables::TABLEROW_EMP> empStructValues;
    empdata myEmpData;
    empdata photoEmpData;
    empdata thermoEmpData;

    string EMP_Formula;
    int EMP_FormulaSort;
    string EMP_FormulaTipo;
    string EMP_FormulaUnit;

    //Photochemical effects vs thermal effects assasment variables
    //Photochemical effects
    double _PhotoEMP;
    string _PhotoFormulaSort;
    string _PhotoFormula;
    double _PhotoEMPH;
    double _PhotoGamma;
    //Thermal effects
    double _ThermoEMP;
    string _ThermoFormulaSort;
    string _ThermoFormula;
    double _ThermoEMPH;

};
#endif // COMPUTEEMP_H
