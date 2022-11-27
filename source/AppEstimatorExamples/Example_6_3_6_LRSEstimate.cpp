#include <iostream>
#include <EntEstimateLib/6_3_6_LRSEstimate.h>

int demonstrateLRSEstimate(void)
{
	entropy_estimator_lib::constants::EnmReturnStatus sRet = entropy_estimator_lib::constants::EnmReturnStatus::ErrorUnexpected;

	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_S(21);
	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_A(3);

	bz_S = 2, 2, 0, 1, 0, 2, 0, 1, 2, 1, 2, 0, 1, 2, 1, 0, 0, 1, 0, 0, 0;
	bz_A = 0, 1, 2;
	entropy_estimator_lib::data_types::t_data_for_estimator	dt;
	memset(&dt, 0, sizeof(dt));
	dt.isModeDemonstration = true;
	dt.bits_per_sample = 3;
	dt.L = bz_S.length(blitz::firstDim);
	dt.k = bz_A.length(blitz::firstDim);
	dt.p_bzInputS = &bz_S;
	dt.p_bzSampleSpaceA = &bz_A;

	dt.t_6_3_6.cutoff = 3;
	dt.t_6_3_6.u = 1;
	dt.t_6_3_6.nu = 1;
	sRet = entropy_estimator_lib::estimators::lrs::estimate(dt);

	std::cout << "u:\t" << dt.t_6_3_6.u << std::endl;
	std::cout << "nu:\t" << dt.t_6_3_6.nu << std::endl;
	std::cout << "\\hat{p}:\t" << dt.t_6_3_6.p_hat << std::endl;
	std::cout << "p_u:" << dt.t_6_3_6.p_u << std::endl;
	std::cout << "min-entropy:" << dt.t_6_3_6.min_entropy << std::endl;

	return 0;
}