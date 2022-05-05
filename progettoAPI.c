#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*____________*
 *  COSTANTI  *
 *____________*/

//Colori albero RB
#define RED   0
#define BLACK 1

//Tipi di comando
#define ADDENT 1
#define DELENT 2
#define ADDREL 3
#define DELREL 4
#define REPORT 5
#define END    6

//Lunghezza del buffer per prendere la stringa
#define LBUFFER 500

//Di quanto aumento il vettore type ogni volta
#define OFFSETTYPE 5

//Lunghezza iniziale dei vettori type dell'albero delle relazioni
#define LTINIZ 3

//offset per il vettore che contiene i nomi delle entita con il numero massimo di un tipo di relazione entrante
#define OFFSETINFONOMI 40

typedef struct strRep{
	char **nomeEnt;
	unsigned int nNomi;
	unsigned int lNomi;
	unsigned int numRelIn;

	char *tipoRel;

}info;

//Struttura che contiene per una entita quante relazioni in ingresso di quel tipo hanno.
typedef struct vectReport{
	unsigned int indexType;
	unsigned int nInput;
}report;

//Vettore dinamico che contiene i tipi di relazione
char **typesRel;
unsigned int lTypes, nTypes;

//Albero RB per le relazioni
typedef struct treeRBRel{
	char *entOrig;
	struct treeRBRel *l,*r,*p;
	char col;

	unsigned int *indexType;
	unsigned int lIndType;
	unsigned int nIndType;
}*nodoRel, dimNodoRel;


//Albero RB per le entita
typedef struct treeRBEnt{
	char *nomeEnt;
	struct treeRBEnt *l,*r,*p;
	char col;

	nodoRel relIn;	//Relazioni entranti
	
	report* rep;
	unsigned int lRep;
	unsigned int nRep;
}*nodoEnt, dimNodoEnt;

//Radice dell'albero entita
nodoEnt radEnt;

//_____FUNZIONI GENERICHE ALBERO ENTITA_____//

//Stampa albero RB
void visitaEnt(nodoEnt T);

void visitaRel(nodoRel T);

//Cerca un nodo entita all'interno dell'albero entita
nodoEnt searchEnt(nodoEnt T, char *name);


//_____FUNZIONI CHE ESEGUONO ROTAZIONI SULL'ALBERO RELAZIONI_____//

//Esegue una left rotate T è la radice, x è il nodo su cui eseguirla. Suppongo che  esista il figlio destro di x
void leftRotateRel(nodoRel *T, nodoRel x);
//Esegue una right rotate T è la radice, x è il nodo su cui eseguirla. Suppongo che  esista il figlio sinistro di x
void rightRotateRel(nodoRel *T, nodoRel x);


//_____FUNZIONI CHE ESEGUONO ROTAZIONI SULL'ALBERO ENTITA_____//

//Esegue una left rotate T è la radice, x è il nodo su cui eseguirla. Suppongo che  esista il figlio destro di x
void leftRotateEnt(nodoEnt *T, nodoEnt x);
//Esegue una right rotate T è la radice, x è il nodo su cui eseguirla. Suppongo che  esista il figlio sinistro di x
void rightRotateEnt(nodoEnt *T, nodoEnt x);


//_____FUNZIONI PER INSERIRE NODI NELL'ALBERO RELAZIONI_____//

//Restituisce nodo creato di un albero, l è numero dei caratteri + 1 per lo \0
nodoRel createNodoRel(char *nameOrig, unsigned int tipo);
//Crea e inserisce nodo nell' albero delle relazioni
void insertNodeRel(nodoRel *T, char *nameOrig, nodoEnt dest, unsigned int indiceTipo);
//Ricolora i nodi ed effettua delle rotazioni per preservare le proprieta dell' albero RB delle relazioni dopo una insert
void insertFixupRel(nodoRel *T, nodoRel x);

//_____FUNZIONI PER INSERIRE NODI NELL'ALBERO ENTITA_____//

//Restituisce nodo creato di un albero, l è numero dei caratteri + 1 per lo \0
nodoEnt createNodoEnt(char *name, unsigned int l);
//Crea e inserisce nodo nell' albero delle entita
void insertNodeEnt(nodoEnt *T, char *name, unsigned int l);
//Ricolora i nodi ed effettua delle rotazioni per preservare le proprieta dell' albero RB delle entita dopo una insert
void insertFixupEnt(nodoEnt *T, nodoEnt x);

//_____GESTIONE NODO NIL PER L'ALBERO RELAZIONI_____//

//Nodo nil per l' ablero RB delle relazioni
nodoRel nilRel;
//Alloca il nodo nilRel
void createNilRel();


//_____GESTIONE NODO NIL PER L'ALBERO ENTITA_____//

//Nodo nil per l' ablero RB delle entita
nodoEnt nilEnt;
//Alloca il nodo nilEnt
void createNilEnt();



//_____FUNZIONI PER LEGGERE I COMANDI_____//
//Ritorna il tipo di comando inserito
char getComando();
//Riceve il tipo di comando e lo esegue
char gestisciComando(char tipo);
//Riceve una parola tra virgolette da stdin e restituisce il puntatore a essa
void getStringa(char *buf, unsigned int *l);
//Da usare quando si vuole andare a capo nel file
void endLine();


//_____FUNZIONI PER VETTORE DINAMICO TIPI_____//
//Inizializza e alloca il vettore con lTypes = LTINIZ
void initType();
//Aggiunge un tipo di relazione al vettore types
void addType(char *idType, unsigned int lIdType);
//Ritorna posizione cella del vettore types relativo al tipo cercato
int searchType(char *idType);


