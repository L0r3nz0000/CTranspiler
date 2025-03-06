# Sintassi di base
## Funzioni

Ogni parte del codice deve essere inclusa all'interno di una funzione. La sintassi per definire una funzione è la seguente:
```
fun nome(arg1, arg2...) {
    # corpo della funzione
}
```
nome: è il nome della funzione.
arg1, arg2...: sono gli argomenti della funzione, separati da virgola.

Esempio:
```
fun saluta(nome) {
    # Stampa un saluto
    print("Ciao, " + nome);
}
```
## Commenti

I commenti sono preceduti dal simbolo # e possono essere su una sola riga. Non sono supportati commenti multilinea al momento.

Esempio:

`# Questo è un commento`
```
fun somma(a, b) {
    # Somma due numeri
    return a + b;
}
```

## Dichiarazione di Variabili

Le variabili in CTranspiler vengono dichiarate utilizzando il simbolo @. CTranspiler rileverà automaticamente il tipo della variabile in base al valore assegnato.

Sintassi:

`@ variabile = valore;`

Esempio:

```
@ numero = 5;
@ pi = 3.14;
@ nome = "Mario";
```

CTranspiler supporta i tipi primitivi come int, float, char, e string. Non è necessario specificare il tipo, verrà dedotto automaticamente dal valore.
## Valorizzazione delle Variabili

Per assegnare un nuovo valore a una variabile esistente, utilizziamo la sintassi:

`variabile = nuovo_valore;`

Esempio:
```
@ x = 10;
x = x + 5;  # x ora è 15
```
## Chiamata di Funzioni

Le funzioni vengono chiamate come in C, passando i parametri tra parentesi.

Esempio:

```
fun saluta(nome) {
    print("Ciao, " + nome);
}

fun main() {
    saluta("Mario");
}
```

## Operazioni Aritmetiche

Le operazioni aritmetiche sono simili al C:

* Somma: +
* Sottrazione: -
* Moltiplicazione: *
* Divisione: /
* Modulo: %

Esempio:
```
@ a = 10;
@ b = 20;
@ risultato = a * b;
```

## Cast di Tipo

CTranspiler supporta il cast di tipo tramite funzioni. Per esempio:

```
toInt(valore) converte un valore in int.
toChar(valore) converte un valore in char.
toString(valore) converte un valore in string.
toFloat(valore) converte un valore in float.
```
Esempio:
```
@ valore = 5.75;
@ intero = toInt(valore);  # intero è ora 5
@ testo = toString(intero);  # testo è ora "5"
```

## Condizioni (if-else)

La sintassi delle condizioni è la stessa del C:
```
if (condizione) {
    # Blocco
} else {
    # Blocco2
}
```

Esempio:
```
fun main() {
    @ numero = 10;
    if (numero > 5) {
        print("Maggiore di 5");
    } else {
        print("Minore o uguale a 5");
    }
}
```

## Cicli

### Ciclo for:

Il ciclo for ha la seguente sintassi:
```
for (variabile = inizio to fine step incremento) {
    # Blocco di codice
}
```

Esempio:
```
fun main() {
    for (i = 0 to 5 step 1) {
        print(i);
    }
}
```
### Ciclo while:

La sintassi per un ciclo while è la seguente:
```
while (condizione) {
    # Blocco di codice
}
```
Esempio:
```
fun main() {
    @ i = 0;
    while (i < 5) {
        print(i);
        i = i + 1;
    }
}
```
