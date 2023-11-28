#include "CapturePhaseUnwrapper.h"
#include "debugger.h"

void CCapturePhaseUnwrapper::compute_phase()
{

	unsigned char* pbuf[capture_phase_image_num];
	for (int i = 0; i < capture_phase_image_num; i++)
	{
		pbuf[i] = (unsigned char*)m_pRaw[i];
	}

	double S1 = 0, S2 = sin(2 * PI / 3), S3 = sin(4 * PI / 3);
	double C1 = 1, C2 = cos(2 * PI / 3), C3 = cos(4 * PI / 3);

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			int idx = i * m_width + j;

			//LightCrafter GRB = (2/3PI, -2/3PI,0 )
			double a3 = m_pRaw[0][idx]; 
			double a1 = m_pRaw[1][idx];
			double a2 = m_pRaw[2][idx];

			double uphase;
			//insert your code here 
			uphase = atan((sqrt(3)*(a1 - a3))/(2*a2 - a1 - a3));
			//uphase = ?
			if (uphase < 0) uphase += 2 * PI;
			m_pPhase[idx] = uphase;

		}
	__visualize_and_save(m_pPhase, m_height, m_width, 2*PI, 0, m_label + "_Wrapped_Phase.png");
}

void CCapturePhaseUnwrapper::LoadImgFromMem(unsigned char* arr[])
{
	for (int i = 0; i < m_nRaw; ++i)
		m_pRaw[i] = arr[i];
}

void CCapturePhaseUnwrapper::compute_modulation()
{

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			int idx = i * m_width + j;


			double a3 = m_pRaw[0][idx];
			double a1 = m_pRaw[1][idx];
			double a2 = m_pRaw[2][idx];

			//insert your code here
			m_pModulation[idx] = (a1 + a2 + a3) / 3;
			//m_pModulation[idx] = ?;
		}
	__visualize_and_save(m_pModulation, m_height, m_width, m_label + "_Modulation.png");
}

void CCapturePhaseUnwrapper::compute_ambient()
{

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			int idx = i * m_width + j;

			double a3 = m_pRaw[0][idx];
			double a1 = m_pRaw[1][idx];
			double a2 = m_pRaw[2][idx];

			//insert your code here
			//m_pAmbient[idx] = ?;
			m_pAmbient[idx] = sqrt(3*pow(a1-a3,2) + pow(2*a2-a1-a3,2))/3;
			m_pAmbient[idx] += m_pModulation[idx];
			if (m_pAmbient[idx] > 255) {
				m_pAmbient[idx] = 255;
			}
		}

	__visualize_and_save(m_pAmbient, m_height, m_width, m_label + "_Ambient.png");
}

