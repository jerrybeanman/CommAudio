#include "CircularBuffer.h"


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	CBInitialize
--
-- DATE:		Febuary 6th, 2016		REVISIONS:
--
-- DESIGNER:	Ruoqi Jia				PROGRAMMER:	Ruoqi Jia
--
-- INTERFACE:	void CBInitialize(CircularBuffer * CBuff, const size_t MaxElements, const size_t ElementSize);
--						~CBUFF		 : Pointer to circular buffer struct
--						~MaxSize	 : Maximum elements that can be stored into CBuff
--						~ElementSize : Size of each element
--
-- RETURNS: void
--
-- NOTES: Malloc memory space for CBuff, initializes its member by pointing head and rear to the same starting address
--------------------------------------------------------------------------------------------------------------------*/
void CBInitialize(CircularBuffer * CBuff, const size_t MaxElements, const size_t ElementSize)
{
    CBuff->buffer = malloc(MaxElements * ElementSize);
    if (CBuff->buffer == NULL)
    {
        return;
    }
    CBuff->buffer_end = (char *)CBuff->buffer + MaxElements * ElementSize;
    CBuff->MaxSize = MaxElements;
    CBuff->Count = 0;
    CBuff->ElementSize = ElementSize;
    CBuff->Front = CBuff->buffer;
    CBuff->Rear = CBuff->buffer;
}

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
void CBFree(CircularBuffer * CBuff)
{
    free(CBuff->buffer);
}


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
void CBPushBack(CircularBuffer * CBuff, const void *item)
{
    /* Comment this block of code out if we want the head to overwirte the tail */
    if (CBuff->Count == CBuff->MaxSize)
        return;

    memcpy(CBuff->Front, item, CBuff->ElementSize);
    CBuff->Front = (char *)CBuff->Front +  CBuff->ElementSize;
    if (CBuff->Front == CBuff->buffer_end)
        CBuff->Front = CBuff->buffer;
    CBuff->Count++;
}

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
void CBPop(CircularBuffer * CBuff, void * item)
{
    if (CBuff->Count == 0)
    {
        return;
    }
    memcpy(item, CBuff->Rear, CBuff->ElementSize);
    CBuff->Rear = (char *)CBuff->Rear + CBuff->ElementSize;
    if (CBuff->Rear == CBuff->buffer_end)
        CBuff->Rear = CBuff->buffer;
    CBuff->Count--;
}
