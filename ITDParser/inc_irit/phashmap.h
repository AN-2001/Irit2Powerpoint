/******************************************************************************
* IrtPHashMap.h	- Header file for simple pointers hash map.		     *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
*******************************************************************************
* Written by Fady Massarwa, Jul. 2016.					     *
******************************************************************************/

#ifndef MISC_P_HASH_MAP_H
#define MISC_P_HASH_MAP_H

typedef struct IritMiscPHashMapStruct *IritMiscPHashMap;
typedef void *IritMiscPHashMapKey;
typedef void *IritMiscPHashMapValue;
typedef void (*IritMiscPHashMapCBFunc)(IritMiscPHashMap HMap, 
				       IritMiscPHashMapKey Key, 
				       IritMiscPHashMapValue Value);

typedef enum {
    MISCP_HMSUCCESS = 0,
    MISCP_HMFAILURE
} MiscPHMResult;

IritMiscPHashMap IritMiscPHashMapCreate(int HashSize);
void IritMiscPHashMapDelete(IritMiscPHashMap HMap);
MiscPHMResult IritMiscPHashMapSet(IritMiscPHashMap HMap, 
			      IritMiscPHashMapKey Key, 
			      IritMiscPHashMapValue Value);
MiscPHMResult IritMiscPHashMapGet(IritMiscPHashMap HMap, 
			      IritMiscPHashMapKey Key, 
			      IritMiscPHashMapValue *Value);
MiscPHMResult IritMiscPHashMapRemove(IritMiscPHashMap HMap,
				     IritMiscPHashMapKey Key);
void IritMiscPHashMapPrint(IritMiscPHashMap HMap);
void IritMiscPHashMapIterate(IritMiscPHashMap HMap, IritMiscPHashMapCBFunc CBFunc);

#endif /* IRT_P_HASH_MAP_H */
