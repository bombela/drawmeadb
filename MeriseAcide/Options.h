/*
 * \file
 * \brief Analyse des options en ligne de commande.
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
 * Analyser les options de la ligne de commande.
 *
 * $Id: Options.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <iostream>

#include "SQLPrint.h"
#include "DataDic.h"
#include "Graphviz.h"

namespace MeriseAcide
{

class Options
{
	public:
		Options(void):
			mMaxIterate(100000),
			mVerboseMode(false),
			mSqlType("mysql"),
			mSqlOutFmt("pdf"),
			mSqlPrintType(false),
			mUseColor(false),
			mPrintHelp(false),
			mDataDicFmt("plain"),
			mDataDicLatexCol(1),
			mGvRenderer(RNEATO),
			mEdgeLen(1.0),
			mColorEntity("gray65"),
			mColorAssoc("indianred1"),
			mColorTable("#b8f196"),
			mSQLFKeyDisabled(false)
			{}
				
		bool	    Parse(int argc, char* argv[]);

		std::string        GetUsage(char const SelfPath[]) const;
		std::string        GetHelp(char const SelfPath[]) const;
		const std::string& GetFileName(void)         const { return mFileName; }
		int                GetMaxIterate(void)       const { return mMaxIterate; }
		bool               IsVerboseMode(void)       const { return mVerboseMode; }
		bool               GetSqlPrintType(void)     const { return mSqlPrintType; }
		std::string        GetSqlFileName(void)      const { return mSqlFileName; }
		std::string        GetSqlType(void)          const { return mSqlType; }
		std::string        GetSqlOutputFmt(void)     const { return mSqlOutFmt; }
		std::string        GetSqlMCDFile(void)       const { return mSqlMCDFile; }
		std::string        GetSqlMPDFile(void)       const { return mSqlMPDFile; }
		bool               GetUseColor(void)         const { return mUseColor; }
		bool               GetPrintHelp(void)        const { return mPrintHelp; }
		ERender            GetGvRenderer(void)       const { return mGvRenderer; }
		float              GetEdgeLen(void)          const { return mEdgeLen; }
		bool               SQLFKeyIsDisabled(void)   const { return mSQLFKeyDisabled; }
  
	  	std::string        GetDataDicFileName(void)  const { return mDataDicFileName; }
	  	std::string        GetDataDicOutputFmt(void) const { return mDataDicFmt; }
	  	DataDic*           NewDataDicObj(void)       const;
	  	Graphviz*          NewGraphvizObj(void)      const;
		SQLPrint*          NewSqlObj(void)           const;

		std::string GetErrorMsg(void) const { return mErrorMsg; }
	
	private:
		std::string     mErrorMsg;
		std::string     mFileName;
		int             mMaxIterate;
		bool            mVerboseMode;
		std::string     mSqlFileName;
		std::string     mSqlType;
		std::string     mSqlOutFmt;
		std::string     mSqlMCDFile;
		std::string     mSqlMPDFile;
		bool            mSqlPrintType;
		bool            mUseColor;
		bool            mPrintHelp;
		std::string     mDataDicFileName;
		std::string     mDataDicFmt;
		unsigned int    mDataDicLatexCol;
		ERender		mGvRenderer;
		float		mEdgeLen;
		std::string     mColorEntity;
		std::string     mColorAssoc;
		std::string     mColorTable;
		bool            mSQLFKeyDisabled;

		bool CheckSqlSupport(const std::string& st);
		bool CheckSqlOuputFmt(const std::string& so);
		bool CheckDataDicFmt(const std::string& df);
		bool CheckGvRenderer(const std::string& rs, ERender& r);
};

}

#endif /* _OPTIONS_H_ */
