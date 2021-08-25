#ifndef COMPUTELEA_H
#define COMPUTELEA_H
#include "leadata.h"
#include "empleatables.h"
#include <string>
#include <cstddef>
#include <array>

using namespace std;

/*! \brief 	Calcola il valore dei Livelli di Esposizioni Accessibili delle Classi 1 e 1M, delle Classi 2 e 2M, della Classe 3R e della Classe 3B,
 *			assegnate la lunghezza d'onda &lambda;, la durata dell'impulso t e l'angolo sotteso dalla sorgente apparente &alpha;.
 * 			Il calcolo viene eseguito leggendo i valori di LEA da una struttura dati.
 *
 * 	Quando instanziata invoca la funzione membro calculate() per la scrittura delle tabelle dei Livelli di Esposizioni Accessibili in array di
 * 	strutture dati del tipo leapdata.
 *  L'impiego della classe dovrà nelle direttive di preprocessore l'inclusione della struttura dati leadata.h, per la scrittura di una riga delle
 *  tabelle LEA  e gli array di strutture leadata empleatables.h, per la scrittura della tabella EMP. La norma di riferimento
 *  è la CEI EN 60825-1 del Giugno 2017.
 *
 */

class ComputeLEA
{
public:
    ComputeLEA(double=EmpLeaTables::HE_NE_WAVELENGTH, double=EmpLeaTables::NATURAL_AVERSION_TIME, double=EmpLeaTables::ALPHA_MIN);

    const static double ALPHA_MIN;
    const static size_t N_LEA{4};

    enum class ClassData {CLASSE_1_1M,
    /**< Dati corrispondenti al LEA di Classe 1 e 1M. */
    CLASSE_2_2M,
    /**< Dati corrispondenti al LEA di Classe 2 e 2M. */
    CLASSE_3R,
    /**< Dati corrispondenti al LEA di Classe 3R. */
    CLASSE_3B,
    /**< Dati corrispondenti al LEA di Classe 3B. */
    SENZA_CLASSIFICA
    /**< Dati non corrispondenti ad alcuna classifica. */
    };

    void calculate();/*!<Segue l'istanza della classe e calcola i parametri del dispositivo invocando la funzione membro protetta
    * computeParameters(), individua gli effetti biologici invocando la funzione membro protetta bioEffects(), calcola i LEA per
    * i quattro casi previsti invocando la funzione membro LEA().
    * Infine ricava le distanze dai diaframmi e i diametri invocando le funzioni membro valuateApertureCondition_3(), valuateCondition_3(),
    * valuateCondition_1(). E' la funzione membro da impiegare ogni volta che si desidera calcolare i LEA di un dispositivo laser. */
    void LEA();/*!< Calcola i valori di LEA per ciascuna classe (Classe 1 e 1M, Classe 2 e 2M, Classe 3R e Classe 3B), oltre che
    * che costruire le formule e le unità di misura corrispondenti (irraadianza, esposizione energetica, potenza ed energia).
    * Tali valori vengono memorizzati in array di 4 elementi. Dove necessario effettua correzioni (ad esempio la scelta di due valori
    * possibili di LEA da valutare in funzione del valore di un parametro impostato). Calcola infine il LEA di Classe due che si ricava
    * in funzione della base dei tempi.*/

    void applyLEA_CorrectionFactor(const double&);/*!< Moltiplica l'array dei valori del LEA per il fattore di correzione. */

    void setWavelength(const double&);/*!< Imposta il valore della lunghezza d'onda &lambda; in nm. */
    double getWavelength() const;/*!< Restituisce il valore della lunghezza d'onda &lambda; impostato in nm. */
    void setPulseWidth(const double&);/*!< Imposta il valore della durata dell'impulso t in secondi. */
    double getPulseWidth() const;/*!< Restituisce il valore della durata dell'impulso t impostato in secondi. */
    void setAlpha(const double&);/*!< Imposta il valore dell'angolo sotteso dalla sorgente apparente &alpha; in mrad. */
    double getAlpha() const;/*!< Restituisce il valore dell'angolo sotteso dalla sorgente apparente &alpha; impostato in mrad. */

    double getC1() const;/*!< Restituisce il valore del parametro C<sub>1</sub>. */
    double getC2() const;/*!< Restituisce il valore del parametro C<sub>2</sub>. */
    double getC3() const;/*!< Restituisce il valore del parametro C<sub>3</sub>. */
    double getC4() const;/*!< Restituisce il valore del parametro C<sub>4</sub>. */
    double getC5() const;/*!< Restituisce il valore del parametro C<sub>5</sub>. */
    double getC6() const;/*!< Restituisce il valore del parametro C<sub>6</sub>. */
    double getC7() const;/*!< Restituisce il valore del parametro C<sub>7</sub>. */
    double getT1() const;/*!< Restituisce il valore del parametro T<sub>1</sub>. */
    double getT2() const;/*!< Restituisce il valore del parametro T<sub>2</sub>. */

