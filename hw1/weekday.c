//2022/1/4
//weekday: calulate out what day it is based on the date user entered
//Yuan Wang

#include <stdio.h>

int main(void){
	
	//variables
	int week;
	int day;
	int month;
	int year;
	int j;
	int k;
	
	//input section
	printf("Please enter a calendar date: \n");
	printf("Day, d = ");
	scanf("%d", &day);
	printf("Month, m = ");
	scanf("%d", &month);
	printf("Year, y = ");
	scanf("%d", &year);
	
	//check for exception in Zeller's congruence
	if (month == 1 || month == 2){
		month = month + 12;
		year = year - 1;
	}
	
	//calculate for the week
	k = year % 100;
	j = year / 100;
	week = (day + (((month + 1) * 13)/5) + k + (k/4) + (j/4) - (2 * j)) % 7;
	
	//restore the value of month and year
	if(month > 12)
	{
		month = month - 12;
		year = year + 1;
	}
	
	//output the value of the week
	printf("For the calendar date %d/%d/%d, the day of the week is %d\n", month, day, year, week);
	
	//output the week in text 
	if(week == 0){
		printf("This is Saturday.\n");
	}
	if(week == 1){
		printf("This is Sunday.\n");
	}
	if(week == 2){
		printf("This is Monday.\n");
	}
	if(week == 3){
		printf("This is Tuesday.\n");
	}
	if(week == 4){
		printf("This is Wednesday.\n");
	}
	if(week == 5){
		printf("This is Thursday.\n");
	}
	if(week == 6){
		printf("This is Friday.\n");
	}
	return 0;
}
//end of function