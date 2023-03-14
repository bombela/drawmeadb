/*
 * \file
 * \brief Fonctions utiles.
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
 * Quelques fonctions et définition utiles.
 *
 * $Id: Utils.cpp 87 2008-08-13 21:12:29Z bombela $
 */

#include "Utils.h"

#include <cctype>
#include <algorithm>

using namespace std;

namespace MeriseAcide
{

// FIXME
// REMOVE THIS ABBERATION !
void NormalizeBlank(std::string& Line)
{
	string::size_type LastPos = -1;
	string::size_type Pos;

	while ( (Pos = Line.find_first_of(BLANKCHAR, LastPos+1)) != string::npos)
	{
		if ( Pos == LastPos+1)
		{
			Line.erase(Pos, 1); 
		}
		else
		{
			Line.replace(Pos, 1, 1, ' ');
			LastPos = Pos;
		}
	}
}

void ToLower(std::string& s)
{
	transform(s.begin(), s.end(), s.begin(), char_tolower());
}

}
