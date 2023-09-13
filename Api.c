#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define COMMAND_ADD_STATION "aggiungi-stazione"
#define COMMAND_ADD_CAR "aggiungi-auto"
#define COMMAND_DESTROY_STATION  "demolisci-stazione"
#define COMMAND_DESTROY_CAR "rottama-auto"

/**data una lista autostrada i cui nodi sono le stazioni costituite da un intero che indica la distanza dall'inizio dell'autostrada e un array di interi che indica i l'autonomia dei veicoli parcheggiati nella stazione. Per spostarsi tra una stazione e l'altra si può prendere un veicolo a patto che l'autonomia basti per raggiungere la stazione di destinazione o una stazione intermedia.si scriva la funzione calcola percorso in C che prende in ingresso due stazioni e calcola il percorso che implichi il minor numero di veicoli presi per arrivare a destinazione. qualora ci sia più di un percorso si prediliga il percorso con le stazioni più vicine a quella di partenza.**/
typedef struct Tabella{
    int km;
    int costo ;
    int predecessore ;
}Tabella;

typedef struct array_stazione{
    int distanza;
    int macchina;
} array_stazione;




// Definizione della struttura dati per la stazione
typedef struct Stazione* stazione_puntatore;
typedef struct Stazione {
    int distanza;
    int veicoloElettrico[512];
    int numVeicoli;
    stazione_puntatore dx;
    stazione_puntatore sx;
} Stazione;


void miglior_macchina(int v[],int n);
void in_order(stazione_puntatore T);
stazione_puntatore Aggiungi_stazione(stazione_puntatore T,int distanza,int numVeicoli,FILE *file_input);
stazione_puntatore Demolisci_stazione(stazione_puntatore T,int distanza);
stazione_puntatore delete_root(stazione_puntatore T);
stazione_puntatore delete_min(stazione_puntatore P,stazione_puntatore T);
stazione_puntatore Aggiungi_veicolo(stazione_puntatore T,int distanza,int autonomia);
stazione_puntatore Rottama_veicolo(stazione_puntatore T,int distanza,int autonomia);
int countElements(stazione_puntatore T,int Stazione_di_partenza,int Stazione_di_arrivo);
void libera(stazione_puntatore T);
void caricaarray(stazione_puntatore albero,array_stazione *a,int *i,int partenza,int arrivo);
void AddToArray(stazione_puntatore T, array_stazione arr[], int *i,int partenza,int arrivo);
void Calcola_percorso(stazione_puntatore T,int Stazione_di_partenza,int Stazione_di_arrivo);



int main() {



    int distanza,numVeicoli,autonomia,partenza,arrivo,val;
    char comando[20];
    FILE *file_input;
    stazione_puntatore root = NULL;


    //file_input = stdin;
    file_input= fopen("file_ingresso.txt","r");
    val = fscanf(file_input, "%s", comando);

    if(file_input != NULL) {

        while(val != EOF){

            if (strcmp(comando, COMMAND_ADD_STATION) == 0) {
                val = fscanf(file_input, "%d %d", &distanza, &numVeicoli);
                root = Aggiungi_stazione(root, distanza, numVeicoli, file_input);
            }

            if (strcmp(comando, COMMAND_DESTROY_STATION) == 0) {
                val =  fscanf(file_input, "%d", &distanza);
                root = Demolisci_stazione(root, distanza);
            }

            if (strcmp(comando, COMMAND_ADD_CAR) == 0) {
                val = fscanf(file_input, "%d %d", &distanza, &autonomia);

                root = Aggiungi_veicolo(root, distanza, autonomia);
            }

            if (strcmp(comando, COMMAND_DESTROY_CAR) == 0) {
                val = fscanf(file_input, "%d %d", &distanza,&autonomia);

                root = Rottama_veicolo(root, distanza, autonomia);
            }

            if (strcmp(comando, "pianifica-percorso") == 0) {
                val = fscanf(file_input, "%d %d", &partenza, &arrivo);

                Calcola_percorso(root, partenza, arrivo);
            }



            val = fscanf(file_input, "%s", comando);
        }
    }else
    {printf("accesso negato");}

    //in_order(root);
    libera(root);
    fclose(file_input);

    return 0;

}











