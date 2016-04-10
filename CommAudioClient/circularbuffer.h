#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdlib.h>
#include <string.h>
/* Can be any type */
struct CircularBuffer
{
    void	*	buffer;			/* data buffer								*/
    void	*	buffer_end;		/* end of data buffer						*/
    int        	MaxSize;		/* maximum number of items in the buffer	*/
    int     	Count;			/* number of items in the buffer			*/
    int         ElementSize;	/* size of each item in the buffer			*/
    void	*	Front;			/* pointer to Front							*/
    void	*	Rear;			/* pointer to Rear							*/
    int         BytesRECV;      /* Number of bytes recieved					*/
};


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	CBInitialize
--
-- DATE:		Febuary 6th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void CBInitialize(CircularBuffer * CBuff, DWORD MaxElements, DWORD ElementSize);
--						~CBUFF		 : Pointer to circular buffer struct
--						~MaxSize	 : Maximum elements that can be stored into CBuff
--						~ElementSize : Size of each element
--
-- RETURNS: void
--
-- NOTES: Malloc memory space for CBuff, initializes its member by pointing head and rear to the same starting address
--------------------------------------------------------------------------------------------------------------------*/
void CBInitialize(CircularBuffer * CBuff, const size_t MaxElements, const size_t ElementSize);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	CBFree
--
-- DATE:		Febuary 6th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void CBFree(CircularBuffer * CBuff);
--						~CBuff : Pointer to circualr buffer struct
--
-- RETURNS: void
--
-- NOTES: Free CB memory from heap
--------------------------------------------------------------------------------------------------------------------*/
void CBFree(CircularBuffer * CBuff);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	CBPushBack
--
-- DATE:		Febuary 6th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void CBPushBack(CircularBuffer * CBuff, const void * item);
--						~CBuff : Pointer to circualr buffer struct
--						~item  : constant void pointer to an item's memory space.
--
-- RETURNS: void
--
-- NOTES: Copy item's memory space into the current memory space pointed by front in CBuff. Increment front by size
--	of item afterward.
--------------------------------------------------------------------------------------------------------------------*/
void CBPushBack(CircularBuffer * CBuff, const void * item);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	CBPop
--
-- DATE:		Febuary 6th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void CBPop(CircularBuffer * CBuff, void * item);
--						~CBuff : Pointer to circular buffer struct
--						~item  : void pointer to an item's memory space
--
-- RETURNS: void
--
-- NOTES: Copy memory space pointed by CBuff's rear into item. Increment rear by size of item afterward.
--------------------------------------------------------------------------------------------------------------------*/
void CBPop(CircularBuffer * CBuff, void * item);

#endif
