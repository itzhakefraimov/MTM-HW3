#ifndef RESTAURANT_HEADER
#define RESTAURANT_HEADER

#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define ERROR_LOCATE_FILE_MSG "Could not locate file. Exiting..."
#define ERROR_MEM_ALLOCATION_MSG "Failed to allocate memory. Exiting..."

#define INSTRUCTIONS_FILE "Instructions.txt"
#define MENU_FILE		  "Manot.txt"
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
	int TotalOrdered;
	struct Item* Next;
}*PItem;

typedef struct Order
{
	Product Data;
	struct Order* Prev;
	struct Order* Next;
}Order, *POrder;

typedef struct Table
{
	POrder OrderHead;
	int Bill;
}Table, *PTable;

typedef struct Restaurant
{
	PItem MenuHead;
	PTable Tables;
	int MaxTables;
}Restaurant, *PRestaurant;

void ConsoleErrorMsg(const char* msg);
void InputInstructions(FILE* ins, FILE* menu, FILE* out, PRestaurant res);
void CreateProducts(FILE* in, FILE* out, PRestaurant res);
void AddItems(FILE* out, PItem* menu_head, const char name[], int quantity);
void OrderItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity);
void RemoveItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity);
void RemoveTable(FILE* out, PRestaurant res, int table_num);
BOOL ValidateTableNumber(FILE* out, const int table_num, const int max_tables);
PItem IsNameExistsInMenu(PItem menu_head, const char name[]);
POrder IsTableHasOrder(POrder order_head, const char name[]);
BOOL IsLastRemainingTable(PRestaurant res);
PItem GetMostOrderedItem(PItem menu_head);
void DeallocateTableData(PTable table);
void DeallocateRestaurant(PRestaurant res);

#endif