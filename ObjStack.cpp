
#include "defs.h"
#include "ObjStack.h"


ObjStack::ObjStack()
{
	top = NULL;
	size = 0;
}

void ObjStack::push(shape_ptr p)
{
	if(size==0)
	{
		top = p;
		top->next=NULL;
		size++;
	}
	else
	{
		p->next=top;
		top=p;
		size++;
	}
	//cout << "PUSHED OBJ " << p->ID << endl;
}

shape_ptr ObjStack::erase(int subject)
{
	shape_ptr p = top;
	if(p->ID==subject)
	{
		top = p->next;
		size--;
		return p;
	}
	else
	{	
		shape_ptr pParent = top;
		while(p->next!=NULL)
		{
			p = p->next;
			if(p->ID==subject)
			{
				pParent->next = p->next;
				size--;
				return p;
			}
			pParent = pParent->next;
		}
	}

	return NULL;
}

shape_ptr ObjStack::find(int subject)
{
	if(size==0) return 0;
	shape_ptr p = top;
	while(p->next!=NULL)
	{
		if(p->ID==subject) return p;
		else p = p->next;
	}
	if(p->ID==subject) return p;
	return 0;
}

shape_ptr ObjStack::pop()
{
	shape_ptr p = top;
	top = p->next;
	size--;
	return p;
}

shape_ptr ObjStack::getTop()
{
	return top;
}

int ObjStack::length()
{
	return size;
}

