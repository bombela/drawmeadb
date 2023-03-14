/*
 * \file
 * \brief Data dictionnary printer.
 * \author Copyright © 2008 Bombela <bombela@free.fr>
 * \date 2008
 *
 * This file is part of MeriseAcide.
 *
 * MeriseAcide is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * MeriseAcide is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with MeriseAcide; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Purpose:
 *
 * Data dictionnary printer : plain text.
 *
 * $Id: DataDic_Latex.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "DataDic_Latex.h"

using namespace std;

namespace MeriseAcide
{

bool DataDic_Latex::Print(std::ostream& s)
{
	mErrorMsg.str("");

	unsigned int curpos_base = 0;
	unsigned int maxcol = mColNum;
	if (maxcol < 1) maxcol = 1;

	unsigned int maxpos = (mDataList.size() / maxcol);
	unsigned int modmax = (mDataList.size() % maxcol);

	if (modmax)
		maxpos++;

	//s << "\\begin{tabular}{ l ";
	//for (unsigned int i = 1; i < maxcol; i++) s << "| l ";
	//s << "}" << "\n";

	while (curpos_base < maxpos)
	{
		unsigned int curpos = curpos_base;
		PrintData(mDataList[curpos], s);

		for (unsigned int
				colnum  = 2;
				colnum <= maxcol;
				colnum++)
		{
			curpos += maxpos;
			if (curpos < mDataList.size())
			{
				s << " & ";
				PrintData( mDataList[curpos], s);
			}
		}

		s << " \\\\\n";

		++curpos_base;
	}

	//s << "\\end{tabular}" << "\n";

	return true;
}

std::string DataDic_Latex::EscapeChar(const std::string& s) const
{
	string r;

	for (string::const_iterator
			c  = s.begin();
			c != s.end();
			c++)
	{
		if ( *c == '_' )
			r.append("\\");
		
		r.append( &(*c), 1 );
	}

	return r;
}

void DataDic_Latex::PrintData(const OneData& d, std::ostream& os) const
{
	if (d.ID)
		os << "\\underline{" << EscapeChar(d.Data) << "}";
	else
		os << EscapeChar(d.Data);

	if (mShowType)
	{
		if (!d.Type.empty())
			os << " & \\textbf{" << d.Type << "}";
		else
			os << " &";
	}
}

}
