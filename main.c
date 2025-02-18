#include <stdio.h>
#include<stdlib.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
typedef struct{
   SDL_Surface *bus;
   SDL_Surface* busM;
   SDL_Surface *zoneVide;
   SDL_Surface *vide;
   SDL_Surface *codeRue3;
   SDL_Surface *codeRue12;
   SDL_Surface *codeRue5;
    SDL_Surface *codeRue6;
    SDL_Surface *codeRue7;
    SDL_Surface *codeRue9;
    SDL_Surface *codeRue10;
    SDL_Surface *codeRue15;
    SDL_Surface *zoneNonVideBus;
    SDL_Surface *zoneNonVideBM;
    SDL_Surface *OptionJeu;
    SDL_Surface *OptionCharger;
    SDL_Surface *Time;
    SDL_Surface *SideBar;
    SDL_Surface *BackgroundMap;
    SDL_Surface *Alerte;
    SDL_Surface *Clock;
    SDL_Surface *Person;
    SDL_Surface *Sens;
    SDL_Surface *Horaire;
    SDL_Surface *Horaire1;
    SDL_Surface *Watch;
    SDL_Surface *Alarme;
    SDL_Surface *YellowAlerte;
    SDL_Surface *BgGains;
    SDL_Surface *Bus0;
    SDL_Surface *Bus1;
    SDL_Surface *Bus2;
    SDL_Surface *Bus3;
    SDL_Surface *Bus4;
    SDL_Surface *Bus5;
    SDL_Surface *Bus6;
    SDL_Surface *Bus7;
    SDL_Surface *Bus8;
    SDL_Surface *Bus9;
    SDL_Surface *Bus10;
    SDL_Surface *Bus11;
    SDL_Surface *Bus12;
    SDL_Surface *Bus13;
    SDL_Surface *Bus14;
    SDL_Surface *Bus15;
    SDL_Surface *Bus16;
    SDL_Surface *Bus17;
    SDL_Surface *BusButton;
    SDL_Surface *Money;
    SDL_Surface *BgGainsFlou;
    SDL_Surface *BackgroundBus;
    SDL_Surface *LigneBus;
    SDL_Surface *SensBus;
    SDL_Surface *DistanceParcourue;
    SDL_Surface *NbPassagers;
    SDL_Surface *MontantTickets;
    SDL_Surface *GainNet;
}image;
typedef struct
{
    int minute;
    int heure;
    int jour;
}Horaire;
typedef struct
{
    int row;
    int column;
    int codeRue;
}Subdivision;
typedef struct
{
   int id;
   int idLigne;
   int sens;
   Horaire horaireDateDepart;
}Tache;
typedef struct
{
    int id;
    Horaire horaireDateSouhaite;
    Horaire dureeHeureMinute;
    int typeActivite;
    int idZoneSrc;
    int idZoneDest;
}Activite;
typedef struct destination
{
    int idLigne;
    int idZoneDest;
    struct destination * suivant;
}destination;
typedef struct destination * pileDestinations;
typedef struct
{
    int cin;
    int idZoneHabitat;
    int idZoneTravail;
    int idZoneCourante;
    int intentionVoyage;
    int idLigneCourante;
    int sensCourant;
    int idZoneDest;
    pileDestinations destinationsIntermediaires ;
    int nombreActivites;
    Activite activites[8];
}Personne;
typedef struct alerte{
    Horaire horaireAlerte ;
    int nombrePassagersEnAttente;
    int maxDureeAttente;
    int minDureeAttente;
    int moyenneDureeAttente;
    struct alerte * suivant;
} alerte;
typedef alerte * pileAlertes;
typedef struct
{
  int id;
  int nombreZones;
  int idZones[20];
  pileAlertes pileAlertesSensAller;
  pileAlertes pileAlertesSensRetour;
  int gainNet;
}Ligne;
typedef struct
{
  int idZoneSrc;
  int idZoneDest;
  int distance;
  Horaire dureeAproximative;
  int nombresubdivisions;
  Subdivision parcours[80];
}Troncon;
typedef struct
{
    int zoneSrc;
    int zoneDest;
    int idLigne;
    int sens;
    int idBus;
    Horaire dateVente;
    Horaire horaireEmbarquement ;
    int distance;
    int prix;
}Ticket;
typedef struct emplacement {
    int cinPersonne;
    Ticket tk;
    struct emplacement * suivant;
} emplacement;
typedef struct {
    emplacement * tete;
    emplacement * queue;
} filePassagers;
typedef struct celluleFile{
    filePassagers file;
    int idLigne;
    int sens;
    struct celluleFile * suivant ;
} celluleFile;
typedef celluleFile * listeFilesPassagers;
typedef struct
{
   int id;
   int row;
   int column;
   int nombrePersonnes;
   Personne personnes[1000];
   listeFilesPassagers listeFilesPassagersEnAttente;
}Zone;
typedef struct
{
   int id;
   int idZoneCourante;
   int row;
   int column;
   int enRoute;
   int idLigneCourante;
   int sensCourant;
   int nombreTaches;
   Tache taches[10];
   int nombrePassagers;
   Personne passagers[60];
   int nombreTickets;
   Ticket tickets[1000];
   int distanceparcourue ;
   int coutsEtChargesParJour ;
   int prixLitreCarburant;
   int gainNet;
}Bus;
typedef struct
{
    int rue;
    int zone;
    int bus;
}Casecarte;
typedef struct Liaison
{
    struct Liaison* precedent;
    int idLigne;
    int idZoneLiaisonLigneSuivante;
    struct Liaison* suivant;
}Liaison;
typedef struct
{
    Liaison *premier;
    Liaison *dernier;
}listeLiaisons;
int estVide(listeLiaisons L)
{
    return L.premier== NULL ;
}
void ajouterEnTeteListeLiaisons (listeLiaisons *l, int idLigne, int
                                 idZoneLiaisonAvecSuivant)
{
    Liaison *nv=malloc(sizeof(Liaison));
    nv->idLigne=idLigne;
    nv->idZoneLiaisonLigneSuivante=idZoneLiaisonAvecSuivant;
    nv->precedent = NULL;
    if(estVide(*l))
    {
        nv->suivant=NULL;
        l->dernier=nv;
        l->premier=nv;
    }
    else
    {
        nv->suivant=l->premier;
        l->premier->precedent=nv;
        l->premier=nv;
    }
}
void ajouterEnFinListeLiaisons (listeLiaisons *l, int idLigne, int
                                idZoneLiaisonAvecSuivant)
{
    if(estVide(*l))
        ajouterEnTeteListeLiaisons(l,idLigne,idZoneLiaisonAvecSuivant);
    else
    {
        Liaison *nv=malloc(sizeof(Liaison));
        nv->idLigne=idLigne;
        nv->idZoneLiaisonLigneSuivante=idZoneLiaisonAvecSuivant;
        nv->suivant=NULL;
        l->dernier->suivant=nv;
        nv->precedent=l->dernier;
        l->dernier=nv;
    }
}
void chargerZones(Zone zones[], int *nombreZones)
{
    FILE *fic = fopen("zones.txt", "r");
    if (fic == NULL)
        exit(1);
    int i=-1;
    while (!feof(fic))
    {
        i++;
        fscanf(fic, "%d %d %d", &zones[i].id, &zones[i].row, &zones[i].column);
    }
    *nombreZones=i+1;
    fclose(fic);
}

void chargerLignes(Ligne lignes[], int *nombreLignes)
{
    FILE *fic = fopen("lignes.txt", "r");
    if (fic == NULL)
        exit(1);
    int i=-1,nbZ=0,j;
    while(!feof(fic))
    {
        if(nbZ==0)
        {
            i++;
            fscanf(fic, "%d %d", &lignes[i].id, &lignes[i].nombreZones);
            nbZ=lignes[i].nombreZones;
            j=0;
        }
        else
        {
            fscanf(fic,"%d",&lignes[i].idZones[j]);
            j++;
            nbZ--;
        }
    }
    *nombreLignes=i+1;
    fclose(fic);
}
void chargerTroncons(Troncon troncons[], int *nombreTroncons)
{
    FILE *fic =fopen("troncons.txt", "r");
    if (fic == NULL)
    {
        exit(1);
    }
    int i = -1, j,nbsub=0;
    while (!feof(fic))
    {
        if(nbsub==0)
        {
            i++;
            fscanf(fic,"%d %d %d %d %d %d %d",&troncons[i].idZoneSrc, &troncons[i].idZoneDest, &troncons[i].distance,
                   &troncons[i].dureeAproximative.jour, &troncons[i].dureeAproximative.heure, &troncons[i].dureeAproximative.minute, &troncons[i].nombresubdivisions);
            nbsub=troncons[i].nombresubdivisions;
            j=0;
        }
        else
        {
            fscanf(fic, "%d %d %d", &troncons[i].parcours[j].row, &troncons[i].parcours[j].column, &troncons[i].parcours[j].codeRue);
            j++;
            nbsub--;
        }
    }
    *nombreTroncons=i+1;
    fclose(fic);
}
void chargerFlotteBus(Bus flotteBus[], int *nombreBus)
{
    FILE *fic =fopen("flotteBus.txt", "r");
    if (fic == NULL)
        exit(1);
    int i = -1, j,nbTch=0;
    while (!feof(fic))
    {
        if(nbTch==0)
        {
            i++;
            fscanf(fic,"%d %d %d %d %d %d %d %d",&flotteBus[i].id,&flotteBus[i].idZoneCourante,&flotteBus[i].row
                   ,&flotteBus[i].column,&flotteBus[i].enRoute,&flotteBus[i].idLigneCourante,&flotteBus[i].sensCourant,&flotteBus[i].nombreTaches);
            nbTch=flotteBus[i].nombreTaches;
            j=0;

            flotteBus[i].coutsEtChargesParJour=10000;
            flotteBus[i].prixLitreCarburant=1000;
            flotteBus[i].nombrePassagers=0;
            flotteBus[i].nombreTickets=0;
        }
        else
        {
            fscanf(fic,"%d %d %d %d %d %d",&flotteBus[i].taches[j].id,&flotteBus[i].taches[j].idLigne,&flotteBus[i].taches[j].sens,
                   &flotteBus[i].taches[j].horaireDateDepart.jour,&flotteBus[i].taches[j].horaireDateDepart.heure,&flotteBus[i].taches[j].horaireDateDepart.minute);
            j++;
            nbTch--;
        }
    }
    *nombreBus=i+1;
    fclose(fic);
}
void chargerLiaisons(listeLiaisons *l)
{
    FILE *fic = fopen("liaisons.txt", "r");
    if (fic == NULL)
    {
        exit(1);
    }

    int idLigne, idZoneLiaisonAvecSuivant;
    while (fscanf(fic, "%d %d", &idLigne, &idZoneLiaisonAvecSuivant) != EOF)
    {
        ajouterEnFinListeLiaisons(l, idLigne, idZoneLiaisonAvecSuivant);
    }

    fclose(fic);
}

void afficherZones(Zone zones[], int nombreZones)
{
    for (int i = 0; i < nombreZones; i++)
    {
        printf("%d %d %d\n", zones[i].id, zones[i].row, zones[i].column);
    }
}
void afficherLignes(Ligne lignes[], int nombreLignes)
{
    for (int i = 0; i < nombreLignes; i++)
    {
        printf("%d %d\n", lignes[i].id, lignes[i].nombreZones);
        for(int j=0; j<lignes[i].nombreZones; j++)
        {
            printf("%d\n",lignes[i].idZones[j]);
        }
    }
}
void afficherTroncons(Troncon troncons[], int nombreTroncons)
{
    for(int i=0; i<nombreTroncons; i++)
    {
        printf("%d %d %d %d %d %d %d\n", troncons[i].idZoneSrc, troncons[i].idZoneDest, troncons[i].distance,
               troncons[i].dureeAproximative.jour, troncons[i].dureeAproximative.heure, troncons[i].dureeAproximative
               .minute, troncons[i].nombresubdivisions);
        for (int j = 0; j < troncons[i].nombresubdivisions; j++)
        {
            printf("%d %d %d\n", troncons[i].parcours[j].row, troncons[i].parcours[j].column, troncons[i].parcours[j].codeRue);
        }
    }
}
void afficherFlotteBus(Bus flotteBus[], int nombreBus)
{
    for(int i=0; i<nombreBus; i++)
    {
        printf("%d %d %d %d %d %d %d %d\n",flotteBus[i].id,flotteBus[i].idZoneCourante,flotteBus[i].row
               ,flotteBus[i].column,flotteBus[i].enRoute,flotteBus[i].idLigneCourante,flotteBus[i].sensCourant,flotteBus[i].nombreTaches);
        for(int j=0; j<flotteBus[i].nombreTaches; j++)
            printf("%d %d %d %d %d %d\n",flotteBus[i].taches[j].id,flotteBus[i].taches[j].idLigne,flotteBus[i].taches[j].sens,
                   flotteBus[i].taches[j].horaireDateDepart.jour,flotteBus[i].taches[j].horaireDateDepart.heure,flotteBus[i].taches[j].horaireDateDepart.minute);
    }
}
void sauvegarderZones(Zone zones[], int nombreZones, char* zonesSauv)
{
    FILE *fic = fopen(zonesSauv, "a");
    fprintf(fic,"Les zones: \n");
    if (fic == NULL)
        exit(1);

    for(int i=0;i<nombreZones;i++)
    {
        fprintf(fic,"%d %d %d\n",zones[i].id, zones[i].row, zones[i].column);
    }
    fclose(fic);
}
void sauvegarderLignes(Ligne lignes[],int nombreLignes, char* lignesSauv)
{
    FILE *fic = fopen(lignesSauv, "a");
    fprintf(fic,"Les lignes: \n");
    if (fic == NULL)
        exit(1);
    for (int i = 0; i < nombreLignes; i++)
    {
        fprintf(fic,"%d %d\n",lignes[i].id,lignes[i].nombreZones);
        for(int j=0;j<lignes[i].nombreZones;j++)
        {
            fprintf(fic,"%d\n",lignes[i].idZones[j]);
        }
    }
    fclose(fic);
}
void sauvegarderTroncons(Troncon troncons[], int nombreTroncons, char* tronconsSauv)
{
    FILE *fic = fopen(tronconsSauv, "a");
    fprintf(fic,"Les troncons: \n");
    if (fic == NULL)
        exit(1);
    for(int i=0; i<nombreTroncons; i++){
        fprintf(fic,"%d %d %d %d %d %d %d\n", troncons[i].idZoneSrc, troncons[i].idZoneDest, troncons[i].distance,
        troncons[i].dureeAproximative.jour, troncons[i].dureeAproximative.heure, troncons[i].dureeAproximative
        .minute, troncons[i].nombresubdivisions);
        for (int j = 0; j < troncons[i].nombresubdivisions; j++) {
            fprintf(fic,"%d %d %d\n", troncons[i].parcours[j].row, troncons[i].parcours[j].column, troncons[i].parcours[j].codeRue);
        }
    }
    fclose(fic);
}
void sauvegarderFlotteBus(Bus flotteBus[], int nombreBus, char* flotteBusSauv)
{
    FILE *fic = fopen(flotteBusSauv, "a");
    fprintf(fic,"Les bus: \n");
    if (fic == NULL)
        exit(1);
     for(int i=0; i<nombreBus; i++)
     {
         fprintf(fic,"%d %d %d %d %d %d %d %d\n",flotteBus[i].id,flotteBus[i].idZoneCourante,flotteBus[i].row
                   ,flotteBus[i].column,flotteBus[i].enRoute,flotteBus[i].idLigneCourante,flotteBus[i].sensCourant,flotteBus[i].nombreTaches);
        for(int j=0;j<flotteBus[i].nombreTaches;j++)
            fprintf(fic,"%d %d %d %d %d %d\n",flotteBus[i].taches[j].id,flotteBus[i].taches[j].idLigne,flotteBus[i].taches[j].sens,
                   flotteBus[i].taches[j].horaireDateDepart.jour,flotteBus[i].taches[j].horaireDateDepart.heure,flotteBus[i].taches[j].horaireDateDepart.minute);
     }
     fclose(fic);
}
void sauvegarderLiaisons(listeLiaisons l, char* liaisonsSauv)
{
    FILE *fic = fopen(liaisonsSauv, "a");
    fprintf(fic,"Les liaisons: \n");
    Liaison *current = l.premier;

    while (current != NULL) {
        fprintf(fic, "%d %d\n", current->idLigne, current->idZoneLiaisonLigneSuivante);
        current = current->suivant;
    }
    fclose(fic);
}