    array <int, N_LEA> getLEA_FormulaSort()const;/*!< Restituisce un array di tipo int con di valori da 1 a 4. L'intero è associato al tipo di unità di misura
    * secondo lo schema riportato di seguito:

     Valore  | Unità di misura
    :-------:|:---------------:
        1    | W/m<sup>2</sup>
        2    | J/m<sup>2</sup>
        3    | W
    */

    string getRadiation() const;/*!< Restituisce il tipo di radiazione emessa dal dispositivo laser. */
    string getSkinDamage() const;/*!< Restituisce il tipo dei possibili danni alla pelle. */
    string getEyeDamage() const;/*!< Restituisce il tipo dei possibili danni all'apparato visivo. */
    double getGamma() const;/*!< Restituisce il valore dell'angolo &gamma; in mrad. L'angolo &gamma; ha significato solo per esposizioni
    * con effetti fotochimici. */

    array <double, N_LEA> getLEA()const;/*!< Restituisce un array di tipo double con i valori dei lea calcolati */
    array <string, N_LEA> getLEA_Formula()const;/*!< Restituisce un array di tipo string con le formule dei lea calcolati */
    array <string, N_LEA> getLEA_FormulaTipo()const;/*!< Restituisce un array di tipo string con il tipo formule dei lea calcolati */
    array <string, N_LEA> getLEA_FormulaUnit()const;/*!< Restituisce un array di tipo string con l'unità di misura dei lea calcolati */

    double getApertureThirdCondition()const;/*!< Restituisce il diametro del diaframma calcolato da valuateApertureCondition_3().*/

    double getApertureStopCond_1()const;/*!< Restituisce il valore del diametro del diaframma in mm riguardante la 1<sup>a</sup> condizione corrispondente ai parametri impostati */
    double getApertureStopCond_3()const;/*!< Restituisce il valore del diametro del diaframma in mm riguardante la 3<sup>a</sup> condizione corrispondente ai parametri impostati */
    double getDistanceCond_1()const;/*!< Restituisce la distanza dal diaframma in mm riguardante la 1<sup>a</sup> condizione corrispondente ai parametri impostati */
    double getDistanceCond_3()const;/*!< Restituisce la distanza dal diaframma in mm riguardante la 1<sup>3</sup> condizione corrispondente ai parametri impostati */

    string typeOfEyeDamage(const int&);/*!< Restituisce il possibile danno al sistema oculare. */
    string typeOfRadiation(const int&);/*!< Restituisce il tipo di radiazione emessa dal dispositivo. */
    string typeOfSkinDamage(const int&);/*!< Restituisce il possibile danno alla cute. */

    std::array<leadata, EmpLeaTables::TABLEROW_1_1M> getLEA_1_1M_Table()const;
    std::array<leadata, EmpLeaTables::TABLEROW_3R> getLEA_3R_Table()const;
    std::array<leadata, EmpLeaTables::TABLEROW_3B> getLEA_3B_Table()const;

protected:
    void selectLea_1M_Row(const std::array<leadata, EmpLeaTables::TABLEROW_1_1M> &);/*!< Seleziona la riga del LEA di Classe 1 e 1M
    * corrispondente ai parametri di ingresso impostati.*/
    void selectLea_3R_Row(const std::array<leadata, EmpLeaTables::TABLEROW_3R> &);/*!< Seleziona la riga del LEA di Classe 3R
    * corrispondente ai parametri di ingresso impostati.*/
    void selectLea_3B_Row(const std::array<leadata, EmpLeaTables::TABLEROW_3B> &);/*!< Seleziona la riga del LEA di Classe 3B
    * corrispondente ai parametri di ingresso impostati.*/

    void writeLeaInStructValues(ClassData);/*!< Scrive le tabelle LEA in strutture dati del tipo empleatables.h. La funzione invoca un metodo statico
    * della classe TablesController perchè le tabelle siano scritte in un'unica struttura dati (singleton).*/


