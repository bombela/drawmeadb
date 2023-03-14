/*
 * \file
 * \brief Cardinalitée.
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
 * Class cardinalitée.
 *
 * $Id: MCDParserCardinality.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "MCDParserCardinality.h"

#include <sstream>
#include <iostream>

#include "MCDParserCardinalityValue.h"

using namespace std;

namespace MeriseAcide
{

bool MCDParserCardinality::LoadFromString(const std::string& Line)
{
	if (Line.size() < 3)
	{
		mErrorMsg = "Trop peu de texte";
		return false;
	}

	string Assoc(Line);

	if (Assoc[0] == '(')
	{
		if (Assoc[Assoc.size()-1] != ')')
		{
			mErrorMsg = "Parenthèse non refermée.";
			return false;
		}
		
		Assoc.erase(0,1);
		Assoc.erase(Assoc.size()-1, 1);
		SetRelativeValue(true);
	}
	else
		SetRelativeValue(false);


	// Search coma and check syntax.
	string::size_type ComaPos = Assoc.find(',');
	if (ComaPos == string::npos)
	{
		mErrorMsg = "Aucune virgule trouvée.";
		return false;
	}

	MCDParserCardinalityValue CadVal;
	if ( !CadVal.LoadFromString(Assoc.substr(0, ComaPos)) )
	{
		mErrorMsg = "Erreur lors de la lecture de la valeur minimale.";
		return false;
	}
	SetMinValue( CadVal );

	if ( !CadVal.LoadFromString(Assoc.substr(ComaPos+1, Assoc.size())) )
	{
		mErrorMsg = "Erreur lors de la lecture de la valeur maximale.";
		return false;
	}
	SetMaxValue( CadVal );

	return true;
}

}
