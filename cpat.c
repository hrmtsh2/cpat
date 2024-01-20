/* C Personal Attendance Tracker(cpat) v1.0.0

   Author - Amitesh Mahapatra */

//TODO: Implement subject-wise attendance functionality

#include <stdio.h>  // FILE, fopen, printf, scanf, sscanf
#include <stdlib.h> // exit
#include <string.h> // strncat, strcat, strcmp, strtok, strlen, strncmp
#include <time.h> // struct tm, time_t, time, localtime, asctime

#define MAXARGSIZE 100 // Maximum no. of characters in an argument to an option.
#define MAXLINESIZE 100 // Maximum no. of characters in a line of a file.
#define MAXFILESIZE 100 // Maximum no. of total characters in a file.

char timetablePath[MAXARGSIZE] = "timetable.txt";
// char attendanceFilePath[MAXARGSIZE] = "attendancesample.txt"; // For testing only
char attendanceFilePath[MAXARGSIZE] = "attendance.txt";
// char subwiseFilePath[MAXARGSIZE] = "subwise.txt"; TODO
char helpFilePath[MAXARGSIZE] = "help.txt";

char today[4]; // Today's Day three letters (+ null terminator)
char todate[30]; // Today's entire date-time (ascdate())

int isequal(char *a, char *b){
	return (strcmp(a, b) == 0);
}

// The return value of strcasecmp() is inverted
int isequalIgnoreCase(char *a, char *b) {
    return (strcasecmp(a, b) == 0);
}


// Set today
void setToday(){
	struct tm *ptr;
	time_t t;
	t = time(NULL);
	ptr = localtime(&t);
	strncpy(today, asctime(ptr), 3);
	strcpy(todate, asctime(ptr));
	today[3] = '\0';	
}

// Show timetable from timetablePath
void showTimetable(char *days) {
    FILE *tt;
    char ttLine[MAXLINESIZE];
    char *toShow = NULL;
    char day[4];

    tt = fopen(timetablePath, "r");
    if(tt == NULL) {
        printf("Couldn't open timetable file\n");
        exit(1);
    }
    int toShowSize = 0;
    while(fgets(ttLine, MAXLINESIZE, tt) != NULL) {
        strncpy(day, ttLine, 3);
        day[3] = '\0';
        if((strstr(days, day) != NULL) || isequal(days, "")) {
            int lineSize = strlen(ttLine);
            toShow = realloc(toShow, toShowSize + lineSize + 1);
            if(toShow == NULL) {
                printf("Couldn't allocate memory for line.\n");
                exit(1);
            }
            strcpy(toShow + toShowSize, ttLine);
            toShowSize += lineSize;
        }
    }
    printf("%s", toShow);
    free(toShow);
    fclose(tt);
}


void enterRow(char *row) {
    FILE *tt, *tempFile;
    char timetable[MAXLINESIZE];
    char tempDay[4]; // Store the first 3 letters of each line as the day (to be compared with rowDay)
    char rowDay[4];
    int found = 0; // Flag to check if the day is found in the timetable (necessary when incorrectly formatted/misspelt days are entered)

    // Get current timetable
    tt = fopen(timetablePath, "r");
    if (tt == NULL) {
        printf("Couldn't open timetable file\n");
        exit(1);
    }
    
    strncpy(rowDay, row, 3);
    rowDay[3] = '\0';

    // Check if the day exists
    while (fgets(timetable, MAXLINESIZE, tt) != NULL) {
        strncpy(tempDay, timetable, 3); // Extract day
        tempDay[3] = '\0';

        if (isequalIgnoreCase(tempDay, rowDay)) {
            found = 1; // Set the flag if the day is found
            break;
        }
    }

    fclose(tt);

    // If the day is found, create a temporary file with the updated content
    if (found) {
        tt = fopen(timetablePath, "r");
        tempFile = fopen("temp_timetable.txt", "w");

        while (fgets(timetable, MAXLINESIZE, tt) != NULL) {
            // Parse the input row
            strncpy(tempDay, timetable, 3); // Extract day without modifying timetable
            tempDay[3] = '\0';

            if (isequalIgnoreCase(tempDay, rowDay)) {
                fprintf(tempFile, "%s\n", row); // Write the updated row
            } else {
                fprintf(tempFile, "%s", timetable); // Write the existing row
            }
        }

        fclose(tt);
        fclose(tempFile);

        // Rename the temporary file to the original file
        remove(timetablePath);
        if (rename("temp_timetable.txt", timetablePath) != 0) {
            printf("Error renaming the temporary file.\n");
            exit(1);
        }
        printf("Day updated successfully.\n");
    } else {
        printf("Day not found in the timetable. Did you misspell something?\n");
    }
}


