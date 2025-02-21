#include "catch.hpp"
#include "ecoevolity/ecoevolity.hpp"

#include "ecoevolity/rng.hpp"
#include "ecoevolity/path.hpp"
#include "ecoevolity/spreadsheet.hpp"

RandomNumberGenerator _PRIOR_SAMPLING_RNG = RandomNumberGenerator();

TEST_CASE("Testing sampling from prior with TimeSizeRateMixer with 6 pairs", "[SamplingPrior]") {

    SECTION("Testing 6 pairs with optimizing") {
        double height_shape = 5.0;
        double height_scale = 0.1;
        double size1_shape = 10.0;
        double size1_scale = 0.1;
        double size2_shape = 2.0;
        double size2_scale = 0.2;
        double size3_shape = 5.0;
        double size3_scale = 0.2;
        double size4_shape = 4.0;
        double size4_scale = 0.5;
        double size5_shape = 8.0;
        double size5_scale = 0.2;
        double size6_shape = 6.0;
        double size6_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-collection-scaler-test1-" + tag + "-t450.cfg";
        std::string log_path = "data/tmp-config-collection-scaler-test1-" + tag + "-t450-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 2000000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size3_shape << "\n";
        os << "                    scale: " << size3_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname3.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size4_shape << "\n";
        os << "                    scale: " << size4_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname4.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size5_shape << "\n";
        os << "                    scale: " << size5_scale << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname5.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size6_shape << "\n";
        os << "                    scale: " << size6_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "12345";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 200001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        SampleSummarizer<double> height_summary4 = prior_sample.summarize<double>("root_height_pop1c");
        SampleSummarizer<double> height_summary5 = prior_sample.summarize<double>("root_height_pop1d");
        SampleSummarizer<double> height_summary6 = prior_sample.summarize<double>("root_height_pop1e");

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary4.sample_size() == expected_sample_size);
        REQUIRE(height_summary4.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary4.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary5.sample_size() == expected_sample_size);
        REQUIRE(height_summary5.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary5.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary6.sample_size() == expected_sample_size);
        REQUIRE(height_summary6.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary6.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");
        SampleSummarizer<double> size4_summary_a = prior_sample.summarize<double>("pop_size_pop1c");
        SampleSummarizer<double> size4_summary_b = prior_sample.summarize<double>("pop_size_pop2c");
        SampleSummarizer<double> size4_summary_c = prior_sample.summarize<double>("pop_size_root_pop1c");
        SampleSummarizer<double> size5_summary_a = prior_sample.summarize<double>("pop_size_pop1d");
        SampleSummarizer<double> size5_summary_b = prior_sample.summarize<double>("pop_size_pop2d");
        SampleSummarizer<double> size5_summary_c = prior_sample.summarize<double>("pop_size_root_pop1d");
        SampleSummarizer<double> size6_summary_a = prior_sample.summarize<double>("pop_size_pop1e");
        SampleSummarizer<double> size6_summary_b = prior_sample.summarize<double>("pop_size_pop2e");
        SampleSummarizer<double> size6_summary_c = prior_sample.summarize<double>("pop_size_root_pop1e");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.02));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.02));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.02));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.02));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.02));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.02));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.02));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.02));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.02));

        REQUIRE(size4_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size4_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size4_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size4_summary_a.mean() == Approx(size4_shape * size4_scale).epsilon(0.01));
        REQUIRE(size4_summary_a.variance() == Approx(size4_shape * size4_scale * size4_scale).epsilon(0.02));
        REQUIRE(size4_summary_b.mean() == Approx(size4_shape * size4_scale).epsilon(0.01));
        REQUIRE(size4_summary_b.variance() == Approx(size4_shape * size4_scale * size4_scale).epsilon(0.02));
        REQUIRE(size4_summary_c.mean() == Approx(size4_shape * size4_scale).epsilon(0.01));
        REQUIRE(size4_summary_c.variance() == Approx(size4_shape * size4_scale * size4_scale).epsilon(0.02));

        REQUIRE(size5_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size5_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size5_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size5_summary_a.mean() == Approx(size5_shape * size5_scale).epsilon(0.01));
        REQUIRE(size5_summary_a.variance() == Approx(size5_shape * size5_scale * size5_scale).epsilon(0.02));
        REQUIRE(size5_summary_b.mean() == Approx(size5_shape * size5_scale).epsilon(0.01));
        REQUIRE(size5_summary_b.variance() == Approx(size5_shape * size5_scale * size5_scale).epsilon(0.02));
        REQUIRE(size5_summary_c.mean() == Approx(size5_shape * size5_scale).epsilon(0.01));
        REQUIRE(size5_summary_c.variance() == Approx(size5_shape * size5_scale * size5_scale).epsilon(0.02));

        REQUIRE(size6_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size6_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size6_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size6_summary_a.mean() == Approx(size6_shape * size6_scale).epsilon(0.01));
        REQUIRE(size6_summary_a.variance() == Approx(size6_shape * size6_scale * size6_scale).epsilon(0.02));
        REQUIRE(size6_summary_b.mean() == Approx(size6_shape * size6_scale).epsilon(0.01));
        REQUIRE(size6_summary_b.variance() == Approx(size6_shape * size6_scale * size6_scale).epsilon(0.02));
        REQUIRE(size6_summary_c.mean() == Approx(size6_shape * size6_scale).epsilon(0.01));
        REQUIRE(size6_summary_c.variance() == Approx(size6_shape * size6_scale * size6_scale).epsilon(0.02));

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with TimeSizeRateMixer no opt", "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) and gamma(5.0, 0.5) prior and no optimizing") {
        double time_shape = 10.0;
        double time_scale = 0.1;
        double size_shape = 5.0;
        double size_scale = 0.2;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-collection-scaler-test1-" + tag + "-t451.cfg";
        std::string log_path = "data/tmp-config-collection-scaler-test1-" + tag + "-t451-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << time_shape << "\n";
        os << "        scale: " << time_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 1000000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "654154";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 100001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(time_shape * time_scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(time_shape * time_scale * time_scale).epsilon(0.01));

        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");

        REQUIRE(size_summary1.sample_size() == 100001);
        REQUIRE(size_summary2.sample_size() == 100001);
        REQUIRE(size_summary3.sample_size() == 100001);

        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));


        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "pop_size_kya",
                "pop_size_fas",
                "pop_size_root_kya",
                "ln_prior",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with TimeSizeRateMixer with opt", "[SamplingPrior]") {
    SECTION("Testing gamma(10.0, 0.1) and gamma(5.0, 0.5) prior with optimizing") {
        double time_shape = 10.0;
        double time_scale = 0.1;
        double size_shape = 5.0;
        double size_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-collection-scaler-test2-" + tag + "-t452.cfg";
        std::string log_path = "data/tmp-config-collection-scaler-test2-" + tag + "-t452-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << time_shape << "\n";
        os << "        scale: " << time_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "123456";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 50001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(time_shape * time_scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(time_shape * time_scale * time_scale).epsilon(0.01));

        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");

        REQUIRE(size_summary1.sample_size() == 50001);
        REQUIRE(size_summary2.sample_size() == 50001);
        REQUIRE(size_summary3.sample_size() == 50001);

        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));


        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "pop_size_kya",
                "pop_size_fas",
                "pop_size_root_kya",
                "ln_prior",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with TimeSizeRateMixer for singleton with opt", "[SamplingPrior]") {
    SECTION("Testing gamma(10.0, 0.1) and gamma(5.0, 0.5) prior for singleton with optimizing") {
        double time_shape = 10.0;
        double time_scale = 0.1;
        double size_shape = 5.0;
        double size_scale = 0.5;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-collection-scaler-test2-" + tag + "-t453.cfg";
        std::string log_path = "data/tmp-config-collection-scaler-test2-" + tag + "-t453-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << time_shape << "\n";
        os << "        scale: " << time_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "934028";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 50001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(time_shape * time_scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(time_shape * time_scale * time_scale).epsilon(0.01));

        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_fas"));
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");

        REQUIRE(size_summary1.sample_size() == 50001);
        REQUIRE(size_summary3.sample_size() == 50001);

        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));


        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "pop_size_kya",
                "pop_size_root_kya",
                "ln_prior",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with TimeSizeRateMixer with constrained pop sizes and opt", "[SamplingPrior]") {
    SECTION("Testing gamma(10.0, 0.1) and gamma(5.0, 0.5) prior with constrained pop sizes and optimizing") {
        double time_shape = 10.0;
        double time_scale = 0.1;
        double size_shape = 5.0;
        double size_scale = 0.5;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-collection-scaler-test3-" + tag + "-t454.cfg";
        std::string log_path = "data/tmp-config-collection-scaler-test3-" + tag + "-t454-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << time_shape << "\n";
        os << "        scale: " << time_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "123456";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 50001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(time_shape * time_scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(time_shape * time_scale * time_scale).epsilon(0.01));

        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");

        std::vector<double> samples1 = prior_sample.get<double>("pop_size_kya");
        std::vector<double> samples2 = prior_sample.get<double>("pop_size_fas");
        std::vector<double> samples3 = prior_sample.get<double>("pop_size_root_kya");

        for (size_t i = 0; i < samples1.size(); ++i) {
            REQUIRE(samples1.at(i) == samples2.at(i));
            REQUIRE(samples1.at(i) == samples3.at(i));
        }

        REQUIRE(size_summary1.sample_size() == 50001);
        REQUIRE(size_summary2.sample_size() == 50001);
        REQUIRE(size_summary3.sample_size() == 50001);

        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));


        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "pop_size_kya",
                "pop_size_fas",
                "pop_size_root_kya",
                "ln_prior",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with TimeSizeRateMixer with fixed pop sizes and 2 pairs", "[SamplingPrior]") {
    SECTION("Testing gamma(10.0, 0.1) and gamma(5.0, 0.5) prior with fixed pop sizes, optimizing, 2 pairs") {
        double time_shape = 10.0;
        double time_scale = 0.1;
        double size_shape = 5.0;
        double size_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-collection-scaler-test3-" + tag + "-t455.cfg";
        std::string log_path = "data/tmp-config-collection-scaler-test3-" + tag + "-t455-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_model_prior:\n";
        os << "    fixed: [0, 1]\n";
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << time_shape << "\n";
        os << "        scale: " << time_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "123456";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 50001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(time_shape * time_scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(time_shape * time_scale * time_scale).epsilon(0.01));

        SampleSummarizer<double> summary2 = prior_sample.summarize<double>("root_height_pop1");
        REQUIRE(summary2.mean() == Approx(time_shape * time_scale).epsilon(0.01));
        REQUIRE(summary2.variance() == Approx(time_shape * time_scale * time_scale).epsilon(0.01));

        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size_summary21 = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size_summary22 = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size_summary23 = prior_sample.summarize<double>("pop_size_root_pop1");

        std::vector<double> samples1 = prior_sample.get<double>("pop_size_kya");
        std::vector<double> samples2 = prior_sample.get<double>("pop_size_fas");
        std::vector<double> samples3 = prior_sample.get<double>("pop_size_root_kya");

        std::vector<double> samples21 = prior_sample.get<double>("pop_size_pop1");
        std::vector<double> samples22 = prior_sample.get<double>("pop_size_pop2");
        std::vector<double> samples23 = prior_sample.get<double>("pop_size_root_pop1");

        for (size_t i = 0; i < samples1.size(); ++i) {
            REQUIRE(samples1.at(i) == samples2.at(i));
            REQUIRE(samples1.at(i) == samples3.at(i));
            if (i > 10) {
                REQUIRE(samples21.at(i) != samples22.at(i));
                REQUIRE(samples21.at(i) != samples23.at(i));
                REQUIRE(samples22.at(i) != samples23.at(i));
            }
        }

        REQUIRE(size_summary1.sample_size() == 50001);
        REQUIRE(size_summary2.sample_size() == 50001);
        REQUIRE(size_summary3.sample_size() == 50001);
        REQUIRE(size_summary21.sample_size() == 50001);
        REQUIRE(size_summary22.sample_size() == 50001);
        REQUIRE(size_summary23.sample_size() == 50001);

        REQUIRE(size_summary1.mean() == Approx(0.005));
        REQUIRE(size_summary1.variance() == Approx(0.0));
        REQUIRE(size_summary2.mean() == Approx(0.005));
        REQUIRE(size_summary2.variance() == Approx(0.0));
        REQUIRE(size_summary3.mean() == Approx(0.005));
        REQUIRE(size_summary3.variance() == Approx(0.0));

        REQUIRE(size_summary21.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary21.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary22.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary22.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary23.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary23.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));


        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_pop1");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "root_height_pop1",
                "pop_size_kya",
                "pop_size_pop1",
                "pop_size_fas",
                "pop_size_pop2",
                "pop_size_root_kya",
                "pop_size_root_pop1",
                "ln_prior",
                "ln_prior_pop1",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with TimeSizeRateMixer with more gamma priors opt", "[SamplingPrior]") {
    SECTION("Testing gamma(10.0, 10.0) and gamma(2.0, 0.5) prior with optimizing") {
        double time_shape = 10.0;
        double time_scale = 10.0;
        double size_shape = 2.0;
        double size_scale = 0.5;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-collection-scaler-test4-" + tag + "-t456.cfg";
        std::string log_path = "data/tmp-config-collection-scaler-test4-" + tag + "-t456-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << time_shape << "\n";
        os << "        scale: " << time_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "123456";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 50001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(time_shape * time_scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(time_shape * time_scale * time_scale).epsilon(0.01));

        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");

        REQUIRE(size_summary1.sample_size() == 50001);
        REQUIRE(size_summary2.sample_size() == 50001);
        REQUIRE(size_summary3.sample_size() == 50001);

        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));


        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "pop_size_kya",
                "pop_size_fas",
                "pop_size_root_kya",
                "ln_prior",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with EventTimeScaler", "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and no optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t457.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t457-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << shape << "\n";
        os << "        scale: " << scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 10001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));


        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "ln_prior",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with EventTimeScaler with optimizing",
        "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-comp-height-scaler-" + tag + "-t458.cfg";
        std::string log_path = "data/tmp-config-comp-height-scaler-" + tag + "-t458-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << shape << "\n";
        os << "        scale: " << scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "4321";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("root_height_kya");
        REQUIRE(samples.size() == 10001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("root_height_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));

        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "root_height_kya",
                "ln_prior",
                "ln_prior_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with RootPopulationSizeScaler", "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and no optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t459.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t459-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1111";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("pop_size_root_kya");
        REQUIRE(samples.size() == 10001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("pop_size_root_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));

        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);
        summary = prior_sample.summarize<double>("ln_likelihood_kya");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {"generation",
            "ln_prior",
            "ln_prior_kya",
            "pop_size_root_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with RootPopulationSizeScaler with optimizing", "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-root-pop-size-scaler-" + tag + "-t460.cfg";
        std::string log_path = "data/tmp-config-root-pop-size-scaler-" + tag + "-t460-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "2222";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("pop_size_root_kya");
        REQUIRE(samples.size() == 10001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("pop_size_root_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "pop_size_root_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with LeafPopulationSizeScaler", "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and no optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t461.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t461-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1111";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples1 = prior_sample.get<double>("pop_size_kya");
        REQUIRE(samples1.size() == 10001);
        std::vector<double> samples2 = prior_sample.get<double>("pop_size_fas");
        REQUIRE(samples2.size() == 10001);

        for (size_t i = 0; i < samples1.size(); ++i) {
            if (i > 0) {
                REQUIRE(samples1.at(i) != samples2.at(i));
            }
        }

        SampleSummarizer<double> summary1 = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE(summary1.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary1.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> summary2 = prior_sample.summarize<double>("pop_size_fas");
        REQUIRE(summary2.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary2.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));

        SampleSummarizer<double> summary;
        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "pop_size_kya",
                "pop_size_fas"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}


TEST_CASE("Testing sampling from prior with LeafPopulationSizeScaler with optimizing", "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t462.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t462-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "2222";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples1 = prior_sample.get<double>("pop_size_kya");
        REQUIRE(samples1.size() == 10001);
        std::vector<double> samples2 = prior_sample.get<double>("pop_size_fas");
        REQUIRE(samples2.size() == 10001);

        for (size_t i = 0; i < samples1.size(); ++i) {
            if (i > 0) {
                REQUIRE(samples1.at(i) != samples2.at(i));
            }
        }

        SampleSummarizer<double> summary1 = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE(summary1.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary1.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> summary2 = prior_sample.summarize<double>("pop_size_fas");
        REQUIRE(summary2.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary2.variance() == Approx(shape * scale * scale).epsilon(0.01));

        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(f_summary.mean() == Approx(0.5));
        REQUIRE(f_summary.variance() == Approx(0.0));

        SampleSummarizer<double> summary;
        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "pop_size_kya",
                "pop_size_fas"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with RootPopulationSizeScaler and LeafPopulationSizeScaler on constrained sizes",
        "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and no optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t463.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t463-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1111";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples1 = prior_sample.get<double>("pop_size_kya");
        REQUIRE(samples1.size() == 10001);
        std::vector<double> samples2 = prior_sample.get<double>("pop_size_fas");
        REQUIRE(samples2.size() == 10001);
        std::vector<double> samples3 = prior_sample.get<double>("pop_size_root_kya");
        REQUIRE(samples2.size() == 10001);

        for (size_t i = 0; i < samples1.size(); ++i) {
            REQUIRE(samples1.at(i) == samples2.at(i));
            REQUIRE(samples1.at(i) == samples3.at(i));
        }

        SampleSummarizer<double> summary = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "pop_size_kya",
                "pop_size_fas",
                "pop_size_root_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with RootPopulationSizeScaler and LeafPopulationSizeScaler on constrained sizes with optimizing",
        "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t464.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t464-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1111";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples1 = prior_sample.get<double>("pop_size_kya");
        REQUIRE(samples1.size() == 10001);
        std::vector<double> samples2 = prior_sample.get<double>("pop_size_fas");
        REQUIRE(samples2.size() == 10001);
        std::vector<double> samples3 = prior_sample.get<double>("pop_size_root_kya");
        REQUIRE(samples2.size() == 10001);

        for (size_t i = 0; i < samples1.size(); ++i) {
            REQUIRE(samples1.at(i) == samples2.at(i));
            REQUIRE(samples1.at(i) == samples3.at(i));
        }

        SampleSummarizer<double> summary = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "pop_size_kya",
                "pop_size_fas",
                "pop_size_root_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from beta(1.5, 2.5) prior with FreqMover",
        "[SamplingPrior]") {

    SECTION("Testing beta(1.5, 2.5) prior and no optimizing") {
        double a = 1.5;
        double b = 2.5;
        double expected_mean = a / (a + b);
        double expected_variance = (a * b) / ((a + b) * (a + b) * (a + b + 1.0));

        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t465.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t465-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << a << "\n";
        os << "                    beta: " << b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("freq_1_kya");
        REQUIRE(samples.size() == 10001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(summary.mean() == Approx(expected_mean).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(expected_variance).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "freq_1_kya"
                };
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from beta(1.5, 2.5) prior with FreqMover and optimizing",
        "[SamplingPrior]") {

    SECTION("Testing beta(1.5, 2.5) prior and optimizing") {
        double a = 1.5;
        double b = 2.5;
        double expected_mean = a / (a + b);
        double expected_variance = (a * b) / ((a + b) * (a + b) * (a + b + 1.0));

        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t466.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t466-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 200000\n";
        os << "    sample_frequency: 5\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << a << "\n";
        os << "                    beta: " << b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234567";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("freq_1_kya");
        REQUIRE(samples.size() == 40001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(summary.mean() == Approx(expected_mean).epsilon(0.001));
        REQUIRE(summary.variance() == Approx(expected_variance).epsilon(0.001));

        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "freq_1_kya"
                };
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from beta(2.5, 1.5) prior with FreqMover and optimizing",
        "[SamplingPrior]") {

    SECTION("Testing beta(2.5, 1.5) prior and optimizing") {
        double a = 2.5;
        double b = 1.5;
        double expected_mean = a / (a + b);
        double expected_variance = (a * b) / ((a + b) * (a + b) * (a + b + 1.0));

        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t467.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t467-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 200000\n";
        os << "    sample_frequency: 5\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << a << "\n";
        os << "                    beta: " << b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "123456";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("freq_1_kya");
        REQUIRE(samples.size() == 40001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("freq_1_kya");
        REQUIRE(summary.mean() == Approx(expected_mean).epsilon(0.001));
        REQUIRE(summary.variance() == Approx(expected_variance).epsilon(0.001));

        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "freq_1_kya"
                };
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with MutationRateScaler",
        "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and no optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t468.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t468-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.001\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "7777";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("mutation_rate_kya");
        REQUIRE(samples.size() == 10001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("mutation_rate_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "mutation_rate_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling from prior with MutationRateScaler with optimizing",
        "[SamplingPrior]") {

    SECTION("Testing gamma(10.0, 0.1) prior and optimizing") {
        double shape = 10.0;
        double scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t469.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t469-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: 10.0\n";
        os << "        scale: 0.001\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 0.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.001\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << shape << "\n";
        os << "                    scale: " << scale << "\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "7777";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);
        std::vector<double> samples = prior_sample.get<double>("mutation_rate_kya");
        REQUIRE(samples.size() == 10001);

        SampleSummarizer<double> summary = prior_sample.summarize<double>("mutation_rate_kya");
        REQUIRE(summary.mean() == Approx(shape * scale).epsilon(0.01));
        REQUIRE(summary.variance() == Approx(shape * scale * scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(summary.mean() == 0.0);
        REQUIRE(summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "mutation_rate_kya"};
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing fully parameterized model for one pair",
        "[SamplingPrior]") {

    SECTION("Testing with no optimizing") {
        double height_shape = 1.0;
        double height_scale = 0.1;
        double size_shape = 10.0;
        double size_scale = 0.0001;
        double f_a = 1.0;
        double f_b = 0.5;
        double expected_f_mean = f_a / (f_a + f_b);
        double expected_f_variance = (f_a * f_b) / ((f_a + f_b) * (f_a + f_b) * (f_a + f_b + 1.0));
        double mult_shape = 10.0;
        double mult_scale = 0.1;
        std::string auto_optimize = "false";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t470.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t470-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a << "\n";
        os << "                    beta: " << f_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult_shape << "\n";
        os << "                    scale: " << mult_scale << "\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "9876";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> mult_summary = prior_sample.summarize<double>("mutation_rate_kya");

        REQUIRE(height_summary.sample_size() == 50001);
        REQUIRE(size_summary1.sample_size() == 50001);
        REQUIRE(size_summary2.sample_size() == 50001);
        REQUIRE(size_summary3.sample_size() == 50001);
        REQUIRE(f_summary.sample_size() == 50001);
        REQUIRE(mult_summary.sample_size() == 50001);

        REQUIRE(height_summary.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(f_summary.mean() == Approx(expected_f_mean).epsilon(0.005));
        REQUIRE(f_summary.variance() == Approx(expected_f_variance).epsilon(0.005));
        REQUIRE(mult_summary.mean() == Approx(mult_shape * mult_scale).epsilon(0.01));
        REQUIRE(mult_summary.variance() == Approx(mult_shape * mult_scale * mult_scale).epsilon(0.01));

        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.sample_size() == 50001);
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<double> size_sample1 = prior_sample.get<double>("pop_size_kya");
        std::vector<double> size_sample2 = prior_sample.get<double>("pop_size_fas");
        std::vector<double> size_sample3 = prior_sample.get<double>("pop_size_root_kya");
        for (size_t i = 0; i < size_sample1.size(); ++i) {
            if (i > 99) {
                REQUIRE(size_sample1.at(i) != size_sample2.at(i));
                REQUIRE(size_sample1.at(i) != size_sample3.at(i));
                REQUIRE(size_sample2.at(i) != size_sample3.at(i));
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing fully parameterized model for one pair with optimization",
        "[SamplingPrior]") {

    SECTION("Testing with optimizing") {
        double height_shape = 1.0;
        double height_scale = 0.1;
        double size_shape = 10.0;
        double size_scale = 0.0001;
        double f_a = 1.0;
        double f_b = 0.5;
        double expected_f_mean = f_a / (f_a + f_b);
        double expected_f_variance = (f_a * f_b) / ((f_a + f_b) * (f_a + f_b) * (f_a + f_b + 1.0));
        double mult_shape = 10.0;
        double mult_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t471.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t471-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a << "\n";
        os << "                    beta: " << f_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult_shape << "\n";
        os << "                    scale: " << mult_scale << "\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "9876";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> mult_summary = prior_sample.summarize<double>("mutation_rate_kya");

        REQUIRE(height_summary.sample_size() == 50001);
        REQUIRE(size_summary1.sample_size() == 50001);
        REQUIRE(size_summary2.sample_size() == 50001);
        REQUIRE(size_summary3.sample_size() == 50001);
        REQUIRE(f_summary.sample_size() == 50001);
        REQUIRE(mult_summary.sample_size() == 50001);

        REQUIRE(height_summary.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(f_summary.mean() == Approx(expected_f_mean).epsilon(0.005));
        REQUIRE(f_summary.variance() == Approx(expected_f_variance).epsilon(0.005));
        REQUIRE(mult_summary.mean() == Approx(mult_shape * mult_scale).epsilon(0.01));
        REQUIRE(mult_summary.variance() == Approx(mult_shape * mult_scale * mult_scale).epsilon(0.01));

        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.sample_size() == 50001);
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<double> size_sample1 = prior_sample.get<double>("pop_size_kya");
        std::vector<double> size_sample2 = prior_sample.get<double>("pop_size_fas");
        std::vector<double> size_sample3 = prior_sample.get<double>("pop_size_root_kya");
        for (size_t i = 0; i < size_sample1.size(); ++i) {
            if (i > 99) {
                REQUIRE(size_sample1.at(i) != size_sample2.at(i));
                REQUIRE(size_sample1.at(i) != size_sample3.at(i));
                REQUIRE(size_sample2.at(i) != size_sample3.at(i));
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing fully parameterized model for one pair with optimization and TimeSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing with optimizing") {
        double height_shape = 1.0;
        double height_scale = 0.1;
        double size_shape = 10.0;
        double size_scale = 0.0001;
        double f_a = 1.0;
        double f_b = 0.5;
        double expected_f_mean = f_a / (f_a + f_b);
        double expected_f_variance = (f_a * f_b) / ((f_a + f_b) * (f_a + f_b) * (f_a + f_b + 1.0));
        double mult_shape = 10.0;
        double mult_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t472.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t472-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 800000\n";
        os << "    sample_frequency: 8\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 0.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape << "\n";
        os << "                    scale: " << size_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a << "\n";
        os << "                    beta: " << f_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult_shape << "\n";
        os << "                    scale: " << mult_scale << "\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "9876345";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> size_summary1 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_summary2 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_summary3 = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> f_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> mult_summary = prior_sample.summarize<double>("mutation_rate_kya");

        REQUIRE(height_summary.sample_size() == 100001);
        REQUIRE(size_summary1.sample_size() == 100001);
        REQUIRE(size_summary2.sample_size() == 100001);
        REQUIRE(size_summary3.sample_size() == 100001);
        REQUIRE(f_summary.sample_size() == 100001);
        REQUIRE(mult_summary.sample_size() == 100001);

        REQUIRE(height_summary.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(size_summary1.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary1.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary2.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.mean() == Approx(size_shape * size_scale).epsilon(0.01));
        REQUIRE(size_summary3.variance() == Approx(size_shape * size_scale * size_scale).epsilon(0.01));
        REQUIRE(f_summary.mean() == Approx(expected_f_mean).epsilon(0.005));
        REQUIRE(f_summary.variance() == Approx(expected_f_variance).epsilon(0.005));
        REQUIRE(mult_summary.mean() == Approx(mult_shape * mult_scale).epsilon(0.01));
        REQUIRE(mult_summary.variance() == Approx(mult_shape * mult_scale * mult_scale).epsilon(0.01));

        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.sample_size() == 100001);
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<double> size_sample1 = prior_sample.get<double>("pop_size_kya");
        std::vector<double> size_sample2 = prior_sample.get<double>("pop_size_fas");
        std::vector<double> size_sample3 = prior_sample.get<double>("pop_size_root_kya");
        for (size_t i = 0; i < size_sample1.size(); ++i) {
            if (i > 99) {
                REQUIRE(size_sample1.at(i) != size_sample2.at(i));
                REQUIRE(size_sample1.at(i) != size_sample3.at(i));
                REQUIRE(size_sample2.at(i) != size_sample3.at(i));
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 2 pairs and alpha 1.0", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 1.0") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t473.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t473-state-run-1.log";
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
        os << "                value: 1.0\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");

        int nshared = 0;
        int total = 0;
        for (size_t i = 0; i < nevents.size(); ++i) {
            REQUIRE((nevents.at(i) == 1 || nevents.at(i) == 2));
            REQUIRE((event_indices1.at(i) == 0 || event_indices1.at(i) == 1));
            REQUIRE((event_indices2.at(i) == 0 || event_indices2.at(i) == 1));
            if (nevents.at(i) == 1) {
                ++nshared;
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
            }
            else {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
            }
            ++total;
        }
        REQUIRE(total == 10001);
        REQUIRE((nshared / 10001.0) == Approx(0.5).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_index_kya",
                "root_height_index_pop1"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 2 pairs and alpha 2.0", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 2.0") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t474.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t474-state-run-1.log";
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
        os << "                value: 2.0\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");

        int nshared = 0;
        int total = 0;
        for (size_t i = 0; i < nevents.size(); ++i) {
            REQUIRE((nevents.at(i) == 1 || nevents.at(i) == 2));
            REQUIRE((event_indices1.at(i) == 0 || event_indices1.at(i) == 1));
            REQUIRE((event_indices2.at(i) == 0 || event_indices2.at(i) == 1));
            if (nevents.at(i) == 1) {
                ++nshared;
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
            }
            else {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
            }
            ++total;
        }
        REQUIRE(total == 10001);
        REQUIRE((nshared / 10001.0) == Approx(1.0/3.0).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_index_kya",
                "root_height_index_pop1"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 2 pairs and alpha 0.5", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 0.5") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t475.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t475-state-run-1.log";
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
        os << "                value: 0.5\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");

        int nshared = 0;
        int total = 0;
        for (size_t i = 0; i < nevents.size(); ++i) {
            REQUIRE((nevents.at(i) == 1 || nevents.at(i) == 2));
            REQUIRE((event_indices1.at(i) == 0 || event_indices1.at(i) == 1));
            REQUIRE((event_indices2.at(i) == 0 || event_indices2.at(i) == 1));
            if (nevents.at(i) == 1) {
                ++nshared;
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
            }
            else {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
            }
            ++total;
        }
        REQUIRE(total == 10001);
        REQUIRE((nshared / 10001.0) == Approx(2.0/3.0).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_index_kya",
                "root_height_index_pop1"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 3 pairs and alpha 1.0", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 1.0") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration = 1.0;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t476.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t476-state-run-1.log";
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
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == 10001);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == 10001);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == 10001);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (std::string m: {"000", "001", "010", "011", "012"}) {
            REQUIRE((model_counts.at(m) / 10001.0) == Approx(std::exp(
                    get_dpp_log_prior_probability(m, concentration))).epsilon(0.01));
        }

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_pop1b",
                "root_height_index_kya",
                "root_height_index_pop1",
                "root_height_index_pop1b"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 3 pairs and alpha 4.0", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 4.0") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration = 4.0;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t477.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t477-state-run-1.log";
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
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == 10001);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == 10001);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == 10001);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (std::string m: {"000", "001", "010", "011", "012"}) {
            REQUIRE((model_counts.at(m) / 10001.0) == Approx(std::exp(
                    get_dpp_log_prior_probability(m, concentration))).epsilon(0.01));
        }

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_pop1b",
                "root_height_index_kya",
                "root_height_index_pop1",
                "root_height_index_pop1b"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 6 pairs and alpha 1.7", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 1.7") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration = 1.7;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t478.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t478-state-run-1.log";
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
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 1000000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname3.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname4.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname5.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "7654321";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        SampleSummarizer<double> height_summary4 = prior_sample.summarize<double>("root_height_pop1c");
        SampleSummarizer<double> height_summary5 = prior_sample.summarize<double>("root_height_pop1d");
        SampleSummarizer<double> height_summary6 = prior_sample.summarize<double>("root_height_pop1e");
        REQUIRE(height_summary1.sample_size() == 100001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary2.sample_size() == 100001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary3.sample_size() == 100001);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary4.sample_size() == 100001);
        REQUIRE(height_summary4.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary4.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary5.sample_size() == 100001);
        REQUIRE(height_summary5.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary5.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary6.sample_size() == 100001);
        REQUIRE(height_summary6.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary6.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<int> event_indices4 = prior_sample.get<int>("root_height_index_pop1c");
        std::vector<int> event_indices5 = prior_sample.get<int>("root_height_index_pop1d");
        std::vector<int> event_indices6 = prior_sample.get<int>("root_height_index_pop1e");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");
        std::vector<double> heights4 = prior_sample.get<double>("root_height_pop1c");
        std::vector<double> heights5 = prior_sample.get<double>("root_height_pop1d");
        std::vector<double> heights6 = prior_sample.get<double>("root_height_pop1e");

        std::map<std::string, int> model_counts;
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0},
                {4, 0},
                {5, 0},
                {6, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            stream << event_indices4.at(i);
            stream << event_indices5.at(i);
            stream << event_indices6.at(i);
            std::string model_str = stream.str();
            if (model_counts.count(model_str) < 1) {
                model_counts[model_str] = 1;
            }
            else {
                ++model_counts[model_str];
            }
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(event_indices1.at(i) == event_indices4.at(i));
                REQUIRE(event_indices1.at(i) == event_indices5.at(i));
                REQUIRE(event_indices1.at(i) == event_indices6.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
                REQUIRE(heights1.at(i) == heights4.at(i));
                REQUIRE(heights1.at(i) == heights5.at(i));
                REQUIRE(heights1.at(i) == heights6.at(i));
            }
            else if (nevents.at(i) == 6) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices1.at(i) != event_indices4.at(i));
                REQUIRE(event_indices1.at(i) != event_indices5.at(i));
                REQUIRE(event_indices1.at(i) != event_indices6.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices4.at(i));
                REQUIRE(event_indices2.at(i) != event_indices5.at(i));
                REQUIRE(event_indices2.at(i) != event_indices6.at(i));
                REQUIRE(event_indices3.at(i) != event_indices4.at(i));
                REQUIRE(event_indices3.at(i) != event_indices5.at(i));
                REQUIRE(event_indices3.at(i) != event_indices6.at(i));
                REQUIRE(event_indices4.at(i) != event_indices5.at(i));
                REQUIRE(event_indices4.at(i) != event_indices6.at(i));
                REQUIRE(event_indices5.at(i) != event_indices6.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights1.at(i) != heights4.at(i));
                REQUIRE(heights1.at(i) != heights5.at(i));
                REQUIRE(heights1.at(i) != heights6.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights4.at(i));
                REQUIRE(heights2.at(i) != heights5.at(i));
                REQUIRE(heights2.at(i) != heights6.at(i));
                REQUIRE(heights3.at(i) != heights4.at(i));
                REQUIRE(heights3.at(i) != heights5.at(i));
                REQUIRE(heights3.at(i) != heights6.at(i));
                REQUIRE(heights4.at(i) != heights5.at(i));
                REQUIRE(heights4.at(i) != heights6.at(i));
                REQUIRE(heights5.at(i) != heights6.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == 100001);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == 100001);

        REQUIRE(model_counts.at("000000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012345") == nevent_counts.at(6));

        for (std::string m: {"000000", "012345", "012344", "012340", "001234", "012314"}) {
            REQUIRE((model_counts.at(m) / 100001.0) == Approx(std::exp(
                    get_dpp_log_prior_probability(m, concentration))).epsilon(0.002));
        }

        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 3 pairs and fully parameterized", "[SamplingPrior]") {

    SECTION("Testing alpha integrated") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t479.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t479-state-run-1.log";
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
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 2\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 3 pairs and fully parameterized and TimeSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing alpha integrated") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t480.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t480-state-run-1.log";
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
        os << "    chain_length: 400000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 40001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling of small concentration", "[SamplingPrior]") {

    SECTION("Testing concentration sampling") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration_shape = 10.0;
        double concentration_scale = 0.01;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t481.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t481-state-run-1.log";
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
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.866).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.0046).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "concentration",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_pop1b",
                "root_height_index_kya",
                "root_height_index_pop1",
                "root_height_index_pop1b"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling of large concentration", "[SamplingPrior]") {

    SECTION("Testing concentration sampling") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration_shape = 10.0;
        double concentration_scale = 2.0;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t482.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t482-state-run-1.log";
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
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "239472";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.1));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.0057).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.854).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "concentration",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_pop1b",
                "root_height_index_kya",
                "root_height_index_pop1",
                "root_height_index_pop1b"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing sampling of diffuse concentration", "[SamplingPrior]") {

    SECTION("Testing concentration sampling") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration_shape = 0.5;
        double concentration_scale = 1.0;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t483.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t483-state-run-1.log";
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
        os << "    chain_length: 2000000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234567";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 200001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.1));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));


        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        // Expected prior probs estimated by 10 million simulations using the
        // 'dmc_estimate_prior_probs.py' tool from the PyMsBayes package
        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.673).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.0725).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "concentration",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_pop1b",
                "root_height_index_kya",
                "root_height_index_pop1",
                "root_height_index_pop1b"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

