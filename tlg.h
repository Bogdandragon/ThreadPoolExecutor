/* ANGHELOIU George-Bogdan - grupa 314CB */
/*-- tlg.h --- LISTA SIMPLU INLANTUITA GENERICA ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celulag
{
  void* info;           /* adresa informatie */
  struct celulag *urm;   /* adresa urmatoarei celule */
  struct celulag *prec;   /* adresa precedentei celule */
} TCelulaG, *TLG; 	/* tipurile Celula, Lista generice */


/* functii lista generica */
TLG Aloc_CelulaG(void* x);

int Ins_IncLG(TLG*, void*);  /*- inserare la inceput reusita sau nu (1/0) -*/
int Ins_DecLG(TLG*, void*);  /*- inserare la final reusita sau nu (1/0) -*/

#endif
