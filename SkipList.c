#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Student.h"
#include "SkipList.h"
#include "Hash.h"
const int MaxLevel = 4;
const int MaxValue = 10000000; // edw edw edw
struct skip_node* CreateSkip()
{
	int i;
	struct skip_node* head = malloc(sizeof(struct skip_node));// the header node is created 
	head->data= NULL;
	head->MaxLevel=MaxLevel;
	head->lvl = MaxLevel;
	head->id= -1;
	head->forward = malloc(MaxLevel * sizeof(struct skip_node*));// we allocate memory for the forward pointers of the header node
	struct skip_node* term = malloc(sizeof(struct skip_node));// the termination node is created
	term->data = NULL;
	term->MaxLevel= MaxLevel;
	term->id= MaxValue ; 
	term->forward = malloc(MaxLevel * sizeof(struct skip_node*));
	for (i = 0; i < MaxLevel; i++)//the forward pointers of the termination node are set to NULL
	{
		term->forward[i] = NULL;
	}
	for (i = 0; i < MaxLevel; i++)// the forward pointers of the header node are pointing to the termination node 
	{
		head->forward[i]= term;
	}
	return head;//return the head node
}

struct student* SearchSkip(struct skip_node* header, int SearchKey)
{
	struct skip_node* iter= header;
	int i;
	for (i = iter->lvl-1; i >= 0 ; i--)// start searching from the higher forward pointer of the current node
	{
		while(iter->forward[i]->id < SearchKey)iter = iter->forward[i]; // if the pointer points to a node whose id is smaller than the key we are looking for , go to that node
	}
	iter= iter->forward[0];// the for loop stops at the next smaller value of the Searchkey so we forward to the next node
	if(iter->id == SearchKey)return iter->data ; // if the id isnt the same then the Search key is not included in the Skip List
	else return NULL;
}

struct skip_node* InsertSkipNode(struct skip_node* header,struct student* Search)
{
	int i;
	struct skip_node* iter =header;
	struct skip_node* update[MaxLevel];
	for (i = MaxLevel-1; i >= 0 ; i--)// search the position that the skip node has to be inserted,like above
	{
		while(iter->forward[i]->id < Search->studid)iter = iter->forward[i];
		update[i] = iter;
	}
	iter = iter->forward[0];
	if(iter->id == Search->studid) // if the student we are trying to insert is already in the skiplist he is deleted (not the entry that already is inside)
	{
		DeleteStudent(Search);
		return NULL;
	}
	else
	{
		int lvl = randomLevel(MaxLevel);
		struct skip_node* temp = MakeSkipNode(lvl,Search,Search->studid); // creates the node and all its forward pointers are set to NULL
		for (i = 0; i < lvl; i++)
		{
			temp->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = temp ;
		}
	}
	return header;
}

struct skip_node* MakeSkipNode(int level,struct student *stud ,int stdid)
{
	int i;
	struct skip_node* temp = malloc(sizeof(struct skip_node));
	temp->id = stud->studid;
	temp->MaxLevel = MaxLevel;
	temp->data = stud ;
	temp->lvl = level;
	temp->forward = malloc(MaxLevel * sizeof(struct skip_node*));
	for (i = 0; i < MaxLevel; i++)
	{
		temp->forward[i] = NULL ;
	}
	return temp;
}

int randomLevel(int MaxLevel)
{
	int i = rand()%MaxLevel+1;
	return i;
}

struct student* DeleteSkipNode(struct skip_node* header,int SearchKey)// returns the student so the entries are deleted first and then the student himself
{
	int i;
	struct student* std = NULL;
	struct skip_node* iter =header;
	struct skip_node* update[MaxLevel];
	for (i = MaxLevel-1; i >=0 ; i--)
	{
		while(iter->forward[i]->id < SearchKey)iter = iter->forward[i];
		update[i] = iter;// we store the last forward pointer of each level in the update array so when the node is deleted we can change their values to the next node of the same level
	}
	iter=iter->forward[0];
	if(iter->id == SearchKey)
	{
		std = iter->data;
		for (i = 0; i < MaxLevel; i++)
		{
			if(update[i]->forward[i] != iter)break;
			update[i]->forward[i] = iter->forward[i];
		}
		free(iter->forward);
		free(iter);
	}
	return std;
}

