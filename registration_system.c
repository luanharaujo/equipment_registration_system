#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRINGS_LEN 50

enum menu_1{Equipamens, All_Alarms, On_Alarms, Exit};
enum menu_1_12{See, Add, Remove, Back};
enum menu_1_3{On_Off, Description, Rating, Search, Most_3, Back2};
enum order_by{By_description, By_rating, By_number_of_activations};

// struct to store equipments informations, it will be implemented as a linked list
typedef struct equipment
{
    int id; // id to association whit the Alarms 
    char name[MAX_STRINGS_LEN];
    char serial_number[MAX_STRINGS_LEN];
    char type[8];// Voltage, Current or Oil
    char registration_date[11];// mm-dd-yyyy
    struct equipment *next;
} equipment;

// struct to store alarm informations, it will be implemented as a linked list
typedef struct alarm
{
    int id; // necessary to the menus
    int equipment_id; // id of the associated Equipament
    char description[MAX_STRINGS_LEN];
    char rating[7];// Low, Medium or Hight
    char registration_date[11];// mm-dd-yyyy
    char in_date[11];// mm-dd-yyyy
    char out_date[11];// mm-dd-yyyy
    int activated;// 0 for inactivated and 1 for activated
    int actions_count;
    struct alarm *next;
} alarm;

// global vareables
int last_id = 0;
int last_alarm_id = 0;

// prototypes
void load_files(equipment *equipments, alarm *alarms);
void save_equipments(equipment *equipments);
void save_alarms(alarm *alarms);
enum menu_1 display_menu_1(void);
void free_equipments(equipment *equipments);
void free_alarms(alarm *alarms);
void crud_equipments(equipment *equipments, alarm *alarms);
void crud_alarms(alarm *alarms, equipment *equipments);
void manage_alarms(equipment *equipments, alarm *alarms);
void add_equipament(equipment *equipments);
void show_equipament(equipment *equipments);
void remove_equipament(equipment *equipments, alarm *alarms);
void add_alarm(alarm *alarms, equipment *equipments);
void show_alarm(alarm *alarms);
void remove_alarm(alarm *alarms);
enum menu_1_12 display_menu_1_1(void);
enum menu_1_12 display_menu_1_2(void);
enum menu_1_3 display_menu_1_3(void);
int id_exist(int id, equipment *equipments);
void toggle(alarm *alarms);
void show_alarms_with_equipment(alarm *alarms, equipment *equipamens);
int before(alarm *alarm1, alarm *alarm2, enum order_by opt);
alarm *order_alarms(alarm *alarms, enum order_by opt);

int main()
{
    // init linked lists
    equipment equipments;
    equipments.next = NULL;
    
    alarm alarms;
    alarms.next = NULL;

    // triing to load pre-existing files
    load_files(&equipments, &alarms);

    // only closes if requested by the user through the menu
    while (1) 
    {
        switch (display_menu_1())
        {
            case Equipamens:
                    crud_equipments(&equipments, &alarms);
                break;
            case All_Alarms:
                    crud_alarms(&alarms, &equipments);
                break; 
            case On_Alarms:
                    manage_alarms(&equipments, &alarms);
                break;
            case Exit:
                //saving progress, cleaning up and leaving
                free_equipments(equipments.next);
                free_alarms(alarms.next);
                return 0;

            default:
                printf("Ivalid Option, press <enter> to continue");
                getchar();
                break;
        }

    }

}

void free_equipments(equipment *equipments)
{
    if (equipments == NULL)
        return;
    free_equipments(equipments->next);
    free(equipments);
}

void free_alarms(alarm *alarms)
{
    if (alarms == NULL)
        return;
    free_alarms(alarms->next);
    free(alarms);
}

// display the firth menu and returns the user answer
enum menu_1 display_menu_1(void)
{
    int ans;

    system("clear");
    printf("%d - See, Add or Remove Equipments\n", Equipamens);
    printf("%d - See, Add or Remove Alarms\n", All_Alarms);
    printf("%d - Manege Alarms\n", On_Alarms);
    printf("%d - Exit\n", Exit);
    printf("Option: ");
    scanf("%d", &ans);
    getchar();
    
    return ans;
}

