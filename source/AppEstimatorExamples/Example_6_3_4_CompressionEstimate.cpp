#include <iostream>
#include <EntEstimateLib/6_3_4_CompressionEstimate.h>

int demonstrateCompressionEstimate(void)
{
	entropy_estimator_lib::constants::EnmReturnStatus sRet = entropy_estimator_lib::constants::EnmReturnStatus::ErrorUnexpected;

	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_S(48);
	blitz::Array<entropy_estimator_lib::data_types::octet, 1>	bz_A(2);

	bz_S = 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1;
	bz_A = 0, 1;
	entropy_estimator_lib::data_types::t_data_for_estimator	dt;
	memset(&dt, 0, sizeof(dt));
	dt.isModeDemonstration = true;
	dt.L = bz_S.length(blitz::firstDim);
	dt.k = bz_A.length(blitz::firstDim);
	dt.p_bzInputS = &bz_S;
	dt.p_bzSampleSpaceA = &bz_A;

	dt.t_6_3_4.b = 6;
	dt.t_6_3_4.d = 4;
	sRet = entropy_estimator_lib::estimators::compression::estimate(dt);

	std::cout << "\t\\bar{X}\t\t:\t" << dt.t_6_3_4.x_bar << std::endl;
	std::cout << "\t\\hat{sigma}\t:\t" << dt.t_6_3_4.sigma_hat << std::endl;
	std::cout << "\t\\bar{X}'\t:\t" << dt.t_6_3_4.x_bar_prime << std::endl;
	std::cout << "\tp\t\t:\t" << dt.t_6_3_4.p << std::endl;
	std::cout << "\tmin-entropy\t:\t" << dt.t_6_3_4.t_common.min_entropy << std::endl;

	return 0;
}