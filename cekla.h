/* -*- c++ -*- $Revision: 862 $
 *
 *  Copyright (C) 2011, BME Declarative Programming Course
 *   Authors: Richard Kapolnai, Peter Szeredi
 *
 *  List functions for Cekla. Released under GPL.
 */

#ifndef CEKLA_H
#define CEKLA_H

/**
 * \english
 * @file cekla.h Cekla C++ library for builtins.
 * @mainpage Cekla C++ library for declarative C++.
 * <h2>Installation</h2>
 * Simply place the file <tt>cekla.h</tt> in the source directory (with
 * the <tt>*.cpp</tt> files) or in the preprocessor include path
 * (directories to be searched for header files).
 * Or for GCC, set the env var CPATH to point to the directory containing
 * <tt>cekla.h</tt>.
 * Test your configuration, for example
 * <pre>\#include "cekla.h"
int is_empty(const #list L) { return L == #nil; }
int main() {
    #writeln(is_empty(#nil));
    #writeln(is_empty(#cons(10, #nil)));
}</pre>
 *
 * <h2>Usage</h2>
 * - In case of using lists (see \link CeklaList \endlink module) or
 *   function types (see \link CeklaFunctional \endlink module), include
 *   <tt>cekla.h</tt>.
 * - Non-declarative function calls are allowed for debugging, for
 *   example you can place a <tt>#writeln</tt> call between statements.
 * - Type <tt>help;</tt> in the Cekla interpreter for the allowed syntax.
 *
 * <h2>Macros to change behaviour</h2>
 * - <tt>NDEBUG</tt>: if defined, #write, #writeln will not print
 *   debugging info (the line number of call in source code).
 * - <tt>ENABLE_SHARED_PTR</tt>: enables garbage collection to
 *   prevent memory leaks, but usually disables GCC optimization of
 *   tail recursive functions.
 *
 * \author Copyright (C) 2011, BME Declarative Programmming Course, Richard Kapolnai, Peter Szeredi
 * \verbatim  http://dp.iit.bme.hu/  $Revision: 862 $ \endverbatim
 * \endenglish
 *
 * \hungarian
 * @file cekla.h Cekla C++ kĂśnyvtĂĄr.
 * @mainpage Cekla C++ kĂśnyvtĂĄr deklaratĂ­v C++-hoz
 * <h2>InstallĂĄlĂĄs</h2>
 * HelyezzĂźk a <tt>cekla.h</tt> fĂĄjlt a forrĂĄsfĂĄjlokkal egy kĂśnyvtĂĄrba,
 * vagy a header fĂĄjlok keresĂŠsi ĂştvonalĂĄba.
 * Vagy GCC esetĂŠn a CPATH kĂśrnyezeti vĂĄltozĂłt ĂĄllĂ­tsuk a <tt>cekla.h</tt>-t
 * tartalmazĂł kĂśnyvtĂĄrra, pĂŠldĂĄul <tt>export CPATH=/opt/cekla/include</tt>.
 * A konfigurĂĄciĂł tesztelĂŠsĂŠhez egy rĂśvid pĂŠlda:
 * <pre>\#include "cekla.h"
int is_empty(const #list L) { return L == #nil; }
int main() {
    #writeln(is_empty(#nil));
    #writeln(is_empty(#cons(10, #nil)));
}</pre>
 *
 * <h2>HasznĂĄlat</h2>
 * - ListĂĄkhoz (lĂĄsd a \link CeklaList \endlink modult) vagy
 *   fĂźggvĂŠnytĂ­pusokhoz (lĂĄsd a \link CeklaFunctional \endlink modult),
 *   hasznĂĄljuk az <tt>#include "cekla.h"</tt> direktĂ­vĂĄt.
 * - Nem-deklaratĂ­v fĂźggvĂŠnyhĂ­vĂĄsok is megengedettek pl. hibakeresĂŠshez
 *   pĂŠldĂĄul hasznĂĄlhatjuk a <tt>#writeln</tt>-t utasĂ­tĂĄsok kĂśzĂśtt
 * - GĂŠpeljĂźk be a <tt>help;</tt> parancsot a Cekla ĂŠrtelmezĹjĂŠben a
 *   megengedett szintaxishoz.
 *
 * <h2>Fontos makrĂłk</h2>
 * - <tt>NDEBUG</tt>: ha definiĂĄljuk, a #write, #writeln hĂ­vĂĄsok nem
 *   Ă­rnak ki debug informĂĄciĂłt (a hĂ­vĂĄs sorszĂĄmĂĄt a forrĂĄskĂłdban).
 * - <tt>ENABLE_SHARED_PTR</tt>: bekapcsolja a szemĂŠtgyĹąjtĂŠst
 *   megakadĂĄlyozva a memĂłriaszivĂĄrgĂĄst, de lehetetlennĂŠ teszi
 *   a GCC-nek, hogy optimalizĂĄlja a jobbrekurzĂ­v fĂźggvĂŠnyeket.
 *
 * \author Copyright (C) 2011, BME DeklaratĂ­v ProgramozĂĄs, KĂĄpolnai RichĂĄrd, Szeredi PĂŠter
 * \verbatim  http://dp.iit.bme.hu/  $Revision: 862 $ \endverbatim
 * \endhungarian
 *
 *
 */