void miglior_macchina(int v[],int n);





stazione_puntatore Aggiungi_stazione(stazione_puntatore T,int distanza,int numVeicoli,FILE *file_input){

    int autonomia,i,val;

    if(T!=NULL){
        if(distanza>T->distanza){
            T->dx = Aggiungi_stazione(T->dx,distanza,numVeicoli,file_input);
        }else{ if(distanza<T->distanza){
                T->sx= Aggiungi_stazione(T->sx,distanza,numVeicoli,file_input);
            }else{
                printf("non aggiunta\n");
                return T;
            }
        }

    }else{
        T = (stazione_puntatore)malloc(sizeof(struct Stazione));
        for(i=0;i<numVeicoli;i++){
            val = fscanf(file_input, "%d", &autonomia);
            if(val==0){}
            T->veicoloElettrico[i] = autonomia;
        }
        miglior_macchina(T->veicoloElettrico,numVeicoli);
        T->sx = NULL;
        T->dx = NULL;
        T->distanza = distanza;
        T->numVeicoli = numVeicoli;
        printf("aggiunta\n");
        return T;
    }

    return T;

}

stazione_puntatore Aggiungi_veicolo(stazione_puntatore T,int distanza,int autonomia){
    int temp;


    if(T!=NULL){
        if(distanza>T->distanza){
            T->dx = Aggiungi_veicolo(T->dx,distanza,autonomia);
        }else{
            if(distanza<T->distanza){
                T->sx= Aggiungi_veicolo(T->sx,distanza,autonomia);
            }else{

                if(T->numVeicoli == 0){
                    T->veicoloElettrico[0] = autonomia;
                    T->numVeicoli ++;}
                else{
                    if (autonomia < T->veicoloElettrico[0]) {
                        T->veicoloElettrico[T->numVeicoli] = autonomia;
                        T->numVeicoli++;}
                    else{
                        if (autonomia > T->veicoloElettrico[0]) {
                            temp = T->veicoloElettrico[0];
                            T->veicoloElettrico[0] = autonomia;
                            T->veicoloElettrico[T->numVeicoli] = temp;
                            T->numVeicoli++;
                        }

                    }
                }

                printf("aggiunta\n");
                return T;
                }

            }

        }else{
            printf("non aggiunta\n");

        }

    return T;
}



stazione_puntatore Demolisci_stazione(stazione_puntatore T,int distanza){


    if(T==NULL){
        printf("non demolita\n");
        return NULL;
    }
    if(T->distanza>distanza){
        T->sx = Demolisci_stazione(T->sx,distanza);
    }else if(T->distanza< distanza){
        T->dx = Demolisci_stazione(T->dx,distanza);
    }else

        T = delete_root(T);

    return T;
}



stazione_puntatore Rottama_veicolo(stazione_puntatore T,int distanza,int autonomia){
    int i,temp;
    if(T==NULL){
        printf("non rottamata\n");
        return NULL;
    }
    if(T->distanza>distanza){
        T->sx = Rottama_veicolo(T->sx,distanza,autonomia);
    }else if(T->distanza< distanza){
        T->dx = Rottama_veicolo(T->dx,distanza,autonomia);
    }else{
        for(i=0;i<T->numVeicoli;i++){
            if(T->veicoloElettrico[i]==autonomia){
                T->veicoloElettrico[i] = 0;
                temp =  T->veicoloElettrico[T->numVeicoli-1] ;
                T->veicoloElettrico[T->numVeicoli-1] = 0;
                T->veicoloElettrico[i] = temp;
                T->numVeicoli--;
                if(i==0){
                miglior_macchina(T->veicoloElettrico,T->numVeicoli);}

                printf("rottamata\n");
                return T;
            }
        }
        printf("non rottamata\n");
    }


    return T;
}


