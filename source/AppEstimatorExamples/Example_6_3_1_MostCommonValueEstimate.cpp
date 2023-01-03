#include <iostream>
#include <EntEstimateLib/6_3_1_MostCommonValueEstimate.h>

int demonstrateMCVEstimate(void)
{
	entropy_estimator_lib::constants::EnmReturnStatus sRet = entropy_estimator_lib::constants::EnmReturnStatus::ErrorUnexpected;

	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_S(20);
	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_A(3);

	bz_S = 0, 1, 1, 2, 0, 1, 2, 2, 0, 1, 0, 1, 1, 0, 2, 2, 1, 0, 2, 1;
	bz_A = 0, 1, 2;
	entropy_estimator_lib::data_types::t_data_for_estimator	dt;
	memset(&dt, 0, sizeof(dt));
	dt.isModeDemonstration = true;
	dt.L = bz_S.length(blitz::firstDim);
	dt.k = bz_A.length(blitz::firstDim);
	dt.p_bzInputS = &bz_S;
	dt.p_bzSampleSpaceA = &bz_A;
	sRet = entropy_estimator_lib::estimators::mcv::estimate(dt);

	std::cout << "p_u:" << dt.t_6_3_1.p_u << std::endl;
	std::cout << "min-entropy:" << dt.t_6_3_1.t_common.min_entropy << std::endl;

	return 0;
}