#ifdef BUILD_WITH_THREADS
TEST_CASE("Testing DPP with 3 pairs, fully parameterized, and 3 threads",
        "[SamplingPrior]") {

    SECTION("Testing multithreading") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t484.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t484-state-run-1.log";
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
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "72349827";
        char arg3[] = "--ignore-data";
        char arg4[] = "--nthreads";
        char arg5[] = "3";
        char * cfg_path = new char[test_path.size() + 1];
        std::copy(test_path.begin(), test_path.end(), cfg_path);
        cfg_path[test_path.size()] = '\0';
        char * argv[] = {
            &arg0[0],
            &arg1[0],
            &arg2[0],
            &arg3[0],
            &arg4[0],
            &arg5[0],
            cfg_path,
            NULL
        };
        int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        int ret;
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}
#endif

#ifdef BUILD_WITH_THREADS
TEST_CASE("Testing DPP with 3 pairs, fully parameterized, and 2 threads",
        "[SamplingPrior]") {

    SECTION("Testing nthreads 2") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t485.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t485-state-run-1.log";
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
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "72349827";
        char arg3[] = "--ignore-data";
        char arg4[] = "--nthreads";
        char arg5[] = "2";
        char * cfg_path = new char[test_path.size() + 1];
        std::copy(test_path.begin(), test_path.end(), cfg_path);
        cfg_path[test_path.size()] = '\0';
        char * argv[] = {
            &arg0[0],
            &arg1[0],
            &arg2[0],
            &arg3[0],
            &arg4[0],
            &arg5[0],
            cfg_path,
            NULL
        };
        int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        int ret;
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}
#endif