//_____FUNZIONI GESTIONE REPORT_____//
//Ritorna posizione cella del vettore report relativo al tipo cercato
int searchReport(nodoEnt n, unsigned int indType);
//Aggiunge un report al vettore report dell' entita
void addReport(nodoEnt d, unsigned int index);
//Incrementa il contatore di relazioni entranti nel report di quell entita
void updateReport(nodoEnt d, unsigned int indReport);

//_____FUNZIONI GESTIONE BETWEEN_____//
//Aggiunge un tipo fra le due entita 
void addTypeBetween(nodoRel r, unsigned int indiceTipo);
//Ritorna posizione cella del vettore indextype relativo al tipo fra entita cercato
int searchTypeBetween(nodoRel r, unsigned int indType);


//_____FUNZIONI PER LA REPORT_____//
//Riempie il vettore di tipo info ricorsivamente (vettore non ordinato) con tutti i dati pronti da stampare per la report
void fillInfo(nodoEnt n, info *i);
//Stampa report
void printReport();
//Riordina info in base ai nomi dei tipi di rel

//_____QUICKSORT PER INFO_____//
void quicksort(info *a, int p, int r);
int partition(info *a, int p, int r);



//_____FUNZIONI PER ELIMINARE NODI DALL'ALBERO RELAZIONI_____//

//Sostituisce un sottoalbero relazioni (con radice u), in qualita di figlio di suo padre, con un altro sottoalbero (con radice v)
void trasplantRel(nodoRel *T, nodoRel u, nodoRel v);
//Cancella un nodo appartenente all' albero delle relazioni
void deleteNodeRel(nodoRel *T, nodoRel relInput);
//Ricolora i nodi ed effettua delle rotazioni per preservare le proprieta dell' albero delle relazioni dopo una delete
void deleteFixupRel(nodoRel *T, nodoRel x);
//Restituisce il minimo di un sottoalbero delle relazioni
nodoRel treeMinRel(nodoRel n);
//Dealloca nodo e tutto quello che c'è dentro
void freeNodoRel(nodoRel x);
//cancella la relazione fra due entita
void delRel(char *idOrig, char *idDest, char *idType);
//Cerca una relazione con un entita  di origine
nodoRel searchRel(nodoRel T, char *entOrig);


//_____FUNZIONI PER ELIMINARE NODI DALL'ALBERO ENTITA_____//

//Sostituisce un sottoalbero entita (con radice u), in qualita di figlio di suo padre, con un altro sottoalbero (con radice v)
void trasplantEnt(nodoEnt *T, nodoEnt u, nodoEnt v);
//Cancella un nodo appartenente all' albero delle entita
void deleteNodeEnt(nodoEnt *T, nodoEnt z);
//Ricolora i nodi ed effettua delle rotazioni per preservare le proprieta dell' albero delle entita dopo una delete
void deleteFixupEnt(nodoEnt *T, nodoEnt x);
//Restituisce il minimo di un sottoalbero delle entita
nodoEnt treeMinEnt(nodoEnt n);
//Dealloca nodo e tutto quello che c'è dentro
void freeNodoEnt(nodoEnt x);


//_____FUNZIONI PER LA DELENT_____//
//Cancella un albero di relazioni in ingresso
void deleteRbRel(nodoRel *h);
//Cancella le relazioni relative a quella entita da tutti gli alberi dei nodi destinatari
void deleteRelFrom(nodoEnt dest, char *nameOrig);



int main(){

	createNilEnt();
	createNilRel();
	initType();

	radEnt = nilEnt;

	char quit = 0;

	while(!quit){
		quit = gestisciComando(getComando());
		
	}

	return 0;
}

void deleteRelFrom(nodoEnt dest, char *nameOrig){
	if(dest->l != nilEnt)
		deleteRelFrom(dest->l, nameOrig);
	if(dest->r != nilEnt)
		deleteRelFrom(dest->r, nameOrig);

	nodoRel relInput = searchRel(dest->relIn, nameOrig);
	if(relInput != nilRel){
		//Elimino i report di quella relazione
		for(int i=0; i < relInput->nIndType; i++){
			unsigned int indRep = searchReport(dest, relInput->indexType[i]);
			dest->rep[indRep].nInput--;
		}
		deleteNodeRel(&(dest->relIn), relInput);
	}
}

void delent(char *name){
	
	nodoEnt n = searchEnt(radEnt, name);
	if(n == nilEnt)
		return;

	deleteRelFrom(radEnt, name);
	deleteRbRel(&(n->relIn));
	deleteNodeEnt(&radEnt, n);

}

void fillInfo(nodoEnt n, info *i){

	//Algoritmo ricorsivo
	if(n->l != nilEnt)
		fillInfo(n->l, i);

	//Per ogni tipo di relazione che ha l entita
	for(int j = 0; j < n->nRep; j++){

		//Se il numero di relazioni entranti di questa entita e maggiore rispetto a quelle che ho controllato fino ad ora, sostituisco le entita salvate con questa
		if(i[n->rep[j].indexType].numRelIn < n->rep[j].nInput){
			i[n->rep[j].indexType].numRelIn = n->rep[j].nInput;
			i[n->rep[j].indexType].nomeEnt[0] = n->nomeEnt;
			i[n->rep[j].indexType].nNomi = 1;
			i[n->rep[j].indexType].tipoRel = typesRel[n->rep[j].indexType];
		}
		//Se il numero e uguale allora aggiungo questa entita a quelle da stampare
		else if(i[n->rep[j].indexType].numRelIn == n->rep[j].nInput){

			//Se il vettore con i nomi delle entita ha finito lo spazio lo rialloco
			if(i[n->rep[j].indexType].nNomi == i[n->rep[j].indexType].lNomi){
				i[n->rep[j].indexType].lNomi += OFFSETINFONOMI;
				i[n->rep[j].indexType].nomeEnt = realloc(i[n->rep[j].indexType].nomeEnt, i[n->rep[j].indexType].lNomi * sizeof(char*));
			}

			i[n->rep[j].indexType].nomeEnt[i[n->rep[j].indexType].nNomi] = n->nomeEnt;
			i[n->rep[j].indexType].nNomi++;
		}
	}

	if(n->r != nilEnt)
		fillInfo(n->r, i);
}