// Format of info.txt
/*
TotalDays = Number of working days till today.
AttendDays = No. of days classes attended.
SkipDays = No. of days classes skipped.
OffDays = No. of days designated as off(won't be counted towards attendance, just stats).
LastAttend = Date last attended (automatically marks non-weekend days in between as skipped) (asctime())
*/
// Format of subwise.txt
/*
SubsList = List of subjects for the sem.
<Sub1>
TotalDays = Number of working days till today.
AttendDays = No. of days classes attended.
SkipDays = No. of days classes skipped.
OffDays = No. of days designated as off(won't be counted towards attendance, just stats).
LastAttend = Date last attended (automatically marks non-weekend days in between as skipped) (asctime())
<Sub2>
...
.
.
<SubN>
...
*/

// Show attendance from info.txt and subwise.txt
void showAttendance(){
	FILE *att, *subwise;
	char attLine[MAXLINESIZE], subwiseLine[MAXLINESIZE];
	int totalDays = 0;
	int attDays = 0;
	float perc;
	att = fopen(attendanceFilePath, "r");
	// subwise = fopen(subwiseFilePath, "r");
	if(att == NULL){
		printf("Error opening attendance information file.\n");
		exit(1);
	}
	/*
	if(subwise == NULL){
		printf("Error opening subjectwise attendance information file.\n");
		exit(1);
	}
	*/
	while(fgets(attLine, MAXLINESIZE, att)){
		printf("%s", attLine);
		if(!(strncmp(attLine, "TotalDays", 9))){
			// totalDays = attLine[strlen(attLine) - 2] - '0';
			sscanf(attLine, "TotalDays = %d", &totalDays);
			continue;
		}
		if(!(strncmp(attLine, "AttendDays", 10))){
			// attDays = attLine[strlen(attLine) - 2] - '0';
			sscanf(attLine, "AttendDays = %d", &attDays);
			continue;
		}
	}
	/*
	while(fgets(subwiseLine, MAXLINESIZE, subwise)){
		printf("%s", subwiseLine);
	}
	*/
	perc = (float)attDays / (float)totalDays * 100.00;
	printf("Total attendance(%d / %d) stands at: %.2f\n", attDays, totalDays, perc);
	fclose(att);
	// fclose(subwise);
}


// TODO: Write subject-wise version
void attendClass(char *classes) {
    FILE *att, *temp;
    char attLine[MAXLINESIZE];
    int totalDays = 0;
    int attDays = 0;
    float perc;

    att = fopen(attendanceFilePath, "r");
    temp = fopen("temp_attendance.txt", "w");

    if(att == NULL || temp == NULL) {
        printf("Error opening attendance information file.\n");
        exit(1);
    }
    
    // Quick maneouvre to the end of the file to check whether LastAttend day matches today.
    fseek(att, -25, SEEK_END);
    fgets(attLine, MAXLINESIZE, att);
    if(strncmp(attLine, todate, 11) == 0){
    	printf("You can't mark your attendance twice a day!\n");
    	exit(1);
    }
    fseek(att, 0, SEEK_SET);

    while(fgets(attLine, MAXLINESIZE, att) != NULL) {
        if(sscanf(attLine, "TotalDays = %d", &totalDays) == 1) {
            totalDays += 1;
            fprintf(temp, "TotalDays = %d\n", totalDays);
        } else if(sscanf(attLine, "AttendDays = %d", &attDays) == 1) {
            attDays += 1;
            fprintf(temp, "AttendDays = %d\n", attDays);
        } else if(strncmp(attLine, "LastAttend", 10) == 0) {
            fprintf(temp, "LastAttend = %s", todate);
        } else {
            fprintf(temp, "%s", attLine);
        }
    }

    perc = (float)attDays / (float)totalDays * 100.00;
    printf("Total attendance(%d / %d) now stands at: %.2f%%\n", attDays, totalDays, perc);

    fclose(att);
    fclose(temp);

    // Rename temp file to the original file
    remove(attendanceFilePath);
    rename("temp_attendance.txt", attendanceFilePath);
}

