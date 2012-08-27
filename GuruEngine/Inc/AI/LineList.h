/************************************************************************************
   File:			LineList.h
   Author:			Michael Sharpe
   Description:		Line drawing. Store lines in a list so I only have to lock the
					vertex buffer once.
/***********************************************************************************/
#ifndef __LINELIST_
#define __LINELIST_


#include "Utility.h"
#include <vector>

/* ***********************************************************
Vertex used by the Line List class
*********************************************************** */
struct Line
{
	Line(CVector3 s, CVector3 f, CVector3 clr) { start = s; finish = f; color = clr; }
	CVector3 start;
	CVector3 finish;
	CVector3 color;
};

/* ***********************************************************
Line List
*********************************************************** */
class LineList
{
public:
	LineList(float lineSize);
	~LineList();

	bool Init(void);
	void Release(void);
	void Render(void);
	void AddLineToList(CVector3 start, CVector3 finish, CVector3 color);
	void ClearList(void);

private:
	bool						m_Initilized;
	std::vector<Line>			m_LineList;
	float						m_LineWidth;
};

#endif