void printReport(){
	

	//alloco vettore ausiliario per la report
	if(nTypes == 0){
		fputs("none\n", stdout);
		return;
	}
	info *infoRep = malloc(nTypes * sizeof(info));
	for(int i=0; i < nTypes; i++){
		infoRep[i].lNomi = OFFSETINFONOMI;
		infoRep[i].nNomi = 0;
		infoRep[i].nomeEnt = malloc(OFFSETINFONOMI * sizeof(char*));
		infoRep[i].numRelIn = 0;
		infoRep[i].tipoRel = NULL;
	}

	fillInfo(radEnt, infoRep);
	quicksort(infoRep, 0, nTypes-1);

	unsigned int isThereRel = 0;

	for(int i=0; i < nTypes; i++){
		//Se il numero di rel in ingresso di quel tipo è zero non stampo niente
		if(infoRep[i].numRelIn != 0){
			fputs("\"", stdout);
			fputs(infoRep[i].tipoRel, stdout);
			fputs("\" ", stdout);

			for(int j = 0; j < infoRep[i].nNomi; j++){
				fputs("\"", stdout);
				fputs(infoRep[i].nomeEnt[j], stdout);
				fputs("\" ", stdout);
			}
			printf("%d; ", infoRep[i].numRelIn);
			isThereRel = 1;
		}
	}

	if(!isThereRel)
		fputs("none", stdout);
	
	fputs("\n", stdout);

	//Dealloco tutto infoRep
	for(int i=0; i < nTypes; i++){
		free(infoRep[i].nomeEnt);
	}
	free(infoRep);

}

void updateReport(nodoEnt d, unsigned int indReport){
	d->rep[indReport].nInput++;
}

void addTypeBetween(nodoRel r, unsigned int indiceTipo){
	
	//Se il vettore dei tipibtwn ha finito lo spazio, alloco altre celle
	if(r->nIndType == r->lIndType){
		r->lIndType += OFFSETTYPE;
		r->indexType = realloc(r->indexType, r->lIndType * sizeof(unsigned int));
	}

	r->indexType[r->nIndType] = indiceTipo;
	r->nIndType++;

}

int searchTypeBetween(nodoRel r, unsigned int indType){

	for(int i = 0; i < r->nIndType; i++){
		if(r->indexType[i] == indType)
			return i;
	}
	return -1;
}

void insertNodeRel(nodoRel *T, char *nameOrig, nodoEnt dest, unsigned int indiceTipo){

	nodoRel y = nilRel, x = *T;

	int risCmp;

	//Scendo fino a quando non trovo una foglia
	while(x != nilRel){
		y = x;
		//Faccio un confronto fra le stringhe
		risCmp = strcmp(nameOrig, x->entOrig);
		//Se la stringa del nodo creato è minore di quella del nodo in cui sono ora vado a sinistra
		if(risCmp < 0)
			x = x->l;
		//Se la stringa del nodo creato è maggiore di quella del nodo in cui sono ora vado a destra
		else if(risCmp >0)
			x = x->r;
		//Se trovo che ce gia una relazione fra le due entita
		else{
			int indiceBtwn = searchTypeBetween(x, indiceTipo);
			//Se ce gia rel di questo tipo non faccio niente
			if(indiceBtwn != -1)
				return;
			//Altrimenti aggiungo nel nodo un indice tipo e aggiorno/creo report
			else{
				addTypeBetween(x, indiceTipo);
				int indReport = searchReport(dest, indiceTipo);
				if(indReport != -1)
					updateReport(dest, indReport);
				else
					addReport(dest, indiceTipo);
				return;
			}
		}
	}

	//Creo nodo
	nodoRel z = createNodoRel(nameOrig, indiceTipo);

	//aggiorno/creo report
	int indReport = searchReport(dest, indiceTipo);
	if(indReport != -1)
		updateReport(dest, indReport);
	else
		addReport(dest, indiceTipo);

	//La foglia che ho trovato diventa il padre di z
	z->p = y;

	//Se l'albero è vuoto il nodo creato diventa la radice
	if(y == nilRel)
		*T = z;

	//Altrimenti se la stringa di z è minore di quella della foglia, z diventa il figlio sinistro
	else if(strcmp(nameOrig, y->entOrig) < 0)
		y->l = z;
	//Altrimenti diventa il figlio destro
	else
		y->r = z;

	//Correggo eventuali proprietà non rispettate
	insertFixupRel(T, z);
}

void addReport(nodoEnt d, unsigned int index){
	
	//Se il vettore dei report ha finito lo spazio, alloco altre celle
	if(d->nRep == d->lRep){
		d->lRep += OFFSETTYPE;
		d->rep = realloc(d->rep, d->lRep * sizeof(report));
	}

	//Aggiungo report
	d->rep[d->nRep].indexType = index;
	d->rep[d->nRep].nInput = 1;
	d->nRep++;
}

