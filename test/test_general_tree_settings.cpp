#include "catch.hpp"
#include "ecoevolity/general_tree_settings.hpp"
#include "ecoevolity/settings_io.hpp"
#include "ecoevolity/parameter.hpp"
#include "ecoevolity/tree.hpp"


TEST_CASE("Testing basic settings", "[PopSizeSettings]") {
    SECTION("Testing basic settings") {
        std::stringstream ss;
        ss << "value: 1.0\n";
        ss << "estimate: true\n";
        ss << "prior:\n";
        ss << "    gamma_distribution:\n";
        ss << "        shape: 2.0\n";
        ss << "        mean: 0.1\n";

        YAML::Node n;
        n = YAML::Load(ss);
        PopSizeSettings settings(n);

        REQUIRE(settings.population_sizes_are_constrained() == true);
        REQUIRE(settings.get_value() == 1.0);
        REQUIRE(settings.is_fixed() == false);
        std::string s = settings.to_string();
        std::string e = (
                "equal_population_sizes: true\n"
                "value: 1\n"
                "estimate: true\n"
                "prior:\n"
                "    gamma_distribution:\n"
                "        shape: 2\n"
                "        scale: 0.05\n"
                );
        REQUIRE(s == e);

        RandomNumberGenerator rng = RandomNumberGenerator(123);

        PositiveRealParameter p = PositiveRealParameter(settings, rng);
        REQUIRE(p.get_value() == 1.0);
        REQUIRE(p.is_fixed() == false);
        REQUIRE(p.get_prior_string() == "gamma(shape = 2, scale = 0.05)");
    }
}

TEST_CASE("Testing explicit pop size constraint", "[PopSizeSettings]") {
    SECTION("Testing explicit constraint") {
        std::stringstream ss;
        ss << "value: 1.0\n";
        ss << "estimate: true\n";
        ss << "equal_population_sizes: true\n";
        ss << "prior:\n";
        ss << "    gamma_distribution:\n";
        ss << "        shape: 2.0\n";
        ss << "        mean: 0.1\n";

        YAML::Node n;
        n = YAML::Load(ss);
        PopSizeSettings settings(n);

        REQUIRE(settings.population_sizes_are_constrained() == true);
        REQUIRE(settings.get_value() == 1.0);
        REQUIRE(settings.is_fixed() == false);
        std::string s = settings.to_string();
        std::string e = (
                "equal_population_sizes: true\n"
                "value: 1\n"
                "estimate: true\n"
                "prior:\n"
                "    gamma_distribution:\n"
                "        shape: 2\n"
                "        scale: 0.05\n"
                );
        REQUIRE(s == e);

        RandomNumberGenerator rng = RandomNumberGenerator(123);

        PositiveRealParameter p = PositiveRealParameter(settings, rng);
        REQUIRE(p.get_value() == 1.0);
        REQUIRE(p.is_fixed() == false);
        REQUIRE(p.get_prior_string() == "gamma(shape = 2, scale = 0.05)");
    }
}

TEST_CASE("Testing unconstrained", "[PopSizeSettings]") {
    SECTION("Testing no constraint") {
        std::stringstream ss;
        ss << "value: 1.0\n";
        ss << "estimate: true\n";
        ss << "equal_population_sizes: false\n";
        ss << "prior:\n";
        ss << "    gamma_distribution:\n";
        ss << "        shape: 2.0\n";
        ss << "        mean: 0.1\n";

        YAML::Node n;
        n = YAML::Load(ss);
        PopSizeSettings settings(n);

        REQUIRE(settings.population_sizes_are_constrained() == false);
        REQUIRE(settings.get_value() == 1.0);
        REQUIRE(settings.is_fixed() == false);
        std::string s = settings.to_string();
        std::string e = (
                "equal_population_sizes: false\n"
                "value: 1\n"
                "estimate: true\n"
                "prior:\n"
                "    gamma_distribution:\n"
                "        shape: 2\n"
                "        scale: 0.05\n"
                );
        REQUIRE(s == e);

        RandomNumberGenerator rng = RandomNumberGenerator(123);

        PositiveRealParameter p = PositiveRealParameter(settings, rng);
        REQUIRE(p.get_value() == 1.0);
        REQUIRE(p.is_fixed() == false);
        REQUIRE(p.get_prior_string() == "gamma(shape = 2, scale = 0.05)");
    }
}

