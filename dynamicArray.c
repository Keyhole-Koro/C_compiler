#include "dynamicArray.h"
	
/*
DynamicArray*
|
|->Data **
	 |     --------------
	 | -> |ptr0|ptr1|ptr2|
		   --------------
	 ----    ----    ----
	|val0|  |val1|  |val2|
	 ----    ----    ----
 value are separated
 */

/*
 deprioritize()
 fetchedProdArray:
 duplicatedProdsArray: 0 1 2 3 4 5
 originalProd: 0 1 2 3 4 5
 ----
 fetchedProdArray: 2
 duplicatedProdsArray: 0 1 5 3 4//2 is removed
 originalProd: 0 1 2 3 4 5//doesnt be changed
 */

/*
 swapElement()
 fetchedSymbols: E T F
 F apperd
 ----
 fetchedSymbols: F T E
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
		arr->ifOverlapArray = createNormalUnsignedCharArray(initialCapacity);
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
void normalAppend(DynamicArray *arr, void *element, Type type) {
	if (!arr->ifOverlap) error("you are supposed to use append: normalAppend\n");
	append(arr, element, dummy_member, type);
}

void normalAppendCopied(DynamicArray *arr, void *element, Type type) {
	if (!arr->ifOverlap) error("you are supposed to use append: normalAppendCopied\n");
	appendCopied(arr, element, dummy_member, type);
}

void append(DynamicArray *arr, void *element, int (referentMember)(Data*, Type), Type type) {
	if (arr->ifOverlap) error("you are supposed to use normal append: append\n");
	if (type != arr->type) error("type mismatch: append\n");

	normalReallocateDynamicArray(arr);

	whenOverlapFalse(arr, referentMember(element, type));

	Data **copy_data = (Data **)malloc(sizeof(element));
	*copy_data = (Data *)element;
	arr->data[++arr->offset] = *copy_data;
}

void appendCopied(DynamicArray *arr, void *element, int (referentMember)(Data*, Type), Type type) {
	if (arr->ifOverlap) error("you are supposed to use normal append: appendCopied\n");
	if (type != arr->type) error("type mismatch: append\n");
	
	normalReallocateDynamicArray(arr);

	whenOverlapFalse(arr, referentMember(element, type));

	Data **copy_data = (Data **)malloc(sizeof(element));
	**copy_data = (Data)*element;
	arr->data[++arr->offset] = *copy_data;
}

void whenOverlapFalse(DynamicArray *arr, int index) {
	if (!arr->ifOverlap) {
		int eventual_index = index - start_point;
		if (arr->ifOverlapArray[eventual_index] == 1) return;
		appendOverlaArray(arr, eventual_index);
	}
}

void appendOverlapArray(DynamicArray *arr, int index) {
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

	if (pos1 < 0 || pos1 > getOffset(arr) || pos2 < 0 || pos2 > getOffset(arr)) {
		error("Index out of bounds: swapElement\n");
		return;
	}
	Data **data = arr->data;

	Data *temp_element = data[pos1];
	data[pos1] = data[pos2];
	data[pos2] = temp_element;
}

bool isOverlap_Insert_1(DynamicArray *arr, int index) {
	unsigned char *ifOverlapArray = arr->ifOverlapArray;
	if (ifOverlapArray[index] == 1) return true;
	ifOverlapArray[index] = 1;
	return false;
}

void initializeDynamicArray(DynamicArray *arr) {
	arr->type = -1;
	arr->data = dummy_data;
	arr->ifOverlap = false;
	//arr-> overlapArray
	arr->offset = -1;
	arr->capacity = -1;
	arr->modifiable = false;
}

void swapWithLastElement(DynamicArray *arr, int pos, Type type) {
	swapElement(arr, pos, getOffset(arr), type);
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

int getKey(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getKey\n");
	Production *prod = (Production *)data;
	return prod->key;
}

int dummy_member(Data *data, Type type) {
	return -1;
}

Data *getData(DynamicArray *arr, int pos, Type type) {
	if (type != arr->type) error("type mismatch: getData\n");

	if (pos < 0 || pos > getOffset(arr)) error("Index out of bounds: getData\n");

	return arr->data[pos];
}

void removeLastElement(DynamicArray *arr) {
	free(arr->data[getOffset(arr)]);
	arr->offset--;
}

int getOffset(DynamicArray* arr) {
	return arr->offset;
}

int getNumElements(DynamicArray *arr) {
	return arr->offset + 1;
}

//this only fetch one data but muiltiple
int fetchPosition(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type) {
	if (type != arr->type) error("Type mismatch: fetchPosition\n");
	Data *d;
	for (int i = 0; i < getNumElements(arr); i++) {
		d = getData(arr, i, type);
		if (customCmp(d, expectedValue)) return i;
	}
	error("Not found\n");
	return -1;
}

DynamicArray *fetchMultiPositions(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data *expectedValue, Type type) {
	if (type != arr->type) error("Type mismatch: fetchPosition\n");
	DynamicArray *dArr = createDynamicArray(getNumElements(arr), false, INT);
	int offset = 0;
	for (int i = 0; i < getNumElements(arr); i++) {
		Data *d = getData(arr, i, type);
		int *n = malloc(sizeof(int));
		*n = i;
		if (customCmp(d, expectedValue)) append(dArr, &n, INT);
	}
	return dArr;
}

DynamicArray *duplicateArray(DynamicArray *originalArr, bool ifModifiable) {
	Type type = originalArr->type;
	DynamicArray *duplicatedArray = createDynamicArray(getNumElements(originalArr), ifModifiable, type);

	for (int i = 0; i < getNumElements(originalArr); i++) {
		Data *copied_data = (Data *)malloc(getDataSize(type));
		memcpy(copied_data, getData(originalArr, i, type), getDataSize(type));
		append(duplicatedArray, copied_data, type);
	}

	return duplicatedArray;
}


void swapRemoveElement(DynamicArray *arr, int pos_deprioritized, Type type) {
	if (type != arr->type) error("Type mismatch: swapRemoveElement\n");
	if (getNumElements(arr) < 2) return;
	swapWithLastElement(arr, pos_deprioritized, type);
	removeLastElement(arr);
}

int calculateSetHash(DynamicArray *array, int (referentMember)(Data*, Type), Type type) {
	if (type != PRODUCTION) error("type mismatch: calculateSetHash\n");
	int hash = 0;

	for (int i = 0; i < getNumElements(array); i++) {
		Data *data = getData(array, i, type);
		hash ^= referentMember(data, type);
	}

	return hash;
}

DynamicArray *extractInCommon(DynamicArray *arr, int (referentMember)(Data*, Type), int expected_
value, Type type)

//not needed
void eliminateOverlap(DynamicArray *arr, int (referentMember)(Data*, Type), Type type) {
	int numElements = getNumElements(arr);
	unsigned char ifExistingArray[numElements];

	for (int i = 0; i < numElements; i++) {
		ifExistingArray[i] = 0;
	}
	for (int i = 0; i < getNumElements(arr); i++) {
		Data *data = getData(arr, i, type);
		if (ifExistingArray[referentMember(data, type)] == 1) {
			swapRemoveElement(arr, i, type);
			continue;
		}
		ifExistingArray[referentMember(data, type)] = 1;
	}
}

Production *getProd_Item(Item *item, Type type) {
	if (type != ITEM) error("type mismatch: getProd_Item\n");
	Production *prod = item->Productions;
	return prod;
}

unsigned char *createNormalUnsignedCharArray(int size) {
	unsigned char *unsignedCharArray = (unsigned char *)calloc(size, sizeof(unsigned char));
	initializeUnsignedCharArraywithZero(unsignedCharArray);
	return unsignedCharArray;
}

void initializeUnsignedCharArraywithZero(unsigned char *unsignedCharArray) {
	for (int i = 0; i < sizeof(unsignedCharArray); i++) {
		unsignedCharArray[i] = 0;
	}
}

unsigned char *setUpifOverlapArray(DynamicArray *existingElementArr, int (referentMember)(Data*, Type), Type type) {
	unsigned char *ifExistingArray = createNormalUnsignedCharArray(existingElementArr->capacity);
	
	initializeUnsignedCharArraywithZero(ifExistingArray);
	
	//to eliminate excess prod which overlap with fetchedProdArray
	for (int i = 0; i < getNumElements(existingElementArr); i++) {
		Data *data = getData(existingElementArr, i, type);
		ifExistingArray[referentMemebr(data, type)] = 1;
	}
	return ifExistingArray;
}

int qsortPartition(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type) {
	if (type != PRODUCTION) error("type mismatch: qsortPartition\n");
	Data *high_prod = getData(arr, high, PRODUCTION);
	int pivot = referentFunc(high_prod, type);
	Data *j_prod;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		j_prod = getData(arr, j, PRODUCTION);
		if (referentFunc(j_prod, type) < pivot) {
			i++;
			swapElement(arr, i, j, PRODUCTION);
		}
	}
	swapElement(arr, i + 1, high, PRODUCTION);
	return (i + 1);
}

int getStartPoint(Type type) {
	int NON_TERMIANL_START = 256;//tmp
	if (type == PRODUCTION) return NON_TERMINA_START;
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

