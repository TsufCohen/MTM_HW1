//
// Created by safi azmi on 23/11/2018.
//

#include "mtm_citizen.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/**---------------------------------------------------------------------------------**/
/**------------------------------Candidate Priority functions-----------------------**/
/**---------------------------------------------------------------------------------**/


/**---------------------------------------------------------------------------------**/
/**------------------------------Candidate Priority Struct--------------------------**/
/**---------------------------------------------------------------------------------**/



typedef struct candidates_priority {
    int candidate_id;
    int priority;
} *Candidates_Priority;

/**---------------------------------------------------------------------------------**/
/**--------------------------creatCandidatePriority Function------------------------**/
/**---------------------------------------------------------------------------------**/

/**
 * function that allocate and create a new candidate priority struct and get's tow arguments
 * candidate id and the priority
 * return values: if the allocate fails returns NULL
 * in every other case a pointer to he new struct
 * */
static Candidates_Priority
creatCandidatePriority(int candidate_id, int candidate_priority) {
    Candidates_Priority new_priority = malloc(sizeof(*new_priority));
    if (new_priority == NULL) {
        return NULL;
    }
    new_priority->candidate_id = candidate_id;
    new_priority->priority = candidate_priority;
    return new_priority;

}

/**---------------------------------------------------------------------------------**/
/**----------Candidate Priority functions:copy,free,equal,greaterthan---------------**/
/**---------------------------------------------------------------------------------**/

Element copyCandidatePriority(Element priority) {
    Candidates_Priority current = priority;
    Candidates_Priority copy_of_priority = creatCandidatePriority(
            current->candidate_id, current->priority);
    if (copy_of_priority == NULL) {
        return NULL;
    }
    return copy_of_priority;

}

void freeCandidatePriority(Element priority) {
    if (priority == NULL) {
        return;
    }
    Candidates_Priority current = priority;
    free(current);
}

bool intCandidateIsEquals(Element e1, Element e2) {

    assert(e1 != NULL || e2 != NULL);
    Candidates_Priority priority_1 = e1;
    Candidates_Priority priority_2 = e2;
    if (priority_1->candidate_id == priority_2->candidate_id) {
        return true;
    }
    return false;
}

bool intCandidateIsGreaterThan(Element e1, Element e2) {
    Candidates_Priority priority_1 = e1;
    Candidates_Priority priority_2 = e2;
    if (priority_1->priority > priority_2->priority) {
        return true;
    }
    return false;
}

/**---------------------------------------------------------------------------------**/
/**-----------------------copyForString function------------------------------------**/
/**---------------------------------------------------------------------------------**/

/**
 * the function get a string arguments and copy it for other string
 * retrun values: if argument that was sent is null then return NULL
 * if the allocate fails it returns NULL
 * in every other case return a pointer to char(string)
 * */
 char *   copyForString(const char *name) {
    if (name == NULL) {
        return NULL;
    }
    char *name_copy = malloc(strlen(name) + 1);
    if (name_copy == NULL) {
        return NULL;
    }
    return strcpy(name_copy, name);
}

/**---------------------------------------------------------------------------------**/
/**-----------------------candidatePriorityExists function--------------------------**/
/**---------------------------------------------------------------------------------**/

/**the function gets two Element and returns if they  contain the same
 * value if yes then it returns true if not it return false
 * */

static bool candidatePriorityExists(Element citizen_1, Element citizen_2) {
    assert(citizen_1 != NULL || citizen_2 != NULL);
    if (intCandidateIsGreaterThan(citizen_1, citizen_2) == false &&
        intCandidateIsGreaterThan(citizen_2, citizen_1) == false) {
        return true;
    }
    return false;
}


/**----------------------------------------------------------------------------------**/
/**---------------------------citizen functions--------------------------------------**/
/**----------------------------------------------------------------------------------**/


/**----------------------------------------------------------------------------------**/
/**-----------------------------------Citizen struct---------------------------------**/
/**----------------------------------------------------------------------------------**/

struct citizen_t {
    char *name_t;
    int age_t;
    int id_t;
    int years_of_eduction_t;
    UniqueOrderedList candidates_priority_t;
};

/**---------------------------------------------------------------------------------**/
/**---------------------------------Citizen Create function--------------------------**/
/**---------------------------------------------------------------------------------**/

