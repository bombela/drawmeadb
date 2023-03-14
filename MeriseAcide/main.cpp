/**
 * \file
 * \brief Le début du programme.
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
 * $Id: main.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include <iostream>
#include <string>

#include "Options.h"
#include "MCDParser.h"
#include "MCDDatabase.h"
#include "MPDDatabase.h"
#include "SQLPrint.h"
#include "Graphviz.h"
#include "DataDic_Plain.h"
#include "MeriseAcide.h"

using namespace std;

int main (int argc, char* argv[])
{
	// Show license.
	cout
		<< "MeriseAcide v" << MeriseAcide_Version << " Copyright © 2008 Bombela <bombela@free.fr>\n"
		<<
		"This program comes with ABSOLUTELY NO WARRANTY; for details run with option '-h'.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; read file 'COPYING' shipped with sources for details.\n" << endl;

	// Parse options.
	MeriseAcide::Options OptParser;
	if ( !OptParser.Parse(argc, argv) )
	{
		cout << OptParser.GetUsage(argv[0]) << endl;
		cerr << "Error when parsing argument : "
			<< OptParser.GetErrorMsg() << endl;
		return 1;
	}

	if ( OptParser.GetPrintHelp() )
	{
		cout << OptParser.GetHelp(argv[0]) << endl;
		return 0;
	}

	// Parse the MCD file.
	MeriseAcide::MCDParser* Parser = new MeriseAcide::MCDParser;
	if ( !Parser->Parse(OptParser.GetFileName(), OptParser.IsVerboseMode()) )
	{
		cerr	<< "Error when parsing ("
			<< OptParser.GetFileName() << "):" << endl;
		cerr	<< Parser->GetErrorMsg() << endl;
		
		delete Parser;
		return 2;
	}

	// Build MCD database.
	MeriseAcide::MCDDatabase* MCDDb = new MeriseAcide::MCDDatabase;
	if ( !MCDDb->BuildFromMCDParser(*Parser, OptParser.IsVerboseMode()) )
	{
		cerr	<< "Error when building MCD database ("
			<< OptParser.GetFileName() << "):" << endl;
		cerr	<< MCDDb->GetErrorMsg() << endl;

		delete Parser;
		delete MCDDb;
		return 3;
	}

	// Not longer used.
	delete Parser;
	
	// Print dictionnary data.
	if ( !OptParser.GetDataDicFileName().empty() )
	{
		MeriseAcide::DataDic* dic = OptParser.NewDataDicObj();

		dic->LoadFromMCDDatabase(*MCDDb);

		if ( !dic->PrintToFile(OptParser.GetDataDicFileName()) )
		{
			cerr
				<< "Error when printing dictionnary data file : "
				<< dic->GetErrorMsg() << endl;

			delete dic;
			delete MCDDb;
			return 4;
		}

		cout
			<< "Dictionnary data file generated : "
			<< OptParser.GetDataDicFileName()
			<< " (" << OptParser.GetDataDicOutputFmt() << ")"
			<< endl;

		delete dic;
	}

	// Print graphical MCD.
	if ( !OptParser.GetSqlMCDFile().empty() )
	{
		MeriseAcide::Graphviz* Gv = OptParser.NewGraphvizObj();

		Gv->Reset(OptParser.GetGvRenderer());
		MCDDb->FillGraph(*Gv, OptParser.GetSqlPrintType());
		Gv->Compute();
		if ( !Gv->Render(
				OptParser.GetSqlOutputFmt(),
				OptParser.GetSqlMCDFile()) ) 
		{
			cerr
				<< "Error when rendering MCD : "
				<< Gv->GetErrorMsg() << endl;
			delete Gv;
			delete MCDDb;
			return 6;
		}

		cout << "MCD rendered : "
			<< OptParser.GetSqlMCDFile() << " ("
			<< OptParser.GetSqlOutputFmt() << ")." << endl;
		
		delete Gv;
	}
	
	// Build MPD database.
	MeriseAcide::MPDDatabase* MPDDb = new MeriseAcide::MPDDatabase;
	if ( !MPDDb->BuildFromMCDDatabase(*MCDDb, OptParser.IsVerboseMode()) )
	{
		cerr	<< "Error when building MPD database ("
			<< OptParser.GetFileName() << ") :" << endl;
		cerr	<< MPDDb->GetErrorMsg() << endl;

		delete MCDDb;
		delete MPDDb;
		return 3;
	}

	// Not longer used.
	delete MCDDb;

	// Print graphical MPD.
	if ( !OptParser.GetSqlMPDFile().empty() )
	{
		MeriseAcide::Graphviz* Gv = OptParser.NewGraphvizObj();

		Gv->Reset(MeriseAcide::RDOT);
		MPDDb->FillGraph(*Gv, OptParser.GetSqlPrintType());
		Gv->Compute();
		if ( !Gv->Render(
				OptParser.GetSqlOutputFmt(),
				OptParser.GetSqlMPDFile()) ) 
		{
			cerr
				<< "Error when rendering MPD : "
				<< Gv->GetErrorMsg() << endl;

			delete MPDDb;
			delete Gv;
			return 6;
		}

		cout << "MPD rendered : "
			<< OptParser.GetSqlMPDFile() << " ("
			<< OptParser.GetSqlOutputFmt() << ")." << endl;
		
		delete Gv;
	}
	
	// Require sql output ?
	if ( !OptParser.GetSqlFileName().empty() )
	{
		// Create SQLPrinter.
		MeriseAcide::SQLPrint* SQL = OptParser.NewSqlObj();

		if ( !SQL->Open(OptParser.GetSqlFileName()) )
		{
			cerr 	<< "Unable to open "
				<< OptParser.GetSqlFileName()
				<< endl;

			delete SQL;
			delete MPDDb;
			return 4;
		}

		// Print SQL MPD.
		if ( !MPDDb->PrintToSQL(*SQL, OptParser.SQLFKeyIsDisabled()) )
		{
			cerr 	<< "Error when generating SQL:\n"
				<<  MPDDb->GetErrorMsg() << endl;

			delete SQL;
			delete MPDDb;
			return 5;
		}

		cout << "SQL generated : "
			<< OptParser.GetSqlFileName() << " ("
			<< OptParser.GetSqlType() << ")." << endl;

		delete SQL;
	}

	delete MPDDb;

	return 0;
}

