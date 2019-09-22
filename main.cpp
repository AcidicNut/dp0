#include "cekla.h"

int main() {
    writeln("Hello, World!");
    return 0;
}

// seged fuggveny, number szamot base lapura valtja
list convertBase(const int number, const int base, list Acc) {
    if ((number / base) == 0) {
        return cons(number % base, Acc);
    } else {
        return convertBase(number / base, base, cons(number % base, Acc));
    }
}

// seged fuggveny, kivalogatja az L listabol a paros indexueket,
// i jeloli azt hogy paros (1) vagy paratlan (0)
list filterEven(list L, int i, list Acc) {
    if (tl(L) == nil) {
        if (i == 1) {
            return cons(hd(L), Acc);
        } else {
            return Acc;
        }
    }

    if (i == 0) {
        return filterEven(tl(L), 1, Acc);
    } else {
        return filterEven(tl(L), 0, cons(hd(L), Acc));
    }
}

// revapp(L, L0) = az L lista megfordítása L0 elé fűzve
list revapp(const list L, const list L0) {
    if (L == nil) {
        return L0;
    }
    return revapp(tl(L), cons(hd(L), L0));
}

// seged fuggveny, az eredeti es egz szurt listabol osszeallit egz kevert listat,
// i jeloli azt hogy paros (1) vagy paratlan (0)
list kevertList(list original, list even, int i, list Acc) {
    if (original == nil) {
        return Acc;
    }
    if (i == 0) {
        return kevertList(tl(original), even, 1, cons(hd(original), Acc));
    } else {
        return kevertList(tl(original), tl(even), 0, cons(hd(even), Acc));
    }
}

// seged fuggveny, L listaban tarolt base alapu szam 10-es szamrendszerbeli alakjat adja vissza
int convertListToNum(list L, int base, int Acc) {
    if (L == nil) {
        return Acc;
    }
    return convertListToNum(tl(L), base, base * Acc + hd(L));
}

// number base alapu szamrendszerben
list convertBase(const int number, const int base) {
    return convertBase(number, base, nil);
}

// minden masodik elem kiszurese
list filterEven(list L) {
    return filterEven(L, 0, nil);
}

// reverse(L) = az L lista megfordítva
list reverse(const list L) {
    return revapp(L, nil);
}

// az eredeti listaba beleilleszti az even lista elemeit, minden masodik helyre
list kevertList(list original) {
    return reverse(kevertList(original, filterEven(original), 0, nil));
}

// L listat kiolvassa 10-es szamrendszerben
int convertListToNum(list L, int base) {
    return convertListToNum(L, base, 0);
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
    return convertListToNum(kevertList(convertBase(S, A)), A);
}