TEST_CASE("Testing ReversibleJumpSampler with 2 pairs", "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 2 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t486.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t486-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "6623";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 10001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");

        int nshared = 0;
        int total = 0;
        for (size_t i = 0; i < nevents.size(); ++i) {
            REQUIRE((nevents.at(i) == 1 || nevents.at(i) == 2));
            REQUIRE((event_indices1.at(i) == 0 || event_indices1.at(i) == 1));
            REQUIRE((event_indices2.at(i) == 0 || event_indices2.at(i) == 1));
            if (nevents.at(i) == 1) {
                ++nshared;
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
            }
            else {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
            }
            ++total;
        }
        REQUIRE(total == expected_sample_size);
        REQUIRE((nshared / (double)expected_sample_size) == Approx(0.5).epsilon(0.01));

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_index_kya",
                "root_height_index_pop1"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 3 pairs", "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 3 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t487.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t487-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 10001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (std::string m: {"000", "001", "010", "011", "012"}) {
            REQUIRE((model_counts.at(m) / (double)expected_sample_size) == Approx(1.0/model_counts.size()).epsilon(0.01));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_pop1b",
                "root_height_index_kya",
                "root_height_index_pop1",
                "root_height_index_pop1b"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 6 pairs", "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 6 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t488.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t488-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 2000000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname3.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname4.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname5.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "68165454";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 200001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        SampleSummarizer<double> height_summary4 = prior_sample.summarize<double>("root_height_pop1c");
        SampleSummarizer<double> height_summary5 = prior_sample.summarize<double>("root_height_pop1d");
        SampleSummarizer<double> height_summary6 = prior_sample.summarize<double>("root_height_pop1e");

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<int> event_indices4 = prior_sample.get<int>("root_height_index_pop1c");
        std::vector<int> event_indices5 = prior_sample.get<int>("root_height_index_pop1d");
        std::vector<int> event_indices6 = prior_sample.get<int>("root_height_index_pop1e");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");
        std::vector<double> heights4 = prior_sample.get<double>("root_height_pop1c");
        std::vector<double> heights5 = prior_sample.get<double>("root_height_pop1d");
        std::vector<double> heights6 = prior_sample.get<double>("root_height_pop1e");

        std::map<std::string, int> model_counts;
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0},
                {4, 0},
                {5, 0},
                {6, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            stream << event_indices4.at(i);
            stream << event_indices5.at(i);
            stream << event_indices6.at(i);
            std::string model_str = stream.str();
            if (model_counts.count(model_str) < 1) {
                model_counts[model_str] = 1;
            }
            else {
                ++model_counts[model_str];
            }
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(event_indices1.at(i) == event_indices4.at(i));
                REQUIRE(event_indices1.at(i) == event_indices5.at(i));
                REQUIRE(event_indices1.at(i) == event_indices6.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
                REQUIRE(heights1.at(i) == heights4.at(i));
                REQUIRE(heights1.at(i) == heights5.at(i));
                REQUIRE(heights1.at(i) == heights6.at(i));
            }
            else if (nevents.at(i) == 6) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices1.at(i) != event_indices4.at(i));
                REQUIRE(event_indices1.at(i) != event_indices5.at(i));
                REQUIRE(event_indices1.at(i) != event_indices6.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices4.at(i));
                REQUIRE(event_indices2.at(i) != event_indices5.at(i));
                REQUIRE(event_indices2.at(i) != event_indices6.at(i));
                REQUIRE(event_indices3.at(i) != event_indices4.at(i));
                REQUIRE(event_indices3.at(i) != event_indices5.at(i));
                REQUIRE(event_indices3.at(i) != event_indices6.at(i));
                REQUIRE(event_indices4.at(i) != event_indices5.at(i));
                REQUIRE(event_indices4.at(i) != event_indices6.at(i));
                REQUIRE(event_indices5.at(i) != event_indices6.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights1.at(i) != heights4.at(i));
                REQUIRE(heights1.at(i) != heights5.at(i));
                REQUIRE(heights1.at(i) != heights6.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights4.at(i));
                REQUIRE(heights2.at(i) != heights5.at(i));
                REQUIRE(heights2.at(i) != heights6.at(i));
                REQUIRE(heights3.at(i) != heights4.at(i));
                REQUIRE(heights3.at(i) != heights5.at(i));
                REQUIRE(heights3.at(i) != heights6.at(i));
                REQUIRE(heights4.at(i) != heights5.at(i));
                REQUIRE(heights4.at(i) != heights6.at(i));
                REQUIRE(heights5.at(i) != heights6.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012345") == nevent_counts.at(6));

        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (std::string m: {"000000", "012345", "012344", "012340", "001234", "012314"}) {
            std::cout << m << ": " << model_counts.at(m) / (double)expected_sample_size << "\n";
        }

        for (std::string m: {"000000", "012345", "012344", "012340", "001234", "012314"}) {
            REQUIRE((model_counts.at(m) / (double)expected_sample_size) == Approx(1.0/bell_float(6)).epsilon(0.002));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(6, kv.first)/bell_float(6)).epsilon(0.002));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary4.sample_size() == expected_sample_size);
        REQUIRE(height_summary4.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary4.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary5.sample_size() == expected_sample_size);
        REQUIRE(height_summary5.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary5.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary6.sample_size() == expected_sample_size);
        REQUIRE(height_summary6.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary6.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));

        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 6 pairs and diffuse gamma", "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 6 pairs and diffuse gamma") {
        double height_shape = 1.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t489.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t489-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 1000000\n";
        os << "    sample_frequency: 5\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname3.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname4.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname5.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "482662423";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 200001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        SampleSummarizer<double> height_summary4 = prior_sample.summarize<double>("root_height_pop1c");
        SampleSummarizer<double> height_summary5 = prior_sample.summarize<double>("root_height_pop1d");
        SampleSummarizer<double> height_summary6 = prior_sample.summarize<double>("root_height_pop1e");

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<int> event_indices4 = prior_sample.get<int>("root_height_index_pop1c");
        std::vector<int> event_indices5 = prior_sample.get<int>("root_height_index_pop1d");
        std::vector<int> event_indices6 = prior_sample.get<int>("root_height_index_pop1e");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");
        std::vector<double> heights4 = prior_sample.get<double>("root_height_pop1c");
        std::vector<double> heights5 = prior_sample.get<double>("root_height_pop1d");
        std::vector<double> heights6 = prior_sample.get<double>("root_height_pop1e");

        std::map<std::string, int> model_counts;
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0},
                {4, 0},
                {5, 0},
                {6, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            stream << event_indices4.at(i);
            stream << event_indices5.at(i);
            stream << event_indices6.at(i);
            std::string model_str = stream.str();
            if (model_counts.count(model_str) < 1) {
                model_counts[model_str] = 1;
            }
            else {
                ++model_counts[model_str];
            }
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(event_indices1.at(i) == event_indices4.at(i));
                REQUIRE(event_indices1.at(i) == event_indices5.at(i));
                REQUIRE(event_indices1.at(i) == event_indices6.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
                REQUIRE(heights1.at(i) == heights4.at(i));
                REQUIRE(heights1.at(i) == heights5.at(i));
                REQUIRE(heights1.at(i) == heights6.at(i));
            }
            else if (nevents.at(i) == 6) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices1.at(i) != event_indices4.at(i));
                REQUIRE(event_indices1.at(i) != event_indices5.at(i));
                REQUIRE(event_indices1.at(i) != event_indices6.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices4.at(i));
                REQUIRE(event_indices2.at(i) != event_indices5.at(i));
                REQUIRE(event_indices2.at(i) != event_indices6.at(i));
                REQUIRE(event_indices3.at(i) != event_indices4.at(i));
                REQUIRE(event_indices3.at(i) != event_indices5.at(i));
                REQUIRE(event_indices3.at(i) != event_indices6.at(i));
                REQUIRE(event_indices4.at(i) != event_indices5.at(i));
                REQUIRE(event_indices4.at(i) != event_indices6.at(i));
                REQUIRE(event_indices5.at(i) != event_indices6.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights1.at(i) != heights4.at(i));
                REQUIRE(heights1.at(i) != heights5.at(i));
                REQUIRE(heights1.at(i) != heights6.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights4.at(i));
                REQUIRE(heights2.at(i) != heights5.at(i));
                REQUIRE(heights2.at(i) != heights6.at(i));
                REQUIRE(heights3.at(i) != heights4.at(i));
                REQUIRE(heights3.at(i) != heights5.at(i));
                REQUIRE(heights3.at(i) != heights6.at(i));
                REQUIRE(heights4.at(i) != heights5.at(i));
                REQUIRE(heights4.at(i) != heights6.at(i));
                REQUIRE(heights5.at(i) != heights6.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012345") == nevent_counts.at(6));

        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (std::string m: {"000000", "012345", "012344", "012340", "001234", "012314"}) {
            std::cout << m << ": " << model_counts.at(m) / (double)expected_sample_size << "\n";
        }

        for (std::string m: {"000000", "012345", "012344", "012340", "001234", "012314"}) {
            REQUIRE((model_counts.at(m) / (double)expected_sample_size) == Approx(1.0/bell_float(6)).epsilon(0.002));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(6, kv.first)/bell_float(6)).epsilon(0.002));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary4.sample_size() == expected_sample_size);
        REQUIRE(height_summary4.mean() == Approx(height_shape * height_scale).epsilon(0.005));
        REQUIRE(height_summary4.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary5.sample_size() == expected_sample_size);
        REQUIRE(height_summary5.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary5.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));
        REQUIRE(height_summary6.sample_size() == expected_sample_size);
        REQUIRE(height_summary6.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary6.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.001));

        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 3 pairs and fully parameterized", "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 3 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t490.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t490-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 600000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "72938742";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 60001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.01));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 3 pairs and fully parameterized and TimeSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 3 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t491.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t491-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 2000000\n";
        os << "    sample_frequency: 20\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "589615439";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 100001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.01));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

