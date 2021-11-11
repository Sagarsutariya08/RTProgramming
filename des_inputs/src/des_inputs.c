#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <sys/types.h>
#include "../../des_inputs/src/des.h"

int main(int argc, char *argv[]) {

	// If args not supplied then show usage
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <controller_pid> \n", argv[0]); // used fprintf to format output
		return EXIT_FAILURE;
	}

	pid_t controllerPid = atoi(argv[1]);

	int res; //response
	Person person;

	int conId = ConnectAttach(ND_LOCAL_NODE, controllerPid, 1, 0, 0);

	if (conId == -1) {
		printf("Can't Connect to Server. Please try again :-O \n");
		return EXIT_FAILURE;
	}

	while (1) {
		/* Display all the options */
		printf(
				"Enter the device (ls= left scan, rs= right scan, ws= weight scale, lo =left open, ro=right open, lc =left closed, rc = right closed , gru = guard right unlock, grl = guard right lock, gll=guard left lock, glu = guard left unlock, exit=exit programs): \n");
		scanf(" %s", person.command);

		// If one of the door is scanned, prompt for person_id
		if (strcmp(person.command, inMessage[LEFT_SCAN]) == 0
				|| strcmp(person.command, inMessage[RIGHT_SCAN]) == 0) {
			printf("\nEnter the Person's ID: \n");
			scanf(" %s", person.person_id);
		}
		// If ws then prompt for weight
		else if (strcmp(person.command, inMessage[WEIGHT_SCALE]) == 0) {
			printf("\nEnter the Person's weight: \n");
			scanf(" %s", person.weight);
		}

		int resid = MsgSend(conId, &person, sizeof(person) + 1, &res,
				sizeof(res));

		if (resid == -1) {
			fprintf(stderr, "Error during MsgSend\n");
			exit(EXIT_FAILURE);
		}

		//exit
		if (strcmp(person.command, "Exit") == 0) {
			break;
		}

		if (res == -1) {
			fprintf(stderr,
					"\nError receiving reply from Controller - Input-Event");
		}
	}

	ConnectDetach(conId);
	return EXIT_SUCCESS;
}
