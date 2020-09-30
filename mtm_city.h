//
// Created by Tsuf Cohen on 25/11/2018
//
#ifndef MTM_CITIES_H_
#define MTM_CITIES_H_

#include <stdbool.h>
#include "mtm_citizen.h"
#include "set.h"

/**
 *
 */
typedef enum mtmCityResult_t{
	MTM_CITIES_MEMORY_ERROR,
	MTM_CITIES_NULL_ARGUMENT,
	MTM_CITIES_ILLEGAL_ID,
	MTM_CITIES_ILLEGAL_AGE,
	MTM_CITIES_ILLEGAL_PRIORITY,
	MTM_CITIES_ILLEGAL_NUMBER_OF_YEARS,
	MTM_CITIES_CITIZEN_ALREADY_EXISTS,
	MTM_CITIES_CITIZEN_DOES_NOT_EXIST,
	MTM_CITIES_CANDIDATE_ALREADY_EXISTS,
	MTM_CITIES_CANDIDATE_DOES_NOT_EXIST,
	MTM_CITIES_ALREADY_SUPPORTED,
	MTM_CITIES_NOT_SUPPORTED,
	MTM_CITIES_CAN_NOT_SUPPORT,
	MTM_CITIES_MUST_SUPPORT,
	MTM_CITIES_PRIORITY_EXISTS,
	MTM_CITIES_NO_CANDIDATES_IN_CITY,
	MTM_CITIES_SUCCESS
}MtmCityResult;

/**
 * pointer to struct mtm_city_t
 */
typedef struct mtm_city_t* MtmCity;

/** this func create a new city with the name and id given.
 *
 * @param cityId  - an unsigend id number for the new city.
 * @param cityName - a name for the new city.
 * @return null if one of the arguments is null or allocation failed.
 *          if success - new MtmCity.
 */
MtmCity createCity (int cityId, const char* cityName) ; //DONE

/**this func copy a city.
 *
 * @return null if the argument is null or allocation failed.
 *          if success - a copy of MtmCity.
 */
MtmCity copyCity (MtmCity) ; //DONE

/**
 * free the MtmCity and it's fields.
 */
void freeCity (MtmCity) ; //DONE

/**
 * this func gets two cities as parameters
 * if they have the same ID - 0
 * if the first is bigger 1
 * if the second is bigger -1.
 */
int compareCity (MtmCity, MtmCity) ; //DONE

/** this func add a new citizen to the given city.
 *
 * @param mtmCity - the city that we want to put the citizen inside.
 * @param citizenName - the citizen name.
 * @param citizenId - the citizen ID.
 * @param citizenAge - the citizen age.
 * @param yearsOfEducation
 * @return
 * MTM_CITIES_MEMORY_ERROR - allocation failed or out of memory.
 * MTM_CITIES_NULL_ARGUMENT - one of the arguments is null.
 * MTM_CITIES_ILLEGAL_ID - if a negative number.
 * MTM_CITIES_ILLEGAL_AGE - if a negative number or zero.
 * MTM_CITIES_ILLEGAL_NUMBER_OF_YEARS - if a negative number.
 * MTM_CITIES_CITIZEN_ALREADY_EXISTS - if the citizen already exists.
 * MTM_CITIES_SUCCESS - for success.
 */
MtmCityResult mtmCityAddCitizen(MtmCity mtmCity, const char *citizenName,
                                int citizenId, int citizenAge,
                                int yearsOfEducation); //DONE

/**
 *
 * @param mtmCity - the city that we want to check if the citizen inside.
 * @param citizenId - the citizen ID.
 * @return true - the citizen is in this city
 *          false - one of the arguments is null or the citizen not in the city.
 */
bool mtmCityContainCitizen(MtmCity mtmCity, int citizenId); //DONE

/**
 *
 * @param mtmCity
 * @param citizenId
 * @return a ptr to a copy of the name if success or null if failed.
 *
 */
char* mtmCityGetCitizenName(MtmCity mtmCity, int citizenId); //DONE

/**
 *
 * @param mtmCity - the city we want to know the id.
 *  ! use this func only with a valid MtmCity !
 * @return the city id.
 */
int mtmCityGetCityId(MtmCity mtmCity); //DONE

/**
 *
 * @param mtmCity
 * @return null or copy of the name
 */
char* mtmCityGetCityName(MtmCity mtmCity);

/**
 *
 * @param mtmCity - the city
 * @param citizenId - the citizen
 * @return  the age of the citizen if success or -1 if failed.
 */
int mtmCityGetCitizenAge(MtmCity mtmCity, int citizenId); //DONE

/**
 *
 * @param mtmCity - the city
 * @param citizenId - the citizen
 * @return  the years of education of the citizen if success or -1 if failed.
 */
