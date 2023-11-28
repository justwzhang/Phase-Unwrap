/*!
*      \file Line.h
*      \brief Three dimensional point class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#ifndef _MESHLIB_LINE_H_
#define _MESHLIB_LINE_H_

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>
#include "Point.h"

namespace MeshLib {

	/*!
	*	\brief CLine class, three dimensional line
	*
	*	Three dimensional line
	*/
	class CLine {

	public:
		/*!
		 * CPoint constructor, specifying \f$(x,y,z)\f$
		 */
		CLine(CPoint& _base, CPoint& _direction) { m_base = _base; m_direction = _direction; };
		/*!
		*	CLine default constructor, initialized as (0,0,0), (0,0,1)
		*/
		CLine() { m_base = CPoint(0, 0, 0); m_direction = CPoint(0, 0, 1); };
		/*!
		*	CLine destructor
		*/
		~CLine() {};

		/*!
		 * base point
		 * \return the base point
		 */
		CPoint& base() 
		{
			return m_base;
		};


		/*!
		 * direction
		 * \return the direction
		 */
		CPoint& direction()
		{
			return m_direction;
		};

	protected:
		/*!
		* base point
		*/
		CPoint m_base;
		CPoint m_direction;
	public:
		void print() {
			std::cout << '(' << base()[0] << ' ' << base()[1] << ' ' << base()[2] << ')';
			std::cout << '(' << direction()[0] << ' ' << direction()[1] << ' ' << direction()[2] << ')' << std::endl;
		}
	};
}//name space MeshLib
#endif //_MESHLIB_LINE_H_ defined
