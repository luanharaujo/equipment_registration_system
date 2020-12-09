#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRINGS_LEN 50

enum menu_1{Equipamens, All_alerts, On_alerts, Exit};
enum menu_1_12{See, Add, Remove, Back};

// struct to store equipaments informations, it will be implemented as a linked list
typedef struct equipament
{
    int id; // id to association whit the Alerts 
    char name[MAX_STRINGS_LEN];
    char serial_number[MAX_STRINGS_LEN];
    char type[8];// Voltage, Current or Oil
    char registration_date[11];// mm-dd-yyyy
    struct equipament *next;
} equipament;

// struct to store alarm informations, it will be implemented as a linked list
typedef struct alarm
{
    int id; //necessary to the menus
    int equipament_id; // id of the associated Equipament
    char description[MAX_STRINGS_LEN];
    char classification[7];// Low, Medium or Hight
    char registration_date[11];// mm-dd-yyyy
    char in_date[11];// mm-dd-yyyy
    char out_date[11];// mm-dd-yyyy
    int actions_count;
    struct alarm *next;
} alarm;

// global vareables
int last_id = 0;
int last_alarm_id = 0;

// prototypes
void load_files(equipament *equipaments, alarm *alarms);
void save_equipaments(equipament *equipaments);
void save_alarms(alarm *alarms);
enum menu_1 display_menu_1(void);
void free_equipaments(equipament *equipaments);
void free_alarms(alarm *alarms);
void crud_equipaments(equipament *equipaments);
void crud_alarms(alarm *alarms, equipament *equipaments);
void manage_alarms(alarm *alarms);
void add_equipament(equipament *equipaments);
void show_equipament(equipament *equipaments);
void remove_equipament(equipament *equipaments);
void add_alarm(alarm *alarms, equipament *equipaments);
void show_alarm(alarm *alarms);
void remove_alarm(alarm *alarms);
enum menu_1_12 display_menu_1_1(void);
enum menu_1_12 display_menu_1_2(void);
int id_exist(int id, equipament *equipaments);

int main()
{
    // init linked lists
    equipament equipaments;
    equipaments.next = NULL;
    
    alarm alarms;
    alarms.next = NULL;

    // triing to load pre-existing files
    load_files(&equipaments, &alarms);

    // only closes if requested by the user through the menu
    while (1) 
    {
        switch (display_menu_1())
        {
            case Equipamens:
                    crud_equipaments(&equipaments);
                break;
            case All_alerts:
                    crud_alarms(&alarms, &equipaments);
                break; 
            case On_alerts:
                    manage_alarms(&alarms);
                break;
            case Exit:
                //saving progress, cleaning up and leaving
                free_equipaments(equipaments.next);
                free_alarms(alarms.next);
                return 0;

            default:
                printf("Ivalid Option, press <enter> to continue");
                getchar();
                break;
        }

    }

}

void free_equipaments(equipament *equipaments)
{
    if (equipaments == NULL)
        return;
    free_equipaments(equipaments->next);
    free(equipaments);
}

void free_alarms(alarm *alarms)
{
    if (alarms == NULL)
        return;
    free_alarms(alarms->next);
    free(alarms);
}

//display the firth menu and returns the user answer
enum menu_1 display_menu_1(void)
{
    int ans;

    system("clear");
    printf("%d - See, Add or Remove Equipaments\n", Equipamens);
    printf("%d - See, Add or Remove Alerts\n", All_alerts);
    printf("%d - Manege Alerts\n", On_alerts);
    printf("%d - Exite\n", Exit);
    printf("Option: ");
    scanf("%d", &ans);
    getchar();
    
    return ans;
}

//CRUD menu to the equipaments
enum menu_1_12 display_menu_1_1(void)
{
    int ans;

    system("clear");
    printf("%d - See all Equipaments\n", See);
    printf("%d - Add new Equipament\n", Add);
    printf("%d - Remove Equipament\n", Remove);
    printf("%d - Return\n", Back);
    printf("Option: ");
    scanf("%d", &ans);
    getchar();
    
    return ans;
}

void add_equipament(equipament *equipaments)
{
    //adding to the beggining of the linked list
    equipament *tmp = malloc(sizeof(equipament));
    tmp->next = equipaments->next;
    equipaments->next = tmp;
    tmp->id = ++last_id;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tmp->registration_date,"%02d-%02d-%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
    
    system("clear");
    
    printf("Equipament name: ");
    scanf("%[^\n]", tmp->name);
    getchar();

    printf("Equipament serial number: ");
    scanf("%[^\n]", tmp->serial_number);
    getchar();

    int ans;
    do
    {
        printf("\n0 - Voltage\n");
        printf("1 - Current\n");
        printf("2 - Oil\n");
        printf("Equipament type(0/1/2): ");
        scanf("%d", &ans);
        getchar();
        if (ans < 0 || ans > 2)
            printf("\nInvalid answer!\n");
    } while (ans < 0 || ans > 2);

    switch (ans)
    {
        case 0:
            strcpy(tmp->type, "Voltage");
            break;
        case 1:
            strcpy(tmp->type, "Current");
            break; 
        case 2:
            strcpy(tmp->type, "Oil");
            break;
    }

    save_equipaments(equipaments);
    system("clear");
    printf("New equipament saved, press <Enter> to continue\n");
    getchar();
}

