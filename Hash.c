#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Student.h"
#include "LinkedList.h"
#include "SkipList.h"
#include "Hash.h"

int HashFunction(int post,struct hash_table* temp)// returns the position of the student in the hash table based on his postcode
{
	int pos = post% (temp->maxentries) ; // hash function distributes students with the modulo function
	return pos;
}

void HashInsertion(struct student* item,struct hash_table* temp)
{
	int pos = HashFunction(item->postcode,temp); // find the position on the hash table
	temp->table[pos]=InsertNode(item,temp->table[pos]);// insert the student in the linked list that starts from the hash->table[pos]
	temp->entries++; // increase the total entries in the hash table by 1
}

struct hash_table* CreateHashTable(int ent)
{
	struct hash_table* p = malloc(sizeof(struct hash_table));
	p->table = malloc(ent * sizeof(struct list_node*)); // ent number of hash entries are created (its given from the command line)
	p->maxentries= ent ;
	p->entries = 0;
	int i;
	for (i = 0; i < ent; i++)
	{
		p->table[i] = NULL;// all table positions are being initialized with NULL
	}
	return p;
}

void DeleteEntry(struct hash_table* head,int postid,int id) // deletes a certain entry
{
	int pos=HashFunction(postid,head);
	DeleteNode(head->table[pos],id); // delete the certain node from the linked list
	head->entries--;
}

void DeleteHashTable(struct hash_table* temp)
{
	int i;
	for(i=0; i< temp->maxentries ; i ++)// for every entry in the hash table delete the linked list
	{
		DeleteList(temp->table[i]);
		temp->table[i] = NULL ;
	}
	free(temp->table);
	free(temp);
	temp= NULL;
}

float HashPostcodeAverage(struct hash_table* temp,int pos,int postid)
{
	float ave = LinkedGpaSum(temp->table[pos],postid); // returns the average gpa of a certain postcode in the linked list
	return ave;
}


void HashTraverse(struct hash_table* head,int pos,int postid)
{
	int i = ListPostIterator(head->table[pos],postid); // returns the number of students that live in the same postcode
	if(head->entries != 0)
	{
		float perc =(float) i / (float)head->entries;
		fprintf(stdout,"%.2f\n",perc); 
	}
	else printf("not found\n");
}

int CoursesToTake(struct hash_table* head ,int postid,char* tmhma)
{
	int num=-1; // guard in case none is found
	int pos = HashFunction(postid,head);
	struct list_node* temp = head->table[pos];
	if (temp != NULL)
	{
		while (temp !=NULL)// run the list
		{
			if(temp->data !=NULL)
			{
				if ( (temp->data->postcode == postid) && (strcmp(temp->data->deprt,tmhma) == 0)) // if the student lives in the postcode and studies in the dpr
				{
					PrintStudent(temp->data); // print him 
					if(num == -1)num =0;
					num+= temp->data->numofcourses; // add the numofcourses that he has in the total num
				}
				temp= temp->next;
			}
			else break;
		}
	}
	return num;
}

void KBestStud(struct hash_table* head,int k , int postid)
{
	struct student* temp[k];
	struct list_node* iter;
	int i,j,counter=1;// counter for the students in the list 
	int pos = HashFunction(postid,head);
	iter= head->table[pos];
	if(iter != NULL)
	{
		while (iter != NULL)// run the list
		
		{
			if (counter <= k)// if students checked are less than k then they are definately in the best
			{
				temp[counter-1]= iter->data;
			}
			else
			{
				float min=temp[0]->gpa; // we keep the minimum value in the temp array so when a value is bigger than it we put it in the min spot
				int pos=0;
				for (i = 1; i < k; i++)// then we run the temp array to update the min value
				{
					if (temp[i]->gpa < min)
					{
						min = temp[i]->gpa;
						pos=i;
					}		
				}	
				if((iter->data->gpa) > (temp[pos]->gpa))  temp[pos]=iter->data; // because the minimum is saved we dont have to check the whole temp array , just the min
			}
			counter++;
			iter= iter->next;
		}
		for (i = 0; i < k-1; i++) // Bubble Sort to print them in increasing order
		{
			for (j = 0; j < k-i-1; j++)
			{
				if((temp[j] != NULL) && (temp[j+1] != NULL) )
				{
					if(temp[j]->gpa > temp[j+1]->gpa)
					{
						struct student* temporary = temp[j];
						temp[j]=temp[j+1];
						temp[j+1]=temporary;
					}
				}
			}
		}
		for (i = 0; i < k; i++)
		{
			if(temp[i]!= NULL)PrintStudent(temp[i]); // print the students in the temp array
		}
		
	}
}

void HashPercentiles(struct hash_table* hash,struct skip_node* head)
{
	int size=10,i,flag=0;//flag is enabled when the postcode we are searching is found in the postid array
	int pos=0;
	int* postid = malloc(size * sizeof(int));
	if (postid == NULL)
	{
		fprintf(stderr,"Malloc Failed\n");
		return;
	}
	for (i = 0; i < size; i++)
	{
		postid[i]= -1;
	}
	struct skip_node* temp = head->forward[0];
	while (temp != NULL)
	{
		if(temp->data != NULL)
		{
			for (i = 0; i < pos ; i++)// we dont have to check the whole size of the array , just the positions that we put values in 
			{
				if((temp->data->postcode)== postid[i]) 
				{	
					flag= 1;
					break; // if the postcode is found in the postid then there is no need in checking the remaining values
				}	
			}
			if (flag == 0)//flag = 0 means that the postcode isnt in the postid array 
			{
				postid[pos]= temp->data->postcode;//we insert the new postcode in the postid array
				pos++;
				int hpos= HashFunction(temp->data->postcode,hash);
				fprintf(stdout,"%d ",temp->data->postcode);
				HashTraverse(hash,hpos,temp->data->postcode);// prints the postcode and the percentage of students that live in it
				if(pos >= size) // if pos reaches the value of size
				{
					size *= 2; // the size is doubled 
					postid=realloc(postid,size*sizeof(int)); // and we realloc the array
					if (postid == NULL)
					{
						fprintf(stderr,"Realloc Failed\n");
						return;
					}	
				}
			}
		}
		flag=0;
		temp= temp->forward[0];
	}
	free(postid);
	
}





