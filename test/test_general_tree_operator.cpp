#include "catch.hpp"
#include "ecoevolity/general_tree_operator.hpp"

#include <limits>
#include <memory>

#include "ecoevolity/node.hpp"
#include "ecoevolity/probability.hpp"
#include "ecoevolity/parameter.hpp"
#include "ecoevolity/stats_util.hpp"
#include "ecoevolity/rng.hpp"


TEST_CASE("Testing NodeHeightSlideBumpScaler with 3 leaves, fixed root, and optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing 3 leaves with fixed root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(1);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0));
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                REQUIRE(tree.get_height(1) == 1.0);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpScaler with 3 leaves, fixed root, and no optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing 3 leaves with fixed root and no optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(2);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_off_auto_optimize();
        op.set_coercable_parameter_value(1.8);

        REQUIRE(! op.auto_optimizing());
        REQUIRE(op.get_coercable_parameter_value() == 1.8);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0));
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                REQUIRE(tree.get_height(1) == 1.0);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - op.get_auto_optimize_delay()));
        REQUIRE(op.get_coercable_parameter_value() == 1.8);

        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpScaler with 3 leaves, variable root, and optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(3);

        double root_height_lower = 0.26;
        double root_height_upper = 0.34;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<UniformDistribution>(
                root_height_lower,
                root_height_upper);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.3);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.15);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 200000;
        unsigned int burnin = 100;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < burnin; ++i) {
            rop.operate(rng, &tree, 1);
            op.operate(rng, &tree, 1);
        }
        for (unsigned int i = 0; i < niterations; ++i) {
            rop.operate(rng, &tree, 1);
            op.operate(rng, &tree, 1, 1);
            if ((i + 1) % sample_freq == 0) {
                root_height_summary.add_sample(tree.get_root_height());
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
            }
        }
        std::cout << rop.header_string();
        std::cout << rop.to_string();
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations + burnin);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations + burnin - 100));
        REQUIRE(rop.get_number_of_attempts() == niterations + burnin);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations + burnin - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpScaler with 3 leaves, gamma root, and optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(4);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            rop.operate(rng, &tree, 1);
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
            }
        }
        std::cout << rop.header_string();
        std::cout << rop.to_string();
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(rop.get_number_of_attempts() == niterations);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations - 100));
        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing RootHeightScaler with 3 leaves, gamma root, and optimizing",
        "[RootHeightScaler]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(54168);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.001);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        NodeHeightScaler<Node> op;
        /* NodeHeightSlideBumpScaler<Node> op; */
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 500000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            rop.operate(rng, &tree, 1);
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_height(1));
            }
        }
        std::cout << rop.header_string();
        std::cout << rop.to_string();
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(rop.get_number_of_attempts() == niterations);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations - 100));
        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);
        
        double eps = 0.001;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpScaler with 4 leaves, fixed root, and optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing 4 leaves with fixed root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(19349871349);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        internal1->add_child(leaf2);
        internal1->add_child(internal0);
        root->add_child(internal1);
        root->add_child(leaf3);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_0_summary;
        SampleSummarizer<double> internal_height_1_summary;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_0_summary.add_sample(tree.get_height(0) / tree.get_height(1));
                internal_height_1_summary.add_sample(tree.get_height(1) / tree.get_height(2));
                REQUIRE(tree.get_height(2) == tree.get_root_height());
                REQUIRE(tree.get_height(2) == 1.0);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(internal_height_0_summary.sample_size() == nsamples);
        REQUIRE(internal_height_1_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(internal_height_0_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_0_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
        REQUIRE(internal_height_1_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_1_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}


TEST_CASE("Testing NodeHeightSlideBumpScaler with 4 leaves, balanced, fixed root, and optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing balanced 4 leaves with fixed root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(54709754);
        /* RandomNumberGenerator rng = RandomNumberGenerator(111); */

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.4);
        std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.25);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.15);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        internal1->add_child(leaf0);
        internal1->add_child(leaf1);
        internal0->add_child(leaf2);
        internal0->add_child(leaf3);
        root->add_child(internal0);
        root->add_child(internal1);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_0_summary;
        SampleSummarizer<double> internal_height_1_summary;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;

        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_0_summary.add_sample(tree.get_node("internal0")->get_height());
                internal_height_1_summary.add_sample(tree.get_node("internal1")->get_height());
                REQUIRE(tree.get_height(2) == tree.get_root_height());
                REQUIRE(tree.get_height(2) == 0.4);
                REQUIRE(tree.get_height_of_youngest_parent(0) == 0.4);
                REQUIRE(tree.get_height_of_youngest_parent(1) == 0.4);
                REQUIRE(tree.get_height(0) < tree.get_height(1));
                REQUIRE(tree.get_node("internal0")->get_height() != tree.get_node("internal1")->get_height());
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(internal_height_0_summary.sample_size() == nsamples);
        REQUIRE(internal_height_1_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 0.4);

        double eps = 0.001;
        std::cout << "int0 mean: " << internal_height_0_summary.mean() << "\n";
        std::cout << "int1 mean: " << internal_height_1_summary.mean() << "\n";
        REQUIRE(internal_height_0_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_0_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
        REQUIRE(internal_height_1_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_1_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}


TEST_CASE("Testing NodeHeightSlideBumpScaler with 4 leaves, gamma root, and optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing 4 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(250925098743);

        double root_height_shape = 5.0;
        double root_height_scale = 0.1;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.125);
        std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        internal1->add_child(leaf2);
        internal1->add_child(internal0);
        root->add_child(internal1);
        root->add_child(leaf3);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);
        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_0_summary;
        SampleSummarizer<double> internal_height_1_summary;

        unsigned int niterations = 1000000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            rop.operate(rng, &tree, 1);
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_0_summary.add_sample(tree.get_height(0) / tree.get_height(1));
                internal_height_1_summary.add_sample(tree.get_height(1) / tree.get_height(2));
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(2) == tree.get_root_height());
                /* std::cout << i + 1 << "\t" << tree.get_root_height() << "\n"; */
            }
        }
        std::cout << rop.header_string();
        std::cout << rop.to_string();
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(rop.get_number_of_attempts() == niterations);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations - 100));
        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_0_summary.sample_size() == nsamples);
        REQUIRE(internal_height_1_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_0_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_0_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
        REQUIRE(internal_height_1_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_1_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}


TEST_CASE("Testing NodeHeightSlideBumpScaler with 4 leaves, balanced, gamma root, and optimizing",
        "[NodeHeightSlideBumpScaler]") {

    SECTION("Testing balanced 4 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(4723445);

        double root_height_shape = 10.0;
        double root_height_scale = 0.08;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.125);
        std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        internal1->add_child(leaf2);
        internal1->add_child(leaf3);
        root->add_child(internal1);
        root->add_child(internal0);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);
        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_0_summary;
        SampleSummarizer<double> internal_height_1_summary;

        unsigned int niterations = 800000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            rop.operate(rng, &tree, 1);
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_0_summary.add_sample(tree.get_node("internal0")->get_height() / tree.get_root_height());
                internal_height_1_summary.add_sample(tree.get_node("internal1")->get_height() / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(2) == tree.get_root_height());
                REQUIRE(tree.get_height_of_youngest_parent(0) == tree.get_root_height());
                REQUIRE(tree.get_height_of_youngest_parent(1) == tree.get_root_height());
                REQUIRE(tree.get_height(0) < tree.get_height(1));
                REQUIRE(tree.get_node("internal0")->get_height() != tree.get_node("internal1")->get_height());
            }
        }
        std::cout << rop.header_string();
        std::cout << rop.to_string();
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(rop.get_number_of_attempts() == niterations);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations - 100));
        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_0_summary.sample_size() == nsamples);
        REQUIRE(internal_height_1_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        std::cout << "int0 mean: " << internal_height_0_summary.mean() << "\n";
        std::cout << "int1 mean: " << internal_height_1_summary.mean() << "\n";
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_0_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_0_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
        REQUIRE(internal_height_1_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_1_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing RootHeightScaler with 4 leaves, gamma root, and optimizing",
        "[RootHeightScaler]") {

    SECTION("Testing 4 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(213579);

        double root_height_shape = 5.0;
        double root_height_scale = 0.1;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.125);
        std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        internal1->add_child(leaf2);
        internal1->add_child(internal0);
        root->add_child(internal1);
        root->add_child(leaf3);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        NodeHeightSlideBumpScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_0_height_summary;
        SampleSummarizer<double> internal_1_height_summary;

        unsigned int niterations = 1000000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            rop.operate(rng, &tree, 1);
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(2) == tree.get_root_height());
                internal_0_height_summary.add_sample(tree.get_height(0) / tree.get_height(1));
                internal_1_height_summary.add_sample(tree.get_height(1) / tree.get_height(2));
            }
        }
        std::cout << rop.header_string();
        std::cout << rop.to_string();
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(rop.get_number_of_attempts() == niterations);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations - 100));
        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_0_height_summary.sample_size() == nsamples);
        REQUIRE(internal_1_height_summary.sample_size() == nsamples);

        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_0_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_0_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
        REQUIRE(internal_1_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_1_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}



TEST_CASE("Testing NodeHeightSlideBumpMover with 3 leaves, fixed root, and optimizing",
        "[NodeHeightSlideBumpMover]") {

    SECTION("Testing 3 leaves with fixed root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(50000);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NodeHeightSlideBumpMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0));
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                REQUIRE(tree.get_height(1) == 1.0);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpMover with 3 leaves, fixed root, and no optimizing",
        "[NodeHeightSlideBumpMover]") {

    SECTION("Testing 3 leaves with fixed root and no optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(6);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NodeHeightSlideBumpMover<Node> op;
        op.turn_off_auto_optimize();
        op.set_coercable_parameter_value(1.8);

        REQUIRE(! op.auto_optimizing());
        REQUIRE(op.get_coercable_parameter_value() == 1.8);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0));
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                REQUIRE(tree.get_height(1) == 1.0);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - op.get_auto_optimize_delay()));
        REQUIRE(op.get_coercable_parameter_value() == 1.8);

        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpMover with 3 leaves, variable root, and optimizing",
        "[NodeHeightSlideBumpMover]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(7);

        double root_height_lower = 0.48;
        double root_height_upper = 0.52;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<UniformDistribution>(
                root_height_lower,
                root_height_upper);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 400000;
        unsigned int burnin = 1000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < burnin; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
        }
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << rop.header_string();
        std::cout << rop.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations + burnin);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations + burnin - 100));
        REQUIRE(rop.get_number_of_attempts() == niterations + burnin);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations + burnin - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpMover with 3 leaves, gamma root, and optimizing",
        "[NodeHeightSlideBumpMover]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(8);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << rop.header_string();
        std::cout << rop.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));
        REQUIRE(rop.get_number_of_attempts() == niterations);
        REQUIRE(rop.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.001;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}



TEST_CASE("Testing NodeHeightSlideBumpSwapScaler with 3 leaves, gamma root, optimizing, no op root",
        "[NodeHeightSlideBumpSwapScaler]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(9);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpSwapScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(false);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 20;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < 100000; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
        }
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << rop.header_string();
        std::cout << rop.to_string();

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == 1.0);
        REQUIRE(freq_02 == 0.0);
        REQUIRE(freq_12 == 0.0);
    }
}

TEST_CASE("Testing NodeHeightSlideBumpSwapScaler with 3 leaves, gamma root, optimizing, and op root",
        "[NodeHeightSlideBumpSwapScaler]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(9);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpSwapScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(true);

        /* RootHeightScaler<Node> rop; */
        /* rop.turn_on_auto_optimize(); */
        /* rop.set_auto_optimize_delay(100); */

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        /* REQUIRE(rop.auto_optimizing()); */
        /* REQUIRE(rop.get_auto_optimize_delay() == 100); */

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 800000;
        unsigned int sample_freq = 20;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < 100000; ++i) {
            op.operate(rng, &tree, 1);
            /* rop.operate(rng, &tree, 1); */
        }
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            /* rop.operate(rng, &tree, 1); */
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        /* std::cout << rop.header_string(); */
        /* std::cout << rop.to_string(); */

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpSwapMover with 3 leaves, gamma root, optimizing, no root op",
        "[NodeHeightSlideBumpSwapMover]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(10);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpSwapMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(false);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        /* for (unsigned int i = 0; i < 100000; ++i) { */
        /*     op.operate(rng, &tree, 1); */
        /* } */
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << rop.header_string();
        std::cout << rop.to_string();

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == 1.0);
        REQUIRE(freq_02 == 0.0);
        REQUIRE(freq_12 == 0.0);
    }
}

