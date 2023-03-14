/*
 * \file
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
 * $Id: MCDParserAttribute.cpp 87 2008-08-13 21:12:29Z bombela $
 */

#include "MCDParserAttribute.h"

#include <sstream>
#include <iostream>
#include <assert.h>

#include "Utils.h"
#include "MCDParserAttributeContainer.h"

using namespace std;

namespace MeriseAcide
{

bool MCDParserAttribute::Check(const std::string& Line)
{
	if (Line.size() < 1)
		return false;
	else
		return true;
}

bool MCDParserAttribute::LoadFromString(const std::string& Line)
{
	if (Line.size() == 0)
	{
		mErrorMsg = "Aucun champ trouvé";
		return false;
	}

	string FieldName(Line);

	// Test the possibilitie of key.
	if (FieldName[0] == '+')
	{
		FieldName.erase(0, 1);
		TrimL(FieldName);

		if (FieldName.empty())
		{
			mErrorMsg = "Empty key";
			return false;
		}

		mKey = true;
	}
	else
		mKey = false;

	// Test the possibilitie of SQLType.
	string::size_type pos = FieldName.find_first_of(BLANKCHAR);
	if (pos == string::npos)
	{
		// No SQLType.
		mSqlType = "";
	}
	else
	{
		// SQLType defined.
		mSqlType = FieldName.substr(pos+1, Line.size());
		Trim(mSqlType);

		// Only name.
		FieldName.erase(pos);
		TrimR(FieldName);
	}

	// Test the possibilitie of composed name.
	if (FieldName[0] == '_')
	{
		// Left composition.
		mComposition = Left;
		FieldName.erase(0,1);
	}
	else if (FieldName[FieldName.size()-1] == '_')
	{
		// Right composition.
		mComposition = Right;
		FieldName.erase(FieldName.size()-1);
	}
	else
		mComposition = None;
	
	// Check validity.
	if ( (mComposition != None) && (FieldName.empty()) )
	{
		if (mKey)
			mErrorMsg = "Empty key";
		else
			mErrorMsg = "Empty field";
		return false;
	}
	else
	{
		// Store Fieldname.
		mName = FieldName;
		return true;
	}
}

std::string MCDParserAttribute::GetName(void) const
{
	assert(mParent);

	string FullName;
	switch (mComposition)
	{
		case None : FullName = mName; break;
		case Left : FullName = mParent->GetName() + '_' + mName; break;
		case Right: FullName = mName + "_" + mParent->GetName(); break;
	}
	
	return FullName;
}

}