void show_equipament(equipament *equipaments)
{
    system("clear");
    while (equipaments->next)
    {
        equipaments = equipaments->next;
        printf("Equipament %d: \n", equipaments->id);
        printf("\tName:              %s\n", equipaments->name);
        printf("\tSerial number:     %s\n", equipaments->serial_number);
        printf("\tType:              %s\n", equipaments->type);
        printf("\tRegistration date: %s\n\n", equipaments->registration_date);
    }

}

void remove_equipament(equipament *equipaments)
{
    int n;
    printf("Number of Equipament to remove: ");
    scanf("%d", &n);
    getchar();

    equipament *aux = equipaments;
    
    while (aux->next)
    {
        aux = aux->next;
        if (aux->id == n)
            break;
    }

    if (aux->id != n)
    {
        printf("Equipment not found\n");
        printf("Press <enter> to return");
        getchar();
    }
    else
    {
        // TO DO: delete related alarms
        equipament *aux2 = aux->next;
        aux->next = aux->next->next;
        free(aux2);

        save_equipaments(equipaments);

        printf("Equipment successfully deleted. New list saved to file.\n");
        printf("Press <enter> to return");
        getchar();
    }
}

//display and implemented options for the user manege the equipament list
void crud_equipaments(equipament *equipaments)
{
    switch (display_menu_1_1())
        {
            case See:
                show_equipament(equipaments);
                printf("\n\nPress <enter> to return.");
                getchar();
                break;
            case Add:
                add_equipament(equipaments);
                break; 
            case Remove:
                show_equipament(equipaments);
                remove_equipament(equipaments);
                break;
            case Back:
                return;
            default:
                printf("Ivalid Option, press <enter> to return");
                getchar();
                break;
        }
}

//return 1 if there is a equipament with maches the id
//returns 0 otherwise 
int id_exist(int id, equipament *equipaments)
{
    while(equipaments->next)
    {
        equipaments = equipaments->next;
        if (equipaments->id == id)
            return 1;
    }
    return 0;
}

void add_alarm(alarm *alarms, equipament *equipaments)
{
    //adding to the beggining of the linked list
    alarm *tmp = malloc(sizeof(alarm));
    tmp->next = alarms->next;
    alarms->next = tmp;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tmp->registration_date,"%02d-%02d-%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);

    strcpy(tmp->in_date,"Never");
    strcpy(tmp->out_date,"Never");
    tmp->actions_count = 0;
    tmp->id = ++last_alarm_id;
    
    //showing equipaments to chose
    int ans;
    do
    {
        show_equipament(equipaments);
        printf("Choose the number of the equipment with which this alarm will be related: ");
        scanf("%d", &ans);
        getchar();
        if (!id_exist(ans, equipaments))
        {
            printf("\nInvalid answer!\n");
            printf("Press <enter> to answer again.\n");
            getchar();
        }
    } while (!id_exist(ans, equipaments));

    tmp->equipament_id = ans;

    printf("Alarm description: ");
    scanf("%[^\n]", tmp->description);
    getchar();

    do
    {
        printf("\n0 - Low\n");
        printf("1 - Medium\n");
        printf("2 - Hight\n");
        printf("Alarm classification(0/1/2): ");
        scanf("%d", &ans);
        getchar();
        if (ans < 0 || ans > 2)
            printf("\nInvalid answer!\n");
    } while (ans < 0 || ans > 2);

    switch (ans)
    {
        case 0:
            strcpy(tmp->classification, "Low");
            break;
        case 1:
            strcpy(tmp->classification, "Medium");
            break; 
        case 2:
            strcpy(tmp->classification, "Hight");
            break;
    }

    save_alarms(alarms);
    system("clear");
    printf("New alarm saved, press <Enter> to continue\n");
    getchar();
}

void show_alarm(alarm *alarms)
{
    system("clear");
    while (alarms->next)
    {
        alarms = alarms->next;
        printf("Alarme  %d: \n", alarms->id);
        printf("\tDescription:       %s\n", alarms->description);
        printf("\tClassification:    %s\n", alarms->classification);
        printf("\tRegistration Date: %s\n", alarms->registration_date);
        printf("\tIn Date:           %s\n", alarms->in_date);
        printf("\tOut Date:          %s\n", alarms->out_date);
        printf("\tThis alarm is related the %d equipament", alarms->equipament_id);
        printf("\tThis alarm was activated %d times\n\n", alarms->actions_count);
    }
}

