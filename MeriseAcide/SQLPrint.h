/*
 * \file
 * \brief SQL Printer.
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
 * Class d'impression SQL.
 *
 * $Id: SQLPrint.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _SQLPRINT_H_
#define _SQLPRINT_H_

#include <string>
#include <list>
#include <fstream>

namespace MeriseAcide
{

typedef std::list<std::string> StringList;

class SQLPrint
{
	public:
		SQLPrint(void);
		SQLPrint(const std::string& FileName);
		SQLPrint(std::ostream& os);
		virtual ~SQLPrint(void);

		bool IsOpen(void) const;
		bool Open(const std::string& FileName);
		bool Open(std::ostream& os);
		void Close();
		void BreakLine(void);

		virtual void DropTableIfExist(const std::string& t) = 0;
		virtual void DisableForeingKeyCheck(void) = 0;
		virtual void CreateTable(const std::string& t) = 0;
		virtual void AddKey(const std::string& f,
				const std::string& t, bool autoinc = true) = 0;
		virtual void AddFKey(const std::string& fk,
				const std::string& t) = 0;
		virtual void PrimaryKey(const StringList& fl) = 0;
		virtual void AddField(const std::string& f, const std::string& t) = 0;
		virtual void AddConstraint(
				const std::string& fk,
				const std::string& t,
				const std::string& k) = 0;
		virtual void CreateTable(void) = 0;
		virtual void EnableForeingKeyCheck(void) = 0;
		virtual void PutComment(const std::string& c) = 0;

	protected:
		std::ostream*  mOStream;

		void NeedComa(void);
		void PutComaIfNeeded(void);
		void ResetComa(void);
		void PutHeader(void);

	private:
		std::string    mFileName;
		std::ofstream  mFileStream;
		bool           mPutComa;
};

}

#endif /* _SQLPRINT_H_ */
