#include <iostream>
#include <EntEstimateLib/6_3_7_MMCWPredictionEstimate.h>

int demonstrateMMCWPredictionEstimate(void)
{
	entropy_estimator_lib::constants::EnmReturnStatus sRet = entropy_estimator_lib::constants::EnmReturnStatus::ErrorUnexpected;

	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_S(12);
	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_A(3);

	bz_S = 1, 2, 1, 0, 2, 1, 1, 2, 2, 0, 0, 0;
	bz_A = 0, 1, 2;
	entropy_estimator_lib::data_types::t_data_for_estimator	dt;
	memset(&dt, 0, sizeof(dt));
	dt.isModeDemonstration = true;
	dt.L = bz_S.length(blitz::firstDim);
	dt.k = bz_A.length(blitz::firstDim);
	dt.p_bzInputS = &bz_S;
	dt.p_bzSampleSpaceA = &bz_A;

	dt.t_6_3_7.w[0] = 3;
	dt.t_6_3_7.w[1] = 5;
	dt.t_6_3_7.w[2] = 7;
	dt.t_6_3_7.w[3] = 9;
	sRet = entropy_estimator_lib::estimators::mmcw_prediction::estimate(dt);

	std::cout << "p_global:" << dt.t_6_3_7.p_global << std::endl;
	std::cout << "p_prime_global:" << dt.t_6_3_7.p_prime_global << std::endl;
	std::cout << "r:\t\t" << dt.t_6_3_7.r << std::endl;
	std::cout << "p_local:" << dt.t_6_3_7.p_local << std::endl;
	std::cout << "min-entropy:" << dt.t_6_3_7.t_common.min_entropy << std::endl;

	return (int)sRet;
}