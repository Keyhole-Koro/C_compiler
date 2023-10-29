#include "dynamicArray.h"

Data dummy_data_instance = {.intData = NULL };
Data *dummy_data = &dummy_data_instance;

DynamicArray *createDynamicArray(int initialCapacity, bool ifAllowModify, int (*referentMember)(Data*, Type), Type type) {
	DynamicArray *arr = (DynamicArray *)malloc(sizeof(DynamicArray));
	if (arr == NULL) error("Memory allocation failed\n");
	arr->data = (Data **)malloc(sizeof(Data *)*initialCapacity);
	if (arr->data == NULL) error("Memory allocation failed\n");
	
	arr->type = type;
	arr->offset = -1;
	arr->capacity = initialCapacity;
	arr->allowModify = ifAllowModify;
    
	if (referentMember != &dummy_member){
        arr->ifAllowOverlap = false;
		arr->ifExistingEleArray = createEmptyUnsignedCharArray(initialCapacity);
        arr->capacity_overlapArray = initialCapacity;
    } else {
        arr->ifAllowOverlap = true;
    }
    arr->referentMember = referentMember;
	
	initializeElementsInDynamicArray(arr, 0);
	
	return arr;
}

void initializeElementsInDynamicArray(DynamicArray *arr, int start_index) {
    for (int i = start_index; i < getArraySize(arr); i++) {
		printf("i: %d\n", i);
        arr->data[getArraySize(arr) + i] = NULL;
    }
}

void allowModify(DynamicArray *arr) {
	arr->allowModify = true;
}

void disableModify(DynamicArray *arr) {
	arr->allowModify = false;
}

void normalReallocateDynamicArray(DynamicArray *arr) {
	if (getArraySize(arr) == arr->capacity) {
		int previous_capacity_size = arr->capacity;
		arr->capacity *= 2;
		arr->data = realloc(arr->data, arr->capacity * sizeof(Data *));
		initializeElementsInDynamicArray(arr, previous_capacity_size+1);
		if (arr->data == NULL) error("Memory allocation failed\n");
	}
}
void append(DynamicArray *arr, void *element, Type type) {
    if (type != arr->type) error("type mismatch: append\n");

    normalReallocateDynamicArray(arr);

    if (ifExistInOverlap(arr, arr->referentMember((Data *)element, type))) return;
    
    Data *copied_ptr = (Data *)malloc(sizeof(element));
    if (copied_ptr == NULL) error("Memory allocation failed\n");
    copied_ptr = (Data *)element;
    arr->data[++arr->offset] = copied_ptr;
}

void appendCopy(DynamicArray *arr, void *element, Type type) {
    if (type != arr->type) error("type mismatch: append\n");
    
    normalReallocateDynamicArray(arr);

    if (ifExistInOverlap(arr, arr->referentMember((Data *)element, type))) return;
    
    Data *copy_data_ptr = (Data *)malloc(getDataSize(type));
    if (copy_data_ptr == NULL) error("Memory allocation failed\n");
	memcpy(copy_data_ptr, element, getDataSize(type));
    arr->data[++arr->offset] = copy_data_ptr;
}

void reallocOverlapArray(DynamicArray *arr, int index) {
    int previous_capacity_size = arr->capacity_overlapArray;

    if (index > arr->capacity_overlapArray) {
        int new_capacity = index * 1.2 * sizeof(unsigned char);
        
        arr->ifExistingEleArray = realloc(arr->ifExistingEleArray, new_capacity);
        arr->capacity_overlapArray = new_capacity;
        if (arr->ifExistingEleArray == NULL) error("Memory allocation failed\n");
        //iintialize
        for (int i = previous_capacity_size+1; i < new_capacity; i++) {
            arr->ifExistingEleArray[i] = 0;
        }
    }
}
bool ifExistInOverlap(DynamicArray *arr, int index) {
    if (arr->ifAllowOverlap) return false;
    if (index > 100) printf("warn: index exceeds 100: index == %d\n", index);

    reallocOverlapArray(arr, index);

    if (arr->ifExistingEleArray[index] == 1) return true;
    appendtoIndexOverlapArray(arr, index);
    return false;
}

