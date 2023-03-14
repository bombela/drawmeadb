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
 * PostgreSQL printer class.
 *
 * $Id: SQLPrint_PostgreSQL.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "SQLPrint_PostgreSQL.h"

#include <iostream>
#include <iterator>

#include "DateTime.h"

using namespace std;

namespace MeriseAcide
{

void SQLPrint_PostgreSQL::DropTableIfExist(const std::string& t)
{
	*mOStream << "DROP TABLE IF EXISTS " << t << " CASCADE;\n";
}

void SQLPrint_PostgreSQL::DisableForeingKeyCheck(void)
{
	mFKeyConstraintDeferred = true;
}

void SQLPrint_PostgreSQL::EnableForeingKeyCheck(void)
{
	PutComaIfNeeded();

	for (std::list<FKeyConstraint>::const_iterator
			i  = mFKeyConstraintList.begin();
			i != mFKeyConstraintList.end();
			i++)
	{
		string hack;
		int pos = i->FKey.find(',');
		if (pos != -1)
			hack = i->FKey.substr(0, pos);
		else
			hack = i->FKey;

		*mOStream << "\nALTER TABLE ONLY " << i->FKeyTable
			<< "\n\tADD CONSTRAINT "
			<< i->FKeyTable << "_" << hack << "_fkey"
			<< "\n\tFOREIGN KEY (" << i->FKey << ")"
			<< "\n\tREFERENCES " << i->KeyTable << "(" << i->Key << ")"
			<< "\n\tON DELETE CASCADE;\n";
	}

	mFKeyConstraintDeferred = false;
	mFKeyConstraintList.clear();
}

void SQLPrint_PostgreSQL::AddConstraint(
		const std::string& fk,
		const std::string& t,
		const std::string& k)
{
	if (mFKeyConstraintDeferred)
	{
		FKeyConstraint c;

		c.FKeyTable = *mLastTableCreated;
		c.FKey      = fk;
		c.KeyTable  = t;
		c.Key       = k;
		mFKeyConstraintList.push_back(c);
	}
	else
	{
		PutComaIfNeeded();

		*mOStream << "\tFOREIGN KEY ( " << fk << " )\n"
			<< "\t\tREFERENCES "<< t << "(" << k << ") ON DELETE CASCADE";

		NeedComa();
	}
}


void SQLPrint_PostgreSQL::CreateTable(const std::string& t)
{
	*mOStream << "CREATE TABLE " << t << " (" << endl;
	mLastTableCreated = &t;
}

void SQLPrint_PostgreSQL::AddKey(const std::string& k, const std::string& t, bool autoinc)
{
	PutComaIfNeeded();

	if (t.empty())
	{
		if (autoinc)
		{
			*mOStream << "\t";
			PutComment("(SERIAL = INTEGER NOT NULL auto incremented by PostgreSQL)");
			*mOStream << "\t" << k << " SERIAL";
		}
		else
		{
			*mOStream << "\t" << k << " INTEGER NOT NULL";
		}
	}
	else
	{
		*mOStream << "\t" << k;
		*mOStream << " " << t;
	}

	NeedComa();
}

void SQLPrint_PostgreSQL::AddFKey(const std::string& fk, const std::string& t)
{
	PutComaIfNeeded();

	*mOStream << "\t" << fk;
	if (t.empty())
	{
		*mOStream << " INTEGER NOT NULL";
	}
	else
	{
		if (t == "SERIAL")
			*mOStream << " INTEGER NOT NULL";
		else if (t == "BIGSERIAL")
			*mOStream << " BIGINT NOT NULL";
		else
			*mOStream << " " << t;
	}

	NeedComa();
}

void SQLPrint_PostgreSQL::PrimaryKey(const StringList& fl)
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

void SQLPrint_PostgreSQL::AddField(const std::string& f, const std::string& t)
{
	PutComaIfNeeded();

	*mOStream << "\t" << f;
	if (t.empty())
		*mOStream << " VARCHAR(30)";
	else
		*mOStream << " " << t;

	NeedComa();
}

void SQLPrint_PostgreSQL::CreateTable()
{
	ResetComa();
	*mOStream << "\n);\n";
	mLastTableCreated = 0;
}

void SQLPrint_PostgreSQL::PutComment(const std::string& c)
{
	*mOStream << "-- " << c << "\n";
}

}