TEST_CASE("Testing PopulationTreeSettings with uniform_root_and_betas tree prior",
        "[PopulationTreeSettings]") {
    SECTION("Testing with uniform_root_and_betas tree prior") {
        std::string cfg_path = "data/dummy.yml";

        std::stringstream cfg_stream;
        cfg_stream << "---\n";
        cfg_stream << "tree_model:\n";
        cfg_stream << "    tree_space: \"generalized\"\n";
        cfg_stream << "    tree_prior:\n";
        cfg_stream << "        uniform_root_and_betas:\n";
        cfg_stream << "            parameters:\n";
        cfg_stream << "                root_height:\n";
        cfg_stream << "                    value: 0.3\n";
        cfg_stream << "                    estimate: true\n";
        cfg_stream << "                    prior:\n";
        cfg_stream << "                        gamma_distribution:\n";
        cfg_stream << "                            shape: 8.0\n";
        cfg_stream << "                            mean: 0.3\n";
        cfg_stream << "                alpha_of_node_height_beta_prior:\n";
        cfg_stream << "                    value: 1.0\n";
        cfg_stream << "                    estimate: true\n";
        cfg_stream << "                    prior:\n";
        cfg_stream << "                        gamma_distribution:\n";
        cfg_stream << "                            shape: 4.0\n";
        cfg_stream << "                            mean: 1.0\n";
        cfg_stream << "data:\n";
        cfg_stream << "    ploidy: 1\n";
        cfg_stream << "    constant_sites_removed: false\n";
        cfg_stream << "    yaml_allele_counts:\n";
        cfg_stream << "        path: \"diploid-dna-constant-missing.yml\"\n";

        std::cout << "Testing the following config:\n";
        std::cout << cfg_stream.str() << "\n";

        PopulationTreeSettings settings = PopulationTreeSettings(cfg_stream, cfg_path);

        REQUIRE(settings.tree_model_settings.tree_prior->get_type() ==
                EcoevolityOptions::TreePrior::uniform_root_and_betas);
        REQUIRE(settings.tree_model_settings.get_tree_space() ==
                EcoevolityOptions::TreeSpace::generalized);

        GeneralTreeOperatorSchedule<BasePopulationTree> op_schedule(
                settings.operator_settings, 3);

        write_settings(std::cout, settings, op_schedule);
    }
}


TEST_CASE("Testing bifurcating-comb conflict", "[TreeModelSettings]") {
    SECTION("Testing settings conflict") {
        std::stringstream ss;
        ss << "tree_space: bifurcating\n";
        ss << "starting_tree: comb\n";

        YAML::Node n;
        n = YAML::Load(ss);
        REQUIRE_THROWS_AS(TreeModelSettings(n, "dummy_path"),
                EcoevolityYamlConfigError &);
    }
    SECTION("Testing working example") {
        std::stringstream ss;
        ss << "tree_space: bifurcating\n";
        ss << "starting_tree: random\n";

        YAML::Node n;
        n = YAML::Load(ss);
        TreeModelSettings settings(n, "dummy_path");
        REQUIRE(settings.get_tree_space_string() == "bifurcating");
    }
}

TEST_CASE("Testing fixed-comb conflict", "[TreeModelSettings]") {
    SECTION("Testing settings conflict") {
        std::stringstream ss;
        ss << "tree_space: fixed\n";
        ss << "starting_tree: comb\n";

        YAML::Node n;
        n = YAML::Load(ss);
        REQUIRE_THROWS_AS(TreeModelSettings(n, "dummy_path"),
                EcoevolityYamlConfigError &);
    }
    SECTION("Testing working example") {
        std::stringstream ss;
        ss << "tree_space: fixed\n";
        ss << "starting_tree: \"[&R]((A:0.1,B:0.1):0.1,C:0.2):0.0\"\n";

        YAML::Node n;
        n = YAML::Load(ss);
        TreeModelSettings settings(n, "dummy_path");
        REQUIRE(settings.get_tree_space_string() == "fixed");
    }
}

