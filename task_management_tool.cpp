/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

/* @Author
Student Name: Faruk Orak
Student ID : 150180058
Date: 15.11.2019 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create() //initialising the list
{	head = NULL;
}

void WorkPlan::close() //delete per node one by one
{
	Task *traverse, *tail;
	head->previous->next = NULL;
	while(head)
	{
		head->previous = NULL;
		tail = head;
		head = tail->next;
		traverse = tail->counterpart;
		while(traverse)
		{
			tail->counterpart = traverse->counterpart;
			delete traverse;
			traverse = tail->counterpart;
		}
		delete tail;
	}
}

void WorkPlan::add(Task *task)
{
	Task *traverse, *tail, *newtask;
	newtask = new Task;
	*newtask = *task;
	newtask->name = new char[strlen(task->name)+1];
	strcpy(newtask->name,task->name);
	newtask->previous = NULL;
	newtask->next = NULL;
	newtask->counterpart = NULL;
	if(head == NULL) //create head
	{
		head = newtask;
		head->previous = head;
		head->next = head;
	}
	else //if head has been already created
	{
		traverse = head;
		if(head->day < newtask->day)
		{
			traverse = head->next;
			while((traverse != head) && (traverse->day < newtask->day)) //searching for day
			{
				traverse = traverse->next;
			}
			if(traverse->day == newtask->day) //if day exist
			{
				if(traverse->time > newtask->time) //place the task top of the day because of the ascending time order
				{
					newtask->counterpart = traverse;
					traverse->next->previous = newtask;
					traverse->previous->next = newtask;
					newtask->previous = traverse->previous;
					newtask->next = traverse->next;
					traverse->next = NULL;
					traverse->previous = NULL;
					return;
				}
				else if(traverse->time < newtask->time)
				{
					while(traverse && (traverse->time < newtask->time))//finding the correct place for newtask
					{
						tail = traverse;
						traverse = traverse->counterpart;
					}
					if(traverse)
					{
						if(traverse->time == newtask->time) //if a task has been already taken at the same day and time, compare priority each other
						{
							if(traverse->priority > newtask->priority) //Delay the new task to another available time
							{
								checkAvailableNextTimesFor(newtask);
								newtask->day = getUsableDay();
								newtask->time = getUsableTime();
								add(newtask);
								return;
							}
							else if(traverse->priority < newtask->priority) // Delay the current appointment to another available time
							{
								tail->counterpart = newtask;
								newtask->next = traverse->next;
								checkAvailableNextTimesFor(traverse);
								traverse->day = getUsableDay();
								traverse->time = getUsableTime();
								add(traverse);
								return;
							}
						}
						else //placing the task to correct place
						{
							tail->counterpart = newtask;
							newtask->counterpart = traverse;
							return;
						}
					}
					else //place the new task to bottom of the day 
					{
						tail->counterpart = newtask;
						newtask->counterpart = traverse;
						return;
					}
				}
				else //if a task has been already taken at the same day and time, compare priority each other
				{
					if(traverse->priority < newtask->priority)
					{
						traverse->next->previous = newtask;
						traverse->previous->next = newtask;
						newtask->counterpart = traverse->counterpart;
						newtask->next = traverse->next;
						newtask->previous = traverse->previous;
						checkAvailableNextTimesFor(traverse);
						traverse->day = getUsableDay();
						traverse->time = getUsableTime();
						add(traverse);
						return;
					}
					else
					{
						checkAvailableNextTimesFor(newtask);
						newtask->day = getUsableDay();
						newtask->time = getUsableTime();
						add(newtask);
					}
				}
			}
			else //if there is not any task at new task's day
			{
				if(traverse == head)
				{
					if(head->next->day != head->day)
					{
						traverse = head->previous;
						newtask->previous = traverse;
						newtask->next = head;
						head->previous = newtask;
						traverse->next = newtask;
						return;
					}
					else
					{
						newtask->previous = head;
						newtask->next = head;
						head->previous = newtask;
						head->next = newtask;
						return;
					}
					
				}
				else 
				{
					newtask->next = traverse;
					newtask->previous = traverse->previous;
					newtask->previous->next = newtask;
					traverse->previous = newtask;
				}
				return;
			}
			
		}
		else if(head->day > newtask->day) //changing the head with new task
		{
			newtask->next = head;
			newtask->previous = head->previous;
			newtask->previous->next = newtask;
			head->previous = newtask;
			head = newtask;
			return;
		}
		else // if head's day equals newtask's day 
		{
			if(head->time < newtask->time)
			{
				do
				{
					tail = traverse;
					traverse = tail->counterpart;
				}while(traverse && traverse->time < newtask->time);
				if(traverse)
				{
					if(traverse->time > newtask->time)
					{
						tail->counterpart = newtask;
						newtask->counterpart = traverse;
						return;
					}
					else if(traverse->time == newtask->time)//compare the priority each other
					{
						if(traverse->priority > newtask->priority) //find an another available time for the new task
						{
							checkAvailableNextTimesFor(newtask);
							newtask->day = getUsableDay();
							newtask->time = getUsableTime();
							add(newtask);
						}
						else //delay the current task
						{
							tail->counterpart = newtask;
							newtask->next = traverse->next;
							checkAvailableNextTimesFor(traverse);
							traverse->day = getUsableDay();
							traverse->time = getUsableTime();
							add(traverse);
							return;
						}
					}
				}
				else //place the new task to bottom of the day
				{
					tail->counterpart = newtask;
					newtask->counterpart = traverse;
					return;
				}
			}
			else if(head->time > newtask->time)
			{
				if(head->next != head) //if there is just one day
				{
					newtask->counterpart = head;
					newtask->next = head->next;
					newtask->previous = head->previous;
					head->next = NULL;
					head->previous = NULL;
					newtask->next->previous = newtask;
					newtask->previous->next = newtask;
					head = newtask;
					return;
				}
				else
				{
					newtask->counterpart = head;
					head->next = NULL;
					head->previous = NULL;
					newtask->next = newtask;
					newtask->previous = newtask;
					head = newtask;
				}
			}
		}
	}
} 

Task * WorkPlan::getTask(int day, int time)
{
	Task *traverse;
	if(head != NULL) //if list has been created
	{
		if(head->day == day)
		{
			traverse = head;
			if(traverse->time == time)
			{
				return traverse;//sending the address of the task
			}
			else
			{
				while(traverse && (traverse->time != time))
				{
					traverse = traverse->counterpart;
				}
				if(traverse == NULL)
				{
					cout << "There is no appointment like that";
					return NULL;
				}
				else
				{
					return traverse;//sending the address of the task
				}
			}
		}
		else
		{
			traverse = head->next;
			{
				while((traverse != head) && (traverse->day != day))//searching for the correct day
				{
					traverse = traverse->next;
				}
				if(head == traverse)
				{
					cout << "There is no appointment like that";
					return NULL;
				}
				else
				{
					while(traverse && (traverse->time != time))//searching for the correct time
					{
						traverse = traverse->counterpart;
					}
					if(traverse == NULL)
					{
						cout << "There is no appointment like that";
						return NULL;
					}
					else
					{
						return traverse;//sending the address of the task
					}
				}
			}
		}
	}
	else
	{
		cout << "List is empty!";
		return NULL;
	}
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	
	struct Pocket{ //this struct is created to store available times
		int day;
		int time;
	};
	int day = 0; // is created to store available next time's day
	int time = 0; // is created to store available next time's time
	int count = 0; // is created to keep available_next_times count
	Task *traverse, *tail;
	Pocket available_next_times[63];
	for(int i = 0; i<64; i++)//set all elements of array 0
	{
		available_next_times[i].time = 0;
		available_next_times[i].day = 0;
	}
	tail = head;
	while(tail->day != delayed->day)
	{
		tail = tail->next;
	}
	traverse = getTask(delayed->day, delayed->time); //set traverse to task which will be delayed
	do{
		day = tail->day;
		while(traverse->counterpart != NULL)
		{
			for(time = traverse->time + 1; time < traverse->counterpart->time; time++) //fill the array with avaliable times until next task
			{
				available_next_times[count].day = day;
				available_next_times[count].time = time;
				count++;
			}
			traverse = traverse->counterpart;
		}
		tail = tail->next;//move to following day
		if(tail != head)
		{
			day = tail->day;
			for(time = 8; time < tail->time; time++)// fill the array with available times until first task of day
			{
				available_next_times[count].day = day;
				available_next_times[count].time = time;
				count++;
			}
		}
		traverse = tail;
	}while(tail != head);
	if(count == 0)//if there is not any available day
	{
		while(tail->day != delayed->day)
		{
			tail = tail->next;
		}
		traverse = getTask(delayed->day, delayed->time);
		
		bool assigned = false;//is created to check the task whether is delayed or not
		do
		{
			do
			{
				tail=tail->next;
			}while(tail->time == 8 && tail->next != head);
			if(tail->next == head && tail->time ==8)
			{
				usable_day = tail->day + 1;
				usable_time = 8;
				assigned = true;
			}
			else
			{
				usable_day = tail->day;
				usable_time = tail->time-1;
				assigned = true;
			}
		}while(!assigned);
	}
	else
	{
		usable_day = available_next_times[0].day; //send first available day
		usable_time = available_next_times[0].time; //send first available time of day
	}
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	Task *traverse, *tail;
	Task newtask;
	char cstr[3];
	newtask.name = cstr;
	
	tail=head;
	while(true)//delay all tasks until there is not task at the day
	{
		do
		{
			tail = tail->next;
		}while(tail->day != day && tail != head);
		traverse = tail;
		if(tail->day == day)
		{
			while(traverse->counterpart != NULL)
			{
				traverse = traverse->counterpart;
			}
			checkAvailableNextTimesFor(traverse); //checking available_next_times for last task of day means that start searching for available times from following day
			usable_time = getUsableTime();
			usable_day = getUsableDay();
			newtask.day = usable_day;
			newtask.time = usable_time;
			newtask.priority = tail->priority;
			strcpy(newtask.name,tail->name);
			remove(tail);
			add(&newtask);
			tail = head;
		}
		else //delaying is finished
		{
			break;
		}
	}
}

void WorkPlan::remove(Task *target)
{
	Task *traverse, *tail;
	traverse = head->next;
	while(traverse->day != target->day && traverse != head)
	{
		traverse = traverse->next;//searching for target (correct day)
	}
	if(traverse->day == target->day)//if the correct day is found
	{
		if(traverse->time < target->time)
		{
			do
			{
				tail = traverse;
				traverse = traverse->counterpart;
			}while(traverse && traverse->time != target->time); //searching for target (correct time)
			if(traverse)//if target is found
			{
				tail->counterpart = traverse->counterpart;
				delete traverse;
			}
			else
			{
				cout << "No such a target";
			}
		}
		else if(traverse->time == target->time && traverse->counterpart != NULL)//make the traverse's counterpart top of day
		{
			traverse->counterpart->previous = traverse->previous;
			traverse->counterpart->next = traverse->next;
			traverse->previous->next = traverse->counterpart;
			traverse->next->previous = traverse->counterpart;
			if(traverse == head)//if traverse is head
			{
				head = traverse->counterpart;//make the traverse's counterpart head
				delete traverse;
			}
			else
			{
				delete traverse;
			}
		}
		else if(traverse->time == target->time && traverse->counterpart == NULL)
		{
			traverse->next->previous = traverse->previous;
			traverse->previous->next = traverse->next;
			if(traverse == head)
			{
				head = traverse->next;
			}
			delete traverse;
		}
	}
	else
	{
		cout << "No such a target";
	}
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
