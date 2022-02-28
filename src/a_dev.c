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

dvar_t *A_NewVar(char *name, char *str, void (*call)(dvar_t *), char save)
{
	dvar_t *dv = malloc(sizeof (*dv));
	// Set up the variable
	astrcpy(dv->name, name, 24);// name
	astrcpy(dv->str, str, 16);	// str
	dv->val = atof(dv->str);	// val
	dv->call = call;			// call
	dv->prev = last;			// next
	dv->save = save;			// save
	
	last = dv;
	return dv;
}

dvar_t *A_DeleteVar(char *name)
{
	// We need: PREV -> SELF -> NEXT
	// A_FindVar wont help us much here, so...
	dvar_t *prev, *self, *next;
	for (prev = last; prev; prev = prev->prev)
		if (!strcmp(prev->prev->name, name))
		{
			self = prev->prev;
			next = self->prev;
			break;
		}
	// If there is no self, nothing was found.
	if (!self)
		return NULL;
	
	free(self);
	prev->prev = next;
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
	for (dv = last; dv; dv = dv->prev)
		if (!strcmp(name, dv->name))
			return dv;
	
	return NULL;
}

// TODO
dvar_t *A_Command(char *cmd)
{
	return NULL;
}

void A_ReadConfig(void)
{
	FILE *f = fopen(A_RelPath("CONFIG"), "r");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		char *cfg = malloc(size);
		fseek(f, 0, SEEK_SET);
		fread(cfg, size, 1, f);
		char name[24];
		char str[16];
		unsigned ni = 0, si = 0, nd = 0; // Name done
		for (int i = 0; i < size; i++)
		{
			if (cfg[i] == ' ')
			{
				name[ni] = 0;
				nd = 1;
			}
			else if (cfg[i] == '\n')
			{
				str[si] = 0;
				if (A_FindVar(name))
					A_SetVar(name, str);
				else
					printf("A_ReadConfig: Variable '%s' does not exist.\n", name);
				// Resetting stuff
				nd = ni = si = 0;
			}
			else if (nd)
				str[si++] = cfg[i];
			else
				name[ni++] = cfg[i];
		}
		fclose(f);
	}
}

void A_WriteConfig(void)
{
	FILE *f = fopen(A_RelPath("CONFIG"), "w");
	dvar_t *dvs = last;
	for (; dvs; dvs = dvs->prev)
	{
		if (dvs->save)
		{
			unsigned lname = strlen(dvs->name);
			unsigned lstr = strlen(dvs->str);
			fwrite(dvs->name, lname, 1, f);
			fwrite(" ", 1, 1, f);
			fwrite(dvs->str, lstr, 1, f);
			fwrite("\n", 1, 1, f);
		}
	}
	fclose(f);
}
