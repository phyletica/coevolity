#include "catch.hpp"

#include "ecoevolity/phycoeval.hpp"
#include "ecoevolity/rng.hpp"
#include "ecoevolity/path.hpp"
#include "ecoevolity/spreadsheet.hpp"

#include "utils_for_testing.hpp"


TEST_CASE("Testing phycoeval cli with 5 leaves, full model, unconstrained sizes",
        "[phycoeval]") {

    SECTION("Testing 5 leaves with full model, unconstrained sizes") {
        RandomNumberGenerator rng = RandomNumberGenerator(2784598);

        double mu_rate_shape = 10.0;
        double mu_rate_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> mu_rate_prior = std::make_shared<GammaDistribution>(
                mu_rate_shape,
                mu_rate_scale);

        double root_height_shape = 20.0;
        double root_height_scale = 0.025;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        double pop_size_shape = 10.0;
        double pop_size_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> pop_size_prior = std::make_shared<GammaDistribution>(
                pop_size_shape,
                pop_size_scale);

        double freq_a = 3.0;
        double freq_b = 2.0;
        std::shared_ptr<ContinuousProbabilityDistribution> freq_prior = std::make_shared<BetaDistribution>(
                freq_a,
                freq_b);

        double height_alpha_shape = 20.0;
        double height_alpha_scale = 0.4;
        std::shared_ptr<ContinuousProbabilityDistribution> height_alpha_prior = std::make_shared<GammaDistribution>(
                height_alpha_shape,
                height_alpha_scale);

        unsigned int chain_length = 10000;
        unsigned int sample_frequency = 20;

        std::string tag = rng.random_string(10);
        std::string test_path = "data/tmp-config-" + tag + "-phyco.cfg";
        std::string log_path = "data/tmp-config-" + tag + "-phyco-state-run-1.log";
        std::string tree_path = "data/tmp-config-" + tag + "-phyco-tree-run-1.log";
        std::ofstream os;
        os.open(test_path);
        os << "data:\n";
        os << "    ploidy: 2\n";
        os << "    constant_sites_removed: false\n";
        os << "    yaml_allele_counts:\n";
        os << "        path: species-5-genomes-4-chars-1000.yml\n";
        os << "tree_model:\n";
        os << "    tree_space: generalized\n";
        os << "    starting_tree: comb\n";
        os << "    tree_prior:\n";
        os << "        uniform_root_and_betas:\n";
        os << "            parameters:\n";
        os << "                root_height:\n";
        os << "                    estimate: true\n";
        os << "                    prior:\n";
        os << "                        gamma_distribution:\n";
        os << "                            shape: " << root_height_shape << "\n";
        os << "                            scale: " << root_height_scale << "\n";
        os << "                alpha_of_node_height_beta_prior:\n";
        os << "                    estimate: true\n";
        os << "                    prior:\n";
        os << "                        gamma_distribution:\n";
        os << "                            shape: " << height_alpha_shape << "\n";
        os << "                            scale: " << height_alpha_scale << "\n";
        os << "branch_parameters:\n";
        os << "    population_size:\n";
        os << "        estimate: true\n";
        os << "        equal_population_sizes: false\n";
        os << "        prior:\n";
        os << "            gamma_distribution:\n";
        os << "                shape: " << pop_size_shape << "\n";
        os << "                scale: " << pop_size_scale << "\n";
        os << "mutation_parameters:\n";
        os << "    freq_1:\n";
        os << "        estimate: true\n";
        os << "        prior:\n";
        os << "            beta_distribution:\n";
        os << "                alpha: " << freq_a << "\n";
        os << "                beta: " << freq_b << "\n";
        os << "    mutation_rate:\n";
        os << "        estimate: true\n";
        os << "        prior:\n";
        os << "            gamma_distribution:\n";
        os << "                shape: " << mu_rate_shape << "\n";
        os << "                scale: " << mu_rate_scale << "\n";
        os << "mcmc_settings:\n";
        os << "    chain_length: " << chain_length << "\n";
        os << "    sample_frequency: " << sample_frequency << "\n";
        os.close();
        REQUIRE(path::exists(test_path));

        char arg0[] = "phycoeval";
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

        ret = phycoeval_main<BasePopulationTree>(argc, argv);
        REQUIRE(ret == 0);

        delete[] cfg_path;

        /* std::map< std::set< std::set<Split> >, unsigned int> split_counts; */

        /* unsigned int count_nheights_1 = 0; */
        /* unsigned int count_nheights_2 = 0; */
        /* unsigned int count_nheights_3 = 0; */
        /* unsigned int count_nheights_4 = 0; */

        /* SampleSummarizer<double> pop_size_summary; */
        /* SampleSummarizer<double> root_pop_size_summary; */
        /* SampleSummarizer<double> leaf0_pop_size_summary; */
        /* SampleSummarizer<double> leaf1_pop_size_summary; */
        /* SampleSummarizer<double> leaf2_pop_size_summary; */
        /* SampleSummarizer<double> leaf3_pop_size_summary; */
        /* SampleSummarizer<double> leaf4_pop_size_summary; */
        /* SampleSummarizer<double> root_height_summary; */
        /* SampleSummarizer<double> internal_0_height_summary; */
        /* SampleSummarizer<double> internal_height_summary; */
        /* SampleSummarizer<double> height_alpha_summary; */
        /* SampleSummarizer<double> height_beta_summary; */
        /* SampleSummarizer<double> internal_height_prior_sample; */

        /* std::vector< std::shared_ptr<PositiveRealParameter> > pop_sizes; */

        /* unsigned int burnin = 10000; */
        /* for (unsigned int i = 0; i < burnin; ++i) { */
        /*     op = op_schedule.draw_operator(rng); */
        /*     if (op->get_type() == BaseGeneralTreeOperatorTemplate::OperatorTypeEnum::topology_model_operator) { */
        /*         op->operate_plus(rng, */
        /*                 &tree, */
        /*                 time_ops, */
        /*                 1, 1, 1); */
        /*     } */
        /*     else { */
        /*         op->operate(rng, &tree, 1, 1); */
        /*     } */
        /* } */

        /* unsigned int niterations = 20000000; */
        /* unsigned int sample_freq = 50; */
        /* unsigned int nsamples = niterations / sample_freq; */

        /* unsigned int sample_count = 0; */
        /* unsigned int report_freq = 10000; */
        /* for (unsigned int i = 0; i < niterations; ++i) { */
        /*     op = op_schedule.draw_operator(rng); */
        /*     if (op->get_type() == BaseGeneralTreeOperatorTemplate::OperatorTypeEnum::topology_model_operator) { */
        /*         op->operate_plus(rng, */
        /*                 &tree, */
        /*                 time_ops, */
        /*                 1, 1, 1); */
        /*     } */
        /*     else { */
        /*         op->operate(rng, &tree, 1, 1); */
        /*     } */
        /*     double a = height_alpha_prior->draw(rng); */
        /*     double b = height_beta_prior->draw(rng); */
        /*     double v = BetaDistribution::get_draw(rng, a, b); */
        /*     internal_height_prior_sample.add_sample(v); */
        /*     if ((i + 1) % sample_freq == 0) { */
        /*         pop_sizes = tree.get_pointers_to_population_sizes(); */
        /*         for (auto pop_size : pop_sizes) { */
        /*             pop_size_summary.add_sample(pop_size->get_value()); */
        /*         } */
        /*         root_pop_size_summary.add_sample(tree.get_root_population_size()); */
        /*         leaf0_pop_size_summary.add_sample(tree.get_node("leaf0")->get_population_size()); */
        /*         leaf1_pop_size_summary.add_sample(tree.get_node("leaf1")->get_population_size()); */
        /*         leaf2_pop_size_summary.add_sample(tree.get_node("leaf2")->get_population_size()); */
        /*         leaf3_pop_size_summary.add_sample(tree.get_node("leaf3")->get_population_size()); */
        /*         leaf4_pop_size_summary.add_sample(tree.get_node("leaf4")->get_population_size()); */
        /*         root_height_summary.add_sample(tree.get_root_height()); */
        /*         height_alpha_summary.add_sample(tree.get_alpha_of_node_height_beta_prior()); */
        /*         height_beta_summary.add_sample(tree.get_beta_of_node_height_beta_prior()); */
        /*         for (unsigned int height_idx = 0; */
        /*                 height_idx < (tree.get_number_of_node_heights() - 1); */
        /*                 ++height_idx) { */
        /*             internal_height_summary.add_sample(tree.get_height(height_idx) / tree.get_height_of_youngest_parent(height_idx)); */
        /*             if (height_idx == 0) { */
        /*                 internal_0_height_summary.add_sample(tree.get_height(height_idx)); */
        /*                 internal_0_height_summary.add_sample(tree.get_height(height_idx) / tree.get_height_of_youngest_parent(height_idx)); */
        /*             } */
        /*         } */
        /*         if (tree.get_number_of_node_heights() == 1) { */
        /*             ++count_nheights_1; */
        /*         } */
        /*         else if (tree.get_number_of_node_heights() == 2) { */
        /*             ++count_nheights_2; */
        /*         } */
        /*         else if (tree.get_number_of_node_heights() == 3) { */
        /*             ++count_nheights_3; */
        /*         } */
        /*         else if (tree.get_number_of_node_heights() == 4) { */
        /*             ++count_nheights_4; */
        /*         } */
        /*         std::set< std::set<Split> > splits = tree.get_splits(false); */
        /*         if (split_counts.count(splits) > 0) { */
        /*             ++split_counts[splits]; */
        /*         } */
        /*         else { */
        /*             split_counts[splits] = 1; */
        /*         } */
        /*         ++sample_count; */
        /*         if (sample_count % report_freq == 0) { */
        /*             std::cout << "Sampled " << sample_count << " of " << nsamples << std::endl; */
        /*         } */
        /*     } */
        /* } */
        /* op_schedule.write_operator_rates(std::cout); */

        /* REQUIRE((count_nheights_1 + count_nheights_2 + count_nheights_3 + count_nheights_4) == nsamples); */

        /* double freq_nheights_1 = count_nheights_1 / (double)nsamples; */
        /* double freq_nheights_2 = count_nheights_2 / (double)nsamples; */
        /* double freq_nheights_3 = count_nheights_3 / (double)nsamples; */
        /* double freq_nheights_4 = count_nheights_4 / (double)nsamples; */

        /* double exp_freq = 1.0/336.0; */
        /* double exp_count = nsamples/336.0; */
        /* std::map< std::set< std::set<Split> >, double> bad_splits; */

        /* double prop_error_threshold = 0.2; */
        /* unsigned int total_trees_sampled = 0; */
        /* std::map< std::set< std::set<Split> >, double> split_freqs; */
        /* double chi_sq_test_statistic = 0.0; */
        /* std::cout << "Total tree topologies sampled: " << split_counts.size() << "\n"; */
        /* for (auto s_c : split_counts) { */
        /*     total_trees_sampled += s_c.second; */
        /*     split_freqs[s_c.first] = s_c.second / (double)nsamples; */
        /*     std::cout << "Tree:\n"; */
        /*     for (auto splitset : s_c.first) { */
        /*         unsigned int s_count = 0; */
        /*         for (auto split : splitset) { */
        /*             if (s_count > 0) { */
        /*                 // Indent shared splits */
        /*                 std::cout << "  "; */
        /*             } */
        /*             std::cout << "  " << split.as_string() << "\n"; */
        /*             ++s_count; */
        /*         } */
        /*     } */
        /*     double prop_error = ((double)s_c.second - exp_count) / exp_count; */
        /*     std::cout << "  nsamples: " << s_c.second << "\n"; */
        /*     std::cout << "  prop error: " << prop_error << "\n"; */
        /*     if (fabs(prop_error) > prop_error_threshold) { */
        /*         bad_splits[s_c.first] = prop_error; */
        /*     } */
        /*     double count_diff = s_c.second - exp_count; */
        /*     chi_sq_test_statistic += (count_diff * count_diff) / exp_count; */
        /* } */

        /* double quantile_chi_sq_335_10 = 368.6; */
        /* std::cout << "Chi-square test statistic: " << chi_sq_test_statistic << "\n"; */
        /* std::cout << "Chi-square(335) 0.9 quantile: " << quantile_chi_sq_335_10 << "\n"; */

        /* std::cout << "BAD SPLITS (proportional error > " << prop_error_threshold << ")\n"; */
        /* for (auto s_e : bad_splits) { */
        /*     std::cout << "\nTree:\n"; */
        /*     for (auto splitset : s_e.first) { */
        /*         unsigned int s_count = 0; */
        /*         for (auto split : splitset) { */
        /*             if (s_count > 0) { */
        /*                 // Indent shared splits */
        /*                 std::cout << "  "; */
        /*             } */
        /*             std::cout << "  " << split.as_string() << "\n"; */
        /*             ++s_count; */
        /*         } */
        /*     } */
        /*     std::cout << "  prop error: " << s_e.second << "\n"; */
        /* } */

        /* write_r_script(split_counts, "../5-leaf-general-tree-test-full-model-free-pop-sizes.r"); */

        /* REQUIRE(total_trees_sampled == nsamples); */

        /* // We should sample every possible tree */
        /* REQUIRE(split_counts.size() == 336); */

        /* double eps = 0.005; */
        /* REQUIRE(pop_size_summary.mean() == Approx(pop_size_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(pop_size_summary.variance() == Approx(pop_size_prior->get_variance()).epsilon(eps)); */
        /* REQUIRE(root_pop_size_summary.mean() == Approx(pop_size_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(root_pop_size_summary.variance() == Approx(pop_size_prior->get_variance()).epsilon(eps)); */
        /* REQUIRE(leaf0_pop_size_summary.mean() == Approx(pop_size_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(leaf0_pop_size_summary.variance() == Approx(pop_size_prior->get_variance()).epsilon(eps)); */
        /* REQUIRE(leaf1_pop_size_summary.mean() == Approx(pop_size_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(leaf1_pop_size_summary.variance() == Approx(pop_size_prior->get_variance()).epsilon(eps)); */
        /* REQUIRE(leaf2_pop_size_summary.mean() == Approx(pop_size_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(leaf2_pop_size_summary.variance() == Approx(pop_size_prior->get_variance()).epsilon(eps)); */
        /* REQUIRE(leaf3_pop_size_summary.mean() == Approx(pop_size_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(leaf3_pop_size_summary.variance() == Approx(pop_size_prior->get_variance()).epsilon(eps)); */
        /* REQUIRE(leaf4_pop_size_summary.mean() == Approx(pop_size_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(leaf4_pop_size_summary.variance() == Approx(pop_size_prior->get_variance()).epsilon(eps)); */

        /* REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps)); */
        /* REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps)); */
        /* REQUIRE(height_alpha_summary.mean() == Approx(height_alpha_prior->get_mean()).epsilon(eps * 2.0)); */
        /* REQUIRE(height_alpha_summary.variance() == Approx(height_alpha_prior->get_variance()).epsilon(eps * 2.0)); */
        /* REQUIRE(height_beta_summary.mean() == Approx(height_beta_prior->get_mean()).epsilon(eps * 2.0)); */
        /* REQUIRE(height_beta_summary.variance() == Approx(height_beta_prior->get_variance()).epsilon(eps * 2.0)); */

        /* /1* REQUIRE(internal_0_height_summary.mean() == Approx(internal_height_prior_sample.mean()).epsilon(eps)); *1/ */
        /* /1* REQUIRE(internal_0_height_summary.variance() == Approx(internal_height_prior_sample.variance()).epsilon(eps)); *1/ */
        /* /1* REQUIRE(internal_height_summary.mean() == Approx(internal_height_prior_sample.mean()).epsilon(eps)); *1/ */
        /* /1* REQUIRE(internal_height_summary.variance() == Approx(internal_height_prior_sample.variance()).epsilon(eps)); *1/ */

        /* for (auto s_f : split_freqs) { */
        /*     REQUIRE(s_f.second == Approx(exp_freq).epsilon(eps)); */
        /* } */

        /* REQUIRE(chi_sq_test_statistic < quantile_chi_sq_335_10); */
    }
}
