#include "catch.hpp"
#include "ecoevolity/ecoevolity.hpp"

#include "ecoevolity/rng.hpp"
#include "ecoevolity/path.hpp"
#include "ecoevolity/spreadsheet.hpp"

RandomNumberGenerator _ECOEVOLITY_CLI_RNG = RandomNumberGenerator();

TEST_CASE("Testing constant sites error", "[EcoevolityCLI]") {

    SECTION("Testing constant sites error") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double u1_shape = 2.0;
        double u1_scale = 1.0;
        double u1_offset = 0.5;
        double u2_shape = 1.0;
        double u2_scale = 0.5;
        double u2_offset = 0.5;
        double u3_shape = 1.0;
        double u3_scale = 2.0;
        double u3_offset = 0.5;
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _ECOEVOLITY_CLI_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + ".cfg";
        std::string log_path = "data/tmp-config-" + tag + "-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    dirichlet_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    gamma_distribution:\n";
        os << "                        shape: " << concentration_shape << "\n";
        os << "                        scale: " << concentration_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 10\n";
        os << "    sample_frequency: 1\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightMultiplierScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ChildPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \"_\"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    use_empirical_mutation_rate_starting_values: false\n";
        os << "    constrain_population_sizes: false\n";
        os << "    constrain_mutation_rates: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-with-constant.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u1_shape << "\n";
        os << "                    scale: " << u1_scale << "\n";
        os << "                    offset: " << u1_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u2_shape << "\n";
        os << "                    scale: " << u2_scale << "\n";
        os << "                    offset: " << u2_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult2_shape << "\n";
        os << "                    scale: " << mult2_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size3_shape << "\n";
        os << "                    scale: " << size3_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u3_shape << "\n";
        os << "                    scale: " << u3_scale << "\n";
        os << "                    offset: " << u3_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "283402";
        char arg3[] = "--ignore-data";
        char * cfg_path = new char[test_path.size() + 1];
        std::copy(test_path.begin(), test_path.end(), cfg_path);
        cfg_path[test_path.size()] = '\0';
        char * argv[] = {
            &arg0[0],
            &arg1[0],
            &arg2[0],
            &arg3[0],
            cfg_path,
            NULL
        };
        int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        int ret;

        REQUIRE_THROWS_AS(ecoevolity_main(argc, argv), EcoevolityConstantSitesError);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing relaxed constant sites setting", "[EcoevolityCLI]") {

    SECTION("Testing constant sites relaxed") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double u1_shape = 2.0;
        double u1_scale = 1.0;
        double u1_offset = 0.5;
        double u2_shape = 1.0;
        double u2_scale = 0.5;
        double u2_offset = 0.5;
        double u3_shape = 1.0;
        double u3_scale = 2.0;
        double u3_offset = 0.5;
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _ECOEVOLITY_CLI_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + ".cfg";
        std::string log_path = "data/tmp-config-" + tag + "-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    dirichlet_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    gamma_distribution:\n";
        os << "                        shape: " << concentration_shape << "\n";
        os << "                        scale: " << concentration_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 10\n";
        os << "    sample_frequency: 1\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightMultiplierScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ChildPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \"_\"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    use_empirical_mutation_rate_starting_values: false\n";
        os << "    constrain_population_sizes: false\n";
        os << "    constrain_mutation_rates: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-with-constant.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u1_shape << "\n";
        os << "                    scale: " << u1_scale << "\n";
        os << "                    offset: " << u1_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u2_shape << "\n";
        os << "                    scale: " << u2_scale << "\n";
        os << "                    offset: " << u2_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult2_shape << "\n";
        os << "                    scale: " << mult2_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size3_shape << "\n";
        os << "                    scale: " << size3_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u3_shape << "\n";
        os << "                    scale: " << u3_scale << "\n";
        os << "                    offset: " << u3_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "283402";
        char arg3[] = "--ignore-data";
        char arg4[] = "--relax-constant-sites";
        char * cfg_path = new char[test_path.size() + 1];
        std::copy(test_path.begin(), test_path.end(), cfg_path);
        cfg_path[test_path.size()] = '\0';
        char * argv[] = {
            &arg0[0],
            &arg1[0],
            &arg2[0],
            &arg3[0],
            &arg4[0],
            cfg_path,
            NULL
        };
        int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        int ret;

        ret = ecoevolity_main(argc, argv);
        REQUIRE(ret == 0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing missing sites error", "[EcoevolityCLI]") {

    SECTION("Testing missing sites error") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double u1_shape = 2.0;
        double u1_scale = 1.0;
        double u1_offset = 0.5;
        double u2_shape = 1.0;
        double u2_scale = 0.5;
        double u2_offset = 0.5;
        double u3_shape = 1.0;
        double u3_scale = 2.0;
        double u3_offset = 0.5;
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _ECOEVOLITY_CLI_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + ".cfg";
        std::string log_path = "data/tmp-config-" + tag + "-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    dirichlet_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    gamma_distribution:\n";
        os << "                        shape: " << concentration_shape << "\n";
        os << "                        scale: " << concentration_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 10\n";
        os << "    sample_frequency: 1\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightMultiplierScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ChildPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \"_\"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    use_empirical_mutation_rate_starting_values: false\n";
        os << "    constrain_population_sizes: false\n";
        os << "    constrain_mutation_rates: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-with-missing.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u1_shape << "\n";
        os << "                    scale: " << u1_scale << "\n";
        os << "                    offset: " << u1_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u2_shape << "\n";
        os << "                    scale: " << u2_scale << "\n";
        os << "                    offset: " << u2_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult2_shape << "\n";
        os << "                    scale: " << mult2_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size3_shape << "\n";
        os << "                    scale: " << size3_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u3_shape << "\n";
        os << "                    scale: " << u3_scale << "\n";
        os << "                    offset: " << u3_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "283402";
        char arg3[] = "--ignore-data";
        char * cfg_path = new char[test_path.size() + 1];
        std::copy(test_path.begin(), test_path.end(), cfg_path);
        cfg_path[test_path.size()] = '\0';
        char * argv[] = {
            &arg0[0],
            &arg1[0],
            &arg2[0],
            &arg3[0],
            cfg_path,
            NULL
        };
        int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        int ret;

        REQUIRE_THROWS_AS(ecoevolity_main(argc, argv), EcoevolityMissingDataError);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing relaxed missing sites setting", "[EcoevolityCLI]") {

    SECTION("Testing missing sites relaxed") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double u1_shape = 2.0;
        double u1_scale = 1.0;
        double u1_offset = 0.5;
        double u2_shape = 1.0;
        double u2_scale = 0.5;
        double u2_offset = 0.5;
        double u3_shape = 1.0;
        double u3_scale = 2.0;
        double u3_offset = 0.5;
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _ECOEVOLITY_CLI_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + ".cfg";
        std::string log_path = "data/tmp-config-" + tag + "-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    dirichlet_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    gamma_distribution:\n";
        os << "                        shape: " << concentration_shape << "\n";
        os << "                        scale: " << concentration_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 10\n";
        os << "    sample_frequency: 1\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ComparisonHeightMultiplierScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        ChildPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \"_\"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    use_empirical_mutation_rate_starting_values: false\n";
        os << "    constrain_population_sizes: false\n";
        os << "    constrain_mutation_rates: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-with-missing.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u1_shape << "\n";
        os << "                    scale: " << u1_scale << "\n";
        os << "                    offset: " << u1_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u2_shape << "\n";
        os << "                    scale: " << u2_scale << "\n";
        os << "                    offset: " << u2_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult2_shape << "\n";
        os << "                    scale: " << mult2_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size3_shape << "\n";
        os << "                    scale: " << size3_scale << "\n";
        os << "        u_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << u3_shape << "\n";
        os << "                    scale: " << u3_scale << "\n";
        os << "                    offset: " << u3_offset << "\n";
        os << "        time_multiplier:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "283402";
        char arg3[] = "--ignore-data";
        char arg4[] = "--relax-missing-sites";
        char * cfg_path = new char[test_path.size() + 1];
        std::copy(test_path.begin(), test_path.end(), cfg_path);
        cfg_path[test_path.size()] = '\0';
        char * argv[] = {
            &arg0[0],
            &arg1[0],
            &arg2[0],
            &arg3[0],
            &arg4[0],
            cfg_path,
            NULL
        };
        int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        int ret;

        ret = ecoevolity_main(argc, argv);
        REQUIRE(ret == 0);

        delete[] cfg_path;
    }
}
