#ifndef RESTAURANT_HEADER
#define RESTAURANT_HEADER

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
}Order, * POrder;

void ConsoleErrorMsg(const char* msg);
void InputInstructions(FILE* f);

void CreateProducts(FILE* in, FILE* out, PItem* menu_head);
void AddItems(FILE* out, PItem* menu_head, const char name[], int quantity);
void OrderItem(int table_num, const char name[], int quantity);
void RemoveItem(int table_num, const char name[], int quantity);
void RemoveTable(int table_num);
struct Item* IsNameExistsInMenu(PItem menu_head, const char name[]);

#endif