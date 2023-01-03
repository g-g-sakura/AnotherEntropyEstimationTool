#include <iostream>
#include <EntEstimateLib/6_3_8_LagPredictionEstimate.h>

int demonstrateLagPredictionEstimate(void)
{
	entropy_estimator_lib::constants::EnmReturnStatus sRet = entropy_estimator_lib::constants::EnmReturnStatus::ErrorUnexpected;

	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_S(10);
	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_A(3);

	bz_S = 2, 1, 3, 2, 1, 3, 1, 3, 1, 2;
	bz_A = 1, 2, 3;
	entropy_estimator_lib::data_types::t_data_for_estimator	dt;
	memset(&dt, 0, sizeof(dt));
	dt.isModeDemonstration = true;
	dt.L = bz_S.length(blitz::firstDim);
	dt.k = bz_A.length(blitz::firstDim);
	dt.p_bzInputS = &bz_S;
	dt.p_bzSampleSpaceA = &bz_A;

	dt.t_6_3_8.D = 3;
	sRet = entropy_estimator_lib::estimators::lag_prediction::estimate(dt);

	std::cout << "p_global:" << dt.t_6_3_8.p_global << std::endl;
	std::cout << "p_prime_global:" << dt.t_6_3_8.p_prime_global << std::endl;
	std::cout << "r:\t\t" << dt.t_6_3_8.r << std::endl;
	std::cout << "p_local:" << dt.t_6_3_8.p_local << std::endl;
	std::cout << "min-entropy:" << dt.t_6_3_8.t_common.min_entropy << std::endl;

	return 0;
}