void delRel(char *idOrig, char *idDest, char *idType){

	//Cerco i due nodi coinvolti, se non li trovo non faccio niente
	nodoEnt orig = searchEnt(radEnt, idOrig);
	if(orig == nilEnt)
		return;
	nodoEnt dest = searchEnt(radEnt, idDest);
	if(dest == nilEnt)
		return;

	//Cerco se c'e il tipo di relazione, se non c'e non faccio niente
	int indTypesRel = searchType(idType);
	if(indTypesRel == -1){
		return;
	}

	nodoRel relInput = searchRel(dest->relIn, orig->nomeEnt);
	//Se non c'è nessuna relazione fra le due entita non faccio niente
	if(relInput == nilRel)
		return;

	//Se non ce nessuna relazione di quel tipo fra le due entita non faccio niente
	int indiceBtwn = searchTypeBetween(relInput, indTypesRel);
	if(indiceBtwn == -1)
		return;

	//Se c'e solo quel tipo di rel fra queste due entita cancello il nodo relazione altrimenti cancello solo l'index tipo relativo
	//Se c'è relazione di questo tipo e nIndType = 1 vuol dire che c'è solo tipo di relazione, quindi cancello il nodo
	if(relInput->nIndType == 1)
		deleteNodeRel(&(dest->relIn), relInput);
	//Altrimenti cancello la cella e sposto tutte le altre celle a sinistra di 1
	else{	
		for(int i = indiceBtwn; i < relInput->nIndType -1; i++){
			relInput->indexType[i] = relInput->indexType[i+1];
		}
		relInput->nIndType--;
		
		//Se c'è troppo scarto fra le celle usate e quelle allocate ne dealloco qualcuna
		if(relInput->nIndType < relInput->lIndType - OFFSETTYPE && relInput->nIndType > OFFSETTYPE){
			relInput->indexType = realloc(relInput->indexType, (relInput->lIndType - (OFFSETTYPE/2)) * sizeof(unsigned int));
			relInput->lIndType = relInput->lIndType - (OFFSETTYPE/2);
		} 
	}

	//Decrementare il coso del report

	dest->rep[searchReport(dest, indTypesRel)].nInput--;

}

void deleteNodeRel(nodoRel *T, nodoRel relInput){
	
	//Cerco se c'è il nodo da cancellare
	nodoRel z = relInput;
	
	//Se non c'è non faccio nulla
	if(z == nilRel)
		return;

	nodoRel y = z,x;
	char yStartColor = y->col;

	//Se z non ha un figlio sx, traspianto z con suo figlio dx
	if(z->l == nilRel){
		x = z->r;
		trasplantRel(T, z, z->r);
	}
	//Altrimenti se z non ha un figlio dx, traspianto z con suo figlio sx
	else if(z->r == nilRel){
		x = z->l;
		trasplantRel(T, z, z->l);
	}
	//Altrimenti (se z ha due figli)
	else{
		//Y = successore di z
		y = treeMinRel(z->r);
		yStartColor = y->col;
		x = y->r;
		//Se z è il padre di y, y diventa padre di x
		if(y->p == z)
			x->p = y;
		//Altrimenti
		else{
			//Traspianto y con suo figlio destro
			trasplantRel(T, y, y->r);
			y->r = z->r;
			y->r->p = y;
		}
		trasplantRel(T, z, y);
		y->l = z->l;
		y->l->p = y;
		y->col = z->col;
	}

	//Dealloco z
	freeNodoRel(z);

	if(yStartColor == BLACK)
		deleteFixupRel(T, x);
}

void addRel(char *idOrig, char *idDest, char *idType, unsigned int lIdType){

	//Cerco i due nodi coinvolti, se non li trovo non faccio niente
	nodoEnt orig = searchEnt(radEnt, idOrig);
	if(orig == nilEnt)
		return;
	nodoEnt dest = searchEnt(radEnt, idDest);
	if(dest == nilEnt)
		return;

	//Cerco se c'e gia il tipo di relazione, se non c'e lo aggiungo
	int indTypesRel = searchType(idType);
	if(indTypesRel == -1){
		addType(idType, lIdType);
		indTypesRel = nTypes-1;
	}

	//Aggiungo la relazione fra le entita nell albero delle relazioni dell entita destinataria e a seconda del risultato aggiorno il report
	insertNodeRel(&(dest->relIn), orig->nomeEnt, dest, indTypesRel);

}


void leftRotateEnt(nodoEnt *T, nodoEnt x){

	//Sposto sottoalbero sx di y nel sottoalbero dx di x
	nodoEnt y = x->r;
	x->r = y->l;

	//Se y ha un figlio sx, faccio diventare x suo padre
	if(y->l != nilEnt)
		y->l->p = x;

	//Collego y al padre di x
	y->p = x->p;

	//Se x era la radice, y diventa la nuova radice
	if(x->p == nilEnt)
		*T = y;

	//Altrimenti y sostituisce x come figlio destro o sinistro del padre di x
	else if(x == x->p->l)
		x->p->l = y;
	else
		x->p->r = y;

	//Collego x a y
	y->l = x;
	x->p = y;
}

void rightRotateEnt(nodoEnt *T, nodoEnt x){

	//Sposto sottoalbero dx di y nel sottoalbero sx di x
	nodoEnt y = x->l;
	x->l = y->r;

	//Se y ha un figlio dx, faccio diventare x suo padre
	if(y->r != nilEnt)
		y->r->p = x;

	//Collego y al padre di x
	y->p = x->p;

	//Se x era la radice, y diventa la nuova radice
	if(x->p == nilEnt)
		*T = y;

	//Altrimenti y sostituisce x come figlio destro o sinistro del padre di x
	else if(x == x->p->l)
		x->p->l = y;
	else
		x->p->r = y;

	//Collego x a y
	y->r = x;
	x->p = y;
}


