#include "dynamicArray.h"

/*
 deprioritize()
 fetchedProdArray:
 duplicatedProdsArray: 0 1 2 3 4 5
 originalProd: 0 1 2 3 4 5
*/

Data dummy_data_instance = {.intData = NULL };
Data *dummy_data = &dummy_data_instance;

DynamicArray *createDynamicArray(int initialCapacity, bool ifModifiable, int (*referentMember)(Data*, Type), Type type) {
	DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
	if (arr == NULL) error("Memory allocation failed\n");
	arr->data = (Data **)malloc(sizeof(Data *)*initialCapacity);
	if (arr->data == NULL) error("Memory allocation failed\n");
	arr->type = type;
	arr->offset = -1;
	arr->capacity = initialCapacity;
	arr->modifiable = ifModifiable;
    
	if (referentMember != &dummy_member){
        arr->ifOverlap = false;
		arr->ifOverlapArray = createEmptyUnsignedCharArray(initialCapacity);
    } else {
        arr->ifOverlap = true;
    }
    arr->referentMember = referentMember;
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
    if (type != arr->type) error("type mismatch: append\n");

    normalReallocateDynamicArray(arr);

    whenOverlapFalse(arr, arr->referentMember(element, type));

    Data *copied_ptr = (Data *)malloc(sizeof(element));
    copied_ptr = (Data *)element;
    arr->data[++arr->offset] = copied_ptr;
}

void appendCopy(DynamicArray *arr, void *element, Type type) {
    if (type != arr->type) error("type mismatch: append\n");
    
    normalReallocateDynamicArray(arr);

    whenOverlapFalse(arr, arr->referentMember(element, type));

    Data *copy_data_ptr = (Data *)malloc(getDataSize(type));
    *copy_data_ptr = *(Data *)element;
    arr->data[++arr->offset] = copy_data_ptr;
}

void whenOverlapFalse(DynamicArray *arr, int index) {
	if (!arr->ifOverlap) {
        if (index > 100) printf("warning: eventual_index == %d\n", index);
		if (arr->ifOverlapArray[index] == 1) return;
		appendAtIndexOverlapArray(arr, index);
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
    for (int i = 0; i < getArraySize(arr); i++) free(arr->data[i]);
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

int getProdKey(Data *data, Type type) {
    if (type != PRODUCTION) error("type mismatch: getProdKey\n");
    Production *prod = (Production *)data;
    return prod->key;
}

DynamicArray *fetchCommonElements(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data *expected_data, Type type) {
    DynamicArray *commonElementsArr = createDynamicArray(getArraySize(arr), true, &dummy_member, type);
    for (int i = 0; i < getArraySize(arr); i++) {
        Data *cmpedData = retriveData(arr, i, type);
        if (customCmp(cmpedData, expected_data)) appendCopy(commonElementsArr, cmpedData, type);
    }
    return commonElementsArr;
}

//bool cmpHash_tranSymbol(Data *data1, Data *data2) {}

//when hash and transitionedSymbol are the same
Item *fetchMatchingData(DynamicArray *itemArray, DynamicArray *expectedProdArray, int expectedSymbol) {
    if (getArraySize(itemArray) == 0) return dummy_item;
    
    int expectedHashedKey = calculateArrayHash(expectedProdArray, getProdKey, PRODUCTION);
    
    //may replace here to extractCertainData()
    for (int i = 0; getArraySize(itemArray); i++) {
        Item *item = (Item *)retriveData(itemArray, i, ITEM);
        if (expectedHashedKey == item->hashed_keys && expectedSymbol == item->transitionedSymbol) return item;
    }
    
    return dummy_item;
}

DynamicArray *cloneArray(DynamicArray *originalArr, bool ifModifiable) {
	Type type = originalArr->type;
	DynamicArray *duplicatedArray = createDynamicArray(getArraySize(originalArr), ifModifiable, originalArr->referentMember, type);

	for (int i = 0; i < getArraySize(originalArr); i++) {
		appendCopy(duplicatedArray, retriveData(originalArr, i, type), type);
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
	if (type != arr->type) error("type mismatch: qsortPartition\n");
	Data *high_prod = retriveData(arr, high, type);
	int pivot = referentFunc(high_prod, type);
	Data *j_prod;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		j_prod = retriveData(arr, j, type);
		if (referentFunc(j_prod, type) < pivot) {
			i++;
			swapElement(arr, i, j, type);
		}
	}
	swapElement(arr, i + 1, high, type);
	return (i + 1);
}

//sort productions according the number of symbols
void quickSort(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type) {
	if (type != arr->type) error("type mismatch: quickSort\n");
	if (low < high) {
		int pi = qsortPartition(arr, referentFunc, low, high, type);

		quickSort(arr, referentFunc, low, pi - 1, type);
		quickSort(arr, referentFunc, pi + 1, high, type);
	}
}

bool cmpInt(Data* data1, Data* data2) {
    return *(int *)data1 == *(int *)data2;
}
int main() {
    initializeProduction(dummy_prod);
    initializeItem(dummy_item);
    // Test Case 1: Creating and appending elements to a dynamic array
    DynamicArray *arr = createDynamicArray(5, true, dummy_member, INT);
    DynamicArray *arrCopy = createDynamicArray(5, true, dummy_member, INT);
    int element1 = 10;
    int element2 = 20;
    int element3 = 20;
    
    int *ptr_ele1 = &element1;
    int *ptr_ele2 = &element2;
    int *ptr_ele3 = &element3;
    append(arr, ptr_ele1, INT);
    append(arr, ptr_ele2, INT);
    append(arr, ptr_ele3, INT);
    
    DynamicArray *cloneArr = cloneArray(arr, arr->modifiable);

    // Test Case 2: Swapping elements in the dynamic array
    printf("Original Array: ");
    for (int i = 0; i <= getArrayOffset(arr); ++i) {
        int *value = (int *)retriveData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    DynamicArray *fetchedArray = fetchCommonElements(arr, cmpInt, (Data *)ptr_ele1, INT);

    printf("extract 20: ");
    for (int i = 0; i <= getArrayOffset(fetchedArray); ++i) {
        int *value = (int *)retriveData(fetchedArray, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    
    return 0;
}
