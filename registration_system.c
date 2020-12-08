#include <stdio.h>
#include <stdlib.h>
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
    char classification[7];// Low, Medium, Hight
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
void save_files(equipament *equipaments, alarm *alarms);
enum menu_1 display_menu_1(void);
void free_equipaments(equipament *equipaments);
void free_alarms(alarm *alarms);
void crud_equipaments(equipament *equipaments);
void crud_alarms(alarm *alarms);
void manage_alarms(alarm *alarms);

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
                save_files(&equipaments, &alarms);
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

//display and implemented options for the user manege the equipament list
void crud_equipaments(equipament *equipaments)
{
    // TO DO
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

//save the corrent data in the equipaments.txt and alarms.txt files
void save_files(equipament *equipaments, alarm *alarms)
{
    // TO DO
}