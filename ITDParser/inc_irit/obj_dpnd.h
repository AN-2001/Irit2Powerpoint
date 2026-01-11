/*****************************************************************************
* Dependencies control over objects.					     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute	of Technology		     *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.1, Mar. 1998   *
*****************************************************************************/

#ifndef	IRIT_OBJ_DPND_H
#define	IRIT_OBJ_DPND_H

#define IPOD_ATTR_FREE_DEPENDENCIES(Dpnds) { if ((Dpnds) != NULL) \
					      IritPrsrODFreeDependencies(Dpnds); }
#define IPOD_ATTR_COPY_DEPENDENCIES(Dpnds) \
      (Dpnds) != NULL ? IritPrsrODCopyDependencies(Dpnds) : NULL;

typedef struct IPODDependsStruct {
    struct IPODDependsStruct *Pnext;
    char *Name;			       /* Name of object that depends on us. */
} IPODDependsStruct;

typedef struct IPODParamsStruct {
    struct IPODParamsStruct *Pnext;
    char *Name;	/* Name of object that serves as a parameter of this object. */
} IPODParamsStruct;

typedef struct IPODObjectDpndncyStruct {
    struct IPODObjectDpndncyStruct *Pnext;	        /* To next in chain. */
    struct IPAttributeStruct *Attr;
    struct IPODParamsStruct *ObjParams;  /* Objects that are params to this. */
    struct IPODDependsStruct *ObjDepends;/* Objects who depends on this obj. */
    char *EvalExpr;          /* An assingment string that updates an object. */
    int EvalIndex;	  /* A simple measure against circular dependencies. */
    int NumVisits;  /* Number of times node is traversed in graph traversal. */
    int NumParams;       /* Number of parameters (length of ObjParams list). */
} IPODObjectDpndncyStruct;

IPODParamsStruct *IritPrsrODNewParametersOfObj(char *Name,
					       IPODParamsStruct *Pnext);
IPODDependsStruct *IritPrsrODNewDependenciesOfObj(char *Name,
						  IPODDependsStruct *Pnext);
IPODObjectDpndncyStruct *IritPrsrODNewDependencies(void);

void IritPrsrODAddParameterToObj(IPODObjectDpndncyStruct **ObjDpnd,
				 char *ParamName);
void IritPrsrODAddDependencyToObj(IPODObjectDpndncyStruct **ObjDpnd,
				  char *DpndName);
void IritPrsrODDelParameterFromObj(IPODObjectDpndncyStruct *ObjDpnd,
				   char *ParamName);
void IritPrsrODDelDependencyFromObj(IPODObjectDpndncyStruct *ObjDpnd,
				    char *DpndName);

IPODParamsStruct *IritPrsrODCopyParametersOfObj(IPODParamsStruct *ObjParams);
IPODDependsStruct *IritPrsrODCopyDependenciesOfObj(
					       IPODDependsStruct *ObjDepends);
IPODObjectDpndncyStruct *IritPrsrODCopyDependencies(
					      IPODObjectDpndncyStruct *Dpnds);

void IritPrsrODFreeParametersOfObj(IPODParamsStruct *ObjParams);
void IritPrsrODFreeDependenciesOfObj(IPODDependsStruct *ObjDepends);
void IritPrsrODFreeDependencies(IPODObjectDpndncyStruct *Dpnds);

void IritPrsrODUpdateAllDependencies(IPODObjectDpndncyStruct *ObjDpnd);
void IritPrsrODEvalOneObject(IPObjectStruct *PObj);

#endif /* IRIT_OBJ_DPND_H */
