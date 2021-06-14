#ifndef TABLESCONTROLLER_H
#define TABLESCONTROLLER_H
#include "empleatables.h"

/*! \brief 	Implementa un singleton per la classe EmpLeaTables e consente una gestione più efficiente della scrittura della tabella EMP e delle tabelle LEA.
 *
 * Utilizza la una variabile statica puntatore privata *singleton dello stesso tipo della classe. Con la funzione statica pubblica *getInstance()
 * restituisce la varibile statica puntatore che punta o alla classe già istanziata o ad una nuova istanza, risulta pertanto impossibile che
 * vi siano più di una istanza delle classe e che la scrittura avvenga su un unico oggetto del tipo EmpLeaTables.
 *
 */

class TablesController
{
public:
    TablesController();
    // Funzione membro del singleton
    static TablesController *getInstance();/*!< Funzione statica he restituisce un puntatore all'istanza della classe.*/

    static void destroy();/*!< Distrugge l'istanza del singleton generata in modo dinamico.*/
    std::array<leadata, EmpLeaTables::TABLEROW_1_1M>  writeLeaInStructValues_1_1M();/*!< Scrive la tabella dei valori di dei Livelli
    * di Esposizione Accessibili di Classe 1 e 1M. La funzione membro verifica la presenza di un'istanza della classe, se già presente
    * passa alla scrittura della tabella altrimenti instanzia la classe con un nuovo oggetto e e solo successivamente procede alla scrittura
    * della tabella. */
    std::array<leadata, EmpLeaTables::TABLEROW_3R> writeLeaInStructValues_3R();/*!< Scrive la tabella dei valori di dei Livelli
    * di Esposizione Accessibili di Classe 3R. La funzione membro verifica la presenza di un'istanza della classe, se già presente passa alla
    * scrittura della tabella altrimenti instanzia la classe con un nuovo oggetto e e solo successivamente procede alla scrittura della tabella.*/
    std::array<leadata, EmpLeaTables::TABLEROW_3B> writeLeaInStructValues_3B();/*!< Scrive la tabella dei valori di dei Livelli
    * di Esposizione Accessibili di Classe 3B. La funzione membro verifica la presenza di un'istanza della classe, se già presente passa alla
    * scrittura della tabella altrimenti instanzia la classe con un nuovo oggetto e e solo successivamente procede alla scrittura della tabella.*/
    std::array<empdata, EmpLeaTables::TABLEROW_EMP> writeEmpInStructValues();/*!< Scrive la tabella dei valori di Esposizione Massima Permessa. La funzione membro verifica
    * la presenza di un'istanza della classe, se già presente passa alla scrittura della tabella altrimenti instanzia la
    * classe con un nuovo oggetto e e solo successivamente procede alla scrittura della tabella. */

private:

  static TablesController *singleton; /*!< variabile statica puntatore di tipo TablesController che punta all'unica istanza. */

   EmpLeaTables *myEmpLeaTables; /*!< Puntatore di tipo TablesController che punta all'unica istanza della classe EmpLeaTables instaziata in modo
   * dinamico nel costruttore della classe. */

};

#endif // TABLESCONTROLLER_H