// CRUD menu to the equipments
enum menu_1_12 display_menu_1_1(void)
{
    int ans;

    system("clear");
    printf("%d - See all equipments\n", See);
    printf("%d - Add new Equipament\n", Add);
    printf("%d - Remove Equipament\n", Remove);
    printf("%d - Return\n", Back);
    printf("Option: ");
    scanf("%d", &ans);
    getchar();
    
    return ans;
}

void add_equipament(equipment *equipments)
{
    // adding to the beggining of the linked list
    equipment *tmp = malloc(sizeof(equipment));
    tmp->next = equipments->next;
    equipments->next = tmp;
    tmp->id = ++last_id;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tmp->registration_date,"%02d-%02d-%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
    
    system("clear");
    
    printf("equipment name: ");
    scanf("%[^\n]", tmp->name);
    getchar();

    printf("equipment serial number: ");
    scanf("%[^\n]", tmp->serial_number);
    getchar();

    int ans;
    do
    {
        printf("\n0 - Voltage\n");
        printf("1 - Current\n");
        printf("2 - Oil\n");
        printf("equipment type(0/1/2): ");
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

    save_equipments(equipments);
    system("clear");
    printf("New equipment saved, press <Enter> to continue\n");
    getchar();
}

void show_equipament(equipment *equipments)
{
    system("clear");
    while (equipments->next)
    {
        equipments = equipments->next;
        printf("equipment %d: \n", equipments->id);
        printf("\tName:              %s\n", equipments->name);
        printf("\tSerial number:     %s\n", equipments->serial_number);
        printf("\tType:              %s\n", equipments->type);
        printf("\tRegistration date: %s\n\n", equipments->registration_date);
    }

}

void remove_equipament(equipment *equipments, alarm *alarms)
{
    int n;
    printf("Number of equipment to remove: ");
    scanf("%d", &n);
    getchar();

    equipment *aux = equipments;
    
    while (aux->next)
    {
        if (aux->next->id == n)
            break;
        aux = aux->next;
    }
    
    if (!aux->next)
    {
        printf("Equipment not found\n");
        printf("Press <enter> to return");
        getchar();
    }
    else
    {
        
        alarm *aux3 = alarms;
        while (aux3->next)
        { 
            if (aux3->next->equipment_id == n)
            {
                alarm *aux4 = aux3->next;
                aux3->next = aux3->next->next;
                free(aux4);
            }
                     
            aux3 = aux3->next;
            if (!aux3)
                break;    
        }

        equipment *aux2 = aux->next;
        aux->next = aux->next->next;
        free(aux2);
       
        save_alarms(alarms);
        
        save_equipments(equipments);
        
        printf("Equipment and related alarms successfully deleted. New list saved to file.\n");
        printf("Press <enter> to return");
        getchar();
    }
}

// display and implemented options for the user manege the equipment list
void crud_equipments(equipment *equipments, alarm *alarms)
{
    while (1)
    {
        switch (display_menu_1_1())
        {
            case See:
                show_equipament(equipments);
                printf("\n\nPress <enter> to return.");
                getchar();
                break;
            case Add:
                add_equipament(equipments);
                break; 
            case Remove:
                show_equipament(equipments);
                remove_equipament(equipments, alarms);
                break;
            case Back:
                return;
            default:
                printf("Ivalid Option, press <enter> to return");
                getchar();
                break;
        }
    }
}

// return 1 if there is a equipment with maches the id
// returns 0 otherwise 
int id_exist(int id, equipment *equipments)
{
    while(equipments->next)
    {
        equipments = equipments->next;
        if (equipments->id == id)
            return 1;
    }
    return 0;
}

void add_alarm(alarm *alarms, equipment *equipments)
{
    // adding to the beggining of the linked list
    alarm *tmp = malloc(sizeof(alarm));
    tmp->next = alarms->next;
    alarms->next = tmp;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tmp->registration_date,"%02d-%02d-%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);

    strcpy(tmp->in_date,"Never");
    strcpy(tmp->out_date,"Never");
    tmp->actions_count = 0;
    tmp->activated = 0;
    tmp->id = ++last_alarm_id;
    
    // showing equipments to chose
    int ans;
    do
    {
        show_equipament(equipments);
        printf("Choose the number of the equipment with which this alarm will be related: ");
        scanf("%d", &ans);
        getchar();
        if (!id_exist(ans, equipments))
        {
            printf("\nInvalid answer!\n");
            printf("Press <enter> to answer again.\n");
            getchar();
        }
    } while (!id_exist(ans, equipments));

    tmp->equipment_id = ans;

    printf("Alarm description: ");
    scanf("%[^\n]", tmp->description);
    getchar();

    do
    {
        printf("\n0 - Low\n");
        printf("1 - Medium\n");
        printf("2 - Hight\n");
        printf("Alarm rating(0/1/2): ");
        scanf("%d", &ans);
        getchar();
        if (ans < 0 || ans > 2)
            printf("\nInvalid answer!\n");
    } while (ans < 0 || ans > 2);

    switch (ans)
    {
        case 0:
            strcpy(tmp->rating, "Low");
            break;
        case 1:
            strcpy(tmp->rating, "Medium");
            break; 
        case 2:
            strcpy(tmp->rating, "Hight");
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
        printf("Alarm  %d: \n", alarms->id);
        if (alarms->activated)
            printf("\tACTIVATED\n");
        else
            printf("\tINACTIVATED\n");  
        printf("\tDescription:          %s\n", alarms->description);
        printf("\tRating:               %s\n", alarms->rating);
        printf("\tRegistration Date:    %s\n", alarms->registration_date);
        printf("\tIn Date:              %s\n", alarms->in_date);
        printf("\tOut Date:             %s\n", alarms->out_date);
        printf("This alarm is related the equipament %d\n", alarms->equipment_id);
        printf("This alarm was activated %d times\n\n", alarms->actions_count);
    }
}

void remove_alarm(alarm *alarms)
{
    int n;
    printf("Number of Alarm to remove: ");
    scanf("%d", &n);
    getchar();

    alarm *aux = alarms;

    while (aux->next)
    {
        if (aux->next->id == n)
            break;
        aux = aux->next;
    }

    if (!aux->next)
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

// CRUD menu to the alarms
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

// display and implemented options for the user manege the alarm list
void crud_alarms(alarm *alarms, equipment *equipments)
{
    while (1)
    {
        switch (display_menu_1_2())
        {
            case See:
                show_alarm(alarms);
                printf("\n\nPress <enter> to return.");
                getchar();
                break;
            case Add:
                add_alarm(alarms, equipments);
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
}


// if the data files exist load then in hte liked list,
// and inform the user if loaded or not
void load_files(equipment *equipments, alarm *alarms)
{
    FILE *fp = fopen("equipments.txt", "r");

    system("clear");

    if(fp == NULL)
    {
        printf("No equipments.txt file found, the programa will proced with no equipments.\n");
    }
    else
    {
        // loading equipments from file
        int id;
        char name[MAX_STRINGS_LEN], serial[MAX_STRINGS_LEN], type[8], date[11];
        int n_equipments = 0;

        fscanf(fp, "ID,NAME,SERIAL NUMBER,TYPE,REGISTRATION_DATE\n");
        while(fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^\n]\n", &id, name, serial, type, date) != EOF)
        {
            n_equipments++;
            equipments->next = malloc(sizeof(equipment));
            equipments = equipments->next;
            equipments->next = NULL;

            equipments->id = id;
            if(id > last_id)
                last_id = id;

            strcpy(equipments->name, name);
            strcpy(equipments->serial_number, serial);
            strcpy(equipments->type, type);
            strcpy(equipments->registration_date, date);
        }
        fclose(fp);
        printf("equipments.txt file successfully loaded. %d equipments found.\n", n_equipments);
    }


    fp = fopen("alarms.txt", "r");
    if(fp == NULL)
    {
        printf("No alarms.txt file found, the programa will proced with no alarms.\n");
    }
    else
    {
        // loading alarms from file
        int id, equipment_id, actions_count, activated;
        char description[MAX_STRINGS_LEN], rating[7], registration_date[11], in_date[11], out_date[11];
        int n_alarms = 0;

        fscanf(fp, "ID,EQUIPMENT ID,DESCRIPTION,RATING,REGISTRATION DATE,IN DATE,OUT DATE,ACTIVATED,ACTIONS COUNT\n");
        while(fscanf(fp, "%d,%d,%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d\n", &id, &equipment_id, description, rating, registration_date, in_date, out_date, &activated, &actions_count) != EOF)
        {
            n_alarms++;
            alarms->next = malloc(sizeof(alarm));
            alarms = alarms->next;
            alarms->next = NULL;

            alarms->id = id;
            if(id > last_alarm_id)
                last_alarm_id = id;

            alarms->equipment_id = equipment_id;
            alarms->actions_count = actions_count;
            alarms->activated = activated;

            strcpy(alarms->description, description);
            strcpy(alarms->rating, rating);
            strcpy(alarms->registration_date, registration_date);
            strcpy(alarms->in_date, in_date);
            strcpy(alarms->out_date, out_date);
        }
        fclose(fp);
        printf("alarms.txt file successfully loaded. %d alarms found.\n", n_alarms);
    }

    printf("Press <enter> to continue");
    getchar();
}

// save the corrent data in the equipments.txt file
void save_equipments(equipment *equipments)
{
    FILE *fp = fopen("equipments.txt", "w");

    fprintf(fp, "ID,NAME,SERIAL NUMBER,TYPE,REGISTRATION_DATE\n");
    
    while (equipments->next)
    {
        equipments = equipments->next;

        fprintf(fp, "%d,", equipments->id);
        fprintf(fp, "%s,", equipments->name);
        fprintf(fp, "%s,", equipments->serial_number);
        fprintf(fp, "%s,", equipments->type);
        fprintf(fp, "%s\n", equipments->registration_date);
    }

    fclose(fp);
}

// save the corrent data in the alarms.txt file
void save_alarms(alarm *alarms)
{
    FILE *fp = fopen("alarms.txt", "w");

    fprintf(fp, "ID,EQUIPMENT ID,DESCRIPTION,RATING,REGISTRATION DATE,IN DATE,OUT DATE,ACTIVATED,ACTIONS COUNT\n");
    
    while (alarms->next)
    {
        alarms = alarms->next;

        fprintf(fp, "%d,", alarms->id);
        fprintf(fp, "%d,", alarms->equipment_id);
        fprintf(fp, "%s,", alarms->description);
        fprintf(fp, "%s,", alarms->rating);
        fprintf(fp, "%s,", alarms->registration_date);
        fprintf(fp, "%s,", alarms->in_date);
        fprintf(fp, "%s,", alarms->out_date);
        fprintf(fp, "%d,", alarms->activated);
        fprintf(fp, "%d\n", alarms->actions_count);
    }

    fclose(fp);
}

enum menu_1_3 display_menu_1_3(void)
{
    int ans;

    system("clear");
    printf("%d - Enable and disable alarms\n", On_Off);
    printf("%d - See all alarms by description\n", Description);
    printf("%d - View all alarms by rating\n", Rating);
    printf("%d - Search alarm\n", Search);
    printf("%d - See 3 most frequent alarms\n", Most_3);
    printf("%d - Return\n", Back2);
    printf("Option: ");
    scanf("%d", &ans);
    getchar();
    
    return ans;
}

// Order the alarms linked list by the opt criterion.
// opt is By_description, By_rating or By_number_of_activations
// the algoritmic implemented is the Merge Sort
alarm *order_alarms(alarm *start, enum order_by opt)
{
    //basic cases
    if (start == NULL)
        return NULL;
    if (start->next == NULL)
        return start;

    // using the Rabbit and Turtle Algorithm to divide the list
    alarm *turtle = start;
    alarm *rabbit = start;

    while(rabbit->next && rabbit->next->next)
    {
        rabbit = rabbit->next->next;
        turtle = turtle->next;
    }

    alarm *middle = turtle->next;
    turtle->next = NULL;

    middle = order_alarms(middle, opt);
    start = order_alarms(start, opt);

    //marging
    alarm *sorted;
    if (before(start, middle, opt))
    {
        sorted = start;
        if (start)
           start = start->next; 
    }    
    else
    {
        sorted = middle;
        if (middle)
           middle = middle->next;
    }

    alarm *tmp = sorted;

    while(start || middle)
    {
        if (before(start, middle, opt))
        {
            tmp->next = start;
            if (start)
                start = start->next; 
        }    
        else
        {
            tmp->next = middle;
            if (middle)
                middle = middle->next;
        }
        tmp = tmp->next;
    }
        
    return sorted;    
}

// Returns 1 if alarm 1 comes before alarm 2 according to the opt criterion.
// Returns 0 otherwise.
// opt is By_description, By_rating or By_number_of_activations
int before(alarm *alarm1, alarm *alarm2, enum order_by opt)
{
    if (!alarm1)
        return 0;
    
    if (!alarm2)
        return 1;

    switch (opt)
    {
        case By_description:
            if (strcmp(alarm1->description, alarm2->description) < 0)
                return 1;
            break;
            
        case By_rating:
            if (strcasecmp(alarm1->rating, "Hight") == 0)
                return 1;
            if (strcasecmp(alarm2->rating, "Low") == 0)
                return 1;
            break;
        
        case By_number_of_activations:
            if (alarm1->actions_count > alarm2->actions_count)
                return 1;
            break;
    }

    return 0;
}

// display and implemented options for the user see, filter an change the Alarms
void manage_alarms(equipment *equipmens, alarm *alarms)
{
    while(1)
    {
        switch (display_menu_1_3())
        {
            case On_Off:
                toggle(alarms);
                break;
            case Description:
                alarms->next = order_alarms(alarms->next, By_description);
                show_alarms_with_equipment(alarms, equipmens);
                break; 
            case Rating:
                alarms->next = order_alarms(alarms->next, By_rating);
                show_alarms_with_equipment(alarms, equipmens);
                break;
            case Search:
                // TO DO
                break;
            case Most_3:
                alarms->next = order_alarms(alarms->next, By_number_of_activations);
                // TO DO
                break;
            case Back2:
                return;
            default:
                printf("Ivalid Option, press <enter> to return");
                getchar();
                break;
        }
    }
}

//shows alarms with their respective equipment in the linked list order
void show_alarms_with_equipment(alarm *alarms, equipment *equipmens)
{
    system("clear");
    while(alarms->next)
    {
        // print alarm data
        alarms = alarms->next;
        printf("Alarm  %d: \n", alarms->id);
        if (alarms->activated)
            printf("\tACTIVATED\n");
        else
            printf("\tINACTIVATED\n");  
        printf("\tDescription:          %s\n", alarms->description);
        printf("\tRating:               %s\n", alarms->rating);
        printf("\tRegistration Date:    %s\n", alarms->registration_date);
        printf("\tIn Date:              %s\n", alarms->in_date);
        printf("\tOut Date:             %s\n", alarms->out_date);
        printf("\tThis alarm was activated %d times\n\n", alarms->actions_count);
        printf("\tThis alarm is related the equipament %d:\n", alarms->equipment_id);

        // find equipment data
        equipment *related = equipmens->next;
        while (related->id != alarms->equipment_id)
            related = related->next;
        
        // print equipment data
        printf("\tEquipment data:\n");
        printf("\t\tName:              %s\n", related->name);
        printf("\t\tSerial number:     %s\n", related->serial_number);
        printf("\t\tType:              %s\n", related->type);
        printf("\t\tRegistration date: %s\n\n", related->registration_date);
    }
    
    printf("Press <enter> to return.\n");
    getchar();
}

void toggle(alarm *alarms)
{
    int ans;

    show_alarm(alarms);
    printf("Which alarm do you want to toggle: ");
    scanf("%d", &ans);
    getchar();

    alarm *aux = alarms;

    while (aux->next)
    {
        aux = aux->next;
        if (aux->id == ans)
        {
            aux->activated = !aux->activated;
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);

            if (aux->activated)
            {
                aux->actions_count++;
                sprintf(aux->in_date,"%02d-%02d-%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
            }
            else
            {
                sprintf(aux->out_date,"%02d-%02d-%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
            }
            save_alarms(alarms);
            break;
        }
    }

    if (aux->id != ans)
    {
        printf("\nAlarm not found, press <enter> to return.\n");
    }
    else
    {
        printf("\nAlarms updated and save in file.\n");
    }
    printf("Press <enter> to continue");
    getchar();
}