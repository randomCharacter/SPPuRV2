#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include "tbb/task.h"
#include "tbb/tick_count.h"

using namespace std;
using namespace tbb;

/* Dimenizije matrica */
#define M   2


unsigned int in[M][M] =
                {
                {0, 1},
                {2, 3},
                };


unsigned int c[M][M] =
                {
                {4, 5},
                {6, 7},
                };


unsigned int alpha[M][M] =
                {
                {8, 9},
                {10, 11},
                };

/* Klasa zadataka druge faze */
class PhaseThreeTask : public task
{

public:

    PhaseThreeTask(int _i, unsigned int* _res) :
        i(_i),
        result(_res)
    {
    }

    task* execute()
    {
        /*
        * Zadaci druge faze samo ispisuju ono sto su dobili kao rezultat
        * zadataka prve faze.
        * Izmeniti kod execute metode na taj nacin da zadaci druge faze
        * nastave racunanje DCT algoritma
        */
        __TBB_ASSERT(ref_count() == 0, NULL);

        result[0] = input1[0] * input2[0];
        result[1] = input1[1] * input2[1];

        if (successor != NULL)
        {
            /*
            * Smanjivanje broja referenci zadatka sledbenika.
            * Ukoliko broj referenci zadatka sledbenika padne na nulu
            * pokrenuti zadatak sledbenik
            */
            if (successor->decrement_ref_count() == 0)
                spawn(*successor);
        }

        return NULL;
    }

public:

    /* koordinate rezultujuceg elementa u matrici */
    int i;
    int j;
    /* ulazni podaci za zadatak druge faze */
    unsigned int input1[M];
    unsigned int input2[M];
    unsigned int *result;
    /* zadatak sledbenik druge faze zadataka */
    empty_task* successor;

};

/* Klasa zadataka druge faze */
class PhaseTwoTask : public task
{

public:

    PhaseTwoTask(int _i, int _j) :
        i(_i),
        j(_j)
    {
    }

    task* execute()
    {
        /*
        * Zadaci druge faze samo ispisuju ono sto su dobili kao rezultat
        * zadataka prve faze.
        * Izmeniti kod execute metode na taj nacin da zadaci druge faze
        * nastave racunanje DCT algoritma
        */
        __TBB_ASSERT(ref_count() == 0, NULL);

        int result = 0;

        /* racunanje odgovarajuceg elementa za sledecu fazu */
        for (int k = 0; k < M; k++)
        {
            result += input1[k] * input2[k];
        }

        /* prolazak kroz niz sledbenika */


        if (successor != NULL)
        {
            /*
            * Smanjivanje broja referenci zadatka sledbenika.
            * Ukoliko broj referenci zadatka sledbenika padne na nulu
            * pokrenuti zadatak sledbenik
            */
            successor->input1[j] = result;
            if (successor->decrement_ref_count() == 0)
                spawn(*successor);
        }

        return NULL;
    }

public:

    /* koordinate rezultujuceg elementa u matrici */
    int i;
    int j;
    /* ulazni podaci za zadatak druge faze */
    unsigned int input1[M];
    unsigned int input2[M];
    /* zadatak sledbenik druge faze zadataka */
    PhaseThreeTask* successor;

};


/* Klasa zadataka prve faze */
class PhaseOneTask : public task
{

public:

    PhaseOneTask(int _i, int _j) :
        i(_i),
        j(_j)
    {
    }

    task* execute()
    {
        __TBB_ASSERT(ref_count() == 0, NULL);

        int result = 0;

        /* racunanje odgovarajuceg elementa za sledecu fazu */
        for (int k = 0; k < M; k++)
        {
            result += input1[k] * input2[k];
        }

        /* prolazak kroz niz sledbenika */
        for (int k = 0; k < M; k++)
        {
            if (successor[k] != NULL)
            {
                /*
                * Postavljanje ulaznih podataka zadatku sledbeniku
                * Napomena: za indeksiranje se koristi "j" koordinata jer
                * je zadacima prve faze zajednicka vrsta
                */
                successor[k]->input1[j] = result;

                /*
                * Smanjivanje broja referenci zadatka sledbenika.
                * Ukoliko broj referenci zadatka sledbenika padne na nulu
                * pokrenuti zadatak sledbenik
                */
                if (successor[k]->decrement_ref_count() == 0)
                    spawn(*successor[k]);
            }
        }
        return NULL;
    }

public:

    /* koordinate rezultujuceg elementa u matrici */
    int i;
    int j;
    /* ulazni podaci za zadatak prve faze */
    unsigned int input1[M];
    unsigned int input2[M];
    /* niz sledbenika zadatka prve faze */
    PhaseTwoTask* successor[M];

};