void leftRotateRel(nodoRel *T, nodoRel x){

	//Sposto sottoalbero sx di y nel sottoalbero dx di x
	nodoRel y = x->r;
	x->r = y->l;

	//Se y ha un figlio sx, faccio diventare x suo padre
	if(y->l != nilRel)
		y->l->p = x;

	//Collego y al padre di x
	y->p = x->p;

	//Se x era la radice, y diventa la nuova radice
	if(x->p == nilRel)
		*T = y;

	//Altrimenti y sostituisce x come figlio destro o sinistro del padre di x
	else if(x == x->p->l)
		x->p->l = y;
	else
		x->p->r = y;

	//Collego x a y
	y->l = x;
	x->p = y;
}

void rightRotateRel(nodoRel *T, nodoRel x){

	//Sposto sottoalbero dx di y nel sottoalbero sx di x
	nodoRel y = x->l;
	x->l = y->r;

	//Se y ha un figlio dx, faccio diventare x suo padre
	if(y->r != nilRel)
		y->r->p = x;

	//Collego y al padre di x
	y->p = x->p;

	//Se x era la radice, y diventa la nuova radice
	if(x->p == nilRel)
		*T = y;

	//Altrimenti y sostituisce x come figlio destro o sinistro del padre di x
	else if(x == x->p->l)
		x->p->l = y;
	else
		x->p->r = y;

	//Collego x a y
	y->r = x;
	x->p = y;
}

void createNilEnt(){

	//Alloco il nodo
	nilEnt = malloc(sizeof(dimNodoEnt));
	
	//Imposto i valori iniziali del nodo
	nilEnt->nomeEnt = NULL;
	nilEnt->col = BLACK;
	nilEnt->p = NULL;
	nilEnt->l = NULL;
	nilEnt->r = NULL;

	nilEnt->relIn = NULL;

	nilEnt->rep = NULL;
	nilEnt->nRep = 0;
	nilEnt->lRep = 0;
}

void createNilRel(){

	//Alloco il nodo
	nilRel = malloc(sizeof(dimNodoRel));
	
	//Imposto i valori iniziali del nodo
	nilRel->entOrig = NULL;
	nilRel->col = BLACK;
	nilRel->p = NULL;
	nilRel->l = NULL;
	nilRel->r = NULL;
	nilRel->indexType = NULL;
	nilRel->nIndType = 0;
	nilRel->lIndType = 0;
}


nodoRel createNodoRel(char *nameOrig, unsigned int tipo){

	//Creo nodo
	nodoRel n = malloc(sizeof(dimNodoRel));

	//punto alla stringa della entita
	n->entOrig = nameOrig;

	//Imposto i valori iniziali del nodo
	n->col = RED;
	n->p = nilRel;
	n->l = nilRel;
	n->r = nilRel;

	n->lIndType = LTINIZ;
	n->indexType = malloc(n->lIndType * sizeof(unsigned int));
	n->indexType[0] = tipo;
	n->nIndType = 1;


	return n;
}

void insertFixupRel(nodoRel *T, nodoRel x){

	//Fino a quando il padre di x è rosso
	while(x->p->col == RED){
		
		//Se il padre di x è figlio sx
		if(x->p == x->p->p->l){
			//Y = zio di x
			nodoRel y = x->p->p->r;
			//CASO 1
			//Se il colore di y è rosso
			if(y->col == RED){
				//Coloro di nero padre e zio, coloro nonno di rosso e faccio puntare x al nonno
				x->p->col = BLACK;
				y->col = BLACK;
				x->p->p->col = RED;
				x = x->p->p;
			}
			//Altrimenti (y è nero)
			else{
				//CASO 2
				//Se x è figlio dx
				if(x == x->p->r){
					//Eseguo leftrotate su padre di x
					x = x->p;
					leftRotateRel(T, x);
				}
				//CASO 3
				//Coloro padre di nero, nonno di rosso e rightrotate su nonno di x
				x->p->col = BLACK;
				x->p->p->col = RED;
				rightRotateRel(T, x->p->p);
			}
		}

		//Se il padre di x è figlio dx
		else{
			//Y = zio di x
			nodoRel y = x->p->p->l;
			//CASO 1
			//Se il colore di y è rosso
			if(y->col == RED){
				//Coloro di nero padre e zio, coloro nonno di rosso e faccio puntare x al nonno
				x->p->col = BLACK;
				y->col = BLACK;
				x->p->p->col = RED;
				x = x->p->p;
			}
			//Altrimenti (y è nero)
			else{
				//CASO 2
				//Se x è figlio sx
				if(x == x->p->l){
					//Eseguo rightrotate su padre di x
					x = x->p;
					rightRotateRel(T, x);
				}
				//CASO 3
				//Coloro padre di nero, nonno di rosso e leftrotate su nonno di x
				x->p->col = BLACK;
				x->p->p->col = RED;
				leftRotateRel(T, x->p->p);
			}
		}
	}
	//Coloro la radice di nero
	(*T)->col = BLACK;
}

nodoEnt createNodoEnt(char *name, unsigned int l){

	//Creo nodo
	nodoEnt n = malloc(sizeof(dimNodoEnt));

	//Alloco e copio la stringa
	n->nomeEnt = malloc(l * sizeof(char));
	strcpy(n->nomeEnt, name);

	//Imposto i valori iniziali del nodo
	n->col = RED;
	n->p = nilEnt;
	n->l = nilEnt;
	n->r = nilEnt;
	n->relIn = nilRel;

	n->lRep = LTINIZ;
	n->rep = malloc(n->lRep * sizeof(report));
	n->nRep = 0;


	return n;
}