void affecterPersonneZone(Zone zones[], Personne pers)
{
    int j;
    j=zones[pers.idZoneHabitat].nombrePersonnes;
    zones[pers.idZoneHabitat].personnes[j]=pers;
    zones[pers.idZoneHabitat].nombrePersonnes++;
}
void genererPersonne(Zone zones[], int nombreZones, Ligne lignes[], int nombreLignes)
{
    srand(time(NULL));
    Personne pers;
    for(int i=0;i<1000;i++)
    {
        pers.cin=i+1;
        pers.idZoneHabitat=rand()%20;
        do
        {
           pers.idZoneTravail=rand()%20;
        }while(pers.idZoneHabitat==pers.idZoneTravail);
        pers.idZoneCourante=pers.idZoneHabitat;
        pers.intentionVoyage=0;
        pers.idLigneCourante=-1;
        pers.sensCourant=-1;
        pers.idZoneDest=-1;
        pers.nombreActivites=3;
        //Activité 0
        pers.activites[0].id=0;
        pers.activites[0].horaireDateSouhaite.jour=0;
        pers.activites[0].horaireDateSouhaite.heure=6+rand()%3;
        pers.activites[0].horaireDateSouhaite.minute=0;
        pers.activites[0].dureeHeureMinute.jour=0;
        pers.activites[0].dureeHeureMinute.heure=8;
        pers.activites[0].dureeHeureMinute.minute=0;
        pers.activites[0].typeActivite=1;
        pers.activites[0].idZoneSrc=pers.idZoneHabitat;
        pers.activites[0].idZoneDest=pers.idZoneTravail;
        //Activite 1
        pers.activites[1].id=1;
        pers.activites[1].horaireDateSouhaite.jour=0;
        pers.activites[1].horaireDateSouhaite.heure=17;
        pers.activites[1].horaireDateSouhaite.minute=0;
        pers.activites[1].dureeHeureMinute.jour=0;
        pers.activites[1].dureeHeureMinute.heure=2;
        pers.activites[1].dureeHeureMinute.minute=0;
        pers.activites[1].typeActivite=2;
        pers.activites[1].idZoneSrc=pers.activites[0].idZoneDest;
        pers.activites[1].idZoneDest=rand()%20;
        while((pers.activites[1].idZoneDest==pers.idZoneTravail)||(pers.activites[1].idZoneDest==pers.idZoneHabitat))
            pers.activites[1].idZoneDest=rand()%20;
        //Activite 2
        pers.activites[2].id=2;
        pers.activites[2].horaireDateSouhaite.jour=0;
        pers.activites[2].horaireDateSouhaite.heure=19;
        pers.activites[2].horaireDateSouhaite.minute=0;
        pers.activites[2].dureeHeureMinute.jour=0;
        pers.activites[2].dureeHeureMinute.heure=1;
        pers.activites[2].dureeHeureMinute.minute=0;
        pers.activites[2].typeActivite=0;
        pers.activites[2].idZoneSrc=pers.activites[1].idZoneDest;
        pers.activites[2].idZoneDest=pers.idZoneHabitat;
        affecterPersonneZone(zones,pers);
    }
}
void afficherPersonnes(Zone zones[], int nombreZones)
{
    for(int i=0; i<nombreZones; i++)
    {
        printf("La zone numéro %d:\n",i);
        for(int j=0; j<zones[i].nombrePersonnes; j++)
        {
            printf("%d %d %d %d %d %d %d %d %d\n",zones[i].personnes[j].cin,zones[i].personnes[j].idZoneHabitat,zones[i].personnes[j].idZoneTravail,
                   zones[i].personnes[j].idZoneCourante,zones[i].personnes[j].intentionVoyage,zones[i].personnes[j].idLigneCourante,zones[i].personnes[j].sensCourant,zones[i].personnes[j].idZoneDest,
                   zones[i].personnes[j].nombreActivites);

            for(int c=0; c<zones[i].personnes[j].nombreActivites; c++)
            {
                Activite o=zones[i].personnes[j].activites[c];
                printf("%d %d %d %d %d %d %d %d %d %d\n",o.id,o.horaireDateSouhaite.jour,o.horaireDateSouhaite.heure,o.horaireDateSouhaite.minute,o.dureeHeureMinute.jour,o.dureeHeureMinute.heure,o.dureeHeureMinute.minute,
                       o.typeActivite,o.idZoneSrc,o.idZoneDest);
            }
        }
    }
}

void sauvegarderPersonnes(Zone zones[], int nombreZones, char* personnes_sauv)
{
    FILE *fic = fopen(personnes_sauv, "a");
    fprintf(fic,"Les personnes: \n");
    if (fic == NULL)
        exit(1);
     for(int i=0;i<nombreZones;i++)
    {
        for(int j=0;j<zones[i].nombrePersonnes;j++)
        {
                    fprintf(fic,"%d %d %d %d %d %d %d %d %d\n",zones[i].personnes[j].cin,zones[i].personnes[j].idZoneHabitat,zones[i].personnes[j].idZoneTravail,
                           zones[i].personnes[j].idZoneCourante,zones[i].personnes[j].intentionVoyage,zones[i].personnes[j].idLigneCourante,zones[i].personnes[j].sensCourant,zones[i].personnes[j].idZoneDest,
                           zones[i].personnes[j].nombreActivites);

                    for(int c=0;c<zones[i].personnes[j].nombreActivites;c++)
                    {
                        Activite o=zones[i].personnes[j].activites[c];
                        fprintf(fic,"%d %d %d %d %d %d %d %d %d %d\n",o.id,o.horaireDateSouhaite.jour,o.horaireDateSouhaite.heure,o.horaireDateSouhaite.minute,o.dureeHeureMinute.jour,o.dureeHeureMinute.heure,o.dureeHeureMinute.minute,
                               o.typeActivite,o.idZoneSrc,o.idZoneDest);
                    }
        }
    }
    fclose(fic);
}
void chargerPersonnes(Zone zones[], int* nombreZones, char* personnes_sauv)
{
    FILE *fic = fopen("personnes_sauv.txt", "r");
    if (fic == NULL)
        exit(1);
    int nbAct=0,k,j=-1;
    Personne pers;
    while(!feof(fic))
    {
        if(nbAct==0)
        {
            j++;
            fscanf(fic,"%d %d %d %d %d %d %d %d %d",&pers.cin,&pers.idZoneHabitat,&pers.idZoneTravail,
                   &pers.idZoneCourante,&pers.intentionVoyage,&pers.idLigneCourante,&pers.sensCourant,&pers.idZoneDest,
                   &pers.nombreActivites);
            nbAct=pers.nombreActivites;
            k=0;
        }
        else
        {
            Activite act=pers.activites[k];
            fscanf(fic,"%d %d %d %d %d %d %d %d %d %d",&act.id,&act.horaireDateSouhaite.jour,&act.horaireDateSouhaite.heure,&act.horaireDateSouhaite.minute,&act.dureeHeureMinute.jour,&act.dureeHeureMinute.heure,&act.dureeHeureMinute.minute,
                   &act.typeActivite,&act.idZoneSrc,&act.idZoneDest);
            k++;
            nbAct--;
        }
    }
    fclose(fic);
}
int calculerDistance(int idZoneSrc, int idZoneDest, Ligne lignes[], int idLigne, Troncon troncons[], int nombreLignes, int nombreTroncons)
{
    int dist = 0;
    int pos1 = -1, pos2 = -1;

    // Trouver les positions des zones source et destination dans la ligne
    for (int j = 0; j < lignes[idLigne].nombreZones; j++)
    {
        if (lignes[idLigne].idZones[j] == idZoneSrc)
            pos1 = j;
        if (lignes[idLigne].idZones[j] == idZoneDest)
            pos2 = j;
    }

    // Si l'une des zones n'a pas été trouvée, renvoyer une distance nulle
    if (pos1 == -1 || pos2 == -1)
        return 0;

    // Calcul de la distance en fonction de l'ordre des zones
    if (pos1 < pos2)
    {
        for (int i = pos1; i < pos2; i++)
        {
            // Vérifier que le tronçon appartient bien à la ligne donnée
            for (int k = 0; k < nombreTroncons; k++)
            {
                if (troncons[k].idZoneSrc == lignes[idLigne].idZones[i] && troncons[k].idZoneDest == lignes[idLigne].idZones[i + 1])
                {
                    dist += troncons[k].distance;
                    break;
                }
            }
        }
    }
    else
    {
        for (int i = pos1; i > pos2; i--)
        {
            // Vérifier que le tronçon appartient bien à la ligne donnée
            for (int k = 0; k < nombreTroncons; k++)
            {
                if (troncons[k].idZoneSrc == lignes[idLigne].idZones[i - 1] && troncons[k].idZoneDest == lignes[idLigne].idZones[i])
                {
                    dist += troncons[k].distance;
                    break;
                }
            }
        }
    }

    return dist;
}

void initialiserCarte(Casecarte carteGeographique[40][40], Zone zones[], Ligne lignes[], Troncon troncons[], Bus
                      flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus)
{
    //initialisation
    for(int i=0; i<40; i++)
    {
        for(int j=0; j<40; j++)
        {
            carteGeographique[i][j].rue=0;
            carteGeographique[i][j].zone=-1;
            carteGeographique[i][j].bus=-1;
        }
    }
    int r,c;
    for(int i=0; i<nombreZones; i++)
    {
        r=zones[i].row;
        c=zones[i].column;
        carteGeographique[r][c].zone=zones[i].id;
    }
    for(int i=0; i<nombreBus; i++)
    {
        r=flotteBus[i].row;
        c=flotteBus[i].column;
        if(carteGeographique[r][c].bus==-1)
            carteGeographique[r][c].bus=flotteBus[i].id;
        else
            carteGeographique[r][c].bus=100;
    }
    for(int i=0; i<nombreTroncons; i++)
    {
        for(int j=0; j<troncons[i].nombresubdivisions; j++)
        {
            r=troncons[i].parcours[j].row;
            c=troncons[i].parcours[j].column;
            carteGeographique[r][c].rue=troncons[i].parcours[j].codeRue;
        }
    }
}
void afficherCarteGeo(image images, SDL_Surface *ecran, Casecarte carteGeographique[40][40], Zone zones[], Bus flotteBus[], int nombreZones,
                      int nombreLignes, int nombreTroncons, int nombreBus, SDL_Color rouge, TTF_Font *font)
{
    SDL_Rect position;
    SDL_Rect positionTexte;
    SDL_Rect positionPers;

    SDL_Color blanc = {0, 0, 0};

    font = TTF_OpenFont("Roboto-Regular.ttf", 11);

    // Appliquer le style bold
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    // Effacement de l'écran
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_Rect positionMap;
    positionMap.x = 0;
    positionMap.y = 0;
    SDL_BlitSurface(images.BackgroundMap, NULL, ecran, &positionMap);

    // Affichage de la carte géographique
    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            position.x = j * 17;
            position.y = i * 17;
            positionTexte.x = j * 17;
            positionTexte.y = i * 17 - 15;

            positionPers.x = j * 17 + 20;
            positionPers.y = i * 17 - 15;

            if ((carteGeographique[i][j].zone != -1) && (carteGeographique[i][j].bus == -1))
            {
                SDL_BlitSurface(images.zoneVide, NULL, ecran, &position);
            }
            else if (( carteGeographique[i][j].zone != -1) && (carteGeographique[i][j].bus != -1))
            {
                if (carteGeographique[i][j].bus < 100)
                    SDL_BlitSurface(images.zoneNonVideBus, NULL, ecran, &position);
                else
                    SDL_BlitSurface(images.zoneNonVideBM, NULL, ecran, &position);

                char texteAffiche[50];
                sprintf(texteAffiche, "zone %d", carteGeographique[i][j].zone);
                SDL_Surface *surfaceTexteZone = TTF_RenderText_Blended(font, texteAffiche, blanc);
                SDL_BlitSurface(surfaceTexteZone, NULL, ecran, &positionTexte);

                SDL_FreeSurface(surfaceTexteZone);
            }
            else if (carteGeographique[i][j].bus != -1)
            {
                if ( carteGeographique[i][j].bus < 100)
                {
                    SDL_BlitSurface(images.bus, NULL, ecran, &position);

                    char texteAffiche[50];
                    sprintf(texteAffiche, "B%-2d", flotteBus[carteGeographique[i][j].bus].id);
                    SDL_Surface *surfaceTexteBus = TTF_RenderText_Blended(font, texteAffiche, blanc);
                    SDL_BlitSurface(surfaceTexteBus, NULL, ecran, &positionTexte);

                    SDL_FreeSurface(surfaceTexteBus);

                    char textePers[50];
                    sprintf(textePers, "%3d", flotteBus[flotteBus[carteGeographique[i][j].bus].id].nombrePassagers);
                    SDL_Surface *surfaceTextePers = TTF_RenderText_Blended(font, textePers, blanc);
                    SDL_BlitSurface(surfaceTextePers, NULL, ecran, &positionPers);

                    SDL_FreeSurface(surfaceTextePers);
                }
                else
                    SDL_BlitSurface(images.busM, NULL, ecran, &position);
            }
            else if (carteGeographique[i][j].rue == 3)
                SDL_BlitSurface(images.codeRue3, NULL, ecran, &position);
            else if (carteGeographique[i][j].rue == 12)
                SDL_BlitSurface(images.codeRue12, NULL, ecran, &position);
            else if (carteGeographique[i][j].rue == 5)
                SDL_BlitSurface(images.codeRue5, NULL, ecran, &position);
            else if (carteGeographique[i][j].rue == 6)
                SDL_BlitSurface(images.codeRue6, NULL, ecran, &position);
            else if (carteGeographique[i][j].rue == 7)
                SDL_BlitSurface(images.codeRue7, NULL, ecran, &position);
            else if (carteGeographique[i][j].rue == 9)
                SDL_BlitSurface(images.codeRue9, NULL, ecran, &position);
            else if (carteGeographique[i][j].rue == 10)
                SDL_BlitSurface(images.codeRue10, NULL, ecran, &position);
            else if (carteGeographique[i][j].rue == 15)
                SDL_BlitSurface(images.codeRue15, NULL, ecran, &position);
        }
    }
}


