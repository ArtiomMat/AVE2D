#include "a_ave.h"
#include "a_dev.h"

static dvar_t *last = NULL;

// Safe ave strcpy
// Returns 1 if copied all bytes to dest
char astrcpy(char *dest, const char *src, unsigned ds)
{
	unsigned i;
	unsigned ss = strlen(src);
	
	for (i = 0; i < ds-1 && i < ss; i++)
		dest[i] = src[i];
	dest[i] = 0; // Finish up with NULL character
	
	return ds == ss;
}

dvar_t *A_NewVar(char *name, char *str, void (*call)(dvar_t *))
{
	dvar_t *dv = malloc(sizeof (*dv));
	// Set up the variable
	astrcpy(dv->name, name, 24);// name
	astrcpy(dv->str, str, 16);	// str
	dv->val = atof(dv->str);	// val
	dv->call = call;			// call
	dv->next = last;			// next
	
	last = dv;
	return dv;
}

dvar_t *A_DeleteVar(char *name)
{
	// We need: PREV -> SELF -> NEXT
	// A_FindVar wont help us much here, so...
	dvar_t *prev, *self, *next;
	for (prev = last; prev; prev = prev->next)
		if (!strcmp(prev->next->name, name))
		{
			self = prev->next;
			next = self->next;
			break;
		}
	// If there is no self, nothing was found.
	if (!self)
		return NULL;
	
	free(self);
	prev->next = next;
	return prev;
}

float A_VarValue(char *name)
{
	dvar_t *dv = A_FindVar(name);
	if (dv)
		return dv->val;
	return 0;
}

dvar_t *A_SetVar(char *name, char *str)
{
	dvar_t *dv = A_FindVar(name);
	if (dv)
	{
		astrcpy(dv->str, str, 16);
		dv->val = atof(dv->str);
		// Call if callable
		if (dv->call)
			dv->call(dv);
	}
	
	return dv;
}

dvar_t *A_FindVar(char *name)
{
	dvar_t *dv;
	for (dv = last; dv; dv = dv->next)
		if (!strcmp(name, dv->name))
			return dv;
	
	return NULL;
}

// TODO
dvar_t *A_Command(char *cmd)
{
	return NULL;
}
