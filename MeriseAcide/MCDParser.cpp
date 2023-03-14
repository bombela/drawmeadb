/*
 * \file
 * \brief Parseur de MCD au format MeriseAcide.
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
 * Ce parser est capable de contruire
 * une base d'inforation depuis un MCD
 * au format MeriseAcide.
 *
 * $Id: MCDParser.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "MCDParser.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Utils.h"
#include "MCDParserEntity.h"
#include "MCDParserAssociation.h"
#include "MCDParserAttribute.h"

using namespace std;

namespace MeriseAcide
{

// Defines
#define MAXLINECHAR 4096

// Enums
enum AnalyseState
{
	AS_START,
	AS_ENTITY,
	AS_ASSOCIATION
}; // FSM : Current state.

MCDParser::MCDParser(void):
	mErrorMsg("") {}

MCDParser::~MCDParser(void)
{
	for_each(mEntityList.begin(), mEntityList.end(), Delete());
	for_each(mAssocList.begin(),  mAssocList.end(),  Delete());
}

bool MCDParser::Parse(const std::string& FileName, bool Verbose)
{
	mErrorMsg.str("");

	if (Verbose)
		cout << "--> MCDParser::Parse (" << FileName << ")" << endl;

	// Open file
	ifstream fs(FileName.c_str());

	if (!fs.is_open())
	{
		mErrorMsg << "Error when opening \""
			<< FileName << "\".";
		return false;
	}

	int                     LineNum = 0;
	char                    LineBuf[MAXLINECHAR];
	string                  Line;
	AnalyseState	        State = AS_START;
	MCDParserAttributeContainer* CurrentFieldContainer = 0;

	Line.reserve(MAXLINECHAR);

	while (!fs.eof())
	{
		fs.getline(LineBuf, MAXLINECHAR);
		Line = LineBuf;
		LineNum++;

		RemoveComment(Line);
		Trim(Line);
		if (Line.empty()) continue;
		
		if      ( MCDParserEntity::Check(Line) )
		{
			MCDParserEntity* e = new MCDParserEntity;
			if (!e->LoadFromString(Line))
			{
				mErrorMsg << "Line " << LineNum
					<< ", Entity parse error:\n " << Line
					<< "\n" << e->GetErrorMsg();
				delete e;
				fs.close();
				return false;
			}

			State = AS_ENTITY;
			mEntityList.push_back(e);
			CurrentFieldContainer = e;

			if (Verbose)
				cout << "Entity: " << *e << endl;
		}
		else if ( MCDParserAssociation::Check(Line) )
		{
			MCDParserAssociation* a = new MCDParserAssociation;
			if (!a->LoadFromString(Line))
			{
				mErrorMsg << "Line " << LineNum
					<< ", Association parse error:\n"
					<< Line
					<< "\n" << a->GetErrorMsg();
				fs.close();
				return false;
			}

			State = AS_ASSOCIATION;
			mAssocList.push_back(a);
			CurrentFieldContainer = a;

			if (Verbose)
				cout << "Assoc : " << *a << endl;
		}
		else if ( (State == AS_ENTITY || State == AS_ASSOCIATION)
				&& MCDParserAttribute::Check(Line) )
		{
			MCDParserAttribute& f = CurrentFieldContainer->NewField();
			if (!f.LoadFromString(Line))
			{
				mErrorMsg << "Line " << LineNum
					<< ", Field error:\n" << Line
					<< "\n" << f.GetErrorMsg();
				fs.close();
				return false;
			}

			if ( (State == AS_ASSOCIATION) && (f.IsKey()) )
			{
				mErrorMsg << "Line " << LineNum
					<< ", Association can't have a key:\n"
					<< Line;
				fs.close();
				return false;
			}

			if (Verbose)
				cout << " Field: " << f << endl;
		}
		else
		{
			mErrorMsg << "Line " << LineNum
				<< ", unknown element:\n" << Line;
			fs.close();
			return false;
		}
	}

	fs.close();
	return true;
}

void MCDParser::RemoveComment(std::string& Line)
{
	int Pos = Line.find('#');
	if (Pos > -1)
		Line.erase(Pos);
}

}