void ajouterEnFinListeFilesPassagers(listeFilesPassagers *l, filePassagers f, int
                                     idLigne, int sens)
{
    celluleFile* n=(celluleFile*)malloc(sizeof(celluleFile));
    n->file=f;
    n->idLigne=idLigne;
    n->sens=sens;
    n->suivant=NULL;
    if(*l == NULL)
    {
        (*l)=n;
    }
    else
    {
        celluleFile* p=*l;
        while(p->suivant!=NULL)
        {
            p=p->suivant;
        }
        p->suivant=n;
    }
}
void creerListe(listeFilesPassagers* l)
{
    (*l)=NULL;
}
void initialiserListesFilePassagers(Ligne lignes[], int nombreLignes, Zone zones[])
{
    for (int i = 0; i < nombreLignes; i++)
    {
        for (int j = 0; j < lignes[i].nombreZones; j++)
        {
            int c = 0;
            while (zones[c].id != lignes[i].idZones[j])
            {
                c++;
            }

            // Initialisation d'une nouvelle file vide
            filePassagers nouvelleFile = { NULL, NULL };

            // Ajouter deux files : une pour chaque sens (0 et 1)
            ajouterEnFinListeFilesPassagers(&zones[c].listeFilesPassagersEnAttente, nouvelleFile, i, 0);
            ajouterEnFinListeFilesPassagers(&zones[c].listeFilesPassagersEnAttente, nouvelleFile, i, 1);
        }
    }
}


void remplissageComplet(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int*
                        nombreZones, int* nombreLignes, int* nombreTroncons, int* nombreBus,listeLiaisons *liaisons)
{
    chargerZones(zones,&(*nombreZones));
    chargerLignes(lignes,&(*nombreLignes));
    chargerTroncons(troncons,&(*nombreTroncons));
    chargerFlotteBus(flotteBus,&(*nombreBus));
    chargerLiaisons(liaisons);
    //initialisation des listeFilePassagers de chaque zone pour toutes les lignes
    for(int i=0;i<*nombreZones;i++){
        creerListe(&zones[i].listeFilesPassagersEnAttente);
    }
    initialiserListesFilePassagers(lignes,*nombreLignes,zones);
}
void affichageComplet(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int
                      nombreZones, int nombreLignes, int nombreTroncons, int nombreBus)
{
    printf("Les zones:\n");
    afficherZones(zones,nombreZones);
    printf("Les lignes:\n");
    afficherLignes(lignes,nombreLignes);
    printf("Les troncons:\n");
    afficherTroncons(troncons,nombreTroncons);
    printf("Les bus:\n");
    afficherFlotteBus(flotteBus,nombreBus);
    printf("Les personnes:\n");
    afficherPersonnes(zones,nombreZones);
}
void sauvegardeComplete(Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int
                        nombreZones, int nombreLignes, int nombreTroncons, int nombreBus, listeLiaisons l, char*
                        nomFichier)
{
    sauvegarderZones(zones,nombreZones,"zonesSauv.txt");
    sauvegarderLignes(lignes,nombreLignes,"lignesSauv.txt");
    sauvegarderTroncons(troncons,nombreTroncons,"tronconsSauv.txt");
    sauvegarderFlotteBus(flotteBus,nombreBus,"flotteBusSauv.txt");
    sauvegarderPersonnes(zones,nombreZones,"personnes_sauv.txt");
    sauvegarderLiaisons(l,"liaisonsSauv.txt");
}
int numTacheSuivante(Horaire horaireCourant, int idBusCourant, Bus flotteBus[])
{
    int i=0;
    int num;
    while(i<flotteBus[idBusCourant].nombreTaches && ((flotteBus[idBusCourant].taches[i].horaireDateDepart.jour!=horaireCourant.jour)||(flotteBus[idBusCourant].taches[i].horaireDateDepart.heure!=horaireCourant.heure)
            ||(flotteBus[idBusCourant].taches[i].horaireDateDepart.minute!=horaireCourant.minute)))
    {
        i++;
    }
    if(i>=flotteBus[idBusCourant].nombreTaches)
        num=-1;
    else
        num=flotteBus[idBusCourant].taches[i].id;
    return num;
}
void deplacerBus(Casecarte carteGeographique[40][40], int idBusCourant, Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus, Horaire horaireCourant)
{
    int i = 0, j = 0, dest, a = 0, r, c, r1, c1;
    //Spécifier la ligne
    if(idBusCourant>=0 && idBusCourant<=5)
        i=0;
    else if(idBusCourant>=6 && idBusCourant<=11)
        i=1;
    else
        i=2;
    // Trouver l'indice de la zone actuelle du bus dans la ligne
    while (flotteBus[idBusCourant].idZoneCourante != lignes[i].idZones[j])
    {
        j++;
    }
    // Déterminer la destination du bus sur la ligne
    if (flotteBus[idBusCourant].sensCourant == 0)
        dest = j + 1;
    else
        dest = j - 1;

    //printf("dest =%d\n",dest);

    int k = 0;
    // Trouver le tronçon correspondant à la prochaine étape du bus
    if (flotteBus[idBusCourant].sensCourant == 0)
    {
        while ((k < nombreTroncons) && ((flotteBus[idBusCourant].idZoneCourante != troncons[k].idZoneSrc) || (lignes[i].idZones[dest] != troncons[k].idZoneDest)))
        {
            k++;
        }
    }
    else
    {
        while ((k < nombreTroncons) && ((flotteBus[idBusCourant].idZoneCourante != troncons[k].idZoneDest) || (lignes[i].idZones[dest] != troncons[k].idZoneSrc)))
            k++;
    }
    // Trouver la position actuelle du bus sur le tronçon
    while ((a < troncons[k].nombresubdivisions) && ((troncons[k].parcours[a].row != flotteBus[idBusCourant].row) || (troncons[k].parcours[a].column != flotteBus[idBusCourant].column)))
    {
        a++;
    }
    // Déplacer le bus selon le sens courant
        //Le bus se trouve dans la 1ere subdivision dans le sens 0
    if ((flotteBus[idBusCourant].sensCourant == 0) && (flotteBus[idBusCourant].row == zones[troncons[k].idZoneSrc].row)
        && (flotteBus[idBusCourant].column == zones[troncons[k].idZoneSrc].column)
        &&(a==troncons[k].nombresubdivisions))
        {
        r = troncons[k].parcours[0].row;
        c = troncons[k].parcours[0].column;
        carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        //probleme en k
        if(k==0 || k==5 || k==13)
        {
            int nb=0,pos;
            for(int b=0;b<nombreBus;b++)
            {
                if((flotteBus[b].row==zones[troncons[k].idZoneSrc].row)&&(flotteBus[b].column==zones[troncons[k].idZoneSrc].column))
                {
                    nb++;
                    pos=b;
                }
            }
            if(nb==0)
                carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = -1;
            else if(nb==1)
                carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = flotteBus[pos].id;
            else
                carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = 100;
        }
        else
        {
            carteGeographique[zones[troncons[k].idZoneSrc].row][zones[troncons[k].idZoneSrc].column].bus = -1;
        }
    }
    //Le bus se trouve dans la 1ere subdivison du zone dans le sens 1
    else if ((flotteBus[idBusCourant].sensCourant == 1) && (flotteBus[idBusCourant].row == zones[troncons[k].idZoneDest].row)
             && (flotteBus[idBusCourant].column == zones[troncons[k].idZoneDest].column)
             &&(a==troncons[k].nombresubdivisions))
    {
        r = troncons[k].parcours[troncons[k].nombresubdivisions - 1].row;
        c = troncons[k].parcours[troncons[k].nombresubdivisions - 1].column;
        carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        if(k==4 || k==12 || k==18 )
        {
           int nb1=0,pos;
            for(int b=0;b<nombreBus;b++)
            {
                if((flotteBus[b].row==zones[troncons[k].idZoneDest].row)&&(flotteBus[b].column==zones[troncons[k].idZoneDest].column))
                {
                    nb1++;
                    pos=b;
                }
            }
            if(nb1==0)
                carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = -1;
            else if(nb1==1)
                carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = flotteBus[pos].id;
            else
            {
                carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = 100;
            }
        }
        else
            carteGeographique[zones[troncons[k].idZoneDest].row][zones[troncons[k].idZoneDest].column].bus = -1;
    }
    //Le bus se trouve dans la derniére subdivision du troncons dans le sens 0

    else if ((a == troncons[k].nombresubdivisions -1 ) && (flotteBus[idBusCourant].sensCourant == 0))
    {
        r = zones[troncons[k].idZoneDest].row;
        c = zones[troncons[k].idZoneDest].column;
        if(carteGeographique[r][c].bus==-1)
            carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        else
            carteGeographique[r][c].bus = 100;

        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        r1 = troncons[k].parcours[a].row;
        c1 = troncons[k].parcours[a].column;
        carteGeographique[r1][c1].bus = -1;
        //terminus
        if (troncons[k].idZoneDest == lignes[i].idZones[lignes[i].nombreZones-1])
        {
            flotteBus[idBusCourant].enRoute = 0;
        }
        flotteBus[idBusCourant].idZoneCourante=troncons[k].idZoneDest;
    }
    //Le bus se trouve dans la derniére subdivision du troncons dans le sens 1
    else if ((a == 0) && (flotteBus[idBusCourant].sensCourant == 1))
    {
        r = zones[troncons[k].idZoneSrc].row;
        c = zones[troncons[k].idZoneSrc].column;
        if(carteGeographique[r][c].bus==-1)
            carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
        else
            carteGeographique[r][c].bus = 100;

        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        r1 = troncons[k].parcours[a].row;
        c1 = troncons[k].parcours[a].column;
        carteGeographique[r1][c1].bus = -1;
        //terminus
        if (troncons[k].idZoneSrc == lignes[i].idZones[0])
        {
            flotteBus[idBusCourant].enRoute = 0;
        }
        flotteBus[idBusCourant].idZoneCourante=troncons[k].idZoneSrc;
    }
    else
    {
        if (flotteBus[idBusCourant].sensCourant == 0)
        {
            r = troncons[k].parcours[a + 1].row;
            c = troncons[k].parcours[a + 1].column;
            if(carteGeographique[r][c].bus ==-1)
                carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
            else
                carteGeographique[r][c].bus=100;
        }
        else if(flotteBus[idBusCourant].sensCourant == 1)
        {
            r = troncons[k].parcours[a - 1].row;
            c = troncons[k].parcours[a - 1].column;
            if(carteGeographique[r][c].bus ==-1)
                carteGeographique[r][c].bus = flotteBus[idBusCourant].id;
            else
                carteGeographique[r][c].bus=100;
        }
        flotteBus[idBusCourant].row = r;
        flotteBus[idBusCourant].column = c;
        r1 = troncons[k].parcours[a].row;
        c1 = troncons[k].parcours[a].column;
        carteGeographique[r1][c1].bus = -1;
    }
    flotteBus[idBusCourant].distanceparcourue++;
}

int numActiviteSuivante(Horaire horaireCourant,int idPersonne,int idZone,Zone zones[])
{
    int i=0;
    int num;
    int z=zones[idZone].personnes[idPersonne].idZoneCourante;
    int nbAct=zones[z].personnes[idPersonne].nombreActivites;
    while(i<nbAct)
    {
        if(zones[z].personnes[idPersonne].activites[i].horaireDateSouhaite.jour!=horaireCourant.jour || zones[z].personnes[idPersonne].activites[i].horaireDateSouhaite.heure!=horaireCourant.heure
                ||zones[z].personnes[idPersonne].activites[i].horaireDateSouhaite.minute!=horaireCourant.minute)
            i++;
        else
            break;
    }
    if(i>=zones[z].personnes[idPersonne].nombreActivites)
        num=-1;
    else
        num=zones[z].personnes[idPersonne].activites[i].id;
    return num;
}
void deplacerPersonne(Personne src[], Personne dest[], int indicePersonneSrc, int*
nombrePersonnesSrc, int* nombrePersonnesDest)
{
    //copier la personne dans  le tableau destination
    dest[*nombrePersonnesDest]=src[indicePersonneSrc];
    (*nombrePersonnesDest)++;
    //supprimer la personne du tableau src
    for(int i=indicePersonneSrc;i<*nombrePersonnesSrc;i++)
        src[i]=src[i+1];
    (*nombrePersonnesSrc)--;
}
void empilerPileDestinations(pileDestinations* p, int idLigne, int idZoneDest)
{
    destination *n=(destination*)malloc(sizeof(destination));
    n->idLigne=idLigne;
    n->idZoneDest=idZoneDest;
    n->suivant=*p;
    *p=n;
}
void depilerPileDestinations(pileDestinations* p)
{
    if(*p != NULL){
        destination *n=*p;
        *p=n->suivant;
        free(n);
    }
}
void rechercherLignes(int idZoneSrcInitiale, int idZoneDestFinale, int*
idLigneZoneSrc, int* idLigneZoneDest, Ligne lignes[] , int nombreLignes)
{
    //Chercher ligne src
    int found=0,i=0;
    while(i<nombreLignes && !found){
        for(int j=0;j<lignes[i].nombreZones;j++){
            if(idZoneSrcInitiale==lignes[i].idZones[j]){
                found=1;
                (*idLigneZoneSrc)=i;
                break;
            }
        }
        i++;
    }
    //chercher ligne dest
    found=0,i=0;
     while(i<nombreLignes && !found){
        for(int j=0;j<lignes[i].nombreZones;j++){
            if(idZoneDestFinale==lignes[i].idZones[j]){
                found=1;
                (*idLigneZoneDest)=i;
                break;
            }
        }
        i++;
    }
}
int rechercherSens(int idZoneSrcIntermediaire, int idZoneDestIntermediaire, int
idLigne, Ligne lignes[])
{
    int pos1,pos2;
    for(int j=0;j<lignes[idLigne].nombreZones;j++)
    {
        if (lignes[idLigne].idZones[j]==idZoneSrcIntermediaire)
            pos1=j;
        else if(lignes[idLigne].idZones[j]==idZoneDestIntermediaire)
            pos2=j;
    }

    if(pos1>pos2)
        return 1;
    else
        return 0;
}
void remplirPileDestinations (pileDestinations* p, listeLiaisons l, int
idZoneSrcInitiale, int idZoneDestFinale ,Ligne lignes[] , int nombreLignes,Personne pers)
{
    (*p) = NULL;
    int idLigneSrc,idLigneDest,foundZoneSrc=0,founZoneDest=0;
    Liaison* pt2;
    rechercherLignes(idZoneSrcInitiale,idZoneDestFinale,&idLigneSrc,&idLigneDest,lignes,nombreLignes);
    //ajouter la zone destination au fin du pile
    empilerPileDestinations(p,idLigneDest,idZoneDestFinale);

    //si la meme ligne en empile une seule fois
    if(idLigneSrc == idLigneDest)
    {
        return ;
    }
    //chercher ou est le source
    Liaison* pt1=l.premier;
    while(foundZoneSrc==0 && founZoneDest==0 && pt1!=NULL)
    {
        if(pt1->idLigne==idLigneSrc){
            foundZoneSrc=1;
        }
        else if(pt1->idLigne==idLigneDest){
            founZoneDest=1;
        }
        else
            pt1=pt1->suivant;
    }

    //cas de precedent(src avant dest)
    if(foundZoneSrc==1)
    {
        int found=0;
        Liaison* pt=l.premier;
        while(pt!=NULL && found==0){
            if(pt->idLigne==idLigneDest)
            {
                found=1;
            }
            else
                pt=pt->suivant;
        }

        //prochaine zoneIntermédiaire
         pt2=pt;
         while(pt2!=NULL && pt2->idLigne!=idLigneSrc){
            pt2=pt2->precedent;
            if(idZoneDestFinale==pt2->idZoneLiaisonLigneSuivante){
                depilerPileDestinations(p);

            }
            empilerPileDestinations(p,pt2->idLigne,pt2->idZoneLiaisonLigneSuivante);
        }
    }
    //cas de suivant(dest avant src)
    else
    {
        //prochaine zoneIntermédiaire
         pt2=pt1;
         while(pt2!=NULL && pt2->idLigne!=idLigneSrc){
            pt2=pt2->suivant;
            if(idZoneDestFinale==pt2->precedent->idZoneLiaisonLigneSuivante){
                depilerPileDestinations(p);
            }
            empilerPileDestinations(p,pt2->idLigne,pt2->precedent->idZoneLiaisonLigneSuivante);
        }
    }
    //Eviter les doublants
    if(*p!=NULL && idZoneSrcInitiale==(*p)->idZoneDest)
    {
            depilerPileDestinations(p);
    }
}