void insertNodeEnt(nodoEnt *T, char *name, unsigned int l){

	nodoEnt y = nilEnt, x = *T;

	int risCmp;

	//Scendo fino a quando non trovo una foglia
	while(x != nilEnt){
		y = x;
		//Faccio un confronto fra le stringhe
		risCmp = strcmp(name, x->nomeEnt);
		//Se la stringa del nodo creato è minore di quella del nodo in cui sono ora vado a sinistra
		if(risCmp < 0)
			x = x->l;
		//Se la stringa del nodo creato è maggiore di quella del nodo in cui sono ora vado a destra
		else if(risCmp >0)
			x = x->r;
		//Se la stringa è già memorizzata non faccio niente
		else
			return;
	}

	//Creo nodo
	nodoEnt z = createNodoEnt(name, l);

	//La foglia che ho trovato diventa il padre di z
	z->p = y;

	//Se l'albero è vuoto il nodo creato diventa la radice
	if(y == nilEnt)
		*T = z;

	//Altrimenti se la stringa di z è minore di quella della foglia, z diventa il figlio sinistro
	else if(strcmp(name, y->nomeEnt) < 0)
		y->l = z;
	//Altrimenti diventa il figlio destro
	else
		y->r = z;

	//Correggo eventuali proprietà non rispettate
	insertFixupEnt(T, z);
}

void insertFixupEnt(nodoEnt *T, nodoEnt x){

	//Fino a quando il padre di x è rosso
	while(x->p->col == RED){
		
		//Se il padre di x è figlio sx
		if(x->p == x->p->p->l){
			//Y = zio di x
			nodoEnt y = x->p->p->r;
			//CASO 1
			//Se il colore di y è rosso
			if(y->col == RED){
				//Coloro di nero padre e zio, coloro nonno di rosso e faccio puntare x al nonno
				x->p->col = BLACK;
				y->col = BLACK;
				x->p->p->col = RED;
				x = x->p->p;
			}
			//Altrimenti (y è nero)
			else{
				//CASO 2
				//Se x è figlio dx
				if(x == x->p->r){
					//Eseguo leftrotate su padre di x
					x = x->p;
					leftRotateEnt(T, x);
				}
				//CASO 3
				//Coloro padre di nero, nonno di rosso e rightrotate su nonno di x
				x->p->col = BLACK;
				x->p->p->col = RED;
				rightRotateEnt(T, x->p->p);
			}
		}

		//Se il padre di x è figlio dx
		else{
			//Y = zio di x
			nodoEnt y = x->p->p->l;
			//CASO 1
			//Se il colore di y è rosso
			if(y->col == RED){
				//Coloro di nero padre e zio, coloro nonno di rosso e faccio puntare x al nonno
				x->p->col = BLACK;
				y->col = BLACK;
				x->p->p->col = RED;
				x = x->p->p;
			}
			//Altrimenti (y è nero)
			else{
				//CASO 2
				//Se x è figlio sx
				if(x == x->p->l){
					//Eseguo rightrotate su padre di x
					x = x->p;
					rightRotateEnt(T, x);
				}
				//CASO 3
				//Coloro padre di nero, nonno di rosso e leftrotate su nonno di x
				x->p->col = BLACK;
				x->p->p->col = RED;
				leftRotateEnt(T, x->p->p);
			}
		}
	}
	//Coloro la radice di nero
	(*T)->col = BLACK;
}

char gestisciComando(char tipo){

	char buffer[LBUFFER];
	char *idOrig, *idDest, *idRel;
	unsigned int lIdOrig, lIdRel,i=0;
	fgets(buffer, LBUFFER, stdin);

	switch(tipo){
		case ADDENT:
			idOrig = strtok(buffer, "\" ");
			while(idOrig[i] != '\0')
				i++;
			lIdOrig = i+1;
			insertNodeEnt(&radEnt, idOrig, lIdOrig);
			break;
		case DELENT:
			idOrig = strtok(buffer, "\" ");

			delent(idOrig);
			break;
		case ADDREL:
			idOrig = strtok(buffer, "\" ");

			idDest = strtok(NULL, "\" ");

			idRel = strtok(NULL, "\" ");
			while(idRel[i] != '\0')
				i++;
			lIdRel = i+1;


			addRel(idOrig, idDest, idRel, lIdRel);
			break;
		case DELREL:
			idOrig = strtok(buffer, "\" ");

			idDest = strtok(NULL, "\" ");

			idRel = strtok(NULL, "\" ");


			delRel(idOrig, idDest, idRel);
			break;
		case REPORT:
			printReport();
			break;
		case END:
			return 1;
			break;
	}

	return 0;
}

nodoEnt searchEnt(nodoEnt T, char *name){

	int risCmp;

	//Scendo fino a quando non trovo una foglia
	while(T != nilEnt){
		//Faccio un confronto fra le stringhe
		risCmp = strcmp(name, T->nomeEnt);

		//Se la stringa del nodo cercato è minore di quella del nodo in cui sono ora vado a sinistra
		if(risCmp < 0)
			T = T->l;
		//Se la stringa del nodo cercato è maggiore di quella del nodo in cui sono ora vado a destra
		else if(risCmp >0)
			T = T->r;
		//Se ho trovato la stringa ritorno il nodo
		else
			return T;
	}

	return nilEnt;
}

void visitaEnt(nodoEnt T){
	if(T->l != nilEnt)
		visitaEnt(T->l);
	fprintf(stdout, "%s\n", T->nomeEnt);
	for(int i = 0; i < T->nRep; i++){
		fprintf(stdout, "%s : %d\n", typesRel[T->rep[i].indexType], T->rep[i].nInput);
	}
	if(T->relIn != nilRel)
		visitaRel(T->relIn);
	fprintf(stdout, "\n");
	if(T->r != nilEnt)
		visitaEnt(T->r);
}

