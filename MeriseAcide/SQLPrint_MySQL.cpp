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
 * MySQL printer class.
 *
 * $Id: SQLPrint_MySQL.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "SQLPrint_MySQL.h"

#include <iostream>
#include <iterator>

#include "DateTime.h"

using namespace std;

namespace MeriseAcide
{

void SQLPrint_MySQL::DropTableIfExist(const std::string& t)
{
	*mOStream << "DROP TABLE IF EXISTS " << t << ";\n";
}

void SQLPrint_MySQL::DisableForeingKeyCheck(void)
{
	*mOStream << "SET FOREIGN_KEY_CHECKS = 0;\n";
}

void SQLPrint_MySQL::CreateTable(const std::string& t)
{
	*mOStream << "CREATE TABLE " << t << " (" << endl;
}

void SQLPrint_MySQL::AddKey(const std::string& k, const std::string& t, bool autoinc)
{
	PutComaIfNeeded();

	*mOStream << "\t" << k;
	if (t.empty())
	{
		*mOStream << " INTEGER UNSIGNED NOT NULL";
		if (autoinc)
			*mOStream << " AUTO_INCREMENT";
	}
	else
		*mOStream << " " << t;

	NeedComa();
}

void SQLPrint_MySQL::AddFKey(const std::string& fk, const std::string& t)
{
	PutComaIfNeeded();

	*mOStream << "\t" << fk;
	if (t.empty())
	{
		*mOStream << " INTEGER UNSIGNED NOT NULL";
	}
	else
		*mOStream << " " << t;

	NeedComa();
}

void SQLPrint_MySQL::PrimaryKey(const StringList& fl)
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

void SQLPrint_MySQL::AddField(const std::string& f, const std::string& t)
{
	PutComaIfNeeded();

	*mOStream << "\t" << f;
	if (t.empty())
		*mOStream << " VARCHAR(30)";
	else
		*mOStream << " " << t;

	NeedComa();
}

void SQLPrint_MySQL::AddConstraint(
		const std::string& fk,
		const std::string& t,
		const std::string& k)
{
	PutComaIfNeeded();

	*mOStream << "\tFOREIGN KEY ( " << fk << " )\n"
		<< "\t\tREFERENCES "<< t << "(" << k << ") ON DELETE CASCADE";

	NeedComa();
}

void SQLPrint_MySQL::CreateTable()
{
	ResetComa();
	*mOStream << "\n) TYPE=INNODB;\n";
}

void SQLPrint_MySQL::EnableForeingKeyCheck(void)
{
	*mOStream << "SET FOREIGN_KEY_CHECKS = 1;\n";
}

void SQLPrint_MySQL::PutComment(const std::string& c)
{
	*mOStream << "-- " << c << "\n";
}

}
