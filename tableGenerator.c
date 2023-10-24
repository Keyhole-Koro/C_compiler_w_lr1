#include "tableGenerator.h"

enum {
	TERMIANL_START = 1,
	NUM,      //number
	ADD,      // +
	SUB,      // -
	MUL,      // *
	DIV,      // /
	L_PARENTHESES, // (
	R_PARENTHESES, // )

	PLUS, //+
	MINUS, //-
	ASTARISK, //*
	SLASH, // /

	FIRST,

	NON_TERMINAL_START = 256,
	EXPR, // E
	TERM, // T
	FACTOR, // F
	ACCEPTED, //accept

	END, //$ temporary

	S,  // shift
	R, // reducce
	G, //goto
	ACC, //acc
};

//pre defined Productions
//{EXPR, ADD, TERM}
//   0    1     2
Production productions[] = {
	{0, EXPR, {EXPR, END, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{1, EXPR, {EXPR, ADD, TERM, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{2, EXPR, {EXPR, SUB, TERM, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{3, EXPR, {TERM, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{4, TERM, {TERM, MUL, FACTOR, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{5, TERM, {TERM, DIV, FACTOR, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{6, TERM, {FACTOR, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{7, FACTOR, {L_PARENTHESES, EXPR, R_PARENTHESES, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{8, FACTOR, {NUM, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
};


//may not needed
void appendCopiedOriginalProd(DynamicArray *arr, int key, Type type) {
	if (type != PRODUCTION) error("type mismatch: appendCopiedOriginalProd");
	appendCopy(arr, &productions[key], type);
}

DynamicArray *insertProdsFromPreDefined() {
	int size = ARRAY_LENGTH(productions);
	DynamicArray *originalProdArray = createDynamicArray(size, true, dummy_member, PRODUCTION);

	for (int i = 0; i < size; i++) {
		appendCopiedOriginalProd(originalProdArray, i, PRODUCTION);
	}
	return originalProdArray;
}

DynamicArray *setUpPreDefinedProd() {
	DynamicArray *originalProdArray = insertProdsFromPreDefined();
	updateCur_Symbol(originalProdArray, PRODUCTION);
	//quickSort(originalProdArary, getCur_symbol, 0, getArrayOffset(originalProdArary), PRODUCTION);
	return originalProdArray;
}

void printProd(DynamicArray *arr) {
	if (arr->type != PRODUCTION) error("type mismatch: printProd\n");

	printf("--------\n|inside of a prod\n");
	printf("|-size: %d\n", getArraySize(arr));
	printf("|-overlap: ");
	arr->ifAllowOverlap ? printf("allowed\n"):printf("not alloweded\n");
	printf("|-modifing: ");
	arr->allowModify ? printf("allowed\n") : printf("not allowed\n");
	for (int i = 0; i < getArraySize(arr); i++) {
		Production *prod = (Production *)retriveData(arr, i, PRODUCTION);
		printf("|prod key left readPosition cur_symbol: %d %d %d %d\n", prod->key, prod->left, prod->readPosition, prod->cur_symbol);
	}
	printf("--------\n");
}

bool isNonTerminal(int symbol) {
	return symbol > NON_TERMINAL_START;
}

int getIntFromData(Data *data, Type type) {
	if (type != INT) error("type mismatch: getIntFromData\n");
	return *(int *)data;
}

int getIntFromDataForSymbol(Data *data, Type type) {
	if (type != INT) error("type mismatch: getIntFromDataForSymbol\n");
	return *(int *)data - NON_TERMINAL_START;
}

int getKeyFromProd(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getKeyFromProd\n");
	return ((Production*)data)->key;
}

int getLeftFromProd(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getLeftFromProd\n");
	return ((Production*)data)->left;
}

int getCur_symbol(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getCur_symbol\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol;
}

int getCur_symbolForOverlapHandler(Data *data, Type type) {
	if (type != PRODUCTION && type != INT) error("type mismatch: getCur_symbolForOverlapHandler\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol - NON_TERMINAL_START;
}

bool cmpCur_symbol(Data *data, Data *expectedData, Type type) {
	if (type != PRODUCTION) error("type mismatch: cmpCur_symbol\n");
	Production *symbol = (Production *)data;
	Production *compedValue = (Production *)expectedData;
	return symbol->cur_symbol == compedValue->cur_symbol;
}

bool cmpHash_keyOfProdsInItem(Data *data, Data *expectedData, Type type) {
	if (type != ITEM) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	return item->hashed_keys == cmpedItem->hashed_keys;
}

bool cmp_readSymbolHash_keyOfProdsInItem(Data *data, Data *expectedData, Type type) {
	if (type != ITEM) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	if (item->hashed_keys != cmpedItem->hashed_keys) return false;
	if (item->readSymbol != cmpedItem->readSymbol) return false;
	return true;
}

int getHash_keyOfProdsInItem(Data *data, Type type) {
	if (type != ITEM) error("type mismatch: getHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	return item->hashed_keys;
}

bool cmpLeftFromProd(Data *data, Data *expectedValue, Type type) {
	if (type != PRODUCTION) error("type mismatch: cmpLefFromProd\n");
	Production *symbol = (Production *)data;
	Production *compedValue = (Production *)expectedValue;
	return symbol->left == compedValue->left;
}

bool cmpStateId(Data *data, Data *expectedValue, Type type) {
	if (type != ITEM) error("type mismatch: cmpStateId\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->stateId == *compedValue;
}

bool cmpReadSymbol(Data* data, Data* expectedValue, Type type) {
	if (type != ITEM) error("type mismatch: cmpReadSymbol\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->readSymbol == *compedValue;
}

void updateCur_Symbol(DynamicArray *prodArr, Type type) {
	if (type != PRODUCTION) error("type mismatch: setSymbol\n");
	for (int i = 0; i < getArraySize(prodArr); i++) {
		Production *prod = (Production *)retriveData(prodArr, i, PRODUCTION);
		//look ahead and increament
		//if (prod->cur_symbol == -1) continue;
		int pos = ++prod->readPosition;
		//update cur_symbol
		int symbol = prod->right[pos];
		prod->cur_symbol = symbol;
	}
}

DynamicArray *Clone_UpdateProds_Cur_symbol(DynamicArray *oldProdArray, Type type) {
	if (type != PRODUCTION) error("type mismatch: setSymbol\n");
	DynamicArray *clonedProdsArray = cloneArray(oldProdArray, true, oldProdArray->referentMember);
	quickSort(clonedProdsArray, getLeftFromProd, 0, getArrayOffset(clonedProdsArray), PRODUCTION);
	updateCur_Symbol(clonedProdsArray, type);
	return clonedProdsArray;
}

//make this optimize later particulary using deprioritizer
bool isClosureItem(DynamicArray *prodArr) {
	if (getArraySize(prodArr) != 1) return false;
	Production *singleProd = (Production *)retriveData(prodArr, 0, PRODUCTION);
	int pos_read = singleProd->readPosition;

	//which means the current symbol is the end of right side
	if (singleProd->right[pos_read + 1] == -1) return true;
	return false;
}

DynamicArray *listCur_symbol(DynamicArray *prodArray, bool ifAllowNon_Terminal){
	if (prodArray->type != PRODUCTION) printf("type mismatch: listCur_symbol\n");
	DynamicArray *listedCur_symbolArray = createDynamicArray(ARRAY_LENGTH(productions), true, getIntFromDataForSymbol, INT);

	for (int i = 0; i < getArraySize(prodArray); i++) {
		Production *prod = (Production *)retriveData(prodArray, i, PRODUCTION);

		//if i remove this this program goes well
		if (!ifAllowNon_Terminal && !isNonTerminal(prod->cur_symbol)) continue;

		appendCopy(listedCur_symbolArray, &(prod->cur_symbol), INT);
	}

	return listedCur_symbolArray;
}

void gatherProdbyCertainCur_symbol(DynamicArray *collectedProdArray, DynamicArray *listedCur_symbolArray, DynamicArray *preDefinedProds, int expected_symbol) {

	Production *expected_data = initializeProduction();

	expected_data->left = expected_symbol;

	DynamicArray *newly_fetchedProdbySingleSymbolArray = fetchCommonElements(preDefinedProds, cmpLeftFromProd, (Data *)expected_data, true, PRODUCTION);

	DynamicArray *newly_listedCur_symbolArray = listCur_symbol(newly_fetchedProdbySingleSymbolArray, true);

	copyPasteArray(newly_fetchedProdbySingleSymbolArray, collectedProdArray);
	copyPasteArray(newly_listedCur_symbolArray, listedCur_symbolArray);
	
	destroyDynamicArray(newly_fetchedProdbySingleSymbolArray);
	destroyDynamicArray(newly_listedCur_symbolArray);
	free(expected_data);
}

DynamicArray *gatherProdswithSymbols(DynamicArray *collectedProdArray) {
	DynamicArray *preDefinedProds = setUpPreDefinedProd();

	DynamicArray *listedCur_symbolArray = listCur_symbol(collectedProdArray, false);

	int num_cur_symbol = getArraySize(listedCur_symbolArray);
	for (int i = 0; i < num_cur_symbol; i++) {

		gatherProdbyCertainCur_symbol(collectedProdArray, listedCur_symbolArray, preDefinedProds, *(int *)retriveData(listedCur_symbolArray, i, INT));

		num_cur_symbol = getArraySize(listedCur_symbolArray);
	}
	return collectedProdArray;
}

Item *findExistingItem(DynamicArray *itemArray, DynamicArray *prodArray, int expected_readSymbol) {
	if (getArraySize(itemArray) <= 1) return dummy_item;

	Item *expected_item = initializeItem();
	expected_item->readSymbol = expected_readSymbol;
	expected_item->hashed_keys = calculateHash(prodArray, getKeyFromProd, PRODUCTION);

	DynamicArray *fetchedItemArray = fetchCommonElements(itemArray, cmp_readSymbolHash_keyOfProdsInItem, (Data *)expected_item, false, ITEM);
	//this is supposed to one item
	if (getArraySize(fetchedItemArray) > 1) printf("fetchedItemArray isnt supposed to have %d elements", getArraySize(fetchedItemArray));
	if (getArraySize(fetchedItemArray) == 1) return (Item *)retriveData(fetchedItemArray, 0, ITEM);

	return dummy_item;
}

void separateProds(DynamicArray *itemArray, DynamicArray *collectedProdArray, int *hashedStateIdOfItem_transit){

	DynamicArray *listedCur_symbolArray = listCur_symbol(collectedProdArray, true);

	Production *expected_data = initializeProduction();

	for(int i = 0; i < getArraySize(listedCur_symbolArray); i++) {\
		int expected_symbol = *(int *)retriveData(listedCur_symbolArray, i, INT);
		
		expected_data->cur_symbol = expected_symbol;

		DynamicArray *fetchedProdbySingleSymbolArray = fetchCommonElements(collectedProdArray, cmpCur_symbol, (Data *)expected_data, true, PRODUCTION);
		if (!isClosureItem(fetchedProdbySingleSymbolArray)) constructItem(itemArray, fetchedProdbySingleSymbolArray, hashedStateIdOfItem_transit, expected_symbol);
	}
	free(expected_data);
}

void constructItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int *hashedStateIdOfItem_transit, int expected_symbol) {
	disableModify(fetchedProdArray);
	updateCur_Symbol(fetchedProdArray, PRODUCTION);

	DynamicArray *collectedProdArray = gatherProdswithSymbols(fetchedProdArray);

	printProd(collectedProdArray);

	Item *existingItem = findExistingItem(itemArray, collectedProdArray, expected_symbol);
	if (existingItem != dummy_item) return;//if existingItem doesnt exist

	Item *item = createItem(getArraySize(itemArray), expected_symbol, (Production *)(fetchedProdArray->data), calculateHash(fetchedProdArray, getKeyFromProd, PRODUCTION), -1);
	append(itemArray, item, ITEM);
	
	updateHashedStateId(hashedStateIdOfItem_transit, item->stateId);
	
	DynamicArray *clonedCollectedProdArray = cloneArray(collectedProdArray, true, dummy_member);
	if (!isClosureItem(collectedProdArray)) separateProds(itemArray, clonedCollectedProdArray, &(item->hashedStateIds_ItemTransit));
}

int main() {
	DynamicArray *itemArray = createDynamicArray(20, false, dummy_member, ITEM);
	DynamicArray *initialProdArray = createDynamicArray(10, true, getKeyFromProd, PRODUCTION);
	appendCopy(initialProdArray, &productions[0], PRODUCTION);
	constructItem(itemArray, initialProdArray, &(dummy_item->hashedStateIds_ItemTransit), FIRST);//FIRST temporary
}


//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()