#include <stdexcept>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <new>
#ifdef ENABLE_INITIALIZER_LIST // __GXX_EXPERIMENTAL_CXX0X__
#  include <initializer_list>
#endif

// Uncomment to disable debugging info in write and writeln
//#define NDEBUG

// Uncomment to enable garbage collection.
// WARNING: defining this macro will cause memory leaks!!!
//#define ENABLE_SHARED_PTR

/** \addtogroup CeklaList
 * \english
 * List handling in C++.
 * \endenglish
 *
 * \hungarian
 * ListakezelĂŠs C++-ban.
 * \endhungarian
 * @{
 */

/**
 * \english
 * List of integers.
 * Strings (C-style character arrays, e.g. <tt>"hello"</tt>) are
 * treated as a list of the codes of the characters, not included
 * the terminating <tt>'\\0'</tt> character. For example:
 * \endenglish
 *
 * \hungarian
 * EgĂŠszek listĂĄja.
 * Sztring (C-nyelvĹą karaktertĂśmb, pl. <tt>"hello"</tt>) karakterkĂłdok
 * listĂĄjĂĄnak tekintendĹ a lezĂĄrĂł <tt>'\\0'</tt>  nĂŠlkĂźl. PĂŠldĂĄul:
 * \endhungarian
 * <tt>cons(72, cons(101, cons(108, cons(108, cons(111, nil))))) == "Hello"</tt>.
 */
class list {

public:

    /**
     * \english
     * Constructs a list of character codes.
     * @throw std::logic_error if NULL pointer is given.
     *
     * Useful for implicit conversion of a string to a list or quick construction. For example
     * <tt>const list L1 = "Hello", L2 = "";</tt>, or
     * <tt>X = tl("Hello")</tt> instead of
     * \endenglish
     *
     * \hungarian
     * FelĂŠpĂ­ti a karakterkĂłdok listĂĄjĂĄt.
     * @throw std::logic_error ha a paramĂŠter NULL pointer.
     *
     * Hasznos sztring implicit konverziĂłjĂĄhoz, pĂŠldĂĄul
     * <tt>const list L1 = "Hello", L2 = "";</tt>, vagy
     * <tt>X = tl("Hello")</tt> rĂśvidĂ­ti az alĂĄbbit:
     * \endhungarian
     * <tt>X = tl(cons('H', cons('e', cons('l', cons('l', cons('o', nil))))))</tt>.
     */
    list(const char *S) throw (std::logic_error);

#ifdef ENABLE_INITIALIZER_LIST
    /**
     * \english
     * Constructs a list of integers given in the initializer_list S.
     * Requires C++0x and ENABLE_INITIALIZER_LIST defined.
     * E.g.:
     * \endenglish
     *
     * \hungarian
     * FelĂŠpĂ­ti listĂĄt az inicializĂĄlĂł lista szĂĄmaibĂłl.
     * Csak C++0x ĂŠs ENABLE_INITIALIZER_LIST definĂĄlĂĄsa esetĂŠn elĂŠrhetĹ.
     * PĂŠldĂĄul:
     * \endhungarian
     * <tt>tl({10, 20, 30})</tt>.
     */
    list(std::initializer_list<int> S);
#endif