void prochaineDestinationIntermediaire(pileDestinations* p,int* idLigne, int*
idZoneDest)
{
    if(*p!=NULL)
    {
          *idLigne=(*p)->idLigne;
          *idZoneDest=(*p)->idZoneDest;
    }
}
int vide_file(filePassagers f){
    return (f.tete == NULL) && (f.queue == NULL);
}
void enfilerFilePassagers(filePassagers * f , int cin,Ticket t){
    emplacement * n;
    n = (emplacement*)malloc(sizeof(emplacement));
    n->cinPersonne= cin;
    n->tk=t;
    n->suivant = NULL;
    if(vide_file(*f))
        f->tete = n;
    else
        f->queue->suivant = n;
    f->queue = n;
}
void defilerFilePassagers (filePassagers* f){
    emplacement* n;
    n = f->tete;
    if(n->suivant == NULL){
        f->tete = NULL;
        f->queue = NULL;
        free(n);
    }
    else{
        f->tete = n->suivant;
        free(n);
    }
}

void empilerPileAlertes(pileAlertes* p, Horaire horaireAlerte,  int nombrePassagersEnAttente,
int maxDureeAttente,int minDureeAttente, int moyenneDureeAttente)
{
    alerte *n=(alerte*)malloc(sizeof(alerte));
    n->horaireAlerte=horaireAlerte;
    n->nombrePassagersEnAttente=nombrePassagersEnAttente;
    n->maxDureeAttente=maxDureeAttente;
    n->minDureeAttente=minDureeAttente;
    n->moyenneDureeAttente=moyenneDureeAttente;

    n->suivant=*p;
    *p=n;
}
void depilerPileAlertes (pileAlertes* p)
{
    if(*p != NULL){
        alerte *n=*p;
        *p=n->suivant;
        free(n);
    }
}
void inverserPileAlertes(pileAlertes* p)
{
    pileAlertes tempPile = NULL;
    while (*p != NULL) {
        alerte *n = *p;
        *p = n->suivant;

        n->suivant = tempPile;
        tempPile = n;
    }
    *p = tempPile;
}
void afficherPileAlerte(pileAlertes* p)
{
    alerte* current = *p;
    while (current != NULL) {
        printf("| %d:%2d:%2d               | %24d | %15d | %15d | %19d |\n",
               current->horaireAlerte.jour,
               current->horaireAlerte.heure,
               current->horaireAlerte.minute,

               current->nombrePassagersEnAttente,
               current->maxDureeAttente,
               current->minDureeAttente,
               current->moyenneDureeAttente);
        printf("--------------------------------------------------------------------------------------------------------------\n");

        current = current->suivant;
    }
}
void afficherPileAlerteFic(FILE *fic, pileAlertes* p)
{
    alerte* current = *p;
    while (current != NULL) {
        fprintf(fic, "| %d:%2d:%2d               | %24d | %15d | %15d | %19d |\n",
                current->horaireAlerte.jour,
                current->horaireAlerte.heure,
                current->horaireAlerte.minute,

                current->nombrePassagersEnAttente,
                current->maxDureeAttente,
                current->minDureeAttente,
                current->moyenneDureeAttente);
        fprintf(fic, "--------------------------------------------------------------------------------------------------------------\n");

        current = current->suivant;
    }
}

void creerPileAlerte(pileAlertes *p1,pileAlertes *p2)
{
    (*p1)=NULL;
    (*p2)=NULL;
}
void initialiserPileAlerte(Ligne lignes[],int nombreLignes)
{
    for(int i=0;i<nombreLignes;i++){
        creerPileAlerte(&lignes[i].pileAlertesSensAller,&lignes[i].pileAlertesSensRetour);
    }
}
void nouvelleAlerte(Horaire horaireAlerte, int dureeAttenteEnMinutes, int idLigne, int
sens, Ligne lignes[])
{
    if(sens==0){
        empilerPileAlertes(&lignes[idLigne].pileAlertesSensAller,horaireAlerte,1,dureeAttenteEnMinutes,dureeAttenteEnMinutes,dureeAttenteEnMinutes);
    }
    else{
        empilerPileAlertes(&lignes[idLigne].pileAlertesSensRetour,horaireAlerte,1,dureeAttenteEnMinutes,dureeAttenteEnMinutes,dureeAttenteEnMinutes);
    }
}
void rechercherLonguesAttentesFilePassager(filePassagers f, int dureeAttenteMaxEnMinutes, int idLigne, int sens, Ligne lignes[], Horaire horaireCourant) {
    emplacement* current = f.tete;
    while (current != NULL) {
        int DureeAttente = (horaireCourant.jour * 900 + horaireCourant.heure * 60 + horaireCourant.minute) -
                           (current->tk.dateVente.jour * 900 + current->tk.dateVente.heure * 60 + current->tk.dateVente.minute);


        if (DureeAttente > dureeAttenteMaxEnMinutes) {
            if (/*(lignes[idLigne].pileAlertesSensAller == NULL && lignes[idLigne].pileAlertesSensRetour == NULL) ||*/
                (sens == 0 && (lignes[idLigne].pileAlertesSensAller == NULL ||
                               (lignes[idLigne].pileAlertesSensAller->horaireAlerte.jour != horaireCourant.jour ||
                                lignes[idLigne].pileAlertesSensAller->horaireAlerte.heure != horaireCourant.heure ||
                                lignes[idLigne].pileAlertesSensAller->horaireAlerte.minute != horaireCourant.minute))) ||
                (sens == 1 && (lignes[idLigne].pileAlertesSensRetour == NULL ||
                               (lignes[idLigne].pileAlertesSensRetour->horaireAlerte.jour != horaireCourant.jour ||
                                lignes[idLigne].pileAlertesSensRetour->horaireAlerte.heure != horaireCourant.heure ||
                                lignes[idLigne].pileAlertesSensRetour->horaireAlerte.minute != horaireCourant.minute)))) {
                nouvelleAlerte(horaireCourant, DureeAttente, idLigne, sens, lignes);
            } else {
                if (sens == 0) {
                    if (DureeAttente > lignes[idLigne].pileAlertesSensAller->maxDureeAttente) {
                        lignes[idLigne].pileAlertesSensAller->maxDureeAttente = DureeAttente;
                    }
                    if (DureeAttente < lignes[idLigne].pileAlertesSensAller->minDureeAttente) {
                        lignes[idLigne].pileAlertesSensAller->minDureeAttente = DureeAttente;
                    }
                    lignes[idLigne].pileAlertesSensAller->moyenneDureeAttente =
                        (lignes[idLigne].pileAlertesSensAller->moyenneDureeAttente * lignes[idLigne].pileAlertesSensAller->nombrePassagersEnAttente + DureeAttente) /
                        (lignes[idLigne].pileAlertesSensAller->nombrePassagersEnAttente + 1);
                    lignes[idLigne].pileAlertesSensAller->nombrePassagersEnAttente++;
                } else {
                    if (DureeAttente > lignes[idLigne].pileAlertesSensRetour->maxDureeAttente) {
                        lignes[idLigne].pileAlertesSensRetour->maxDureeAttente = DureeAttente;
                    }
                    if (DureeAttente < lignes[idLigne].pileAlertesSensRetour->minDureeAttente) {
                        lignes[idLigne].pileAlertesSensRetour->minDureeAttente = DureeAttente;
                    }
                    lignes[idLigne].pileAlertesSensRetour->moyenneDureeAttente =
                        (lignes[idLigne].pileAlertesSensRetour->moyenneDureeAttente * lignes[idLigne].pileAlertesSensRetour->nombrePassagersEnAttente + DureeAttente) /
                        (lignes[idLigne].pileAlertesSensRetour->nombrePassagersEnAttente + 1);
                    lignes[idLigne].pileAlertesSensRetour->nombrePassagersEnAttente++;
                }
            }
        }
        current = current->suivant;
    }
}
void rechercherLonguesAttentesToutesZones(Zone zones[], int nombreZones, Ligne
lignes[], Horaire horaireCourant, int dureeAttenteMaxEnMinutes)
{
    for(int i=0;i<nombreZones;i++){
        celluleFile *current=zones[i].listeFilesPassagersEnAttente;
        while(current!=NULL){
            rechercherLonguesAttentesFilePassager(current->file,dureeAttenteMaxEnMinutes,current->idLigne,current->sens,lignes,horaireCourant);
            current=current->suivant;
        }
    }
}
void afficherAlerte(image images, Ligne lignes[], int nombreLignes, Horaire horaireCourant, TTF_Font *font, SDL_Color couleurTexte, SDL_Surface *ecran) {
    font = TTF_OpenFont("Roboto-Regular.ttf", 15);
    TTF_Init();
    TTF_Font* font1 = TTF_OpenFont("Roboto-Regular.ttf", 15);
    // Appliquer le style bold
    TTF_SetFontStyle(font1, TTF_STYLE_BOLD);

    SDL_Rect rectEffacer;
    rectEffacer.x = 870;       // Position X de la zone à effacer
    rectEffacer.y = 0;         // Position Y de la zone à effacer
    rectEffacer.w = 490;       // Largeur de la zone à effacer
    rectEffacer.h = ecran->h;  // Hauteur de l'écran

    SDL_FillRect(ecran, &rectEffacer, SDL_MapRGB(ecran->format, 255, 255, 255)); // Remplir la zone spécifiée avec du blanc

    //Affichage du sideBar
    SDL_Rect positionSideBar;
    positionSideBar.x = 840;
    positionSideBar.y = 0;
    SDL_BlitSurface(images.SideBar, NULL, ecran, &positionSideBar);

    //Affichage du bouton alarme
    SDL_Rect positionAlarme;
    positionAlarme.x = 870;
    positionAlarme.y = 610;
    SDL_BlitSurface(images.Alarme, NULL, ecran, &positionAlarme);

    //Affichage du bouton YellowAlerte
    SDL_Rect positionYellowAlerte;
    positionYellowAlerte.x = 1260;
    positionYellowAlerte.y = 610;
    SDL_BlitSurface(images.YellowAlerte, NULL, ecran, &positionYellowAlerte);

    //Affichage du bouton Alerte rouge
    SDL_Rect positionAlerte;
    positionAlerte.x = 930;
    positionAlerte.y = 20;
    SDL_BlitSurface(images.Alerte, NULL, ecran, &positionAlerte);

    // Afficher l'horaire courant
    char texteAffiche[50];
    //sprintf(texteAffiche, "Horaire courant %d: %02d: %02d", horaireCourant.jour, horaireCourant.heure, horaireCourant.minute);
    sprintf(texteAffiche, "Saturation des lignes");
    SDL_Rect positionTexteHoraire = {1030, 180, 0, 0};
    SDL_Surface *surfaceTexteHoraire = TTF_RenderText_Blended(font1, texteAffiche, couleurTexte);
    SDL_BlitSurface(surfaceTexteHoraire, NULL, ecran, &positionTexteHoraire);
    SDL_FreeSurface(surfaceTexteHoraire);
    TTF_CloseFont(font1);


     //Affichage du watch
    SDL_Rect positionWatch;
    positionWatch.x = 960;
    positionWatch.y = 160;
    SDL_BlitSurface(images.Watch, NULL, ecran, &positionWatch);

    //Affichage du watch
    positionWatch.x = 1210;
    positionWatch.y = 160;
    SDL_BlitSurface(images.Watch, NULL, ecran, &positionWatch);

    SDL_Surface *tabImages[4];
    tabImages[0] = images.Horaire;
    tabImages[1] = images.Horaire;
    tabImages[2] = images.Horaire;
    tabImages[3] = images.Horaire;

    // Afficher les en-têtes (Passagers, MaxDurée, etc.)
    const char *headers[] = {"Passagers", "MaxDurée", "MinDurée", "MoyenneDurée"};
    int xOffsets[] = {950, 1050, 1150, 1250};
    for (int i = 0; i < 4; i++) {
        //Affichage du séparateur
        SDL_Rect positionButton;
        positionButton.x = xOffsets[i]-60;
        positionButton.y = 190;
        SDL_BlitSurface(images.Horaire, NULL, ecran, &positionButton);

        SDL_Rect positionHeader = {xOffsets[i]-20, 260, 0, 0};
        SDL_Surface *surfaceHeader = TTF_RenderText_Blended(font, headers[i], couleurTexte);
        SDL_BlitSurface(surfaceHeader, NULL, ecran, &positionHeader);
        SDL_FreeSurface(surfaceHeader);
    }

    // Afficher les alertes avec espacement uniforme
    int yBase = 330; // Position de départ en Y
    int lineHeight = 50; // Hauteur entre chaque ligne

    for (int i = 0; i < 3; i++) {
        // Sens 0
        if (lignes[i].pileAlertesSensAller != NULL) {  // Modification ici
             //Affichage des cases
            SDL_Rect positionCase;
            positionCase.x=835;
            positionCase.y=yBase-35;
            SDL_BlitSurface(images.Horaire1, NULL, ecran, &positionCase);

            char texteAffiche1[50];
            sprintf(texteAffiche1, "L%d     S0", i);

            SDL_Rect positionTexteLigneSens = {860, yBase, 0, 0};
            SDL_Surface *surfaceTexteLigneSens = TTF_RenderText_Blended(font, texteAffiche1, couleurTexte);
            SDL_BlitSurface(surfaceTexteLigneSens, NULL, ecran, &positionTexteLigneSens);
            SDL_FreeSurface(surfaceTexteLigneSens);

            //Affichage du séparateur
            SDL_Rect positionSens;
            positionSens.x = 880;
            positionSens.y = yBase;
            SDL_BlitSurface(images.Sens, NULL, ecran, &positionSens);

            // Afficher les données (Passagers, MaxDurée, etc.)
            char data[4][20];
            sprintf(data[0], "%d", lignes[i].pileAlertesSensAller->nombrePassagersEnAttente);
            sprintf(data[1], "%d", lignes[i].pileAlertesSensAller->maxDureeAttente);
            sprintf(data[2], "%d", lignes[i].pileAlertesSensAller->minDureeAttente);
            sprintf(data[3], "%d", lignes[i].pileAlertesSensAller->moyenneDureeAttente);

            //Affichage des icones
            SDL_Surface *tabImages[4];
            tabImages[0] = images.Person;
            tabImages[1] = images.Clock;
            tabImages[2] = images.Clock;
            tabImages[3] = images.Clock;

            for (int j = 0; j < 4; j++) {
                SDL_Rect positionData = {xOffsets[j]-10, yBase, 0, 0};
                SDL_Surface *surfaceData = TTF_RenderText_Blended(font, data[j], couleurTexte);
                SDL_BlitSurface(surfaceData, NULL, ecran, &positionData);
                SDL_FreeSurface(surfaceData);

                //Affichage des icones
                SDL_Rect positionIcon;
                positionIcon.x = xOffsets[j]+40;
                positionIcon.y = yBase-5;
                SDL_BlitSurface(tabImages[j], NULL, ecran, &positionIcon);
            }

            yBase += lineHeight; // Augmente l'espacement pour la prochaine ligne
        }

        // Sens 1
        if (lignes[i].pileAlertesSensRetour != NULL) {  // Modification ici
             //Affichage des cases
            SDL_Rect positionCase;
            positionCase.x=835;
            positionCase.y=yBase-35;
            SDL_BlitSurface(images.Horaire1, NULL, ecran, &positionCase);

            char texteAffiche1[50];
            sprintf(texteAffiche1, "L%d     S1", i);

            SDL_Rect positionTexteLigneSens = {860, yBase, 0, 0};
            SDL_Surface *surfaceTexteLigneSens = TTF_RenderText_Blended(font, texteAffiche1, couleurTexte);
            SDL_BlitSurface(surfaceTexteLigneSens, NULL, ecran, &positionTexteLigneSens);
            SDL_FreeSurface(surfaceTexteLigneSens);

            //Affichage du séparateur
            SDL_Rect positionSens;
            positionSens.x = 880;
            positionSens.y = yBase;
            SDL_BlitSurface(images.Sens, NULL, ecran, &positionSens);

            // Afficher les données (Passagers, MaxDurée, etc.)
            char data[4][20];
            sprintf(data[0], "%d", lignes[i].pileAlertesSensRetour->nombrePassagersEnAttente);
            sprintf(data[1], "%d", lignes[i].pileAlertesSensRetour->maxDureeAttente);
            sprintf(data[2], "%d", lignes[i].pileAlertesSensRetour->minDureeAttente);
            sprintf(data[3], "%d", lignes[i].pileAlertesSensRetour->moyenneDureeAttente);

            SDL_Surface *tabImages[4];
            tabImages[0] = images.Person;
            tabImages[1] = images.Clock;
            tabImages[2] = images.Clock;
            tabImages[3] = images.Clock;

            for (int j = 0; j < 4; j++) {
                SDL_Rect positionData = {xOffsets[j]-10, yBase, 0, 0};
                SDL_Surface *surfaceData = TTF_RenderText_Blended(font, data[j], couleurTexte);
                SDL_BlitSurface(surfaceData, NULL, ecran, &positionData);
                SDL_FreeSurface(surfaceData);

                //Affichage des icones
                SDL_Rect positionIcon;
                positionIcon.x = xOffsets[j]+40;
                positionIcon.y = yBase-5;
                SDL_BlitSurface(tabImages[j], NULL, ecran, &positionIcon);
            }

            yBase += lineHeight; // Augmente l'espacement pour la prochaine ligne
        }
    }
}