void visitaRel(nodoRel T){
	if(T->l != nilRel)
		visitaRel(T->l);
	for(int i = 0; i < T->nIndType; i++)
		fprintf(stdout, "%s (%s)\n", T->entOrig, typesRel[T->indexType[i]]);
	if(T->r != nilRel)
		visitaRel(T->r);
}

void endLine(){
	while(fgetc(stdin) != '\n');
}

char getComando(){
	
	char com[7];
	fgets(com, 7, stdin);

	//Confronto stringa letta con tutti i comandi e ritorno il tipo di comando
	if(strncmp(com, "addent", 6) == 0){
		return ADDENT;
	}
	else if(strncmp(com, "delent", 6) == 0){
		return DELENT;
	}
	else if(strncmp(com, "addrel", 6) == 0){
		return ADDREL;
	}
	else if(strncmp(com, "delrel", 6) == 0){
		return DELREL;
	}
	else if(strncmp(com, "report", 6) == 0){
		return REPORT;
	}
	else if(strncmp(com, "end", 3) == 0){
		return END;
	}
	return 0;
}

void getStringa(char *buf, unsigned int *l){
	
	char c = ' ';
	unsigned int i = 0;

	while(c == ' ')
		c = fgetc(stdin);

	fscanf(stdin, "%s", buf);

	while(buf[i] != '\"')
		i++;

	buf[i] = '\0';

	*l = i+1;
}

void initType(){
	nTypes = 0;
	lTypes = LTINIZ;

	typesRel = malloc(lTypes * sizeof(char*));
}

void addType(char *idType, unsigned int lIdType){

	//Se il vettore dei tipi ha finito lo spazio, alloco altre celle
	if(nTypes == lTypes){
		lTypes += OFFSETTYPE;
		typesRel = realloc(typesRel, lTypes * sizeof(char*));
	}

	//Alloco la stringa e salvo nuovo tipo relazione
	typesRel[nTypes] = malloc(lIdType * sizeof(char));
	strcpy(typesRel[nTypes], idType);

	nTypes++;
}

int searchType(char *idType){

	//Ntypes = n° elementi, quindi ultima cella occupata è ntypes-1
	for(int i = 0; i < nTypes; i++){
		if(strcmp(typesRel[i], idType) == 0)
			return i;
	}
	return -1;
}

int searchReport(nodoEnt n, unsigned int indType){

	//Nrep = n° elementi, quindi ultima cella occupata è nRep-1
	for(int i = 0; i < n->nRep; i++){
		if(n->rep[i].indexType == indType)
			return i;
	}
	return -1;
}

void quicksort(info *a, int p, int r){
    if(p < r){
    	int q = partition(a, p, r);
    	quicksort(a, p, q-1);
    	quicksort(a, q+1, r);
    }
}

int partition(info *a, int p, int r){

	char *x = a[r].tipoRel;
 	int i = p-1;

 	info b;

 	for(int j=p; j < r; j++){
 		if(x != NULL && a[j].tipoRel != NULL){
	 		if(strcmp(a[j].tipoRel, x) <= 0){
	 			i++;
	 			b = a[i];
	 			a[i] = a[j];
	 			a[j] = b;
	 		}
 		}
 	}
 	b = a[i+1];
 	a[i+1] = a[r];
 	a[r] = b;
 	return i+1;
}

void trasplantRel(nodoRel *T, nodoRel u, nodoRel v){

	//Se u è la radice, la radice diventa v
	if(u->p == nilRel)
		*T = v;
	//Altrimenti se u è figlio sx, metto v come figlio sx del padre di u
	else if(u == u->p->l)
		u->p->l = v;
	//Altrimenti se u è figlio dx, metto v come figlio dx del padre di u
	else
		u->p->r = v;
	//Imposto il padre di u come padre di v
	v->p = u->p;
}

nodoRel treeMinRel(nodoRel n){

	//Scendo verso la foglia più a sinistra e la restituisco
	while(n->l != nilRel)
		n = n->l;
	return n;
}

void freeNodoRel(nodoRel x){
	free(x->indexType);
	free(x);
}

void deleteFixupRel(nodoRel *T, nodoRel x){

	nodoRel w;

	while(x != *T && x->col == BLACK){
		if(x == x->p->l){
			
			w = x->p->r;
			if(w->col == RED){
				w->col = BLACK;
				x->p->col = RED;
				leftRotateRel(T, x->p);
				w = x->p->r;
			}
			if(w->l->col == BLACK && w->r->col == BLACK){
				w->col = RED;
				x = x->p;
			}
			else{
			 	if(w->r->col == BLACK){
			 		w->l->col = BLACK;
			 		w->col = RED;
			 		rightRotateRel(T, w);
			 		w = x->p->r;
				}
				w->col = x->p->col;
				x->p->col = BLACK;
				w->r->col = BLACK;
				leftRotateRel(T, x->p);
				x = *T;
			}
		}

		else{
			
			w = x->p->l;
			if(w->col == RED){
				w->col = BLACK;
				x->p->col = RED;
				rightRotateRel(T, x->p);
				w = x->p->l;
			}
			if(w->r->col == BLACK && w->l->col == BLACK){
				w->col = RED;
				x = x->p;
			}
			else{
			 	if(w->l->col == BLACK){
			 		w->r->col = BLACK;
			 		w->col = RED;
			 		leftRotateRel(T, w);
			 		w = x->p->l;
				}
				w->col = x->p->col;
				x->p->col = BLACK;
				w->l->col = BLACK;
				rightRotateRel(T, x->p);
				x = *T;
			}
		}
	}

	x->col = BLACK;
}

