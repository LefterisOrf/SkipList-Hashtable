#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Student.h"
#include "LinkedList.h"
#include "SkipList.h"
#include "Hash.h"


int main(int argc,char* argv[])
{	
	int entries=-1;
	char file[15];
	if(argc < 3) // to run the program we need at least 2 arguments to specify the maximum number of positions that the hash table must have
	{
		fprintf(stderr,"Not enough arguments!\n");
		return 1;
	}
	else
	{
		if(strcmp(argv[1],"-b") == 0)// if the first argument is the number of hash entries
		{
			entries = atoi(argv[2]);
			if(argc >= 5)
			{
				if(strcmp(argv[3],"-l")== 0)strcpy(file,argv[4]);
			}
		}
		else if(strcmp(argv[1],"-l") == 0)// else the first argument specifies the file name
		{
			strcpy(file,argv[2]);
			if(argc >=5)
			{
				if(strcmp(argv[3],"-b") == 0)entries = atoi(argv[2]);
			}
		}
		else fprintf(stderr,"Ivalid arguments\n");
	}
	struct skip_node * skip_list = CreateSkip();
	if(entries <= 0)// if the number of hash entries is invalid print a message and stop the program
	{
		fprintf(stderr,"Invalid number of entries!\n");
		return -1;
	}
	struct hash_table* hash= CreateHashTable(entries);
	FILE *fp;
	fp = fopen(file,"r"); 
	if(fp == NULL)
	{ 
		printf("Could not open the file \n"); //if the file isnt specified or for some reason could not be opened , the program takes input from the stdin (keyboard)
		fp=stdin; 
	}
	char querry[3];
	fscanf(fp,"%s",querry);//we put the querry in a variable with 3 char length (max 2 used)
	while(strcmp(querry,"e") !=0) 
	{
		if(strcmp(querry,"i") == 0) // Insertion 
		{
			int id,armath;
			struct skip_node* temp;
			char epitheto[30],onoma[30],tmhma[10],taxkwd[10];//these variables are used temporarily to store the values before the student is allocated an inserted to the hash and the skiplist
			float mesos;
			fscanf(fp,"%d %s %s %f %d %s %s",&id,epitheto,onoma,&mesos,&armath,tmhma,taxkwd);
			if(strlen(taxkwd)== 5)// if the postcode is valid
			{
				int post = atoi(taxkwd);
				struct student* std =CreateStudent(id,epitheto,onoma,mesos,armath,tmhma,post);// the student is created
				temp=InsertSkipNode(skip_list,std);//and then is being inserted in the SkipList
				if (temp !=NULL)// if InsertSkipNode returns NULL , it means that the student has already been inserted in the SkipList and at the HashTable 
				{			
					HashInsertion(std,hash);// Insert student in the hash table
					PrintStudent(std);
				}
			}
			else
			{
				fprintf(stderr,"Invalid Postcode at Insertion\n");
			}
		}
		else if(strcmp(querry,"q") == 0)// Question
		{
			int id;
			fscanf(fp,"%d",&id);
			struct student* std= SearchSkip(skip_list,id);//SearchSkip returns a pointer to the student with ID if he is on the list or NULL if he is not
			fprintf(stderr,"%s %d\n",querry,id);
			if(std != NULL)PrintStudent(std);
			else fprintf(stderr,"not found\n");
		}
		else if(strcmp(querry,"m") == 0) // Change Gpa and Num of courses
		{
			int id,armath;
			float mesos;
			fscanf(fp,"%d %f %d",&id,&mesos,&armath);
			fprintf(stderr,"%s %d %f %d\n",querry,id,mesos,armath);
			struct student* std= SearchSkip(skip_list,id);//search the student based on the id 
			if(std == NULL)fprintf(stderr,"not found\n");
			else
			{
				ChangeGpaNum(std,mesos,armath);// change his gpa and numofcourses
				PrintStudent(std);//print the student with his new values
			}
		}
		else if(strcmp(querry,"d") == 0) // Delete 
		{
			int id;
			fscanf(fp,"%d",&id);
			fprintf(stderr,"%s %d\n",querry,id);
			struct student* std =DeleteSkipNode(skip_list,id); // DeleteSkipNode deletes the entry from the skiplist and returns a pointer to the student. This means its easier to delete him from the Hash Table
			if(std != NULL) 
			{
				DeleteEntry(hash,std->postcode,id);
				PrintStudent(std);
				DeleteStudent(std);
			}
			//else fprintf(stderr,"not found\n"); // delete isnt needed to print something if you dont find the student
		}
		else if(strcmp(querry,"ra") == 0)// Average from studidA to studidB
		{
			int id1,id2;
			fscanf(fp,"%d %d",&id1,&id2);
			fprintf(stderr,"%s %d %d\n",querry,id1,id2);
			float av = SearchStudidAB(skip_list,id1,id2);
			printf("%.2f\n",av);
		}
		else if(strcmp(querry,"a") == 0) // Average for a certain postcode
		{
			char post[10];
			fscanf(fp,"%s",post);
			fprintf(stderr,"%s %s\n",querry,post);
			if(strlen(post)== 5)// if the postcode is valid
			{
				int postid = atoi(post);
				int pos = HashFunction(postid,hash);// find the position in the hash table based on the postcode
				if((pos >= 0)  && (pos <= hash->maxentries))
				{
					float aver = HashPostcodeAverage(hash,pos,postid);// returns the percentage of students that live in the certain postcode
					printf("%.2f\n",aver);
				}
			}
		}
		else if(strcmp(querry,"ta") == 0) // k best students in postcode
		{
			int num;
			char post[10];
			fscanf(fp,"%d %s",&num,post);
			fprintf(stderr,"%s %d %s\n",querry,num,post);
			int postid = atoi(post);
			KBestStud(hash,num,postid);
		}
		else if(strcmp(querry,"b") == 0)// 		k worst students
		{
			int num;
			fscanf(fp,"%d",&num);
			fprintf(stderr,"%s %d\n",querry,num);
			KWorstStudents(skip_list,num);
		}
		else if(strcmp(querry,"ct") == 0) // Courses to take
		{
			char post[10];
			char tmhma[10];
			fscanf(fp,"%s %s",post,tmhma);
			fprintf(stderr,"%s %s %s\n",querry,post,tmhma);
			if (strlen(post)== 5)
			{
				int postid = atoi(post);
				int num= CoursesToTake(hash,postid,tmhma);
				if (num == -1)// if none is found the function returns -1
				{
					fprintf(stderr,"not found\n");
				}
				else printf("%d\n",num);
			}
			else fprintf(stderr,"Invalid Postcode\n");
		}
		else if(strcmp(querry,"f") == 0) // Find gpa
		{
			float mesos;
			fscanf(fp,"%f",&mesos);
			printf("%s %.2f\n",querry,mesos);
			FindGpa(skip_list, mesos);//prints the student(s) that owe the most courses and have a gpa higher than the one given 
		}
		else if(strcmp(querry,"p") == 0)//Percentage of students in a certain postcode 
		{
			char taxkwd[10];
			fscanf(fp,"%s",taxkwd);
			fprintf(stderr,"%s %s\n",querry,taxkwd);
			if(strlen(taxkwd)== 5)
			{
				int postid = atoi(taxkwd);
				int pos = HashFunction(postid,hash);// find the position in the hash table
				HashTraverse(hash,pos,postid);// prints the percentage of students that live in a certain postcode
			}
			else printf("not found\n");
		}
		else if(strcmp(querry,"pe") == 0)//12
		{
			fprintf(stderr,"%s\n",querry);
			HashPercentiles(hash,skip_list);//prints the percentage of students that live in each postcode
		}
		else
		{
			printf("Ivalid querry!\n");
			return -1;
		}
		fscanf(fp,"%s",querry);
	}
	fprintf(stderr,"%s\n",querry); // print the last querry ( exit )
	fclose(fp);
	DeleteSkipList(skip_list);// delete the SkipList and all its entries
	DeleteHashTable(hash); // delete hash table and all its entries
	return 0;
}
