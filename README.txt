
SPLASH SCREEN
=============

#define NOSPLASHSHIT
to remove splash screen code

STUFF files
===========

Game data files.

HEADER

	Magic byte
		Just a 0x69
	Stuff table:
		Number of table entries(4 byte uint)

		Sequences of ID's(8 byte id), GLOBAL start offset(4 byte uint), size(4 byte uint)
		"SOME_ID", 0, 256,
		"OTHER", 256, 2

		Note: ID's are meant to be null terminated strings, you still have total freedom though.

STUFF

	Stuff can be anything really, it's just data, the game decides how it's used.

How to create a STUFF file?
===========================

1. Create a text file and put 'i'(0x69) in it.
2. Use STF_Write to write stuff to it.
3. Use STF_Close to finish all the writing.

You have a STUFF file! With stuff in it!