void remove_alarm(alarm *alarms)
{
    int n;
    printf("Number of Alarme to remove: ");
    scanf("%d", &n);
    getchar();

    alarm *aux = alarms;

    while (aux->next)
    {
        aux = aux->next;
        if (aux->id == n)
            break;
    }

    if (aux->id != n)
    {
        printf("Alarm not found\n");
        printf("Press <enter> to return");
        getchar();
    }
    else
    {
        alarm *aux2 = aux->next;
        aux->next = aux->next->next;
        free(aux2);

        save_alarms(alarms);

        printf("Alarm successfully deleted. New list saved to file.\n");
        printf("Press <enter> to return");
        getchar();
    }
}

//CRUD menu to the alarms
enum menu_1_12 display_menu_1_2(void)
{
    int ans;

    system("clear");
    printf("%d - See all Alarms\n", See);
    printf("%d - Add new Alarm\n", Add);
    printf("%d - Remove Alarm\n", Remove);
    printf("%d - Return\n", Back);
    printf("Option: ");
    scanf("%d", &ans);
    getchar();
    
    return ans;
}

//display and implemented options for the user manege the alarm list
void crud_alarms(alarm *alarms, equipament *equipaments)
{
    switch (display_menu_1_2())
        {
            case See:
                show_alarm(alarms);
                printf("\n\nPress <enter> to return.");
                getchar();
                break;
            case Add:
                add_alarm(alarms, equipaments);
                break; 
            case Remove:
                show_alarm(alarms);
                remove_alarm(alarms);
                break;
            case Back:
                return;
            default:
                printf("Ivalid Option, press <enter> to return");
                getchar();
                break;
        }
}

//display and implemented options for the user see, filter an change the alerts
void manage_alarms(alarm *alarms)
{
    // TO DO
}

// if the data files exist load then in hte liked list,
// and inform the user if loaded or not
void load_files(equipament *equipaments, alarm *alarms)
{
    FILE *fp = fopen("equipaments.txt", "r");

    system("clear");

    if(fp == NULL)
    {
        printf("No equipaments.txt file found, the programa will proced with no equipaments.\n");
    }
    else
    {
        int id;
        char name[MAX_STRINGS_LEN], serial[MAX_STRINGS_LEN], type[MAX_STRINGS_LEN], date[MAX_STRINGS_LEN];

        fscanf(fp, "ID,NAME,SERIAL NUMBER,TYPE,REGISTRATION_DATE\n");
        while(fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^\n]\n", &id, name, serial, type, date) != EOF)
        {
            equipaments->next = malloc(sizeof(equipament));
            equipaments = equipaments->next;
            equipaments->next = NULL;

            equipaments->id = id;
            if(id > last_id)
                last_id = id;

            strcpy(equipaments->name, name);
            strcpy(equipaments->serial_number, serial);
            strcpy(equipaments->type, type);
            strcpy(equipaments->registration_date, date);
        }
        fclose(fp);
        printf("equipments.txt file successfully loaded.\n");
    }

    // TO DO: load the alarms.txt

    printf("Press <enter> to continue");
    getchar();
}

//save the corrent data in the equipaments.txt file
void save_equipaments(equipament *equipaments)
{
    FILE *fp = fopen("equipaments.txt", "w");

    fprintf(fp, "ID,NAME,SERIAL NUMBER,TYPE,REGISTRATION_DATE\n");
    
    while (equipaments->next)
    {
        equipaments = equipaments->next;

        fprintf(fp, "%d,", equipaments->id);
        fprintf(fp, "%s,", equipaments->name);
        fprintf(fp, "%s,", equipaments->serial_number);
        fprintf(fp, "%s,", equipaments->type);
        fprintf(fp, "%s\n", equipaments->registration_date);
    }

    fclose(fp);
}

//save the corrent data in the alarms.txt file
void save_alarms(alarm *alarms)
{
    FILE *fp = fopen("alarms.txt", "w");

    fprintf(fp, "ID,EQUIPMENT ID,DESCRIPTION,CLASSIFICATION,REGISTRATION DATE,IN DATE,OUT DATE,ACTIONS COUNT\n");
    
    while (alarms->next)
    {
        alarms = alarms->next;

        fprintf(fp, "%d,", alarms->id);
        fprintf(fp, "%d,", alarms->equipament_id);
        fprintf(fp, "%s,", alarms->description);
        fprintf(fp, "%s,", alarms->classification);
        fprintf(fp, "%s,", alarms->registration_date);
        fprintf(fp, "%s,", alarms->in_date);
        fprintf(fp, "%s,", alarms->out_date);
        fprintf(fp, "%d\n", alarms->actions_count);
    }

    fclose(fp);
}