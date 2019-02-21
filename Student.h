struct student
{
	int studid;
	char* surname;
	char* firstname;
	float gpa;
	int numofcourses;
	char* deprt;
	int postcode;
};


struct student* CreateStudent(int ,char*,char*, float ,int,char*,int );

void DeleteStudent(struct student* );

void PrintStudent(struct student* );

void ChangeGpaNum(struct student* ,float  , int );
