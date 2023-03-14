/*
 * \file
 * \brief Wrapper graphviz.
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
 * Wrapper pour la lib graphviz.
 *
 * $Id: Graphviz.h 84 2008-07-25 22:12:56Z bombela $
 */

#ifndef _GRAPHVIZ_H_
#define _GRAPHVIZ_H_

#include <string>
#include <list>
#include <gvc.h>

namespace MeriseAcide
{

enum ETableStyle { TSEntity, TSAssoc, TSTable };
enum ERender { RNEATO, RFDP, RDOT, RTWOPI, RCIRCO };

/**
 * This class wrap an graph creation with Graphiz.
 * When this class is created, you can make ONE
 * graph and render this to multiple format.
 * This only usable for make digraph.
 * All code of this class are oriented to
 * make MPD and MCD graphical view.
 */
class Graphviz
{
	public:
		Graphviz(void);
		~Graphviz(void);

		void Reset(ERender r = RNEATO);

		const Agnode_t* AddTable(const std::string& n,
				ETableStyle s = TSEntity);

		void  AddLink(
				const Agnode_t* Start,
				const Agnode_t* Dest,
				const std::string& l); // Tail Label
		void  AddLink(
				const Agnode_t* Start,
				const Agnode_t* Dest,
				const std::string& tp, // Tail Port
				const std::string& hp);// Head Port

		void AddKey(const Agnode_t* n, const std::string& k);
		void AddKey(const Agnode_t* n, const std::string& k,
				const std::string& id);
		void AddField(const Agnode_t* n, const std::string& f);
		void AddField(const Agnode_t* n, const std::string& f,
				const std::string& id);
		
		bool Compute(void);
		void ResetCompute(void);
		bool Render(const std::string& fmt, const std::string& filename);

		void SetKValue(double v)   { mKVal = v; }
		void SetMaxIterate(int v)  { mMaxIter = v; }
		void SetUseColor(bool v)   { mUseColor = v; }
		void SetEdgeLen(float v)   { mEdgeLen = v; }
		void SetColor(ETableStyle s, const std::string& c);

		const std::string& GetErrorMsg(void) const { return mErrorMsg; }

	private:
		std::string mErrorMsg;
		GVC_t*      mGVC;
		Agraph_t*   mGraph;
		ERender     mRender;
		int         mMaxIter;
		float       mKVal;
		bool        mComputed;
		bool        mUseColor;
		int         mCurIdxColor;
		float       mEdgeLen;
		std::string mColorEntity;
		std::string mColorAssoc;
		std::string mColorTable;
		
		void Reset_Make(ERender r);
		void Reset_Free(void);
		void SetGraphDefaultValue(const std::string& vn, float v);
		void SetGraphDefaultValue(const std::string& vn, const std::string& v);
		void SetNodeDefaultValue(const std::string& vn, float v);
		void SetNodeDefaultValue(const std::string& vn, const std::string& v);
		void SetEdgeDefaultValue(const std::string& vn, float v);
		void SetEdgeDefaultValue(const std::string& vn, const std::string& v);
		const char* NextColor(void);
};

}

#endif /* _GRAPHVIZ_H_ */
