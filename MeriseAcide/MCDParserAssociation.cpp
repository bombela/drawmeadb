/*
 * \file
 * \brief Association.
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
 * Class association.
 *
 * $Id: MCDParserAssociation.cpp 88 2008-11-16 17:00:12Z bombela $
 */

#include "MCDParserAssociation.h"

#include <sstream>

#include "Utils.h"
#include "CheckParser.h"

using namespace std;

namespace MeriseAcide
{

// Abonnement    0,n    Concerne    0,n    Personne, Matière
// a n,n b n,n c
bool MCDParserAssociation::Check(std::string& Line)
{
	if (Line.size() < 13) return false;
	
	CheckParser cp(Line);
	if (!cp.Goto(BLANKCHAR)) return false;
	cp.Skip(BLANKCHAR);
	if (!cp.Goto(",")) return false;
	if (!cp.Goto(BLANKCHAR)) return false;
	cp.Skip(BLANKCHAR);
	if (!cp.Goto(BLANKCHAR)) return false;
	cp.Skip(BLANKCHAR);
	if (!cp.Goto(",")) return false;
	if (!cp.Goto(BLANKCHAR)) return false;
	cp.Skip(BLANKCHAR);
	return !cp.Eos();
}

bool MCDParserAssociation::LoadFromString(std::string& Line)
{
	string::size_type First, Next;
	string Sub;
	string Assoc = Line;

	// FIXME
	// Yes, it's help to reduce
	// intellectual complexity,
	// but it's not really good
	// for the computer ;)
	// It's possible to directly use
	// find_first_of/find_first_not_of
	// without big work.
	NormalizeBlank(Assoc);

	// Left entity
	Next = Assoc.find(' ');
	mLeftEntity = Assoc.substr(0, Next);
	First = Next+1;

	// Left cardinality
	Next = Assoc.find(' ', First);
	Sub = Assoc.substr(First, Next - First);
	if (!mLeftCardinality.LoadFromString(Sub))
	{
		mErrorMsg = "Error when reading left cardinality:\n";
		mErrorMsg.append(mLeftCardinality.GetErrorMsg());
		return false;
	}
	First = Next+1;
	
	// Assoc name
	Next = Assoc.find(' ', First);
	SetName( Assoc.substr(First, Next - First) );
	First = Next+1;

	// Right cardinality
	Next = Assoc.find(' ', First);
	Sub = Assoc.substr(First, Next - First);
	if (!mRightCardinality.LoadFromString(Sub))
	{
		mErrorMsg = "Error when reading right cardinality:\n";
		mErrorMsg.append(mRightCardinality.GetErrorMsg());
		return false;
	}
	First = Next+1;

	// Right entity (may be many)
	Sub = Assoc.substr(First, Assoc.size());
	istringstream iss(Sub);
	mRightEntity.clear();

	while ( getline(iss, Sub, ',') )
	{
		if (!Sub.empty())
		{
			Trim(Sub);
			mRightEntity.push_back(Sub);
		}
	}

	return true;
}

}