int calculerPrix(int distance)
{
    int prix=0;
    prix=(distance+30)*10;
    return prix;
}
Ticket venteTicket(Horaire horaireVente, int idZoneSrc, int idZoneDest, int idLigne, int sens, Ligne lignes[] ,
Troncon troncons[] , int nombreLignes, int nombreTroncons)
{
    Ticket tickets;
    tickets.zoneSrc=idZoneSrc;
    tickets.zoneDest=idZoneDest;
    tickets.dateVente=horaireVente;
    tickets.dateVente.jour=horaireVente.jour;
    tickets.dateVente.heure=horaireVente.heure;
    tickets.dateVente.minute=horaireVente.minute;
    tickets.idLigne=idLigne;
    tickets.sens=sens;
    tickets.idBus=-1;
    tickets.horaireEmbarquement.jour=-1;
    tickets.horaireEmbarquement.heure=-1;
    tickets.horaireEmbarquement.minute=-1;
    tickets.distance=calculerDistance(idZoneSrc,idZoneDest,lignes,tickets.idLigne,troncons,nombreLignes,nombreTroncons);
    tickets.prix=calculerPrix(tickets.distance);

    return tickets;
}
void majPersonnesUneZone(Horaire horaireCourant,Zone zones[], int idZone, Ligne lignes[] , int
nombreLignes , Troncon troncons[], int nombreTroncons ,listeLiaisons l)
{
    for(int i=0;i<zones[idZone].nombrePersonnes;i++)
    {
       for(int j=0;j<zones[idZone].personnes[i].nombreActivites;j++)
        {
            if((horaireCourant.jour==zones[idZone].personnes[i].activites[j].horaireDateSouhaite.jour
           &&horaireCourant.heure==zones[idZone].personnes[i].activites[j].horaireDateSouhaite.heure
           &&horaireCourant.minute==zones[idZone].personnes[i].activites[j].horaireDateSouhaite.minute))
           {
               zones[idZone].personnes[i].intentionVoyage=1;
               zones[idZone].personnes[i].idZoneCourante=zones[idZone].personnes[i].activites[j].idZoneSrc;
               remplirPileDestinations(&zones[idZone].personnes[i].destinationsIntermediaires,l
                                       ,zones[idZone].personnes[i].activites[j].idZoneSrc,zones[idZone].personnes[i].activites[j].idZoneDest
                                       ,lignes,nombreLignes,zones[idZone].personnes[i]);
                prochaineDestinationIntermediaire(&zones[idZone].personnes[i].destinationsIntermediaires,&zones[idZone].personnes[i].idLigneCourante
                                                  ,&zones[idZone].personnes[i].idZoneDest);
                zones[idZone].personnes[i].sensCourant=rechercherSens(zones[idZone].personnes[i].idZoneCourante,zones[idZone].personnes[i].idZoneDest,
                                                                      zones[idZone].personnes[i].idLigneCourante,lignes);

                //chercher quelle file à remplir
               celluleFile *p=zones[idZone].listeFilesPassagersEnAttente;
               while (p != NULL &&
               (p->idLigne != zones[idZone].personnes[i].destinationsIntermediaires->idLigne ||
                p->sens != zones[idZone].personnes[i].sensCourant))
                {
                    p = p->suivant;
                }
                if(p!=NULL)
                {
                      Ticket tk=venteTicket(horaireCourant,idZone,zones[idZone].personnes[i].idZoneDest,zones[idZone].personnes[i].idLigneCourante,
                                     zones[idZone].personnes[i].sensCourant,lignes,troncons,nombreLignes,nombreTroncons);
                        enfilerFilePassagers(&p->file,zones[idZone].personnes[i].cin,tk);
                }
           }
        }
    }
}
void majPersonnesToutesZones(Horaire horaireCourant, Zone zones[], int nombreZones,
Ligne lignes[] , int nombreLignes , Troncon troncons[], int nombreTroncons, listeLiaisons l)
{
    for(int i=0;i<nombreZones;i++)
    {
        majPersonnesUneZone(horaireCourant,zones,i,lignes,nombreLignes,troncons,nombreTroncons,l);
    }
}
int calculerGainNet(Bus flotteBus[], int nombreBus, Ligne lignes[], int nombreLignes)
{
    int gainNet = 0;

    for (int i = 0; i < nombreLignes; i++) {
        lignes[i].gainNet = 0;
    }

    for(int i=0;i<nombreBus;i++){
        flotteBus[i].gainNet=0;
    }
    for (int i = 0; i < nombreBus; i++) {
        int gain = 0;
        for (int j = 0; j < flotteBus[i].nombreTickets; j++)
            gain += flotteBus[i].tickets[j].prix;

        int carburantConsome = ((flotteBus[i].distanceparcourue / 30) + 1) * flotteBus[i].prixLitreCarburant;
        flotteBus[i].gainNet = gain - (carburantConsome + flotteBus[i].coutsEtChargesParJour);

        if (flotteBus[i].idLigneCourante >= 0 && flotteBus[i].idLigneCourante < nombreLignes) {
            lignes[flotteBus[i].idLigneCourante].gainNet += flotteBus[i].gainNet;
        }
    }

    for (int i = 0; i < nombreLignes; i++) {
        gainNet += lignes[i].gainNet;
    }

    return gainNet;
}
void afficherStatistiques(int jour,SDL_Surface *tab[],TTF_Font *font,SDL_Color couleurTexte,image images, SDL_Surface *ecran, Bus flotteBus[], int nombreBus, Ligne lignes[], int nombreLignes) {
        font = TTF_OpenFont("Roboto-Regular.ttf", 15);
        SDL_Rect rectEffacer;
        rectEffacer.x = 870;       // Position X de la zone à effacer
        rectEffacer.y = 0;         // Position Y de la zone à effacer
        rectEffacer.w = 490;       // Largeur de la zone à effacer
        rectEffacer.h = ecran->h;  // Hauteur de l'écran


        // Calcul et affichage des gains nets
        int g = calculerGainNet(flotteBus, nombreBus, lignes, nombreLignes);

        // Affichage du séparateur
        SDL_Rect positionGains;
        positionGains.x = 840;
        positionGains.y = 0;
        SDL_BlitSurface(images.BgGains, NULL, ecran, &positionGains);

        SDL_Rect positionBus;
        int xBase = 880;
        int yBase = 220;
        int imageWidth = 50;
        int espaceHorizontal = 40;
        int imagesParLigne = 5;

        for (int i = 0; i < nombreBus; i++) {
            positionBus.x = xBase + (i % imagesParLigne) * (imageWidth + espaceHorizontal);
            positionBus.y = yBase + (i / imagesParLigne) * (imageWidth + 25);

            // Afficher l'image du bus
            SDL_BlitSurface(tab[i], NULL, ecran, &positionBus);

            SDL_Rect positionBusButton;
            positionBusButton.x = positionBus.x-5;
            positionBusButton.y = positionBus.y + imageWidth+5;
            SDL_BlitSurface(images.BusButton, NULL, ecran, &positionBusButton);
            // Afficher le texte en dessous de l'image
            char texteAffiche1[50];
            sprintf(texteAffiche1, "Bus %d", i);
            SDL_Rect positionTexteLigneSens = {positionBus.x, positionBus.y + imageWidth+6, 0, 0};
            SDL_Surface *surfaceTexteLigneSens = TTF_RenderText_Blended(font, texteAffiche1, couleurTexte);
            SDL_BlitSurface(surfaceTexteLigneSens, NULL, ecran, &positionTexteLigneSens);
            SDL_FreeSurface(surfaceTexteLigneSens);

            TTF_Font *font1 = TTF_OpenFont("Roboto-Regular.ttf", 25);
            sprintf(texteAffiche1, "Statistiques du jour %d", jour);
            SDL_Rect positionTexteStat = {990, 170};
            SDL_Surface *surfaceTexteStat = TTF_RenderText_Blended(font1, texteAffiche1, couleurTexte);
            SDL_BlitSurface(surfaceTexteStat, NULL, ecran, &positionTexteStat);
            SDL_FreeSurface(surfaceTexteStat);
    }

    for (int i = 0; i < nombreLignes; i++) {
        // Appliquer le style bold
        TTF_SetFontStyle(font, TTF_STYLE_BOLD);

        SDL_Color blanc={255,255,255};
        SDL_Rect positionLigne;
        int xBase = 880;
        int yBase = 550;
        int imageWidth = 160;
        int imageHeight = 60;
        int espaceHorizontal = 40;
        int espaceVertical = 20;
        int imagesParLigne = 2; // Nombre d'images par ligne

        // Calcul des positions
        positionLigne.x = xBase + (i % imagesParLigne) * (imageWidth + espaceHorizontal);
        positionLigne.y = yBase + (i / imagesParLigne) * (imageHeight + espaceVertical);

        // Afficher le texte au-dessus de l'image
        SDL_Rect positionTexteLigne;
        positionTexteLigne.x = positionLigne.x + 20; // Centré par rapport à l'image
        positionTexteLigne.y = positionLigne.y - 15; // Position au-dessus de l'image
        char texteLigne[50];
        sprintf(texteLigne, "Ligne %d", i);
        SDL_Surface *surfaceTexteLigne = TTF_RenderText_Blended(font, texteLigne, blanc);
        SDL_BlitSurface(surfaceTexteLigne, NULL, ecran, &positionTexteLigne);
        SDL_FreeSurface(surfaceTexteLigne);

        // Afficher l'image de la ligne
        SDL_BlitSurface(images.Money, NULL, ecran, &positionLigne);

        // Afficher le gain net sous l'image
        SDL_Rect positionGainTexte;
        positionGainTexte.x = positionLigne.x + 50; // Centré par rapport à l'image
        positionGainTexte.y = positionLigne.y + imageHeight - 42; // Position en bas de l'image
        char texteGain[50];
        sprintf(texteGain, "%d", lignes[i].gainNet);
        SDL_Surface *surfaceGainTexte = TTF_RenderText_Blended(font, texteGain, blanc);
        SDL_BlitSurface(surfaceGainTexte, NULL, ecran, &positionGainTexte);
        SDL_FreeSurface(surfaceGainTexte);
    }


        // Mettre à jour l'écran
        SDL_Flip(ecran);
}