Citizen citizenCreate(const char *name, int age, int id, int years_of_eduction) {
    if (name == NULL || age <= 0 || id < 0 || years_of_eduction < 0) {
        return NULL;
    }
    Citizen new_citizen = malloc(sizeof(*new_citizen));
    if (new_citizen == NULL) {
        return NULL;
    }
    new_citizen->name_t = copyForString(name);
    if(new_citizen->name_t==NULL){
        free(new_citizen);
        return NULL;
    }
    new_citizen->age_t = age;
    new_citizen->id_t = id;
    new_citizen->years_of_eduction_t = years_of_eduction;
    new_citizen->candidates_priority_t = uniqueOrderedListCreate(
            copyCandidatePriority, freeCandidatePriority, intCandidateIsEquals,
            intCandidateIsGreaterThan);
    if (new_citizen->candidates_priority_t == NULL) {
        free(new_citizen->name_t);
        free(new_citizen);
        return NULL;
    }
    return new_citizen;
}
/**---------------------------------------------------------------------------------**/
/**---------------------------------getCitizenName function-------------------------**/
/**---------------------------------------------------------------------------------**/

char *getCitizenName(Citizen citizen) {
    if (citizen == NULL || citizen->name_t == NULL) {
        return NULL;
    }
    return copyForString(citizen->name_t);
}

/**---------------------------------------------------------------------------------**/
/**---------------------------------citizenCopy function----------------------------**/
/**---------------------------------------------------------------------------------**/

Citizen citizenCopy(
        Citizen citizen_to_copy) {                           /**some of the values i think we should not check but we will see later**/
    if (citizen_to_copy == NULL || citizen_to_copy->name_t == NULL ||
        citizen_to_copy->age_t <= 0 || citizen_to_copy->id_t < 0 ||
        citizen_to_copy->years_of_eduction_t < 0) {
        return NULL;
    }
    Citizen new_citizen = citizenCreate(citizen_to_copy->name_t,
                                       citizen_to_copy->age_t,
                                       citizen_to_copy->id_t,
                                       citizen_to_copy->years_of_eduction_t);
    if (new_citizen == NULL) {
        return NULL;
    }

    uniqueOrderedListDestroy(
            new_citizen->candidates_priority_t);           /**before we copy just to make sure that we set free the allocate we did in creating the list**/
    new_citizen->candidates_priority_t = uniqueOrderedListCopy(
            citizen_to_copy->candidates_priority_t);
    if (new_citizen->candidates_priority_t == NULL) {
        free(new_citizen->name_t);
        free(new_citizen);
        return NULL;                    /**cheak here if we should return error memory allocate or just null */
    }
    return new_citizen;
}

/**---------------------------------------------------------------------------------**/
/**---------------------------------destroyCitizen function--------------------------**/
/**---------------------------------------------------------------------------------**/

void destroyCitizen(Citizen citizen_to_destroy) {
    if(!citizen_to_destroy){
        return;
    }
    uniqueOrderedListDestroy(citizen_to_destroy->candidates_priority_t);
    free(citizen_to_destroy->name_t);          /**to ask here if with or with out the * **/
    free(citizen_to_destroy);
}

/**---------------------------------------------------------------------------------**/
/**---------------------------------getCitizenAge function--------------------------**/
/**---------------------------------------------------------------------------------**/

int getCitizenAge(Citizen citizen) {
    if (citizen == NULL || citizen->age_t <= 0) {
        return -1;
    }
    return citizen->age_t;
}

/**---------------------------------------------------------------------------------**/
/**---------------------------------getCitizenId function---------------------------**/
/**---------------------------------------------------------------------------------**/

int getCitizenId(Citizen citizen) {
    if (citizen == NULL || citizen->id_t < 0) {
        return -1;
    }
    return citizen->id_t;
}

/**---------------------------------------------------------------------------------**/
/**------------------------getCitizenYearsOfEduction function-----------------------**/
/**---------------------------------------------------------------------------------**/

int getCitizenYearsOfEduction(Citizen citizen) {
    if (citizen == NULL || citizen->years_of_eduction_t < 0) {
        return -1;
    }
    return citizen->years_of_eduction_t;
}

/**---------------------------------------------------------------------------------**/
/**---------------------------------citizenCompare function-------------------------**/
/**---------------------------------------------------------------------------------**/

