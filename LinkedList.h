struct list_node
{
	struct student *data;
	struct list_node *next;
};

struct list_node* CreateList(struct list_node* );

struct list_node* InsertNode(struct student* ,struct list_node* head);

struct list_node* DeleteNode(struct list_node* ,int );

int ListPostIterator(struct list_node* ,int);

void DeleteList(struct list_node* );

struct student* GetCertainItem(int ,struct list_node*);

struct student* GetLastItem(struct list_node*);

float LinkedGpaSum(struct list_node* ,int );