TEST_CASE("Testing NodeHeightSlideBumpSwapMover with 3 leaves, gamma root, optimizing, and root op",
        "[NodeHeightSlideBumpSwapMover]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(10);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpSwapMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(true);

        /* RootHeightScaler<Node> rop; */
        /* rop.turn_on_auto_optimize(); */
        /* rop.set_auto_optimize_delay(100); */

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        /* REQUIRE(rop.auto_optimizing()); */
        /* REQUIRE(rop.get_auto_optimize_delay() == 100); */

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        /* for (unsigned int i = 0; i < 100000; ++i) { */
        /*     op.operate(rng, &tree, 1); */
        /* } */
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            /* rop.operate(rng, &tree, 1); */
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        /* std::cout << rop.header_string(); */
        /* std::cout << rop.to_string(); */

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}



TEST_CASE("Testing NeighborHeightNodeSwap with 3 leaves, gamma root",
        "[NeighborHeightNodeSwap]") {

    SECTION("Testing 3 leaves with variable root") {
        RandomNumberGenerator rng = RandomNumberGenerator(11000);

        double root_height_shape = 100.0;
        double root_height_scale = 0.01;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NeighborHeightNodeSwap<Node> op;

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);

        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        double eps = 0.005;

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}

TEST_CASE("Testing NeighborHeightNodeSwap with 3 leaves, fixed root",
        "[NeighborHeightNodeSwap]") {

    SECTION("Testing 3 leaves with fixed root") {
        RandomNumberGenerator rng = RandomNumberGenerator(12);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NeighborHeightNodeSwap<Node> op;

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);

        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        double eps = 0.005;

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}


TEST_CASE("Testing NeighborHeightNodePermute with 3 leaves, gamma root",
        "[NeighborHeightNodePermute]") {

    SECTION("Testing 3 leaves with variable root") {
        RandomNumberGenerator rng = RandomNumberGenerator(13);

        double root_height_shape = 100.0;
        double root_height_scale = 0.01;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NeighborHeightNodePermute<Node> op;

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);

        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        double eps = 0.005;

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}

TEST_CASE("Testing NeighborHeightNodePermute with 3 leaves, fixed root",
        "[NeighborHeightNodePermute]") {

    SECTION("Testing 3 leaves with fixed root") {
        RandomNumberGenerator rng = RandomNumberGenerator(14);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.5);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NeighborHeightNodePermute<Node> op;

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 200000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);

        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        double eps = 0.005;

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}




TEST_CASE("Testing NodeHeightSlideBumpPermuteScaler with 3 leaves, gamma root, optimizing, no root op",
        "[NodeHeightSlideBumpPermuteScaler]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(15);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpPermuteScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(false);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 20;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < 100000; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
        }
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << rop.header_string();
        std::cout << rop.to_string();

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == 1.0);
        REQUIRE(freq_02 == 0.0);
        REQUIRE(freq_12 == 0.0);
    }
}

TEST_CASE("Testing NodeHeightSlideBumpPermuteScaler with 3 leaves, gamma root, optimizing, and root op",
        "[NodeHeightSlideBumpPermuteScaler]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(15);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpPermuteScaler<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(true);

        /* RootHeightScaler<Node> rop; */
        /* rop.turn_on_auto_optimize(); */
        /* rop.set_auto_optimize_delay(100); */

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        /* REQUIRE(rop.auto_optimizing()); */
        /* REQUIRE(rop.get_auto_optimize_delay() == 100); */

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 20;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < 100000; ++i) {
            op.operate(rng, &tree, 1);
            /* rop.operate(rng, &tree, 1); */
        }
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            /* rop.operate(rng, &tree, 1); */
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        /* std::cout << rop.header_string(); */
        /* std::cout << rop.to_string(); */

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}

TEST_CASE("Testing NodeHeightSlideBumpPermuteMover with 3 leaves, gamma root, optimizing, no root op",
        "[NodeHeightSlideBumpPermuteMover]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(16);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpPermuteMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(false);

        RootHeightScaler<Node> rop;
        rop.turn_on_auto_optimize();
        rop.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        REQUIRE(rop.auto_optimizing());
        REQUIRE(rop.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        /* for (unsigned int i = 0; i < 100000; ++i) { */
        /*     op.operate(rng, &tree, 1); */
        /* } */
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            rop.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << rop.header_string();
        std::cout << rop.to_string();

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == 1.0);
        REQUIRE(freq_02 == 0.0);
        REQUIRE(freq_12 == 0.0);
    }
}

TEST_CASE("Testing NodeHeightSlideBumpPermuteMover with 3 leaves, gamma root, optimizing, and root op",
        "[NodeHeightSlideBumpPermuteMover]") {

    SECTION("Testing 3 leaves with variable root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(16);

        double root_height_shape = 10.0;
        double root_height_scale = 0.05;
        std::shared_ptr<ContinuousProbabilityDistribution> root_height_prior = std::make_shared<GammaDistribution>(
                root_height_shape,
                root_height_scale);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 0.5);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.25);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);
        tree.set_root_node_height_prior(root_height_prior);

        tree.ignore_data();
        tree.estimate_root_height();

        NodeHeightSlideBumpPermuteMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);
        op.set_operate_on_root(true);

        /* RootHeightScaler<Node> rop; */
        /* rop.turn_on_auto_optimize(); */
        /* rop.set_auto_optimize_delay(100); */

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);
        /* REQUIRE(rop.auto_optimizing()); */
        /* REQUIRE(rop.get_auto_optimize_delay() == 100); */

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> root_height_summary;
        SampleSummarizer<double> internal_height_summary;

        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        /* for (unsigned int i = 0; i < 100000; ++i) { */
        /*     op.operate(rng, &tree, 1); */
        /* } */
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            /* rop.operate(rng, &tree, 1); */
            if ((i + 1) % sample_freq == 0) {
                internal_height_summary.add_sample(tree.get_height(0) / tree.get_root_height());
                root_height_summary.add_sample(tree.get_root_height());
                REQUIRE(tree.get_height(1) == tree.get_root_height());
                if (tree.get_root_ptr()->is_child("leaf0")) {
                    ++count_12;
                }
                if (tree.get_root_ptr()->is_child("leaf1")) {
                    ++count_02;
                }
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++count_01;
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        /* std::cout << rop.header_string(); */
        /* std::cout << rop.to_string(); */

        REQUIRE(root_height_summary.sample_size() == nsamples);
        REQUIRE(internal_height_summary.sample_size() == nsamples);
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        double eps = 0.005;
        REQUIRE(root_height_summary.mean() == Approx(root_height_prior->get_mean()).epsilon(eps));
        REQUIRE(root_height_summary.variance() == Approx(root_height_prior->get_variance()).epsilon(eps));
        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));

        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "freq of ((1,2),0): " << freq_12 << "\n";
        REQUIRE(freq_01 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_02 == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(freq_12 == Approx(1.0/3.0).epsilon(eps));
    }
}


