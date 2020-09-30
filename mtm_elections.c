//
// Created by Tsuf Cohen on 25/11/2018
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "set.h"
#include "mtm_elections.h"
#include "mtm_city.h"
#include "mtm_citizen.h"
#include "mtm_print.h"

#define CANDIDATE_MINIMUM_AGE 21
#define VOTERS_MINIMUM_AGE 17

struct mtm_elections_t {
    Set cities;
};

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/   cast the city functions for the set     \||/  *******************
 *****   \/           ***************************       \/   **************** */
static SetElement electionsCopyCity(SetElement city){
    return copyCity(city);
}

static void electionsFreeCity(SetElement city){
    freeCity(city);
}

static int electionsCompareCity(SetElement city1, SetElement city2){
    return compareCity(city1, city2);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\    cast the city functions for the set    /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/              mtmElectionsCreate           \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmElections mtmElectionsCreate() {
    MtmElections new_elections = malloc(sizeof(*new_elections));
    if(!new_elections) {
        return NULL;
    }
    new_elections->cities = setCreate(electionsCopyCity, electionsFreeCity,
                                                         electionsCompareCity);
    if(!new_elections->cities) {
        free(new_elections);
        return NULL;
    }
        return new_elections;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\               mtmElectionsCreate          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/              mtmElectionsDestroy          \||/  *******************
 *****   \/           ***************************       \/   **************** */
void mtmElectionsDestroy(MtmElections mtmElections) {
    setDestroy(mtmElections->cities);
    free(mtmElections);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              mtmElectionsDestroy          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/              mtmElectionsAddCity          \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmElectionsResult mtmElectionsAddCity(MtmElections mtmElections,
                                       const char *cityName, int cityId) {
    if(!mtmElections || !cityName) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    else if(cityId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    else {
        MtmCity tmp_city = createCity(cityId, cityName);
        if(!tmp_city) {
            return MTM_ELECTIONS_MEMORY_ERROR;
        }
        switch(setAdd(mtmElections->cities, tmp_city)) {
            case SET_ITEM_ALREADY_EXISTS:
                freeCity(tmp_city);
                return MTM_ELECTIONS_CITY_ALREADY_EXISTS;
            case SET_SUCCESS:
                freeCity(tmp_city);
                return MTM_ELECTIONS_SUCCESS;
            default:
                freeCity(tmp_city);
                return MTM_ELECTIONS_MEMORY_ERROR;
        }
    }
}

//******************************************************************************
/** use to check if city already exist and get the city name*/
static char* getCityName(MtmElections mtmElections, int cityId) {
    if(!mtmElections || cityId<0) {
        return NULL;
    }
    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==cityId) {
            return mtmCityGetCityName(iterator);
        }
        else {
            continue;
        }
    }
    return NULL;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              mtmElectionsAddCity          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/              mtmElectionsAddCitizen       \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmElectionsResult mtmElectionsAddCitizen(MtmElections mtmElections,
                                          const char *citizenName,
                                          int citizenId,
                                          int citizenAge, int yearsOfEducation,
                                          int cityId) {
    if(!mtmElections || !citizenName) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(cityId<0 || citizenId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    if(citizenAge<=0) {
        return MTM_ELECTIONS_ILLEGAL_AGE;
    }
    if(yearsOfEducation<0) {
        return MTM_ELECTIONS_ILLEGAL_NUMBER_OF_YEARS;
    }
    int tmp_city_id;
    switch (MtmElectionsCitizenGetCity(mtmElections, citizenId, &tmp_city_id)) {
        case MTM_ELECTIONS_SUCCESS:
            return MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS;
        default:
            break;
    }
    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==cityId) {
            switch(mtmCityAddCitizen(iterator, citizenName, citizenId,
                                     citizenAge, yearsOfEducation)) {
                case MTM_CITIES_SUCCESS:
                    return MTM_ELECTIONS_SUCCESS;
                default:
                    return MTM_ELECTIONS_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\              mtmElectionsAddCitizen       /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/   functions to get citizen information    \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmElectionsResult MtmElectionsCitizenGetName(MtmElections mtmElections,
                                              int citizenId, char **name){
    if(!mtmElections || !name){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    else if(citizenId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    else {
        SET_FOREACH(MtmCity, iterator, mtmElections->cities){
            if(mtmCityContainCitizen(iterator, citizenId)){
                *name = mtmCityGetCitizenName(iterator, citizenId);
                if(!(*name)){
                    return MTM_ELECTIONS_MEMORY_ERROR;
                }
                else {
                    return MTM_ELECTIONS_SUCCESS;
                }
            }
            else {
                continue;
            }
        }
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
}

// *****************************************************************************

MtmElectionsResult MtmElectionsCitizenGetCity(MtmElections mtmElections,
                                              int citizenId, int *cityId) {
    if(!mtmElections || !cityId){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    else if(citizenId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    else {
        SET_FOREACH(MtmCity, iterator, mtmElections->cities){
            if(mtmCityContainCitizen(iterator, citizenId)){
                *cityId = mtmCityGetCityId(iterator);
                if((*cityId)<0){
                    return MTM_ELECTIONS_MEMORY_ERROR;
                }
                else {
                    return MTM_ELECTIONS_SUCCESS;
                }
            }
            else {
                continue;
            }
        }
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
}

// *****************************************************************************

MtmElectionsResult MtmElectionsCitizenGetAge(MtmElections mtmElections,
                                             int citizenId, int *age) {
    if(!mtmElections || !age){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    else if(citizenId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    else {
        SET_FOREACH(MtmCity, iterator, mtmElections->cities){
            if(mtmCityContainCitizen(iterator, citizenId)){
                *age = mtmCityGetCitizenAge(iterator, citizenId);
                if((*age)<0){
                    return MTM_ELECTIONS_MEMORY_ERROR;
                }
                else {
                    return MTM_ELECTIONS_SUCCESS;
                }
            }
            else {
                continue;
            }
        }
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
}

// *****************************************************************************

MtmElectionsResult MtmElectionsCitizenGetEducation(MtmElections mtmElections,
                                                   int citizenId,
                                                   int *yearsOfEducation) {
    if(!mtmElections || !yearsOfEducation){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(citizenId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    SET_FOREACH(MtmCity, iterator, mtmElections->cities){
        if(mtmCityContainCitizen(iterator, citizenId)){
            *yearsOfEducation = mtmCityGetCitizenEducation(iterator, citizenId);
            if((*yearsOfEducation)<0){
                return MTM_ELECTIONS_MEMORY_ERROR;
            }
            else {
                return MTM_ELECTIONS_SUCCESS;
            }
        }
        else {
            continue;
        }
    }
    return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
}

// *****************************************************************************

static MtmElectionsResult getTotalCitizenDetails(MtmElections mtmElections,
                                                 int citizenId, char **name,
                                                 int *age, int *education,
                                                 int *cityId) {
    switch(MtmElectionsCitizenGetCity(mtmElections, citizenId, cityId)) {
        case MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST:
            return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
        case MTM_ELECTIONS_SUCCESS:
            break;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    switch(MtmElectionsCitizenGetName(mtmElections, citizenId, name)) {
        case MTM_ELECTIONS_SUCCESS:
            break;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    switch(MtmElectionsCitizenGetAge(mtmElections, citizenId, age)) {
        case MTM_ELECTIONS_SUCCESS:
            break;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    switch(MtmElectionsCitizenGetEducation(mtmElections, citizenId, education)){
        case MTM_ELECTIONS_SUCCESS:
            break;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }
    return MTM_ELECTIONS_SUCCESS;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\   functions to get citizen information    /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/        functions to handle candidates     \||/  *******************
 *****   \/           ***************************       \/   **************** */
static MtmElectionsResult getCandidateCityId(MtmElections mtmElections,
                                             int candidateId, int *cityId) {
    if(!mtmElections || !cityId){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    else if(candidateId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    else {
        SET_FOREACH(MtmCity, iterator, mtmElections->cities){
            if(mtmCityContainCandidate(iterator, candidateId)){
                *cityId = mtmCityGetCityId(iterator);
                if((*cityId)<0){
                    return MTM_ELECTIONS_MEMORY_ERROR;
                }
                else {
                    return MTM_ELECTIONS_SUCCESS;
                }
            }
            else {
                continue;
            }
        }
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
}

// *****************************************************************************

static bool isTheCitizenInThisCity(MtmElections mtmElections, int citizenId,
                                   int cityId) {
    if(!mtmElections){
        return false;
    }
    if(citizenId<0 || cityId<0) {
        return false;
    }

    int tmp_id=0;
    MtmElectionsCitizenGetCity(mtmElections, citizenId, &tmp_id);
    if(tmp_id==cityId) {
        return true;
    }
    else {
        return false;
    }
}

// *****************************************************************************

MtmElectionsResult mtmElectionsAddCandidate(MtmElections mtmElections,
                                            int candidateId, int cityId) {
    if(!mtmElections){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(candidateId<0 || cityId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    char* tmp_city_name=getCityName(mtmElections, cityId);
    if(!tmp_city_name) {
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }
    free(tmp_city_name);

    if(!isTheCitizenInThisCity(mtmElections, candidateId, cityId)){
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }

    int tmp_age;
    MtmElectionsCitizenGetAge(mtmElections, candidateId, &tmp_age);
    if(tmp_age<CANDIDATE_MINIMUM_AGE) {
        return MTM_ELECTIONS_AGE_NOT_APPROPRIATE;
    }

    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==cityId) {
            switch(mtmCityAddCandidate(iterator, candidateId)) {
                case MTM_CITIES_CANDIDATE_ALREADY_EXISTS:
                    return MTM_ELECTIONS_CANDIDATE_ALREADY_EXISTS;
                case MTM_CITIES_SUCCESS:
                    return MTM_ELECTIONS_SUCCESS;
                default :
                    return MTM_ELECTIONS_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_ELECTIONS_MEMORY_ERROR;
}

// *****************************************************************************

MtmElectionsResult mtmElectionsWithdrawCandidate(MtmElections mtmElections,
                                                 int candidateId, int cityId) {
    if(!mtmElections){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(candidateId<0 || cityId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    else {
        SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
            if(mtmCityGetCityId(iterator)==cityId) {
                switch(mtmCityWithdrawCandidate(iterator, candidateId)) {
                    case MTM_CITIES_CANDIDATE_DOES_NOT_EXIST:
                        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
                    case MTM_CITIES_SUCCESS:
                        return MTM_ELECTIONS_SUCCESS;
                    default :
                        return MTM_ELECTIONS_MEMORY_ERROR;
                }
            }
            else {
                continue;
            }
        }
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }

}

// *****************************************************************************

MtmElectionsResult mtmElectionsSupportCandidate(MtmElections mtmElections,
                                                int citizenId, int candidateId,
                                                int priority) {
    if(!mtmElections){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(candidateId<0 || citizenId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    if(priority<0) {
        return MTM_ELECTIONS_ILLEGAL_PRIORITY;
    }

    int tmp_citizen_city_id;
    switch(MtmElectionsCitizenGetCity(mtmElections, citizenId,
                                      &tmp_citizen_city_id)) {
        case MTM_ELECTIONS_SUCCESS:
            break;
        case MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST:
            return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    int tmp_candidate_city_id;
    switch(getCandidateCityId(mtmElections,candidateId,&tmp_candidate_city_id)){
        case MTM_ELECTIONS_SUCCESS:
            break;
        case MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST:
            return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    if(tmp_candidate_city_id!=tmp_citizen_city_id){
        return MTM_ELECTIONS_NOT_SAME_CITY;
    }

    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==tmp_citizen_city_id) {
            switch(mtmCitySupportCandidate(iterator, citizenId, candidateId,
                                           priority)) {
                case MTM_CITIES_ALREADY_SUPPORTED:
                    return MTM_ELECTIONS_ALREADY_SUPPORTED;
                case MTM_CITIES_CAN_NOT_SUPPORT:
                    return MTM_ELECTIONS_CAN_NOT_SUPPORT;
                case MTM_CITIES_PRIORITY_EXISTS:
                    return MTM_ELECTIONS_PRIORITY_EXISTS;
                case MTM_CITIES_SUCCESS:
                    return MTM_ELECTIONS_SUCCESS;
                default :
                    return MTM_ELECTIONS_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_ELECTIONS_MEMORY_ERROR;
}

// *****************************************************************************

MtmElectionsResult mtmElectionsCancelSupport(MtmElections mtmElections,
                                             int citizenId, int candidateId) {
    if(!mtmElections){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(candidateId<0 || citizenId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    int tmp_citizen_city_id;
    switch(MtmElectionsCitizenGetCity(mtmElections, citizenId,
                                      &tmp_citizen_city_id)) {
        case MTM_ELECTIONS_SUCCESS:
            break;
        case MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST:
            return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    int tmp_candidate_city_id;
    switch(getCandidateCityId(mtmElections, candidateId,
                                      &tmp_candidate_city_id)) {
        case MTM_ELECTIONS_SUCCESS:
            break;
        case MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST:
            return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==tmp_citizen_city_id) {
            switch(mtmCityCancelSupport(iterator, citizenId, candidateId)) {
                case MTM_CITIES_MUST_SUPPORT:
                    return MTM_ELECTIONS_MUST_SUPPORT;
                case MTM_CITIES_NOT_SUPPORTED:
                    return MTM_ELECTIONS_NOT_SUPPORTED;
                case MTM_CITIES_SUCCESS:
                    return MTM_ELECTIONS_SUCCESS;
                default :
                    return MTM_ELECTIONS_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_ELECTIONS_MEMORY_ERROR;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\        functions to handle candidates     /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/          mtmElectionsChangeAddress        \||/  *******************
 *****   \/           ***************************       \/   **************** */
static MtmElectionsResult removeCitizenFromCity(MtmElections mtmElections,
                                                int citizenId, int cityId) {
    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==cityId) {
            switch(mtmCityRemoveCitizen(iterator, citizenId)) {
                case MTM_CITIES_SUCCESS:
                    return MTM_ELECTIONS_SUCCESS;
                default :
                    return MTM_ELECTIONS_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_ELECTIONS_MEMORY_ERROR;
}

// *****************************************************************************

static MtmElectionsResult moveCitizenToNewCity(MtmElections mtmElections,
                                               char *citizenName, int citizenId,
                                               int citizenAge,
                                               int yearsOfEducation,
                                               int oldCityId, int newCityId) {
    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==newCityId) {
            switch(mtmCityAddCitizen(iterator, citizenName, citizenId,
                                            citizenAge, yearsOfEducation)) {
                case MTM_CITIES_CITIZEN_ALREADY_EXISTS:
                    return MTM_ELECTIONS_SUCCESS;
                case MTM_CITIES_SUCCESS:
                    switch(removeCitizenFromCity(mtmElections,
                                                  citizenId, oldCityId)) {
                        case MTM_ELECTIONS_SUCCESS:
                            return MTM_ELECTIONS_SUCCESS;
                        default:
                            removeCitizenFromCity(mtmElections,
                                                  citizenId, newCityId);
                            return MTM_ELECTIONS_MEMORY_ERROR;
                    }
                default:
                    return MTM_ELECTIONS_MEMORY_ERROR;
            }
        }
        else {
            continue;
        }
    }
    return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
}

// *****************************************************************************

MtmElectionsResult mtmElectionsChangeAddress(MtmElections mtmElections,
                                             int citizenId, int cityId) {
    if(!mtmElections){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(citizenId<0 || cityId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    int tmp_citizen_old_city_id;
    int tmp_citizen_age;
    int tmp_citizen_education;
    char* tmp_citizen_name = NULL;
    switch(getTotalCitizenDetails(mtmElections, citizenId, &tmp_citizen_name,
                                  &tmp_citizen_age, &tmp_citizen_education,
                                  &tmp_citizen_old_city_id)) {
        case MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST:
            return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
        case MTM_ELECTIONS_SUCCESS:
            break;
        default:
            return MTM_ELECTIONS_MEMORY_ERROR;
    }

    switch(moveCitizenToNewCity(mtmElections, tmp_citizen_name, citizenId,
                                tmp_citizen_age, tmp_citizen_education,
                                tmp_citizen_old_city_id, cityId)) {
        case MTM_ELECTIONS_CITY_DOES_NOT_EXIST:
            free(tmp_citizen_name);
            return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
        case MTM_ELECTIONS_SUCCESS:
            free(tmp_citizen_name);
            return MTM_ELECTIONS_SUCCESS;
        default:
            free(tmp_citizen_name);
            return MTM_ELECTIONS_MEMORY_ERROR;
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\          mtmElectionsChangeAddress        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/              mtmElectionsRankByAge        \||/  *******************
 *****   \/           ***************************       \/   **************** */
int mtmElectionsRankByAge(MtmElections mtmElections, int citizen, void* pAge) {
    if(!mtmElections || !pAge || citizen<0){
        return 0;
    }
    int minimal_age = *(int*)pAge;
    if(!minimal_age) {
        return 0;
    }
    int citizen_age;
    switch(MtmElectionsCitizenGetAge(mtmElections, citizen, &citizen_age)) {
        case MTM_ELECTIONS_SUCCESS:
            return citizen_age/minimal_age;
        default:
            return 0;
    }
}

// *****************************************************************************

static int normalRankByMinimumAge(MtmElections mtmElections, int citizen, void* pAge) {
    if(!mtmElections || citizen<0){
        return 0;
    }

    int citizen_age;
    switch(MtmElectionsCitizenGetAge(mtmElections, citizen, &citizen_age)) {
        case MTM_ELECTIONS_SUCCESS:
            if(citizen_age/VOTERS_MINIMUM_AGE) {
                return 1;
            }
            return 0;
        default:
            return 0;
    }
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\            mtmElectionsRankByAge          /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/   cast the citizen functions for the uol  \||/  *******************
 *****   \/           ***************************       \/   **************** */
static Element electionsCopyCitizen(Element citizen){
    return citizenCopy(citizen);
}

static void electionsFreeCitizen(Element citizen){
    destroyCitizen(citizen);
}

static bool electionsEqualsCitizens(Element citizen1, Element citizen2){
    if(citizenCompare(citizen1, citizen2)==0) {
        return true;
    }
    return false;
}

static bool electionsCitizenGreaterThan(Element citizen1, Element citizen2){
    return greaterCitizenByNameOrId(citizen1, citizen2);
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\   cast the citizen functions for the uol  /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||          mtmElectionsPerformElections      \||/  *******************
 *****   \/           ***************************       \/   **************** */
static Citizen electionsFindCityWinner(MtmElections elections, MtmCity city,
                                      RankFunc rank,
                                      void *auxilaryData) {
    if(!elections || !city || !rank) {
        return NULL;
    }
    Set tmp_citizens_set = mtmCityGetCitizens(city);
    if(!tmp_citizens_set) {
        return NULL;
    }
    Set tmp_candidates_set = mtmCityGetCandidates(city);
    if(!tmp_candidates_set) {
        setDestroy(tmp_citizens_set);
        return NULL;
    }
    int max_votes=0;
    Citizen current_winner=NULL;
    SET_FOREACH(int*, candidate_iterator, tmp_candidates_set) {
        int current_votes=0;
        SET_FOREACH(Citizen, citizen_iterator, tmp_citizens_set) {
            if(getFirstPriority(citizen_iterator)==*candidate_iterator) {
                current_votes+= rank(elections, getCitizenId(citizen_iterator),
                                                                auxilaryData);
            }
        }
        if(!current_winner || current_votes>max_votes) {
            current_winner=mtmCityGetCitizen(city, *candidate_iterator);
            max_votes=current_votes;
            continue;
        }
        if(current_votes==max_votes) {
            Citizen tmp_citizen=mtmCityGetCitizen(city, *candidate_iterator);
            if(greaterCitizenByNameOrId(current_winner, tmp_citizen)){
                current_winner=tmp_citizen;
            }
        }
    }
    setDestroy(tmp_citizens_set);
    setDestroy(tmp_candidates_set);
    return current_winner;
}

//******************************************************************************

static UniqueOrderedList performElectionsWithRankFunc(MtmElections mtmElections,
                                               RankFunc rank,
                                               void *auxilaryData) {
    if(!mtmElections || (rank&&(!auxilaryData))) {
        return NULL;
    }
    UniqueOrderedList winners = uniqueOrderedListCreate(electionsCopyCitizen,
                                                        electionsFreeCitizen,
                                                        electionsEqualsCitizens,
                                                   electionsCitizenGreaterThan);
    if(!winners) {
        return NULL;
    }
    if(!rank) {
        SET_FOREACH(MtmCity, city_iterator, mtmElections->cities) {
            switch(uniqueOrderedListInsert(winners, electionsFindCityWinner
                                                (mtmElections, city_iterator,
                                     normalRankByMinimumAge, auxilaryData))){
                case UNIQUE_ORDERED_LIST_NULL_ARGUMENT:
                case UNIQUE_ORDERED_LIST_SUCCESS:
                    continue;
                default:
                    return NULL;
            }
        }
    }
    else {
        SET_FOREACH(MtmCity, city_iterator, mtmElections->cities) {
            switch(uniqueOrderedListInsert(winners, electionsFindCityWinner
                            (mtmElections, city_iterator, rank, auxilaryData))){
                case UNIQUE_ORDERED_LIST_NULL_ARGUMENT:
                case UNIQUE_ORDERED_LIST_SUCCESS:
                    continue;
                default:
                    return NULL;
            }
        }
    }
    return winners;
}

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/  cast functions of winner id for the uol  \||/  *******************
 *****   \/           ***************************       \/   **************** */
static Element copyOnlyWinnerId(Element winner){
    int* new_winner = malloc(sizeof(*new_winner));
    if(!new_winner) {
        return NULL;
    }
    *new_winner = *(int*)winner;
    return new_winner;
}

static void freeOnlyWinnerId(Element winner){
    if(!winner) {
        return;
    }
    free(winner);
}

static bool neutralEqualsOnlyWinnerId(Element citizen1, Element citizen2){
    return false;
}

static bool neutralGreaterThanOnlyWinnerId(Element citizen1, Element citizen2){
    return false;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\  cast functions of winner id for the uol  /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

static UniqueOrderedList filterTheListToIdOnly (UniqueOrderedList list) {
    if(!list) {
        return NULL;
    }
    UniqueOrderedList filtered_list = uniqueOrderedListCreate(copyOnlyWinnerId,
                                                            freeOnlyWinnerId,
                                                    neutralEqualsOnlyWinnerId,
                                                neutralGreaterThanOnlyWinnerId);
    if(!filtered_list){
        return NULL;
    }
    for(Citizen iterator = uniqueOrderedListGetLowest(list) ; iterator ;
                                    iterator = uniqueOrderedListGetNext(list)) {
        int citizenId = getCitizenId(iterator);
        switch(uniqueOrderedListInsert(filtered_list, &citizenId)){
            case UNIQUE_ORDERED_LIST_SUCCESS:
                continue;
            default:
                uniqueOrderedListDestroy(filtered_list);
                return NULL;
        }
    }
    uniqueOrderedListDestroy(list);
    return filtered_list;
}

//******************************************************************************

UniqueOrderedList mtmElectionsPerformElections(MtmElections mtmElections,
                                                               RankFunc rank,
                                                           void *auxilaryData,
                                                       const char *filename) {
    if(!mtmElections || !filename || (rank&&(!auxilaryData))) {
        return NULL;
    }
    UniqueOrderedList winners = performElectionsWithRankFunc(mtmElections, rank,
                                                                auxilaryData);
    if(!winners) {
        return NULL;
    }

    FILE* tmp_file = fopen(filename, "a");
    for(Element winner_iterator=uniqueOrderedListGetLowest(winners);
                                                                winner_iterator;
                            winner_iterator=uniqueOrderedListGetNext(winners)){
        int winnerId = getCitizenId(winner_iterator);
        int winnerAge, winnerEducation, cityId;
        char* winnerName=NULL;
        getTotalCitizenDetails(mtmElections, winnerId, &winnerName, &winnerAge,
                                                    &winnerEducation, &cityId);
        char* cityName=getCityName(mtmElections, cityId);
        if(!tmp_file) {
            mtmPrintFileError(filename, stderr);
        }
        else {
            mtmPrintMayorDetails(winnerId,winnerAge,winnerEducation,winnerName, cityName, cityId, tmp_file);
        }
        free(winnerName);
        free(cityName);
    }
    if(tmp_file) {
        fclose(tmp_file);
    }

    winners = filterTheListToIdOnly(winners);
    return winners;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\         mtmElectionsPerformElections      /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

// *****************************************************************************
/**  *   ||           ***************************       ||   *******************
 *****  \||/            mtmElectionsMayorOfCity        \||/  *******************
 *****   \/           ***************************       \/   **************** */
MtmElectionsResult mtmElectionsMayorOfCity(MtmElections mtmElections,
                                            int cityId, int *mayor,
                                            const char *filename) {
    if(!mtmElections || !mayor || !filename) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(cityId<0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    SET_FOREACH(MtmCity, iterator, mtmElections->cities) {
        if(mtmCityGetCityId(iterator)==cityId){
            FILE* tmp_file = fopen(filename, "a");
            switch(mtmCityGetMayor(iterator, mayor)) {
                case MTM_CITIES_NO_CANDIDATES_IN_CITY:
                    if(tmp_file) {
                        fclose(tmp_file);
                    }
                    return MTM_ELECTIONS_NO_CANDIDATES_IN_CITY;
                case MTM_CITIES_SUCCESS:
                    if(!tmp_file) {
                        return MTM_ELECTIONS_FILE_ERROR;
                    }
                    char* city=getCityName(mtmElections, cityId);
                    int age, education;
                    char* name;
                    MtmElectionsCitizenGetAge(mtmElections, *mayor, &age);
                    MtmElectionsCitizenGetEducation(mtmElections, *mayor,
                                                                    &education);
                    MtmElectionsCitizenGetName(mtmElections, *mayor, &name);

                    mtmPrintMayorDetails(*mayor, age, education, name, city,
                                                             cityId, tmp_file);
                    free(city);
                    free(name);
                    fclose(tmp_file);
                    return MTM_ELECTIONS_SUCCESS;
                default:
                    if(tmp_file) {
                        fclose(tmp_file);
                    }
                    return MTM_ELECTIONS_MEMORY_ERROR;
            }
        }
    }
    return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
}
/**  *   /\           ***************************       /\   *******************
 *****  /||\            mtmElectionsMayorOfCity        /||\  *******************
 *****   ||           ***************************       ||   **************** */
// *****************************************************************************