int citizenCompare(Citizen h1, Citizen h2) {
    assert(h1 != NULL || h2 != NULL);
    if (h1->id_t == h2->id_t) {
        return 0;
    }
    return h1->id_t > h2->id_t ? 1:-1;
}

/**---------------------------------------------------------------------------------**/
/**-----------------------citizenBecomeCandidate function---------------------------**/
/**---------------------------------------------------------------------------------**/


CitizenResult citizenBecomeCandidate(Citizen citizen) {
    if (citizen==NULL || citizen->candidates_priority_t == NULL) {
        return CITIZEN_NULL_ARGUMENT;
    }
    UniqueOrderedList back_up_for_priority = uniqueOrderedListCopy(
            citizen->candidates_priority_t);   /**back up in case the allocation fails then we have to restore the program to how it was before this command*/
    if (back_up_for_priority == NULL) {
        return CITIZEN_OUT_OF_MEMORY;
    }
    uniqueOrderedListClear(citizen->candidates_priority_t);
    assert(uniqueOrderedListSize(citizen->candidates_priority_t) == 0);
    Candidates_Priority new_priority = creatCandidatePriority(citizen->id_t, 0);
    if (new_priority == NULL) {
        citizen->candidates_priority_t = back_up_for_priority;    /**here if the allocation fails then we restore the lost data back*/
        return CITIZEN_OUT_OF_MEMORY;
    }
    UniqueOrderedListResult temp = uniqueOrderedListInsert(
            citizen->candidates_priority_t, new_priority);
    if (temp == UNIQUE_ORDERED_LIST_OUT_OF_MEMORY) {
        freeCandidatePriority(new_priority);
        citizen->candidates_priority_t = back_up_for_priority;    /**here if the allocation fails then we restore the lost data back*/
        return CITIZEN_OUT_OF_MEMORY;
    }
    assert(temp == UNIQUE_ORDERED_LIST_SUCCESS);
    freeCandidatePriority(
            new_priority);        /**to ask and see but uniqueOrderedListInsert make a copy of the element so after that i have to free it so i will not have a memory leak**/
    uniqueOrderedListDestroy(
            back_up_for_priority);     /**destory the memory there is no need the function works*/
    return CITIZEN_SUCCESS;
}
/**---------------------------------------------------------------------------------**/
/**-----------------------removeCandidatePriority function--------------------------**/
/**---------------------------------------------------------------------------------**/

CitizenResult removeCandidatePriority(Citizen citizen, int candidate_id) {
    if (citizen==NULL || citizen->candidates_priority_t == NULL) {
        return CITIZEN_NULL_ARGUMENT;
    }
    if (citizen->id_t ==
        candidate_id) {       /**in case that the user want to delete for a candidate his own support we will not allow that!!! */
        return CITIZEN_MUST_SUPPORT;
    }
    Candidates_Priority candidate_priority_to_remove = creatCandidatePriority(
            candidate_id, 3);
    if (candidate_priority_to_remove == NULL) {
        return CITIZEN_OUT_OF_MEMORY;
    }
    UniqueOrderedListResult temp = uniqueOrderedListRemove(
            citizen->candidates_priority_t, candidate_priority_to_remove);
    assert(temp != UNIQUE_ORDERED_LIST_NULL_ARGUMENT);
    freeCandidatePriority(
            candidate_priority_to_remove);        /**freeing the memory that we allocate*/
    if (temp == UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST) {
        return CITIZEN_NOT_SUPPORT;
    }
    return CITIZEN_SUCCESS;
}
/**---------------------------------------------------------------------------------**/
/**------------------checkIfCitizenGotTHisPriority function-------------------------**/
/**---------------------------------------------------------------------------------**/

/**
 * function that gets a citizen and a candidate priority and find's if the citizen list of priority's
 * got the same priority that was sent in the candidate priority if not it insert the candidate
 * priority in the list of the citizen.return values:
 * if allocation falis CITIZEN_OUT_OF_MEMORY
 * if the priority exists then PRIORITY_EXISTS
 * in csae of success CITIZEN_SUCCESS
 * */