TEST_CASE("Testing NodeHeightSlideBumpMover with 2 nested internals, fixed root, and optimizing",
        "[NodeHeightSlideBumpMover]") {

    SECTION("Testing 2 nested internals with fixed root and optimizing") {
        RandomNumberGenerator rng = RandomNumberGenerator(17);

        std::shared_ptr<Node> root = std::make_shared<Node>("root", 1.0);
        std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.7);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.3);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        internal1->add_child(internal0);
        internal1->add_child(leaf2);
        root->add_child(internal1);
        root->add_child(leaf3);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        NodeHeightSlideBumpMover<Node> op;
        op.turn_on_auto_optimize();
        op.set_auto_optimize_delay(100);

        REQUIRE(op.auto_optimizing());
        REQUIRE(op.get_auto_optimize_delay() == 100);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal0_height_summary;
        SampleSummarizer<double> internal1_height_summary;

        unsigned int niterations = 400000;
        unsigned int sample_freq = 5;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                internal0_height_summary.add_sample(tree.get_height(0) / tree.get_height(1));
                internal1_height_summary.add_sample(tree.get_height(1) / tree.get_height(2));
                REQUIRE(tree.get_height(2) == tree.get_root_height());
                REQUIRE(tree.get_height(2) == 1.0);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);
        REQUIRE(op.get_number_of_attempts_for_correction() == (niterations - 100));

        REQUIRE(internal0_height_summary.sample_size() == nsamples);
        REQUIRE(internal1_height_summary.sample_size() == nsamples);
        
        UniformDistribution prior(0.0, 1.0);

        std::cout << "internal0 mean: " << internal0_height_summary.mean() << "\n";
        std::cout << "internal1 mean: " << internal1_height_summary.mean() << "\n";

        double eps = 0.001;
        REQUIRE(internal0_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal0_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
        REQUIRE(internal1_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal1_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::merge with 3 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge with 3 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(30);

        // MOVE FROM: ((A:0.1,B:0.1):0.2,C:0.3)
        // MOVE TO:   (A:0.3,B:0.3,C:0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1 = 1
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1 * 1 * 1/3 * 1/0.3
        //                  = 1/(3*0.3)
        // HR = 1/(3*0.3) / 1 = 1/(3*0.3)
        
        unsigned int nsamples = 50;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.1);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

            internal0->add_child(leaf0);
            internal0->add_child(leaf1);
            root->add_child(internal0);
            root->add_child(leaf2);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 0);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));

            double ln_hastings = op.propose(rng,
                    &tree);
            double exp_ln_hastings = std::log(1.0 / (3.0 * 0.3));
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 1);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_height(0) == 0.3);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 0))).epsilon(1e-8));
        }
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::split with 3 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing split with 3 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(30);

        // MOVE FROM: (A:0.3,B:0.3,C:0.3)
        // MOVE TO:   ((A:<0.3,B:<0.3):<0.3,C:0.3)
        //            OR
        // MOVE TO:   ((A:<0.3,C:<0.3):<0.3,B:0.3)
        //            OR
        // MOVE TO:   ((B:<0.3,C:<0.3):<0.3,C:0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1 * 1 * 1/3 * 1/0.3
        //                  = 1/(3*0.3)
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1 = 1
        // HR = 1 / 1/(3*0.3) = (3*0.3)
        
        unsigned int nsamples = 10000;
        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

            root->add_child(leaf0);
            root->add_child(leaf1);
            root->add_child(leaf2);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_number_of_node_heights() == 1);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 0))).epsilon(1e-8));

            double ln_hastings = op.propose(rng,
                    &tree);
            double exp_ln_hastings = std::log(3.0 * 0.3);
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 0);
            REQUIRE(tree.get_height(1) == 0.3);
            REQUIRE(tree.get_height(0) < 0.3);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));
            if (tree.get_root_ptr()->is_child("leaf2")) {
                ++count_01;
            }
            if (tree.get_root_ptr()->is_child("leaf1")) {
                ++count_02;
            }
            if (tree.get_root_ptr()->is_child("leaf0")) {
                ++count_12;
            }
        }
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        double eps = 0.01;
        std::cout << "Freq of ((0,1),2): " << count_01 / (double)nsamples << "\n";
        std::cout << "Freq of ((0,2),1): " << count_02 / (double)nsamples << "\n";
        std::cout << "Freq of ((1,2),0): " << count_12 / (double)nsamples << "\n";
        REQUIRE(count_01 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(count_02 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(count_12 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler with 3 leaves and fixed root",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing 3 leaves with fixed root") {
        RandomNumberGenerator rng = RandomNumberGenerator(18);

        double root_ht = 0.2;
        std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.1);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        SplitLumpNodesRevJumpSampler<Node> op;

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int count_012 = 0;
        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 2000000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            // std::cout << tree.to_parentheses() << "\n";
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                if (tree.get_root_ptr()->get_number_of_children() == 3) {
                    ++count_012;
                }
                else {
                    if (tree.get_root_ptr()->is_child("leaf0")) {
                        ++count_12;
                    }
                    if (tree.get_root_ptr()->is_child("leaf1")) {
                        ++count_02;
                    }
                    if (tree.get_root_ptr()->is_child("leaf2")) {
                        ++count_01;
                    }
                    internal_height_summary.add_sample(tree.get_height(0));
                }
                REQUIRE(tree.get_root_height() == root_ht);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);

        REQUIRE((count_01 + count_02 + count_12 + count_012) == nsamples);
        REQUIRE(internal_height_summary.sample_size() == (count_01 + count_02 + count_12));

        double freq_012 = count_012 / (double)nsamples;
        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "Freq of (0,1,2): " << freq_012 << "\n";
        std::cout << "Freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "Freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "Freq of ((1,2),0): " << freq_12 << "\n";

        double eps = 0.001;

        REQUIRE(freq_012 == Approx(0.25).epsilon(eps));
        REQUIRE(freq_01 == Approx(0.25).epsilon(eps));
        REQUIRE(freq_02 == Approx(0.25).epsilon(eps));
        REQUIRE(freq_12 == Approx(0.25).epsilon(eps));
        
        UniformDistribution prior(0.0, root_ht);

        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler with 3 leaves, fixed root and operate_plus",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing 3 leaves, fixed root and operate_plus") {
        RandomNumberGenerator rng = RandomNumberGenerator(19191919);

        double root_ht = 0.2;
        std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
        std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.1);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);

        internal0->add_child(leaf0);
        internal0->add_child(leaf1);
        root->add_child(internal0);
        root->add_child(leaf2);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        SplitLumpNodesRevJumpSampler<Node> op;
        std::shared_ptr< NodeHeightSlideBumpMover<Node> > node_height_op = std::make_shared<NodeHeightSlideBumpMover<Node> >();
        std::vector< std::shared_ptr< GeneralTreeOperatorTemplate<Node> > > other_ops;
        other_ops.push_back(node_height_op);

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int count_012 = 0;
        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;

        unsigned int niterations = 1000000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            // std::cout << tree.to_parentheses() << "\n";
            op.operate_plus(rng, &tree, other_ops, 1, 2);
            if ((i + 1) % sample_freq == 0) {
                if (tree.get_root_ptr()->get_number_of_children() == 3) {
                    ++count_012;
                }
                else {
                    if (tree.get_root_ptr()->is_child("leaf0")) {
                        ++count_12;
                    }
                    if (tree.get_root_ptr()->is_child("leaf1")) {
                        ++count_02;
                    }
                    if (tree.get_root_ptr()->is_child("leaf2")) {
                        ++count_01;
                    }
                    internal_height_summary.add_sample(tree.get_height(0));
                }
                REQUIRE(tree.get_root_height() == root_ht);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << node_height_op->to_string();

        REQUIRE((count_01 + count_02 + count_12 + count_012) == nsamples);
        REQUIRE(internal_height_summary.sample_size() == (count_01 + count_02 + count_12));

        double freq_012 = count_012 / (double)nsamples;
        double freq_01 = count_01 / (double)nsamples;
        double freq_02 = count_02 / (double)nsamples;
        double freq_12 = count_12 / (double)nsamples;
        std::cout << "Freq of (0,1,2): " << freq_012 << "\n";
        std::cout << "Freq of ((0,1),2): " << freq_01 << "\n";
        std::cout << "Freq of ((0,2),1): " << freq_02 << "\n";
        std::cout << "Freq of ((1,2),0): " << freq_12 << "\n";

        double eps = 0.001;

        REQUIRE(freq_012 == Approx(0.25).epsilon(eps));
        REQUIRE(freq_01 == Approx(0.25).epsilon(eps));
        REQUIRE(freq_02 == Approx(0.25).epsilon(eps));
        REQUIRE(freq_12 == Approx(0.25).epsilon(eps));
        
        UniformDistribution prior(0.0, root_ht);

        REQUIRE(internal_height_summary.mean() == Approx(prior.get_mean()).epsilon(eps));
        REQUIRE(internal_height_summary.variance() == Approx(prior.get_variance()).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler with 4 leaves and fixed root",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing 4 leaves with fixed root") {
        RandomNumberGenerator rng = RandomNumberGenerator(20);

        double root_ht = 0.5;
        std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        root->add_child(leaf0);
        root->add_child(leaf1);
        root->add_child(leaf2);
        root->add_child(leaf3);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        SplitLumpNodesRevJumpSampler<Node> op;

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int count_0123 = 0;
        unsigned int count_0_ = 0;
        unsigned int count_1_ = 0;
        unsigned int count_2_ = 0;
        unsigned int count_3_ = 0;
        unsigned int count_01_ = 0;
        unsigned int count_02_ = 0;
        unsigned int count_03_ = 0;
        unsigned int count_12_ = 0;
        unsigned int count_13_ = 0;
        unsigned int count_23_ = 0;
        unsigned int count_01_23 = 0;
        unsigned int count_02_13 = 0;
        unsigned int count_03_12 = 0;
        unsigned int count_gen_01_23 = 0;
        unsigned int count_gen_02_13 = 0;
        unsigned int count_gen_03_12 = 0;
        unsigned int count_gen_01_2_3 = 0;
        unsigned int count_gen_01_3_2 = 0;
        unsigned int count_gen_02_1_3 = 0;
        unsigned int count_gen_02_3_1 = 0;
        unsigned int count_gen_03_1_2 = 0;
        unsigned int count_gen_03_2_1 = 0;
        unsigned int count_gen_12_0_3 = 0;
        unsigned int count_gen_12_3_0 = 0;
        unsigned int count_gen_13_0_2 = 0;
        unsigned int count_gen_13_2_0 = 0;
        unsigned int count_gen_23_0_1 = 0;
        unsigned int count_gen_23_1_0 = 0;
        unsigned int count_3_heights = 0;
        unsigned int count_2_heights = 0;

        unsigned int niterations = 5000000;
        unsigned int sample_freq = 20;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                if (tree.get_number_of_node_heights() == 1) {
                    ++count_0123;
                    REQUIRE(tree.get_root_ptr()->get_number_of_children() == 4);
                }
                else if (tree.get_number_of_node_heights() == 2) {
                    ++count_2_heights;
                    if (tree.get_root_ptr()->get_number_of_children() == 2) {
                        if (tree.get_root_ptr()->get_child(0)->is_leaf() ||
                                tree.get_root_ptr()->get_child(1)->is_leaf()) {
                            if(tree.get_root_ptr()->is_child("leaf0")) {
                                ++count_0_;
                            }
                            else if(tree.get_root_ptr()->is_child("leaf1")) {
                                ++count_1_;
                            }
                            else if(tree.get_root_ptr()->is_child("leaf2")) {
                                ++count_2_;
                            }
                            else if(tree.get_root_ptr()->is_child("leaf3")) {
                                ++count_3_;
                            }
                            else {
                                REQUIRE(0 == 1);
                            }
                        }
                        else {
                            if (
                                    (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(0)->is_child("leaf1"))
                                    ||
                                    (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                               ) {
                                ++count_01_23;
                            }
                            else if (
                                    (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(0)->is_child("leaf2"))
                                    ||
                                    (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                               ) {
                                ++count_02_13;
                            }
                            else if (
                                    (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(0)->is_child("leaf3"))
                                    ||
                                    (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                               ) {
                                ++count_03_12;
                            }
                            else {
                                REQUIRE(0 == 1);
                            }
                        }
                    }
                    else if (tree.get_root_ptr()->get_number_of_children() == 3) {
                        if (
                                tree.get_root_ptr()->is_child("leaf0") &&
                                tree.get_root_ptr()->is_child("leaf1")
                            )
                        {
                            ++count_23_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf0") &&
                                tree.get_root_ptr()->is_child("leaf2")
                            )
                        {
                            ++count_13_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf0") &&
                                tree.get_root_ptr()->is_child("leaf3")
                            )
                        {
                            ++count_12_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf1") &&
                                tree.get_root_ptr()->is_child("leaf2")
                            )
                        {
                            ++count_03_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf1") &&
                                tree.get_root_ptr()->is_child("leaf3")
                            )
                        {
                            ++count_02_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf2") &&
                                tree.get_root_ptr()->is_child("leaf3")
                            )
                        {
                            ++count_01_;

                        }
                        else {
                            REQUIRE(0 == 1);
                        }
                    }
                    else {
                        REQUIRE(0 == 1);
                    }
                }
                else if (tree.get_number_of_node_heights() == 3) {
                    ++count_3_heights;
                    if ((! tree.get_root_ptr()->get_child(0)->is_leaf()) &&
                        (! tree.get_root_ptr()->get_child(1)->is_leaf())) {
                        if (
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(0)->is_child("leaf1"))
                                ||
                                (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                           ) {
                            ++count_gen_01_23;
                        }
                        else if (
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(0)->is_child("leaf2"))
                                ||
                                (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                           ) {
                            ++count_gen_02_13;
                        }
                        else if (
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(0)->is_child("leaf3"))
                                ||
                                (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                           ) {
                            ++count_gen_03_12;
                        }
                        else {
                            REQUIRE(0 == 1);
                        }
                    }
                    else {
                        // general ladderized topology
                        if (tree.get_root_ptr()->is_child("leaf3") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf2") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                            )
                        {
                            ++count_gen_01_2_3;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf2") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf3") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                            )
                        {
                            ++count_gen_01_3_2;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf3") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf1") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                            )
                        {
                            ++count_gen_02_1_3;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf1") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf3") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                            )
                        {
                            ++count_gen_02_3_1;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf2") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf1") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                            )
                        {
                            ++count_gen_03_1_2;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf1") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf2") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                            )
                        {
                            ++count_gen_03_2_1;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf3") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf0"))
                            )
                        {
                            ++count_gen_12_0_3;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf0") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf3") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                            )
                        {
                            ++count_gen_12_3_0;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf2") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf0"))
                            )
                        {
                            ++count_gen_13_0_2;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf0") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf2") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                            )
                        {
                            ++count_gen_13_2_0;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf1") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf0"))
                            )
                        {
                            ++count_gen_23_0_1;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf0") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf1") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                            )
                        {
                            ++count_gen_23_1_0;
                        }
                        else {
                            REQUIRE(0 == 1);
                        }
                    }
                }
                else {
                    REQUIRE(0 == 1);
                }
                REQUIRE(tree.get_root_height() == root_ht);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);

        REQUIRE((count_0123 + count_2_heights + count_3_heights) == nsamples);
        REQUIRE((count_01_23 +
                count_02_13 +
                count_03_12 +
                count_0_ +
                count_1_ +
                count_2_ +
                count_3_ +
                count_01_ +
                count_02_ +
                count_03_ +
                count_12_ +
                count_13_ +
                count_23_) == count_2_heights);
        REQUIRE((count_gen_01_23 +
                count_gen_02_13 +
                count_gen_03_12 +
                count_gen_01_2_3 +
                count_gen_01_3_2 +
                count_gen_02_1_3 +
                count_gen_02_3_1 +
                count_gen_03_1_2 +
                count_gen_03_2_1 +
                count_gen_12_0_3 +
                count_gen_12_3_0 +
                count_gen_13_0_2 +
                count_gen_13_2_0 +
                count_gen_23_0_1 +
                count_gen_23_1_0) == count_3_heights);

        double freq_2_heights = count_2_heights / (double)nsamples;
        double freq_3_heights = count_3_heights / (double)nsamples;
        double freq_0123 = count_0123 / (double)nsamples;
        double freq_01_23 = count_01_23 / (double)nsamples;
        double freq_02_13 = count_02_13 / (double)nsamples;
        double freq_03_12 = count_03_12 / (double)nsamples;
        double freq_gen_01_23 = count_gen_01_23 / (double)nsamples;
        double freq_gen_02_13 = count_gen_02_13 / (double)nsamples;
        double freq_gen_03_12 = count_gen_03_12 / (double)nsamples;
        double freq_0_ = count_0_ / (double)nsamples;
        double freq_1_ = count_1_ / (double)nsamples;
        double freq_2_ = count_2_ / (double)nsamples;
        double freq_3_ = count_3_ / (double)nsamples;
        double freq_01_ = count_01_ / (double)nsamples;
        double freq_02_ = count_02_ / (double)nsamples;
        double freq_03_ = count_03_ / (double)nsamples;
        double freq_12_ = count_12_ / (double)nsamples;
        double freq_13_ = count_13_ / (double)nsamples;
        double freq_23_ = count_23_ / (double)nsamples;
        double freq_gen_01_2_3 = count_gen_01_2_3 / (double)nsamples;
        double freq_gen_01_3_2 = count_gen_01_3_2 / (double)nsamples;
        double freq_gen_02_1_3 = count_gen_02_1_3 / (double)nsamples;
        double freq_gen_02_3_1 = count_gen_02_3_1 / (double)nsamples;
        double freq_gen_03_1_2 = count_gen_03_1_2 / (double)nsamples;
        double freq_gen_03_2_1 = count_gen_03_2_1 / (double)nsamples;
        double freq_gen_12_0_3 = count_gen_12_0_3 / (double)nsamples;
        double freq_gen_12_3_0 = count_gen_12_3_0 / (double)nsamples;
        double freq_gen_13_0_2 = count_gen_13_0_2 / (double)nsamples;
        double freq_gen_13_2_0 = count_gen_13_2_0 / (double)nsamples;
        double freq_gen_23_0_1 = count_gen_23_0_1 / (double)nsamples;
        double freq_gen_23_1_0 = count_gen_23_1_0 / (double)nsamples;

        double exp_freq = 1.0/29.0;

        std::cout << "Freq of (0,1,2,3): " << freq_0123 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((0,1),2,3): " << freq_01_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((0,2),1,3): " << freq_02_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((0,3),1,2): " << freq_03_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((1,2),0,3): " << freq_12_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((1,3),0,2): " << freq_13_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((2,3),0,1): " << freq_23_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (0,(1,2,3)): " << freq_0_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (1,(0,2,3)): " << freq_1_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (2,(1,0,3)): " << freq_2_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (3,(1,2,0)): " << freq_3_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of shared ((0,1),(2,3)): " << freq_01_23 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of shared ((0,2),(1,3)): " << freq_02_13 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of shared ((0,3),(1,2)): " << freq_03_12 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen ((0,1),(2,3)): " << freq_gen_01_23 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen ((0,2),(1,3)): " << freq_gen_02_13 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen ((0,3),(1,2)): " << freq_gen_03_12 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,1),2),3): " << freq_gen_01_2_3 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,1),3),2): " << freq_gen_01_3_2 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,2),1),3): " << freq_gen_02_1_3 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,2),3),1): " << freq_gen_02_3_1 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,3),1),2): " << freq_gen_03_1_2 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,3),2),1): " << freq_gen_03_2_1 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,2),0),3): " << freq_gen_12_0_3 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,2),3),0): " << freq_gen_12_3_0 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,3),0),2): " << freq_gen_13_0_2 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,3),2),0): " << freq_gen_13_2_0 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((2,3),0),1): " << freq_gen_23_0_1 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((2,3),1),0): " << freq_gen_23_1_0 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of 2 heights: " << freq_2_heights << " (expected " << 13 * exp_freq << ")\n";
        std::cout << "Freq of 3 heights: " << freq_3_heights << " (expected " << 15 * exp_freq << ")\n";

        double eps = 0.001;

        REQUIRE(freq_2_heights == Approx(13 * exp_freq).epsilon(eps));
        REQUIRE(freq_3_heights == Approx(15 * exp_freq).epsilon(eps));

        REQUIRE(freq_0123 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_01_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_02_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_03_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_12_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_13_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_23_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_0_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_1_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_2_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_3_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_01_23 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_02_13 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_03_12 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_01_23 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_02_13 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_03_12 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_01_2_3 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_01_3_2 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_02_1_3 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_02_3_1 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_03_1_2 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_03_2_1 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_12_0_3 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_12_3_0 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_13_0_2 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_13_2_0 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_23_0_1 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_23_1_0 == Approx(exp_freq).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler with 4 leaves, fixed root, and operate_plus",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing 4 leaves with fixed root and operate_plus") {
        RandomNumberGenerator rng = RandomNumberGenerator(21);

        double root_ht = 0.5;
        std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

        root->add_child(leaf0);
        root->add_child(leaf1);
        root->add_child(leaf2);
        root->add_child(leaf3);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        SplitLumpNodesRevJumpSampler<Node> op;
        std::shared_ptr< NodeHeightSlideBumpMover<Node> > node_height_op = std::make_shared<NodeHeightSlideBumpMover<Node> >();
        /* std::shared_ptr< NeighborHeightNodeSwap<Node> > node_swap_op = std::make_shared<NeighborHeightNodeSwap<Node> >(); */
        std::vector< std::shared_ptr< GeneralTreeOperatorTemplate<Node> > > other_ops;
        other_ops.push_back(node_height_op);
        /* other_ops.push_back(node_swap_op); */

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        SampleSummarizer<double> internal_height_summary;

        unsigned int count_0123 = 0;
        unsigned int count_0_ = 0;
        unsigned int count_1_ = 0;
        unsigned int count_2_ = 0;
        unsigned int count_3_ = 0;
        unsigned int count_01_ = 0;
        unsigned int count_02_ = 0;
        unsigned int count_03_ = 0;
        unsigned int count_12_ = 0;
        unsigned int count_13_ = 0;
        unsigned int count_23_ = 0;
        unsigned int count_01_23 = 0;
        unsigned int count_02_13 = 0;
        unsigned int count_03_12 = 0;
        unsigned int count_gen_01_23 = 0;
        unsigned int count_gen_02_13 = 0;
        unsigned int count_gen_03_12 = 0;
        unsigned int count_gen_01_2_3 = 0;
        unsigned int count_gen_01_3_2 = 0;
        unsigned int count_gen_02_1_3 = 0;
        unsigned int count_gen_02_3_1 = 0;
        unsigned int count_gen_03_1_2 = 0;
        unsigned int count_gen_03_2_1 = 0;
        unsigned int count_gen_12_0_3 = 0;
        unsigned int count_gen_12_3_0 = 0;
        unsigned int count_gen_13_0_2 = 0;
        unsigned int count_gen_13_2_0 = 0;
        unsigned int count_gen_23_0_1 = 0;
        unsigned int count_gen_23_1_0 = 0;
        unsigned int count_3_heights = 0;
        unsigned int count_2_heights = 0;

        unsigned int niterations = 1000000;
        unsigned int sample_freq = 10;
        unsigned int nsamples = niterations / sample_freq;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate_plus(rng, &tree, other_ops, 1, 2, 2);
            if ((i + 1) % sample_freq == 0) {
                /* std::cout << "prior: " << tree.get_log_prior_density_value() << "\n"; */
                if (tree.get_number_of_node_heights() == 1) {
                    ++count_0123;
                    REQUIRE(tree.get_root_ptr()->get_number_of_children() == 4);
                }
                else if (tree.get_number_of_node_heights() == 2) {
                    ++count_2_heights;
                    if (tree.get_root_ptr()->get_number_of_children() == 2) {
                        if (tree.get_root_ptr()->get_child(0)->is_leaf() ||
                                tree.get_root_ptr()->get_child(1)->is_leaf()) {
                            if(tree.get_root_ptr()->is_child("leaf0")) {
                                ++count_0_;
                            }
                            else if(tree.get_root_ptr()->is_child("leaf1")) {
                                ++count_1_;
                            }
                            else if(tree.get_root_ptr()->is_child("leaf2")) {
                                ++count_2_;
                            }
                            else if(tree.get_root_ptr()->is_child("leaf3")) {
                                ++count_3_;
                            }
                            else {
                                REQUIRE(0 == 1);
                            }
                        }
                        else {
                            if (
                                    (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(0)->is_child("leaf1"))
                                    ||
                                    (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                               ) {
                                ++count_01_23;
                            }
                            else if (
                                    (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(0)->is_child("leaf2"))
                                    ||
                                    (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                               ) {
                                ++count_02_13;
                            }
                            else if (
                                    (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(0)->is_child("leaf3"))
                                    ||
                                    (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                    tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                               ) {
                                ++count_03_12;
                            }
                            else {
                                REQUIRE(0 == 1);
                            }
                        }
                    }
                    else if (tree.get_root_ptr()->get_number_of_children() == 3) {
                        if (
                                tree.get_root_ptr()->is_child("leaf0") &&
                                tree.get_root_ptr()->is_child("leaf1")
                            )
                        {
                            ++count_23_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf0") &&
                                tree.get_root_ptr()->is_child("leaf2")
                            )
                        {
                            ++count_13_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf0") &&
                                tree.get_root_ptr()->is_child("leaf3")
                            )
                        {
                            ++count_12_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf1") &&
                                tree.get_root_ptr()->is_child("leaf2")
                            )
                        {
                            ++count_03_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf1") &&
                                tree.get_root_ptr()->is_child("leaf3")
                            )
                        {
                            ++count_02_;

                        }
                        else if (
                                tree.get_root_ptr()->is_child("leaf2") &&
                                tree.get_root_ptr()->is_child("leaf3")
                            )
                        {
                            ++count_01_;

                        }
                        else {
                            REQUIRE(0 == 1);
                        }
                    }
                    else {
                        REQUIRE(0 == 1);
                    }
                }
                else if (tree.get_number_of_node_heights() == 3) {
                    ++count_3_heights;
                    if ((! tree.get_root_ptr()->get_child(0)->is_leaf()) &&
                        (! tree.get_root_ptr()->get_child(1)->is_leaf())) {
                        if (
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(0)->is_child("leaf1"))
                                ||
                                (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                           ) {
                            ++count_gen_01_23;
                        }
                        else if (
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(0)->is_child("leaf2"))
                                ||
                                (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                           ) {
                            ++count_gen_02_13;
                        }
                        else if (
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(0)->is_child("leaf3"))
                                ||
                                (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                                tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                           ) {
                            ++count_gen_03_12;
                        }
                        else {
                            REQUIRE(0 == 1);
                        }
                    }
                    else {
                        // general ladderized topology
                        if (tree.get_root_ptr()->is_child("leaf3") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf2") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                            )
                        {
                            ++count_gen_01_2_3;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf2") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf3") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                            )
                        {
                            ++count_gen_01_3_2;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf3") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf1") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                            )
                        {
                            ++count_gen_02_1_3;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf1") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf3") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                            )
                        {
                            ++count_gen_02_3_1;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf2") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf1") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                            )
                        {
                            ++count_gen_03_1_2;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf1") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf2") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                            )
                        {
                            ++count_gen_03_2_1;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf3") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf0"))
                            )
                        {
                            ++count_gen_12_0_3;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf0") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf3") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                            )
                        {
                            ++count_gen_12_3_0;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf2") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf0"))
                            )
                        {
                            ++count_gen_13_0_2;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf0") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf2") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                            )
                        {
                            ++count_gen_13_2_0;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf1") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf0") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf0"))
                            )
                        {
                            ++count_gen_23_0_1;
                        }
                        else if (tree.get_root_ptr()->is_child("leaf0") && 
                                (tree.get_root_ptr()->get_child(0)->is_child("leaf1") ||
                                 tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                            )
                        {
                            ++count_gen_23_1_0;
                        }
                        else {
                            REQUIRE(0 == 1);
                        }
                    }
                }
                else {
                    REQUIRE(0 == 1);
                }
                REQUIRE(tree.get_root_height() == root_ht);
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();
        std::cout << node_height_op->to_string();
        /* std::cout << node_swap_op->to_string(); */


        REQUIRE((count_0123 + count_2_heights + count_3_heights) == nsamples);
        REQUIRE((count_01_23 +
                count_02_13 +
                count_03_12 +
                count_0_ +
                count_1_ +
                count_2_ +
                count_3_ +
                count_01_ +
                count_02_ +
                count_03_ +
                count_12_ +
                count_13_ +
                count_23_) == count_2_heights);
        REQUIRE((count_gen_01_23 +
                count_gen_02_13 +
                count_gen_03_12 +
                count_gen_01_2_3 +
                count_gen_01_3_2 +
                count_gen_02_1_3 +
                count_gen_02_3_1 +
                count_gen_03_1_2 +
                count_gen_03_2_1 +
                count_gen_12_0_3 +
                count_gen_12_3_0 +
                count_gen_13_0_2 +
                count_gen_13_2_0 +
                count_gen_23_0_1 +
                count_gen_23_1_0) == count_3_heights);

        double freq_2_heights = count_2_heights / (double)nsamples;
        double freq_3_heights = count_3_heights / (double)nsamples;
        double freq_0123 = count_0123 / (double)nsamples;
        double freq_01_23 = count_01_23 / (double)nsamples;
        double freq_02_13 = count_02_13 / (double)nsamples;
        double freq_03_12 = count_03_12 / (double)nsamples;
        double freq_gen_01_23 = count_gen_01_23 / (double)nsamples;
        double freq_gen_02_13 = count_gen_02_13 / (double)nsamples;
        double freq_gen_03_12 = count_gen_03_12 / (double)nsamples;
        double freq_0_ = count_0_ / (double)nsamples;
        double freq_1_ = count_1_ / (double)nsamples;
        double freq_2_ = count_2_ / (double)nsamples;
        double freq_3_ = count_3_ / (double)nsamples;
        double freq_01_ = count_01_ / (double)nsamples;
        double freq_02_ = count_02_ / (double)nsamples;
        double freq_03_ = count_03_ / (double)nsamples;
        double freq_12_ = count_12_ / (double)nsamples;
        double freq_13_ = count_13_ / (double)nsamples;
        double freq_23_ = count_23_ / (double)nsamples;
        double freq_gen_01_2_3 = count_gen_01_2_3 / (double)nsamples;
        double freq_gen_01_3_2 = count_gen_01_3_2 / (double)nsamples;
        double freq_gen_02_1_3 = count_gen_02_1_3 / (double)nsamples;
        double freq_gen_02_3_1 = count_gen_02_3_1 / (double)nsamples;
        double freq_gen_03_1_2 = count_gen_03_1_2 / (double)nsamples;
        double freq_gen_03_2_1 = count_gen_03_2_1 / (double)nsamples;
        double freq_gen_12_0_3 = count_gen_12_0_3 / (double)nsamples;
        double freq_gen_12_3_0 = count_gen_12_3_0 / (double)nsamples;
        double freq_gen_13_0_2 = count_gen_13_0_2 / (double)nsamples;
        double freq_gen_13_2_0 = count_gen_13_2_0 / (double)nsamples;
        double freq_gen_23_0_1 = count_gen_23_0_1 / (double)nsamples;
        double freq_gen_23_1_0 = count_gen_23_1_0 / (double)nsamples;

        double exp_freq = 1.0/29.0;

        std::cout << "Freq of (0,1,2,3): " << freq_0123 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((0,1),2,3): " << freq_01_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((0,2),1,3): " << freq_02_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((0,3),1,2): " << freq_03_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((1,2),0,3): " << freq_12_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((1,3),0,2): " << freq_13_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of ((2,3),0,1): " << freq_23_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (0,(1,2,3)): " << freq_0_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (1,(0,2,3)): " << freq_1_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (2,(1,0,3)): " << freq_2_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of (3,(1,2,0)): " << freq_3_ << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of shared ((0,1),(2,3)): " << freq_01_23 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of shared ((0,2),(1,3)): " << freq_02_13 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of shared ((0,3),(1,2)): " << freq_03_12 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen ((0,1),(2,3)): " << freq_gen_01_23 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen ((0,2),(1,3)): " << freq_gen_02_13 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen ((0,3),(1,2)): " << freq_gen_03_12 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,1),2),3): " << freq_gen_01_2_3 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,1),3),2): " << freq_gen_01_3_2 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,2),1),3): " << freq_gen_02_1_3 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,2),3),1): " << freq_gen_02_3_1 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,3),1),2): " << freq_gen_03_1_2 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((0,3),2),1): " << freq_gen_03_2_1 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,2),0),3): " << freq_gen_12_0_3 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,2),3),0): " << freq_gen_12_3_0 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,3),0),2): " << freq_gen_13_0_2 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((1,3),2),0): " << freq_gen_13_2_0 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((2,3),0),1): " << freq_gen_23_0_1 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of gen (((2,3),1),0): " << freq_gen_23_1_0 << " (expected " << exp_freq << ")\n";
        std::cout << "Freq of 2 heights: " << freq_2_heights << " (expected " << 13 * exp_freq << ")\n";
        std::cout << "Freq of 3 heights: " << freq_3_heights << " (expected " << 15 * exp_freq << ")\n";

        double eps = 0.001;

        REQUIRE(freq_2_heights == Approx(13 * exp_freq).epsilon(eps));
        REQUIRE(freq_3_heights == Approx(15 * exp_freq).epsilon(eps));

        REQUIRE(freq_0123 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_01_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_02_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_03_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_12_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_13_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_23_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_0_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_1_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_2_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_3_ == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_01_23 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_02_13 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_03_12 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_01_23 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_02_13 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_03_12 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_01_2_3 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_01_3_2 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_02_1_3 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_02_3_1 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_03_1_2 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_03_2_1 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_12_0_3 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_12_3_0 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_13_0_2 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_13_2_0 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_23_0_1 == Approx(exp_freq).epsilon(eps));
        REQUIRE(freq_gen_23_1_0 == Approx(exp_freq).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::merge with ladderized tree with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge with ladderized tree with 4") {
        RandomNumberGenerator rng = RandomNumberGenerator(22);

        // MOVE FROM: (((A:0.1,B:0.1):0.1,C:0.2):0.2,D:0.4)
        // MOVE TO:   ((A:0.2,B:0.2,C:0.2):0.2,D:0.4)
        //            OR
        //            ((A:0.1,B:0.1):0.3,C:0.4,D:0.4)
        //
        // HR of move to ((A:0.2,B:0.2,C:0.2):0.2,D:0.4)
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1/2 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/3 * 1/0.2
        //                  = 1/(6*0.2)
        // HR = 1/(6*0.2) / 1/2 = 2/(6*0.2) = 1 / (3*0.2)
        //
        // HR of move to ((A:0.1,B:0.1):0.3,C:0.4,D:0.4)
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1/2 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/3 * 1/0.3
        //                  = 1/(6*0.3)
        // HR = 1/(6*0.3) / 1/2 = 2/(6*0.3) = 1 / (3*0.3)

        unsigned int nsamples = 10000;
        unsigned int root_poly_count = 0;
        unsigned int internal_poly_count = 0;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.4;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.2);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.1);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal0->add_child(leaf0);
            internal0->add_child(leaf1);
            internal1->add_child(leaf2);
            internal1->add_child(internal0);
            root->add_child(leaf3);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                            (1.0 / 0.4) *
                            (1.0 / 0.2)
                            )).epsilon(1e-8));

            double ln_hastings = op.propose(rng,
                    &tree);
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / root_ht)).epsilon(1e-8));
            // Pr(forward move) = pr(merging) * pr(picking node) = 1 * 1/2 = 1/2
            // Pr(reverse move) = pr(splitting) * pr(picking node) * pr(paritioning nodes) * pr(new height)
            // = 1/2 * 1 * 1/3 * 1/height diff = 1/6d
            // HR = 1/6d / 1/2 = 2 / 6d = 1 / 3d
            if (tree.get_root_ptr()->get_number_of_children() == 2) {
                double exp_ln_hastings = std::log(1.0 / (3.0 * 0.2));
                REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
                REQUIRE(tree.get_root_ptr()->is_child("leaf3"));
                REQUIRE(! tree.get_root_ptr()->is_child("leaf2"));
                ++internal_poly_count;
            }
            else if (tree.get_root_ptr()->get_number_of_children() == 3) {
                double exp_ln_hastings = std::log(1.0 / (3.0 * 0.3));
                REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
                REQUIRE(tree.get_root_ptr()->is_child("leaf3"));
                REQUIRE(tree.get_root_ptr()->is_child("leaf2"));
                ++root_poly_count;
            }
            else {
                REQUIRE(0 == 1);
            }
        }
        REQUIRE(root_poly_count + internal_poly_count == nsamples);
        REQUIRE(root_poly_count / (double)nsamples == Approx(0.5).epsilon(0.01));
        REQUIRE(internal_poly_count / (double)nsamples == Approx(0.5).epsilon(0.01));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::split to ladderized tree with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing split to ladderized tree with 4") {
        RandomNumberGenerator rng = RandomNumberGenerator(23);

        // MOVE FROM: ((A:0.2,B:0.2,C:0.2):0.1,D:0.3)
        // MOVE TO:   (((A:<0.2,B:<0.2):?,C:0.2):0.1,D:0.3)
        //            OR
        //            (((A:<0.2,C:<0.2):?,B:0.2):0.1,D:0.3)
        //            OR
        //            (((B:<0.2,C:<0.2):?,A:0.2):0.1,D:0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/3 * 1/(0.2)
        //                  = 1/(6*0.2)
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1/2 = 1/2
        // HR = 1/2 / 1/(6*0.2) = (6*0.2) / 2 = (3*0.2)

        unsigned int nsamples = 10000;
        unsigned int count_01 = 0;
        unsigned int count_02 = 0;
        unsigned int count_12 = 0;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.2);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal1->add_child(leaf0);
            internal1->add_child(leaf1);
            internal1->add_child(leaf2);
            root->add_child(leaf3);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / root_ht)).epsilon(1e-8));

            double ln_hastings = op.propose_split(rng,
                    &tree);
            double exp_ln_hastings = std::log(3.0 * 0.2);
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 3);
            REQUIRE(tree.get_number_of_splittable_heights() == 0);
            REQUIRE(tree.get_root_ptr()->is_child("leaf3"));
            REQUIRE(tree.get_root_ptr()->get_number_of_children() == 2);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                            (1.0 / root_ht) *
                            (1.0 / 0.2)
                            )).epsilon(1e-8));
            if (tree.get_root_ptr()->get_child(0)->is_child("leaf2") ||
                tree.get_root_ptr()->get_child(1)->is_child("leaf2")) {
                ++count_01;
            }
            else if (tree.get_root_ptr()->get_child(0)->is_child("leaf1") ||
                     tree.get_root_ptr()->get_child(1)->is_child("leaf1")) {
                ++count_02;
            }
            else if (tree.get_root_ptr()->get_child(0)->is_child("leaf0") ||
                     tree.get_root_ptr()->get_child(1)->is_child("leaf0")) {
                ++count_12;
            }
            else {
                REQUIRE(0 == 1);
            }
        }
        double eps = 0.01;
        REQUIRE(count_01 + count_02 + count_12 == nsamples);
        REQUIRE(count_01 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(count_02 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(count_12 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::split from root poly to general tree with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing split from root poly to general tree with 4 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(24);

        // MOVE FROM: ((A:0.2,B:0.2):0.1,C:0.3,D:0.3)
        // MOVE TO:   ((A:0.2,B:0.2):0.1,(C:0.2<x<0.3, D:0.2<x<0.3):<0.1)
        //            OR
        //            (((A:0.2,B:0.2):<0.1,C:<0.3),D:0.3)
        //            OR
        //            (((A:0.2,B:0.2):<0.1,D:<0.3),C:0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/3 * 1/(0.3-0.2)
        //                  = 1/(6*0.1)
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1/2 = 1/2
        // HR = 1/2 / 1/(6*0.1) = (6*0.1) / 2 = (3*0.1)

        unsigned int nsamples = 10000;
        unsigned int ladder_count_2 = 0;
        unsigned int ladder_count_3 = 0;
        unsigned int balanced_count = 0;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.2);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal1->add_child(leaf0);
            internal1->add_child(leaf1);
            root->add_child(leaf2);
            root->add_child(leaf3);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / root_ht)).epsilon(1e-8));

            double ln_hastings = op.propose_split(rng,
                    &tree);
            double exp_ln_hastings = std::log(3.0 * 0.1);
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 3);
            REQUIRE(tree.get_number_of_splittable_heights() == 0);
            REQUIRE(tree.get_root_ptr()->get_number_of_children() == 2);
            REQUIRE(tree.get_height(0) == 0.2);
            REQUIRE(tree.get_height(2) == 0.3);
            REQUIRE(
                    ((tree.get_height(1) < 0.3) && (tree.get_height(1) > 0.2))
                    );
            tree.compute_log_likelihood_and_prior(true);
            if (tree.get_root_ptr()->get_child(0)->is_leaf() ||
                tree.get_root_ptr()->get_child(1)->is_leaf()) {
                if (tree.get_root_ptr()->is_child("leaf2")) {
                    ++ladder_count_2;
                }
                else if (tree.get_root_ptr()->is_child("leaf3")) {
                    ++ladder_count_3;
                }
                else {
                    REQUIRE(0 == 1);
                }
                REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                                (1.0 / root_ht) *
                                (1.0 / tree.get_height(1))
                                )).epsilon(1e-8));
            }
            else if ((! tree.get_root_ptr()->get_child(0)->is_leaf()) &&
                (! tree.get_root_ptr()->get_child(1)->is_leaf())) {
                ++balanced_count;
                REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                                (1.0 / root_ht) *
                                (1.0 / root_ht)
                                )).epsilon(1e-8));
            }
            else {
                REQUIRE(0 == 1);
            }
        }
        std::cout << "Freq of (((0,1),2),3): " << ladder_count_3 / (double)nsamples << "\n";
        std::cout << "Freq of (((0,1),3),2): " << ladder_count_2 / (double)nsamples << "\n";
        std::cout << "Freq of ((0,1),(2,3)): " << balanced_count / (double)nsamples << "\n";
        double eps = 0.01;
        REQUIRE(ladder_count_2 + ladder_count_3 + balanced_count == nsamples);
        REQUIRE(ladder_count_2 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(ladder_count_3 / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
        REQUIRE(balanced_count / (double)nsamples == Approx(1.0/3.0).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::split from comb with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing split from comb with 4 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(25);

        // MOVE FROM: (A:0.3,B:0.3,C:0.3,D:0.3)
        // MOVE TO:   13 possible trees: 
        //            (A:0.3,B:0.3,(C:<0.3,D:<0.3):<0.3)
        //            (A:0.3,C:0.3,(B:<0.3,D:<0.3):<0.3)
        //            (C:0.3,B:0.3,(A:<0.3,D:<0.3):<0.3)
        //            (A:0.3,D:0.3,(B:<0.3,C:<0.3):<0.3)
        //            (B:0.3,D:0.3,(A:<0.3,C:<0.3):<0.3)
        //            (C:0.3,D:0.3,(A:<0.3,B:<0.3):<0.3)
        //            (A:0.3,(B:<0.3,C:<0.3,D:<0.3):<0.3)
        //            (B:0.3,(A:<0.3,C:<0.3,D:<0.3):<0.3)
        //            (C:0.3,(B:<0.3,A:<0.3,D:<0.3):<0.3)
        //            (D:0.3,(B:<0.3,C:<0.3,A:<0.3):<0.3)
        //            ((A:<0.3,B:<0.3):<0.3,(C:<0.3,D:<0.3):<0.3)
        //            ((A:<0.3,C:<0.3):<0.3,(B:<0.3,D:<0.3):<0.3)
        //            ((A:<0.3,D:<0.3):<0.3,(C:<0.3,B:<0.3):<0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1 * 1 * 1/13 * 1/0.3
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1/2 * 1 = 1/2
        // HR = 1/2 / 1 / (13*0.3) = (13*0.3) / 2

        unsigned int nsamples = 100000;
        unsigned int balanced_01_23_count = 0;
        unsigned int balanced_02_13_count = 0;
        unsigned int balanced_03_12_count = 0;
        unsigned int root_poly_01_count = 0;
        unsigned int root_poly_02_count = 0;
        unsigned int root_poly_03_count = 0;
        unsigned int root_poly_12_count = 0;
        unsigned int root_poly_13_count = 0;
        unsigned int root_poly_23_count = 0;
        unsigned int internal_poly_0_count = 0;
        unsigned int internal_poly_1_count = 0;
        unsigned int internal_poly_2_count = 0;
        unsigned int internal_poly_3_count = 0;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            root->add_child(leaf0);
            root->add_child(leaf1);
            root->add_child(leaf2);
            root->add_child(leaf3);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 0))).epsilon(1e-8));

            // Calling propose, because split should be only possibility
            double ln_hastings = op.propose(rng,
                    &tree);
            double exp_ln_hastings = std::log((13.0 * 0.3) / 2.0);
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_height(1) == root_ht);
            REQUIRE(tree.get_height(0) < root_ht);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));
            if (tree.get_root_ptr()->get_number_of_children() == 3) {
                if (tree.get_root_ptr()->is_child("leaf2") &&
                    tree.get_root_ptr()->is_child("leaf3")) {
                    ++root_poly_01_count;
                }
                else if (tree.get_root_ptr()->is_child("leaf1") &&
                    tree.get_root_ptr()->is_child("leaf3")) {
                    ++root_poly_02_count;
                }
                else if (tree.get_root_ptr()->is_child("leaf1") &&
                    tree.get_root_ptr()->is_child("leaf2")) {
                    ++root_poly_03_count;
                }
                else if (tree.get_root_ptr()->is_child("leaf0") &&
                    tree.get_root_ptr()->is_child("leaf3")) {
                    ++root_poly_12_count;
                }
                else if (tree.get_root_ptr()->is_child("leaf0") &&
                    tree.get_root_ptr()->is_child("leaf2")) {
                    ++root_poly_13_count;
                }
                else if (tree.get_root_ptr()->is_child("leaf0") &&
                    tree.get_root_ptr()->is_child("leaf1")) {
                    ++root_poly_23_count;
                }
                else {
                    REQUIRE(0 == 1);
                }
            }
            else if (tree.get_root_ptr()->get_number_of_children() == 2) {
                if (tree.get_root_ptr()->get_child(0)->is_leaf() ||
                    tree.get_root_ptr()->get_child(1)->is_leaf()) {
                    if (tree.get_root_ptr()->is_child("leaf0")) {
                        ++internal_poly_0_count;
                    }
                    else if (tree.get_root_ptr()->is_child("leaf1")) {
                        ++internal_poly_1_count;
                    }
                    else if (tree.get_root_ptr()->is_child("leaf2")) {
                        ++internal_poly_2_count;
                    }
                    else if (tree.get_root_ptr()->is_child("leaf3")) {
                        ++internal_poly_3_count;
                    }
                    else {
                        REQUIRE(0 == 1);
                    }
                }
                else if ((! tree.get_root_ptr()->get_child(0)->is_leaf()) &&
                    (! tree.get_root_ptr()->get_child(1)->is_leaf())) {
                    if (
                            (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                             tree.get_root_ptr()->get_child(0)->is_child("leaf1")) ||
                            (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                             tree.get_root_ptr()->get_child(1)->is_child("leaf1"))
                        ) {
                        ++balanced_01_23_count;
                    }
                    else if (
                            (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                             tree.get_root_ptr()->get_child(0)->is_child("leaf2")) ||
                            (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                             tree.get_root_ptr()->get_child(1)->is_child("leaf2"))
                        ) {
                        ++balanced_02_13_count;
                    }
                    else if (
                            (tree.get_root_ptr()->get_child(0)->is_child("leaf0") &&
                             tree.get_root_ptr()->get_child(0)->is_child("leaf3")) ||
                            (tree.get_root_ptr()->get_child(1)->is_child("leaf0") &&
                             tree.get_root_ptr()->get_child(1)->is_child("leaf3"))
                        ) {
                        ++balanced_03_12_count;
                    }
                    else {
                        REQUIRE(0 == 1);
                    }
                }
                else {
                    REQUIRE(0 == 1);
                }
            }
            else {
                REQUIRE(0 == 1);
            }
        }

        double eps = 0.003;
        REQUIRE(balanced_01_23_count +
                balanced_02_13_count +
                balanced_03_12_count +
                internal_poly_0_count +
                internal_poly_1_count +
                internal_poly_2_count +
                internal_poly_3_count +
                root_poly_01_count +
                root_poly_02_count +
                root_poly_03_count +
                root_poly_12_count +
                root_poly_13_count +
                root_poly_23_count == nsamples);
        // There are 3 balanced-shared node trees
        //           6 trees with trichotomy at root
        //           4 trees with trichotomy that is a child of the root
        std::cout << "Freq of ((0,1),(2,3)): " << balanced_01_23_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,2),(1,3)): " << balanced_02_13_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,3),(1,2)): " << balanced_03_12_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,1,2),3): "   << internal_poly_3_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,1,3),2): "   << internal_poly_2_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,3,2),1): "   << internal_poly_1_count / (double)nsamples << "\n";
        std::cout << "Freq of ((3,1,2),0): "   << internal_poly_0_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,1),2,3): "   << root_poly_01_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,2),1,3): "   << root_poly_02_count / (double)nsamples << "\n";
        std::cout << "Freq of ((0,3),2,1): "   << root_poly_03_count / (double)nsamples << "\n";
        std::cout << "Freq of ((1,2),0,3): "   << root_poly_12_count / (double)nsamples << "\n";
        std::cout << "Freq of ((1,3),0,2): "   << root_poly_13_count / (double)nsamples << "\n";
        std::cout << "Freq of ((2,3),0,1): "   << root_poly_23_count / (double)nsamples << "\n";
        REQUIRE(balanced_01_23_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(balanced_02_13_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(balanced_03_12_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(internal_poly_0_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(internal_poly_1_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(internal_poly_2_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(internal_poly_3_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(root_poly_01_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(root_poly_02_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(root_poly_03_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(root_poly_12_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(root_poly_13_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
        REQUIRE(root_poly_23_count / (double)nsamples == Approx(1.0/13.0).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::merge from balanced to comb with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge from balanced to comb with 4 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(26);

        // MOVE FROM: ((A:0.2,B:0.2):0.1,(C:0.2, D:0.2):0.1)
        // MOVE TO:   (A:0.3,B:0.3,C:0.3,D:0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1/2 * 1 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1 * 1 * 1/13 * 1/0.3
        //                  there are 13 ways to split up the root polytomy
        // HR = 1/(13*0.3) / 1/2 = 2/(13*0.3)

        unsigned int nsamples = 20;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.2);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.2);
            internal0->set_height_parameter(internal1->get_height_parameter());
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal0->add_child(leaf0);
            internal0->add_child(leaf1);
            internal1->add_child(leaf2);
            internal1->add_child(leaf3);
            root->add_child(internal0);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_height(0) == 0.2);
            REQUIRE(tree.get_height(1) == 0.3);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));

            double ln_hastings = op.propose_merge(rng,
                    &tree,
                    false);
            double exp_ln_hastings = std::log(2.0 / (13.0 * 0.3));
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 1);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_root_ptr()->get_number_of_children() == 4);
            REQUIRE(tree.get_height(0) == 0.3);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 0))).epsilon(1e-8));
        }
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::merge from internal poly to comb with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge from internal poly to comb with 4 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(27);

        // MOVE FROM: ((A:0.2,B:0.2,C:0.2):0.1,D:0.3)
        // MOVE TO:   (A:0.3,B:0.3,C:0.3,D:0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1/3 * 1 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1 * 1 * 1/13 * 1/0.3
        //                  there are 13 ways to split up the root polytomy
        // HR = 1/(13*0.3) / 1/2 = 2/(13*0.3)

        unsigned int nsamples = 20;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.2);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal0->add_child(leaf0);
            internal0->add_child(leaf1);
            internal0->add_child(leaf2);
            root->add_child(leaf3);
            root->add_child(internal0);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));

            double ln_hastings = op.propose_merge(rng,
                    &tree,
                    false);
            double exp_ln_hastings = std::log(2.0 / (13.0 * 0.3));
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 1);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_root_ptr()->get_number_of_children() == 4);
            REQUIRE(tree.get_height(0) == 0.3);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 0))).epsilon(1e-8));
        }
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::merge from root poly to comb with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge from root poly to comb with 4 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(28);

        // MOVE FROM: ((A:0.2,B:0.2):0.1,C:0.3,D:0.3)
        // MOVE TO:   (A:0.3,B:0.3,C:0.3,D:0.3)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1/2 * 1 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1 * 1 * 1/13 * 1/0.3
        //                  There are 13 possible ways to break up the root polytomy
        //                  = 1/(13*0.3)
        // HR = 1/(13*0.3) / 1/2 = 2/(13*0.3)

        unsigned int nsamples = 20;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.2);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal0->add_child(leaf0);
            internal0->add_child(leaf1);
            root->add_child(leaf2);
            root->add_child(leaf3);
            root->add_child(internal0);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));

            double ln_hastings = op.propose_merge(rng,
                    &tree,
                    false);
            double exp_ln_hastings = std::log(2.0 / (13.0 * 0.3));
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 1);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_root_ptr()->get_number_of_children() == 4);
            REQUIRE(tree.get_height(0) == 0.3);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 0))).epsilon(1e-8));
        }
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::split from shared to balanced with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing split from shared to balanced with 4 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(29);

        // MOVE FROM: ((A:0.2,B:0.2):0.1,(C:0.2, D:0.2):0.1)
        // MOVE TO:   ((A:<0.2,B:<0.2):>0.1,(C:0.2, D:0.2):0.1)
        //            OR
        //            ((A:0.2,B:0.2):0.1,(C:<0.2, D:<0.2):>0.1)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/2 * 1/0.2
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1/2 = 1/2
        // HR = 1/2 / 1/(4*0.2) = 2*0.2

        unsigned int split0_count = 0;
        unsigned int split1_count = 0;
        unsigned int nsamples = 10000;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.3;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.2);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.2);
            internal0->set_height_parameter(internal1->get_height_parameter());
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal0->add_child(leaf0);
            internal0->add_child(leaf1);
            internal1->add_child(leaf2);
            internal1->add_child(leaf3);
            root->add_child(internal0);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_number_of_node_heights() == 2);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));

            double ln_hastings = op.propose_split(rng,
                    &tree);
            double exp_ln_hastings = std::log(2.0 * 0.2);
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_number_of_node_heights() == 3);
            REQUIRE(tree.get_number_of_splittable_heights() == 0);
            REQUIRE(tree.get_root_ptr()->get_number_of_children() == 2);
            REQUIRE(tree.get_height(2) == 0.3);
            REQUIRE(tree.get_height(1) == 0.2);
            REQUIRE(tree.get_height(0) < 0.2);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 2))).epsilon(1e-8));
            if (tree.get_root_ptr()->get_child(0)->get_height() < 0.2) {
                ++split0_count;
            }
            else if (tree.get_root_ptr()->get_child(1)->get_height() < 0.2) {
                ++split1_count;
            }
            else {
                REQUIRE(0 == 1);
            }
        }
        REQUIRE(split0_count + split1_count == nsamples);
        double eps = 0.01;
        REQUIRE(split0_count / (double)nsamples == Approx(0.5).epsilon(eps));
        REQUIRE(split1_count / (double)nsamples == Approx(0.5).epsilon(eps));
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::merge from balanced general with 4 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge from balanced general with 4 leaves") {
        RandomNumberGenerator rng = RandomNumberGenerator(30);

        // MOVE FROM: ((A:0.2,B:0.2):0.2,(C:0.1, D:0.1):0.3)
        // MOVE TO:   ((A:0.2,B:0.2):0.2,(C:0.2, D:0.2):0.2)
        //            OR
        //            (A:0.4,B:0.4,(C:0.1, D:0.1):0.3)
        //
        // HR of move to ((A:0.2,B:0.2):0.2,(C:0.2, D:0.2):0.2)
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1/2 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/2 * 1/0.2
        //                  = 1/(4*0.2)
        // HR = 1/(4*0.2) / 1/2 = 2/(4*0.2) = 1 / (2*0.2)
        //
        // HR of move to ((A:0.4,B:0.4,(C:0.1, D:0.1):0.3)
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1 * 1/2 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(choosing nodes to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/3 * 1/0.3
        //                  = 1/(6*0.3)
        // HR = 1/(6*0.3) / 1/2 = 2/(6*0.3) = 1 / (3*0.3)
        unsigned int balanced_count = 0;
        unsigned int root_poly_count = 0;
        unsigned int nsamples = 10000;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.4;
            std::shared_ptr<Node> root = std::make_shared<Node>("root", root_ht);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>("internal0", 0.2);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>("internal1", 0.1);
            std::shared_ptr<Node> leaf0 = std::make_shared<Node>("leaf0", 0.0);
            std::shared_ptr<Node> leaf1 = std::make_shared<Node>("leaf1", 0.0);
            std::shared_ptr<Node> leaf2 = std::make_shared<Node>("leaf2", 0.0);
            std::shared_ptr<Node> leaf3 = std::make_shared<Node>("leaf3", 0.0);

            internal0->add_child(leaf0);
            internal0->add_child(leaf1);
            internal1->add_child(leaf2);
            internal1->add_child(leaf3);
            root->add_child(internal0);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_number_of_node_heights() == 3);
            REQUIRE(tree.get_number_of_splittable_heights() == 0);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 2))).epsilon(1e-8));

            double ln_hastings = op.propose(rng,
                    &tree);
            if (tree.get_root_ptr()->get_number_of_children() == 3) {
                double exp_ln_hastings = std::log(1.0 / (3.0 * 0.3));
                REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
                REQUIRE(tree.get_number_of_node_heights() == 2);
                REQUIRE(tree.get_number_of_splittable_heights() == 1);
                REQUIRE(tree.get_height(0) == 0.1);
                REQUIRE(tree.get_height(1) == 0.4);
                ++root_poly_count;
            }
            else if (tree.get_root_ptr()->get_number_of_children() == 2) {
                // Pr(forward) = Pr(merge) * Pr(pick node) = 1 * 1/2 = 1/2
                // Pr(reverse) = Pr(split) * Pr(pick height) * Pr(parition nodes) * Pr(new height)
                //             = 1/2 * 1 * 1/2 * 1/d = 1/4d
                // HR = 1/4d / 1/2 = 2/4d = 1/2d
                double exp_ln_hastings = std::log(1.0 / (2.0 * 0.2));
                REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
                REQUIRE(tree.get_number_of_node_heights() == 2);
                REQUIRE(tree.get_number_of_splittable_heights() == 1);
                REQUIRE(tree.get_height(0) == 0.2);
                REQUIRE(tree.get_height(1) == 0.4);
                ++balanced_count;
            }
            else {
                REQUIRE(0 == 1);
            }
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / pow(root_ht, 1))).epsilon(1e-8));
        }
        REQUIRE(root_poly_count + balanced_count == nsamples);
        double eps = 0.01;
        REQUIRE(root_poly_count / (double)nsamples == Approx(0.5).epsilon(eps));
        REQUIRE(balanced_count / (double)nsamples == Approx(0.5).epsilon(eps));
    }
}



