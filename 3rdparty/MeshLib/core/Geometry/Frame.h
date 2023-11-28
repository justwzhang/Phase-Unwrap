/*!
*      \file Frame.h
*      \brief Three dimensional Frame class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#ifndef _MESHLIB_FRAME_H_
#define _MESHLIB_FRAME_H_

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include "Point.h"

namespace MeshLib {

	/*!
	*	\brief CFrame class, three dimensional Frame
	*
	*	Three dimensional frame
	*/
	class CFrame {

	public:
		/*!
		 * CFrame constructor, specifying \f$(x,y,z)\f$
		 */
		CFrame(CPoint& _origin, CPoint& _e0, CPoint& _e1, CPoint& _e2) { 
			m_origin = _origin; 
			m_e[0] = _e0; 
			m_e[1] = _e1;
			m_e[2] = _e2;
		};
		/*!
		*	CFrame default constructor, initialized as (0,0,0), (1,0,0),(0,1,0),(0,0,1)
		*/
		CFrame() { m_origin = CPoint(0, 0, 0); 
			m_e[0] = CPoint(1, 0, 0);
			m_e[1] = CPoint(0, 1, 0);
			m_e[2] = CPoint(0, 0, 1);
		};
		/*!
		*	CFrame destructor
		*/
		~CFrame() {};

		/*!
		*	set the frame to be the Identity
		*/
		void Identity()
		{
			m_origin = CPoint(0, 0, 0);
			m_e[0] = CPoint(1, 0, 0);
			m_e[1] = CPoint(0, 1, 0);
			m_e[2] = CPoint(0, 0, 1);
		}
		/*!
		 * base point
		 * \return the base point
		 */
		CPoint& origin()
		{
			return m_origin;
		};
		/*!
		 * direction
		 * \return the direction
		 */
		CPoint& e(int k) { assert(k >= 0 && k < 3); return m_e[k]; };
		/*
		*	coordinate transform
		*/
		CPoint operator*(CPoint v) 
		{
			CPoint w = v - m_origin;
			return CPoint(w * m_e[0], w * m_e[1], w * m_e[2]);
		};

		void write_to_file(std::string filename)
		{
			std::ofstream wf(filename, std::ios::out | std::ios::binary);
			if (!wf) {
				std::cout << "Cannot open file!" << std::endl;
				return;
			}

			double _buf[12];
			for(int k = 0; k < 3; k ++ )
				_buf[k] = m_origin[k];
			for (int i = 0; i < 3; i++)
				for (int k = 0; k < 3; k++)
					_buf[(i + 1) * 3 + k] = m_e[i][k];
			wf.write((char*)_buf, sizeof(double) * 4);
			wf.close();
			if (!wf.good()) {
				std::cout << "Error occurred at writing time!" << std::endl;
				return;
			}
		}

		void read_from_file(std::string filename)
		{
			std::ifstream rf(filename, std::ios::in | std::ios::binary);
			if (!rf) {
				std::cout << "Cannot open file!" << std::endl;
				return;
			}

			double _buf[12];
			rf.read((char*)_buf, sizeof(double) * 4);
			for (int k = 0; k < 3; k++) {
				m_origin[k] = _buf[k];
			}
			for (int i = 0; i < 3; i++)
				for (int k = 0; k < 3; k++)
					m_e[i][k] = _buf[(i + 1) * 3 + k];
			rf.close();
			if (!rf.good()) {
				std::cout << "Error occurred at reading time!" << std::endl;
				return;
			}
		}

		void print() {
			std::cout << "Frame" << std::endl;
			std::cout << '('<< m_origin[0] << ' ' << m_origin[1] << ' ' << m_origin[2] << ')' << std::endl;
			for (int i = 0; i < 3; i++) {
				std::cout << '(' << m_e[i][0] << ' ' << m_e[i][1] << ' ' << m_e[i][2] << ')' << std::endl;
			}
			std::cout << std::endl;
		}

	protected:
		/*!
		* base point
		*/
		CPoint m_origin;
		CPoint m_e[3];
	};

	/*!
	 *	Read a CPoint from a string
	 * \param str string
	 * \param p three dimenionsal point
	 * \return the (x,y,z) value is read from str and assigned to p.
	 */
	 /*
	 inline void operator>>(const std::string& str, CPoint& p)
	 {
		 std::string t = str;
		 t.erase(0, t.find_first_not_of("()"));
		 t.erase(t.find_last_not_of("()") + 1);
		 std::istringstream iss(t);

		 iss >> p[0] >> p[1] >> p[2];
	 }
	 */

}//name space MeshLib

#endif //_MESHLIB_LINE_H_ defined