static CitizenResult checkIfCitizenGotThisPriority(Citizen citizen,
                                                   Candidates_Priority priority_to_check) {
    assert(citizen != NULL || priority_to_check != NULL);
    Element current_priority = uniqueOrderedListGetLowest(
            citizen->candidates_priority_t);
    if (current_priority == NULL) {
        UniqueOrderedListResult result1 = uniqueOrderedListInsert(
                citizen->candidates_priority_t, priority_to_check);
        if (result1 == UNIQUE_ORDERED_LIST_OUT_OF_MEMORY) {
            return CITIZEN_OUT_OF_MEMORY;
        }
        return CITIZEN_SUCCESS;
    }
    if (candidatePriorityExists(current_priority, priority_to_check) == true) {
        return PRIORITY_EXISTS;
    }
    current_priority = uniqueOrderedListGetNext(citizen->candidates_priority_t);
    while (current_priority != NULL) {
        if (candidatePriorityExists(current_priority, priority_to_check) ==
            true) {
            return PRIORITY_EXISTS;
        }
        current_priority = uniqueOrderedListGetNext(
                citizen->candidates_priority_t);
    }
    UniqueOrderedListResult result = uniqueOrderedListInsert(
            citizen->candidates_priority_t, priority_to_check);
    if (result == UNIQUE_ORDERED_LIST_OUT_OF_MEMORY) {
        return CITIZEN_OUT_OF_MEMORY;
    }
    return CITIZEN_SUCCESS;
}
/**---------------------------------------------------------------------------------**/
/**------------------------citizenNewCandidateInsert function-----------------------**/
/**---------------------------------------------------------------------------------**/


CitizenResult citizenNewCandidateInsert(Citizen citizen, int candidate_id,
                                        int priority_to_insert) {
    assert(citizen != NULL || candidate_id >= 0 || priority_to_insert >= 0);
    Candidates_Priority to_insert = creatCandidatePriority(candidate_id,
                                                           priority_to_insert);
    if (to_insert == NULL) {
        return CITIZEN_OUT_OF_MEMORY;
    }
    if (true ==
        uniqueOrderedListContains(citizen->candidates_priority_t, to_insert)) {
        freeCandidatePriority(to_insert);
        return CITIZEN_ALREADY_SUPPORTED;
    }
    Candidates_Priority if_citizen_candidate = creatCandidatePriority(
            citizen->id_t, 5);
    if (if_citizen_candidate == NULL) {
        freeCandidatePriority(to_insert);
        return CITIZEN_OUT_OF_MEMORY;
    }
    if(uniqueOrderedListSize(citizen->candidates_priority_t)>0) {
        if (intCandidateIsEquals(
                uniqueOrderedListGetLowest(citizen->candidates_priority_t),
                if_citizen_candidate) == true) {
            freeCandidatePriority(to_insert);
            freeCandidatePriority(if_citizen_candidate);
            return CITIZEN_CAN_NOT_SUPPORT;
        }
    }
    freeCandidatePriority(if_citizen_candidate);
    CitizenResult temp = checkIfCitizenGotThisPriority(citizen, to_insert);
    freeCandidatePriority(to_insert);
    return temp;
}


/**---------------------------------------------------------------------------------**/
/**-----------------------candidateBecomeCitizen function---------------------------**/
/**---------------------------------------------------------------------------------**/

CitizenResult candidateBecomeCitizen(Citizen citizen) {
    if (citizen == NULL) {
        return CITIZEN_NULL_ARGUMENT;
    }
    uniqueOrderedListClear(citizen->candidates_priority_t);
    return CITIZEN_SUCCESS;
}


/**---------------------------------------------------------------------------------**/
/**-----------------------getFirstPriority function---------------------------------**/
/**---------------------------------------------------------------------------------**/

int getFirstPriority(Citizen citizen ){
    if(!citizen){
        return -1;                          //erorr value
    }
    Candidates_Priority tmp =uniqueOrderedListGetLowest(citizen->candidates_priority_t);
    if(!tmp){
        return -2;                          //citizen is natural
    }
    return tmp->candidate_id;
}
/**---------------------------------------------------------------------------------**/
/**-----------------------greaterCitizenByNameOrId Function-------------------------**/
/**---------------------------------------------------------------------------------**/


bool greaterCitizenByNameOrId(Citizen citizen_1,Citizen citizen_2){
    assert( citizen_1!=NULL || citizen_2!=NULL);
    int tmp =strcmp(citizen_1->name_t,citizen_2->name_t);
    if(tmp==0){
        if(citizen_1->id_t < citizen_2->id_t){
            return true;
        }
        return false;
    }
    return tmp > 0 ? true:false;
}
/**---------------------------------------------------------------------------------**/