// Expectations for tree with 5 leaves:
// ----------------------------------------------------------------------------
// # of unlabeled                            # of trees (labelings)      
//  topologies
// ----------------------------------------------------------------------------
// (a,b,c,d,e)                                                     = 1
// (a,(b,c,d,e))           = 5 choose 4                            = 5
// (a,b,(c,d,e))           = 5 choose 3                            = 10 
// (a,b,c,(d,e))           = 5 choose 2                            = 10
// !((a,b,c),(d,e))        = 5 choose 3                            = 10
// (a,(b,(c,d,e)))         = (5 choose 3) * 2!                     = 20 
// !((a,b),(c,d),e)        = ((5 choose 2) * (3 choose 2)) / 2     = 15
// !(((a,b),(c,d)),e       = ((5 choose 2) * (3 choose 2)) / 2     = 15
// !(((a,b),c),(d,e))      = (5 choose 2) * (3 choose 2)           = 30
// (a,(b,(c,(d,e))))       = (5 choose 2) * 3!                     = 60
// (a,b,(c,(d,e)))         = (5 choose 2) * (3 choose 2)           = 30
// (a,(b,c,(d,e)))         = (5 choose 2) * (3 choose 2)           = 30
// ----------------------------------------------------------------------------
// TOTAL                                                           = 236
//
// 236 matches Felsenstein 1978, but we need to account for shared node
// heights. The topologies above prefixed with '!' are topologies that have
// potentially shared node heights. For each shared node configuration of these
// topologies, we have to add that many additional trees, which we do below.
//
// ----------------------------------------------------------------------------
// # of unlabeled                            # of trees (labelings)      
//  topologies
// ----------------------------------------------------------------------------
// ((a,b,c)*,(d,e)*)       = 5 choose 3                            = 10
// ((a,b)*,(c,d)*,e)       = ((5 choose 2) * (3 choose 2)) / 2     = 15
// (((a,b)*,(c,d)*),e      = ((5 choose 2) * (3 choose 2)) / 2     = 15
// (((a,b),c)*,(d,e)*)     = (5 choose 2) * (3 choose 2)           = 30
// (((a,b)*,c),(d,e)*)     = (5 choose 2) * (3 choose 2)           = 30
// ----------------------------------------------------------------------------
// GRAND TOTAL # OF TREE MODELS                                    = 336
//
// The asterisks in the topologies above indicated shared node heights.
TEST_CASE("Testing SplitLumpNodesRevJumpSampler with 5 leaves and fixed root",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing 5 leaves with fixed root") {
        RandomNumberGenerator rng = RandomNumberGenerator(44246543554);

        double root_ht = 0.5;
        std::shared_ptr<Node> root = std::make_shared<Node>(5, "root", root_ht);
        std::shared_ptr<Node> leaf0 = std::make_shared<Node>(0, "leaf0", 0.0);
        std::shared_ptr<Node> leaf1 = std::make_shared<Node>(1, "leaf1", 0.0);
        std::shared_ptr<Node> leaf2 = std::make_shared<Node>(2, "leaf2", 0.0);
        std::shared_ptr<Node> leaf3 = std::make_shared<Node>(3, "leaf3", 0.0);
        std::shared_ptr<Node> leaf4 = std::make_shared<Node>(4, "leaf4", 0.0);

        root->add_child(leaf0);
        root->add_child(leaf1);
        root->add_child(leaf2);
        root->add_child(leaf3);
        root->add_child(leaf4);

        BaseTree<Node> tree(root);

        tree.ignore_data();
        tree.fix_root_height();

        SplitLumpNodesRevJumpSampler<Node> op;

        // Initialize prior probs
        tree.compute_log_likelihood_and_prior(true);

        std::map< std::set< std::set<Split> >, unsigned int> split_counts;

        unsigned int count_nheights_1 = 0;
        unsigned int count_nheights_2 = 0;
        unsigned int count_nheights_3 = 0;
        unsigned int count_nheights_4 = 0;

        unsigned int niterations = 50000000;
        unsigned int sample_freq = 50;
        unsigned int nsamples = niterations / sample_freq;

        unsigned int sample_count = 0;
        unsigned int report_freq = 1000;
        for (unsigned int i = 0; i < niterations; ++i) {
            op.operate(rng, &tree, 1);
            if ((i + 1) % sample_freq == 0) {
                if (tree.get_number_of_node_heights() == 1) {
                    ++count_nheights_1;
                }
                else if (tree.get_number_of_node_heights() == 2) {
                    ++count_nheights_2;
                }
                else if (tree.get_number_of_node_heights() == 3) {
                    ++count_nheights_3;
                }
                else if (tree.get_number_of_node_heights() == 4) {
                    ++count_nheights_4;
                }
                std::set< std::set<Split> > splits = tree.get_splits(false);
                if (split_counts.count(splits) > 0) {
                    ++split_counts[splits];
                }
                else {
                    split_counts[splits] = 1;
                }
                ++sample_count;
                if (sample_count % report_freq == 0) {
                    std::cout << "Sampled " << sample_count << " of " << nsamples << "\n";
                }
            }
        }
        std::cout << op.header_string();
        std::cout << op.to_string();

        REQUIRE(op.get_number_of_attempts() == niterations);

        REQUIRE((count_nheights_1 + count_nheights_2 + count_nheights_3 + count_nheights_4) == nsamples);

        double freq_nheights_1 = count_nheights_1 / (double)nsamples;
        double freq_nheights_2 = count_nheights_2 / (double)nsamples;
        double freq_nheights_3 = count_nheights_3 / (double)nsamples;
        double freq_nheights_4 = count_nheights_4 / (double)nsamples;

        double exp_freq = 1.0/336.0;
        double exp_count = nsamples/336.0;
        std::map< std::set< std::set<Split> >, double> bad_splits;

        unsigned int total_trees_sampled = 0;
        std::map< std::set< std::set<Split> >, double> split_freqs;
        std::cout << "Total tree topologies sampled: " << split_counts.size() << "\n";
        for (auto s_c : split_counts) {
            total_trees_sampled += s_c.second;
            split_freqs[s_c.first] = s_c.second / (double)nsamples;
            std::cout << "Tree:\n";
            for (auto splitset : s_c.first) {
                unsigned int s_count = 0;
                for (auto split : splitset) {
                    if (s_count > 0) {
                        // Indent shared splits
                        std::cout << "  ";
                    }
                    std::cout << "  " << split.as_string() << "\n";
                    ++s_count;
                }
            }
            double prop_error = ((double)s_c.second - exp_count) / exp_count;
            std::cout << "  nsamples: " << s_c.second << "\n";
            std::cout << "  prop error: " << prop_error << "\n";
            if (fabs(prop_error) > 0.3) {
                bad_splits[s_c.first] = prop_error;
            }
        }

        std::cout << "BAD SPLITS\n";
        for (auto s_e : bad_splits) {
            std::cout << "\nTree:\n";
            for (auto splitset : s_e.first) {
                unsigned int s_count = 0;
                for (auto split : splitset) {
                    if (s_count > 0) {
                        // Indent shared splits
                        std::cout << "  ";
                    }
                    std::cout << "  " << split.as_string() << "\n";
                    ++s_count;
                }
            }
            std::cout << "  prop error: " << s_e.second << "\n";
        }

        REQUIRE(total_trees_sampled == nsamples);

        // We should sample every possible tree
        REQUIRE(split_counts.size() == 336);

        double eps = 0.001;

        for (auto s_f : split_freqs) {
            REQUIRE(s_f.second == Approx(exp_freq).epsilon(eps));
        }
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::merge with 3-2 shared tree with 5 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge 3-2 shared tree") {
        RandomNumberGenerator rng = RandomNumberGenerator(22);

        // MOVE FROM: ((A:0.1,B:0.1)*:0.1,(C:0.1,D:0.1,E:0.1)*:0.1)
        // MOVE TO:   (A:0.2,B:0.2,C:0.2,D:0.2,E:0.2)
        //
        // HR = pr(reverse move) / pr(forward move)
        // pr(forward move) = pr(choose to merge) * pr(choose height)
        //                  =  1/2 * 1 = 1/2
        // pr(reverse move) = pr(choose to split) * pr(choose height) * pr(partitioning branches to move down) * pr(new height)
        //                  = 1 * 1 * (1/(52-2)) * 1/0.2
        //                  = 1/(50*0.2)
        // HR = 1/(50*0.2) / 1/2 = 2/(50*0.2)
        
        std::string expected_tree_str = "(A:0.2,B:0.2,C:0.2,D:0.2,E:0.2)[&height_index=0,height=0.2];";
        BaseTree<Node> expected_tree(expected_tree_str);

        unsigned int nsamples = 20;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.2;
            std::shared_ptr<Node> root = std::make_shared<Node>(7, "root", root_ht);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>(6, "internal1", 0.1);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>(5, "internal0", 0.1);
            internal0->set_height_parameter(internal1->get_height_parameter());
            std::shared_ptr<Node> leaf_a = std::make_shared<Node>(0, "A", 0.0);
            std::shared_ptr<Node> leaf_b = std::make_shared<Node>(1, "B", 0.0);
            std::shared_ptr<Node> leaf_c = std::make_shared<Node>(2, "C", 0.0);
            std::shared_ptr<Node> leaf_d = std::make_shared<Node>(3, "D", 0.0);
            std::shared_ptr<Node> leaf_e = std::make_shared<Node>(4, "E", 0.0);

            internal0->add_child(leaf_a);
            internal0->add_child(leaf_b);
            internal1->add_child(leaf_c);
            internal1->add_child(leaf_d);
            internal1->add_child(leaf_e);
            root->add_child(internal0);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / 0.2)).epsilon(1e-8));

            double ln_hastings = op.propose_merge(rng,
                    &tree,
                    false);
            REQUIRE(tree.get_number_of_node_heights() == 1);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0)).epsilon(1e-8));

            double exp_ln_hastings = std::log(2.0 / (50.0 * 0.2));
            REQUIRE(ln_hastings == Approx(exp_ln_hastings).epsilon(1e-8));
            REQUIRE(tree.get_splits_by_height_index() == expected_tree.get_splits_by_height_index());
        }
    }
}

