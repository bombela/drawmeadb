/*
 * \file
 * \brief SQL Printer specialization.
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
 * SQLite printer class.
 *
 * $Id: SQLPrint_SQLite.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "SQLPrint_SQLite.h"

#include <iostream>
#include <iterator>

#include "DateTime.h"

using namespace std;

namespace MeriseAcide
{

void SQLPrint_SQLite::DropTableIfExist(const std::string& t)
{
	*mOStream << "DROP TABLE IF EXISTS " << t << ";\n";
}

void SQLPrint_SQLite::DisableForeingKeyCheck(void)
{
}

void SQLPrint_SQLite::CreateTable(const std::string& t)
{
	*mOStream << "CREATE TABLE " << t << " (" << endl;
}

void SQLPrint_SQLite::AddKey(const std::string& k, const std::string& t, bool autoinc)
{
	PutComaIfNeeded();

	if (t.empty())
	{
		if (autoinc)
		{
			*mOStream << "\t";
			PutComment("(Auto incremented by SQLite)");
		}
		*mOStream << "\t" << k;
		*mOStream << " INTEGER NOT NULL";
	}
	else
	{
		*mOStream << "\t" << k;
		*mOStream << " " << t;
	}

	NeedComa();
}

void SQLPrint_SQLite::AddFKey(const std::string& fk, const std::string& t)
{
	PutComaIfNeeded();

	*mOStream << "\t" << fk;
	if (t.empty())
	{
		*mOStream << " INTEGER NOT NULL";
	}
	else
	{
		*mOStream << " " << fk;
	}

	NeedComa();
}

void SQLPrint_SQLite::PrimaryKey(const StringList& fl)
{
	PutComaIfNeeded();
	
	*mOStream << "\tPRIMARY KEY ( ";
	
	int i = 0, c = fl.size();
	for (StringList::const_iterator
			str  = fl.begin();
			str != fl.end();
			str++)
	{
		*mOStream << *str;
		if (++i < c)
			*mOStream << ", ";
	}

	*mOStream << " )";

	NeedComa();
}

void SQLPrint_SQLite::AddField(const std::string& f, const std::string& t)
{
	PutComaIfNeeded();

	*mOStream << "\t" << f;
	if (t.empty())
		*mOStream << " VARCHAR(30)";
	else
		*mOStream << " " << t;

	NeedComa();
}

void SQLPrint_SQLite::AddConstraint(
		const std::string& fk,
		const std::string& t,
		const std::string& k)
{
	return ;
	PutComaIfNeeded();

	*mOStream << "\tFOREIGN KEY ( " << fk << " )\n"
		<< "\t\tREFERENCES "<< t << "(" << k << ") ON DELETE CASCADE";

	NeedComa();
}

void SQLPrint_SQLite::CreateTable()
{
	ResetComa();
	*mOStream << "\n);\n";
}

void SQLPrint_SQLite::EnableForeingKeyCheck(void)
{
}

void SQLPrint_SQLite::PutComment(const std::string& c)
{
	*mOStream << "-- " << c << "\n";
}

}
