/*
 * \file
 * \brief Entity.
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
 * Classe entitée. Simple classe
 * polymorphique pour gérer une entitée.
 *
 * $Id: MCDParserEntity.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "MCDParserEntity.h"

#include <string>

#include "Utils.h"

using namespace std;

namespace MeriseAcide
{

bool MCDParserEntity::Check(const std::string& Line)
{
	if (Line.size() < 2) return false;
	return (Line[0] == '[' && Line[Line.size()-1] == ']');
}

bool MCDParserEntity::LoadFromString(std::string& Line)
{
	string Name = Line.substr(1, Line.size() - 2);
	Trim(Name);
	if (Name.empty())
	{
		mErrorMsg = "Entitée vide.";
		return false;
	}
	else
	{
		SetName(Name);
		return true;
	}
}

}
