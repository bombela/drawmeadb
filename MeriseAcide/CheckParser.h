/*
 * \file
 * \brief CheckParser.
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
 * Simple string checker. The goal is to forward into the string
 * an check part by part.
 *
 * $Id: CheckParser.h 88 2008-11-16 17:00:12Z bombela $
 */

#ifndef _CHECKPARSER_H_
#define _CHECKPARSER_H_

#include <string>

class CheckParser
{
	public:
		CheckParser(const std::string& s):
			mStart(s.begin()), mEnd(s.end()) {}

		bool Goto(const char c[]);
		int  Skip(const char c[]);
		bool Eos() const { return mStart == mEnd; }

	private:
		std::string::const_iterator mStart;
		std::string::const_iterator mEnd;
};

#endif /* _CHECKPARSER_H_ */