    /**
     * \english
     * Compares two lists.
     * @return True (nonzero) if two lists are equival.
     * \endenglish
     *
     * \hungarian
     * ĂsszehasonlĂ­t kĂŠt listĂĄt.
     * @return Igaz (nem nulla), ha a kĂŠt lista egyezik.
     * \endhungarian
     */
    bool operator==(const list & Rhs) const;
    /**
     * \english
     * Compares two lists.
     * @return True (nonzero) if two lists are not equival.
     * @see #operator==
     * \endenglish
     *
     * \hungarian
     * ĂsszehasonlĂ­t kĂŠt listĂĄt.
     * @return Igaz (nem nulla), ha a kĂŠt lista nem egyezik.
     * @see #operator==
     * \endhungarian
     */
    bool operator!=(const list & Rhs) const;

#ifdef ENABLE_SHARED_PTR
    ~list();
#endif

    // for cons, hd, tl, because couldn't convince Doxygen
    // to omit friend declarations
    list(int, const list);
    int head() const;
    list tail() const;
private:
    // BFF
//    friend list cons(int Head, const list Tail);
//    friend int hd(const list L);
//    friend list tl(const list L);

    struct elem;

#ifndef ENABLE_SHARED_PTR
    // C pointer, causes memory leak
    typedef const elem * shared_ptr;
#else
    // our replacement of TR1 shared_ptr for reference counting
    // because need to do nasty things in ~list(), need a public counter
    struct shared_ptr {
        const elem * p;
        void inc() { if (p) p->refcount++; }
        void dec() { if (p) if (0 == --(p->refcount)) delete p; }
        shared_ptr(const elem *p = NULL) : p(p) { inc(); }
        shared_ptr(const shared_ptr & o) : p(NULL) { *this = o; }
        shared_ptr & operator=(const shared_ptr & o) { dec(); p = o.p; inc(); return *this; }
        const elem* operator->() const { return p; }
        operator const elem* const & () const { return p; }
        //const elem operator*() const { //TODO }
        ~shared_ptr() { dec(); }
    };
#endif

    struct elem {
        // Underlying data
        int data;
        // The rest of the list
        shared_ptr next;

        elem(int data, shared_ptr next = 0) : data(data), next(next)
#ifdef ENABLE_SHARED_PTR
        , refcount(0)
#endif
        {}

#ifdef ENABLE_SHARED_PTR
        // for smart pointer
        mutable int refcount;
    private:
        void operator=(const elem&);
        elem(const elem&);
#endif
    };

    // The first (head) element of the list
    shared_ptr first;

};

/**
 * \english
 * Returns a new list which starts with Head and followed by Tail.
 * @param Head The element will be the first element of the list.
 * @param Tail The list will be the rest of the list.
 * @return The constructed list.
 *
 * Complexity: O(1).
 *
 * For example:
 * \endenglish
 *
 * \hungarian
 * Visszaad egy Ăşj listĂĄt, aminek elsĹ eleme Head, a farka a Tail lista.
 * @param Head Az elem lesz az Ăşj lista elsĹ eleme.
 * @param Tail A lista lesz az Ăşj lista tĂśbbi eleme.
 * @return A felĂŠpĂ­tett lista.
 *
 * FutĂĄsi idĹ: O(1).
 *
 * PĂŠldĂĄul:
 * \endhungarian
 * <tt>cons('H', cons('e', cons('l', cons('l', cons('o', nil))))) == "Hello"</tt>.
 */