// TODO: Write subject-wise version
void leaveClass(char *classes) {
    FILE *att, *temp;
    char attLine[MAXLINESIZE];
    int totalDays = 0;
    int attDays = 0;
    int skipDays = 0;
    float perc;

    att = fopen(attendanceFilePath, "r");
    temp = fopen("temp_attendance.txt", "w");

    if(att == NULL || temp == NULL) {
        printf("Error opening attendance information file.\n");
        exit(1);
    }
    
    // Quick maneouvre to the end of the file to check whether LastAttend day matches today.
    fseek(att, -25, SEEK_END);
    fgets(attLine, MAXLINESIZE, att);
    if(strncmp(attLine, todate, 11) == 0){
    	printf("You can't mark your attendance twice a day!\n");
    	exit(1);
    }
    fseek(att, 0, SEEK_SET);

    while(fgets(attLine, MAXLINESIZE, att) != NULL) {
        if(sscanf(attLine, "TotalDays = %d", &totalDays) == 1) {
            totalDays += 1;
            fprintf(temp, "TotalDays = %d\n", totalDays);
        } else if(sscanf(attLine, "SkipDays = %d", &skipDays) == 1) {
            skipDays += 1;
            fprintf(temp, "SkipDays = %d\n", skipDays);
        } else if(sscanf(attLine, "AttendDays = %d", &attDays) == 1){
        	// Just store it for attendance calculation.
        	fprintf(temp, "%s", attLine);
        } else if(strncmp(attLine, "LastAttend", 10) == 0) {
            fprintf(temp, "LastAttend = %s", todate);
        } else {
            fprintf(temp, "%s", attLine);
        }
    }

    perc = (float)attDays / (float)totalDays * 100.00;
    printf("Total attendance(%d / %d) now stands at: %.2f%%\n", attDays, totalDays, perc);

    fclose(att);
    fclose(temp);

    // Rename temp file to the original file
    remove(attendanceFilePath);
    rename("temp_attendance.txt", attendanceFilePath);
}

// TODO: Write subject-wise version
void markOff(char *classes){
	// only doing something in the subjectwise version makes sense, not overall.
	FILE *att, *temp;
	char attLine[MAXLINESIZE];
	int totalDays = 0;
	int attDays = 0;
	int offDays = 0;
	float perc;
	att = fopen(attendanceFilePath, "r");
	temp = fopen("temp_attendance.txt", "w");
	
    if(att == NULL || temp == NULL) {
        printf("Error opening attendance information file.\n");
        exit(1);
    }
    
    // Quick maneouvre to the end of the file to check whether LastAttend day matches today.
    fseek(att, -25, SEEK_END);
    fgets(attLine, MAXLINESIZE, att);
    printf("%s", attLine);
    if(strncmp(attLine, todate, 11) == 0){
    	printf("You can't mark your attendance twice a day!\n");
    	exit(1);
    }
    fseek(att, 0, SEEK_SET);

    while(fgets(attLine, MAXLINESIZE, att) != NULL) {
        if(sscanf(attLine, "TotalDays = %d", &totalDays) == 1) {
        	// Just store it for attendance calculation.
            fprintf(temp, "%s", attLine);
        } else if(sscanf(attLine, "OffDays = %d", &offDays) == 1) {
            offDays += 1;
            fprintf(temp, "OffDays = %d\n", offDays);
        } else if(sscanf(attLine, "AttendDays = %d", &attDays) == 1){
        	// Just store it for attendance calculation.
        	fprintf(temp, "%s", attLine);
        } else if(strncmp(attLine, "LastAttend", 10) == 0) {
            fprintf(temp, "LastAttend = %s", todate);
        } else {
            fprintf(temp, "%s", attLine);
        }
    }
    
    perc = (float)attDays / (float)totalDays * 100.00;
    printf("Total attendance(%d / %d) now stands at: %.2f%%\n", attDays, totalDays, perc);

    fclose(att);
    fclose(temp);

    // Rename temp file to the original file
    remove(attendanceFilePath);
    rename("temp_attendance.txt", attendanceFilePath);  
    	
}

