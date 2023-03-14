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
 * $Id: MPDTable.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "MPDTable.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace MeriseAcide
{

void MPDTable::AddKey(const MPDField& k)
{
	mKeyList.push_back( UniqField(k, mKeyList) );
}

void MPDTable::AddForeingKey(const MPDField& fk)
{
	mFKeyList.push_back( UniqField(fk, mFKeyList) );
}

void MPDTable::AddForeingKey(const MPDFieldList& fkl)
{
	for (MPDFieldList::const_iterator
			i  = fkl.begin();
			i != fkl.end();
			i++)
	{
		AddForeingKey( *i );
	}
}

void MPDTable::AddKey(const MPDFieldList& kl)
{
	for (MPDFieldList::const_iterator
			i  = kl.begin();
			i != kl.end();
			i++)
	{
		AddKey( *i );
	}
}

MPDField MPDTable::UniqField(const MPDField& f, const MPDFieldList& RefList) const
{
	MPDField Field = f;

	string Name = Field.GetName();
	int PostNumber = 1;
	const MPDField* FoundField;
	do
	{
		if ( (FoundField = FindFieldByName(RefList, Name)) )
		{
			ostringstream os;
			os << Field.GetName() << ++PostNumber;
			Name = os.str();
		}
	}
	while(FoundField);
	
	if (PostNumber > 0)
		Field.SetName( Name );

	return Field;
}

const MPDField* MPDTable::FindFieldByName(const MPDFieldList& fl,
		const std::string& n) const
{
	for (MPDFieldList::const_iterator
			i  = fl.begin();
			i != fl.end();
			i++)
	{
		if ( n == (*i).GetName() )
			return &(*i);
	}

	return 0;
}

}