list cons(int Head, const list Tail);

/**
 * \english
 * Returns the head of the non-empty list L.
 * @return The first element of L.
 *
 * Complexity: O(1).
 *
 * For example:
 * \endenglish
 *
 * \hungarian
 * Visszaadja a nemĂźres L lista fejĂŠt.
 * @return L elsĹ eleme.
 *
 * FutĂĄsi idĹ: O(1).
 *
 * PĂŠldĂĄul:
 * \endhungarian
 * <tt>hd("Hello") == 'H'</tt>.
 */
int hd(const list L);

/**
 * \english
 * Returns the tail of the non-empty list L.
 * @return A list missing the first element of L.
 *
 * Complexity: O(1).
 *
 * For example:
 * \endenglish
 *
 * \hungarian
 * Visszaadja a nemĂźres L lista farkĂĄt.
 * @return L lista elemei, az elsĹ elem kivĂŠtelĂŠvel.
 *
 * FutĂĄsi idĹ: O(1).
 *
 * PĂŠldĂĄul:
 * \endhungarian
 * <tt>tl("Hello") == "ello"</tt>.
 */
list tl(const list L);

/**
 * \english
 * The empty list.
 * Fact: <tt>nil == ""</tt>.
 * Useful to decide if a list is empty: <tt>tl("e") == nil</tt>,
 * or constructing a list:
 * \endenglish
 *
 * \hungarian
 * Az Ăźres lista.
 * A <tt>nil == ""</tt> teljesĂźl.
 * HasznĂĄlhatĂł egy lista vizsgĂĄlatĂĄra, hogy Ăźres-e: <tt>tl("e") == nil</tt>,
 * vagy lista ĂŠpĂ­tĂŠsĂŠre:
 * \endhungarian
 * <tt>cons('H', cons('e', cons('l', cons('l', cons('o', nil))))) == "Hello"</tt>.
 */
extern const list nil;

// ------------------- Other improvements ----------------------

/**
 * \english
 * Writes X to the stdout.
 * @param X Its type can be int, const string or list.
 * Lists are printed using heuristics: if contains an integer not in
 * 32..126, then treated as a list of integers (e.g. [10, 20, 30]),
 * else treated as a list of character codes (e.g. "hello").
 * \endenglish
 *
 * \hungarian
 * KiĂ­rja X-et a standard kimenetre.
 * @param X TĂ­pusa lehet int, sztringkonstans vagy lista.
 * Ha a lista nemcsak 32..126 kĂśzĂśtti szĂĄmokat tartalmaz, egĂŠsz listakĂŠnt
 * Ă­rĂłdik ki (pl. [10, 20, 30]), kĂźlĂśnben karakterkĂłd-listakĂŠnt (pl. "hello").
 * \endhungarian
 */
template <typename any_type>
void write(const any_type & X);

#ifdef NDEBUG
/**
 * \english
 * Writes X to the stdout followed by a newline.
 * \endenglish
 *
 * \hungarian
 * KiĂ­rja X-et a standard kimenetre, ĂŠs egy Ăşjsorjelet.
 * \endhungarian
 * @see write.
 */
template <typename any_type>
void writeln(const any_type & X);
#else
/**
 * \english
 * Writes X to the stdout followed by debugging info and a newline.
 * \endenglish
 *
 * \hungarian
 * KiĂ­rja X-et a standard kimenetre, debug infĂłval, majd egy Ăşjsorjelet.
 * \endhungarian
 */
#define writeln(X) detail::_writeln(X, __FILE__, __LINE__)
#endif

