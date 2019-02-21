struct skip_node
{
	int MaxLevel;
	int id;
	int lvl; // to lvl edw kathorizei posoi forward pointes tha einai energoi
	struct student* data;
	struct skip_node **forward;
};

struct skip_node* CreateSkip();

struct student* SearchSkip(struct skip_node*,int );

struct skip_node* InsertSkipNode(struct skip_node* ,struct student*);

struct skip_node* MakeSkipNode(int,struct student * , int);

int randomLevel(int );

struct student* DeleteSkipNode(struct skip_node* ,int );

void DeleteSkipList(struct skip_node*);//diagrafei olh thn skip list

float SearchStudidAB(struct skip_node* ,int ,int );

void FindGpa(struct skip_node* ,float );

void KWorstStudents(struct skip_node* ,int );
