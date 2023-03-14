/*
 * \file
 * \brief MPD Field.
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
 * $Id: MPDField.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "MPDField.h"

#include <string>

using namespace std;

namespace MeriseAcide
{

std::string MPDField::GetFullName(void) const
{
	if (IsForeingKey())
		return "fk" + mName;
	else
		return mName;
}

std::string MPDField::GetFullNameAscii(void) const
{
	string r = GetFullName();
	
	for (string::iterator i = r.begin();
			i != r.end();
			i++)
	{
		if ( *i & 0x80 ) // >= 128
			*i = '&';
	}

	return r;
}

}
