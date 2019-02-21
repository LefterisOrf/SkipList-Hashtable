#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Student.h"

struct student* CreateStudent(int id,char* epitheto , char* onoma , float mesos ,int armathimatwn , char* tmhma , int taxkwd)
{
	struct student* temp = malloc(sizeof(struct student));
	temp->studid = id;
	temp->surname = malloc((strlen(epitheto)+1)*sizeof(char));
	strcpy(temp->surname,epitheto);
	temp->firstname = malloc((strlen(onoma)+1)*sizeof(char));
	strcpy(temp->firstname,onoma);
	temp->gpa = mesos;
	temp->numofcourses=armathimatwn;
	temp->deprt = malloc((strlen(tmhma)+1)*sizeof(char));
	strcpy(temp->deprt,tmhma);
	temp->postcode=taxkwd;
	return temp;
}

void PrintStudent(struct student* temp)
{
	printf("%d %s %s %.2f %s %d\n",temp->studid ,temp->surname ,temp->firstname,temp->gpa,temp->deprt,temp->postcode);
}


void DeleteStudent(struct student* temp)
{
	free(temp->firstname);
	temp->firstname= NULL;
	free(temp->deprt);
	temp->deprt= NULL;
	free(temp->surname); 
	free(temp);
	temp= NULL ;	
}

void ChangeGpaNum(struct student* stud,float mesos , int armath)
{
	stud->gpa = mesos;
	stud->numofcourses=armath;
}
