/*!
*      \file Plane.h
*      \brief Three dimensional plane class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#ifndef _MESHLIB_PLANE_H_
#define _MESHLIB_PLANE_H_

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>
#include "Point.h"

namespace MeshLib {

	/*!
	*	\brief CPlane class, three dimensional plane
	*
	*	Three dimensional plane
	*/
	class CPlane {

	public:
		/*!
		 * CPlane constructor, specifying \f$(x,y,z)\f$
		 */
		CPlane(CPoint& _base, CPoint& _normal) { m_base = _base; m_normal = _normal; };
		/*!
		*	CPlane default constructor, initialized as (0,0,0), (0,0,1)
		*/
		CPlane() { m_base = CPoint(0, 0, 0); m_normal = CPoint(0, 0, 1); };
		/*!
		*	CPlane destructor
		*/
		~CPlane() {};

		/*!
		 * base point
		 * \return the base point
		 */
		CPoint& base()
		{
			return m_base;
		};


		/*!
		 * normal
		 * \return the normal direction
		 */
		CPoint& normal()
		{
			return m_normal;
		};

	protected:
		/*!
		* base point
		*/
		CPoint m_base;
		CPoint m_normal;
	public:
		void print() {
			std::cout << '(' << base()[0] << ' ' << base()[1] << ' ' << base()[2] << ')';
			std::cout << '(' << normal()[0] << ' ' << normal()[1] << ' ' << normal()[2] << ')' << std::endl;
		}
	};
}//name space MeshLib

#endif //_MESHLIB_Plane_H_ defined
