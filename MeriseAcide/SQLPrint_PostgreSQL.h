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
 * $Id: SQLPrint_PostgreSQL.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _SQLPRINT_POSTGRESQL_H_
#define _SQLPRINT_POSTGRESQL_H_

#include "SQLPrint.h"

#include <string>
#include <list>
#include <fstream>

namespace MeriseAcide
{

class SQLPrint_PostgreSQL: public SQLPrint
{
	public:
		SQLPrint_PostgreSQL():
			mFKeyConstraintDeferred(false),
			mLastTableCreated(0)
			{ }

		void DropTableIfExist(const std::string& t);
		void DisableForeingKeyCheck(void);
		void CreateTable(const std::string& t);
		void AddKey(const std::string& f,
				const std::string& t, bool autoinc = true);
		void AddFKey(const std::string& fk, const std::string& t);
		void PrimaryKey(const StringList& fl);
		void AddField(const std::string& f, const std::string& t);
		void AddConstraint(
				const std::string& fk,
				const std::string& t,
				const std::string& k);
		void CreateTable(void);
		void EnableForeingKeyCheck(void);
		void PutComment(const std::string& c);
	
	private:
		struct FKeyConstraint
		{
			std::string FKeyTable;
			std::string FKey;
			std::string KeyTable;
			std::string Key;
		};

		bool                      mFKeyConstraintDeferred;
		std::list<FKeyConstraint> mFKeyConstraintList;
		const std::string*        mLastTableCreated;
};

}

#endif /* _SQLPRINT_POSTGRESQL_H_ */
