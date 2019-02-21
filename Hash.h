struct hash_table
{
	struct list_node** table;
	int maxentries;
	int entries;
};

int HashFunction(int,struct hash_table*);

void HashInsertion(struct student* , struct hash_table*);

void DeleteEntry(struct hash_table* ,int ,int );

struct hash_table* CreateHashTable(int );

void DeleteHashTable(struct hash_table* );

void HashTraverse(struct hash_table* head,int pos,int postid);

float HashPostcodeAverage(struct hash_table* ,int ,int );

int CoursesToTake(struct hash_table* ,int ,char* );

void KBestStud(struct hash_table* ,int  , int );

void HashPercentiles(struct hash_table* , struct skip_node* );
