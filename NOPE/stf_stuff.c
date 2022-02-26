#include <stdio.h>

#include "stf_stuff.h"
#include "cl_main.h"

#define IDSIZE	8
#define OFFSET_MAGIC	0
#define OFFSET_TABLESIZE 1
#define OFFSET_TABLE	5

typedef struct
{
	int8_t id[IDSIZE];
	uint32_t start;
	uint32_t size;
} entry_t;

struct
{
	entry_t *table;
	uint32_t tablesize;

	FILE *f;
	char name[256];
} stf;

bool STF_Open(const char *fp)
{
	strcpy(stf.name, fp);
	if (stf.f)
	{
		printf("STF_Open: Cannot open another stuff file at the same time.\n");
		return false;
	}
	stf.f = fopen(fp, "r+");

	if (!stf.f)
	{
		printf("STF_Open: '%s' does not exist, creating one.\n", fp);
		stf.f = fopen(fp, "w+");
		char buff[] = {0x69, 0, 0, 0, 0};
		fwrite(buff, sizeof(buff), 1, stf.f);
		fseek(stf.f, 0, SEEK_SET);

		stf.tablesize = 0;
		stf.table = malloc(0);
	}

	else
	{
		// Magic number check
		char code = fgetc(stf.f);
		if (code != 0x69)
		{
			fclose(stf.f);
			printf("STF_Open: '%s' is not a STUFF file.\n", fp);
			return false;
		}

		// Table size
		fread(&stf.tablesize, 4, 1, stf.f);
		// Table entries
		stf.table = malloc(sizeof (entry_t) * stf.tablesize);
		for (unsigned i = 0; i < stf.tablesize; i++)
			fread(stf.table+i, sizeof(entry_t), 1, stf.f);
	}
	return true;
}

// -1 if not found
// index otherwise
int Find(const char *id)
{
	if (!stf.f)
		return false;
	
	uint8_t size;
	for (size = 0; size <= IDSIZE && id[size]; size++);
	
	for (int i = 0; i < stf.tablesize; i++)
	{
		int csize;
		for (csize = 0; csize <= IDSIZE && stf.table[i].id[csize]; csize++);
		if (csize!=size || csize == 0) // Size different OR erased?
			continue;

		bool broke = 0;
		for (int c = 0; c < size; c++)
			if (stf.table[i].id[c] != id[c])
				broke = 1;
		if (!broke)
			return i;
		
	}

	return -1;
}

int STF_SizeOf(const char *id)
{
	if (!stf.f)
		return false;
	
	int ei = Find(id);
	if (ei == -1)
	{
		printf("STF_Find: '%s' is a bad ID.\n", id);
		return -1;
	}
	return stf.table[ei].size;
}

bool STF_Read(const char *id, char *where, unsigned maxsize)
{
	if (!stf.f)
		return false;
	
	int ei = Find(id);
	if (ei == -1)
	{
		printf("STF_Find: '%s' is a bad ID.\n", id);
		return false;
	}
	
	entry_t e = stf.table[ei];

	fseek(stf.f, e.start, SEEK_SET);
	if (maxsize > e.size)
		maxsize = e.size;
	
	fread(where, maxsize, 1, stf.f);

	return true;
}

// added=true then +=
// else -=
void DidEntry(bool added)
{
	// If it was added we already put the offset anyway
	for (int i = 0; i < stf.tablesize; i++)
	{
		stf.table[i].start += added?sizeof(entry_t):-sizeof(entry_t);
		
		// Go to the start bytes of the current entry

		fseek(stf.f, OFFSET_TABLE + (i * sizeof(entry_t)) + IDSIZE, SEEK_SET);
		fwrite(&(stf.table[i].start), 4, 1, stf.f);
	}
}

void PutBufferIn(FILE *f, unsigned loc, char *b, unsigned len)
{
	char 	*br;
	long 	brsize;
	// Saving the right buffer
	fseek(f, 0, SEEK_END);
	brsize = ftell(f) - loc;
	if (brsize < 0) // If it's less, then well, loc is bigger than SEEK_END
		return;
	br = malloc(brsize);
	fseek(f, loc, SEEK_SET);
	fread(br, brsize, 1, f);
	// Writing b
	fseek(f, loc, SEEK_SET);
	fwrite(b, len, 1, f);
	// Putting the right buffer back
	fwrite(br, brsize, 1, f);
	free(br);
}