void appendtoIndexOverlapArray(DynamicArray *arr, int index) {
	arr->ifExistingEleArray[index] = 1;
}

void swapElement(DynamicArray *arr, int pos1, int pos2, Type type) {
	if (arr->allowModify == false) error("not allowed to be modified: swapElement\n");
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

void initializeDynamicArray(DynamicArray *arr) {
	arr->type = -1;
	arr->data = &dummy_data;
	arr->ifAllowOverlap = false;
	//arr-> overlapArray
	arr->offset = -1;
	arr->capacity = -1;
	arr->allowModify = false;
}

void swapWithLastElement(DynamicArray *arr, int index, Type type) {
	swapElement(arr, index, getArrayOffset(arr), type);
}

void destroyDynamicArray(DynamicArray* arr) {
    for (int i = 0; i < getArraySize(arr); i++) free(arr->data[i]);
	free(arr->data);
	free(arr);
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
	if (arr->allowModify == false) error("not allowed to be modified: removeLastElement\n");
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

DynamicArray *fetchCommonElements(DynamicArray *arr, bool (customCmp)(Data*, Data*, Type), Data *expected_data, bool ifRemoveElement, Type type) {
    DynamicArray *commonElementsArr = createDynamicArray(getArraySize(arr), true, &dummy_member, type);
	int condition = getArraySize(arr);
    
    for (int i = 0; i < condition; i++) {
        Data *cmpedData = retriveData(arr, i, type);
                
        if (customCmp(cmpedData, expected_data, type)) {
			appendCopy(commonElementsArr, cmpedData, type);
			if (ifRemoveElement) {
				removeElement(arr, i, type);
				i--;
				condition--;
			}
		}
    }
    return commonElementsArr;
}

//bool cmpHash_tranSymbol(Data *data1, Data *data2) {}

//when hash and readSymbol are the same
Item *fetchMatchingData(DynamicArray *itemArray, DynamicArray *expectedProdArray, int expectedSymbol) {
    if (getArraySize(itemArray) == 0) return dummy_item;
    
    int expectedHashedKey = calculateHash(expectedProdArray, getProdKey, PRODUCTION);
    
    //may replace here to extractCertainData()
    for (int i = 0; getArraySize(itemArray); i++) {
        Item *item = (Item *)retriveData(itemArray, i, ITEM);
        if (expectedHashedKey == item->hashed_keys && expectedSymbol == item->readSymbol) return item;
    }
    
    return dummy_item;
}

void copyPasteArray(DynamicArray *copiedArr, DynamicArray *pastedArr) {
	if (copiedArr->type != pastedArr->type) error("type mismatch: copyPasteArray\n");
	for (int i = 0; i < getArraySize(copiedArr); i++) {
		appendCopy(pastedArr, retriveData(copiedArr, i, copiedArr->type), pastedArr->type);
    }
}

DynamicArray *cloneArray(DynamicArray *originalArr, bool ifAllowModify, int (*referentMember)(Data*, Type)) {
	Type type = originalArr->type;
	DynamicArray *clonedArray = createDynamicArray(getArraySize(originalArr), ifAllowModify, referentMember, type);

	for (int i = 0; i < getArraySize(originalArr); i++) {
        Data *d = retriveData(originalArr, i, type);
		appendCopy(clonedArray, d, type);
	}

	return clonedArray;
}
//please care about i in for() when you use this
void removeElement(DynamicArray *arr, int index, Type type) {
	if (arr->allowModify == false) error("not allowed to be modified: removeElement\n");
	if (type != arr->type) error("Type mismatch: swapRemoveElement\n");
	if (getArraySize(arr) < 2) return;
	swapWithLastElement(arr, index, type);
	removeLastElement(arr);
}

int calculateHash(DynamicArray *array, int (referentMember)(Data*, Type), Type type) {
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
			removeElement(arr, i, type);
			continue;
		}
		ifExistingArray[referentMember(data, type) - start_point] = 1;
	}
    destoryUnsignedCharArray(ifExistingArray);
}

Production *getProdFromItem(Item *item, Type type) {
	if (type != ITEM) error("type mismatch: getProdFromItem\n");
	Production *prod = *item->Productions;
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


