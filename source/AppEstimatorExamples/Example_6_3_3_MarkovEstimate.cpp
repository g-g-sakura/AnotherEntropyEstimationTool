#include <iostream>
#include <EntEstimateLib/6_3_3_MarkovEstimate.h>

int demonstrateMarkovEstimate(void)
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
	sRet = entropy_estimator_lib::estimators::markov::estimate(dt);

	blitz::Array<double, 1>	bz_initial(dt.t_6_3_3.p_initial, blitz::shape(2), blitz::duplicateData);
	blitz::Array<double, 2>	bz_transition(dt.t_6_3_3.p_transition[0], blitz::shape(2, 2), blitz::duplicateData);
	blitz::Array<double, 1> bz_negativeLogPoverLength(dt.t_6_3_3.negativeLogPoverLength, blitz::shape(14), blitz::duplicateData);

	std::cout << "min-entropy:" << dt.t_6_3_3.min_entropy << std::endl;
	std::cout << "most likely sequence index:" << dt.t_6_3_3.mostLikelySequenceCategoryIndex << std::endl;


	std::cout << "estimated intial probabilities:" << bz_initial << std::endl;
	std::cout << "transition matrix:" << bz_transition << std::endl;
	std::cout << "- log_2(probability) / length:" << bz_negativeLogPoverLength << std::endl;

	return 0;
}