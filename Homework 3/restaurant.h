#ifndef RESTAURANT_HEADER
#define RESTAURANT_HEADER

#define MEM_ALLOCATION_MSG "Failed to allocate memory. Exiting..."

#define INSTRUCTIONS_FILE "Instructions.txt"
#define ORDERS_FILE		  "Manot.txt"
#define OUTPUT_FILE		  "Output.txt"
#define MAX_PRODUCT_NAME  50

typedef enum { false, true }BOOL;

typedef struct Product
{
	char* ProductName;
	int Quantity;
	int Price;
}Product;

typedef struct Item
{
	Product Dish;
	struct Item* Next;
}*PItem;

typedef struct Order
{
	Product Data;
	struct Item* Prev;
	struct Item* Next;
}Order, *POrder;

typedef struct Table
{
	POrder Orders;
	int Bill;
}Table, *PTable;

typedef struct Restaurant
{
	PItem MenuHead;
	PTable Tables;
	int MaxTables;
	int CurrTables;
}Restaurant, *PRestaurant;

void ConsoleErrorMsg(const char* msg);
void InputInstructions(FILE* ins, FILE* menu, FILE* out, PRestaurant res);

void CreateProducts(FILE* in, FILE* out, PRestaurant res);
void AddItems(FILE* out, PItem* menu_head, const char name[], int quantity);
void OrderItem(int table_num, const char name[], int quantity);
void RemoveItem(int table_num, const char name[], int quantity);
void RemoveTable(int table_num);
PItem IsNameExistsInMenu(PItem menu_head, const char name[]);

#endif