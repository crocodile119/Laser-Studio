#ifndef EMPLEATABLES_H
#define EMPLEATABLES_H

#include "leadata.h"
#include "empdata.h"
#include <QtGui>
#include <array>

/*! \brief 	Legge i file binari contenenti i dati delle tabelle dell'Esposizione Massima Permessa e dei LEA e ne scrive i valori in array della STD.
 *
 * La funzione legge i file binari generati dallo streaming su file delle librerie QTextStream e QDataStream e li memorizza in array della STD
 * del tipo empdata o leadata a seconda del caso. Una volta scritti gli array la lettura dei valori delle tabelle avviene impiegando le funzioni di
 * get presente per ciascuna tipologia.
 *
 */

class EmpLeaTables
{
public:
    enum tableRows{TABLEROW_1_1M=41,
    /**< Numero di righe della tabella del LEA di Classe 1 e 1M */
    TABLEROW_3R=38,
    /**< Numero di righe della tabella del LEA di Classe 3R*/
    TABLEROW_3B=21,
    /**< Numero di righe della tabella del LEA di Classe 3B*/
    TABLEROW_EMP=70
    /**< Numero di righe della tabella del EMP*/
    };

    enum typeOfOutput{UV=1,
    /**< Banda di lunghezza d'onda tra 180 e 400 nm */
    VIS=2,
    /**< Banda di lunghezza d'onda tra 400 e 700 nm */
    IRA=3,
    /**< Banda di lunghezza d'onda tra 700 e 1050 nm */
    IRA_2=4,
    /**< Banda di lunghezza d'onda tra 1400 e 2600 nm */
    VIS_2=5,
    /**< Banda di lunghezza d'onda tra 400 e 600 nm */
    VIS_3=6,
    /**< Banda di lunghezza d'onda tra 600 e 700 nm */
    IRA_IRB=7
    /**< Banda di lunghezza d'onda tra 2600 e 10<sup>6</sup> nm */
    };

    const static double HE_NE_WAVELENGTH;/*!< Lunghezza d'onda dell'emissione del laser Elio Neon pari a 633 nm.*/
    const static double NATURAL_AVERSION_TIME;/*!< Tempo di riflesso palpebrale pari a 0,25 secondi.*/
    const static double ALPHA_MIN;/*!< Il valore minimo dell'angolo &alpha; paria a 1,5 mrad. */
    const static double ALPHA_MAX;/*!< Il valore massimo dell'angolo &alpha; paria a 100 mrad. */

    EmpLeaTables();
    void writeEmpValues();/*!< Scrive la tabella dell'EMP in un array della libreria std del tipo empdata.*/
    void writeLeaValues_1_1M();/*!< Scrive la tabella del LEA di Classe 1 e 1M in un array della libreria std del tipo leadata.*/
    void writeLeaValues_3R();/*!< Scrive la tabella del LEA di Classe 3R in un array della libreria std del tipo leadata.*/;
    void writeLeaValues_3B();/*!< Scrive la tabella del LEA di Classe 3B in un array della libreria std del tipo leadata.*/;
    std::array<empdata, TABLEROW_EMP> getEmpData();/*!< Restituisce la tabella dell'EMP in un array della libreria std del tipo empdata.*/
    std::array<leadata, TABLEROW_1_1M> getLeaData_1_1M();/*!< Restituisce la tabella del LEA di Classe 1 e 1M in un array della libreria std
    * del tipo leadata.*/
    std::array<leadata, TABLEROW_3R> getLeaData_3R();/*!< Restituisce la tabella del LEA di Classe 3R in un array della libreria std
    * del tipo leadata.*/
    std::array<leadata, TABLEROW_3B> getLeaData_3B();/*!< Restituisce la tabella del LEA di Classe 3B in un array della libreria std
    * del tipo leadata.*/

private:
    std::array<empdata, TABLEROW_EMP> empValues;
    std::array<leadata, TABLEROW_1_1M> leaValues_1_1M;
    std::array<leadata, TABLEROW_3R> leaValues_3R;
    std::array<leadata, TABLEROW_3B> leaValues_3B;
};

#endif // EMPLEATABLES_H
