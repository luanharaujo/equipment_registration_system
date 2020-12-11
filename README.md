# Equipment Registration System

## Description

This is a system for recording equipment and alerts related to them. For more details see the [specification file]('./../specification_files/Firmware%20-%20Especificação%20Prova%20Prática%20Programador.pdf).

## Prerequisites

This program was developed in a Ubuntu - 20.04.1 LTS running on a WSL - V2 inside a Windows 10, using has a compiler the gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0. But **probably works on any Unix-based system with a modern GCC**.

## Compilation

```bash
gcc registration_system.c -o registration_system
```
## Call

```bash
./registration_system
```

## Usage

The text files are in csv format and can be filled in manually to speed up the program testing process, however the guidelines in the section **Observations** must be followed.

Two examples of data from text files are shown below.

#### equipments.txt
```txt
ID,NAME,SERIAL NUMBER,TYPE,REGISTRATION_DATE
1,Sensor 1,123abc,Voltage,12-08-2020
2,Sensor 2,123abc,Oil,12-08-2020
3,Sensor 3,123abc,Current,12-09-2020
4,Sensor 4,123abc,Voltage,12-09-2020
5,Sensor 5,123abc,Oil,12-10-2020
```
#### alarms.txt
```txt
ID,EQUIPMENT ID,DESCRIPTION,RATING,REGISTRATION DATE,IN DATE,OUT DATE,ACTIVATED,ACTIONS COUNT
1,1,over heating,Medium,12-10-2020,12-09-2020,12-09-2020,1,7
2,2,leaking,High,12-10-2020,12-08-2020,Never,1,1
3,3,disconnect,Low,12-10-2020,Never,Never,0,0
```

### Main Menu

The main menu has 4 options as shown below. The first two (options 0 and 1) are for viewing and removing, equipment and alarms, respectively. The third (option 2) is to manage the system, where you can activate and deactivate the alarms, and where you have all forms of visualization. This sub menu is where the user will stay most of the time during the operation. The last one (option 3) is for closing the program.

```txt
0 - See, Add or Remove Equipments
1 - See, Add or Remove Alarms
2 - Manege Alarms
3 - Exit
```
Each of the submenus will be executed as many times as necessary and will only be returned to the main menu if requested by the user through the last option of each menu.

### Menus 0 and 1

These menus are very basic, each one has an option to view, add and delete their respective data (Equipment and Alarms). 

#### Menu 0
```txt
0 - See all equipments
1 - Add new Equipment
2 - Remove Equipment
3 - Return
```
#### Menu 1
```txt
0 - See all Alarms
1 - Add new Alarm
2 - Remove Alarm
3 - Return
```
It is worth mentioning that the option to "See all Alarms" in sub-menu 1, does not have important information, such as the name of the related equipment, as this option is only to assist in data management and not to be used for viewing and managing Alarms.

It is also important to know that once a data addition is started, it is not possible to cancel it. The options in that case are to finish entering the data and then delete it, or interrupt the program (Ctrl + C), since the text files are updated with each change of data, this will not cause loss of additional data.

### Menu 2

This is the most important menu of the program and each of its options is described below.

```txt
0 - Enable and disable alarms
1 - See activated alarms by description
2 - See activated alarms by rating
3 - Search alarm
4 - See 3 most frequent alarms
5 - Return
```
<ol start="0">
  <li>This option shows all alarms (activated and deactivated) so  that the user can choose which alarm to change. The chosen alarm will be changed from enabled to disabled or the opposite, depending on what state it was originally in.</li>
  <li>This option will show only active alarms with their respective equipment in asciibecal order of the description field.</li>
  <li>This option will show only active alarms with their respective equipment in order of rating. In order: High, Medium and Low.</li>
  <li>This option will ask for a word or phrase to be searched, and will show all alarms (activated and deactivated) that contain this word in its description field with their respective equipment. It is important to emphasize that the search is case sensitive.</li>
  <li>It will sort the alarms from the most frequent to the least frequent, and will show the top three of the list with their respective equipment. <b>Tie</b> cases were not treated and depended on previous use of the program, so it can be considered random.</li>
</ol>

## Observations

* The maximum length of any string was defined as 50 chars, if that is not enough that can be changed in the `#define MAX_STRINGS_LEN 50`
  
* Since the text files are formatted as csv, there should be no commas in any of the fields. 
  
* It is assumed that all alarms are linked to existing equipment, which will be the case if the text files are not changed manually. Failure to comply with this condition may result in a program crash.

* If the files are filled in manually, the first column of each file (ID) must be composed of distinct integers.   

## Possible problems and improvements

* The text files are completely rewritten with each change in the data. This should not be a problem for an amount of equipment / sensors in the order of tens of thousands (or even hundreds of thousands, depending on the computer). If the amount of data is much larger than this, is suggested the changing to a proper database.
  
* The program's interface, although functional, is very inefficient, especially in data entry. If the data already exists, it is suggested to implement a routine to import such data.