nodoRel searchRel(nodoRel T, char *entOrig){
	int risCmp;

	//Scendo fino a quando non trovo una foglia
	while(T != nilRel){
		//Faccio un confronto fra le stringhe
		risCmp = strcmp(entOrig, T->entOrig);

		//Se la stringa del nodo cercato è minore di quella del nodo in cui sono ora vado a sinistra
		if(risCmp < 0)
			T = T->l;
		//Se la stringa del nodo cercato è maggiore di quella del nodo in cui sono ora vado a destra
		else if(risCmp >0)
			T = T->r;
		//Se ho trovato la stringa ritorno il nodo
		else
			return T;
	}

	return nilRel;
}

void deleteRbRel(nodoRel *h){

	nodoRel z = *h;

	while(z != nilRel){

		nodoRel y = z,x;

		//Se z non ha un figlio sx, traspianto z con suo figlio dx
		if(z->l == nilRel){
			x = z->r;
			trasplantRel(h, z, z->r);
		}
		//Altrimenti se z non ha un figlio dx, traspianto z con suo figlio sx
		else if(z->r == nilRel){
			x = z->l;
			trasplantRel(h, z, z->l);
		}
		//Altrimenti (se z ha due figli)
		else{
			//Y = successore di z
			y = treeMinRel(z->r);
			x = y->r;
			//Se z è il padre di y, y diventa padre di x
			if(y->p == z)
				x->p = y;
			//Altrimenti
			else{
				//Traspianto y con suo figlio destro
				trasplantRel(h, y, y->r);
				y->r = z->r;
				y->r->p = y;
			}
			trasplantRel(h, z, y);
			y->l = z->l;
			y->l->p = y;
			y->col = z->col;
		}
		//Dealloco z
		freeNodoRel(z);
		z = *h;
	}
}



void trasplantEnt(nodoEnt *T, nodoEnt u, nodoEnt v){

	//Se u è la radice, la radice diventa v
	if(u->p == nilEnt)
		*T = v;
	//Altrimenti se u è figlio sx, metto v come figlio sx del padre di u
	else if(u == u->p->l)
		u->p->l = v;
	//Altrimenti se u è figlio dx, metto v come figlio dx del padre di u
	else
		u->p->r = v;
	//Imposto il padre di u come padre di v
	v->p = u->p;
}

nodoEnt treeMinEnt(nodoEnt n){

	//Scendo verso la foglia più a sinistra e la restituisco
	while(n->l != nilEnt)
		n = n->l;
	return n;
}

void deleteNodeEnt(nodoEnt *T, nodoEnt z){
	
	//Se non c'è non faccio nulla
	if(z == nilEnt)
		return;

	nodoEnt y = z,x;
	char yStartColor = y->col;

	//Se z non ha un figlio sx, traspianto z con suo figlio dx
	if(z->l == nilEnt){
		x = z->r;
		trasplantEnt(T, z, z->r);
	}
	//Altrimenti se z non ha un figlio dx, traspianto z con suo figlio sx
	else if(z->r == nilEnt){
		x = z->l;
		trasplantEnt(T, z, z->l);
	}
	//Altrimenti (se z ha due figli)
	else{
		//Y = successore di z
		y = treeMinEnt(z->r);
		yStartColor = y->col;
		x = y->r;
		//Se z è il padre di y, y diventa padre di x
		if(y->p == z)
			x->p = y;
		//Altrimenti
		else{
			//Traspianto y con suo figlio destro
			trasplantEnt(T, y, y->r);
			y->r = z->r;
			y->r->p = y;
		}
		trasplantEnt(T, z, y);
		y->l = z->l;
		y->l->p = y;
		y->col = z->col;
	}

	//Dealloco z
	freeNodoEnt(z);

	if(yStartColor == BLACK)
		deleteFixupEnt(T, x);
}

void freeNodoEnt(nodoEnt x){
	free(x->nomeEnt);
	free(x->rep);
	free(x);
}

void deleteFixupEnt(nodoEnt *T, nodoEnt x){

	nodoEnt w;

	while(x != *T && x->col == BLACK){
		if(x == x->p->l){
			
			w = x->p->r;
			if(w->col == RED){
				w->col = BLACK;
				x->p->col = RED;
				leftRotateEnt(T, x->p);
				w = x->p->r;
			}
			if(w->l->col == BLACK && w->r->col == BLACK){
				w->col = RED;
				x = x->p;
			}
			else{
			 	if(w->r->col == BLACK){
			 		w->l->col = BLACK;
			 		w->col = RED;
			 		rightRotateEnt(T, w);
			 		w = x->p->r;
				}
				w->col = x->p->col;
				x->p->col = BLACK;
				w->r->col = BLACK;
				leftRotateEnt(T, x->p);
				x = *T;
			}
		}

		else{
			
			w = x->p->l;
			if(w->col == RED){
				w->col = BLACK;
				x->p->col = RED;
				rightRotateEnt(T, x->p);
				w = x->p->l;
			}
			if(w->r->col == BLACK && w->l->col == BLACK){
				w->col = RED;
				x = x->p;
			}
			else{
			 	if(w->l->col == BLACK){
			 		w->r->col = BLACK;
			 		w->col = RED;
			 		leftRotateEnt(T, w);
			 		w = x->p->l;
				}
				w->col = x->p->col;
				x->p->col = BLACK;
				w->l->col = BLACK;
				rightRotateEnt(T, x->p);
				x = *T;
			}
		}
	}

	x->col = BLACK;
}