TEST_CASE("Testing SplitLumpNodesRevJumpSampler::split with 3-2 shared tree with 5 leaves",
        "[SplitLumpNodesRevJumpSampler]") {

    SECTION("Testing merge 3-2 shared tree") {
        RandomNumberGenerator rng = RandomNumberGenerator(765492347);

        // MOVE FROM: ((A:0.1,B:0.1)*:0.1,(C:0.1,D:0.1,E:0.1)*:0.1)
        // MOVE TO:   ((A:0.05,B:0.05):0.15,(C:0.1,D:0.1,E:0.1):0.1)
        //            OR
        //            ((A:0.1,B:0.1):0.1,(C:0.05,D:0.05,E:0.05):0.15)
        //            OR
        //            ((A:0.1,B:0.1)*:0.1,(C:0.1,(D:0.05,E:0.05):0.05)*:0.1)
        //            ((A:0.1,B:0.1)*:0.1,(D:0.1,(C:0.05,E:0.05):0.05)*:0.1)
        //            ((A:0.1,B:0.1)*:0.1,(E:0.1,(C:0.05,D:0.05):0.05)*:0.1)
        //            OR
        //            ((A:0.05,B:0.05)*:0.15,(C:0.1,(D:0.05,E:0.05)*:0.05):0.1)
        //            ((A:0.05,B:0.05)*:0.15,(D:0.1,(C:0.05,E:0.05)*:0.05):0.1)
        //            ((A:0.05,B:0.05)*:0.15,(E:0.1,(C:0.05,D:0.05)*:0.05):0.1)
        //
        // MOVE TO:   ((A:0.05,B:0.05):0.15,(C:0.1,D:0.1,E:0.1):0.1)
        // HR = pr(reverse move) / pr(forward move)
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1/2 * 1/2 = 1/4
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choose to move down) * pr(new height)
        //                  = 1/2 * 1 * 1/3 (can choose one node, the other, or both to move down) * 1/0.1
        //                  = 1/0.6
        // HR = 1/4 / 1/0.6 = 0.6/4 = 0.3/2 = 0.15
        //
        //
        // MOVE TO:   ((A:0.1,B:0.1):0.1,(C:0.05,D:0.05,E:0.05):0.15)
        //            OR
        //            ((A:0.1,B:0.1)*:0.1,(C:0.1,(D:0.05,E:0.05):0.05)*:0.1)
        //            ((A:0.1,B:0.1)*:0.1,(D:0.1,(C:0.05,E:0.05):0.05)*:0.1)
        //            ((A:0.1,B:0.1)*:0.1,(E:0.1,(C:0.05,D:0.05):0.05)*:0.1)
        // HR = pr(reverse move) / pr(forward move)
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1/2 * 1/2 = 1/4
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choose to move down) * pr(partition poly) * pr(new height)
        //                  = 1/2 * 1 * 1/3 * 1/4 * 1/0.1
        //                  = 1/(24*0.1) = 1/2.4
        // HR = 1/4 / 1/2.4 = 2.4/4 = 0.6
        //
        //
        // MOVE TO:   ((A:0.05,B:0.05)*:0.15,(C:0.1,(D:0.05,E:0.05)*:0.05):0.1)
        //            ((A:0.05,B:0.05)*:0.15,(D:0.1,(C:0.05,E:0.05)*:0.05):0.1)
        //            ((A:0.05,B:0.05)*:0.15,(E:0.1,(C:0.05,D:0.05)*:0.05):0.1)
        // HR = pr(reverse move) / pr(forward move)
        // pr(reverse move) = pr(choose to merge) * pr(choose height)
        //                  =  1/2 * 1/2 = 1/4
        // pr(forward move) = pr(choose to split) * pr(choose height) * pr(choose to move down) * pr(partition poly) * pr(new height)
        //                  = 1/2 * 1 * 1/3 * 1/3 * 1/0.1
        //                  = 1/(18*0.1) = 1/1.8
        // HR = 1/4 / 1/1.8 = 1.8/4 = 0.45
        
        BaseTree<Node> tree_cde("((A:0.05,B:0.05)[&height_index=0,height=0.05]:0.15,(C:0.1,D:0.1,E:0.1)[&height_index=1,height=0.1]:0.1)[&height_index=2,height=0.2];");
        BaseTree<Node> tree_ab("((A:0.1,B:0.1)[&height_index=1,height=0.1]:0.1,(C:0.05,D:0.05,E:0.05)[&height_index=0,height=0.05]:0.15)[&height_index=2,height=0.2];");

        BaseTree<Node> tree_abc("((A:0.1,B:0.1)[&height_index=1,height=0.1]:0.1,(C:0.1,(D:0.05,E:0.05)[&height_index=0,height=0.05]:0.05)[&height_index=1,height=0.01]:0.1)[&height_index=2,height=0.2];");
        BaseTree<Node> tree_abd("((A:0.1,B:0.1)[&height_index=1,height=0.1]:0.1,(D:0.1,(C:0.05,E:0.05)[&height_index=0,height=0.05]:0.05)[&height_index=1,height=0.01]:0.1)[&height_index=2,height=0.2];");
        BaseTree<Node> tree_abe("((A:0.1,B:0.1)[&height_index=1,height=0.1]:0.1,(E:0.1,(C:0.05,D:0.05)[&height_index=0,height=0.05]:0.05)[&height_index=1,height=0.01]:0.1)[&height_index=2,height=0.2];");

        BaseTree<Node> tree_c("((A:0.05,B:0.05)[&height_index=0,height=0.05]:0.15,(C:0.1,(D:0.05,E:0.05)[&height_index=0,height=0.05]:0.05)[&height_index=1,height=0.1]:0.1)[&height_index=2,height=0.2];");
        BaseTree<Node> tree_d("((A:0.05,B:0.05)[&height_index=0,height=0.05]:0.15,(D:0.1,(C:0.05,E:0.05)[&height_index=0,height=0.05]:0.05)[&height_index=1,height=0.1]:0.1)[&height_index=2,height=0.2];");
        BaseTree<Node> tree_e("((A:0.05,B:0.05)[&height_index=0,height=0.05]:0.15,(E:0.1,(C:0.05,D:0.05)[&height_index=0,height=0.05]:0.05)[&height_index=1,height=0.1]:0.1)[&height_index=2,height=0.2];");

        std::set<std::map< unsigned int, std::set<Split> > > choose_ab;
        choose_ab.insert(tree_cde.get_splits_by_height_index());
        double choose_ab_hr = std::log(0.15);

        std::set<std::map< unsigned int, std::set<Split> > > choose_cde;
        choose_cde.insert(tree_ab.get_splits_by_height_index());
        choose_cde.insert(tree_abc.get_splits_by_height_index());
        choose_cde.insert(tree_abd.get_splits_by_height_index());
        choose_cde.insert(tree_abe.get_splits_by_height_index());
        double choose_cde_hr = std::log(0.6);

        std::set<std::map< unsigned int, std::set<Split> > > choose_both;
        choose_both.insert(tree_c.get_splits_by_height_index());
        choose_both.insert(tree_d.get_splits_by_height_index());
        choose_both.insert(tree_e.get_splits_by_height_index());
        double choose_both_hr = std::log(0.45);

        std::map<std::map< unsigned int, std::set<Split> >, unsigned int> counts;
        for (auto splits : choose_ab) {
            counts[splits] = 0;
        }
        for (auto splits : choose_cde) {
            counts[splits] = 0;
        }
        for (auto splits : choose_both) {
            counts[splits] = 0;
        }

        std::map< unsigned int, std::set<Split> > splits;
        unsigned int nsamples = 50000;
        for (unsigned int i = 0; i < nsamples; ++i) {
            double root_ht = 0.2;
            std::shared_ptr<Node> root = std::make_shared<Node>(7, "root", root_ht);
            std::shared_ptr<Node> internal1 = std::make_shared<Node>(6, "internal1", 0.1);
            std::shared_ptr<Node> internal0 = std::make_shared<Node>(5, "internal0", 0.1);
            internal0->set_height_parameter(internal1->get_height_parameter());
            std::shared_ptr<Node> leaf_a = std::make_shared<Node>(0, "A", 0.0);
            std::shared_ptr<Node> leaf_b = std::make_shared<Node>(1, "B", 0.0);
            std::shared_ptr<Node> leaf_c = std::make_shared<Node>(2, "C", 0.0);
            std::shared_ptr<Node> leaf_d = std::make_shared<Node>(3, "D", 0.0);
            std::shared_ptr<Node> leaf_e = std::make_shared<Node>(4, "E", 0.0);

            internal0->add_child(leaf_a);
            internal0->add_child(leaf_b);
            internal1->add_child(leaf_c);
            internal1->add_child(leaf_d);
            internal1->add_child(leaf_e);
            root->add_child(internal0);
            root->add_child(internal1);

            BaseTree<Node> tree(root);

            tree.ignore_data();
            tree.fix_root_height();

            SplitLumpNodesRevJumpSampler<Node> op;

            // Initialize prior probs
            tree.compute_log_likelihood_and_prior(true);
            REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(1.0 / 0.2)).epsilon(1e-8));

            double ln_hastings = op.propose_split(rng,
                    &tree);
            REQUIRE(tree.get_number_of_node_heights() == 3);
            REQUIRE(tree.get_number_of_splittable_heights() == 1);
            tree.compute_log_likelihood_and_prior(true);

            splits = tree.get_splits_by_height_index();
            if (choose_ab.count(splits) > 0) {
                /* std::cout << "Proposed move AB\n"; */
                REQUIRE(ln_hastings == Approx(choose_ab_hr).epsilon(1e-8));
                REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                            (1.0 / 0.2) * (1.0 / 0.2)
                        )).epsilon(1e-8));
                /* std::cout << "Passed move AB\n"; */
            }
            else if (choose_cde.count(splits) > 0) {
                /* std::cout << "Proposed move CDE\n"; */
                REQUIRE(ln_hastings == Approx(choose_cde_hr).epsilon(1e-8));
                if (tree.get_mapped_nodes(1).size() > 1) {
                    REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                                (1.0 / 0.2) * (1.0 / 0.1)
                            )).epsilon(1e-8));
                }
                else {
                    REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                                (1.0 / 0.2) * (1.0 / 0.2)
                            )).epsilon(1e-8));
                }
                /* std::cout << "Passed move CDE\n"; */
            }
            else if (choose_both.count(splits) > 0) {
                /* std::cout << "Proposed move BOTH\n"; */
                REQUIRE(ln_hastings == Approx(choose_both_hr).epsilon(1e-8));
                REQUIRE(tree.get_log_prior_density_value() == Approx(std::log(
                            (1.0 / 0.2) * (1.0 / 0.1)
                        )).epsilon(1e-8));
                /* std::cout << "Passed move BOTH\n"; */
            }
            else {
                std::cout << "Unexpected tree\n";
                REQUIRE(0 == 1);
            }
            ++counts[splits];
        }

        for (auto splits_count : counts) {
            if (choose_ab.count(splits_count.first) > 0) {
                std::cout << "Expected freq: " << 1.0/3.0 << "\n";
                std::cout << "Sample freq: " << splits_count.second / (double)nsamples << "\n";
            }
            else if (choose_cde.count(splits_count.first) > 0) {
                std::cout << "Expected freq: " << 1.0/12.0 << "\n";
                std::cout << "Sample freq: " << splits_count.second / (double)nsamples << "\n";
            }
            else if (choose_both.count(splits_count.first) > 0) {
                std::cout << "Expected freq: " << 1.0/9.0 << "\n";
                std::cout << "Sample freq: " << splits_count.second / (double)nsamples << "\n";
            }
            else {
                std::cout << "Unexpected tree\n";
                REQUIRE(0 == 1);
            }
        }

        double eps = 0.005;
        for (auto splits_count : counts) {
            if (choose_ab.count(splits_count.first) > 0) {
                REQUIRE((splits_count.second / (double)nsamples) == Approx(1.0/3.0).epsilon(eps));
            }
            else if (choose_cde.count(splits_count.first) > 0) {
                REQUIRE((splits_count.second / (double)nsamples) == Approx(1.0/12.0).epsilon(eps));
            }
            else if (choose_both.count(splits_count.first) > 0) {
                REQUIRE((splits_count.second / (double)nsamples) == Approx(1.0/9.0).epsilon(eps));
            }
            else {
                std::cout << "Unexpected tree\n";
                REQUIRE(0 == 1);
            }
        }
    }
}
