//
// Created by Tsuf Cohen on 25/11/2018
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mtm_city.h"
#include "mtm_citizen.h"
#include "set.h"

#define VOTERS_MINIMUM_AGE 17

struct mtm_city_t {
    char* cityName;
    int cityId;
    Set citizens;
    Set candidates;

};

/** ********* a candidate will be a positive integer(ID only) *************** */
// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/    the candidates functions for the set   \||/  *******************
 *****   \/           ***************************       \/   **************** */
static SetElement cityCopyCandidate(SetElement candidate){
    int* new_candidate = malloc(sizeof(*new_candidate));
    if(!new_candidate) {
        return NULL;
    }
    *new_candidate = *(int*)candidate;
    return new_candidate;
}

static void cityFreeCandidate(SetElement candidate){
    if(!candidate) {
        return;
    }
    free(candidate);
}

static int cityCompareCandidate(SetElement candidate1, SetElement candidate2){
    if((*(int*)candidate1)==(*(int*)candidate2)) {
        return 0;
    }
    else {
        return ((*(int*)candidate1)>(*(int*)candidate2) ? 1 : -1 );
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\    the candidates functions for the set   /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/   cast the citizen functions for the set  \||/  *******************
 *****   \/           ***************************       \/   **************** */
static SetElement cityCopyCitizen(SetElement citizen){
    return citizenCopy(citizen);
}

static void cityFreeCitizen(SetElement citizen){
    destroyCitizen(citizen);
}

static int cityCompareCitizen(SetElement citizen1, SetElement citizen2){
    return citizenCompare(citizen1, citizen2);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\   cast the citizen functions for the set  /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                  createCity               \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmCity createCity (int cityId, const char* cityName) {
    if(cityId<0 || !cityName) {
        return NULL;
    }
    MtmCity new_city = malloc(sizeof(*new_city));
    if(!new_city) {
        return NULL;
    }

    new_city->cityId = cityId;
    new_city->cityName = copyForString(cityName);
    if(!new_city->cityName) {
        free(new_city);
        return NULL;
    }
    new_city->citizens = setCreate(cityCopyCitizen, cityFreeCitizen,
                                                         cityCompareCitizen);
    if(!new_city->citizens) {
        free(new_city->cityName);
        free(new_city);
        return NULL;
    }
    new_city->candidates = setCreate(cityCopyCandidate, cityFreeCandidate,
                                                         cityCompareCandidate);
    if(!new_city->candidates) {
        free(new_city->cityName);
        setDestroy(new_city->citizens);
        free(new_city);
        return NULL;
    }

    return new_city;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                  createCity               /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                 freeCity               \||/  *******************
 *****   \/           ***************************       \/   **************** */
void freeCity (MtmCity mtmCity) {
    if(!mtmCity){
        return;
    }
    if(mtmCity->cityName) {
        free(mtmCity->cityName);
    }
    setDestroy(mtmCity->citizens);
    setDestroy(mtmCity->candidates);
    free(mtmCity);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                  freeCity               /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                 copyCity                  \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmCity copyCity (MtmCity mtmCity) {
    if(!mtmCity) {
        return NULL;
    }
    MtmCity new_copy = createCity(mtmCity->cityId, mtmCity->cityName);
    if(!new_copy) {
        return NULL;
    }
    setDestroy(new_copy->citizens);
    setDestroy(new_copy->candidates);
    new_copy->citizens = setCopy(mtmCity->citizens);
    if(!new_copy->citizens) {
        free(new_copy->cityName);
        return NULL;
    }
    new_copy->candidates = setCopy(mtmCity->candidates);
    if(!new_copy->candidates) {
        free(new_copy->cityName);
        setDestroy(new_copy->citizens);
    }
    return new_copy;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                  copyCity                 /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                 compareCity               \||/  *******************
 *****   \/           ***************************       \/   **************** */
int compareCity (MtmCity a, MtmCity b) {
    if(a->cityId==b->cityId){
        return 0;
    }
    else {
        return (a->cityId)>(b->cityId) ? 1 : -1;
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                  compareCity              /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/               mtmCityAddCitizen           \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmCityResult mtmCityAddCitizen(MtmCity mtmCity, const char *citizenName,
                                int citizenId, int citizenAge,
                                int yearsOfEducation) {
    if(!mtmCity|| !citizenName) {
        return MTM_CITIES_NULL_ARGUMENT;
    }
    else if(citizenId<0) {
        return MTM_CITIES_ILLEGAL_ID;
    }
    else if(citizenAge<=0) {
        return MTM_CITIES_ILLEGAL_AGE;
    }
    else if(yearsOfEducation<0) {
        return MTM_CITIES_ILLEGAL_NUMBER_OF_YEARS;
    }
    else {
        Citizen tmp_citizen = citizenCreate(citizenName, citizenAge, citizenId,
                                                              yearsOfEducation);
        if(!tmp_citizen) {
            return MTM_CITIES_MEMORY_ERROR;
        }
        switch(setAdd(mtmCity->citizens, tmp_citizen)) {
            case SET_NULL_ARGUMENT:
                cityFreeCitizen(tmp_citizen);
                return MTM_CITIES_NULL_ARGUMENT;
            case SET_ITEM_ALREADY_EXISTS:
                cityFreeCitizen(tmp_citizen);
                return MTM_CITIES_CITIZEN_ALREADY_EXISTS;
            case SET_SUCCESS:
                cityFreeCitizen(tmp_citizen);
                return MTM_CITIES_SUCCESS;
            default:
                cityFreeCitizen(tmp_citizen);
                return MTM_CITIES_MEMORY_ERROR;
        }
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              mtmCityAddCitizen            /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/            mtmCityRemoveCitizen           \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmCityResult mtmCityRemoveCitizen(MtmCity mtmCity, int citizenId) {
    if(!mtmCity) {
        return MTM_CITIES_NULL_ARGUMENT;
    }
    if(citizenId<0) {
        return MTM_CITIES_ILLEGAL_ID;
    }
    Citizen tmp_citizen = citizenCreate("name", 1, citizenId, 1);
    if(!tmp_citizen) {
        return MTM_CITIES_MEMORY_ERROR;
    }
    switch(setRemove(mtmCity->citizens, tmp_citizen)) {
        case SET_ITEM_DOES_NOT_EXIST:
            cityFreeCitizen(tmp_citizen);
            return MTM_CITIES_CITIZEN_DOES_NOT_EXIST;
        case SET_SUCCESS:
            switch(mtmCityWithdrawCandidate(mtmCity, citizenId)) {
                case MTM_CITIES_CANDIDATE_DOES_NOT_EXIST:
                case MTM_CITIES_SUCCESS:
                    cityFreeCitizen(tmp_citizen);
                    return MTM_CITIES_SUCCESS;
                default:
                    setAdd(mtmCity->citizens, tmp_citizen);
                    cityFreeCitizen(tmp_citizen);
                    return MTM_CITIES_MEMORY_ERROR;
            }
        default:
            cityFreeCitizen(tmp_citizen);
            return MTM_CITIES_MEMORY_ERROR;
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              mtmCityRemoveCitizen         /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/            mtmCityContainCitizen          \||/  *******************
 *****   \/           ***************************       \/   **************** */
bool mtmCityContainCitizen(MtmCity mtmCity, int citizenId) {
    if(!mtmCity || citizenId<0) {
        return false;
    }

    Citizen tmp_citizen = citizenCreate("tmp_citizen", 1, citizenId, 0);
    if(!tmp_citizen) {
        return false;
    }
    if(setIsIn(mtmCity->citizens, tmp_citizen)){
        cityFreeCitizen(tmp_citizen);
        return true;
    }
    else {
        cityFreeCitizen(tmp_citizen);
        return false;
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\            mtmCityContainCitizen          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/   functions to get citizen information    \||/  *******************
 *****   \/           ***************************       \/   **************** */
char* mtmCityGetCitizenName(MtmCity mtmCity, int citizenId) {
    if(!mtmCity || citizenId<0) {
        return NULL;
    }
    SET_FOREACH(Citizen, iterator, mtmCity->citizens){
        if(getCitizenId(iterator)==citizenId){
            return getCitizenName(iterator);
        }
        else {
            continue;
        }
    }
    return NULL;
}

// *****************************************************************************

int mtmCityGetCityId(MtmCity mtmCity) {
    if(!mtmCity) {
        return -1;
    }
    int tmp_city_id = mtmCity->cityId;
    return tmp_city_id;
}

// *****************************************************************************

char* mtmCityGetCityName(MtmCity mtmCity) {
    if(!mtmCity) {
        return NULL;
    }
   return copyForString(mtmCity->cityName);
}

// *****************************************************************************

int mtmCityGetCitizenAge(MtmCity mtmCity, int citizenId) {
    if(!mtmCity || citizenId<0) {
        return -1;
    }
    SET_FOREACH(Citizen, iterator, mtmCity->citizens){
        if(getCitizenId(iterator)==citizenId){
            return getCitizenAge(iterator);
        }
        else {
            continue;
        }
    }
    return -1;
}

// *****************************************************************************

int mtmCityGetCitizenEducation(MtmCity mtmCity, int citizenId) {
    if(!mtmCity || citizenId<0) {
        return -1;
    }
    SET_FOREACH(Citizen, iterator, mtmCity->citizens){
        if(getCitizenId(iterator)==citizenId){
            return getCitizenYearsOfEduction(iterator);
        }
        else {
            continue;
        }
    }
    return -1;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\   functions to get citizen information    /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/        functions to handle candidates     \||/  *******************
 *****   \/           ***************************       \/   **************** */
bool mtmCityContainCandidate(MtmCity mtmCity, int candidateId) {
    if(!mtmCity || candidateId<0) {
        return false;
    }
    return setIsIn(mtmCity->candidates, &candidateId);

}

// *****************************************************************************

MtmCityResult mtmCityAddCandidate(MtmCity mtmCity, int candidateId) {
    if(!mtmCity) {
        return MTM_CITIES_NULL_ARGUMENT;
    }
    if(candidateId<0) {
        return MTM_CITIES_ILLEGAL_ID;
    }
    switch(setAdd(mtmCity->candidates, &candidateId)) {
        case SET_ITEM_ALREADY_EXISTS:
            return MTM_CITIES_CANDIDATE_ALREADY_EXISTS;
        case SET_SUCCESS:
            SET_FOREACH(Citizen, iterator, mtmCity->citizens) {
                if(getCitizenId(iterator)==candidateId){
                    switch(citizenBecomeCandidate(iterator)) {
                        case CITIZEN_SUCCESS:
                            return MTM_CITIES_SUCCESS;
                        default:
                            setRemove(mtmCity->candidates, &candidateId);
                            return MTM_CITIES_MEMORY_ERROR;
                    }
                }
                else {
                    continue;
                }
            }
            setRemove(mtmCity->candidates, &candidateId);
            return MTM_CITIES_CITIZEN_DOES_NOT_EXIST;
        default:
            return MTM_CITIES_MEMORY_ERROR;
    }
}

// *****************************************************************************

MtmCityResult mtmCityWithdrawCandidate(MtmCity mtmCity, int candidateId) {
    if(!mtmCity) {
        return MTM_CITIES_NULL_ARGUMENT;
    }
    if(candidateId<0) {
        return MTM_CITIES_ILLEGAL_ID;
    }
    Set backup_citizens = setCopy(mtmCity->citizens);
    if(!backup_citizens) {
        return MTM_CITIES_MEMORY_ERROR;
    }
    switch(setRemove(mtmCity->candidates, &candidateId)) {
        case SET_ITEM_DOES_NOT_EXIST:
            setDestroy(backup_citizens);
            return MTM_CITIES_CANDIDATE_DOES_NOT_EXIST;
        case SET_SUCCESS:
            SET_FOREACH(Citizen, iterator, mtmCity->citizens) {
                switch(removeCandidatePriority(iterator, candidateId)) {
                    case CITIZEN_NOT_SUPPORT:
                    case CITIZEN_SUCCESS:
                        continue;
                    case CITIZEN_MUST_SUPPORT:
                        switch(candidateBecomeCitizen(iterator)) {
                            case CITIZEN_SUCCESS:
                                continue;
                            default:
                                setDestroy(mtmCity->citizens);
                                mtmCity->citizens=backup_citizens;
                                setAdd(mtmCity->candidates, &candidateId);
                                return MTM_CITIES_MEMORY_ERROR;
                        }
                    default:
                        setDestroy(mtmCity->citizens);
                        mtmCity->citizens=backup_citizens;
                        setAdd(mtmCity->candidates, &candidateId);
                        return MTM_CITIES_MEMORY_ERROR;
                }
            }
            setDestroy(backup_citizens);
            return MTM_CITIES_SUCCESS;
        default:
            setDestroy(backup_citizens);
            return MTM_CITIES_MEMORY_ERROR;
    }
}

// *****************************************************************************

MtmCityResult mtmCitySupportCandidate(MtmCity mtmCity, int citizenId,
                                      int candidateId, int priority) {
    if(!mtmCity) {
        return MTM_CITIES_NULL_ARGUMENT;
    }
    if(candidateId<0 || citizenId<0) {
        return MTM_CITIES_ILLEGAL_ID;
    }
    if(priority<0) {
        return MTM_CITIES_ILLEGAL_PRIORITY;
    }
    if(!setIsIn(mtmCity->candidates, &candidateId)){
        return MTM_CITIES_CANDIDATE_DOES_NOT_EXIST;
    }
    SET_FOREACH(Citizen, iterator, mtmCity->citizens) {
        if(getCitizenId(iterator)==citizenId){
            switch(citizenNewCandidateInsert(iterator,candidateId,priority)){
                case CITIZEN_ALREADY_SUPPORTED:
                    return MTM_CITIES_ALREADY_SUPPORTED;
                case CITIZEN_CAN_NOT_SUPPORT:
                    return MTM_CITIES_CAN_NOT_SUPPORT;
                case PRIORITY_EXISTS:
                    return MTM_CITIES_PRIORITY_EXISTS;
                case CITIZEN_SUCCESS:
                    return MTM_CITIES_SUCCESS;
                default:
                    return MTM_CITIES_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_CITIES_CITIZEN_DOES_NOT_EXIST;
}

// *****************************************************************************

MtmCityResult mtmCityCancelSupport(MtmCity mtmCity, int citizenId,
                                   int candidateId) {
    if(!mtmCity) {
        return MTM_CITIES_NULL_ARGUMENT;
    }
    if(candidateId<0 || citizenId<0) {
        return MTM_CITIES_ILLEGAL_ID;
    }
    if(!setIsIn(mtmCity->candidates, &candidateId)){
        return MTM_CITIES_CANDIDATE_DOES_NOT_EXIST;
    }
    SET_FOREACH(Citizen, iterator, mtmCity->citizens) {
        if(getCitizenId(iterator)==citizenId){
            switch(removeCandidatePriority(iterator, candidateId)) {
                case CITIZEN_MUST_SUPPORT:
                    return MTM_CITIES_MUST_SUPPORT;
                case CITIZEN_NOT_SUPPORT:
                    return MTM_CITIES_NOT_SUPPORTED;
                case CITIZEN_SUCCESS:
                    return MTM_CITIES_SUCCESS;
                default:
                    return MTM_CITIES_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_CITIES_CITIZEN_DOES_NOT_EXIST;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\        functions to handle candidates     /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmCityGetCitizen          \||/  *******************
 *****   \/           ***************************       \/   **************** */
Citizen mtmCityGetCitizen(MtmCity mtmCity, int citizenId) {
    if(!mtmCity || citizenId<0) {
        return NULL;
    }
    SET_FOREACH(Citizen, iterator, mtmCity->citizens) {
        if(getCitizenId(iterator)==citizenId) {
            return iterator;
        }
    }
    return NULL;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\             mtmCityGetCitizen             /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmCityGetCitizens         \||/  *******************
 *****   \/           ***************************       \/   **************** */
Set mtmCityGetCitizens(MtmCity mtmCity) {
    if(!mtmCity) {
        return NULL;
    }
    return setCopy(mtmCity->citizens);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\             mtmCityGetCitizens            /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/                mtmCityGetCandidates       \||/  *******************
 *****   \/           ***************************       \/   **************** */
Set mtmCityGetCandidates(MtmCity mtmCity) {
    if(!mtmCity) {
        return NULL;
    }

    return setCopy(mtmCity->candidates);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\             mtmCityGetCandidates          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/               mtmCityGetMayor             \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmCityResult mtmCityGetMayor(MtmCity mtmCity, int* mayor) {
    if(!mtmCity || !mayor) {
        return MTM_CITIES_NULL_ARGUMENT;
    }
    if(!setGetSize(mtmCity->candidates)) {
        return MTM_CITIES_NO_CANDIDATES_IN_CITY;
    }
    int max_votes=0;
    Citizen current_winner=NULL;
    SET_FOREACH(int*, candidate_iterator, mtmCity->candidates) {
        int current_votes=0;
        SET_FOREACH(Citizen, citizen_iterator, mtmCity->citizens) {
            if(getFirstPriority(citizen_iterator)==*candidate_iterator) {
                if(getCitizenAge(citizen_iterator)<=VOTERS_MINIMUM_AGE) {
                    continue;
                }
                else {
                    current_votes++;
                }
            }
        }
        if(!current_winner || current_votes>max_votes) {
            current_winner=mtmCityGetCitizen(mtmCity, *candidate_iterator);
            max_votes=current_votes;
            continue;
        }
        if(current_votes==max_votes) {
            Citizen tmp_citizen=mtmCityGetCitizen(mtmCity, *candidate_iterator);
            if(greaterCitizenByNameOrId(current_winner, tmp_citizen)){
                current_winner=tmp_citizen;
            }
        }
    }
    *mayor=getCitizenId(current_winner);
    return MTM_CITIES_SUCCESS;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\                mtmCityGetMayor            /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************
