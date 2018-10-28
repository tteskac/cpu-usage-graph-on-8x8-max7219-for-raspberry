#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <cstring>
#include <math.h>       /* pow */

#include <iostream>
using namespace std;

#include "Max7219/Max7219.h"

void initialise();
void update();
int get_cpu_usage();
void shift(int newValue);
void updateDisplay();
int map(int x, int in_min, int in_max, int out_min, int out_max);

// Init matrix display on pins:
// DATA/DIN:    0 - GPIO 17 (WiringPi pin num 0) header pin 11
// CLOCK/CLK:   3 - GPIO 22 (WiringPi pin num 3) header pin 15
// LOAD/CS:     4 - GPIO 23 (WiringPi pin num 4) header pin 16
Max7219 display(0,3,4); 

int main(int argc, char* argv[]) {

	initialise();
    
    display.SetBrightness(0); // valid 0-15
    //display.Send(1, 1);

	if (argc < 2)
	{
		cout << "ERROR: Enter delay, e.g. 500 for 500ms" << endl;
	    return 1;
	}

	int x = atoi(argv[1]);

	while(1) {
		int cpu_usage = get_cpu_usage();
		//cout << "CPU: " << cpu_usage << "%" << endl;
        shift(cpu_usage);
        updateDisplay();
        delay(x);
	}	      
	
	return 0 ;
}


int history[8] = {0,0,0,0,0,0,0,0};
void shift(int newValue)
{
    for (int i=0; i<7; i++)
    {
        history[i] = history[i+1];
    }
    history[7] = map(newValue, 0, 100, 0, 9);
}

void updateDisplay()
{
    for (int i=0; i<8; i++)
    {
        display.Send(i+1, pow(2, history[i])-1);
    }
}

// Helper
int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



/*
    CPU history based on:
    https://www.raspberrypi.org/forums/viewtopic.php?t=18964
*/

void initialise()
{
    // Verify we are root
    if(geteuid() != 0)
    {
        printf("This program must run as root\n");
        exit(0);
    }

    if(wiringPiSetup() == -1)
    {
        printf("wiringPiSetup() failed!\n");
        exit(1);
    }

    // printf("Initialise GPIO pins...\n");

    // // Initalise all LED's and default them off
    // for(int i=0; i < number_leds; i++)
    // {
    //     pinMode(i, OUTPUT);
    //     digitalWrite(i, 0);
    // }
}

// void update()
// {
//     // Disable all LED's

//     for(int i=0; i < number_leds; i++)
//         digitalWrite(i, 0);

//     int cpu_usage = get_cpu_usage();
//     int leds_lit = cpu_usage/(100/(number_leds+1));

//     for(int i=0; i < leds_lit; i++)
//         digitalWrite(i, 1);
// }

int previous_tjif = 0;
int previous_ijif = 0;

// Return value 0-100
int get_cpu_usage()
{
    char *buf = (char*)malloc(48);

    // Read the cpu status from /proc/stat
    FILE *fp = fopen("/proc/stat", "r");
    fread(buf, sizeof(char), 48, fp);
    fclose(fp);

    // Strip the prefixed "cpu  "
    char *cpuline = (char*)malloc(48);
    for(unsigned int i=5; i<strlen(buf); i++)
        cpuline[i-5] = buf[i];

    cpuline[40] = '\0';

    // Parse the proc/stat information into seperate jiffie containers
    int user_jif, nice_jif, system_jif, idle_jif;
    int iowait_jif;

    char *tokbuf = strtok(cpuline, " ");
    user_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    nice_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    system_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    idle_jif = atoi(tokbuf);
    tokbuf = strtok(NULL, " ");
    iowait_jif = atoi(tokbuf);

    // Get the workload from that
    int tjif = user_jif + nice_jif + system_jif + idle_jif + iowait_jif;

    int delta_total = tjif - previous_tjif;
    int delta_idle = idle_jif - previous_ijif;
    int delta_usage = (1000*(delta_total-delta_idle)/(delta_total+5))/10;
    previous_tjif = tjif;
    previous_ijif = idle_jif;

    free(cpuline);
    free(buf);

    return delta_usage;
}