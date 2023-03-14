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
 * Parse command line.
 *
 * $Id: Options.cpp 86 2008-08-09 20:07:04Z bombela $
 */

#include "Options.h"

#include <unistd.h>
#include <iostream>
#include <sstream>

#include "Utils.h"
#include "SQLPrint_MySQL.h"
#include "SQLPrint_SQLite.h"
#include "SQLPrint_PostgreSQL.h"
#include "DataDic_Plain.h"
#include "DataDic_Latex.h"

using namespace std;

namespace MeriseAcide
{

/**
 * Note : this function will be modifie the position
 * of element in argv.
 *
 * \return True if ok, false if not.
 */ 
bool Options::Parse(int argc, char* argv[])
{
	int opt;

	opterr = 0;
	while ( (opt = getopt(argc, argv, ":hvs:t:f:m:p:Tcd:D:l:r:e:E:A:B:n")) != -1)
	{
		switch (opt)
		{
			case 'h':
				mPrintHelp = true;
				return true;

			case 'v':
				mVerboseMode = true;
				break;

			case 'i':
				mMaxIterate = atoi(optarg);
				break;

			case 's':
				mSqlFileName = optarg;
				break;

			case 't':
				mSqlType = optarg;
				ToLower(mSqlType);
				if (!CheckSqlSupport(mSqlType))
					return false;
				break;

			case 'f':
				mSqlOutFmt = optarg;
				ToLower(mSqlOutFmt);
				if (!CheckSqlOuputFmt(mSqlOutFmt))
					return false;
				break;

			case 'm':
				mSqlMCDFile = optarg;
				break;

			case 'p':
				mSqlMPDFile = optarg;
				break;

			case 'T':
				mSqlPrintType = true;
				break;

			case 'c':
				mUseColor = true;
				break;

			case 'd':
				mDataDicFileName = optarg;
				break;

			case 'D':
				mDataDicFmt = optarg;
				ToLower(mDataDicFmt);
				if (!CheckDataDicFmt(mDataDicFmt))
					return false;
				break;

			case 'l':
				mDataDicLatexCol = atoi(optarg);
				break;

			case 'r':
				{
					string GvRenderer = optarg;
					ToLower(GvRenderer);
					if (!CheckGvRenderer(GvRenderer, mGvRenderer))
						return false;
					break;
				}
			
			case 'e':
				mEdgeLen = atof(optarg);
				break;

			case 'E':
				mColorEntity = optarg;
				break;

			case 'A':
				mColorAssoc = optarg;
				break;

			case 'B':
				mColorTable = optarg;
				break;

			case 'n':
				mSQLFKeyDisabled = true;
				break;

			case ':':
				mErrorMsg = "Argument option forgotten for \"-";
				mErrorMsg.push_back( static_cast<char>(optopt) );
				mErrorMsg.append("\"");
				return false;

			case '?':
				mErrorMsg = "Uknown parameter : \"-";
				mErrorMsg.push_back( static_cast<char>(optopt) );
				mErrorMsg.append("\"");
				return false;

			default:
				mErrorMsg = "Argument not implemented !";
				return false;
		}
	}

	if (optind >= argc)
	{
		mErrorMsg = "MCD file argument expected !";
		return false;
	}
	else
		mFileName = argv[optind];

	return true;
}

bool Options::CheckSqlSupport(const std::string& st)
{
	static const char* Fmt[] =
	{
	       "mysql",
	       "sqlite",
	       "postgresql",
	       0
	};

	for (int i = 0; Fmt[i] != 0; i++)
		if (Fmt[i] == st)
			return true;

	mErrorMsg = "SQL output type not supported : \"";
	mErrorMsg.append(st);
	mErrorMsg.append("\"");
	return false;
}

SQLPrint* Options::NewSqlObj(void) const
{
	if (mSqlType == "mysql")
		return new SQLPrint_MySQL;

	if (mSqlType == "sqlite")
		return new SQLPrint_SQLite;

	if (mSqlType == "postgresql")
		return new SQLPrint_PostgreSQL;

	return 0;
}

bool Options::CheckSqlOuputFmt(const std::string& so)
{
	static const char* Fmt[] =
	{
	       "pdf",
	       "png",
	       0
	};

	for (int i = 0; Fmt[i] != 0; i++)
		if (Fmt[i] == so)
			return true;

	mErrorMsg = "Output format (" + so + ") unknown";
	return false;
}

bool Options::CheckDataDicFmt(const std::string& df)
{
	static const char* Fmt[] =
	{
	       "plain",
	       "latex",
	       0
	};

	for (int i = 0; Fmt[i] != 0; i++)
		if (Fmt[i] == df)
			return true;

	mErrorMsg = "Data dictionnary output format not supported : \"";
	mErrorMsg.append(df).append("\"");
	return false;
}

bool Options::CheckGvRenderer(const std::string& rs, ERender& r)
{
	struct Render
	{
		const char* Name;
		ERender     Enum;
	};

	static const Render RenderTab[] =
	{
		{"neato", RNEATO},
		{"fdp",   RFDP},
		{"dot",   RDOT},
		{"twopi", RTWOPI},
		{"circo", RCIRCO},
		{0,       RNEATO}
	};

	for (int i = 0; RenderTab[i].Name != 0; i++)
		if (RenderTab[i].Name == rs)
		{
			r = RenderTab[i].Enum;
			return true;
		}

	mErrorMsg = "Render not supported : \"" + rs + "\"";
	return false;
}

DataDic* Options::NewDataDicObj(void) const
{
	DataDic* r = 0;

	if (mDataDicFmt == "plain")
		r = new DataDic_Plain;

	if (mDataDicFmt == "latex")
	{
		DataDic_Latex* l = new DataDic_Latex;
		l->SetColNum(mDataDicLatexCol);
		r = l;
	}

	if (r)
		r->SetShowType(mSqlPrintType);

	return r;
}

std::string Options::GetUsage(char const SelfPath[]) const
{
	std::ostringstream Result;
	Result	<< "Usage : " << SelfPath
		<< " [-h] [-v]\n"
		<< " [-s CreateDB.sql [-t <MySQL|SQLite|PostgreSQL>] [-n]]\n"
		<< " [-m MCDFileName -p MPDFileName [-f <pdf|png>]"
		<< " [-c [-E/A/B Color]] [-r <fdp|neato>] [-e EdgeLen]]\n"
		<< " [-d DictionnaryDataFileName [-D <plain|latex>] [-l NumCol]] [-T]\n"
		<< " MeriseAcideFileName";
	return Result.str();
}

std::string Options::GetHelp(char const SelfPath[]) const
{
	std::ostringstream Result;

	Result
		<<
			"MeriseAcide is distributed in the hope that it will be useful,\n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
			"GNU General Public License for more details.\n\n"
		<< GetUsage(SelfPath) << "\n\n"
		<< "\
-h\n\
	Print this help.\n\
\n\
-v\n\
	Verbose mode.\n\
\n\
-s CreateDB.sql [-t <MySQL|SQLite|PostgreSQL>] [-n]\n\
	Generate SQL for creating database.\n\
	The default output use MySQL type.\n\
	\n\
	-n\n\
		Don't generate foreing key constraint.\n\
	\n\
\n\
-m MCDFileName -p MPDFileName [-f <pdf|png>] [-c [-E/A/B Color]]\
 [-r <fdp|neato>] [-e EdgeLen]\n\
	Generate graphical MCD and/or MPD.\n\
	The default format is pdf.\n\
	\n\
	-c [-E Color -A Color -B Color]\n\
		Add prety nice color.\n\
		For override default color :\n\
		\n\
		-E Color\n\
			Entity color.\n\
		\n\
		-A Color\n\
			Association color.\n\
		\n\
		-B Color\n\
			Table color.\n\
		\n\
		Color is a graphviz color.\n\
		You can also use the HTML format. ie: #FF67AA\n\
	\n\
	-r <fdp|neato>\n\
		Select the renderer for MCD.\n\
		Default is neato.\n\
	\n\
	-e EdgeLen\n\
		Prefered edge distance (numeric value).\n\
		Default value is 1.0.\n\
\n\
-d DictionnaryDataFileName [-D <plain|latex>] [-l NumCol]\n\
	Generate dictionnary data file.\n\
	The default format is plain.\n\
	\n\
	-l NumCol\n\
		When output format is latex,\n\
		-l can be used for specifie\n\
		the number tabular colums.\n\
		Default is 1.\n\
\n\
-T\n\
	Print SQL type on graph and dictionnary.\n\
	\n\
MeriseAcideFileName\n\
	The input MeriseAcide file\n\
	with MCD definition.\n\
";

	return Result.str();
}

Graphviz* Options::NewGraphvizObj(void) const
{
	Graphviz* r = new Graphviz;

	r->SetMaxIterate(mMaxIterate);
	r->SetUseColor(mUseColor);
	r->SetEdgeLen(mEdgeLen);
	r->SetColor(TSEntity, mColorEntity);
	r->SetColor(TSAssoc, mColorAssoc);
	r->SetColor(TSTable, mColorTable);

	return r;
}

}
