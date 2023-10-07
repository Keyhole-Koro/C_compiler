#include "dynamicArray.h"

/*
 deprioritize()
 fetchedProdArray:
 duplicatedProdsArray: 0 1 2 3 4 5
 originalProd: 0 1 2 3 4 5
*/
DynamicArray *createDynamicArray(int initialCapacity, bool ifModifiable, bool ifOverlap, Type type) {
	DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
	if (arr == NULL) error("Memory allocation failed\n");
	arr->data = (Data **)malloc(sizeof(Data *)*initialCapacity);
	if (arr->data == NULL) error("Memory allocation failed\n");
	arr->type = type;
	arr->offset = -1;
	arr->capacity = initialCapacity;
	arr->modifiable = ifModifiable;

	arr->ifOverlap = ifOverlap;
	if (!ifOverlap){
		arr->ifOverlapArray = createEmptyUnsignedCharArray(initialCapacity);
		arr->start_point = getStartPoint(type);
	}
	return arr;
}

void normalReallocateDynamicArray(DynamicArray *arr) {
	if (arr->offset == arr->capacity) {
		arr->capacity *= 2;
		arr->data = realloc(arr->data, arr->capacity * SIZE_OF_A_MEMORY);
		if (arr->data == NULL) error("Memory allocation failed\n");
	}
}
void append(DynamicArray *arr, void *element, Type type) {
	if (!arr->ifOverlap) error("you are supposed to use append: append\n");
	conditionalAppend(arr, element, dummy_member, type);
}

void appendCopy(DynamicArray *arr, void *element, Type type) {
	if (!arr->ifOverlap) error("you are supposed to use append: appendCopy\n");
	conditionalAppendCopy(arr, element, dummy_member, type);
}

void conditionalAppend(DynamicArray *arr, void *element, int (referentMember)(Data*, Type), Type type) {
	if (arr->ifOverlap) error("you are supposed to use normal append: conditionalAppend\n");
	if (type != arr->type) error("type mismatch: append\n");

	normalReallocateDynamicArray(arr);

	whenOverlapFalse(arr, referentMember(element, type));

	Data *copied_ptr = (Data *)malloc(sizeof(element));
    copied_ptr = (Data *)element;
	arr->data[++arr->offset] = copied_ptr;
}

void conditionalAppendCopy(DynamicArray *arr, void *element, int (referentMember)(Data*, Type), Type type) {
	if (arr->ifOverlap) error("you are supposed to use normal append: conditionalAppendCopy\n");
	if (type != arr->type) error("type mismatch: append\n");
	
	normalReallocateDynamicArray(arr);

	whenOverlapFalse(arr, referentMember(element, type));

	Data *copy_data_ptr = (Data *)malloc(getDataSize(type));
	*copy_data_ptr = *(Data *)element;
	arr->data[++arr->offset] = copy_data_ptr;
}

void whenOverlapFalse(DynamicArray *arr, int index) {
	if (!arr->ifOverlap) {
		int eventual_index = index - arr->start_point;
        if (eventual_index > 100) printf("warning: eventual_index == %d\n", eventual_index);
		if (arr->ifOverlapArray[eventual_index] == 1) return;
		appendAtIndexOverlapArray(arr, eventual_index);
	}
}

void appendAtIndexOverlapArray(DynamicArray *arr, int index) {
	printf("index: %d\n", index);
	if (index > arr->capacity) {
		arr->data = realloc(arr->data, index * sizeof(unsigned char));
		if (arr->data == NULL) error("Memory allocation failed\n");
	}
	arr->ifOverlapArray[index] = 1;
}

void swapElement(DynamicArray *arr, int pos1, int pos2, Type type) {
	if (arr->modifiable == false) error("not allowed to be modified: swapElement\n");
	if (type != arr->type) {
		error("type mismatch: swapElement\n");
		return;
	}

	if (pos1 < 0 || pos1 > getArrayOffset(arr) || pos2 < 0 || pos2 > getArrayOffset(arr)) {
		error("Index out of bounds: swapElement\n");
		return;
	}
	Data **data = arr->data;

	Data *temp_element = data[pos1];
	data[pos1] = data[pos2];
	data[pos2] = temp_element;
}

