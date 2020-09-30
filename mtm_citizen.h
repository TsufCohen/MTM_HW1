//
// Created by safi azmi on 23/11/2018.
//

#ifndef HW1_CITIZEN_H
#define HW1_CITIZEN_H

#include <stdbool.h>
#include "uniqueOrderedList/uniqueOrderedList.h"


/** Result type which is used for returning error values
 * from Citizen functions
 * */

typedef enum citizenResult_t {
    CITIZEN_OUT_OF_MEMORY,
    CITIZEN_SUCCESS,
    CITIZEN_NULL_ARGUMENT,
    CITIZEN_MUST_SUPPORT,
    CITIZEN_NOT_SUPPORT,
    CITIZEN_ALREADY_SUPPORTED,
    CITIZEN_CAN_NOT_SUPPORT,
    PRIORITY_EXISTS
}CitizenResult ;


typedef struct citizen_t *Citizen;

/**
 function that allocate memory if it fails to do so it returns a NULL value
 also NULL will be the retrn value if it gets a NUll argument or if one of the int values
 is smaller than zero expect the first int it must be greater than zero.
 in every other case it return a new citizen addrees.
 * */

Citizen citizenCreate(const char *name, int age, int id, int years_of_eduction);

/**
 * the function allocate a new memory and return new_citizen address if it success
 * * if citizen to copy is a null it will return a null value also it will have a null value when
 * *if the memory allocate fail in creat the new citizen **/

Citizen citizenCopy(Citizen);


/**
* Deallocates an existing citizen. Clears all elements by using the
* stored free function.
*/

void destroyCitizen(Citizen);

/**
 * duplicate the citizen name and return the address of the copy back if the copy process fails then return NULL
 * if the argument that sent was null then also it will return a null value
 */

char *getCitizenName(Citizen);


/**
 * the function gets the age of the citizen if the argument that was sent is null
 * then it will returns -1 in every another case it will return the int value of the citizen age
 */
int getCitizenAge(Citizen);

/**
 * the function gets the ID of the citizen if the argument that was sent is null
 * then it will returns -1 in every another case it will return the int value of the citizen ID
 */

int getCitizenId(Citizen);


/**
 * the function gets the years of eduction  of the citizen if the argument that was sent is null
 * then it will returns -1 in every another case it will return the int value of the citizen years of eduction
 */
int getCitizenYearsOfEduction(Citizen);

/**
 * the function gets to arguments of type Citizen and return if the values are
 * equal or not it returns 0 for equal and 1 if the first argument bigger than the second and
 * -1 if the second bigger than the first
 * */
int citizenCompare(Citizen,Citizen);

/**the function argument is a Citizen
 * the function allocate memory for back up in case the allocation failed
 * the function updates the priority of the citizen only to vote for himself
 * and delete the old priority's.
 * return value :CITIZEN_NULL_ARGUMENT if the argument that wsa sent is NULL
 * in case the allocate fails the value will be  CITIZEN_OUT_OF_MEMORY and
 * in every other case CITIZEN_SUCCESS
 * */
CitizenResult citizenBecomeCandidate(Citizen);

/**
 * the function get tow arguments a citizen and id for a candidate and we remove
 * the candidate priority from the citizen list of priority.
 * return values : CITIZEN_NULL_ARGUMENT if a null argument was sent
 * if the allocate fails then CITIZEN_OUT_OF _MEMORY
 * if the citizen is also the candidate that we want to remove then CITIZEN_MUST_SUPPORT and we dont change his priority
 * if the citizen dose not support this candidate then CITIZEN_NOT_SUPPORT
 * in every other case it return CITIZEN_SUCCESS
 * */

CitizenResult   removeCandidatePriority(Citizen citizen,int candidate_id);

/**
 * the function check if we can insert to the citizen new candidate priority
 * the function gets three arguments Citizen & the candidate id and the priority in this same order
 * return values:
 * if there is allocation memory problem CITIZEN_OUT_OF_MEMORY
 * if the citizen already support the candidate CITIZEN_ALREADY_SUPPORTED
 * if the citizen is a candidate then CITIZEN_CAN_NOT_SUPPORT
 * if the citizen got a priority that equals the one that was sent PRIORITY_EXISTS
 * in every other case CITIZEN_SUCCESS
 * **/
CitizenResult   citizenNewCandidateInsert(Citizen citizen,int candidate_id,int priority_to_insert);


/**
 * the function get a citizen argument and delete his list of priority's
 * return value:
 * if the argument is null then CITIZEN_NULL_ARGUMENT
 * in every other case CITIZEN_SUCCESS
 * */


CitizenResult candidateBecomeCitizen(Citizen);


/**
 * the function get a string arguments and copy it for other string
 * retrun values: if argument that was sent is null then return NULL
 * if the allocate fails it returns NULL
 * in every other case return a pointer to char(string)
 * */
char *   copyForString(const char *name);


/**
 * the function gets a citizen argument and return to witch candidate he will vote
 * return value's:-1 for Null argument that was sent to the function
 * -2 for a natural citizen that vot for no one
 * in every other case the candidate id that the citizen will vot to
 * **/

int getFirstPriority(Citizen);



/**
 * the function gets tow citizen arguments and compare how is bigger by there Lexicography
 * of there name's if they have the same name's then by there id's.
 * return value: true if citizen_1 name is bigger or his id number is smaller in every other case false
 * **/

bool greaterCitizenByNameOrId(Citizen citizen_1,Citizen citizen_2);



#endif //HW1_CITIZEN_H
