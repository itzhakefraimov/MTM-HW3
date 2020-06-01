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
}*POrder;

typedef struct Table
{
	POrder OrderHead;
	int Bill;
}*PTable;

typedef struct Restaurant
{
	PItem MenuHead;
	PTable Tables;
	int MaxTables;
}Restaurant, *PRestaurant;

/**
 * Outputs a meesage to the console and exits the program afterwards
 *
 * @param msg Error to output
 * @note exits The program with errorcode EXIT_FAILURE
 */
void ConsoleErrorMsg(const char* msg);

/**
 * Handles instruction calls from input files
 *
 * @param ins File of Instructions
 * @param menu File of Menu
 * @param out Output file
 * @param res The Restaurant Entity
 */
void InputInstructions(FILE* ins, FILE* menu, FILE* out, PRestaurant res);

/**
 * Creates the Restaurant's menu using an input file
 *
 * @param in File of Menu
 * @param out Output file
 * @param res The Restaurant Entity
 * @note Every menu Item must have a unique name
 */
void CreateProducts(FILE* in, FILE* out, PRestaurant res);

/**
 * Adds a specified quantity to a desired Item in the Restaurant's menu
 *
 * @param out Output file
 * @param menu_head Head of the menu's linked list
 * @param name Name of the Item to add to
 * @param quantity Quantity to add
 */
void AddItems(FILE* out, PItem* menu_head, const char name[], int quantity);

/**
 * Creates an order to a specified table 
 *
 * @param out Output file
 * @param res The Restaurant Entity
 * @param table_num The Table number to assign the order to
 * @param name Name of the Item to be ordered
 * @param quantity Quantity of the order
 */
void OrderItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity);

/**
 * Removes a specified quantity from a table's order
 *
 * @param out Output file
 * @param res The Restaurant Entity
 * @param table_num The Table number to remove the order from
 * @param name Name of the Item to be returned
 * @param quantity Quantity to be returned
 */
void RemoveItem(FILE* out, PRestaurant res, int table_num, const char name[], int quantity);

/**
 * Cleans a specified table's data and outputs the bill & items ordered
 *
 * @param out Output file
 * @param res The Restaurant Entity
 * @param table_num The Table number to close
 * @note If given table is last remaining - outputs also the most selling Item
 */
void RemoveTable(FILE* out, PRestaurant res, int table_num);

/**
 * Checks if given table_num doesn't exceed maximum tables or <= 0
 *
 * @param out Output file
 * @param table_num The Table number to validate
 * @param max_tables The maximum tables the restaurant can hold
 * @returns TRUE - if 0 < table_num <= max_tables; Otherwise - FALSE
 */
BOOL ValidateTableNumber(FILE* out, const int table_num, const int max_tables);

/**
 * Checks if a given name exists as an Item in the menu
 *
 * @param menu_head Head of the menu's linked list
 * @param name Name of the item to search
 * @returns Address of Item - if found; Otherwise - NULL
 */
PItem IsNameExistsInMenu(PItem menu_head, const char name[]);

/**
 * Checks if a table has an existing order with a specified name
 *
 * @param order_head Head of the table's orders linked list
 * @param name Name of the Order Item to search
 * @returns Address of Order - if found; Otherwise - NULL
 */
POrder IsTableHasOrder(POrder order_head, const char name[]);

/**
 * Checks if there's only one active table in the Restaurant
 *
 * @param res The Restaurant Entity
 * @returns TRUE - 1 Active table; Otherwise - FALSE
 */
BOOL IsLastRemainingTable(PRestaurant res);

/**
 * Retrieves most ordered Item from a given Restaurant's menu
 *
 * @param menu_head Head of the menu's linked list
 * @returns Address of most Ordered Item; Otherwise - NULL
 * @note returns NULL when there are no Items in the menu or 
 * no orders has been made so far
 */
PItem GetMostOrderedItem(PItem menu_head);

/**
 * Deallocates and empties a table's data
 *
 * @param table Address of the Table to deallocate
 */
void DeallocateTableData(PTable table);

/**
 * Deallocates an entire Restaurant Entity
 *
 * @param res Address of the Restaurant to deallocate
 * @note utilizes DeallocateTableData for each Table
 */
void DeallocateRestaurant(PRestaurant res);

#endif