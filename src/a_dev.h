#pragma once

// Inspired by Quake CVARS
typedef struct dvar_s
{
	char name[24];
	char str[16];
	float val; // Cached float value
	void (*call)(struct dvar_s *);
	
	struct dvar_s *next;
} dvar_t;

// To make dev var not callabe simply set call to NULL
dvar_t *A_NewVar(char *name, char *str, void (*call)(dvar_t *));
dvar_t *A_DeleteVar(char *name);
float A_VarValue(char *name);
// If the dev var is callable it will set str to str
// And do call(self)
dvar_t *A_SetVar(char *name, char *str);
// Don't fuck around with the returned var if
// You don't know what you are doing, use functions.
dvar_t *A_FindVar(char *name);
// Command syntax:
// NAME STR
// STR is optional, depending on your needs.
dvar_t *A_Command(char *cmd);
