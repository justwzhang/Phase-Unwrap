#ifndef _BASE_PHASE_UNWRAPPER_H_
#define _BASE_PHASE_UNWRAPPER_H_

#include <iostream>
#include <fstream>
#include <string>

#ifndef PI
#define PI 3.141592653589793238462643383279
#endif

class CBasePhaseUnwrapper {

public:
	//constructor
	CBasePhaseUnwrapper(int width, int height, int numRaws) {
		m_width		 = width;
		m_height	 = height;
		m_nRaw = numRaws;

		//allocate ambient matrix
		m_pAmbient = new double[m_width * m_height];
		if (m_pAmbient == NULL)
		{
			std::cerr << "Error in memory allocation for ambient" << std::endl;
			return;
		}

		//allocate modulation matrix
		m_pModulation = new double[m_width * m_height];
		if (m_pModulation == NULL)
		{
			std::cerr<< "Error in allocating modulation buffer" << std::endl;
			return;
		}
		//allocate phase matrix
		m_pPhase = new double[m_width * m_height];
		if (m_pPhase == NULL)
		{
			std::cerr << "Error in allocating phase buffer" << std::endl;
			return;
		}

		//allocate unwrapped phase matrix
		m_pUnwrappedPhase = new double[m_width * m_height];
		if (m_pUnwrappedPhase == NULL)
		{
			std::cerr << "Error in allocating unwrapped phase buffer" << std::endl;
			return;
		}

		//allocate wrap count
		m_pPeriod = new int[m_width * m_height];
		if (m_pPeriod == NULL)
		{
			std::cerr << "Error in memory allocation in _compute_unwrapped_phase" << std::endl;
			return;
		}

	};
	//destructor
	~CBasePhaseUnwrapper() {

		if (m_pModulation != NULL) delete[]m_pModulation;
		m_pModulation = NULL;
		if (m_pPhase != NULL) delete[]m_pPhase;
		m_pPhase = NULL;
		if (m_pUnwrappedPhase != NULL) delete[]m_pUnwrappedPhase;
		m_pUnwrappedPhase = NULL;
		if (m_pAmbient != NULL) delete[]m_pAmbient;
		m_pAmbient = NULL;
		if (m_pPeriod != NULL)  delete[]m_pPeriod;
		m_pPeriod = NULL;
	};

protected:
	//raw images
	unsigned char* m_pRaw[6];
	//ambient component
	double* m_pAmbient;
	//modulation component
	double* m_pModulation;
	//relative phase component
	double* m_pPhase;
	//number of periods 
	int* m_pPeriod;
	//absolute phase component
	double* m_pUnwrappedPhase;
	//image width and height
	long m_width, m_height;
	//number of raw images
	int  m_nRaw;

public:
	//compute ambient component
	virtual void compute_ambient() {};
	//compute modulation component
	virtual void compute_modulation() {};
	//compute relative phase
	virtual void compute_phase() {};
	//compute unwrapped phase
	virtual void compute_unwrapped_phase() {};
	//write the unwrapped phase to file
	void write_unwrapped_phase_to_file(std::string filename);

public:
	//unwrapped phase
	double* unwrapped_phase() { return m_pUnwrappedPhase; };
	//wrapped phase
	double* wrapped_phase() { return m_pPhase; };
	//modulation
	double* modulation() { return m_pModulation; };
	//ambient
	double* ambient() { return m_pAmbient; };

protected:
	//Gaussian filter
	//void Gauss_filter( double* pImage);
};


#endif

