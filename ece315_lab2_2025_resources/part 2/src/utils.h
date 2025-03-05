/*
 * utils.h
 *
 *  Created on: Jan 17, 2025
 *      Author: Antonio Andara Lara
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#define MAX_USERS 3
#define MAX_LEN         32
#define HASH_LENGTH     32
#define HASH_STR_SIZE   ((2 * HASH_LENGTH) + 1)

/* Structure for login data from the user */
typedef struct {
    char username[MAX_LEN];
    char password[MAX_LEN];
} LoginData;

/* Structure for a registered user (only the hash is stored) */
typedef struct {
    char hashString[HASH_STR_SIZE];
} RegisteredUser;

/* User array */
RegisteredUser registeredUsers[MAX_USERS] = {
    { "FB5CA52079B973132E55445FDD20D7A242D42A97371BADEB1B80E5392CFF2E1D" },
    { "B747045056725C26DC380ECBB966E084E7CAF69440C5A064EFA40F56D20043EC" },
};

int registeredUserCount = 2;

bool loggedIn = false;

TickType_t xPollPeriod = 100U;

#endif /* SRC_UTILS_H_ */
