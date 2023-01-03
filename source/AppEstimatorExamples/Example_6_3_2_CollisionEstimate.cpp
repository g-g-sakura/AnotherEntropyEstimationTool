#include <iostream>
#include <EntEstimateLib/6_3_2_CollisionEstimate.h>

int demonstrateCollisionEstimate(void)
{
	entropy_estimator_lib::constants::EnmReturnStatus sRet = entropy_estimator_lib::constants::EnmReturnStatus::ErrorUnexpected;

	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_S(40);
	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_A(2);

	bz_S = 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0;
	bz_A = 0, 1;
	entropy_estimator_lib::data_types::t_data_for_estimator	dt;
	memset(&dt, 0, sizeof(dt));
	dt.isModeDemonstration = true;
	dt.L = bz_S.length(blitz::firstDim);
	dt.k = bz_A.length(blitz::firstDim);
	dt.p_bzInputS = &bz_S;
	dt.p_bzSampleSpaceA = &bz_A;
	sRet = entropy_estimator_lib::estimators::collision::estimate(dt);

	std::cout << "p:" << dt.t_6_3_2.p << std::endl;
	std::cout << "min-entropy:" << dt.t_6_3_2.t_common.min_entropy << std::endl;

	return 0;
}