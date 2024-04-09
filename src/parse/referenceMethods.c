#include "referenceMethods.h"

bool isNonTerminal(int symbol) {
	return symbol > NON_TERMINAL_START;
}

int getIntFromData(void *data, DataType *dataType) {
	if (strcmp(dataType->name, "int")) error("type mismatch: getIntFromData\n");
	return *(int *)data;
}

int getIntFromDataForSymbol(void *data, DataType *dataType) {
	if (strcmp(dataType->name, "int")) error("type mismatch: getIntFromDataForSymbol\n");
    int sym = *(int *)data;
    if (sym > NON_TERMINAL_START) return abs(sym - NON_TERMINAL_START) + END_TERMINAL;
    if (TERMIANL_START < sym && sym < END_TERMINAL) return sym;
    return NONE;
}