int mtmCityGetCitizenEducation(MtmCity mtmCity, int citizenId); //DONE

/**
 *
 * @param mtmCity
 * @param citizenId
 * @return true if the candidate in this city.
 */
bool mtmCityContainCandidate(MtmCity mtmCity, int citizenId) ;

/**
 *
 * @param mtmCity
 * @param candidateId
 * @return
 * MTM_CITIES_NULL_ARGUMENT
 * MTM_CITIES_ILLEGAL_ID
 * MTM_CITIES_CANDIDATE_ALREADY_EXISTS
 * MTM_CITIES_MEMORY_ERROR
 * MTM_CITIES_SUCCESS
 * MTM_CITIES_CITIZEN_DOES_NOT_EXIST
 */
MtmCityResult mtmCityAddCandidate(MtmCity mtmCity, int candidateId); //DONE

/**
 *
 * @param mtmCity
 * @param candidateId
 * @return
 * MTM_CITIES_NULL_ARGUMENT
 * MTM_CITIES_ILLEGAL_ID
 * MTM_CITIES_CANDIDATE_DOES_NOT_EXIST
 * MTM_CITIES_MEMORY_ERROR
 * MTM_CITIES_SUCCESS
 */
MtmCityResult mtmCityWithdrawCandidate(MtmCity mtmCity, int candidateId); //DONE

/**
 *
 * @param mtmCity
 * @param citizenId - the citizen we want to add priority.
 * @param candidateId - the candidate we want to add.
 * @param priority - the priority .
 * @return
 * MTM_CITIES_NULL_ARGUMENT
 * MTM_CITIES_ILLEGAL_ID
 * MTM_CITIES_ILLEGAL_PRIORITY
 * MTM_CITIES_CANDIDATE_DOES_NOT_EXIST
 * MTM_CITIES_CITIZEN_DOES_NOT_EXIST
 * MTM_CITIES_ALREADY_SUPPORTED
 * MTM_CITIES_CAN_NOT_SUPPORT
 * MTM_CITIES_PRIORITY_EXISTS
 * MTM_CITIES_MEMORY_ERROR
 * MTM_CITIES_SUCCESS
 */
MtmCityResult mtmCitySupportCandidate(MtmCity mtmCity, int citizenId,
                                      int candidateId, int priority); //DONE

/**
 *
 * @param mtmCity
 * @param citizenId - the citizen we want to remove priority.
 * @param candidateId - the candidate we want to remove.
 * @return
 * MTM_CITIES_NULL_ARGUMENT
 * MTM_CITIES_ILLEGAL_ID
 * MTM_CITIES_CANDIDATE_DOES_NOT_EXIST
 * MTM_CITIES_MUST_SUPPORT
 * MTM_CITIES_NOT_SUPPORTED
 * MTM_CITIES_SUCCESS
 * MTM_CITIES_MEMORY_ERROR
 * MTM_CITIES_CITIZEN_DOES_NOT_EXIST
 */
MtmCityResult mtmCityCancelSupport(MtmCity mtmCity, int citizenId,
                                   int candidateId); //DONE

/**
 *
 * @param mtmCity
 * @param citizenId
 * @return
 * MTM_CITIES_NULL_ARGUMENT
 * MTM_CITIES_ILLEGAL_ID
 * MTM_CITIES_MEMORY_ERROR
 * MTM_CITIES_CITIZEN_DOES_NOT_EXIST
 * MTM_CITIES_SUCCESS
 */
MtmCityResult mtmCityRemoveCitizen(MtmCity mtmCity, int citizenId); //DONE

/**
 * get city and citizen id and return the citizen ptr.
 * @param mtmCity
 * @param citizenId
 * @param lastIterator
 * @return null if mtmCity is null or citizenId<0 , Citizen ptr if success.
 */
Citizen mtmCityGetCitizen(MtmCity mtmCity, int citizenId);

/**
 *
 * @param mtmCity
 * @param citizenId
 * @return null if mtmCity is null, copy of Citizens set if success.
 * !don't forget to Deallocate when finish.
 */
Set mtmCityGetCitizens(MtmCity mtmCity);

/**
 *
 * @param mtmCity
 * @return null if mtmCity is null, copy of Candidates set if success.
 *  !don't forget to Deallocate when finish.
 */
Set mtmCityGetCandidates(MtmCity mtmCity);

/**
 * return the mayor id to the given ptr.
 * @param mtmCity
 * @param mayor
 * @return
 * MTM_CITIES_NO_CANDIDATES_IN_CITY
 * MTM_CITIES_SUCCESS
 * MTM_CITIES_NULL_ARGUMENT
 */
MtmCityResult mtmCityGetMayor(MtmCity mtmCity, int* mayor);

#endif /* MTM_CITIES_H_ */