    void computePhotoGamma();/*!< Calcola il valore dell'angolo &gamma; espresso in mrad. L'angolo &gamma; ha significato solo per esposizioni
    * con effetti fotochimici. */
    string valuateFormulaTipo(leadata &);/*!< Ricava dalla riga di LEA di Classe 1 e 1M, Classe 3R e Classe 3B, corrispondente ai parametri impostati,
    * il tipo di grandezza con la quale si esprime il LEA.*/
    string valuateFormulaUnit(leadata &);/*!< Ricava dalla riga di LEA di Classe 1 e 1M, Classe 3R e Classe 3B, corrispondente ai parametri impostati,
    * ill'unità di misura con la quale si esprime il LEA.*/


    void computeAlpha_max();/*!< Calcola il valore di &alpha;<sub>max</sub> secondo quanto previsto dalla CEI EN 60825-1 del Giugno 2017.*/
    string valuateFormula(leadata &);/*!< Costruisce dalla riga di LEA di Classe 1 e 1M, Classe 3R e Classe 3B, corrispondente ai parametri impostati,
    * la formula del LEA.*/

    double LEA_Value(leadata &);/*!< Calcola il valore del LEA .*/
    void computeParameters();/*!< Calcola i valori dei parametri.*/
    double compute_t(leadata &);/*!< Restituisce un array di tipo int con di valori da 1 a 3. L'intero è associato al tipo di unità di fattore
    * da considerare nel calcolo del valore del LEA, nella costruzione della formula e del tipo di grandezza

     Valore  | Valore del coefficiente
    :-------:|:---------------:
        0    | 1
        1    | t<sup>0,75</sup>
        2    | t<sup>0,25</sup>
        3    | t
    */

    double valuate_LEA_2M_Value();/*!< Calcola il valore del LEA di Classe 2 e 2M assegnato il LEA di Classe 1 e 1M.*/
    string valuate_LEA_2M_Formula();/*!< Calcola la formula del LEA di Classe 2 e 2M assegnata la formula del LEA di Classe 1 e 1M.*/
    string valuate_LEA_2M_FormulaUnit();/*!< Ricava l'unità di misura del LEA di Classe 2 e 2M assegnato il LEA di Classe 1 e 1M.*/
    string valuate_LEA_2M_FormulaTipo();/*!< Ricava il tipo di LEA di Classe 2 e 2M  (irradianza o esposizione energetica) assegnato il LEA di Classe 1 e 1M.*/

    void bioEffects();/*!< Ricava gli effetti biologici.*/

    void valuateApertureCondition_3();/*!< Calcola il valore del diametro del diaframma della 3<sup>a</sup> condizione da impiegare nella valutazione della
    * prima condizione	e della terza.
    condizione        | diametro diaframma 3<sup>a</sup> condizione
    :----------------:|:-----------:
           t ≤ 0,35 s | 1
    0,35 s < t < 10 s | 1,5 t3/8
             t ≥ 10 s | 3,5


    */
    void valuateCondition_1();/*!< Calcola il valore del diametro del diaframma e della distanza dalla sorgente dal diaframma per la 1<sup>a</sup> condizione.*/
    void valuateCondition_3();/*!< Calcola il valore del diametro del diaframma e della distanza dalla sorgente dal diaframma per la 3<sup>a</sup> condizione.
    * Per la distanza della distanza dal diaframma si suppone nota la posizione del waist*/

private:
    double wavelength;
    double pulseWidth;
    double alpha;
    double alpha_max;

    std::array<leadata, EmpLeaTables::TABLEROW_1_1M> leaStructValues_1M;
    std::array<leadata, EmpLeaTables::TABLEROW_3R>leaStructValues_3R;
    std::array<leadata, EmpLeaTables::TABLEROW_3B>leaStructValues_3B;

    leadata myLeaData;

    array <double, N_LEA> LEA_Result;

    array <string, N_LEA> LEA_Formula;
    array <int, N_LEA> LEA_FormulaSort;
    array <string, N_LEA> LEA_FormulaTipo;
    array <string, N_LEA> LEA_FormulaUnit;

    leadata myLeaData_1M;
    leadata myLeaData_3R;
    leadata myLeaData_3B;

    double C1;
    double C2;
    double C3;
    double C4;
    double C5;
    double C6;
    double C7;
    double T1;
    double T2;
    double photoGamma;
    string radiation;
    string skinDamage;
    string eyeDamage;
    string note;

    double apertureStopCond_1;
    double distanceCond_1;
    double apertureStopCond_3;
    double distanceCond_3;
    double apertureThirdCondition;
    double diameterAtStopCond_1;
    double diameterAtStopCond_3;
    double couplingFactor_1;
    double couplingFactor_2;
};
#endif // COMPUTELEA_H