/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l() { return nil; }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E) { return cons(E, nil); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2) { return cons(E1,l(E2)); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2,int E3) { return cons(E1,l(E2,E3)); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2,int E3,int E4) { return cons(E1,l(E2,E3,E4)); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2,int E3,int E4,int E5) { return cons(E1,l(E2,E3,E4,E5)); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2,int E3,int E4,int E5,int E6) { return cons(E1,l(E2,E3,E4,E5,E6)); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2,int E3,int E4,int E5,int E6,int E7) { return cons(E1,l(E2,E3,E4,E5,E6,E7)); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2,int E3,int E4,int E5,int E6,int E7,int E8) { return cons(E1,l(E2,E3,E4,E5,E6,E7,E8)); }
/// \english Returns the list containging the parameters. \endenglish \hungarian Visszaadja a paramĂŠterekbĹl alkotott listĂĄt. \endhungarian
list l(int E1,int E2,int E3,int E4,int E5,int E6,int E7,int E8,int E9) { return cons(E1,l(E2,E3,E4,E5,E6,E7,E8,E9)); }
/**
 * \english
 * Returns the list containging the parameters.
 * E.g.:
 * \endenglish
 *
 * \hungarian
 * Visszaadja a paramĂŠterekbĹl alkotott listĂĄt.
 * PĂŠldĂĄul:
 * \endhungarian
 * <tt>const list L = l(10,20,30,40,50,60,70,80,90,0);</tt>
 */
list l(int E1,int E2,int E3,int E4,int E5,int E6,int E7,int E8,int E9,int E10) { return cons(E1,l(E2,E3,E4,E5,E6,E7,E8,E9,E10)); }

/** @}
 * \addtogroup CeklaFunctional
 * \english
 * Functional objects for high order functions in C++. @{
 * Can be used to pass an integer function. For example, the following
 * program outputs
 * \endenglish
 *
 * \hungarian
 * TĂ­pusok magasabbrendĹą fĂźggvĂŠnyekhez. @{
 * EgĂŠsz-fĂźggvĂŠnyek ĂĄtadĂĄsĂĄra hasznĂĄlhatĂłak. PĂŠldĂĄul az alĂĄbbi program
 * kimenete
 * \endhungarian
 * <tt>1</tt>:
 * <pre>\#include "cekla.h"
<b>// Returns true if Predicate(X) is true for some element X of list L</b>
int contains(const #fun1 Predicate, const list L) {
    if (L == nil) return 0;
    else if (Predicate(hd(L))) return 1;
    else return contains(Predicate, tl(L)));
}
int even(const int x) { return x % 2 == 0; }
int main() {
    const list L = cons(1, cons(2, cons(3, nil)));
    write(contains(even, L));   <b>// prints if L contains even number</b>
}</pre>
 */

/**
 * \english
 * Unary function for high order functions.
 * \endenglish
 *
 * \hungarian
 * EgyparamĂŠteres fĂźggvĂŠnytĂ­pus magasabbrendĹą fĂźggvĂŠnyekhez.
 * \endhungarian
 */
typedef int (*fun1)(int);
/**
 * \english
 * Binary function for high order functions.
 * \endenglish
 *
 * \hungarian
 * KĂŠtparamĂŠteres fĂźggvĂŠnytĂ­pus magasabbrendĹą fĂźggvĂŠnyekhez.
 * \endhungarian
 */
typedef int (*fun2)(int, int);
/// @}

// -------------- Details, not important to a regular user -----------

namespace detail {

// how much chars are already printed to this line
    extern volatile int Tab;

// prints X, the current source_file:source_line and a newline (for debugging)
    template <typename any_type>
    void _writeln(const any_type & X, const char * Source_file, int Source_line);

// error handling
    void error(const char *Errmsg) throw (std::logic_error);

// list printing.
    std::ostream & operator<<(std::ostream & Os, const list & L);

}

// -------------- Implementation, not important to a regular user -----------
// -------------- WARNING! --------------
// Causes linker error in case of multiple source files:
// "multiple definition of ..."
// Could be put into a source file (and a lib) but it's easier to deploy in IDEs

const list nil = "";

template <typename any_type>
void write(const any_type & X) {
    using namespace detail;
    std::stringstream S;
    S << X;
    detail::Tab += (int)S.str().length();
    std::cout << X;
}

