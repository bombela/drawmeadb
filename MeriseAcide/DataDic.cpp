/*
 * \file
 * \brief Data dictionnary representation.
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
 * Data dictionnary representation.
 * Can print data dictionnary with many
 * representation.
 *
 * $Id: DataDic.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "DataDic.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace MeriseAcide
{

bool DataDic::LoadFromMCDDatabase(const MCDDatabase& mcd)
{
	for (MCDEntityList::const_iterator
			eI  = mcd.GetEntityList().begin();
			eI != mcd.GetEntityList().end();
			eI++)
	{
		OneData d;

		d.ID = true;
		for (MCDAttributeList::const_iterator
				kI  = eI->GetKeyList().begin();
				kI != eI->GetKeyList().end();
				kI++)
		{
			d.Data = kI->GetName();
			d.Type = kI->GetSqlType();
			if ( !kI->IsComposedName() )
				d.Data.append("_").append(eI->GetName());

			mDataList.push_back(d);
		}

		d.ID = false;
		for (MCDAttributeList::const_iterator
				fI  = eI->GetFieldList().begin();
				fI != eI->GetFieldList().end();
				fI++)
		{
			d.Data = fI->GetName();
			d.Type = fI->GetSqlType();
			if ( !fI->IsComposedName() )
				d.Data.append("_").append(eI->GetName());

			mDataList.push_back(d);
		}
	}

	return true;
}

bool DataDic::PrintToFile(const std::string& FileName)
{
	mErrorMsg.str("");

	ofstream of(FileName.c_str());

	if ( ! of.is_open() )
	{
		mErrorMsg << "Error when oppening " << FileName;
		return false;
	}

	bool r = Print(of);
	of.close();
	return r;
}

}
