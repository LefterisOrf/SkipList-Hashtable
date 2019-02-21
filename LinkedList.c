#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Student.h"
#include "LinkedList.h"


struct list_node* InsertNode(struct student* item,struct list_node* head)
{
	if(head == NULL)
	{
		head = malloc(sizeof(struct list_node));
		head->data = item ;
		head->next = NULL;
	}
	else
	{
		head->next = InsertNode(item,head->next); // edw otan epistrepsei h klisi tis sinartisis tha mpei o neos komvos sto tmima next tou proigoumenou
	}
	return head;
}

struct list_node* DeleteNode(struct list_node* head , int id)
{
	struct list_node* temp;// o temp deixnei ston epomeno tou diegrammenou komvou
	if(head == NULL ) return NULL;
	else
	{
		if(head->next->data->studid == id)
		{
			temp= head->next->next;
			free(head->next);
			head->next = temp ;
			return head;
		}
		else
		{
			head->next=DeleteNode(head->next,id);
			return head;
		}
	}
}
struct student* GetCertainItem(int id,struct list_node* p)
{
	if(p == NULL) return NULL;
	else
	{
		while(p!= NULL)
		{
			if(id == p->data->studid ) return p->data;
			else p=p->next;
		}
	}
	printf("Item not found\n");
	return NULL;
}

struct student* GetLastItem(struct list_node* head)
{
	if(head == NULL) return NULL;
	else
	{
		while(head->next!= NULL)head= head->next;
		return head->data;
	}
}

int ListPostIterator(struct list_node* head,int postid)
{
	int i=1; // 1 giati an einai ena stoixeio mesa sthn lista mono den tha kanei kanena i++
	if(head == NULL)
	{
		printf("Error null in ListPostIterator!\n");
		return -5;
	}
	while(head->next != NULL)
	{
		if(head->data->postcode == postid)i++;
		head=head->next;
	}
	return i;
}
void DeleteList(struct list_node* head)
{
	struct list_node* temp = head ;
	struct list_node* iter;
	while(temp != NULL)
	{
		iter= temp->next;
		//if(temp->data != NULL )DeleteStudent(head->data); // o deiktis student exei ginei hdh free apo thn deleteskiplist ( deixnoyn ston idio komvo )
		free(temp);
		temp= iter;
	}
}


float LinkedGpaSum(struct list_node* head,int postid)
{
	struct list_node* temp = head;
	float sum =0.0;
	int num=0;
	if (temp != NULL)
	{
		while(temp!= NULL)
		{
			if(temp->data->postcode == postid)
			{
				sum += temp->data->gpa ;
				num++;
			}
			temp = temp->next;
		}
	}
	if(num > 0)return sum / (float)num;
	else return 0;
}
