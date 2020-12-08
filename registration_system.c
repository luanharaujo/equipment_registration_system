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
    int equipament_id; // id of the associated Equipament
    char description[MAX_STRINGS_LEN];
    char classification[7];// Low, Medium or Hight
    char registration_date[11];// mm-dd-yyyy
    char in_date[11];// mm-dd-yyyy
    char out_date[11];// mm-dd-yyyy
    int actions_cout;
    struct alarm *next;
} alarm;

// global vareables
int last_id = 0;

// prototypes
void load_files(equipament *equipaments, alarm *alarms);
void save_equipaments(equipament *equipaments);
void save_alarms(alarm *alarms);
enum menu_1 display_menu_1(void);
void free_equipaments(equipament *equipaments);
void free_alarms(alarm *alarms);
void crud_equipaments(equipament *equipaments);
void crud_alarms(alarm *alarms);
void manage_alarms(alarm *alarms);
void add_equipament(equipament *equipaments);
void show_equipament(equipament *equipaments);

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
                    crud_alarms(&alarms);
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

//display the firth menu and returns the user answer
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
            printf("\nResposta invalida!!\n");
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

    printf("\n\nPress <enter> to return.");
    getchar();
}

//display and implemented options for the user manege the equipament list
void crud_equipaments(equipament *equipaments)
{
    switch (display_menu_1_1())
        {
            case See:
                show_equipament(equipaments);
                break;
            case Add:
                add_equipament(equipaments);
                break; 
            case Remove:
                // TO DO
                break;
            case Back:
                return;
            default:
                printf("Ivalid Option, press <enter> to return");
                getchar();
                break;
        }
}

//display and implemented options for the user manege the alarm list
void crud_alarms(alarm *alarms)
{
    // TO DO
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
    // TO DO
}

//save the corrent data in the equipaments.txt file
void save_equipaments(equipament *equipaments)
{
    // TO DO
}

//save the corrent data in the alarms.txt file
void save_alarms(alarm *alarms)
{
    // TO DO
}