#ifdef NDEBUG
template <typename any_type>
void writeln(const any_type & X) {
    write(X);
    std::cout << std::endl;
}
#else
namespace detail {
    template <typename any_type>
    void _writeln(const any_type & X, const char * Source_file, int Source_line) {
        write(X);
        using namespace std;
        cout << setw(60 - Tab) << ' ' << setw(0);
        cout << " // " << Source_file << ":" << Source_line;
        Tab = 0;
        cout << endl;
    }
}
#endif

list::list(const char *S) throw (std::logic_error) : first() {
    if (!S) detail::error("ERROR in list(char*): null pointer given");
    *this = nil;
    for (const char * P = S + strlen(S) - 1; P >= S; P--)
        *this = cons(*P, *this);
}

#ifdef ENABLE_INITIALIZER_LIST
list::list(std::initializer_list<int> S) : first() {
    for (auto P = S.begin(); P != S.end(); ++P)
        *this = cons(*P, *this);
}
#endif

bool list::operator==(const list & Rhs) const {
    shared_ptr p1, p2;
    for (p1 = first, p2 = Rhs.first; p1 && p2; p1 = p1->next, p2 = p2->next)
        if (p1->data != p2->data)
            return false;
    return !p1 && !p2;
}

bool list::operator!=(const list & Rhs) const {
    return ! (*this == Rhs);
}

#ifdef ENABLE_SHARED_PTR
list::~list() {
    // Non-recursive deletion of linked elements
    // The builtin desctruction would cause stack overflow for big lists:
    // ~list() calls ~shared_ptr calls delete calls ~shared_ptr ...
    // So we delete elements with refcount==1
    for (const elem * p = first; p && p->refcount == 1; p = first) {
        first.p = NULL; // disable delete
        first = p->next;
        delete p;
    }
}
#endif

inline
list::list(int Head, const list Tail) : first() {
    first = new list::elem(Head, Tail.first);
}

list cons(int Head, const list Tail) {
    return list(Head, Tail);
}

inline
int list::head() const {
    if (first==NULL)
        detail::error("ERROR in head(): list is empty");
    return first->data;
}

int hd(const list L) {
    return L.head();
}

inline
list list::tail() const {
    if (first==NULL)
        detail::error("ERROR in tail(): list is empty");
    list L0 = nil;
    L0.first = first->next;
    return L0;
}

list tl(const list L) {
    // // prevents a function from being considered for inlining
    // __attribute__((noinline))
    // // From the GCC Manual: To keep such calls from being optimized away,
    // // put in the called function, to serve as a special side-effect.
    // asm ("");
    return L.tail();
}

namespace detail {

    void memory_error() {
        std::cerr << "ERROR: out of memory" << std::endl;
        throw std::bad_alloc();
    }

// to start automatically before main()
    struct bootstrap {
        bootstrap() {
            std::set_new_handler(memory_error);
        }
    } _startup;

    volatile int Tab = 0;

    void error(const char *errmsg) throw (std::logic_error) {
        std::logic_error e(errmsg);
        std::cerr << e.what() << std::endl;
        throw e;
    }

    std::ostream & operator<<(std::ostream & Os, const list & L) {
        bool Is_szoveg = true;
        for (list L0 = L; L0 != nil; L0 = tl(L0)) {
            if (hd(L0) < 32 || 126 < hd(L0)) {
                Is_szoveg = false;
                break;
            }
        }
        if (Is_szoveg && L != nil) {
            //  os << '"';
            for (list L0 = L; L0 != nil; L0 = tl(L0))
                Os << (char) hd(L0);
            //  os << '"';
        }
        else {
            Os << "[";
            bool First = true;
            for (list L0 = L; L0 != nil; L0 = tl(L0)) {
                if (!First) Os << ",";
                Os << hd(L0);
                First = false;
            }
            Os << "]";
        }
        return Os;
    }

}

#endif /* CEKLA_H */