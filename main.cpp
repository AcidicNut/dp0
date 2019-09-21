#include "cekla.h"

int main() {
    writeln("Hello, World!");
    return 0;
}

list convertBase(const int number, const int base, list Acc) {
    if ((number / base) == 0)
        return cons(number % base, Acc);
    else
        return convertBase(number / base, base, cons(number % base, Acc));
}

// number base alapu szamrendszerben
list convertBase(const int number, const int base) {
    return convertBase(number, base, nil);
}

list filterEven(list L, int i, list Acc) {
    if (tl(L) == nil) {
        if (i == 1)
            return cons(hd(L), Acc);
        else
            return Acc;
    }

    if (i == 0) {
        return filterEven(tl(L), 1, Acc);
    } else {
        return filterEven(tl(L), 0, cons(hd(L), Acc));
    }
}

// filter every second number from list
list filterEven(list L) {
    return filterEven(L, 0, nil);
}

/* osszekevert(S, A) == SK, ha SK az S szám A alapú összekevert változata
   (S>0, A>1 egész számok).
   Egy S szám A alapú összekevertjét úgy kapjuk meg, hogy az S számot
   felírjuk az A alapú számrendszerben, a számjegyeit egy listával
   ábrázoljuk, a listát a fentiek szerint átrendezzük, majd a kapott
   lista elemeit egy A alapú szám jegyeinek tekintve előállítjuk a
   keresett értéket.
*/
int osszekevert(const int S, const int A) {
    writeln(convertBase(13, 2));
}