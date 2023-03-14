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
 * $Id: Cardinality.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _CARDINALITY_H_
#define _CARDINALITY_H_

#include <string>
#include <iostream>
#include <sstream>

#include "CardinalityValue.h"

namespace MeriseAcide
{

/**
 * \brief Cardinalitée.
 *
 * Cette class représente une cardinalitée.
 */
class Cardinality
{
	public:
		virtual ~Cardinality(void) { }

		bool IsRelative(void) const { return mRelative; }

		const CardinalityValue& GetMin(void)     const { return mMin; }
		const CardinalityValue& GetMax(void)     const { return mMax; }

	protected:
		void SetRelativeValue(bool v) { mRelative = v; }
		void SetMinValue(const CardinalityValue& v) { mMin = v; }
		void SetMaxValue(const CardinalityValue& v) { mMax = v; }

	private:
		bool	    mRelative;
		CardinalityValue mMin;
		CardinalityValue mMax;
};

// Trait specialization for Cardinality with ostream.
template <class charT, class Traits>
std::basic_ostream<charT, Traits> &operator<<(
		std::basic_ostream<charT, Traits> &os,
		const Cardinality &c)
{
	// Init output stream.
	typename std::basic_ostream<charT, Traits>::sentry init(os);
	if (init)
	{
		// Output !
		if (c.IsRelative())
			os << "(";
		os << c.GetMin() << "," << c.GetMax();
		if (c.IsRelative())
			os << ")";
	}
	return os;
}

}

#endif /* _CARDINALITY_H_ */
