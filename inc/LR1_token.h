#ifndef LR1_TOKEN_H
#define LR1_TOKEN_H

enum {
    NONE = -1,

	NON_TERMINAL_START = 256,
	EXPRRESSION, // E
	TERM, // T
	FACTOR, // F
	ACCEPTED, //accept

	END, //$ temporary

	S,  // shift
	R, // reducce
	G, //goto
	ACC, //acc
};

#endif