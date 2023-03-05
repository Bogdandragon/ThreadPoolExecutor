/* ANGHELOIU George-Bogdan - grupa 314CB */
/*--- functiiLG.c -- operatii de baza pentru lista simplu inlantuita generica---*/
#include "tlg.h"

TLG Aloc_CelulaG(void* x)
{
	TLG aux;
  	aux = (TLG)malloc(sizeof(TCelulaG));
   	if(!aux) return NULL;
   	
	aux->info = x;
	aux->urm = NULL;
	aux->prec = NULL;
	return aux;
}

int Ins_IncLG(TLG* aL, void* ae)
{
	TLG aux = Aloc_CelulaG(ae);
	if(!aux)
	    return 0;

	aux->urm = *aL;
	if(*aL) {
		if((*aL)->prec){
			(*aL)->prec->urm = aux;
			aux->prec = (*aL)->prec;
		} else {
			aux->prec = (*aL);
			(*aL)->urm = aux;
		}
			
		(*aL)->prec = aux;
	} else {
		aux->prec = NULL;
	}
	*aL = aux;

	return 1;
}

int Ins_DecLG(TLG* aL, void* ae)
{
	TLG aux = Aloc_CelulaG(ae);
	if(!aux)
	    return 0;
	if(*aL == NULL) {
		*aL = aux;
		aux->urm = aux;
		aux->prec = aux;
		return 1;
	}
	
	(*aL)->prec->urm = aux;
	aux->prec = (*aL)->prec;
	aux->urm = *aL;
	(*aL)->prec = aux;
	
	return 1;
}