//TODO: Customised help message for each command.
void showHelp(char *command){
	if(strcmp(command, "") == 0){
		FILE *help;
		char helpLine[MAXLINESIZE];
		help = fopen(helpFilePath, "r");
		while(fgets(helpLine, MAXLINESIZE, help)){
			printf("%s", helpLine);
		}
		fclose(help);
	}
}

int main(int argc, char *argv[]){

	setToday();
	printf("\n%s\n", todate);

	int attendanceShown = 0;
	// showAttendance() is called when no argument is provided
	if(argc == 1){
		showAttendance();
		printf("\n\n");
		showHelp("");
		attendanceShown = 1;
		exit(1);
	}
	// Exit if too many arguments passed.
	if(argc > 3) {
		printf("Too many options/arguments (perhaps missing \"\" in list?). Exiting.\n");
		showHelp("");
		exit(1);
	}	

	char *option = argv[1];
	if(isequal(option, "-t")  || isequal(option, "--timetable")){
		char *days;
		if(argc == 3){
			days = argv[2];
		} else if(argc == 2){
			days = "";
		} else {
			printf("Incorrect command usage. See help.\n");
		}
		showTimetable(days);
	} else if(isequal(option, "-e") || isequal(option, "--enter")) {
		char *row;
		if(argc == 3){
			row = argv[2];
		} else {
			printf("Incorrect command usage. See help..\n");
			exit(1);
		}
		enterRow(row);
	} else if(isequal(option, "-a") || isequal(option, "--attend")) {
		char *classes;
		if(argc == 3){
			classes = argv[2];
		} else if(argc == 2){
			classes = "";
		} else {
			printf("Incorrect command usage. See help.\n");
			exit(1);
		}
		attendClass(classes);		
	} else if(isequal(option, "-l") || isequal(option, "--leave")) {
		char *classes;
		if(argc == 3){
			classes = argv[2];
		} else if(argc == 2){
			classes = "";
		} else {
			printf("Incorrect command usage. See help (-h or --help).\n");
			exit(1);
		}
		leaveClass(classes);
	} else if(isequal(option, "-v") || isequal(option, "--view")) {
		showAttendance();
	} else if(isequal(option, "-h") || isequal(option, "--help")) {
		char *command;
		if(argc == 3){
			command = argv[2];
		} else {
			command = "";
		}
		showHelp(command);
	} else if(isequal(option, "-o") || isequal(option, "--off")) {
		char *classes;
		if(argc == 3){
			classes = argv[2];
		} else if(argc == 2){
			classes = "";
		} else {
			printf("Incorrect command usage. See help (-h or --help).\n");
			exit(1);
		}
		markOff(classes);
	} else if(isequal(option, "-c") || isequal(option, "--cancelled")) {
		;
	} else if(isequal(option, "-s") || isequal(option, "--sett")){
		if(argc == 3){
			// timetablePath = argv[2];
			strcpy(timetablePath, argv[2]);
		} else {
			printf("Incorrect command usage. See help (-h or --help).\n");
			exit(1);
		}
	} else {
		(!attendanceShown) ? showAttendance() : printf("%c", ' ');
		printf("\n\n");
		showHelp(""); 
	}


	return 0;
}