stazione_puntatore delete_root(stazione_puntatore T){
    if(T==NULL){
        printf("non demolita\n");
        return NULL;
    }

    if(T->dx != NULL && T->sx != NULL){
        stazione_puntatore  min = delete_min(T,T->dx);
        T->distanza = min->distanza;
        for(int i = 0;i<min->numVeicoli;i++){
            T->veicoloElettrico[i] = min->veicoloElettrico[i];}
        T->numVeicoli = min->numVeicoli;
        free(min);
        printf("demolita\n");
        return T;
    }

    stazione_puntatore new_root;
    if(T->sx == NULL){
        new_root= T->dx;
    }else {
        new_root = T->sx;
    }

    printf("demolita\n");
    free(T);


    return new_root;

}



stazione_puntatore delete_min(stazione_puntatore P,stazione_puntatore T){
    if(P==NULL||T==NULL){
        return NULL;
    }
    if(T->sx != NULL){
        return delete_min(T,T->sx);
    }

    if(T==P->sx){
        P->sx = T->dx;
    }else
        P->dx = T->dx;

    return T;
}



void miglior_macchina(int v[],int n) {
int i,temp,miglior_macchina = 0,i_mem =0;

    for (i = 0; i < n ; i++) {
        if(v[i]>miglior_macchina){
            miglior_macchina = v[i];
            i_mem = i;
        }
    }

    temp = v[0];
    v[0] = miglior_macchina;
    v[i_mem] = temp;
}






int countElements(stazione_puntatore T,int Stazione_di_partenza,int Stazione_di_arrivo) {

        int ris =0;
        if ( T !=NULL){
            if ( ( T->distanza ) <= Stazione_di_arrivo && T->distanza >= Stazione_di_partenza ){
            ris ++;
            ris=ris+countElements( T->sx , Stazione_di_partenza,Stazione_di_arrivo )+ countElements( T->dx ,Stazione_di_partenza, Stazione_di_arrivo ) ;}else{
                ris=ris+countElements( T->sx , Stazione_di_partenza,Stazione_di_arrivo )+ countElements( T->dx ,Stazione_di_partenza, Stazione_di_arrivo );

            }
        }

        return ris ;

}

void libera(stazione_puntatore T){
    if(T!=NULL){
        libera(T->sx);
        libera(T->dx);
        free(T);
    }else return;

}
void in_order(stazione_puntatore T){
    if(T!=NULL){
        in_order(T->sx);
        printf("%d |",T->distanza);
        in_order(T->dx);
    }else return;

}

void caricaarray(stazione_puntatore albero,array_stazione *a,int *i,int partenza,int arrivo)
{int temp;

    if(albero != NULL)
    {
        if ( ( albero->distanza ) <= arrivo && albero->distanza >= partenza ){

            temp = (*i)++;
            a[ temp ].distanza = albero->distanza;
            a[ temp ].macchina = albero->veicoloElettrico[0];


         }

        caricaarray(albero->sx,a,i,partenza,arrivo);
        caricaarray(albero->dx,a,i,partenza,arrivo);
    }
}



void AddToArray(stazione_puntatore T, array_stazione arr[], int *i,int partenza,int arrivo){
    if(T == NULL){
        return ;}
    else{

    if ( ( T->distanza ) <= arrivo && T->distanza >= partenza ){
    arr[*i].distanza = T->distanza;
    arr[*i].macchina = T->veicoloElettrico[0];
    ++*i;
    }
    AddToArray(T->sx, arr, i,partenza,arrivo);
    AddToArray(T->dx, arr, i,partenza,arrivo);
    }
}