/* Glavna programska nit */
int main()
{
    unsigned int res[M][M] = {0};

    cout << endl << "DCT parallel version" << endl << endl;

    /* pocetno vreme */
    tick_count startTime = tick_count::now();

    /* prazan zadatak, koji ceka da se zadati posao obavi */
    empty_task* empty = new (task::allocate_root()) empty_task();
    empty->set_ref_count(3); //faza dva ima 4 zadatka + 1 za cekanje

    PhaseThreeTask* rrr0 = new (task::allocate_root()) PhaseThreeTask(0, res[0]);
    rrr0->set_ref_count(2);
  	rrr0->input2[0] = alpha[0][0];
  	rrr0->input2[1] = alpha[0][1];
    rrr0->successor = empty;

    PhaseThreeTask* rrr1 = new (task::allocate_root()) PhaseThreeTask(1, res[1]);
    rrr1->set_ref_count(2);
  	rrr1->input2[0] = alpha[1][0];
  	rrr1->input2[1] = alpha[1][1];
    rrr1->successor = empty;

    /* zadatak koji izracunava RR[0][0] */
    PhaseTwoTask* rr00 = new (task::allocate_root()) PhaseTwoTask(0, 0);
    rr00->set_ref_count(2);
  	rr00->input2[0] = c[0][0];
  	rr00->input2[1] = c[0][1];
    rr00->successor = rrr0;

    /* zadatak koji izracunava RR[0][1] */
    PhaseTwoTask* rr01 = new (task::allocate_root()) PhaseTwoTask(0, 1);
    rr01->set_ref_count(2);
  	rr01->input2[0] = c[1][0];
  	rr01->input2[1] = c[1][1];
    rr01->successor = rrr0;

    /* zadatak koji izracunava RR[1][0] */
    PhaseTwoTask* rr10 = new (task::allocate_root()) PhaseTwoTask(1, 0);
    rr10->set_ref_count(2);
  	rr10->input2[0] = c[0][0];
  	rr10->input2[1] = c[0][1];
    rr10->successor = rrr1;

    /* zadatak koji izracunava RR[1][1] */
    PhaseTwoTask* rr11 = new (task::allocate_root()) PhaseTwoTask(1, 1);
    rr11->set_ref_count(2);
  	rr11->input2[0] = c[1][0];
  	rr11->input2[1] = c[1][1];
    rr11->successor = rrr1;

    /* zadatak koji izracunava R[0][0] */
    PhaseOneTask* r00 = new (task::allocate_root()) PhaseOneTask(0, 0);
    r00->successor[0] = rr00;
    r00->successor[1] = rr01;
    r00->input1[0] = in[0][0];
    r00->input1[1] = in[0][1];
    r00->input2[0] = c[0][0];
    r00->input2[1] = c[1][0];
    task::spawn(*r00);

    /* zadatak koji izracunava R[0][1] */
    PhaseOneTask* r01 = new (task::allocate_root()) PhaseOneTask(0, 1);
    r01->successor[0] = rr00;
    r01->successor[1] = rr01;
    r01->input1[0] = in[0][0];
    r01->input1[1] = in[0][1];
    r01->input2[0] = c[0][1];
    r01->input2[1] = c[1][1];
    task::spawn(*r01);

    /* zadatak koji izracunava R[1][0] */
    PhaseOneTask* r10 = new (task::allocate_root()) PhaseOneTask(1, 0);
    r10->successor[0] = rr10;
    r10->successor[1] = rr11;
    r10->input1[0] = in[1][0];
    r10->input1[1] = in[1][1];
    r10->input2[0] = c[0][0];
    r10->input2[1] = c[1][0];
    task::spawn(*r10);

    /* zadatak koji izracunava R[1][1] */
    PhaseOneTask* r11 = new (task::allocate_root()) PhaseOneTask(1, 1);
    r11->successor[0] = rr10;
    r11->successor[1] = rr11;
    r11->input1[0] = in[1][0];
    r11->input1[1] = in[1][1];
    r11->input2[0] = c[0][1];
    r11->input2[1] = c[1][1];
    task::spawn(*r11);


    /* cekanje praznog zadatka */
    empty->wait_for_all();

    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        std::cout << res[i][j] << " ";
      }
      std::cout << endl;
    }


    task::destroy(*empty);

    /* Krajnje vreme */
    tick_count endTime = tick_count::now();

    cout << endl << "Execution time [" << M << "][" << M << "]: " << (endTime-startTime).seconds()*1000 << "ms." << endl << endl;

    return 0;
}
