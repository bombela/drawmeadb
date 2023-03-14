/*
 * \file
 * \brief Cardinalitée.
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
 * Class cardinalitée.
 *
 * $Id: MCDParserCardinality.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _MCDPARSERCARDINALITY_H_
#define _MCDPARSERCARDINALITY_H_

#include <string>
#include <iostream>
#include <sstream>

#include "Cardinality.h"

namespace MeriseAcide
{

/**
 * \brief Cardinalitée.
 *
 * Cette class représente une cardinalitée.
 * Elle est capable de se charger depuis
 * une chaîne de texte au format MeriseAcide.
 */
class MCDParserCardinality: public Cardinality
{
	public:
		bool        LoadFromString(const std::string& Line);
		std::string GetErrorMsg(void) const { return mErrorMsg; }

	private:
		std::string mErrorMsg;
};

}

#endif /* _MCDPARSERCARDINALITY_H_ */