bool hasOverlap_Insert(DynamicArray *arr, int index) {
	unsigned char *ifOverlapArray = arr->ifOverlapArray;
	if (ifOverlapArray[index] == 1) return true;
	ifOverlapArray[index] = 1;
	return false;
}

void initializeDynamicArray(DynamicArray *arr) {
	arr->type = -1;
	arr->data = &dummy_data;
	arr->ifOverlap = false;
	//arr-> overlapArray
	arr->offset = -1;
	arr->capacity = -1;
	arr->modifiable = false;
}

void swapWithLastElement(DynamicArray *arr, int pos, Type type) {
	swapElement(arr, pos, getArrayOffset(arr), type);
}


void destroyDynamicArray(DynamicArray* arr) {
	free(arr->data);
	free(arr);
}
//add safety
bool cmpStateId(Data *data, Data *expectedValue) {
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->stateId == *compedValue;
}

bool cmpTransitionedSymbol(Data* data, Data* expectedValue) {
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->transitionedSymbol == *compedValue;
}

int getElementKey(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getElementKey\n");
	Production *prod = (Production *)data;
	return prod->key;
}

int dummy_member(Data *data, Type type) {
	return -1;
}

Data *retriveData(DynamicArray *arr, int pos, Type type) {
	if (type != arr->type) error("type mismatch: retriveData\n");

	if (pos < 0 || pos > getArrayOffset(arr)) error("Index out of bounds: retriveData\n");

	return arr->data[pos];
}

void removeLastElement(DynamicArray *arr) {
	free(arr->data[getArrayOffset(arr)]);
	arr->offset--;
}

int getArrayOffset(DynamicArray* arr) {
	return arr->offset;
}

int getArraySize(DynamicArray *arr) {
	return arr->offset + 1;
}

//this only fetch one data but muiltiple
int fetchPosition(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type) {
	if (type != arr->type) error("Type mismatch: fetchPosition\n");
	Data *d;
	for (int i = 0; i < getArraySize(arr); i++) {
		d = retriveData(arr, i, type);
		if (customCmp(d, expectedValue)) return i;
	}
	error("Not found\n");
	return -1;
}

//bool cmpHash_tranSymbol(Data *data1, Data *data2) {}

//when hash and transitionedSymbol are the same
Item *fetchMatchingItem(DynamicArray *itemArray, DynamicArray *expectedProdArray, int expectedSymbol) {
    if (getArraySize(itemArray) == 0) return dummy_item;
    
    int expectedHashedKey = calculateArrayHash(expectedProdArray, getElementKey, PRODUCTION);
    
    //may replace here to extractCertainData()
    for (int i = 0; getArraySize(itemArray); i++) {
        Item *item = (Item *)retriveData(itemArray, i, ITEM);
        if (expectedHashedKey == item->hashed_keys && expectedSymbol == item->transitionedSymbol) return item;
    }
    
    return dummy_item;
}

DynamicArray *cloneArray(DynamicArray *originalArr, bool ifModifiable) {
	Type type = originalArr->type;
	DynamicArray *duplicatedArray = createDynamicArray(getArraySize(originalArr), ifModifiable, true, type);

	for (int i = 0; i < getArraySize(originalArr); i++) {
		Data *copied_data = (Data *)malloc(getDataSize(type));
		memcpy(copied_data, retriveData(originalArr, i, type), getDataSize(type));
		append(duplicatedArray, copied_data, type);
	}

	return duplicatedArray;
}

void swapRemoveElement(DynamicArray *arr, int pos_deprioritized, Type type) {
	if (type != arr->type) error("Type mismatch: swapRemoveElement\n");
	if (getArraySize(arr) < 2) return;
	swapWithLastElement(arr, pos_deprioritized, type);
	removeLastElement(arr);
}

