#include <errno.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include "../../des_inputs/src/des.h"

int main(int argc, char* argv[]) {

	Display display;
	int chanelId;
	int response = 0;

	printf("The display is running as PID: %d\n", getpid());
	// Create channel
	chanelId = ChannelCreate(0);

	while (1) {
		int receiveId = MsgReceive(chanelId, &display, sizeof(display), NULL);

		if (receiveId == -1) {
			fprintf(stderr, "\nError receiving message from Controller\n");
			exit(EXIT_FAILURE);
		}

		if (strcmp(display.person.command, inMessage[LEFT_SCAN]) == 0
				|| strcmp(display.person.command, inMessage[RIGHT_SCAN]) == 0) {
			printf("\nPerson scanned ID, ID=%s  \n", display.person.person_id);
		}

		if (strcmp(display.person.command, inMessage[WEIGHT_SCALE]) == 0) {
			printf("\nPerson weighed, Weight=%s  \n", display.person.weight);

		}

		if (strcmp(display.person.command, inMessage[EXIT]) != 0) {
			printf("%s\n", display.msg);
		}

		int replyid = MsgReply(receiveId, EOK, &response, sizeof(response));

		if (replyid == -1) {
			fprintf(stderr, "\nError replying to Controller \n");
			exit(EXIT_FAILURE);
		}

		if (strcmp(display.person.command, inMessage[EXIT]) == 0) {
			break;
		}

	}

	printf("Exiting Display.\n");

	// Destroy the channel when done
	ChannelDestroy(chanelId);
	return EXIT_SUCCESS;
}
