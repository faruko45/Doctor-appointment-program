/*
PLEASE DO NOT CHANGE THIS FILE 
*/

/* @Author
Student Name: Faruk Orak
Student ID : 150180058
Date: 15.11.2019 */

#define NAME_LENGTH 2

struct Task{
	char *name;
	int day;
	int time;
	int priority;

	Task *previous;
	Task *next;
	Task *counterpart;
};