int calculateArrayHash(DynamicArray *array, int (referentMember)(Data*, Type), Type type) {
	int hash = 0;

	for (int i = 0; i < getArraySize(array); i++) {
		Data *data = retriveData(array, i, type);
		hash ^= referentMember(data, type);
	}

	return hash;
}

//this only return one data
Data *extractCertainData(DynamicArray *arr, bool (customCmp)(Data*, Data* ,Type), Data *expected_data, Type type) {
    for (int i = 0; i < getArraySize(arr); i++) {
        Data *data = retriveData(arr, i, type);
        if (customCmp(data, expected_data, type)) return data;
    }
    return dummy_data;//not yet defined
}

//may not needed
void eliminateOverlap(DynamicArray *arr, int (referentMember)(Data*, Type), int start_point, Type type) {
	int numElements = getArraySize(arr);
    unsigned char *ifExistingArray = createEmptyUnsignedCharArray(numElements);
    
	for (int i = 0; i < numElements; i++) {
		Data *data = retriveData(arr, i, type);
		if (ifExistingArray[referentMember(data, type) - start_point] == 1) {
			swapRemoveElement(arr, i, type);
			continue;
		}
		ifExistingArray[referentMember(data, type) - start_point] = 1;
	}
    destoryUnsignedCharArray(ifExistingArray);
}

Production *getProdFromItem(Item *item, Type type) {
	if (type != ITEM) error("type mismatch: getProdFromItem\n");
	Production *prod = item->Productions;
	return prod;
}


unsigned char *createEmptyUnsignedCharArray(int size) {
	unsigned char *unsignedCharArray = (unsigned char *)calloc(size, sizeof(unsigned char));
	initializeUnsignedCharArraytoZero(unsignedCharArray);
	return unsignedCharArray;
}

void initializeUnsignedCharArraytoZero(unsigned char *unsignedCharArray) {
	for (int i = 0; i < sizeof(unsignedCharArray); i++) {
		unsignedCharArray[i] = 0;
	}
}

void destoryUnsignedCharArray(unsigned char *arr) {
    free(arr);
}

unsigned char *initializeOverlapArray(DynamicArray *existingElementArr, int (referentMember)(Data*, Type), Type type) {
	unsigned char *ifExistingArray = createEmptyUnsignedCharArray(existingElementArr->capacity);
	
	initializeUnsignedCharArraytoZero(ifExistingArray);
	
	//to eliminate excess prod which overlap with fetchedProdArray
	for (int i = 0; i < getArraySize(existingElementArr); i++) {
		Data *data = retriveData(existingElementArr, i, type);
		ifExistingArray[referentMember(data, type)] = 1;
	}
	return ifExistingArray;
}

int qsortPartition(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type) {
	if (type != PRODUCTION) error("type mismatch: qsortPartition\n");
	Data *high_prod = retriveData(arr, high, PRODUCTION);
	int pivot = referentFunc(high_prod, type);
	Data *j_prod;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		j_prod = retriveData(arr, j, PRODUCTION);
		if (referentFunc(j_prod, type) < pivot) {
			i++;
			swapElement(arr, i, j, PRODUCTION);
		}
	}
	swapElement(arr, i + 1, high, PRODUCTION);
	return (i + 1);
}

int getStartPoint(Type type) {
	int NON_TERMINAL_START = 256;//tmp
	if (type == PRODUCTION) return NON_TERMINAL_START;
	return 0;
}
//sort productions according the number of symbols
void quickSort(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type) {
	if (type != PRODUCTION) error("type mismatch: quickSort\n");
	if (low < high) {
		int pi = qsortPartition(arr, referentFunc, low, high, PRODUCTION);

		quickSort(arr, referentFunc, low, pi - 1, PRODUCTION);
		quickSort(arr, referentFunc, pi + 1, high, PRODUCTION);
	}
}