void Calcola_percorso(stazione_puntatore T,int Stazione_di_partenza,int Stazione_di_arrivo) {
    int n,i=0,j,cont=1,k=0,t;
    bool indietro = false;
    array_stazione *v,temp;
    Tabella *tabella;
    int *percorso_finale;

    if(Stazione_di_partenza==Stazione_di_arrivo){
        printf("%d\n",Stazione_di_partenza);
        return;
    }

    if(Stazione_di_partenza>Stazione_di_arrivo){
        indietro = true;
        t = Stazione_di_partenza;
        Stazione_di_partenza = Stazione_di_arrivo;
        Stazione_di_arrivo = t;

    }

    n = countElements(T,Stazione_di_partenza,Stazione_di_arrivo);

    v = malloc(n*sizeof(array_stazione) );
    tabella = malloc(n*sizeof (Tabella));

    caricaarray(T,v,&i,Stazione_di_partenza,Stazione_di_arrivo);

// bubble sort necessario? me sa di sì

    if(indietro){
        for (i = 0; i < n - 1; i++) {
            for (j = 0; j < n - i - 1; j++) {
                if (v[j].distanza < v[j + 1].distanza) {
                    temp = v[j];
                    v[j] = v[j + 1];
                    v[j + 1] = temp;
                }
            }
        }
    }else {
        for (i = 0; i < n - 1; i++) {
            for (j = 0; j < n - i - 1; j++) {
                if (v[j].distanza > v[j + 1].distanza) {
                    temp = v[j];
                    v[j] = v[j + 1];
                    v[j + 1] = temp;
                }
            }
        }
    }


    for(i=0;i<n;i++){
        tabella[i].km=v[i].distanza;
        tabella[i].predecessore = INT_MAX;
        tabella[i].costo = INT_MAX;

    }
    tabella[0].costo = 0;

    //dijkstra
    if(indietro){
        for(i=0;i<n;i++){
            for(j=i+1;j<n;j++){
                if(v[i].distanza-v[i].macchina<=v[j].distanza){
                    if (tabella[i].costo + 1 <= tabella[j].costo || tabella[i].costo == INT_MAX) {
                        if (tabella[j].predecessore > tabella[i].km) {
                            tabella[j].costo = tabella[i].costo + 1;
                            tabella[j].predecessore = tabella[i].km;
                        }
                    }
                }
            }



        }
    }
    else{
        for(i=0;i<n;i++){
            for(j=i+1;j<n;j++){
                if(v[i].distanza+v[i].macchina>=v[j].distanza){
                    if (tabella[i].costo + 1 <= tabella[j].costo || tabella[i].costo == INT_MAX) {
                        if (tabella[j].predecessore > tabella[i].km) {
                            tabella[j].costo = tabella[i].costo + 1;
                            tabella[j].predecessore = tabella[i].km;
                        }
                    }
                }
            }



        }}






    i = n-1;
    while(i!=0){
        if(tabella[i].costo!=INT_MAX && tabella[i].predecessore != INT_MAX ){
            for(j= i;tabella[j].km != tabella[i].predecessore;j--);
            i = j;
            cont++;
        }else{
            printf("nessun percorso\n");
            free(tabella);
            free(v);
            return;
        }



    }



    percorso_finale = malloc(cont *sizeof(int));

    i = n-1;


    while(i!=0){
        if(tabella[i].costo!=INT_MAX && tabella[i].predecessore != INT_MAX){
            percorso_finale[k] = tabella[i].km;

            k++;
            for(j= i;tabella[i].predecessore!= tabella[j].km;j--);
            i = j;}
    }

    percorso_finale[k] = tabella[i].km;

    for(i=cont-1; i>-1; i--){
        if(i==0){
            printf("%d",percorso_finale[i]);}else{
            printf("%d ",percorso_finale[i]);
        }
    }
    printf("\n");

    free(percorso_finale);
    free(tabella);
    free(v);


}