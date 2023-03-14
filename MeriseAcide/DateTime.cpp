/**
 * \bref Date and time class helper.
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
 * $Id: DateTime.cpp 84 2008-07-25 22:12:56Z bombela $
 */

#include "DateTime.h"

#include <string>

using namespace std;

namespace MeriseAcide
{

void DateTime::LoadFromNow(void)
{
	tzset();
	time_t AbsoluteTime = time(0);
	
	// Warning not thread safe.
	mBrokenTime = *localtime(&AbsoluteTime);
}

std::string DateTime::Fmt(const std::string& f)
{
	char buf[31];
	strftime(buf, 31, f.c_str(), &mBrokenTime);

	return string(buf);
}

}