TEST_CASE("Testing fixed-random conflict", "[TreeModelSettings]") {
    SECTION("Testing settings conflict") {
        std::stringstream ss;
        ss << "tree_space: fixed\n";
        ss << "starting_tree: random\n";

        YAML::Node n;
        n = YAML::Load(ss);
        REQUIRE_THROWS_AS(TreeModelSettings(n, "dummy_path"),
                EcoevolityYamlConfigError &);
    }
}

TEST_CASE("Testing conflict between random and fixed root height",
        "[TreeModelSettings]") {
    SECTION("Testing settings conflict") {
        std::stringstream ss;
        ss << "starting_tree: random\n";
        ss << "tree_prior:\n";
        ss << "    uniform_root_and_betas:\n";
        ss << "        parameters:\n";
        ss << "            root_height:\n";
        ss << "                estimate: false\n";

        YAML::Node n;
        n = YAML::Load(ss);
        REQUIRE_THROWS_AS(TreeModelSettings(n, "dummy_path"),
                EcoevolityYamlConfigError &);
    }
    SECTION("Testing working example") {
        std::stringstream ss;
        ss << "starting_tree: \"[&R]((A:0.1,B:0.1):0.1,C:0.2):0.0\"\n";
        ss << "tree_prior:\n";
        ss << "    uniform_root_and_betas:\n";
        ss << "        parameters:\n";
        ss << "            root_height:\n";
        ss << "                estimate: false\n";

        YAML::Node n;
        n = YAML::Load(ss);
        TreeModelSettings settings(n, "dummy_path");
        std::map<std::string, PositiveRealParameterSettings> parameters;
        parameters = settings.tree_prior->get_parameter_settings();
        PositiveRealParameterSettings root_ht = parameters.at("root_height");
        REQUIRE(root_ht.is_fixed());
        REQUIRE(std::isnan(root_ht.get_value()));
    }
}

TEST_CASE("Testing conflict between root height provided and in starting tree",
        "[TreeModelSettings]") {
    SECTION("Testing settings conflict") {
        std::stringstream ss;
        ss << "starting_tree: \"[&R]((A:0.1,B:0.1):0.1,C:0.2):0.0\"\n";
        ss << "tree_prior:\n";
        ss << "    uniform_root_and_betas:\n";
        ss << "        parameters:\n";
        ss << "            root_height:\n";
        ss << "                value: 0.3\n";

        YAML::Node n;
        n = YAML::Load(ss);
        REQUIRE_THROWS_AS(TreeModelSettings(n, "dummy_path"),
                EcoevolityYamlConfigError &);
    }
    SECTION("Testing working example") {
        std::stringstream ss;
        ss << "starting_tree: \"[&R]((A:0.1,B:0.1):0.1,C:0.2):0.0\"\n";
        ss << "tree_prior:\n";
        ss << "    uniform_root_and_betas:\n";
        ss << "        parameters:\n";
        ss << "            root_height:\n";
        ss << "                estimate: true\n";

        YAML::Node n;
        n = YAML::Load(ss);
        TreeModelSettings settings(n, "dummy_path");
        std::map<std::string, PositiveRealParameterSettings> parameters;
        parameters = settings.tree_prior->get_parameter_settings();
        PositiveRealParameterSettings root_ht = parameters.at("root_height");
        REQUIRE(! root_ht.is_fixed());
        REQUIRE(std::isnan(root_ht.get_value()));
    }
    SECTION("Testing alt working example") {
        std::stringstream ss;
        ss << "starting_tree: random\n";
        ss << "tree_prior:\n";
        ss << "    uniform_root_and_betas:\n";
        ss << "        parameters:\n";
        ss << "            root_height:\n";
        ss << "                value: 0.3\n";

        YAML::Node n;
        n = YAML::Load(ss);
        TreeModelSettings settings(n, "dummy_path");
        std::map<std::string, PositiveRealParameterSettings> parameters;
        parameters = settings.tree_prior->get_parameter_settings();
        PositiveRealParameterSettings root_ht = parameters.at("root_height");
        REQUIRE(root_ht.get_value() == 0.3);
    }
}
