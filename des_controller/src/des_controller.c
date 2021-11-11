#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/wait.h>

#include "../../des_inputs/src/des.h"

int main(int argc, char* argv[]) {
	// If args not supplied then show usage
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <controller_pid> \n", argv[0]);
		return EXIT_FAILURE;
	}

	int LDC = 1;
	int LDL = 1;
	int RDC = 1;
	int RDL = 1;

	pid_t PID = atoi(argv[1]);
	int rcvId;
	int rpyId;
	int chId;
	int displayConnect;
	Person person;
	Display display;
	int msgReply = 0;
	int displayReply;
	char send_msg[256];

	/* Creating a channel for Inputs to connect */
	chId = ChannelCreate(0);
	if (chId == -1) {
		fprintf(stderr, "\nFailed to create the channel.");
		exit(EXIT_FAILURE);
	}

	/* Connecting to channel created by Display */
	displayConnect = ConnectAttach(0, PID, 1, 0, 0);
	if (displayConnect == -1) {
		printf("\nCannot Connect to Display.");
		exit(EXIT_FAILURE);
	}

	printf("The controller is running as PID: %d\n", getpid());
	printf("Waiting for Person...\n");

	while (1) {
		strcpy(person.command, "");
		rcvId = MsgReceive(chId, &person, sizeof(person), NULL);
		strcpy(send_msg, "");

		if (strcmp(person.command, inMessage[LEFT_SCAN]) == 0) {

			if (!((LDC == 1) && (LDL == 1) && (RDC == 1) && (RDL == 1))) {
				strcpy(send_msg,
						"Someone else is trying to use the gate. Please wait your turn.");
			}
		} else if (strcmp(person.command, inMessage[RIGHT_SCAN]) == 0) {
			if (!((LDC == 1) && (LDL == 1) && (RDC == 1) && (RDL == 1))) {

				strcpy(send_msg,
						"Someone else is trying to use the gate. Please wait your turn.");
			}
		} else if (strcmp(person.command, inMessage[WEIGHT_SCALE]) == 0) {

			if ((LDC == 1) && (RDC == 1)) {

				strcpy(send_msg, "Cannot weight person. Door is closed.");
			}
		} else if (strcmp(person.command, inMessage[LEFT_OPEN]) == 0) {

			if (LDL == 0) {
				LDC = 0;
				strcpy(send_msg, "Person Opened Left Door");
			} else if (LDL == 1) {
				strcpy(send_msg, "Cannot open Left-door. Door is Locked");
			}

		} else if (strcmp(person.command, inMessage[RIGHT_OPEN]) == 0) {
			if (RDL == 0) {
				RDC = 0;
				strcpy(send_msg, "Person Opened Right Door");
			} else if (RDL == 1) {
				strcpy(send_msg, "Cannot open Right-door. Door is Locked");
			}

		} else if (strcmp(person.command, inMessage[LEFT_CLOSED]) == 0) {
			if (LDC == 0) {
				LDC = 1;
				strcpy(send_msg, "Left Door Closed (automatically)");
			} else if (LDC == 1) {
				strcpy(send_msg, "Left-Door is already closed.");
			}
		} else if (strcmp(person.command, inMessage[RIGHT_CLOSED]) == 0) {
			if (RDC == 0) {
				RDC = 1;
				strcpy(send_msg, "Right Door Closed (automatically)");
			} else if (RDC == 1) {
				strcpy(send_msg, "Right-Door is already closed.");
			}
		} else if (strcmp(person.command, inMessage[GUARD_RIGHT_UNLOCK]) == 0) {
			if (RDL == 0) {
				strcpy(send_msg, "Right-Door Is Already Unlocked.");
			} else if (RDL == 1) {
				RDL = 0;
				strcpy(send_msg, "Right Door Unlocked By Guard");
			}
		} else if (strcmp(person.command, inMessage[GUARD_RIGHT_LOCK]) == 0) {
			if (RDL == 1) {
				strcpy(send_msg, "Right-Door Is Already Locked.");
			} else if (RDL == 0) {
				RDL = 1;
				strcpy(send_msg, "Right Door Locked By Guard \n");
			}
		} else if (strcmp(person.command, inMessage[GUARD_LEFT_LOCK]) == 0) {
			if (LDL == 1) {
				strcpy(send_msg, "Left-Door Is Already Locked.");
			} else if (LDL == 0) {
				LDL = 1;
				strcpy(send_msg, "Left Door Locked By Guard");
			}
		} else if (strcmp(person.command, inMessage[GUARD_LEFT_UNLOCK]) == 0) {
			if (LDL == 0) {
				strcpy(send_msg, "Left-Door Is Already Unlocked.");
			} else if (LDL == 1) {
				LDL = 0;
				strcpy(send_msg, "Left Door Unlocked By Guard");
			}
		}

		display.person = person;
		strcpy(display.msg, send_msg);
		int responseid = MsgSend(displayConnect, &display, sizeof(display),
				&displayReply, sizeof(displayReply));

		if (responseid == -1) {
			fprintf(stderr, "\nError during MsgSend. Exiting. \n");
			exit(EXIT_FAILURE);
		}
		if (displayReply == -1) {
			printf("\nError receiving reply from Display\n");
		}

		rpyId = MsgReply(rcvId, 1, &msgReply, sizeof(msgReply));
		if (rpyId == -1) {
			printf("\nError replying to Inputs. Exiting. \n");
			return EXIT_FAILURE;
		}

		if (strcmp(person.command, inMessage[EXIT]) == 0) {
			break;
		}

	}
	printf("Exiting Controller.\n");
	// destroy the channel when done
	ChannelDestroy(chId);
	return EXIT_SUCCESS;
}
