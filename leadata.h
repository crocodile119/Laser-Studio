#ifndef LEADATA_H_
#define LEADATA_H_

/*! \brief 	Struttura dati per la scrittura di una singola riga della una delle tabella dei valori dei livelli di Emissione Accessibile per l'occhio.
 *
 * La struttura dati rappresenta in modo simbolico un valore dell'Livello di Emissione Accessibile.
 */

struct leadata {
    int formulaNumber;/*!< Numero progressivo del LEA letto dal file binario EMPdataBinary.dat */
    double wavelenght1;/*!< Primo estremo del range di lunghezze d'onda espresso in nm corrispondente al valore letto. */
    double wavelenght2;/*!< Secondo estremo del range di lunghezze d'onda espresso in nm corrispondente al valore letto. */
    double time1;/*!< Primo istante dell'impulso espresso in secondi corrispondente al valore letto. */
    double time2;/*!< Secondo istante dell'impulso espresso in secondi corrispondente al valore letto. */
    int C1;/*!< Coefficiente C<sub>1</sub> pari a 1 se presente nella formula del LEA 0 altrimenti. */
    int C2;/*!< Coefficiente C<sub>2</sub> pari a 1 se presente nella formula del LEA 0 altrimenti. */
    int C3;/*!< Coefficiente C<sub>3</sub> pari a 1 se presente nella formula del LEA 0 altrimenti. */
    int C4;/*!< Coefficiente C<sub>4</sub> pari a 1 se presente nella formula del LEA 0 altrimenti. */
    int C5;/*!< Coefficiente C<sub>5</sub> pari a 1 se presente nella formula del LEA 0 altrimenti. */
    int C6;/*!< Coefficiente C<sub>6</sub> pari a 1 se presente nella formula del LEA 0 altrimenti. */
    int C7;/*!< Coefficiente C<sub>7</sub> pari a 1 se presente nella formula del LEA 0 altrimenti. */
    int t;/*!< Durata dell'impulso t pari a 1 se presente nella formula del LEA come t<sup>0,75</sup>, pari a 2 se presente come
    * t<sup>0,25</sup>,  pari a 3 se presente come t, 0 altrimenti. */
    int effects;/*!< Gli effetti della radiazione considerata che sono riferiti alla banda di emissione secondo quanto riportato di seguito.
    *
    *  Valore  | Banda [nm]
    * :-------:|:---------------:
    *     1    | 180 - 400
    *     2    | 400 - 700
    *     3    | 700 - 1050
    *     4    | 1400 - 2600
    *     5    | 400 - 600
    *     6    | 600 - 700
    *     7    | 2600 10<sup>6</sup>
    */
    double formula;/*!< Il coefficiente numerico della formula del LEA. */
    int sort;/*!< Fornisce il tipo di formula e vale 1 se il LEA è espresso in irradianza, 2 se è espresso in esposizione energetica,
    3 se è espresso in potenza e 4 se è espresso in joule. */
    };
#endif /* LEADATA_H_ */