#ifdef BUILD_WITH_THREADS
TEST_CASE("Testing ReversibleJumpSampler with 3 pairs, fully parameterized, and 2 threads",
        "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 3 pairs and 2 threads") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t492.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t492-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 400000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "571518456";
        char arg3[] = "--ignore-data";
        char arg4[] = "--nthreads";
        char arg5[] = "2";
        char * cfg_path = new char[test_path.size() + 1];
        std::copy(test_path.begin(), test_path.end(), cfg_path);
        cfg_path[test_path.size()] = '\0';
        char * argv[] = {
            &arg0[0],
            &arg1[0],
            &arg2[0],
            &arg3[0],
            &arg4[0],
            &arg5[0],
            cfg_path,
            NULL
        };
        int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
        int ret;
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 40001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.005));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}
#endif

TEST_CASE("Testing DPP with 2 singletons and 1 pair, fully parameterized",
        "[SamplingPrior]") {

    SECTION("Testing alpha integrated") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t493.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t493-state-run-1.log";
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
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "72349827";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_fas"));
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_pop2"));
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 2 singletons and 1 pair, fully parameterized, and TimeSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing alpha integrated") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t494.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t494-state-run-1.log";
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
        os << "    chain_length: 400000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 40001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_fas"));
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_pop2"));
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 2 singletons, 1 pair, and fully parameterized",
        "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 2 singletons and a pair") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t495.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t495-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 800000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "46184354";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 80001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_fas"));
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_pop2"));
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.01));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 2 singletons, 1 pair, TimeSizeRateMixer, and fully parameterized",
        "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 2 singletons and a pair") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t496.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t496-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 600000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "684126";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 60001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_fas"));
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_pop2"));
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.01));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing fixed 012 and fully parameterized", "[SamplingPrior]") {

    SECTION("Testing fixed 012 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t497.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t497-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    fixed: [0, 1, 2]\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 200000\n";
        os << "    sample_frequency: 20\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "58961543";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 10001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.05));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.05));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.05));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.05));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.05));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.05));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.05));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.05));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.05));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.05));

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == 0);
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == 0);

        REQUIRE(model_counts.at("012") == expected_sample_size);
        REQUIRE(nevent_counts.at(3) == expected_sample_size);

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing fixed 012 and fully parameterized and TimeSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing fixed 012 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t498.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t498-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    fixed: [0, 1, 2]\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 200000\n";
        os << "    sample_frequency: 20\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "58961543";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 10001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.05));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.05));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.05));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.05));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.05));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.05));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.05));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.05));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.05));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.05));

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == 0);
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == 0);

        REQUIRE(model_counts.at("012") == expected_sample_size);
        REQUIRE(nevent_counts.at(3) == expected_sample_size);

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing fixed 000 and fully parameterized", "[SamplingPrior]") {

    SECTION("Testing fixed 000 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t499.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t499-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    fixed: [0, 0, 0]\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 200000\n";
        os << "    sample_frequency: 20\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "58961543";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 10001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.05));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.05));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.05));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.05));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.05));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.05));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.05));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.05));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.05));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.05));

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == 0);
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == 0);

        REQUIRE(model_counts.at("000") == expected_sample_size);
        REQUIRE(nevent_counts.at(1) == expected_sample_size);

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing fixed 000 and fully parameterized and TimeSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing fixed 000 pairs") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t500.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t500-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    fixed: [0, 0, 0]\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 200000\n";
        os << "    sample_frequency: 20\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "58961543";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 10001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.05));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.05));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.05));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.05));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.05));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.05));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.05));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.05));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.05));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.05));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.05));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.05));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.05));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.05));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.05));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.05));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.05));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.05));

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == 0);
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == 0);

        REQUIRE(model_counts.at("000") == expected_sample_size);
        REQUIRE(nevent_counts.at(1) == expected_sample_size);

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 3 dirichlet pairs and fully parameterized", "[SamplingPrior]") {

    SECTION("Testing DPP with 3 dirichlet pairs") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        std::vector<double> mult1_alpha = {10.0, 10.0, 10.0};
        std::vector<double> mult2_alpha = {10.0, 15.0, 5.0};
        std::vector<double> mult3_alpha = {15.0, 5.0, 10.0};
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t501.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t501-state-run-1.log";
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
        os << "    chain_length: 400000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 3\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MeanPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RelativePopulationSizeMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult1_alpha.at(0);
        for (unsigned int i = 1; i < mult1_alpha.size(); ++i) {
            os << ", " << mult1_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult2_alpha.at(0);
        for (unsigned int i = 1; i < mult2_alpha.size(); ++i) {
            os << ", " << mult2_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult3_alpha.at(0);
        for (unsigned int i = 1; i < mult3_alpha.size(); ++i) {
            os << ", " << mult3_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "28947598";
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
        ret = ecoevolity_main<DirichletCollectionSettings, ComparisonDirichletPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        unsigned int nnodes = 3;
        DirichletDistribution dir_dist1 = DirichletDistribution(mult1_alpha);
        std::vector<double> dir1_expected_means = dir_dist1.get_mean();
        for (unsigned int i = 0; i < dir1_expected_means.size(); ++i) {
            dir1_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir1_expected_variances = dir_dist1.get_variance();
        for (unsigned int i = 0; i < dir1_expected_variances.size(); ++i) {
            dir1_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist2 = DirichletDistribution(mult2_alpha);
        std::vector<double> dir2_expected_means = dir_dist2.get_mean();
        for (unsigned int i = 0; i < dir2_expected_means.size(); ++i) {
            dir2_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir2_expected_variances = dir_dist2.get_variance();
        for (unsigned int i = 0; i < dir2_expected_variances.size(); ++i) {
            dir2_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist3 = DirichletDistribution(mult3_alpha);
        std::vector<double> dir3_expected_means = dir_dist3.get_mean();
        for (unsigned int i = 0; i < dir3_expected_means.size(); ++i) {
            dir3_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir3_expected_variances = dir_dist3.get_variance();
        for (unsigned int i = 0; i < dir3_expected_variances.size(); ++i) {
            dir3_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 40001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size_mult1_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_kya");
        SampleSummarizer<double> size_mult1_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_fas");
        SampleSummarizer<double> size_mult1_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_kya");
        SampleSummarizer<double> size_mult2_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1");
        SampleSummarizer<double> size_mult2_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2");
        SampleSummarizer<double> size_mult2_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1");
        SampleSummarizer<double> size_mult3_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1b");
        SampleSummarizer<double> size_mult3_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2b");
        SampleSummarizer<double> size_mult3_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1b");

        std::cout << "\n";
        std::cout << "kya: "  << size_mult1_summary_0.mean() << "\n";
        std::cout << "fas: "  << size_mult1_summary_1.mean() << "\n";
        std::cout << "root: " << size_mult1_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult1_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_0.mean() == Approx(dir1_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_0.variance() == Approx(dir1_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.mean() == Approx(dir1_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.variance() == Approx(dir1_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_2.mean() == Approx(dir1_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult1_summary_2.variance() == Approx(dir1_expected_variances.at(2)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1: " << size_mult2_summary_0.mean() << "\n";
        std::cout << "pop2: " << size_mult2_summary_1.mean() << "\n";
        std::cout << "root: " << size_mult2_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult2_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_0.mean() == Approx(dir2_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_0.variance() == Approx(dir2_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.mean() == Approx(dir2_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.variance() == Approx(dir2_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_2.mean() == Approx(dir2_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult2_summary_2.variance() == Approx(dir2_expected_variances.at(2)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1b: " << size_mult3_summary_0.mean() << "\n";
        std::cout << "pop2b: " << size_mult3_summary_1.mean() << "\n";
        std::cout << "root: "  << size_mult3_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult3_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_0.mean() == Approx(dir3_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_0.variance() == Approx(dir3_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.mean() == Approx(dir3_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.variance() == Approx(dir3_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.mean() == Approx(dir3_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.variance() == Approx(dir3_expected_variances.at(2)).epsilon(0.01));


        SampleSummarizer<double> size1_summary = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size2_summary = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size3_summary = prior_sample.summarize<double>("pop_size_pop1b");

        REQUIRE(size1_summary.sample_size() == expected_sample_size);
        REQUIRE(size1_summary.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary.sample_size() == expected_sample_size);
        REQUIRE(size2_summary.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary.sample_size() == expected_sample_size);
        REQUIRE(size3_summary.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 3 dirichlet pairs and fully parameterized without TimeMeanSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing DPP with 3 dirichlet pairs without TimeMeanSizeRateMixer") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        std::vector<double> mult1_alpha = {10.0, 10.0, 10.0};
        std::vector<double> mult2_alpha = {10.0, 15.0, 5.0};
        std::vector<double> mult3_alpha = {15.0, 5.0, 10.0};
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t502.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t502-state-run-1.log";
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
        os << "    chain_length: 400000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 3\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MeanPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RelativePopulationSizeMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult1_alpha.at(0);
        for (unsigned int i = 1; i < mult1_alpha.size(); ++i) {
            os << ", " << mult1_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult2_alpha.at(0);
        for (unsigned int i = 1; i < mult2_alpha.size(); ++i) {
            os << ", " << mult2_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult3_alpha.at(0);
        for (unsigned int i = 1; i < mult3_alpha.size(); ++i) {
            os << ", " << mult3_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "9379834";
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
        ret = ecoevolity_main<DirichletCollectionSettings, ComparisonDirichletPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        unsigned int nnodes = 3;
        DirichletDistribution dir_dist1 = DirichletDistribution(mult1_alpha);
        std::vector<double> dir1_expected_means = dir_dist1.get_mean();
        for (unsigned int i = 0; i < dir1_expected_means.size(); ++i) {
            dir1_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir1_expected_variances = dir_dist1.get_variance();
        for (unsigned int i = 0; i < dir1_expected_variances.size(); ++i) {
            dir1_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist2 = DirichletDistribution(mult2_alpha);
        std::vector<double> dir2_expected_means = dir_dist2.get_mean();
        for (unsigned int i = 0; i < dir2_expected_means.size(); ++i) {
            dir2_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir2_expected_variances = dir_dist2.get_variance();
        for (unsigned int i = 0; i < dir2_expected_variances.size(); ++i) {
            dir2_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist3 = DirichletDistribution(mult3_alpha);
        std::vector<double> dir3_expected_means = dir_dist3.get_mean();
        for (unsigned int i = 0; i < dir3_expected_means.size(); ++i) {
            dir3_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir3_expected_variances = dir_dist3.get_variance();
        for (unsigned int i = 0; i < dir3_expected_variances.size(); ++i) {
            dir3_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 40001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size_mult1_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_kya");
        SampleSummarizer<double> size_mult1_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_fas");
        SampleSummarizer<double> size_mult1_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_kya");
        SampleSummarizer<double> size_mult2_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1");
        SampleSummarizer<double> size_mult2_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2");
        SampleSummarizer<double> size_mult2_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1");
        SampleSummarizer<double> size_mult3_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1b");
        SampleSummarizer<double> size_mult3_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2b");
        SampleSummarizer<double> size_mult3_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1b");

        std::cout << "\n";
        std::cout << "kya: "  << size_mult1_summary_0.mean() << "\n";
        std::cout << "fas: "  << size_mult1_summary_1.mean() << "\n";
        std::cout << "root: " << size_mult1_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult1_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_0.mean() == Approx(dir1_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_0.variance() == Approx(dir1_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.mean() == Approx(dir1_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.variance() == Approx(dir1_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_2.mean() == Approx(dir1_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult1_summary_2.variance() == Approx(dir1_expected_variances.at(2)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1: " << size_mult2_summary_0.mean() << "\n";
        std::cout << "pop2: " << size_mult2_summary_1.mean() << "\n";
        std::cout << "root: " << size_mult2_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult2_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_0.mean() == Approx(dir2_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_0.variance() == Approx(dir2_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.mean() == Approx(dir2_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.variance() == Approx(dir2_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_2.mean() == Approx(dir2_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult2_summary_2.variance() == Approx(dir2_expected_variances.at(2)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1b: " << size_mult3_summary_0.mean() << "\n";
        std::cout << "pop2b: " << size_mult3_summary_1.mean() << "\n";
        std::cout << "root: "  << size_mult3_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult3_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_0.mean() == Approx(dir3_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_0.variance() == Approx(dir3_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.mean() == Approx(dir3_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.variance() == Approx(dir3_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.mean() == Approx(dir3_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.variance() == Approx(dir3_expected_variances.at(2)).epsilon(0.01));


        SampleSummarizer<double> size1_summary = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size2_summary = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size3_summary = prior_sample.summarize<double>("pop_size_pop1b");

        REQUIRE(size1_summary.sample_size() == expected_sample_size);
        REQUIRE(size1_summary.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary.sample_size() == expected_sample_size);
        REQUIRE(size2_summary.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary.sample_size() == expected_sample_size);
        REQUIRE(size3_summary.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing DPP with 3 dirichlet pairs and fully parameterized with only TimeMeanSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing DPP with 3 dirichlet pairs with only TimeMeanSizeRateMixer") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        std::vector<double> mult1_alpha = {10.0, 10.0, 10.0};
        std::vector<double> mult2_alpha = {10.0, 15.0, 5.0};
        std::vector<double> mult3_alpha = {15.0, 5.0, 10.0};
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t503.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t503-state-run-1.log";
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
        os << "    chain_length: 400000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 3\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult1_alpha.at(0);
        for (unsigned int i = 1; i < mult1_alpha.size(); ++i) {
            os << ", " << mult1_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult2_alpha.at(0);
        for (unsigned int i = 1; i < mult2_alpha.size(); ++i) {
            os << ", " << mult2_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult3_alpha.at(0);
        for (unsigned int i = 1; i < mult3_alpha.size(); ++i) {
            os << ", " << mult3_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "93745873";
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
        ret = ecoevolity_main<DirichletCollectionSettings, ComparisonDirichletPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        unsigned int nnodes = 3;
        DirichletDistribution dir_dist1 = DirichletDistribution(mult1_alpha);
        std::vector<double> dir1_expected_means = dir_dist1.get_mean();
        for (unsigned int i = 0; i < dir1_expected_means.size(); ++i) {
            dir1_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir1_expected_variances = dir_dist1.get_variance();
        for (unsigned int i = 0; i < dir1_expected_variances.size(); ++i) {
            dir1_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist2 = DirichletDistribution(mult2_alpha);
        std::vector<double> dir2_expected_means = dir_dist2.get_mean();
        for (unsigned int i = 0; i < dir2_expected_means.size(); ++i) {
            dir2_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir2_expected_variances = dir_dist2.get_variance();
        for (unsigned int i = 0; i < dir2_expected_variances.size(); ++i) {
            dir2_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist3 = DirichletDistribution(mult3_alpha);
        std::vector<double> dir3_expected_means = dir_dist3.get_mean();
        for (unsigned int i = 0; i < dir3_expected_means.size(); ++i) {
            dir3_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir3_expected_variances = dir_dist3.get_variance();
        for (unsigned int i = 0; i < dir3_expected_variances.size(); ++i) {
            dir3_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 40001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size_mult1_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_kya");
        SampleSummarizer<double> size_mult1_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_fas");
        SampleSummarizer<double> size_mult1_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_kya");
        SampleSummarizer<double> size_mult2_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1");
        SampleSummarizer<double> size_mult2_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2");
        SampleSummarizer<double> size_mult2_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1");
        SampleSummarizer<double> size_mult3_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1b");
        SampleSummarizer<double> size_mult3_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2b");
        SampleSummarizer<double> size_mult3_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1b");

        std::cout << "\n";
        std::cout << "kya: "  << size_mult1_summary_0.mean() << "\n";
        std::cout << "fas: "  << size_mult1_summary_1.mean() << "\n";
        std::cout << "root: " << size_mult1_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult1_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_0.mean() == Approx(dir1_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_0.variance() == Approx(dir1_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.mean() == Approx(dir1_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.variance() == Approx(dir1_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_2.mean() == Approx(dir1_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult1_summary_2.variance() == Approx(dir1_expected_variances.at(2)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1: " << size_mult2_summary_0.mean() << "\n";
        std::cout << "pop2: " << size_mult2_summary_1.mean() << "\n";
        std::cout << "root: " << size_mult2_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult2_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_0.mean() == Approx(dir2_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_0.variance() == Approx(dir2_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.mean() == Approx(dir2_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.variance() == Approx(dir2_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_2.mean() == Approx(dir2_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult2_summary_2.variance() == Approx(dir2_expected_variances.at(2)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1b: " << size_mult3_summary_0.mean() << "\n";
        std::cout << "pop2b: " << size_mult3_summary_1.mean() << "\n";
        std::cout << "root: "  << size_mult3_summary_2.mean() << "\n";
        std::cout << "\n";
        REQUIRE(size_mult3_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_0.mean() == Approx(dir3_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_0.variance() == Approx(dir3_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.mean() == Approx(dir3_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.variance() == Approx(dir3_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.mean() == Approx(dir3_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.variance() == Approx(dir3_expected_variances.at(2)).epsilon(0.01));


        SampleSummarizer<double> size1_summary = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size2_summary = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size3_summary = prior_sample.summarize<double>("pop_size_pop1b");

        REQUIRE(size1_summary.sample_size() == expected_sample_size);
        REQUIRE(size1_summary.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary.sample_size() == expected_sample_size);
        REQUIRE(size2_summary.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary.sample_size() == expected_sample_size);
        REQUIRE(size3_summary.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.367).epsilon(0.01));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.163).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 2 dirichlet singletons, 1 dirichlet pair, and fully parameterized",
        "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 2 singletons and a pair") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        std::vector<double> mult1_alpha = {10.0, 10.0};
        std::vector<double> mult2_alpha = {10.0, 15.0};
        std::vector<double> mult3_alpha = {15.0, 5.0, 10.0};
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t504.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t504-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 800000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MeanPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RelativePopulationSizeMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult1_alpha.at(0);
        for (unsigned int i = 1; i < mult1_alpha.size(); ++i) {
            os << ", " << mult1_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult2_alpha.at(0);
        for (unsigned int i = 1; i < mult2_alpha.size(); ++i) {
            os << ", " << mult2_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult3_alpha.at(0);
        for (unsigned int i = 1; i < mult3_alpha.size(); ++i) {
            os << ", " << mult3_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "68412635";
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
        ret = ecoevolity_main<DirichletCollectionSettings, ComparisonDirichletPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int nnodes = 2;
        DirichletDistribution dir_dist1 = DirichletDistribution(mult1_alpha);
        std::vector<double> dir1_expected_means = dir_dist1.get_mean();
        for (unsigned int i = 0; i < dir1_expected_means.size(); ++i) {
            dir1_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir1_expected_variances = dir_dist1.get_variance();
        for (unsigned int i = 0; i < dir1_expected_variances.size(); ++i) {
            dir1_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist2 = DirichletDistribution(mult2_alpha);
        std::vector<double> dir2_expected_means = dir_dist2.get_mean();
        for (unsigned int i = 0; i < dir2_expected_means.size(); ++i) {
            dir2_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir2_expected_variances = dir_dist2.get_variance();
        for (unsigned int i = 0; i < dir2_expected_variances.size(); ++i) {
            dir2_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist3 = DirichletDistribution(mult3_alpha);
        std::vector<double> dir3_expected_means = dir_dist3.get_mean();
        for (unsigned int i = 0; i < dir3_expected_means.size(); ++i) {
            dir3_expected_means.at(i) *= 3.0;
        }
        std::vector<double> dir3_expected_variances = dir_dist3.get_variance();
        for (unsigned int i = 0; i < dir3_expected_variances.size(); ++i) {
            dir3_expected_variances.at(i) *= (9.0);
        }

        unsigned int expected_sample_size = 80001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size_mult1_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_kya");
        SampleSummarizer<double> size_mult1_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_root_kya");
        SampleSummarizer<double> size_mult2_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1");
        SampleSummarizer<double> size_mult2_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1");
        SampleSummarizer<double> size_mult3_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1b");
        SampleSummarizer<double> size_mult3_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2b");
        SampleSummarizer<double> size_mult3_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1b");

        std::cout << "\n";
        std::cout << "kya: "  << size_mult1_summary_0.mean() << "\n";
        std::cout << "root: " << size_mult1_summary_1.mean() << "\n";
        REQUIRE(size_mult1_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_0.mean() == Approx(dir1_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_0.variance() == Approx(dir1_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.mean() == Approx(dir1_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.variance() == Approx(dir1_expected_variances.at(1)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1: " << size_mult2_summary_0.mean() << "\n";
        std::cout << "root: " << size_mult2_summary_1.mean() << "\n";
        REQUIRE(size_mult2_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_0.mean() == Approx(dir2_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_0.variance() == Approx(dir2_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.mean() == Approx(dir2_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.variance() == Approx(dir2_expected_variances.at(1)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1b: " << size_mult3_summary_0.mean() << "\n";
        std::cout << "pop2b: " << size_mult3_summary_1.mean() << "\n";
        std::cout << "root: "  << size_mult3_summary_2.mean() << "\n";
        REQUIRE(size_mult3_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_0.mean() == Approx(dir3_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_0.variance() == Approx(dir3_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.mean() == Approx(dir3_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.variance() == Approx(dir3_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.mean() == Approx(dir3_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.variance() == Approx(dir3_expected_variances.at(2)).epsilon(0.01));


        SampleSummarizer<double> size1_summary = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size2_summary = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size3_summary = prior_sample.summarize<double>("pop_size_pop1b");

        REQUIRE(size1_summary.sample_size() == expected_sample_size);
        REQUIRE(size1_summary.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary.sample_size() == expected_sample_size);
        REQUIRE(size2_summary.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary.sample_size() == expected_sample_size);
        REQUIRE(size3_summary.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.01));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 2 dirichlet singletons, 1 dirichlet pair, and fully parameterized with no TimeMeanSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 2 singletons and a pair and no TimeMeanSizeRateMixer") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        std::vector<double> mult1_alpha = {10.0, 10.0};
        std::vector<double> mult2_alpha = {10.0, 15.0};
        std::vector<double> mult3_alpha = {15.0, 5.0, 10.0};
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t505.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t505-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 600000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        MeanPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RelativePopulationSizeMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult1_alpha.at(0);
        for (unsigned int i = 1; i < mult1_alpha.size(); ++i) {
            os << ", " << mult1_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult2_alpha.at(0);
        for (unsigned int i = 1; i < mult2_alpha.size(); ++i) {
            os << ", " << mult2_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult3_alpha.at(0);
        for (unsigned int i = 1; i < mult3_alpha.size(); ++i) {
            os << ", " << mult3_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "27457849";
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
        ret = ecoevolity_main<DirichletCollectionSettings, ComparisonDirichletPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int nnodes = 2;
        DirichletDistribution dir_dist1 = DirichletDistribution(mult1_alpha);
        std::vector<double> dir1_expected_means = dir_dist1.get_mean();
        for (unsigned int i = 0; i < dir1_expected_means.size(); ++i) {
            dir1_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir1_expected_variances = dir_dist1.get_variance();
        for (unsigned int i = 0; i < dir1_expected_variances.size(); ++i) {
            dir1_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist2 = DirichletDistribution(mult2_alpha);
        std::vector<double> dir2_expected_means = dir_dist2.get_mean();
        for (unsigned int i = 0; i < dir2_expected_means.size(); ++i) {
            dir2_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir2_expected_variances = dir_dist2.get_variance();
        for (unsigned int i = 0; i < dir2_expected_variances.size(); ++i) {
            dir2_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist3 = DirichletDistribution(mult3_alpha);
        std::vector<double> dir3_expected_means = dir_dist3.get_mean();
        for (unsigned int i = 0; i < dir3_expected_means.size(); ++i) {
            dir3_expected_means.at(i) *= 3.0;
        }
        std::vector<double> dir3_expected_variances = dir_dist3.get_variance();
        for (unsigned int i = 0; i < dir3_expected_variances.size(); ++i) {
            dir3_expected_variances.at(i) *= (9.0);
        }

        unsigned int expected_sample_size = 60001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size_mult1_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_kya");
        SampleSummarizer<double> size_mult1_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_root_kya");
        SampleSummarizer<double> size_mult2_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1");
        SampleSummarizer<double> size_mult2_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1");
        SampleSummarizer<double> size_mult3_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1b");
        SampleSummarizer<double> size_mult3_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2b");
        SampleSummarizer<double> size_mult3_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1b");

        std::cout << "\n";
        std::cout << "kya: "  << size_mult1_summary_0.mean() << "\n";
        std::cout << "root: " << size_mult1_summary_1.mean() << "\n";
        REQUIRE(size_mult1_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_0.mean() == Approx(dir1_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_0.variance() == Approx(dir1_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.mean() == Approx(dir1_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.variance() == Approx(dir1_expected_variances.at(1)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1: " << size_mult2_summary_0.mean() << "\n";
        std::cout << "root: " << size_mult2_summary_1.mean() << "\n";
        REQUIRE(size_mult2_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_0.mean() == Approx(dir2_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_0.variance() == Approx(dir2_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.mean() == Approx(dir2_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.variance() == Approx(dir2_expected_variances.at(1)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1b: " << size_mult3_summary_0.mean() << "\n";
        std::cout << "pop2b: " << size_mult3_summary_1.mean() << "\n";
        std::cout << "root: "  << size_mult3_summary_2.mean() << "\n";
        REQUIRE(size_mult3_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_0.mean() == Approx(dir3_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_0.variance() == Approx(dir3_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.mean() == Approx(dir3_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.variance() == Approx(dir3_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.mean() == Approx(dir3_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.variance() == Approx(dir3_expected_variances.at(2)).epsilon(0.01));


        SampleSummarizer<double> size1_summary = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size2_summary = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size3_summary = prior_sample.summarize<double>("pop_size_pop1b");

        REQUIRE(size1_summary.sample_size() == expected_sample_size);
        REQUIRE(size1_summary.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary.sample_size() == expected_sample_size);
        REQUIRE(size2_summary.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary.sample_size() == expected_sample_size);
        REQUIRE(size3_summary.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.01));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing ReversibleJumpSampler with 2 dirichlet singletons, 1 dirichlet pair, and fully parameterized with only TimeMeanSizeRateMixer",
        "[SamplingPrior]") {

    SECTION("Testing rjMCMC with 2 singletons and a pair and only TimeMeanSizeRateMixer") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        std::vector<double> mult1_alpha = {10.0, 10.0};
        std::vector<double> mult2_alpha = {10.0, 15.0};
        std::vector<double> mult3_alpha = {15.0, 5.0, 10.0};
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t506.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t506-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    uniform:\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 400000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        ModelOperator:\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult1_alpha.at(0);
        for (unsigned int i = 1; i < mult1_alpha.size(); ++i) {
            os << ", " << mult1_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult2_alpha.at(0);
        for (unsigned int i = 1; i < mult2_alpha.size(); ++i) {
            os << ", " << mult2_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        population_size_multipliers:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                dirichlet_distribution:\n";
        os << "                    alpha: [" << mult3_alpha.at(0);
        for (unsigned int i = 1; i < mult3_alpha.size(); ++i) {
            os << ", " << mult3_alpha.at(i);
        }
        os << "]\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "68412635";
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
        ret = ecoevolity_main<DirichletCollectionSettings, ComparisonDirichletPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int nnodes = 2;
        DirichletDistribution dir_dist1 = DirichletDistribution(mult1_alpha);
        std::vector<double> dir1_expected_means = dir_dist1.get_mean();
        for (unsigned int i = 0; i < dir1_expected_means.size(); ++i) {
            dir1_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir1_expected_variances = dir_dist1.get_variance();
        for (unsigned int i = 0; i < dir1_expected_variances.size(); ++i) {
            dir1_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist2 = DirichletDistribution(mult2_alpha);
        std::vector<double> dir2_expected_means = dir_dist2.get_mean();
        for (unsigned int i = 0; i < dir2_expected_means.size(); ++i) {
            dir2_expected_means.at(i) *= (double)nnodes;
        }
        std::vector<double> dir2_expected_variances = dir_dist2.get_variance();
        for (unsigned int i = 0; i < dir2_expected_variances.size(); ++i) {
            dir2_expected_variances.at(i) *= ((double)nnodes * (double)nnodes);
        }

        DirichletDistribution dir_dist3 = DirichletDistribution(mult3_alpha);
        std::vector<double> dir3_expected_means = dir_dist3.get_mean();
        for (unsigned int i = 0; i < dir3_expected_means.size(); ++i) {
            dir3_expected_means.at(i) *= 3.0;
        }
        std::vector<double> dir3_expected_variances = dir_dist3.get_variance();
        for (unsigned int i = 0; i < dir3_expected_variances.size(); ++i) {
            dir3_expected_variances.at(i) *= (9.0);
        }

        unsigned int expected_sample_size = 40001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size_mult1_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_kya");
        SampleSummarizer<double> size_mult1_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_root_kya");
        SampleSummarizer<double> size_mult2_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1");
        SampleSummarizer<double> size_mult2_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1");
        SampleSummarizer<double> size_mult3_summary_0 = prior_sample.summarize<double>("pop_size_multiplier_pop1b");
        SampleSummarizer<double> size_mult3_summary_1 = prior_sample.summarize<double>("pop_size_multiplier_pop2b");
        SampleSummarizer<double> size_mult3_summary_2 = prior_sample.summarize<double>("pop_size_multiplier_root_pop1b");

        std::cout << "\n";
        std::cout << "kya: "  << size_mult1_summary_0.mean() << "\n";
        std::cout << "root: " << size_mult1_summary_1.mean() << "\n";
        REQUIRE(size_mult1_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult1_summary_0.mean() == Approx(dir1_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_0.variance() == Approx(dir1_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.mean() == Approx(dir1_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult1_summary_1.variance() == Approx(dir1_expected_variances.at(1)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1: " << size_mult2_summary_0.mean() << "\n";
        std::cout << "root: " << size_mult2_summary_1.mean() << "\n";
        REQUIRE(size_mult2_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult2_summary_0.mean() == Approx(dir2_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_0.variance() == Approx(dir2_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.mean() == Approx(dir2_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult2_summary_1.variance() == Approx(dir2_expected_variances.at(1)).epsilon(0.01));

        std::cout << "\n";
        std::cout << "pop1b: " << size_mult3_summary_0.mean() << "\n";
        std::cout << "pop2b: " << size_mult3_summary_1.mean() << "\n";
        std::cout << "root: "  << size_mult3_summary_2.mean() << "\n";
        REQUIRE(size_mult3_summary_0.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_1.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_2.sample_size() == expected_sample_size);
        REQUIRE(size_mult3_summary_0.mean() == Approx(dir3_expected_means.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_0.variance() == Approx(dir3_expected_variances.at(0)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.mean() == Approx(dir3_expected_means.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_1.variance() == Approx(dir3_expected_variances.at(1)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.mean() == Approx(dir3_expected_means.at(2)).epsilon(0.01));
        REQUIRE(size_mult3_summary_2.variance() == Approx(dir3_expected_variances.at(2)).epsilon(0.01));


        SampleSummarizer<double> size1_summary = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size2_summary = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size3_summary = prior_sample.summarize<double>("pop_size_pop1b");

        REQUIRE(size1_summary.sample_size() == expected_sample_size);
        REQUIRE(size1_summary.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary.sample_size() == expected_sample_size);
        REQUIRE(size2_summary.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary.sample_size() == expected_sample_size);
        REQUIRE(size3_summary.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        REQUIRE_THROWS(prior_sample.summarize<double>("concentration"));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(1.0/bell_float(3)).epsilon(0.01));
        }
        for (auto const & kv: nevent_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(stirling2_float(3, kv.first)/bell_float(3)).epsilon(0.005));
        }

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}


TEST_CASE("Testing relative root models with with mix of 5 comparisons", "[SamplingPrior]") {

    SECTION("Testing relative root models") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double rel_size_shape_0 = 20.0;
        double rel_size_scale_0 = 0.05;
        double rel_size_shape_2 = 100.0;
        double rel_size_scale_2 = 0.02;
        double rel_size_shape_4 = 10.0;
        double rel_size_scale_4 = 0.05;
        double size_shape_0 = 10.0;
        double size_scale_0 = 0.0001;
        double size_shape_1 = 5.0;
        double size_scale_1 = 0.001;
        double size_shape_3 = 20.0;
        double size_scale_3 = 0.0005;
        double size_shape_4 = 4.0;
        double size_scale_4 = 0.002;
        double f_a_0 = 2.0;
        double f_b_0 = 1.1;
        double f_a_1 = 1.0;
        double f_b_1 = 0.5;
        double f_a_2 = 1.5;
        double f_b_2 = 1.8;
        double f_a_3 = 1.0;
        double f_b_3 = 1.0;
        double f_a_4 = 2.0;
        double f_b_4 = 2.0;
        
        std::shared_ptr<BetaDistribution> f_prior_0 = std::make_shared<BetaDistribution>(f_a_0, f_b_0);
        std::shared_ptr<BetaDistribution> f_prior_1 = std::make_shared<BetaDistribution>(f_a_1, f_b_1);
        std::shared_ptr<BetaDistribution> f_prior_2 = std::make_shared<BetaDistribution>(f_a_2, f_b_2);
        std::shared_ptr<BetaDistribution> f_prior_3 = std::make_shared<BetaDistribution>(f_a_3, f_b_3);
        std::shared_ptr<BetaDistribution> f_prior_4 = std::make_shared<BetaDistribution>(f_a_4, f_b_4);

        double mu_shape_1 = 100.0;
        double mu_scale_1 = 0.005;
        double mu_shape_2 = 100.0;
        double mu_scale_2 = 0.02;
        double mu_shape_3 = 50.0;
        double mu_scale_3 = 0.02;
        double mu_shape_4 = 20.0;
        double mu_scale_4 = 0.05;
        double concentration = 1.5;

        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-t507.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-t507-state-run-1.log";
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
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 100\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 2\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape_0 << "\n";
        os << "                    scale: " << size_scale_0 << "\n";
        os << "        root_relative_population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << rel_size_shape_0 << "\n";
        os << "                    scale: " << rel_size_scale_0 << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a_0 << "\n";
        os << "                    beta: " << f_b_0 << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape_1 << "\n";
        os << "                    scale: " << size_scale_1 << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a_1 << "\n";
        os << "                    beta: " << f_b_1 << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mu_shape_1 << "\n";
        os << "                    scale: " << mu_scale_1 << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        root_relative_population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << rel_size_shape_2 << "\n";
        os << "                    scale: " << rel_size_scale_2 << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a_2 << "\n";
        os << "                    beta: " << f_b_2 << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mu_shape_2 << "\n";
        os << "                    scale: " << mu_scale_2 << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname3.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape_3 << "\n";
        os << "                    scale: " << size_scale_3 << "\n";
        os << "        root_relative_population_size:\n";
        os << "            value: 2.0\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a_3 << "\n";
        os << "                    beta: " << f_b_3 << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mu_shape_3 << "\n";
        os << "                    scale: " << mu_scale_3 << "\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname4-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size_shape_4 << "\n";
        os << "                    scale: " << size_scale_4 << "\n";
        os << "        root_relative_population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << rel_size_shape_4 << "\n";
        os << "                    scale: " << rel_size_scale_4 << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f_a_4 << "\n";
        os << "                    beta: " << f_b_4 << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mu_shape_4 << "\n";
        os << "                    scale: " << mu_scale_4 << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "7294828";
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
        ret = ecoevolity_main<RelativeRootCollectionSettings, ComparisonRelativeRootPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary_0 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary_1 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary_2 = prior_sample.summarize<double>("root_height_pop1b");
        SampleSummarizer<double> height_summary_3 = prior_sample.summarize<double>("root_height_pop1c");
        SampleSummarizer<double> height_summary_4 = prior_sample.summarize<double>("root_height_pop1d");

        REQUIRE(height_summary_0.sample_size() == expected_sample_size);
        REQUIRE(height_summary_0.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary_0.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary_1.sample_size() == expected_sample_size);
        REQUIRE(height_summary_1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary_1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary_2.sample_size() == expected_sample_size);
        REQUIRE(height_summary_2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary_2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary_3.sample_size() == expected_sample_size);
        REQUIRE(height_summary_3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary_3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary_4.sample_size() == expected_sample_size);
        REQUIRE(height_summary_4.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary_4.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size_0_summary_leaf0 = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size_0_summary_leaf1 = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size_0_summary_root;
        std::vector<double> leaf0_sizes = prior_sample.get<double>("pop_size_kya");
        std::vector<double> leaf1_sizes = prior_sample.get<double>("pop_size_fas");
        std::vector<double> root_sizes = prior_sample.get<double>("pop_size_root_kya");
        for (unsigned int i = 0; i < root_sizes.size(); ++i) {
            double leaf_mean = (leaf0_sizes.at(i) + leaf1_sizes.at(i)) / 2.0;
            double rel_root_size = root_sizes.at(i) / leaf_mean;
            size_0_summary_root.add_sample(rel_root_size);
        }

        SampleSummarizer<double> size_1_summary_leaf0 = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size_1_summary_leaf1 = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size_1_summary_root;
        leaf0_sizes = prior_sample.get<double>("pop_size_pop1");
        leaf1_sizes = prior_sample.get<double>("pop_size_pop2");
        root_sizes = prior_sample.get<double>("pop_size_root_pop1");
        for (unsigned int i = 0; i < root_sizes.size(); ++i) {
            double leaf_mean = (leaf0_sizes.at(i) + leaf1_sizes.at(i)) / 2.0;
            double rel_root_size = root_sizes.at(i) / leaf_mean;
            size_1_summary_root.add_sample(rel_root_size);
        }

        SampleSummarizer<double> size_2_summary_leaf0 = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size_2_summary_leaf1 = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size_2_summary_root;
        leaf0_sizes = prior_sample.get<double>("pop_size_pop1b");
        leaf1_sizes = prior_sample.get<double>("pop_size_pop2b");
        root_sizes = prior_sample.get<double>("pop_size_root_pop1b");
        for (unsigned int i = 0; i < root_sizes.size(); ++i) {
            double leaf_mean = (leaf0_sizes.at(i) + leaf1_sizes.at(i)) / 2.0;
            double rel_root_size = root_sizes.at(i) / leaf_mean;
            size_2_summary_root.add_sample(rel_root_size);
        }

        SampleSummarizer<double> size_3_summary_leaf0 = prior_sample.summarize<double>("pop_size_pop1c");
        SampleSummarizer<double> size_3_summary_leaf1 = prior_sample.summarize<double>("pop_size_pop2c");
        SampleSummarizer<double> size_3_summary_root;
        leaf0_sizes = prior_sample.get<double>("pop_size_pop1c");
        leaf1_sizes = prior_sample.get<double>("pop_size_pop2c");
        root_sizes = prior_sample.get<double>("pop_size_root_pop1c");
        for (unsigned int i = 0; i < root_sizes.size(); ++i) {
            double leaf_mean = (leaf0_sizes.at(i) + leaf1_sizes.at(i)) / 2.0;
            double rel_root_size = root_sizes.at(i) / leaf_mean;
            size_3_summary_root.add_sample(rel_root_size);
        }

        SampleSummarizer<double> size_4_summary_leaf0 = prior_sample.summarize<double>("pop_size_pop1d");
        SampleSummarizer<double> size_4_summary_root;
        leaf0_sizes = prior_sample.get<double>("pop_size_pop1d");
        root_sizes = prior_sample.get<double>("pop_size_root_pop1d");
        for (unsigned int i = 0; i < root_sizes.size(); ++i) {
            double leaf_mean = leaf0_sizes.at(i);
            double rel_root_size = root_sizes.at(i) / leaf_mean;
            size_4_summary_root.add_sample(rel_root_size);
        }

        REQUIRE(size_0_summary_leaf0.sample_size() == expected_sample_size);
        REQUIRE(size_0_summary_leaf1.sample_size() == expected_sample_size);
        REQUIRE(size_0_summary_root.sample_size() == expected_sample_size);
        REQUIRE(size_0_summary_leaf0.mean() == Approx(size_shape_0 * size_scale_0).epsilon(0.01));
        REQUIRE(size_0_summary_leaf0.variance() == Approx(size_shape_0 * size_scale_0 * size_scale_0).epsilon(0.01));
        REQUIRE(size_0_summary_leaf1.mean() == Approx(size_shape_0 * size_scale_0).epsilon(0.01));
        REQUIRE(size_0_summary_leaf1.variance() == Approx(size_shape_0 * size_scale_0 * size_scale_0).epsilon(0.01));
        REQUIRE(size_0_summary_root.mean() == Approx(rel_size_shape_0 * rel_size_scale_0).epsilon(0.01));
        REQUIRE(size_0_summary_root.variance() == Approx(rel_size_shape_0 * rel_size_scale_0 * rel_size_scale_0).epsilon(0.01));

        REQUIRE(size_1_summary_leaf0.sample_size() == expected_sample_size);
        REQUIRE(size_1_summary_leaf1.sample_size() == expected_sample_size);
        REQUIRE(size_1_summary_root.sample_size() == expected_sample_size);
        REQUIRE(size_1_summary_leaf0.mean() == Approx(size_shape_1 * size_scale_1).epsilon(0.01));
        REQUIRE(size_1_summary_leaf0.variance() == Approx(size_shape_1 * size_scale_1 * size_scale_1).epsilon(0.01));
        REQUIRE(size_1_summary_leaf1.mean() == Approx(size_shape_1 * size_scale_1).epsilon(0.01));
        REQUIRE(size_1_summary_leaf1.variance() == Approx(size_shape_1 * size_scale_1 * size_scale_1).epsilon(0.01));
        REQUIRE(size_1_summary_root.mean() == Approx(1.0));
        REQUIRE(size_1_summary_root.variance() == Approx(0.0));

        REQUIRE(size_2_summary_leaf0.sample_size() == expected_sample_size);
        REQUIRE(size_2_summary_leaf1.sample_size() == expected_sample_size);
        REQUIRE(size_2_summary_root.sample_size() == expected_sample_size);
        REQUIRE(size_2_summary_leaf0.mean() == Approx(0.005));
        REQUIRE(size_2_summary_leaf0.variance() == Approx(0.0));
        REQUIRE(size_2_summary_leaf1.mean() == Approx(0.005));
        REQUIRE(size_2_summary_leaf1.variance() == Approx(0.0));
        REQUIRE(size_2_summary_root.mean() == Approx(rel_size_shape_2 * rel_size_scale_2).epsilon(0.01));
        REQUIRE(size_2_summary_root.variance() == Approx(rel_size_shape_2 * rel_size_scale_2 * rel_size_scale_2).epsilon(0.01));

        REQUIRE(size_3_summary_leaf0.sample_size() == expected_sample_size);
        REQUIRE(size_3_summary_leaf1.sample_size() == expected_sample_size);
        REQUIRE(size_3_summary_root.sample_size() == expected_sample_size);
        REQUIRE(size_3_summary_leaf0.mean() == Approx(size_shape_3 * size_scale_3).epsilon(0.01));
        REQUIRE(size_3_summary_leaf0.variance() == Approx(size_shape_3 * size_scale_3 * size_scale_3).epsilon(0.01));
        REQUIRE(size_3_summary_leaf1.mean() == Approx(size_shape_3 * size_scale_3).epsilon(0.01));
        REQUIRE(size_3_summary_leaf1.variance() == Approx(size_shape_3 * size_scale_3 * size_scale_3).epsilon(0.01));
        REQUIRE(size_3_summary_root.mean() == Approx(2.0));
        REQUIRE(size_3_summary_root.variance() == Approx(0.0));

        REQUIRE(size_4_summary_leaf0.sample_size() == expected_sample_size);
        REQUIRE(size_4_summary_root.sample_size() == expected_sample_size);
        REQUIRE(size_4_summary_leaf0.mean() == Approx(size_shape_4 * size_scale_4).epsilon(0.01));
        REQUIRE(size_4_summary_leaf0.variance() == Approx(size_shape_4 * size_scale_4 * size_scale_4).epsilon(0.01));
        REQUIRE(size_4_summary_root.mean() == Approx(rel_size_shape_4 * rel_size_scale_4).epsilon(0.01));
        REQUIRE(size_4_summary_root.variance() == Approx(rel_size_shape_4 * rel_size_scale_4 * rel_size_scale_4).epsilon(0.01));


        SampleSummarizer<double> f_0_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f_1_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f_2_summary = prior_sample.summarize<double>("freq_1_pop1b");
        SampleSummarizer<double> f_3_summary = prior_sample.summarize<double>("freq_1_pop1c");
        SampleSummarizer<double> f_4_summary = prior_sample.summarize<double>("freq_1_pop1d");

        REQUIRE(f_0_summary.sample_size() == expected_sample_size);
        REQUIRE(f_0_summary.mean() ==        Approx(f_prior_0->get_mean()).epsilon(0.01));
        REQUIRE(f_0_summary.variance() ==    Approx(f_prior_0->get_variance()).epsilon(0.01));

        REQUIRE(f_1_summary.sample_size() == expected_sample_size);
        REQUIRE(f_1_summary.mean() ==        Approx(f_prior_1->get_mean()).epsilon(0.01));
        REQUIRE(f_1_summary.variance() ==    Approx(f_prior_1->get_variance()).epsilon(0.01));

        REQUIRE(f_2_summary.sample_size() == expected_sample_size);
        REQUIRE(f_2_summary.mean() ==        Approx(f_prior_2->get_mean()).epsilon(0.01));
        REQUIRE(f_2_summary.variance() ==    Approx(f_prior_2->get_variance()).epsilon(0.01));

        REQUIRE(f_3_summary.sample_size() == expected_sample_size);
        REQUIRE(f_3_summary.mean() ==        Approx(f_prior_3->get_mean()).epsilon(0.01));
        REQUIRE(f_3_summary.variance() ==    Approx(f_prior_3->get_variance()).epsilon(0.01));

        REQUIRE(f_4_summary.sample_size() == expected_sample_size);
        REQUIRE(f_4_summary.mean() ==        Approx(f_prior_4->get_mean()).epsilon(0.01));
        REQUIRE(f_4_summary.variance() ==    Approx(f_prior_4->get_variance()).epsilon(0.01));

        SampleSummarizer<double> mu_0_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mu_1_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mu_2_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        SampleSummarizer<double> mu_3_summary = prior_sample.summarize<double>("mutation_rate_pop1c");
        SampleSummarizer<double> mu_4_summary = prior_sample.summarize<double>("mutation_rate_pop1d");

        REQUIRE(mu_0_summary.sample_size() == expected_sample_size);
        REQUIRE(mu_0_summary.mean() == Approx(1.0));
        REQUIRE(mu_0_summary.variance() == Approx(0.0));

        REQUIRE(mu_1_summary.sample_size() == expected_sample_size);
        REQUIRE(mu_1_summary.mean() == Approx(mu_shape_1 * mu_scale_1).epsilon(0.01));
        REQUIRE(mu_1_summary.variance() == Approx(mu_shape_1 * mu_scale_1 * mu_scale_1).epsilon(0.01));

        REQUIRE(mu_2_summary.sample_size() == expected_sample_size);
        REQUIRE(mu_2_summary.mean() == Approx(mu_shape_2 * mu_scale_2).epsilon(0.01));
        REQUIRE(mu_2_summary.variance() == Approx(mu_shape_2 * mu_scale_2 * mu_scale_2).epsilon(0.01));

        REQUIRE(mu_3_summary.sample_size() == expected_sample_size);
        REQUIRE(mu_3_summary.mean() == Approx(mu_shape_3 * mu_scale_3).epsilon(0.01));
        REQUIRE(mu_3_summary.variance() == Approx(mu_shape_3 * mu_scale_3 * mu_scale_3).epsilon(0.01));

        REQUIRE(mu_4_summary.sample_size() == expected_sample_size);
        REQUIRE(mu_4_summary.mean() == Approx(mu_shape_4 * mu_scale_4).epsilon(0.01));
        REQUIRE(mu_4_summary.variance() == Approx(mu_shape_4 * mu_scale_4 * mu_scale_4).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration));
        REQUIRE(conc_summary.variance() == Approx(0.0));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<int> event_indices4 = prior_sample.get<int>("root_height_index_pop1c");
        std::vector<int> event_indices5 = prior_sample.get<int>("root_height_index_pop1d");

        std::map<std::string, int> model_counts;
        std::map<int, int> nevent_counts;

        REQUIRE(nevents.size() == expected_sample_size);
        REQUIRE(event_indices1.size() == expected_sample_size);
        REQUIRE(event_indices2.size() == expected_sample_size);
        REQUIRE(event_indices3.size() == expected_sample_size);
        REQUIRE(event_indices4.size() == expected_sample_size);
        REQUIRE(event_indices5.size() == expected_sample_size);

        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            stream << event_indices4.at(i);
            stream << event_indices5.at(i);
            std::string model_str = stream.str();
            if (model_counts.count(model_str) < 1) {
                model_counts[model_str] = 1;
            }
            else {
                ++model_counts[model_str];
            }
            int n = nevents.at(i);
            if (nevent_counts.count(n) < 1) {
                nevent_counts[n] = 1;
            }
            else {
                ++nevent_counts[n];
            }
        }

        REQUIRE(model_counts.at("00000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("01234") == nevent_counts.at(5));

        unsigned int tally = 0;
        for (auto const & kv: model_counts) {
            tally += kv.second;
        }
        REQUIRE(tally == expected_sample_size);
        tally = 0;
        for (auto const & kv: nevent_counts) {
            tally += kv.second;
        }
        REQUIRE(tally == expected_sample_size);

        for (auto const & kv: model_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: nevent_counts) {
            std::cout << kv.first << ": " << kv.second / (double)expected_sample_size << "\n";
        }
        for (auto const & kv: model_counts) {
            REQUIRE((kv.second / (double)expected_sample_size) == Approx(std::exp(
                    get_dpp_log_prior_probability(kv.first, concentration))).epsilon(0.01));
        }

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}


TEST_CASE("Testing PYP with 2 pairs and alpha 1.0, discount 0", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 1.0, dscount 0") {
        double concentration = 1.0;
        double discount = 0.0;
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-tpyp1.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-tpyp1-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    pitman_yor_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "            discount:\n";
        os << "                value: " << discount << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");

        int nshared = 0;
        int total = 0;
        for (size_t i = 0; i < nevents.size(); ++i) {
            REQUIRE((nevents.at(i) == 1 || nevents.at(i) == 2));
            REQUIRE((event_indices1.at(i) == 0 || event_indices1.at(i) == 1));
            REQUIRE((event_indices2.at(i) == 0 || event_indices2.at(i) == 1));
            if (nevents.at(i) == 1) {
                ++nshared;
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
            }
            else {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
            }
            ++total;
        }
        REQUIRE(total == 10001);
        REQUIRE((nshared / 10001.0) == Approx(0.5).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_index_kya",
                "root_height_index_pop1"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing PYP with 2 pairs and alpha 1.0, discount 0.5", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 1.0, dscount 0.5") {
        double concentration = 1.0;
        double discount = 0.5;
        double height_shape = 10.0;
        double height_scale = 0.1;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-tpyp2.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-tpyp2-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    pitman_yor_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "            discount:\n";
        os << "                value: " << discount << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");

        int nshared = 0;
        int total = 0;
        for (size_t i = 0; i < nevents.size(); ++i) {
            REQUIRE((nevents.at(i) == 1 || nevents.at(i) == 2));
            REQUIRE((event_indices1.at(i) == 0 || event_indices1.at(i) == 1));
            REQUIRE((event_indices2.at(i) == 0 || event_indices2.at(i) == 1));
            if (nevents.at(i) == 1) {
                ++nshared;
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
            }
            else {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
            }
            ++total;
        }
        REQUIRE(total == 10001);
        REQUIRE((nshared / 10001.0) == Approx(1.0/4.0).epsilon(0.01));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_index_kya",
                "root_height_index_pop1"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing PYP with 3 pairs and alpha 1.5, discount 0.7", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 1.5, discount 0.7") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration = 1.5;
        double discount = 0.7;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-tpyp3.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-tpyp3-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    pitman_yor_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "            discount:\n";
        os << "                value: " << discount << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 100000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "1234";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == 10001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == 10001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == 10001);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == 10001);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == 10001);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        for (std::string m: {"000", "001", "010", "011", "012"}) {
            REQUIRE((model_counts.at(m) / 10001.0) == Approx(std::exp(
                    get_pyp_log_prior_probability(m, concentration, discount))).epsilon(0.01));
        }

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        std::vector<std::string> columns_to_ignore = {
                "generation",
                "ln_prior",
                "ln_prior_kya",
                "ln_prior_pop1",
                "number_of_events",
                "root_height_kya",
                "root_height_pop1",
                "root_height_pop1b",
                "root_height_index_kya",
                "root_height_index_pop1",
                "root_height_index_pop1b"
        };
        SampleSummarizer<double> summary;
        for (auto const &kv: prior_sample.get_data()) {
            bool test = true;
            for (auto const &p: columns_to_ignore) {
                if (kv.first == p) {
                    test = false;
                }
            }
            if (test) {
                summary = prior_sample.summarize<double>(kv.first);
                REQUIRE(summary.variance() == 0.0);
            }
        }

        delete[] cfg_path;
    }
}

TEST_CASE("Testing PYP with 6 pairs and alpha 1.7, discount 0.2", "[SamplingPrior]") {

    SECTION("Testing alpha fixed to 1.7, discount 0.2") {
        double height_shape = 10.0;
        double height_scale = 0.1;
        double concentration = 1.7;
        double discount = 0.2;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-tpyp4.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-tpyp4-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    pitman_yor_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                value: " << concentration << "\n";
        os << "                estimate: false\n";
        os << "            discount:\n";
        os << "                value: " << discount << "\n";
        os << "                estimate: false\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 1000000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.3\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 0.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: true\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            value: 0.005\n";
        os << "            estimate: false\n";
        os << "        freq_1:\n";
        os << "            value: 0.5\n";
        os << "            estimate: false\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname2.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname3.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname4.nex\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname5.nex\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "7654321";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        SampleSummarizer<double> height_summary4 = prior_sample.summarize<double>("root_height_pop1c");
        SampleSummarizer<double> height_summary5 = prior_sample.summarize<double>("root_height_pop1d");
        SampleSummarizer<double> height_summary6 = prior_sample.summarize<double>("root_height_pop1e");
        REQUIRE(height_summary1.sample_size() == 100001);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary2.sample_size() == 100001);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary3.sample_size() == 100001);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary4.sample_size() == 100001);
        REQUIRE(height_summary4.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary4.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary5.sample_size() == 100001);
        REQUIRE(height_summary5.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary5.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));
        REQUIRE(height_summary6.sample_size() == 100001);
        REQUIRE(height_summary6.mean() == Approx(height_shape * height_scale).epsilon(0.001));
        REQUIRE(height_summary6.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.002));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<int> event_indices4 = prior_sample.get<int>("root_height_index_pop1c");
        std::vector<int> event_indices5 = prior_sample.get<int>("root_height_index_pop1d");
        std::vector<int> event_indices6 = prior_sample.get<int>("root_height_index_pop1e");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");
        std::vector<double> heights4 = prior_sample.get<double>("root_height_pop1c");
        std::vector<double> heights5 = prior_sample.get<double>("root_height_pop1d");
        std::vector<double> heights6 = prior_sample.get<double>("root_height_pop1e");

        std::map<std::string, int> model_counts;
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0},
                {4, 0},
                {5, 0},
                {6, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            stream << event_indices4.at(i);
            stream << event_indices5.at(i);
            stream << event_indices6.at(i);
            std::string model_str = stream.str();
            if (model_counts.count(model_str) < 1) {
                model_counts[model_str] = 1;
            }
            else {
                ++model_counts[model_str];
            }
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(event_indices1.at(i) == event_indices4.at(i));
                REQUIRE(event_indices1.at(i) == event_indices5.at(i));
                REQUIRE(event_indices1.at(i) == event_indices6.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
                REQUIRE(heights1.at(i) == heights4.at(i));
                REQUIRE(heights1.at(i) == heights5.at(i));
                REQUIRE(heights1.at(i) == heights6.at(i));
            }
            else if (nevents.at(i) == 6) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices1.at(i) != event_indices4.at(i));
                REQUIRE(event_indices1.at(i) != event_indices5.at(i));
                REQUIRE(event_indices1.at(i) != event_indices6.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices4.at(i));
                REQUIRE(event_indices2.at(i) != event_indices5.at(i));
                REQUIRE(event_indices2.at(i) != event_indices6.at(i));
                REQUIRE(event_indices3.at(i) != event_indices4.at(i));
                REQUIRE(event_indices3.at(i) != event_indices5.at(i));
                REQUIRE(event_indices3.at(i) != event_indices6.at(i));
                REQUIRE(event_indices4.at(i) != event_indices5.at(i));
                REQUIRE(event_indices4.at(i) != event_indices6.at(i));
                REQUIRE(event_indices5.at(i) != event_indices6.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights1.at(i) != heights4.at(i));
                REQUIRE(heights1.at(i) != heights5.at(i));
                REQUIRE(heights1.at(i) != heights6.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights4.at(i));
                REQUIRE(heights2.at(i) != heights5.at(i));
                REQUIRE(heights2.at(i) != heights6.at(i));
                REQUIRE(heights3.at(i) != heights4.at(i));
                REQUIRE(heights3.at(i) != heights5.at(i));
                REQUIRE(heights3.at(i) != heights6.at(i));
                REQUIRE(heights4.at(i) != heights5.at(i));
                REQUIRE(heights4.at(i) != heights6.at(i));
                REQUIRE(heights5.at(i) != heights6.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == 100001);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == 100001);

        REQUIRE(model_counts.at("000000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012345") == nevent_counts.at(6));

        for (std::string m: {"000000", "012345", "012344", "012340", "001234", "012314"}) {
            REQUIRE((model_counts.at(m) / 100001.0) == Approx(std::exp(
                    get_pyp_log_prior_probability(m, concentration, discount))).epsilon(0.002));
        }

        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing PYP with 3 pairs and fully parameterized", "[SamplingPrior]") {

    SECTION("Testing alpha and discount integrated") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        double discount_a = 4.0;
        double discount_b = 2.0;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-tpyp5.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-tpyp5-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    pitman_yor_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    gamma_distribution:\n";
        os << "                        shape: " << concentration_shape << "\n";
        os << "                        scale: " << concentration_scale << "\n";
        os << "            discount:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    beta_distribution:\n";
        os << "                        alpha: " << discount_a << "\n";
        os << "                        beta: " << discount_b << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 2\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        SampleSummarizer<double> size1_summary_b = prior_sample.summarize<double>("pop_size_fas");
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        SampleSummarizer<double> size2_summary_b = prior_sample.summarize<double>("pop_size_pop2");
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_b.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_b.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        SampleSummarizer<double> disc_summary = prior_sample.summarize<double>("discount");
        REQUIRE(disc_summary.sample_size() == expected_sample_size);
        REQUIRE(disc_summary.mean() == Approx(discount_a / (discount_a + discount_b)).epsilon(0.01));
        double expected_discount_var = ( (discount_a * discount_b) /
                ((discount_a + discount_b) *
                 (discount_a + discount_b) *
                 (discount_a + discount_b + 1)) );
        REQUIRE(disc_summary.variance() == Approx(expected_discount_var).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.0872).epsilon(0.005));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.651).epsilon(0.005));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}

TEST_CASE("Testing PYP with 2 singletons and 1 pair, fully parameterized",
        "[SamplingPrior]") {

    SECTION("Testing alpha and discount integrated") {
        double height_shape = 10.0;
        double height_scale = 0.001;
        double size1_shape = 10.0;
        double size1_scale = 0.0001;
        double size2_shape = 2.0;
        double size2_scale = 0.001;
        double size3_shape = 5.0;
        double size3_scale = 0.0005;
        double f1_a = 2.0;
        double f1_b = 1.1;
        double f2_a = 1.0;
        double f2_b = 0.5;
        double f3_a = 1.5;
        double f3_b = 1.8;
        double expected_f1_mean = f1_a / (f1_a + f1_b);
        double expected_f1_variance = (f1_a * f1_b) / ((f1_a + f1_b) * (f1_a + f1_b) * (f1_a + f1_b + 1.0));
        double expected_f2_mean = f2_a / (f2_a + f2_b);
        double expected_f2_variance = (f2_a * f2_b) / ((f2_a + f2_b) * (f2_a + f2_b) * (f2_a + f2_b + 1.0));
        double expected_f3_mean = f3_a / (f3_a + f3_b);
        double expected_f3_variance = (f3_a * f3_b) / ((f3_a + f3_b) * (f3_a + f3_b) * (f3_a + f3_b + 1.0));
        double mult2_shape = 100.0;
        double mult2_scale = 0.005;
        double mult3_shape = 100.0;
        double mult3_scale = 0.02;
        double concentration_shape = 5.0;
        double concentration_scale = 0.2;
        double discount_a = 3.5;
        double discount_b = 2.3;
        std::string auto_optimize = "true";
        std::string tag = _PRIOR_SAMPLING_RNG.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-tpyp6.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-tpyp6-state-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "event_time_prior:\n";
        os << "    gamma_distribution:\n";
        os << "        shape: " << height_shape << "\n";
        os << "        scale: " << height_scale << "\n";
        os << "event_model_prior:\n";
        os << "    pitman_yor_process:\n";
        os << "        parameters:\n";
        os << "            concentration:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    gamma_distribution:\n";
        os << "                        shape: " << concentration_shape << "\n";
        os << "                        scale: " << concentration_scale << "\n";
        os << "            discount:\n";
        os << "                estimate: true\n";
        os << "                prior:\n";
        os << "                    beta_distribution:\n";
        os << "                        alpha: " << discount_a << "\n";
        os << "                        beta: " << discount_b << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: 500000\n";
        os << "    sample_frequency: 10\n";
        os << "operator_settings:\n";
        os << "    auto_optimize: " << auto_optimize << "\n";
        os << "    auto_optimize_delay: 10000\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        ModelOperator:\n";
        os << "            number_of_auxiliary_categories: 5\n";
        os << "            weight: 1.0\n";
        os << "        ConcentrationScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        TimeSizeRateMixer:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 0.0\n";
        os << "        EventTimeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "global_comparison_settings:\n";
        os << "    operators:\n";
        os << "        TimeRootSizeMixer:\n";
        os << "            scale: 0.2\n";
        os << "            weight: 0.0\n";
        os << "        MutationRateScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        RootPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        LeafPopulationSizeScaler:\n";
        os << "            scale: 0.5\n";
        os << "            weight: 1.0\n";
        os << "        FreqMover:\n";
        os << "            window: 0.1\n";
        os << "            weight: 1.0\n";
        os << "    genotypes_are_diploid: true\n";
        os << "    markers_are_dominant: false\n";
        os << "    population_name_delimiter: \" \"\n";
        os << "    population_name_is_prefix: true\n";
        os << "    constant_sites_removed: true\n";
        os << "    equal_population_sizes: false\n";
        os << "comparisons:\n";
        os << "- comparison:\n";
        os << "    path: hemi129-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size1_shape << "\n";
        os << "                    scale: " << size1_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f1_a << "\n";
        os << "                    beta: " << f1_b << "\n";
        os << "        mutation_rate:\n";
        os << "            value: 1.0\n";
        os << "            estimate: false\n";
        os << "- comparison:\n";
        os << "    path: hemi129-altname1-singleton.nex\n";
        os << "    parameters:\n";
        os << "        population_size:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << size2_shape << "\n";
        os << "                    scale: " << size2_scale << "\n";
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f2_a << "\n";
        os << "                    beta: " << f2_b << "\n";
        os << "        mutation_rate:\n";
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
        os << "        freq_1:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                beta_distribution:\n";
        os << "                    alpha: " << f3_a << "\n";
        os << "                    beta: " << f3_b << "\n";
        os << "        mutation_rate:\n";
        os << "            estimate: true\n";
        os << "            prior:\n";
        os << "                gamma_distribution:\n";
        os << "                    shape: " << mult3_shape << "\n";
        os << "                    scale: " << mult3_scale << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "ecoevolity";
        char arg1[] = "--seed";
        char arg2[] = "72349827";
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
        ret = ecoevolity_main<CollectionSettings, ComparisonPopulationTreeCollection>(argc, argv);
        REQUIRE(ret == 0);
        REQUIRE(path::exists(log_path));

        spreadsheet::Spreadsheet prior_sample;
        prior_sample.update(log_path);

        unsigned int expected_sample_size = 50001;

        SampleSummarizer<double> height_summary1 = prior_sample.summarize<double>("root_height_kya");
        SampleSummarizer<double> height_summary2 = prior_sample.summarize<double>("root_height_pop1");
        SampleSummarizer<double> height_summary3 = prior_sample.summarize<double>("root_height_pop1b");
        REQUIRE(height_summary1.sample_size() == expected_sample_size);
        REQUIRE(height_summary1.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary1.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.sample_size() == expected_sample_size);
        REQUIRE(height_summary2.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary2.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.sample_size() == expected_sample_size);
        REQUIRE(height_summary3.mean() == Approx(height_shape * height_scale).epsilon(0.01));
        REQUIRE(height_summary3.variance() == Approx(height_shape * height_scale * height_scale).epsilon(0.01));

        SampleSummarizer<double> size1_summary_a = prior_sample.summarize<double>("pop_size_kya");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_fas"));
        SampleSummarizer<double> size1_summary_c = prior_sample.summarize<double>("pop_size_root_kya");
        SampleSummarizer<double> size2_summary_a = prior_sample.summarize<double>("pop_size_pop1");
        REQUIRE_THROWS(prior_sample.summarize<double>("pop_size_pop2"));
        SampleSummarizer<double> size2_summary_c = prior_sample.summarize<double>("pop_size_root_pop1");
        SampleSummarizer<double> size3_summary_a = prior_sample.summarize<double>("pop_size_pop1b");
        SampleSummarizer<double> size3_summary_b = prior_sample.summarize<double>("pop_size_pop2b");
        SampleSummarizer<double> size3_summary_c = prior_sample.summarize<double>("pop_size_root_pop1b");

        REQUIRE(size1_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size1_summary_a.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_a.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.mean() == Approx(size1_shape * size1_scale).epsilon(0.01));
        REQUIRE(size1_summary_c.variance() == Approx(size1_shape * size1_scale * size1_scale).epsilon(0.01));

        REQUIRE(size2_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size2_summary_a.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_a.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.mean() == Approx(size2_shape * size2_scale).epsilon(0.01));
        REQUIRE(size2_summary_c.variance() == Approx(size2_shape * size2_scale * size2_scale).epsilon(0.01));

        REQUIRE(size3_summary_a.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_b.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_c.sample_size() == expected_sample_size);
        REQUIRE(size3_summary_a.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_a.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_b.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.mean() == Approx(size3_shape * size3_scale).epsilon(0.01));
        REQUIRE(size3_summary_c.variance() == Approx(size3_shape * size3_scale * size3_scale).epsilon(0.01));

        SampleSummarizer<double> f1_summary = prior_sample.summarize<double>("freq_1_kya");
        SampleSummarizer<double> f2_summary = prior_sample.summarize<double>("freq_1_pop1");
        SampleSummarizer<double> f3_summary = prior_sample.summarize<double>("freq_1_pop1b");
        REQUIRE(f1_summary.sample_size() == expected_sample_size);
        REQUIRE(f2_summary.sample_size() == expected_sample_size);
        REQUIRE(f3_summary.sample_size() == expected_sample_size);
        REQUIRE(f1_summary.mean() ==     Approx(expected_f1_mean).epsilon(0.01));
        REQUIRE(f2_summary.mean() ==     Approx(expected_f2_mean).epsilon(0.01));
        REQUIRE(f3_summary.mean() ==     Approx(expected_f3_mean).epsilon(0.01));
        REQUIRE(f1_summary.variance() == Approx(expected_f1_variance).epsilon(0.01));
        REQUIRE(f2_summary.variance() == Approx(expected_f2_variance).epsilon(0.01));
        REQUIRE(f3_summary.variance() == Approx(expected_f3_variance).epsilon(0.01));

        SampleSummarizer<double> mult1_summary = prior_sample.summarize<double>("mutation_rate_kya");
        SampleSummarizer<double> mult2_summary = prior_sample.summarize<double>("mutation_rate_pop1");
        SampleSummarizer<double> mult3_summary = prior_sample.summarize<double>("mutation_rate_pop1b");
        REQUIRE(mult1_summary.sample_size() == expected_sample_size);
        REQUIRE(mult2_summary.sample_size() == expected_sample_size);
        REQUIRE(mult3_summary.sample_size() == expected_sample_size);
        REQUIRE(mult1_summary.mean() == 1.0);
        REQUIRE(mult1_summary.variance() == 0.0);
        REQUIRE(mult2_summary.mean() == Approx(mult2_shape * mult2_scale).epsilon(0.01));
        REQUIRE(mult2_summary.variance() == Approx(mult2_shape * mult2_scale * mult2_scale).epsilon(0.01));
        REQUIRE(mult3_summary.mean() == Approx(mult3_shape * mult3_scale).epsilon(0.01));
        REQUIRE(mult3_summary.variance() == Approx(mult3_shape * mult3_scale * mult3_scale).epsilon(0.01));

        SampleSummarizer<double> conc_summary = prior_sample.summarize<double>("concentration");
        REQUIRE(conc_summary.sample_size() == expected_sample_size);
        REQUIRE(conc_summary.mean() == Approx(concentration_shape * concentration_scale).epsilon(0.01));
        REQUIRE(conc_summary.variance() == Approx(concentration_shape * concentration_scale * concentration_scale).epsilon(0.01));

        std::vector<int> nevents = prior_sample.get<int>("number_of_events");
        std::vector<int> event_indices1 = prior_sample.get<int>("root_height_index_kya");
        std::vector<int> event_indices2 = prior_sample.get<int>("root_height_index_pop1");
        std::vector<int> event_indices3 = prior_sample.get<int>("root_height_index_pop1b");
        std::vector<double> heights1 = prior_sample.get<double>("root_height_kya");
        std::vector<double> heights2 = prior_sample.get<double>("root_height_pop1");
        std::vector<double> heights3 = prior_sample.get<double>("root_height_pop1b");

        std::map<std::string, int> model_counts = {
                {"000", 0},
                {"001", 0},
                {"010", 0},
                {"011", 0},
                {"012", 0}
        };
        std::map<int, int> nevent_counts = {
                {1, 0},
                {2, 0},
                {3, 0}
        };
        for (size_t i = 0; i < nevents.size(); ++i) {
            std::ostringstream stream;
            stream << event_indices1.at(i);
            stream << event_indices2.at(i);
            stream << event_indices3.at(i);
            std::string model_str = stream.str();
            REQUIRE(model_counts.count(model_str) == 1);
            REQUIRE(nevent_counts.count(nevents.at(i)) == 1);
            ++model_counts[model_str];
            ++nevent_counts[nevents.at(i)];
            if (nevents.at(i) == 1) {
                REQUIRE(event_indices1.at(i) == event_indices2.at(i));
                REQUIRE(event_indices1.at(i) == event_indices3.at(i));
                REQUIRE(heights1.at(i) == heights2.at(i));
                REQUIRE(heights1.at(i) == heights3.at(i));
            }
            else if (nevents.at(i) == 3) {
                REQUIRE(event_indices1.at(i) != event_indices2.at(i));
                REQUIRE(event_indices1.at(i) != event_indices3.at(i));
                REQUIRE(event_indices2.at(i) != event_indices3.at(i));
                REQUIRE(heights1.at(i) != heights2.at(i));
                REQUIRE(heights1.at(i) != heights3.at(i));
                REQUIRE(heights2.at(i) != heights3.at(i));
            }
        }
        int total = 0;
        for (auto const &kv: model_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);
        total = 0;
        for (auto const &kv: nevent_counts) {
            total += kv.second;
        }
        REQUIRE(total == expected_sample_size);

        REQUIRE(model_counts.at("000") == nevent_counts.at(1));
        REQUIRE(model_counts.at("012") == nevent_counts.at(3));
        REQUIRE((model_counts.at("001") + model_counts.at("010") + model_counts.at("011")) == nevent_counts.at(2));

        REQUIRE((model_counts.at("000") / (double)expected_sample_size) == Approx(0.108).epsilon(0.005));
        REQUIRE((model_counts.at("012") / (double)expected_sample_size) == Approx(0.593).epsilon(0.005));

        // Make sure the rest of the prior sample is as expected
        SampleSummarizer<double> lnl_summary = prior_sample.summarize<double>("ln_likelihood");
        REQUIRE(lnl_summary.mean() == 0.0);
        REQUIRE(lnl_summary.variance() == 0.0);

        delete[] cfg_path;
    }
}