void sauvegarderRapport(char* nomFichier, int j, Zone zones[], Ligne lignes[], Troncon troncons[], Bus flotteBus[],
                        int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus, listeLiaisons l)
{
    // Créer un nom unique pour le fichier de rapport
    char tempNomFichier[50];
    sprintf(tempNomFichier, "rapport_%d.txt", j);

    // Ouvrir le fichier de rapport en mode "w" pour l'écraser à chaque fois
    FILE *fic = fopen(tempNomFichier, "w");

    // Écriture des informations principales dans le rapport
    fprintf(fic, "Rapport de la journée %d\n", j);
    int g = calculerGainNet(flotteBus, nombreBus, lignes, nombreLignes);
    fprintf(fic, "\nGains par ligne et gain total\n");
    fprintf(fic, "-----------------------------\n");
    fprintf(fic, "| Lignes      | Gain net    |\n");
    fprintf(fic, "-----------------------------\n");
    for (int i = 0; i < nombreLignes; i++) {
        fprintf(fic, "| Ligne %2d    | %10d  |\n", i, lignes[i].gainNet);
        fprintf(fic, "-----------------------------\n");
    }
    fprintf(fic, "| Total       | %10d  |\n", g);
    fprintf(fic, "-----------------------------\n");

    fprintf(fic, "Gains par bus\n");
    fprintf(fic, "-------------------------------------------------------------------------------------------------\n");
    fprintf(fic, "| Bus      | Ligne | Sens | Distance Parcourue | Nombre Passagers | Montant Tickets | Gain Net  |\n");
    fprintf(fic, "-------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < nombreBus; i++) {
        int gains = 0;
        for (int j = 0; j < flotteBus[i].nombreTickets; j++) {
            gains += flotteBus[i].tickets[j].prix;
        }
        fprintf(fic, "| Bus %2d   | %5d | %4d | %18d | %16d | %15d | %9d |\n", i,
                flotteBus[i].idLigneCourante, flotteBus[i].sensCourant, flotteBus[i].distanceparcourue,
                flotteBus[i].nombreTickets, gains, flotteBus[i].gainNet);
        fprintf(fic, "-------------------------------------------------------------------------------------------------\n");
    }

    fprintf(fic, "\nAlertes par ligne et sens\n");
    for (int i = 0; i < nombreLignes; i++) {
        inverserPileAlertes(&lignes[i].pileAlertesSensAller);
        inverserPileAlertes(&lignes[i].pileAlertesSensRetour);
        fprintf(fic, "--------------------------------------------------------------------------------------------------------------\n");
        fprintf(fic, "| Ligne %d - Sens 0      | nombrePassagersEnAttente | maxDureeAttente | minDureeAttente | moyenneDureeAttente |\n", i);
        fprintf(fic, "--------------------------------------------------------------------------------------------------------------\n");
        afficherPileAlerteFic(fic,&lignes[i].pileAlertesSensAller);

        fprintf(fic, "--------------------------------------------------------------------------------------------------------------\n");
        fprintf(fic, "| Ligne %d - Sens 1      | nombrePassagersEnAttente | maxDureeAttente | minDureeAttente | moyenneDureeAttente |\n", i);
        fprintf(fic, "--------------------------------------------------------------------------------------------------------------\n");
        afficherPileAlerteFic(fic,&lignes[i].pileAlertesSensRetour);
    }

    fclose(fic);

    // Sauvegarde dans les fichiers spécifiques en mode "append"
    sauvegarderZones(zones, nombreZones, tempNomFichier);
    sauvegarderLignes(lignes, nombreLignes, tempNomFichier);
    sauvegarderTroncons(troncons, nombreTroncons, tempNomFichier);
    sauvegarderFlotteBus(flotteBus, nombreBus, tempNomFichier);
    sauvegarderPersonnes(zones, nombreZones, tempNomFichier);
    sauvegarderLiaisons(l, tempNomFichier);

    // Réinitialisation des fichiers spécifiques pour la journée
    FILE *clean;
    clean = fopen("zonesSauv.txt", "w"); fclose(clean);
    clean = fopen("flotteBusSauv.txt", "w"); fclose(clean);
    clean = fopen("tronconsSauv.txt", "w"); fclose(clean);
    clean = fopen("lignesSauv.txt", "w"); fclose(clean);
    clean = fopen("liaisonsSauv.txt", "w"); fclose(clean);
    clean = fopen("personnes_sauv.txt", "w"); fclose(clean);

    // Sauvegarde dans les fichiers spécifiques réinitialisés
    sauvegarderZones(zones, nombreZones, "zonesSauv.txt");
    sauvegarderLignes(lignes, nombreLignes, "lignesSauv.txt");
    sauvegarderTroncons(troncons, nombreTroncons, "tronconsSauv.txt");
    sauvegarderFlotteBus(flotteBus, nombreBus, "flotteBusSauv.txt");
    sauvegarderPersonnes(zones, nombreZones, "personnes_sauv.txt");
    sauvegarderLiaisons(l, "liaisonsSauv.txt");
}
void embarquerPersonnes(Zone zones[], Bus flotteBus[] , int idZone, int idBus, Horaire horaireCourant)
{
        //checrher la file correspondante
        if(flotteBus[idBus].enRoute==1){
            celluleFile *p=zones[idZone].listeFilesPassagersEnAttente;
            while ((p != NULL) && ((flotteBus[idBus].idLigneCourante != p->idLigne) || (flotteBus[idBus].sensCourant != p->sens))) {
                p = p->suivant;
            }
            if(p!=NULL){
                emplacement* current=p->file.tete;
                int nbPlacesVidesBus=60-flotteBus[idBus].nombrePassagers;
                while((nbPlacesVidesBus!=0) && (current != NULL)){
                    nbPlacesVidesBus--;
                    current->tk.idBus=idBus;
                    current->tk.horaireEmbarquement.jour=horaireCourant.jour;
                    current->tk.horaireEmbarquement.heure=horaireCourant.heure;
                    current->tk.horaireEmbarquement.minute=horaireCourant.minute;
                    flotteBus[idBus].tickets[flotteBus[idBus].nombreTickets]=current->tk;
                    flotteBus[idBus].nombreTickets++;
                    //checrher la pesronne à déplacer
                    int i=0;
                    while(i<zones[idZone].nombrePersonnes && zones[idZone].personnes[i].cin != current->cinPersonne){
                        i++;
                    }
                    deplacerPersonne(zones[idZone].personnes,flotteBus[idBus].passagers,i,&zones[idZone].nombrePersonnes
                    ,&flotteBus[idBus].nombrePassagers);
                    defilerFilePassagers(&p->file);
                    current=current->suivant;
                }
            }
        }
}
void debarquerPersonnes(Zone zones[], Bus flotteBus[] , int idZone, int idBus,Horaire horaireCourant,
Ligne lignes[])
{
    int j;
    for(int i=0;i<flotteBus[idBus].nombrePassagers;i++)
    {
         if(flotteBus[idBus].passagers[i].destinationsIntermediaires!=NULL && flotteBus[idBus].passagers[i].destinationsIntermediaires->idZoneDest == idZone)
            {
                deplacerPersonne(flotteBus[idBus].passagers,zones[idZone].personnes,i,&flotteBus[idBus].nombrePassagers,
                &zones[idZone].nombrePersonnes);
                j=zones[idZone].nombrePersonnes-1;

                depilerPileDestinations(&zones[idZone].personnes[j].destinationsIntermediaires);
                if(zones[idZone].personnes[j].destinationsIntermediaires==NULL){
                    zones[idZone].personnes[j].intentionVoyage=0;
                }
                else{
                    zones[idZone].personnes[j].idZoneCourante=idZone;
                    prochaineDestinationIntermediaire(&zones[idZone].personnes[j].destinationsIntermediaires,
                                                      &zones[idZone].personnes[j].idLigneCourante,&zones[idZone].personnes[j].idZoneDest);
                    zones[idZone].personnes[j].sensCourant=rechercherSens(zones[idZone].personnes[j].idZoneCourante,zones[idZone].personnes[j].idZoneDest
                                                                             ,zones[idZone].personnes[j].idLigneCourante,lignes);
                }
            }
    }
}
void deplacertEtReinitialiserTicketsBus(Bus flotteBus[],int nombreBus, Ticket tickets[],int *nombreTickets)
{
    //Deplacement des tickets vers le tableau Ticket
    for(int i=0;i<nombreBus;i++)
    {
        for(int j=0;j<flotteBus[i].nombreTickets;j++)
        {
            tickets[*nombreTickets]=flotteBus[i].tickets[j];
            (*nombreTickets)++;
        }
    }
    //Reinisialisation des tableaux Tickets des bus
     for(int i=0;i<nombreBus;i++)
     {
         for(int j=0;j<flotteBus[i].nombreTickets;j++)
         {
             flotteBus[i].tickets[j].dateVente.jour=-1;
             flotteBus[i].tickets[j].dateVente.heure=-1;
             flotteBus[i].tickets[j].dateVente.minute=-1;
             flotteBus[i].tickets[j].distance=-1;
             flotteBus[i].tickets[j].idBus=-1;
             flotteBus[i].tickets[j].idLigne=-1;
             flotteBus[i].tickets[j].prix=-1;
             flotteBus[i].tickets[j].sens=-1;
             flotteBus[i].tickets[j].zoneDest=-1;
             flotteBus[i].tickets[j].zoneSrc=-1;
         }
         flotteBus[i].nombreTickets=0;
     }
}
void initialiserBus(Bus flotteBus[], int nombreBus, Ligne lignes[]) {
    int idLigne;
    for (int i = 0; i < nombreBus; i++) {
        idLigne = flotteBus[i].idLigneCourante;

        // Déterminer le sens courant du bus
        if (flotteBus[i].idZoneCourante == lignes[idLigne].idZones[0]) {
            flotteBus[i].sensCourant = 0; // Sens 0
        } else {
            flotteBus[i].sensCourant = 1; // Sens 1
        }

        // Initialiser les sens des tâches alternativement
        int currentSens = flotteBus[i].sensCourant;
        for (int j = 0; j < flotteBus[i].nombreTaches; j++) {
            flotteBus[i].taches[j].sens = currentSens;
            currentSens = 1 - currentSens; // Alterner entre 0 et 1
        }
    }
}
void initialiseJour(int *nombrePersonnesInitial, Personne **personnesInitial,Casecarte carteGeographique[40][40],Zone zones[],Ligne lignes[],Bus flotteBus[],Troncon troncons[]
                    ,int nombreLignes,int nombreTroncons,int nombreZones,int nombreBus,int j,Ticket tickets[],int *nombreTickets)
{
            // Restauration de l'état initial pour le nouveau jour
            for(int z = 0; z < nombreZones; z++) {
                zones[z].nombrePersonnes = nombrePersonnesInitial[z];
                for(int p = 0; p < nombrePersonnesInitial[z]; p++) {
                    zones[z].personnes[p] = personnesInitial[z][p];
                }
            }

             // Réinitialisation des états des personnes et mise à jour des activités
            for(int a = 0; a < nombreZones; a++) {
                for(int b = 0; b < zones[a].nombrePersonnes; b++) {
                    zones[a].personnes[b].idZoneCourante = zones[a].personnes[b].idZoneHabitat;
                    for(int c = 0; c < zones[a].personnes[b].nombreActivites; c++) {
                        zones[a].personnes[b].activites[c].horaireDateSouhaite.jour = j;
                    }
                }
            }

            // Retour des passagers des bus à leurs zones d'habitat
                    for(int b = 0; b < nombreBus; b++) {
                        while(flotteBus[b].nombrePassagers > 0) {
                            Personne p = flotteBus[b].passagers[--flotteBus[b].nombrePassagers];
                            int zoneHabitat = p.idZoneHabitat;

                            if(p.idZoneCourante != zoneHabitat) {
                                for(int z = 0; z < nombreZones; z++) {
                                    if(z == zoneHabitat) {
                                        for(int idx = 0; idx < zones[z].nombrePersonnes; idx++) {
                                            if(zones[z].personnes[idx].cin == p.cin) {
                                                zones[z].personnes[idx].idZoneCourante = zoneHabitat;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
            initialiserCarte(carteGeographique, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus);
            initialiserPileAlerte(lignes,nombreLignes);

            //initialisation des listeFilePassagers de chaque zone pour toutes les lignes
            for(int i=0;i<nombreZones;i++){
                creerListe(&zones[i].listeFilesPassagersEnAttente);
            }
            initialiserListesFilePassagers(lignes,nombreLignes,zones);
            //initialisation des bus
            initialiserBus(flotteBus,nombreBus,lignes);
            for(int a=0;a<nombreBus;a++){
                 for(int b=0;b<flotteBus[a].nombreTaches;b++){
                            flotteBus[a].taches[b].horaireDateDepart.jour=j;
                  }
            }
            for(int a=0;a<nombreZones;a++){
                  for(int b=0;b<zones[a].nombrePersonnes;b++){
                       for(int c=0;c<zones[a].personnes[b].nombreActivites;c++){
                                  zones[a].personnes[b].activites[c].horaireDateSouhaite.jour=j;
                   }
                        }
           }

            deplacertEtReinitialiserTicketsBus(flotteBus,nombreBus,tickets,nombreTickets);
           //dépiler les piles alertes
            for(int i=0;i<nombreLignes;i++){
                while(lignes[i].pileAlertesSensAller != NULL){
                    depilerPileAlertes(&lignes[i].pileAlertesSensAller);
                }
                while(lignes[i].pileAlertesSensRetour != NULL){
                    depilerPileAlertes(&lignes[i].pileAlertesSensRetour);
                }
            }
}
void deroulerMinute(SDL_Surface *tab[],image images,int m,int h,int j,SDL_Surface *ecran,Casecarte carteGeographique[40][40], Ticket tickets[], Zone zones[], Ligne lignes[], Troncon
                    troncons[], Bus flotteBus[], int nombreZones, int nombreLignes, int nombreTroncons, int nombreBus,int*nombreTickets, listeLiaisons l,TTF_Font *font, SDL_Color couleurTexte)
{
    Horaire now;
    int i,k,v,counter=0;
    now.minute=m;
    now.heure=h;
    now.jour=j;

    majPersonnesToutesZones(now,zones,nombreZones,lignes,nombreLignes,troncons,nombreTroncons,l);
    for(i=0; i<nombreBus; i++)
    {
        if(flotteBus[i].enRoute==0)
        {
            k=numTacheSuivante(now,i,flotteBus);
            if(k!=-1)
            {
                //Maj du bus
                flotteBus[i].enRoute=1;
                flotteBus[i].idLigneCourante=flotteBus[i].taches[k].idLigne;
                flotteBus[i].sensCourant=flotteBus[i].taches[k].sens;

            }
        }
        if(flotteBus[i].row==zones[flotteBus[i].idZoneCourante].row && flotteBus[i].column==zones[flotteBus[i].idZoneCourante].column)
        {
            debarquerPersonnes(zones,flotteBus,zones[flotteBus[i].idZoneCourante].id,i,now,lignes);
            embarquerPersonnes(zones,flotteBus,zones[flotteBus[i].idZoneCourante].id,i,now);
        }
        if(flotteBus[i].enRoute==1)
        {
            deplacerBus(carteGeographique,i,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,now);
        }

        if(h==21 && m==59)
        {
            afficherStatistiques(j,tab,font,couleurTexte,images,ecran,flotteBus,nombreBus,lignes,nombreLignes);
        }
        else{
            afficherAlerte(images,lignes,nombreLignes,now,font,couleurTexte,ecran);
        }
    }
    rechercherLonguesAttentesToutesZones(zones,nombreZones,lignes,now,45);
}

void afficherMenuEtTemps(SDL_Surface *tab[],image images,SDL_Surface *ecran, TTF_Font *font, int indiceVitesse, int j, int h, int m, SDL_Color noir, SDL_Color rouge) {

    // Affichage du temps avec l'image en arrière-plan
        char texteAffiche[50];
        sprintf(texteAffiche, "   0%d              %2d             %2d", j, h, m);

        // Charger et bliter l'image (arrière-plan)
        SDL_Rect positionTempsImage;
        positionTempsImage.x=350;
        positionTempsImage.y=-5;
        SDL_BlitSurface(images.Time, NULL, ecran, &positionTempsImage);


        // Créer et bliter le texte (premier plan)
        SDL_Rect positionTexteTemps;
        positionTexteTemps.x = 375;
        positionTexteTemps.y = 10;
        SDL_Surface *surfaceTexteTemps = TTF_RenderText_Blended(font, texteAffiche, noir);
        SDL_BlitSurface(surfaceTexteTemps, NULL, ecran, &positionTexteTemps);
        SDL_FreeSurface(surfaceTexteTemps);

        // Affichage du menu des vitesses
        int y_depart = 200;
        for (int i = 0; i < 5; i++) {
        SDL_Rect positionImage;
        positionImage.x=650;
        positionImage.y=y_depart+i*50;
        if(i==indiceVitesse){
            SDL_BlitSurface(tab[i+5],NULL,ecran,&positionImage);
        }
        else{
            SDL_BlitSurface(tab[i],NULL,ecran,&positionImage);
        }
    }
}

   // Fonction pour afficher les informations d'un bus
    void afficherInfosBus(Bus bus, SDL_Surface *tabInfosBus[], TTF_Font *font, SDL_Surface *ecran) {
        SDL_Color noir = {0, 0, 0};
        char texteAffiche[50];

        // Dimensions et positions des images
        int espaceHorizontal = 980;
        int espaceVertical = 320;
        int margeHorizontale = 120; // Espacement entre les images
        int margeVerticale = 70;  // Espacement entre les lignes

        // Calcul des positions pour chaque image
        SDL_Rect positionsImages[6];
        for (int i = 0; i < 6; i++) {
            int col = i % 2; // Colonne (0 ou 1)
            int row = i / 2; // Ligne
            positionsImages[i].x = espaceHorizontal + col *  margeHorizontale;
            positionsImages[i].y = espaceVertical + row *  margeVerticale;
        }
        TTF_Font *font1 = TTF_OpenFont("Roboto-Regular.ttf", 12);

        SDL_Rect positionTexte = {
            .x = positionsImages[0].x+50,
            .y = positionsImages[0].y + 25
        };

        SDL_Surface *surfaceTexte = TTF_RenderText_Blended(font1, "Ligne courante", noir);
        SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);

        sprintf(texteAffiche, "%d", bus.idLigneCourante);
        afficherTexteSurImage(texteAffiche, font, noir, ecran, positionsImages[0], tabInfosBus[0]);

        positionTexte.x = positionsImages[1].x+50;
        positionTexte.y = positionsImages[1].y + 25;

        surfaceTexte = TTF_RenderText_Blended(font1, "Sens courant", noir);
        SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);

        sprintf(texteAffiche, "%d", bus.sensCourant);
        afficherTexteSurImage(texteAffiche, font, noir, ecran, positionsImages[1], tabInfosBus[1]);

        positionTexte.x = positionsImages[2].x+50;
        positionTexte.y = positionsImages[2].y + 25;

        surfaceTexte = TTF_RenderText_Blended(font1, "Distance parcourue", noir);
        SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);

        sprintf(texteAffiche, "%d", bus.distanceparcourue);
        afficherTexteSurImage(texteAffiche, font, noir, ecran, positionsImages[2], tabInfosBus[2]);

        positionTexte.x = positionsImages[3].x+50;
        positionTexte.y = positionsImages[3].y + 25;

        surfaceTexte = TTF_RenderText_Blended(font1, "Nombre de tickets", noir);
        SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);

        sprintf(texteAffiche, "%d", bus.nombreTickets);
        afficherTexteSurImage(texteAffiche, font, noir, ecran, positionsImages[3], tabInfosBus[3]);

        int MontantTickets=0;
        for(int i=0;i<bus.nombreTickets;i++){
            MontantTickets+=bus.tickets[i].prix;
        }
        positionTexte.x = positionsImages[4].x+50;
        positionTexte.y = positionsImages[4].y + 25;

        surfaceTexte = TTF_RenderText_Blended(font1, "Gains tickets", noir);
        SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);

        sprintf(texteAffiche, "%d", MontantTickets);
        afficherTexteSurImage(texteAffiche, font, noir, ecran, positionsImages[4], tabInfosBus[4]);

        positionTexte.x = positionsImages[5].x+50;
        positionTexte.y = positionsImages[5].y + 25;

        surfaceTexte = TTF_RenderText_Blended(font1, "Gaint net", noir);
        SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);

        sprintf(texteAffiche, "%d", bus.gainNet);
        afficherTexteSurImage(texteAffiche, font, noir, ecran, positionsImages[5], tabInfosBus[5]);
    }

    // Fonction pour afficher du texte sur une image
    void afficherTexteSurImage(char *texte, TTF_Font *font, SDL_Color couleur, SDL_Surface *ecran, SDL_Rect positionImage, SDL_Surface *image) {
        font = TTF_OpenFont("Roboto-Regular.ttf", 15);

        // Position de l'image
        SDL_BlitSurface(image, NULL, ecran, &positionImage);

        // Calcul de la position du texte à l'intérieur de l'image
        SDL_Rect positionTexte = {
            .x = positionImage.x + 70,  // Décalage horizontal
            .y = positionImage.y + 55, // Décalage vertical
            .w = 0,
            .h = 0
        };

        // Création de la surface pour le texte
        SDL_Surface *surfaceTexte = TTF_RenderText_Blended(font, texte, couleur);

        // Affichage du texte
        SDL_BlitSurface(surfaceTexte, NULL, ecran, &positionTexte);

        // Nettoyage
        SDL_FreeSurface(surfaceTexte);
    }

    int main(int argc, char *argv[])
    {
        Zone *zones=malloc(sizeof(Zone)*20);
        Ligne *lignes=malloc(sizeof(Ligne)*3);
        Troncon *troncons=malloc(sizeof(Troncon)*19);
        Bus *flotteBus=malloc(sizeof(Bus)*18);
        Ticket *tickets=malloc(sizeof(Ticket)*20000);
        Casecarte carteGeographique[40][40];
        Horaire now;

        listeLiaisons liaisons;
        liaisons.premier = NULL;
        liaisons.dernier = NULL;
        int nombreZones,nombreLignes,choix,nombreTroncons,nombreBus,nombreTickets=0,m=0,h=6,j=0;
        now.jour=j;
        now.heure=h;
        now.minute=m;

        int continuer = 1;
        SDL_Surface *ecran = NULL, *menu = NULL;
        SDL_Rect positionMenu;
        SDL_Event event;
        TTF_Font* font = NULL;

        SDL_Surface* texte = NULL;

        image images;
        images.bus=IMG_Load("bus.png");
        images.busM=IMG_Load("busM.jpeg");
        images.codeRue10=IMG_Load("codeRue5.jpg");
        images.codeRue12=IMG_Load("codeRue12.jpg");
        images.codeRue15=IMG_Load("codeRue15.jpg");
        images.codeRue3=IMG_Load("codeRue3.jpg");
        images.codeRue5=IMG_Load("codeRue10.jpg");
        images.codeRue6=IMG_Load("codeRue6.jpg");
        images.codeRue7=IMG_Load("codeRue7.jpg");
        images.codeRue9=IMG_Load("codeRue9.jpg");
        images.zoneVide=IMG_Load("zoneVide.png");
        images.zoneNonVideBM=IMG_Load("zoneNonVideBM.png");
        images.zoneNonVideBus=IMG_Load("zoneNonVideBus.png");
        images.OptionJeu=IMG_Load("OptionJeu.png");
        images.OptionCharger=IMG_Load("OptionCharger.png");
        images.Time = IMG_Load("Time.png");
        images.SideBar=IMG_Load("SideBar.png");
        //images.BackgroundMap=IMG_Load("Background.png");
        images.BackgroundMap=IMG_Load("Bg.jpg");
        images.Alerte=IMG_Load("Alerte.png");
        images.Clock = IMG_Load("Clock.png");
        images.Person = IMG_Load("Person.png");
        images.Sens = IMG_Load("Sens.png");
        images.Horaire = IMG_Load("Horaire.png");
        images.Horaire1 = IMG_Load("Horaire1.png");
        images.Watch = IMG_Load("station.png");
        images.Alarme = IMG_Load("alarme.png");
        images.YellowAlerte = IMG_Load ("yellowAlerte.png");
        images.BgGains = IMG_Load("BgGains.png");
        images.BusButton = IMG_Load("BusButton.png");
        images.Bus0 = IMG_Load("bus0.png");
        images.Bus1 = IMG_Load("bus1.png");
        images.Bus2 = IMG_Load("bus2.png");
        images.Bus3 = IMG_Load("bus3.png");
        images.Bus4 = IMG_Load("bus4.png");
        images.Bus5 = IMG_Load("bus5.png");
        images.Bus6 = IMG_Load("bus6.png");
        images.Bus7 = IMG_Load("bus7.png");
        images.Bus8 = IMG_Load("bus8.png");
        images.Bus9 = IMG_Load("bus9.png");
        images.Bus10 = IMG_Load("bus10.png");
        images.Bus11 = IMG_Load("bus11.png");
        images.Bus12 = IMG_Load("bus12.png");
        images.Bus13 = IMG_Load("bus13.png");
        images.Bus14 = IMG_Load("bus14.png");
        images.Bus15 = IMG_Load("bus15.png");
        images.Bus16 = IMG_Load("bus16.png");
        images.Bus17 = IMG_Load("bus17.png");
        images.Money = IMG_Load("Money.png");
        images.BgGainsFlou = IMG_Load("BgGainsFlou.jpg");
        images.BackgroundBus = IMG_Load("BackgroundBus.png");
        images.LigneBus = IMG_Load("LigneBus.png");
        images.SensBus = IMG_Load("SensBus.png");
        images.DistanceParcourue = IMG_Load("DistanceParcourue.png");
        images.NbPassagers = IMG_Load("NbPassagers.png");
        images.MontantTickets = IMG_Load("MontantTickets.png");
        images.GainNet = IMG_Load("GainNet.png");

        SDL_Surface* tabImages[10];

        tabImages[0] = IMG_Load("1Minute.png");
        tabImages[1] = IMG_Load("10Minutes.png");
        tabImages[2] = IMG_Load("1Heure.png");
        tabImages[3] = IMG_Load("4Heures.png");
        tabImages[4] = IMG_Load("1Jour.png");
        tabImages[5] = IMG_Load("1Minuteclique.png");
        tabImages[6] = IMG_Load("10Minutesclique.png");
        tabImages[7] = IMG_Load("1Heureclique.png");
        tabImages[8] = IMG_Load("4Heuresclique.png");
        tabImages[9] = IMG_Load("1Jourclique.png");

        SDL_Surface *tabImagesBus[18];

        tabImagesBus[0] = images.Bus0;
        tabImagesBus[1] = images.Bus1;
        tabImagesBus[2] = images.Bus2;
        tabImagesBus[3] = images.Bus3;
        tabImagesBus[4] = images.Bus4;
        tabImagesBus[5] = images.Bus5;
        tabImagesBus[6] = images.Bus6;
        tabImagesBus[7] = images.Bus7;
        tabImagesBus[8] = images.Bus8;
        tabImagesBus[9] = images.Bus9;
        tabImagesBus[10] = images.Bus10;
        tabImagesBus[11] = images.Bus11;
        tabImagesBus[12] = images.Bus12;
        tabImagesBus[13] = images.Bus13;
        tabImagesBus[14] = images.Bus14;
        tabImagesBus[15] = images.Bus15;
        tabImagesBus[16] = images.Bus16;
        tabImagesBus[17] = images.Bus17;

        SDL_Surface *tabInfosBus[7];
        tabInfosBus[0] = images.LigneBus;
        tabInfosBus[1] = images.SensBus;
        tabInfosBus[2] = images.DistanceParcourue;
        tabInfosBus[3] = images.NbPassagers;
        tabInfosBus[4] = images.MontantTickets;
        tabInfosBus[5] = images.GainNet;

        SDL_Init(SDL_INIT_VIDEO);

        SDL_WM_SetIcon(IMG_Load("bus.png"), NULL); // L'icône doit être chargée avant SDL_SetVideoMode
        ecran = SDL_SetVideoMode(1360, 684, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);//ici les dimensions de l'écran
        SDL_WM_SetCaption("SRTB", NULL);
        int indiceOption=0,indiceVitesse=0,numecran=0;

        menu = IMG_Load("Ville.png");

        positionMenu.x = 0;
        positionMenu.y = 0;

        // texte
        TTF_Init();
        font = TTF_OpenFont("Roboto-Regular.ttf", 25);

        SDL_Color noir = {0, 0, 0};
        SDL_Color blanc = { 255, 255, 255 };
        SDL_Color rouge = { 255, 0, 0 };
        SDL_Rect positionText;


        remplissageComplet(zones, lignes, troncons, flotteBus, &nombreZones, &nombreLignes, &nombreTroncons, &nombreBus, &liaisons);
        initialiserCarte(carteGeographique, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus);
        initialiserPileAlerte(lignes,nombreLignes);

        images.SideBar = SDL_DisplayFormat(images.SideBar);

        // Allocation dynamique de la mémoire
        Personne **personnesInitial = (Personne **)malloc(nombreZones * sizeof(Personne *));
        int *nombrePersonnesInitial = (int *)malloc(nombreZones * sizeof(int));
        while (continuer)
        {
            SDL_WaitEvent(&event);
          switch (event.type) {
        case SDL_QUIT:
            continuer = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:

                if (numecran == 1) {
                    numecran = 0; // Revenir à l'écran numéro 0
                } else if (numecran == 0) {
                    continuer = 0; // Quitter le programme
                }
                break;
                case SDLK_RETURN:
                    numecran=1;
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN :
        if (event.button.button == SDL_BUTTON_LEFT ) {
            int x = event.button.x;
            int y = event.button.y;
            if (numecran == 0) {
                if ((x >= 550 && x <= 920) && (y >= 400 && y <= 500)) {
                    indiceOption = 0;
                } else if ((x >= 550 && x <= 920) && (y >= 500 && y <= 600)) {
                    indiceOption = 1;
                }
                else{
                    break;
                }
                if (indiceOption == 0) {
                    genererPersonne(zones, nombreZones, lignes, nombreLignes);
                    numecran = 1;

                    for(int i = 0; i < nombreZones; i++) {
                        personnesInitial[i] = (Personne *)malloc(300 * sizeof(Personne));
                    }

                    // Sauvegarde de l'état initial
                    for(int z = 0; z < nombreZones; z++) {
                        nombrePersonnesInitial[z] = zones[z].nombrePersonnes;
                        for(int p = 0; p < zones[z].nombrePersonnes; p++) {
                            personnesInitial[z][p] = zones[z].personnes[p];
                        }
                    }
                } else {
                    chargerPersonnes(zones, &nombreZones, "personnesSauv.txt");
                    numecran = 1;
                    for(int i = 0; i < nombreZones; i++) {
                        personnesInitial[i] = (Personne *)malloc(300 * sizeof(Personne));
                    }

                    // Sauvegarde de l'état initial
                    for(int z = 0; z < nombreZones; z++) {
                        nombrePersonnesInitial[z] = zones[z].nombrePersonnes;
                        for(int p = 0; p < zones[z].nombrePersonnes; p++) {
                            personnesInitial[z][p] = zones[z].personnes[p];
                        }
                    }
            }} else if (numecran==1) {
                if((x>= 650 && x <= 800) && (y>=200 && y<=235)){
                    indiceVitesse=0;
                }
                else if((x >= 650 && x <= 800) && (y>=250 && y<=285)){
                    indiceVitesse=1;
                }
                else if((x >= 650 && x <= 800) && (y>=300 && y<=335)){
                    indiceVitesse=2;
                }
                else if((x >= 650 && x <= 800) && (y>=350 && y<=385)){
                    indiceVitesse=3;
                }
                else if((x >= 650 && x <= 800) &&(y>=400 && y<=435)){
                    indiceVitesse=4;
                }
                else{
                    break;
                }
                if (indiceVitesse == 0) {
                    afficherCarteGeo(images, ecran, carteGeographique, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, rouge, font);
                    afficherMenuEtTemps(tabImages,images, ecran, font, indiceVitesse, j, h, m, noir, rouge);
                    deroulerMinute(tabImagesBus,images,m, h, j, ecran, carteGeographique, tickets, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, &nombreTickets, liaisons, font, noir);

                    m++;
                    if(h==21 && m==59){

                        sauvegarderRapport("Statistique.txt",j,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,liaisons);
                    }
                    if (m == 60) {
                        h++;
                        m = 0;
                    }
                    if (h == 22 && m == 0) {
                        numecran=2;
                        h = 6;
                        j++;

                    }
                    now.jour=j;
                    now.heure=h;
                    now.minute=m;
                    SDL_Flip(ecran);

                } else if (indiceVitesse == 1) {
                    int counter = 10;
                    while (counter--) {
                        afficherCarteGeo(images, ecran, carteGeographique, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, rouge, font);
                        afficherMenuEtTemps(tabImages,images, ecran, font, indiceVitesse, j, h, m, noir, rouge);
                        deroulerMinute(tabImagesBus,images,m, h, j, ecran, carteGeographique, tickets, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, &nombreTickets, liaisons, font, noir);
                        m++;
                        if(h==21 && m==59){
                            sauvegarderRapport("Statistique.txt",j,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,liaisons);
                        }
                        if (m == 60) {
                            h++;
                            m = 0;
                        }
                        if (h == 22 && m == 0) {
                            numecran=2;
                            h = 6;
                            j++;

                        }
                        now.jour=j;
                        now.heure=h;
                        now.minute=m;
                        SDL_Flip(ecran);
                    }
                } else if (indiceVitesse == 2) {
                    int counter = 60;
                    while (counter--) {
                        afficherCarteGeo(images, ecran, carteGeographique, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, rouge, font);
                        afficherMenuEtTemps(tabImages,images, ecran, font, indiceVitesse, j, h, m, noir, rouge);
                        deroulerMinute(tabImagesBus,images,m, h, j, ecran, carteGeographique, tickets, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, &nombreTickets, liaisons, font, noir);

                        m++;
                        if(h==21 && m==59){
                            afficherStatistiques(now.jour,tabImagesBus,font,noir,images,ecran,flotteBus,nombreBus,lignes,nombreLignes);
                            sauvegarderRapport("Statistique.txt",j,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,liaisons);
                        }
                        if (m == 60) {
                            h++;
                            m = 0;
                        }
                        if (h == 22 && m == 0) {
                            numecran=2;
                            h = 6;
                            j++;

                        }
                         now.jour=j;
                        now.heure=h;
                        now.minute=m;
                        SDL_Flip(ecran);
                    }
                } else if (indiceVitesse == 3) {
                    int counter = 240;
                    while (counter--) {
                        afficherCarteGeo(images, ecran, carteGeographique, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, rouge, font);
                        afficherMenuEtTemps(tabImages, images,ecran, font, indiceVitesse, j, h, m, noir, rouge);
                        deroulerMinute(tabImagesBus,images,m, h, j, ecran, carteGeographique, tickets, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, &nombreTickets, liaisons, font, noir);

                        m++;
                        if(h==21 && m==59){
                            sauvegarderRapport("Statistique.txt",j,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,liaisons);
                        }
                        if (m == 60) {
                            h++;
                            m = 0;
                        }
                        if (h == 22 && m == 0) {
                            numecran=2;
                            h = 6;
                            j++;

                        }
                        now.jour=j;
                        now.heure=h;
                        now.minute=m;
                        SDL_Flip(ecran);
                    }
                } else {
                    int counter;
                    counter=(21-now.heure)*60+(60-now.minute);
                    while (counter--) {
                        afficherCarteGeo(images, ecran, carteGeographique, zones, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, rouge, font);
                        afficherMenuEtTemps(tabImages, images,ecran, font, indiceVitesse, j, h, m, noir, rouge);
                        deroulerMinute(tabImagesBus,images,m, h, j, ecran, carteGeographique, tickets, zones, lignes, troncons, flotteBus, nombreZones, nombreLignes, nombreTroncons, nombreBus, &nombreTickets,
                                       liaisons, font, noir);

                        m++;
                        if(h==21 && m==59){

                            sauvegarderRapport("Statistique.txt",j,zones,lignes,troncons,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,liaisons);
                        }
                        if (m == 60) {
                            h++;
                            m = 0;
                        }
                        if (h == 22 && m == 0) {
                            numecran=2;
                            h = 6;
                            j++;

                        }
                        now.jour=j;
                        now.heure=h;
                        now.minute=m;
                        SDL_Flip(ecran);
                    }
                }
            }
            else if (numecran == 2) {

                // Position de base des images des bus
                int xBase = 880;
                int yBase = 220;
                int imageWidth = 50;
                int espaceHorizontal = 40;
                int imagesParLigne = 5;

                for (int i = 0; i < nombreBus; i++) {
                    // Calculer la position de l'image du bus
                    int busX = xBase + (i % imagesParLigne) * (imageWidth + espaceHorizontal);
                    int busY = yBase + (i / imagesParLigne) * (imageWidth + 25);

                    // Vérifier si le clic est à l'intérieur des limites de l'image du bus
                    if (x >= busX && x <= busX + imageWidth && y >= busY && y <= busY + imageWidth) {
                        SDL_Rect positionBgGainsFlou;
                        positionBgGainsFlou.x=840;
                        positionBgGainsFlou.y=0;
                        SDL_BlitSurface(images.BgGainsFlou,NULL,ecran,&positionBgGainsFlou);

                        SDL_Rect positionBackgroundBus;
                        positionBackgroundBus.x=950;
                        positionBackgroundBus.y=30;
                        SDL_BlitSurface(images.BackgroundBus,NULL,ecran,&positionBackgroundBus);

                        char texteAffiche1[50];
                        sprintf(texteAffiche1, "Bus %d", i);
                        SDL_Rect positionTexteLigneSens = {1100, 157, 0, 0};
                        SDL_Surface *surfaceTexteLigneSens = TTF_RenderText_Blended(font, texteAffiche1, noir);
                        SDL_BlitSurface(surfaceTexteLigneSens, NULL, ecran, &positionTexteLigneSens);
                        SDL_FreeSurface(surfaceTexteLigneSens);

                        // Afficher l'image détaillée du bus sélectionné
                        SDL_Rect positionBus;
                        positionBus.x = 1100;
                        positionBus.y = 240;
                        SDL_BlitSurface(tabImagesBus[i], NULL, ecran, &positionBus);

                        afficherInfosBus(flotteBus[i], tabInfosBus, font, ecran);

                        numecran = 3;
                        break; // Sortir de la boucle une fois le bus sélectionné
                    }
                }

                if((x>=1300 && x<=1380) && (y>=100 && y<=130)){
                    numecran=1;
                }
            }
            else if(numecran==3){

                if((x>=1265 && x<=1295) && (y>=155 && y<=185)){
                   numecran=2;
                }
                else{
                    break;
                }
            }
        }
        break;
}

         // éffacement de l'écran
            //SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
                if(numecran==0){
                    // attachement de l'image du menu à l'écran
                    SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
                    for(int i=0;i<2;i++){
                        SDL_Rect positionMenuJeu;
                        positionMenuJeu.x=550;
                        positionMenuJeu.y=400+i*100;
                        if(i==0)
                            SDL_BlitSurface(images.OptionJeu,NULL,ecran,&positionMenuJeu);
                        else
                            SDL_BlitSurface(images.OptionCharger,NULL,ecran,&positionMenuJeu);
                        }
                } else if(numecran==1){
                    if(h==6 && m==0){
                        initialiseJour(nombrePersonnesInitial, personnesInitial,carteGeographique,zones,lignes,flotteBus,troncons,nombreLignes,nombreTroncons,nombreZones,nombreBus,j,tickets,&nombreTickets);
                    }
                    afficherCarteGeo(images,ecran,carteGeographique,zones,flotteBus,nombreZones,nombreLignes,nombreTroncons,nombreBus,noir,font);
                    afficherMenuEtTemps(tabImages,images,ecran, font, indiceVitesse, j, h, m, noir, rouge);
                    afficherAlerte(images,lignes,nombreLignes,now,font,noir,ecran);
                }
                else if(numecran==2){
                    afficherStatistiques(now.jour,tabImagesBus,font,noir,images,ecran,flotteBus,nombreBus,lignes,nombreLignes);
                }
            //Mise à jour de l'écran
            SDL_Flip(ecran);
        }

        SDL_FreeSurface(texte);

        TTF_CloseFont(font);
        SDL_FreeSurface(menu);

        TTF_Quit();
        SDL_Quit();

        free(zones);
        free(lignes);
        free(troncons);
        free(flotteBus);
        free(tickets);

        // Libération de la mémoire
        for(int i = 0; i < nombreZones; i++) {
            free(personnesInitial[i]);
        }
        free(personnesInitial);
        free(nombrePersonnesInitial);


        //libérer les images
        SDL_FreeSurface(images.bus);
        SDL_FreeSurface(images.busM);
        SDL_FreeSurface(images.zoneVide);
        SDL_FreeSurface(images.zoneNonVideBus);
        SDL_FreeSurface(images.zoneNonVideBM);
        SDL_FreeSurface(images.codeRue3);
        SDL_FreeSurface(images.codeRue12);
        SDL_FreeSurface(images.codeRue15);
        SDL_FreeSurface(images.codeRue6);
        SDL_FreeSurface(images.codeRue7);
        SDL_FreeSurface(images.codeRue9);
        SDL_FreeSurface(images.codeRue10);
        SDL_FreeSurface(images.OptionJeu);
        SDL_FreeSurface(images.OptionCharger);
        SDL_FreeSurface(images.Time);
        SDL_FreeSurface(images.SideBar);
        SDL_FreeSurface(images.BackgroundMap);
        SDL_FreeSurface(images.Alerte);
        SDL_FreeSurface(images.Clock);
        SDL_FreeSurface(images.Person);
        SDL_FreeSurface(images.Sens);
        SDL_FreeSurface(images.Horaire);
        SDL_FreeSurface(images.Horaire1);
        SDL_FreeSurface(images.Watch);
        SDL_FreeSurface(images.Alarme);
        SDL_FreeSurface(images.YellowAlerte);
        SDL_FreeSurface(images.BgGains);
        SDL_FreeSurface(images.BusButton);
        SDL_FreeSurface(images.Bus0);
        SDL_FreeSurface(images.Bus1);
        SDL_FreeSurface(images.Bus2);
        SDL_FreeSurface(images.Bus3);
        SDL_FreeSurface(images.Bus4);
        SDL_FreeSurface(images.Bus5);
        SDL_FreeSurface(images.Bus6);
        SDL_FreeSurface(images.Bus7);
        SDL_FreeSurface(images.Bus8);
        SDL_FreeSurface(images.Bus9);
        SDL_FreeSurface(images.Bus10);
        SDL_FreeSurface(images.Bus11);
        SDL_FreeSurface(images.Bus12);
        SDL_FreeSurface(images.Bus13);
        SDL_FreeSurface(images.Bus14);
        SDL_FreeSurface(images.Bus15);
        SDL_FreeSurface(images.Bus16);
        SDL_FreeSurface(images.Bus17);
        SDL_FreeSurface(images.Money);
        SDL_FreeSurface(images.BgGainsFlou);
        SDL_FreeSurface(images.BackgroundBus);

        for (int i = 0; i < 10; i++) {
            if (tabImages[i] != NULL) {
                SDL_FreeSurface(tabImages[i]);
                tabImages[i] = NULL;
            }
        }

        for (int i=0;i<18;i++){
            SDL_FreeSurface(tabImagesBus[i]);
        }

        for(int i=0;i<6;i++){
            SDL_FreeSurface(tabInfosBus);
        }

        return EXIT_SUCCESS;
    }