void DeleteSkipList(struct skip_node* header)
{
	struct skip_node* temp = header;
	struct skip_node* iter;
	while(temp !=NULL)
	{
		iter=temp->forward[0];
		free(temp->forward);
		if(temp->data != NULL)DeleteStudent(temp->data);// termination node's data section is NULL
		free(temp);
		temp= iter;
	}
}

float SearchStudidAB(struct skip_node* head,int studidA,int studidB)// finds the average gpa of the students between studidA and studidB
{
	if ((studidA < studidB) && (studidA < MaxValue) && (studidB < MaxValue))
	{
		struct skip_node* temp= head;
		struct skip_node* studB;
		int i,numofstudents=0;
		float sum =0;
		for (i = MaxLevel -1; i >= 0 ; i--)
		{
			while((temp->forward[i]->id) < studidA )temp = temp->forward[i];
		}
		struct skip_node* studA = temp->forward[0]; // ston studA mpainei o protos komvos poy einai >= apo to studidA (o protos megalyteros se periptosi pou den yparxei to studidA stin skiplist
		temp = head;
		for (i = MaxLevel -1; i >= 0 ; i--)
		{
			while((temp->forward[i]->id) < studidB )temp = temp->forward[i];
		}
		if (temp->forward[0]->id == studidB )studB = temp->forward[0]; // ston studB mpainei o komvos poy exei id oso to studidB
		else studB = temp;// an den yparxei to studidB sthn skiplist , ston studB mpainei o komvos me to amesos mikrotero id apo to studidB
		temp=studA;
		while ((temp->id) <= (studB->id))
		{
			sum+= temp->data->gpa;
			numofstudents ++;
			temp = temp->forward[0];
		}
		if(numofstudents > 0)return sum / (float) numofstudents;
		else return -1;
	}
}

void FindGpa(struct skip_node* head,float mesos)
{
	struct skip_node* temp = head->forward[0];
	int maxcourses=0;
	if (temp != NULL)
	{
		while (temp!= NULL)// this loop finds the max numofcourses in the list
		{
			if(temp->data!= NULL)
			{
				if((temp->data->numofcourses)> maxcourses)maxcourses = temp->data->numofcourses;
			}
			temp=temp->forward[0];
		}
		temp=head->forward[0];
		while(temp!= NULL)
		{
			if(temp->data!= NULL)
			{
				if ((temp->data->numofcourses == maxcourses) && ((temp->data->gpa) > mesos))
				{
					PrintStudent(temp->data);
				}
			}
			temp=temp->forward[0];
		}
	}
}


void KWorstStudents(struct skip_node* head,int k)
{
	struct student* temp[k];
	struct skip_node* iter=head->forward[0];// the head node in the skip list has its data section set to NULL so we start searching from the second node
	int i,counter=1;
	while (iter != NULL && (iter->data != NULL) )
	{
		if (counter <= k)
		{
			temp[counter-1]=iter->data;
		}
		else
		{
			float max=temp[0]->gpa; // variable max is showing the maximum gpa currently in the temp array 
			int pos=0;				// variable pos is showing the current position of the maximum gpa in the array
			for (i = 1; i < k; i++)
			{
				if(temp[i]->gpa > max)
				{
					max=temp[i]->gpa;
					pos=i;
				}
			}
			if((iter->data->gpa) < (temp[pos]->gpa))  temp[pos]=iter->data;	
		}
		iter = iter->forward[0];
		counter++;
	}
	for (i = 0; i < k; i++)
	{
		if(temp[i] != NULL) PrintStudent(temp[i]);
	}
}
