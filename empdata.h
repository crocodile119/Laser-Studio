#ifndef EMPPDATA_H
#define EMPPDATA_H

/*! \brief 	Struttura dati per la scrittura di una singola riga della tabella dei valori di Esposizione Massima Permessa per l'occhio.
 *
 * La struttura dati rappresenta in modo simbolico un valore dell'Esposzione Massima Permessa.
 */

struct empdata {
    int formulaNumber;/*!< Numero progressivo dell'EMP letto dal file binario EMPdataBinary.dat */
    double wavelenght1;/*!< Primo estremo del range di lunghezze d'onda espresso in nm corrispondente al valore letto. */
    double wavelenght2;/*!< Secondo estremo del range di lunghezze d'onda espresso in nm corrispondente al valore letto. */
    double time1;/*!< Primo istante dell'impulso espresso in secondi corrispondente al valore letto. */
    double time2;/*!< Secondo istante dell'impulso espresso in secondi corrispondente al valore letto. */
    int CA;/*!< Coefficiente C<sub>A</sub> pari a 1 se presente nella formula dell'EMP 0 altrimenti. */
    int CB;/*!< Coefficiente C<sub>A</sub> pari a 1 se presente nella formula dell'EMP 0 altrimenti. */
    int CC;/*!< Coefficiente C<sub>A</sub> pari a 1 se presente nella formula dell'EMP 0 altrimenti. */
    int CE;/*!< Coefficiente C<sub>A</sub> pari a 1 se presente nella formula dell'EMP 0 altrimenti. */
    int t;/*!< Durata dell'impulso t pari a 1 se presente nella formula dell'EMP come t<sup>0,75<sup>, pari a 2 se presente come
    * t<sup>0,25<sup>,  0 altrimenti. */
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
    double formula;/*!< Il coefficiente numerico della formula dell'EMP. */
    int sort;/*!< Fornisce il tipo di formula e vale 0 se l'EMP è espresso in irradianza, 1 se è espresso in esposizione energetica. */
    };
#endif /* EMPPDATA_H */