// FIXME: Gotta rewrite the file
// fwrite wont work sadly.
void RemoveBufferIn(char *fp, unsigned loc, unsigned len)
{
	FILE *f = fopen(fp, "r");
	fseek(f, 0, SEEK_END);
	long flen = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	char *str = calloc(flen, 1);
	fread(str, flen, 1, f);

	fclose(f);
	str[loc] = 0;
	str[flen] = 0;
	char 	*bl = str;
	char	*br = str + loc + len;
	
	printf("%s\n\n%s\n\n", bl, br);
	fopen(fp, "w");
	fwrite(bl, loc, 1, f);
	fwrite(br, flen - (loc+len), 1, f);

	fclose(f);
	free(str);
}

bool STF_Erase(const char *id)
{
	if (!stf.f)
		return false;
	// Getting the entry
	entry_t e;
	int ei = Find(id);
	if (ei == -1)
	{
		printf("STF_Find: '%s' is a bad ID.\n", id);
		return false;
	}
	e = stf.table[ei];
	
	// Decrmenting the table count
	fseek(stf.f, 1, SEEK_SET);
	uint32_t size; // We cant modify the tablesize here
	fread(& (size), sizeof (size), 1, stf.f);
	size--;
	fwrite(& (size), sizeof (size), 1, stf.f);
	
	printf("%d size %d\n", e.start, e.size);
	// Removing stuff
	fclose(stf.f);
	RemoveBufferIn(stf.name, e.start, e.size);
	fopen(stf.name, "r+");

	// Removing the table entry
	unsigned loc = OFFSET_TABLE + ei * sizeof(entry_t);
	fclose(stf.f);
	RemoveBufferIn(stf.name, loc, sizeof(entry_t));
	fopen(stf.name, "r+");

	// Offsetting all other entries
	// The thing is, that only in DidEntry, will the offset be realized
	// because right now, we are just offsetting it in the stf not the file.
	for (int i = ei+1; i < stf.tablesize; i++)
		stf.table[i].start -= stf.table[ei].size;

	stf.table[ei].id[0] = 0;

	DidEntry(0);
}

// FIXME: Turns previous stuff into junk if you write twice, try
// NOTE: This junk shit was only confirmed with new files, try existing files too please.
/*
	STF_Open("STUFF");
	STF_Write("DORDOR", "DOR IS GAY", sizeof ("DOR IS GAY"));
	STF_Write("ARTIOM", "WHYTHOUGH?", sizeof ("WHYTHOUGH?"));
	char where[50];
	STF_Read("DORDOR", where, 50);
	printf(where);
	STF_Close();
*/
bool STF_Write(const char *id, const char *stuff, unsigned size)
{
	if (!stf.f)
		return false;
	if (Find(id) != -1)
	{
		printf("STF_Write: '%s' ID already exists.\n", id);
		return false;
	}
	if (id[0] == 0)
	{
		printf("STF_Write: ID must begin with a non NULL terminator.\n", id);
		return false;
	}
	entry_t e;
	// Setting up the entry

	// entry ID
	char nulled = 0;
	for (uint8_t i = 0; i < IDSIZE; i++)
	{
		if (id[i] == 0)
			nulled = 1;
		
		if (nulled)
			e.id[i] = 0;
		else
			e.id[i] = id[i];
	}

	// entry size
	e.size = size;

	//printf("ENTRY '%s':\n\tSIZE: %d\n\tSTART: %d\n\n", id, e.size, e.start);

	// table size bytes
	stf.tablesize++;
	fseek(stf.f, 1, SEEK_SET);
	fwrite(&(stf.tablesize), sizeof(stf.tablesize), 1, stf.f);

	// entry start offset, NOT considering the append of the entry
	// The consideration will be in the end with DidEntry
	fseek(stf.f, 0, SEEK_END);
	e.start = ftell(stf.f);

	// Appending entry bytes to the table
	// fseek(stf.f, 1, SEEK_SET); // Reset the position of cursor after fwrite
	unsigned loc = OFFSET_TABLE + (stf.tablesize-1) * sizeof(entry_t);
	// printf("STR %s\n", stuff);
	PutBufferIn(stf.f, loc, &e, sizeof(e));

	// Appending the stuff bytes to the end(their offset)
	fseek(stf.f, 0, SEEK_END);
	fwrite(stuff, size, 1, stf.f);

	// Adding the entry to the table array in stf
	stf.table = realloc(stf.table, stf.tablesize * sizeof(entry_t));
	stf.table[stf.tablesize-1] = e;

	DidEntry(1);
}

void STF_Close(void)
{
	if (!stf.f)
		return;
	fclose(stf.f);
	free(stf.table);
}
