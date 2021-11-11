/*
 * des.h
 *
 *  Created on: Feb 14, 2021
 *      Author: hurdleg
 */

#ifndef DOOR_ENTRY_SYSTEM_H_
#define DOOR_ENTRY_SYSTEM_H_

#define NUM_STATES 0	// where n = number of states in your finite state machine (FSM)
typedef enum {
	NO_ERROR = 0
} State;

#define NUM_INPUTS 12

typedef enum {
	LEFT_SCAN = 0,
	RIGHT_SCAN = 1,
	WEIGHT_SCALE = 2,
	LEFT_OPEN = 3,
	RIGHT_OPEN = 4,
	LEFT_CLOSED = 5,
	RIGHT_CLOSED = 6,
	GUARD_RIGHT_LOCK = 7,
	GUARD_LEFT_LOCK = 8,
	GUARD_LEFT_UNLOCK = 9,
	GUARD_RIGHT_UNLOCK = 10,
	EXIT = 11
} Input;

const char *inMessage[NUM_INPUTS] = { "ls", "rs", "ws", "lo", "ro", "lc", "rc",
		"grl", "gll", "glu", "gru", "exit" };

#define NUM_OUTPUTS 0	// where n equals the number of output messages from the FSM.
typedef enum {
	NO_ERROR2 = 0
} Output;

const char *outMessage[NUM_OUTPUTS] = {
//TODO :: each output message. For example, "Person opened left door"
		};

// inputs client sends a Person struct to its server, the controller
typedef struct {
	char person_id[10];
	char weight[10];
	char command[5];
	int state;
} Person;

// controller client sends a Display struct to its server, the display
typedef struct {
	Person person;
	char msg[200];
} Display;

#endif /* DOOR_ENTRY_SYSTEM_H_ */
