#include "catch.hpp"
#include "ecoevolity/data.hpp"

/**
 *
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=5;
 *     Format datatype=standard symbols="012" gap=-;
 *     Matrix
 * pop1_a  00000
 * pop1_b  00202
 * pop1_c  01101
 * pop2_c  2-122
 * pop2_d  02202
 *     ;
 * End;
 */
TEST_CASE("Testing small, diploid, standard data set", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-data-ntax5-nchar5.nex") {
        std::string nex_path = "data/diploid-standard-data-ntax5-nchar5.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);
        REQUIRE(bd.get_path() == nex_path);

        std::vector<unsigned int> expected_wts = {2,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(4);
        expected_allele_counts[0] = {6, 4};
        expected_allele_counts[1] = {6, 2};
        expected_allele_counts[2] = {6, 4};
        expected_allele_counts[3] = {6, 4};

        std::vector< std::vector<unsigned int> > expected_red_counts(4);
        expected_red_counts[0] = {0, 2};
        expected_red_counts[1] = {1, 2};
        expected_red_counts[2] = {3, 3};
        expected_red_counts[3] = {3, 4};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
            for (unsigned int pop_idx = 0; pop_idx < bd.get_number_of_populations(); ++pop_idx) {
                REQUIRE(bd.get_allele_count(pattern_idx, pop_idx) ==
                        expected_allele_counts.at(pattern_idx).at(pop_idx));
                REQUIRE(bd.get_red_allele_count(pattern_idx, pop_idx) ==
                        expected_red_counts.at(pattern_idx).at(pop_idx));
            }
        }

        std::vector<unsigned int> expected_max_cts = {6,4};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);

        REQUIRE_THROWS_AS(bd.get_pattern_weight(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(4), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 c", "pop2 d"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        //fold patterns
        unsigned int number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);
        REQUIRE(bd.get_path() == nex_path);

        expected_red_counts[2] = {3, 1};
        expected_red_counts[3] = {3, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }
    }
}

/**
 *
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=5;
 *     Format datatype=standard symbols="012" gap=-;
 *     Matrix
 * pop1_a  00000
 * pop1_b  00202
 * pop1_c  01101
 * pop2_c  2-122
 * pop2_d  02202
 *     ;
 * End;
 */
TEST_CASE("Testing diploid standard data with 012 as dominant", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-data-ntax5-nchar5.nex as dominant") {
        std::string nex_path = "data/diploid-standard-data-ntax5-nchar5.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, true), EcoevolityInvalidCharacterError);
    }
}

/**
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=5;
 *     Format datatype=standard symbols="012" gap=-;
 *     Matrix
 * pop1_a  11100
 * pop1_b  00001
 * pop2_c  11101
 * pop2_d  10011
 * pop2_e  1-?01

 *     ;
 * End;
 */
TEST_CASE("Testing standard diploid with only 0/1 genotypes", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-only-01.nex") {
        std::string nex_path = "data/diploid-standard-only-01.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        std::vector<unsigned int> expected_wts = {2,2,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(3);
        expected_allele_counts[0] = {4, 6};
        expected_allele_counts[1] = {4, 4};
        expected_allele_counts[2] = {4, 6};

        std::vector< std::vector<unsigned int> > expected_red_counts(3);
        expected_red_counts[0] = {1, 3};
        expected_red_counts[1] = {1, 1};
        expected_red_counts[2] = {0, 1};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(3), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);
    }
}

TEST_CASE("Testing standard haploid with a 2 genotype", "[BiallelicData]") {

    SECTION("Testing data/haploid-standard-012.nex") {
        std::string nex_path = "data/haploid-standard-012.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, false), NxsException);
    }

    SECTION("Testing data/haploid-standard-012.nex as diploid") {
        std::string nex_path = "data/haploid-standard-012.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true), NxsException);
    }

    SECTION("Testing data/haploid-standard-012.nex as dominant") {
        std::string nex_path = "data/haploid-standard-012.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, false, true), EcoevolityBiallelicDataError);
    }

    SECTION("Testing data/haploid-standard-012.nex as diploid and dominant") {
        std::string nex_path = "data/haploid-standard-012.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, true), NxsException);
    }
}

/**
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=5;
 *     Format datatype=standard symbols="01" gap=-;
 *     Matrix
 * pop1_a  11100
 * pop1_b  00001
 * pop2_c  11101
 * pop2_d  10011
 * pop2_e  1-?00
 *     ;
 * End;
 */
TEST_CASE("Testing standard haploid", "[BiallelicData]") {

    SECTION("Testing data/haploid-standard.nex") {
        std::string nex_path = "data/haploid-standard.nex";
        BiallelicData bd(nex_path, '_', true, false);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,2,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(4);
        expected_allele_counts[0] = {2, 3};
        expected_allele_counts[1] = {2, 2};
        expected_allele_counts[2] = {2, 3};
        expected_allele_counts[3] = {2, 3};

        std::vector< std::vector<unsigned int> > expected_red_counts(4);
        expected_red_counts[0] = {1, 3};
        expected_red_counts[1] = {1, 1};
        expected_red_counts[2] = {0, 1};
        expected_red_counts[3] = {1, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(4), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        unsigned int number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_red_counts[0] = {1, 0};
        expected_red_counts[3] = {1, 1};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }
    }
}

/**
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=5;
 *     Format datatype=standard symbols="01" gap=-;
 *     Matrix
 * pop1_a  11100
 * pop1_b  00001
 * pop2_c  11101
 * pop2_d  10011
 * pop2_e  1-?00
 *     ;
 * End;
 */
TEST_CASE("Testing standard haploid dominant", "[BiallelicData]") {

    SECTION("Testing data/haploid-standard.nex as dominant") {
        std::string nex_path = "data/haploid-standard.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, false, true), EcoevolityBiallelicDataError);
    }

    SECTION("Testing data/haploid-standard.nex as diploid and dominant") {
        std::string nex_path = "data/haploid-standard.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, true), EcoevolityBiallelicDataError);
    }

    SECTION("Testing data/haploid-standard.nex as diploid") {
        std::string nex_path = "data/haploid-standard.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, false), EcoevolityBiallelicDataError);
    }
}

/**
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=5;
 *     Format datatype=standard symbols="012" gap=-;
 *     Matrix
 * pop1_a  2220000
 * pop1_b  0000200
 * pop2_c  2220202
 * pop2_d  2002200
 * pop2_e  2-?0220
 *     ;
 * End;
 */
TEST_CASE("Testing standard diploid dominant", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-dominant.nex as dominant") {
        std::string nex_path = "data/diploid-standard-dominant.nex";
        BiallelicData bd(nex_path, '_', true, true, true);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {2,2,3};

        std::vector< std::vector<unsigned int> > expected_allele_counts(3);
        expected_allele_counts[0] = {4, 6};
        expected_allele_counts[1] = {4, 4};
        expected_allele_counts[2] = {4, 6};

        std::vector< std::vector<unsigned int> > expected_red_counts(3);
        expected_red_counts[0] = {2, 6};
        expected_red_counts[1] = {2, 2};
        expected_red_counts[2] = {0, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(3), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        unsigned int number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_red_counts[0] = {2, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }
    }
}

/**
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=5;
 *     Format datatype=standard symbols="012" gap=-;
 *     Matrix
 * pop1_a  2220000
 * pop1_b  0000200
 * pop2_c  2220202
 * pop2_d  2002200
 * pop2_e  2-?0220
 *     ;
 * End;
 */
TEST_CASE("Testing standard diploid dominant as NOT dominant", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-dominant.nex as NOT dominant") {
        std::string nex_path = "data/diploid-standard-dominant.nex";
        BiallelicData bd(nex_path, '_', true, true, false);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {2,2,3};

        std::vector< std::vector<unsigned int> > expected_allele_counts(3);
        expected_allele_counts[0] = {4, 6};
        expected_allele_counts[1] = {4, 4};
        expected_allele_counts[2] = {4, 6};

        std::vector< std::vector<unsigned int> > expected_red_counts(3);
        expected_red_counts[0] = {2, 6};
        expected_red_counts[1] = {2, 2};
        expected_red_counts[2] = {0, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(3), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        unsigned int number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_red_counts[0] = {2, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }
    }
}

TEST_CASE("Testing for constant diploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-constant0.nex") {
        std::string nex_path = "data/diploid-standard-constant0.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 6);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(6);
        expected_allele_counts[0] = {6, 4};
        expected_allele_counts[1] = {6, 2};
        expected_allele_counts[2] = {6, 4};
        expected_allele_counts[3] = {6, 4};
        expected_allele_counts[4] = {6, 4};
        expected_allele_counts[5] = {4, 2};

        std::vector< std::vector<unsigned int> > expected_red_counts(6);
        expected_red_counts[0] = {0, 0};
        expected_red_counts[1] = {1, 2};
        expected_red_counts[2] = {3, 3};
        expected_red_counts[3] = {0, 2};
        expected_red_counts[4] = {3, 4};
        expected_red_counts[5] = {0, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(6), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_expected_wts = {1,1,1,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(4);
        rm_expected_allele_counts[0] = {6, 2};
        rm_expected_allele_counts[1] = {6, 4};
        rm_expected_allele_counts[2] = {6, 4};
        rm_expected_allele_counts[3] = {6, 4};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(4);
        rm_expected_red_counts[0] = {1, 2};
        rm_expected_red_counts[1] = {3, 3};
        rm_expected_red_counts[2] = {0, 2};
        rm_expected_red_counts[3] = {3, 4};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(4), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);
        
        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        rm_expected_red_counts[1] = {3, 1};
        rm_expected_red_counts[3] = {3, 0};
        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }
    }


    SECTION("Testing data/diploid-standard-constant2.nex") {
        std::string nex_path = "data/diploid-standard-constant2.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 6);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(6);
        expected_allele_counts[0] = {6, 4};
        expected_allele_counts[1] = {6, 2};
        expected_allele_counts[2] = {6, 4};
        expected_allele_counts[3] = {6, 4};
        expected_allele_counts[4] = {6, 4};
        expected_allele_counts[5] = {4, 2};

        std::vector< std::vector<unsigned int> > expected_red_counts(6);
        expected_red_counts[0] = {6, 4};
        expected_red_counts[1] = {1, 2};
        expected_red_counts[2] = {3, 3};
        expected_red_counts[3] = {0, 2};
        expected_red_counts[4] = {3, 4};
        expected_red_counts[5] = {4, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(6), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_expected_wts = {1,1,1,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(4);
        rm_expected_allele_counts[0] = {6, 2};
        rm_expected_allele_counts[1] = {6, 4};
        rm_expected_allele_counts[2] = {6, 4};
        rm_expected_allele_counts[3] = {6, 4};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(4);
        rm_expected_red_counts[0] = {1, 2};
        rm_expected_red_counts[1] = {3, 3};
        rm_expected_red_counts[2] = {0, 2};
        rm_expected_red_counts[3] = {3, 4};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(4), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        rm_expected_red_counts[1] = {3, 1};
        rm_expected_red_counts[3] = {3, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }
    }
}


TEST_CASE("Testing for constant haploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/haploid-standard-constant.nex") {
        std::string nex_path = "data/haploid-standard-constant.nex";
        BiallelicData bd(nex_path, '_', true, false);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,2,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {3, 2};
        expected_allele_counts[1] = {3, 1};
        expected_allele_counts[2] = {3, 2};
        expected_allele_counts[3] = {3, 2};
        expected_allele_counts[4] = {2, 1};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {0, 0};
        expected_red_counts[1] = {1, 1};
        expected_red_counts[2] = {2, 2};
        expected_red_counts[3] = {0, 1};
        expected_red_counts[4] = {2, 1};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_expected_wts = {1,2,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(3);
        rm_expected_allele_counts[0] = {3, 1};
        rm_expected_allele_counts[1] = {3, 2};
        rm_expected_allele_counts[2] = {3, 2};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(3);
        rm_expected_red_counts[0] = {1, 1};
        rm_expected_red_counts[1] = {2, 2};
        rm_expected_red_counts[2] = {0, 1};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(3), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        rm_expected_red_counts[1] = {1, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }
    }

    SECTION("Testing data/haploid-standard-constant.nex as dominant") {
        std::string nex_path = "data/haploid-standard-constant.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, false, true), EcoevolityBiallelicDataError);
    }

    SECTION("Testing data/haploid-standard-constant.nex as diploid") {
        std::string nex_path = "data/haploid-standard-constant.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, false), EcoevolityBiallelicDataError);
    }

    SECTION("Testing data/haploid-standard-constant.nex as diploid and dominant") {
        std::string nex_path = "data/haploid-standard-constant.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, true), EcoevolityBiallelicDataError);
    }
}



TEST_CASE("Testing for constant dominant diploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-constant-dominant.nex as dominant") {
        std::string nex_path = "data/diploid-standard-constant-dominant.nex";
        BiallelicData bd(nex_path, '_', true, true, true);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,2,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {6, 4};
        expected_allele_counts[1] = {6, 2};
        expected_allele_counts[2] = {6, 4};
        expected_allele_counts[3] = {6, 4};
        expected_allele_counts[4] = {4, 2};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {6, 4};
        expected_red_counts[1] = {2, 2};
        expected_red_counts[2] = {4, 4};
        expected_red_counts[3] = {0, 2};
        expected_red_counts[4] = {0, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_expected_wts = {1,2,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(3);
        rm_expected_allele_counts[0] = {6, 2};
        rm_expected_allele_counts[1] = {6, 4};
        rm_expected_allele_counts[2] = {6, 4};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(3);
        rm_expected_red_counts[0] = {2, 2};
        rm_expected_red_counts[1] = {4, 4};
        rm_expected_red_counts[2] = {0, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(3), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        rm_expected_red_counts[1] = {2, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }
    }
}

TEST_CASE("Testing for missing haploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/haploid-standard-missing.nex") {
        std::string nex_path = "data/haploid-standard-missing.nex";
        BiallelicData bd(nex_path, '_', true, false);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {2,2,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {1, 0};
        expected_allele_counts[1] = {2, 2};
        expected_allele_counts[2] = {2, 3};
        expected_allele_counts[3] = {0, 0};
        expected_allele_counts[4] = {0, 3};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {1, 0};
        expected_red_counts[1] = {1, 1};
        expected_red_counts[2] = {1, 2};
        expected_red_counts[3] = {0, 0};
        expected_red_counts[4] = {0, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 3);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 2);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_expected_wts = {2,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(2);
        rm_expected_allele_counts[0] = {2, 2};
        rm_expected_allele_counts[1] = {2, 3};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(2);
        rm_expected_red_counts[0] = {1, 1};
        rm_expected_red_counts[1] = {1, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(2), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(2), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(2), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 2);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        rm_expected_red_counts[1] = {1, 1};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }
    }
}

TEST_CASE("Testing for constant AND missing haploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/haploid-standard-missing.nex") {
        std::string nex_path = "data/haploid-standard-missing.nex";
        BiallelicData bd(nex_path, '_', true, false);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {2,2,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {1, 0};
        expected_allele_counts[1] = {2, 2};
        expected_allele_counts[2] = {2, 3};
        expected_allele_counts[3] = {0, 0};
        expected_allele_counts[4] = {0, 3};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {1, 0};
        expected_red_counts[1] = {1, 1};
        expected_red_counts[2] = {1, 2};
        expected_red_counts[3] = {0, 0};
        expected_red_counts[4] = {0, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_expected_wts = {2,1,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(3);
        rm_expected_allele_counts[0] = {2, 2};
        rm_expected_allele_counts[1] = {2, 3};
        rm_expected_allele_counts[2] = {0, 3};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(3);
        rm_expected_red_counts[0] = {1, 1};
        rm_expected_red_counts[1] = {1, 2};
        rm_expected_red_counts[2] = {0, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(3), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 2);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_rm_expected_wts = {2,1};

        std::vector< std::vector<unsigned int> > rm_rm_expected_allele_counts(2);
        rm_rm_expected_allele_counts[0] = {2, 2};
        rm_rm_expected_allele_counts[1] = {2, 3};

        std::vector< std::vector<unsigned int> > rm_rm_expected_red_counts(2);
        rm_rm_expected_red_counts[0] = {1, 1};
        rm_rm_expected_red_counts[1] = {1, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(2), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(2), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(2), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_rm_expected_labels = {"pop1 a", "pop1 b"};
        REQUIRE(bd.get_sequence_labels(0) == rm_rm_expected_labels);
        rm_rm_expected_labels.clear();
        rm_rm_expected_labels = {"pop2 c", "pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 2);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        rm_rm_expected_red_counts[1] = {1, 1};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_rm_expected_red_counts.at(pattern_idx));
        }
    }
}

TEST_CASE("Testing for constant AND missing dominant haploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/haploid-standard-missing.nex as dominant") {
        std::string nex_path = "data/haploid-standard-missing.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, false, true), EcoevolityBiallelicDataError);
    }

    SECTION("Testing data/haploid-standard-missing.nex as diploid") {
        std::string nex_path = "data/haploid-standard-missing.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, false), EcoevolityBiallelicDataError);
    }

    SECTION("Testing data/haploid-standard-missing.nex as diploid and dominant") {
        std::string nex_path = "data/haploid-standard-missing.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, true), EcoevolityBiallelicDataError);
    }
}

TEST_CASE("Testing for constant AND missing diploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-missing.nex") {
        std::string nex_path = "data/diploid-standard-missing.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {3,1,2,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {6, 0};
        expected_allele_counts[1] = {0, 4};
        expected_allele_counts[2] = {6, 4};
        expected_allele_counts[3] = {6, 2};
        expected_allele_counts[4] = {6, 2};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {3, 0};
        expected_red_counts[1] = {0, 3};
        expected_red_counts[2] = {3, 3};
        expected_red_counts[3] = {0, 0};
        expected_red_counts[4] = {6, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 3);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_expected_wts = {2,1,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(3);
        rm_expected_allele_counts[0] = {6, 4};
        rm_expected_allele_counts[1] = {6, 2};
        rm_expected_allele_counts[2] = {6, 2};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(3);
        rm_expected_red_counts[0] = {3, 3};
        rm_expected_red_counts[1] = {0, 0};
        rm_expected_red_counts[2] = {6, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(3), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(3), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 1);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> rm_rm_expected_wts = {2};

        std::vector< std::vector<unsigned int> > rm_rm_expected_allele_counts(1);
        rm_rm_expected_allele_counts[0] = {6, 4};

        std::vector< std::vector<unsigned int> > rm_rm_expected_red_counts(1);
        rm_rm_expected_red_counts[0] = {3, 3};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(1), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(1), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(1), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_rm_expected_labels);
        rm_rm_expected_labels.clear();
        rm_rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 0);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 1);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        rm_rm_expected_red_counts[0] = {3, 1};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_rm_expected_red_counts.at(pattern_idx));
        }
    }
}

TEST_CASE("Testing for removing all site patterns", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-all-remove.nex") {
        std::string nex_path = "data/diploid-standard-all-remove.nex";
        BiallelicData bd(nex_path);
        int number_removed = bd.remove_constant_patterns();
        REQUIRE_THROWS_AS(bd.remove_missing_population_patterns(), EcoevolityBiallelicDataError);
    }
}

TEST_CASE("Testing for mirrored diploid site patterns", "[BiallelicData]") {

    SECTION("Testing data/diploid-standard-missing.nex") {
        std::string nex_path = "data/diploid-standard-missing.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {3,1,2,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {6, 0};
        expected_allele_counts[1] = {0, 4};
        expected_allele_counts[2] = {6, 4};
        expected_allele_counts[3] = {6, 2};
        expected_allele_counts[4] = {6, 2};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {3, 0};
        expected_red_counts[1] = {0, 3};
        expected_red_counts[2] = {3, 3};
        expected_red_counts[3] = {0, 0};
        expected_red_counts[4] = {6, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Removing patterns
        unsigned int number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        std::vector<unsigned int> rm_expected_wts = {3,1,2,2};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(4);
        rm_expected_allele_counts[0] = {6, 0};
        rm_expected_allele_counts[1] = {0, 4};
        rm_expected_allele_counts[2] = {6, 4};
        rm_expected_allele_counts[3] = {6, 2};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(4);
        rm_expected_red_counts[0] = {3, 0};
        rm_expected_red_counts[1] = {0, 1};
        rm_expected_red_counts[2] = {3, 1};
        rm_expected_red_counts[3] = {0, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(4), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);
    }
}

/**
 *
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=5 nchar=6;
 *     Format datatype=standard symbols="012" gap=-;
 *     Matrix
 * pop1_a  ATC-CR
 * pop1_b  GCCCYA
 * pop1_c  ACTGTA
 * pop2_c  ATTGCA
 * pop2_d  GCC?YA
 *     ;
 * End;
 */
TEST_CASE("Testing small, diploid, dna data set", "[BiallelicData]") {

    SECTION("Testing data/diploid-dna.nex") {
        std::string nex_path = "data/diploid-dna.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {2,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {6, 4};
        expected_allele_counts[1] = {6, 4};
        expected_allele_counts[2] = {4, 2};
        expected_allele_counts[3] = {6, 4};
        expected_allele_counts[4] = {6, 4};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {2, 2};
        expected_red_counts[1] = {4, 2};
        expected_red_counts[2] = {2, 2};
        expected_red_counts[3] = {3, 1};
        expected_red_counts[4] = {1, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);
    }

    SECTION("Testing data/diploid-dna.nex as haploid") {
        std::string nex_path = "data/diploid-dna.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, false), EcoevolityInvalidCharacterError);
    }

    SECTION("Testing data/diploid-dna.nex as dominant") {
        std::string nex_path = "data/diploid-dna.nex";
        REQUIRE_THROWS_AS(BiallelicData bd(nex_path, '_', true, true, true), EcoevolityInvalidCharacterError);
    }

    SECTION("Testing folding of data/diploid-dna.nex") {
        std::string nex_path = "data/diploid-dna.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {2,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(5);
        expected_allele_counts[0] = {6, 4};
        expected_allele_counts[1] = {6, 4};
        expected_allele_counts[2] = {4, 2};
        expected_allele_counts[3] = {6, 4};
        expected_allele_counts[4] = {6, 4};

        std::vector< std::vector<unsigned int> > expected_red_counts(5);
        expected_red_counts[0] = {2, 2};
        expected_red_counts[1] = {4, 2};
        expected_red_counts[2] = {2, 2};
        expected_red_counts[3] = {3, 1};
        expected_red_counts[4] = {1, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);

        // Folding
        unsigned int number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 2);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        std::vector<unsigned int> rm_expected_wts = {3,1,1,1};

        std::vector< std::vector<unsigned int> > rm_expected_allele_counts(4);
        rm_expected_allele_counts[0] = {6, 4};
        rm_expected_allele_counts[1] = {4, 2};
        rm_expected_allele_counts[2] = {6, 4};
        rm_expected_allele_counts[3] = {6, 4};

        std::vector< std::vector<unsigned int> > rm_expected_red_counts(4);
        rm_expected_red_counts[0] = {2, 2};
        rm_expected_red_counts[1] = {2, 0};
        rm_expected_red_counts[2] = {3, 1};
        rm_expected_red_counts[3] = {1, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < rm_expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == rm_expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == rm_expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == rm_expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(4), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE_THROWS_AS(bd.get_population_label(2), std::out_of_range);

        std::vector<std::string> rm_expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == rm_expected_labels);
        rm_expected_labels.clear();
        rm_expected_labels = {"pop2 d", "pop2 e"};
        REQUIRE(bd.get_sequence_labels(1) == rm_expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(2), std::out_of_range);
    }
}

TEST_CASE("Testing diploid dna with missing, mirrored, and constant sites", "[BiallelicData]") {

    SECTION("Testing data/diploid-dna-constant-missing.nex") {
        std::string nex_path = "data/diploid-dna-constant-missing.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 8);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {2,1,1,1,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(8);
        expected_allele_counts[0] = {6, 6, 4};
        expected_allele_counts[1] = {6, 6, 4};
        expected_allele_counts[2] = {6, 6, 4};
        expected_allele_counts[3] = {4, 2, 2};
        expected_allele_counts[4] = {6, 6, 4};
        expected_allele_counts[5] = {6, 6, 0};
        expected_allele_counts[6] = {4, 2, 2};
        expected_allele_counts[7] = {6, 6, 4};

        std::vector< std::vector<unsigned int> > expected_red_counts(8);
        expected_red_counts[0] = {2, 4, 2};
        expected_red_counts[1] = {4, 4, 2};
        expected_red_counts[2] = {2, 2, 2};
        expected_red_counts[3] = {2, 2, 0};
        expected_red_counts[4] = {3, 2, 2};
        expected_red_counts[5] = {1, 0, 0};
        expected_red_counts[6] = {0, 0, 0};
        expected_red_counts[7] = {0, 0, 0};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(8), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(8), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(8), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);

        // Folding
        unsigned int number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 7);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_wts.clear();
        expected_wts = {2,2,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 0});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({3, 2, 2});
        expected_red_counts.push_back({1, 0, 0});
        expected_red_counts.push_back({0, 0, 0});
        expected_red_counts.push_back({0, 0, 0});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(7), std::out_of_range);
    
        // Remove missing
        number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 6);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_wts.clear();
        expected_wts = {2,2,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({3, 2, 2});
        expected_red_counts.push_back({0, 0, 0});
        expected_red_counts.push_back({0, 0, 0});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(6), std::out_of_range);

        // Remove missing
        number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 4);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_wts.clear();
        expected_wts = {2,2,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({3, 2, 2});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(4), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(4), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        expected_labels.clear();
        expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);

        std::vector<unsigned int> expected_max_cts = {6,6,4};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);
    }
}

TEST_CASE("Testing diploid dna with missing, mirrored, constant sites, and no hets", "[BiallelicData]") {

    SECTION("Testing data/diploid-dna-constant-missing-nohets.nex") {
        std::string nex_path = "data/diploid-dna-constant-missing-nohets.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 9);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,1,1,1,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(9);
        expected_allele_counts[0] = {6, 6, 4};
        expected_allele_counts[1] = {6, 6, 4};
        expected_allele_counts[2] = {6, 6, 4};
        expected_allele_counts[3] = {4, 2, 2};
        expected_allele_counts[4] = {6, 6, 4};
        expected_allele_counts[5] = {6, 6, 0};
        expected_allele_counts[6] = {4, 2, 2};
        expected_allele_counts[7] = {6, 6, 4};
        expected_allele_counts[8] = {6, 6, 4};

        std::vector< std::vector<unsigned int> > expected_red_counts(9);
        expected_red_counts[0] = {2, 4, 2};
        expected_red_counts[1] = {4, 4, 2};
        expected_red_counts[2] = {2, 2, 2};
        expected_red_counts[3] = {2, 2, 0};
        expected_red_counts[4] = {2, 0, 0};
        expected_red_counts[5] = {4, 6, 0};
        expected_red_counts[6] = {0, 0, 0};
        expected_red_counts[7] = {0, 0, 0};
        expected_red_counts[8] = {4, 2, 4};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        std::vector<unsigned int> expected_max_cts = {6,6,4};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);

        REQUIRE_THROWS_AS(bd.get_pattern_weight(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(9), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);

        // Remove constant
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 7);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 0});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({4, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({4, 6, 0});
        expected_red_counts.push_back({4, 2, 4});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(7), std::out_of_range);
    
        // Remove missing
        number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 6);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({4, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({4, 2, 4});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(6), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_wts.clear();
        expected_wts = {1,2,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({2, 4, 0});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        expected_labels.clear();
        expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);
    }

    SECTION("Testing data/diploid-dna-constant-missing-nohets.nex as dominant") {
        std::string nex_path = "data/diploid-dna-constant-missing-nohets.nex";
        BiallelicData bd(nex_path, '_', true, true, true);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 9);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,1,1,1,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(9);
        expected_allele_counts[0] = {6, 6, 4};
        expected_allele_counts[1] = {6, 6, 4};
        expected_allele_counts[2] = {6, 6, 4};
        expected_allele_counts[3] = {4, 2, 2};
        expected_allele_counts[4] = {6, 6, 4};
        expected_allele_counts[5] = {6, 6, 0};
        expected_allele_counts[6] = {4, 2, 2};
        expected_allele_counts[7] = {6, 6, 4};
        expected_allele_counts[8] = {6, 6, 4};

        std::vector< std::vector<unsigned int> > expected_red_counts(9);
        expected_red_counts[0] = {2, 4, 2};
        expected_red_counts[1] = {4, 4, 2};
        expected_red_counts[2] = {2, 2, 2};
        expected_red_counts[3] = {2, 2, 0};
        expected_red_counts[4] = {2, 0, 0};
        expected_red_counts[5] = {4, 6, 0};
        expected_red_counts[6] = {0, 0, 0};
        expected_red_counts[7] = {0, 0, 0};
        expected_red_counts[8] = {4, 2, 4};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(9), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);

        // Remove constant
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 7);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 0});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({4, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({4, 6, 0});
        expected_red_counts.push_back({4, 2, 4});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(7), std::out_of_range);
    
        // Remove missing
        number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 6);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({4, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({4, 2, 4});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(6), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_wts.clear();
        expected_wts = {1,2,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({6, 6, 4});
        expected_allele_counts.push_back({6, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({2, 4, 0});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        expected_labels.clear();
        expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);
    }

    SECTION("Testing data/diploid-dna-constant-missing-nohets.nex as haploid") {
        std::string nex_path = "data/diploid-dna-constant-missing-nohets.nex";
        BiallelicData bd(nex_path, '_', true, false);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 9);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,1,1,1,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(9);
        expected_allele_counts[0] = {3, 3, 2};
        expected_allele_counts[1] = {3, 3, 2};
        expected_allele_counts[2] = {3, 3, 2};
        expected_allele_counts[3] = {2, 1, 1};
        expected_allele_counts[4] = {3, 3, 2};
        expected_allele_counts[5] = {3, 3, 0};
        expected_allele_counts[6] = {2, 1, 1};
        expected_allele_counts[7] = {3, 3, 2};
        expected_allele_counts[8] = {3, 3, 2};

        std::vector< std::vector<unsigned int> > expected_red_counts(9);
        expected_red_counts[0] = {1, 2, 1};
        expected_red_counts[1] = {2, 2, 1};
        expected_red_counts[2] = {1, 1, 1};
        expected_red_counts[3] = {1, 1, 0};
        expected_red_counts[4] = {1, 0, 0};
        expected_red_counts[5] = {2, 3, 0};
        expected_red_counts[6] = {0, 0, 0};
        expected_red_counts[7] = {0, 0, 0};
        expected_red_counts[8] = {2, 1, 2};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(9), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);

        // Remove constant
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 7);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({2, 1, 1});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 0});
        expected_allele_counts.push_back({3, 3, 2});

        expected_red_counts.clear();
        expected_red_counts.push_back({1, 2, 1});
        expected_red_counts.push_back({2, 2, 1});
        expected_red_counts.push_back({1, 1, 1});
        expected_red_counts.push_back({1, 1, 0});
        expected_red_counts.push_back({1, 0, 0});
        expected_red_counts.push_back({2, 3, 0});
        expected_red_counts.push_back({2, 1, 2});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(7), std::out_of_range);
    
        // Remove missing
        number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 6);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({2, 1, 1});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 2});

        expected_red_counts.clear();
        expected_red_counts.push_back({1, 2, 1});
        expected_red_counts.push_back({2, 2, 1});
        expected_red_counts.push_back({1, 1, 1});
        expected_red_counts.push_back({1, 1, 0});
        expected_red_counts.push_back({1, 0, 0});
        expected_red_counts.push_back({2, 1, 2});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(6), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(! bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_wts.clear();
        expected_wts = {1,2,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({2, 1, 1});
        expected_allele_counts.push_back({3, 3, 2});
        expected_allele_counts.push_back({3, 3, 2});

        expected_red_counts.clear();
        expected_red_counts.push_back({1, 2, 1});
        expected_red_counts.push_back({1, 1, 1});
        expected_red_counts.push_back({1, 1, 0});
        expected_red_counts.push_back({1, 0, 0});
        expected_red_counts.push_back({1, 2, 0});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        expected_labels.clear();
        expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);

        std::vector<unsigned int> expected_max_cts = {3,3,2};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);
    }
}

/*
 * #NEXUS
 * Begin data;
 *     Dimensions ntax=8 nchar=9;
 *     Format datatype=dna gap=-;
 *     Matrix
 * pop1_a  ATC-CGGAT
 * pop1_b  ---C-AGA-
 * pop1_c  ACTGTA?AC
 * pop2_d  ATCGCAGAC
 * pop2_e  GCT?CA-AT
 * pop2_f  GCC?CA-AT
 * pop3_g  GCT?C?GAC
 * pop3_h  ATCCC??AC
 *     ;
 * End;
 */
TEST_CASE("Testing change in max sample size", "[BiallelicData]") {
    SECTION("Testing data/diploid-dna-sample-size-changes-nohets.nex") {
        std::string nex_path = "data/diploid-dna-sample-size-changes-nohets.nex";
        BiallelicData bd(nex_path);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 9);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == true);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        std::vector<unsigned int> expected_wts = {1,1,1,1,1,1,1,1,1};

        std::vector< std::vector<unsigned int> > expected_allele_counts(9);
        expected_allele_counts[0] = {4, 6, 4};
        expected_allele_counts[1] = {4, 6, 4};
        expected_allele_counts[2] = {4, 6, 4};
        expected_allele_counts[3] = {4, 2, 2};
        expected_allele_counts[4] = {4, 6, 4};
        expected_allele_counts[5] = {6, 6, 0};
        expected_allele_counts[6] = {4, 2, 2};
        expected_allele_counts[7] = {6, 6, 4};
        expected_allele_counts[8] = {4, 6, 4};

        std::vector< std::vector<unsigned int> > expected_red_counts(9);
        expected_red_counts[0] = {0, 4, 2};
        expected_red_counts[1] = {2, 4, 2};
        expected_red_counts[2] = {2, 2, 2};
        expected_red_counts[3] = {2, 2, 0};
        expected_red_counts[4] = {2, 0, 0};
        expected_red_counts[5] = {4, 6, 0};
        expected_red_counts[6] = {0, 0, 0};
        expected_red_counts[7] = {0, 0, 0};
        expected_red_counts[8] = {2, 2, 4};

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        std::vector<unsigned int> expected_max_cts = {6,6,4};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);

        REQUIRE_THROWS_AS(bd.get_pattern_weight(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(9), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(9), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        std::vector<std::string> expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);

        // Remove constant
        unsigned int number_removed = bd.remove_constant_patterns();
        REQUIRE(number_removed == 2);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 7);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == true);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({6, 6, 0});
        expected_allele_counts.push_back({4, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({0, 4, 2});
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({4, 6, 0});
        expected_red_counts.push_back({2, 2, 4});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        expected_max_cts = {6,6,4};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);

        REQUIRE_THROWS_AS(bd.get_pattern_weight(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(7), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(7), std::out_of_range);
    
        // Remove missing
        number_removed = bd.remove_missing_population_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 6);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == true);
        REQUIRE(bd.patterns_are_folded() == false);

        expected_wts.clear();
        expected_wts = {1,1,1,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({0, 4, 2});
        expected_red_counts.push_back({2, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({2, 2, 4});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
        }

        expected_max_cts = {4,6,4};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);

        REQUIRE_THROWS_AS(bd.get_pattern_weight(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(6), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(6), std::out_of_range);

        // Folding
        number_removed = bd.fold_patterns();
        REQUIRE(number_removed == 1);
        REQUIRE(bd.get_number_of_populations() == 3);
        REQUIRE(bd.get_number_of_patterns() == 5);
        REQUIRE(! bd.markers_are_dominant());
        REQUIRE(bd.genotypes_are_diploid());
        REQUIRE(bd.has_constant_patterns() == false);
        REQUIRE(bd.has_missing_population_patterns() == false);
        REQUIRE(bd.get_path() == nex_path);
        REQUIRE(bd.has_mirrored_patterns() == false);
        REQUIRE(bd.patterns_are_folded() == true);

        expected_wts.clear();
        expected_wts = {1,2,1,1,1};

        expected_allele_counts.clear();
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 2, 2});
        expected_allele_counts.push_back({4, 6, 4});
        expected_allele_counts.push_back({4, 6, 4});

        expected_red_counts.clear();
        expected_red_counts.push_back({0, 4, 2});
        expected_red_counts.push_back({2, 2, 2});
        expected_red_counts.push_back({2, 2, 0});
        expected_red_counts.push_back({2, 0, 0});
        expected_red_counts.push_back({2, 4, 0});

        for (unsigned int pattern_idx = 0; pattern_idx < expected_wts.size(); ++pattern_idx) {
            REQUIRE(bd.get_pattern_weight(pattern_idx) == expected_wts.at(pattern_idx));
            REQUIRE(bd.get_allele_counts(pattern_idx) == expected_allele_counts.at(pattern_idx));
            REQUIRE(bd.get_red_allele_counts(pattern_idx) == expected_red_counts.at(pattern_idx));
            for (unsigned int pop_idx = 0; pop_idx < bd.get_number_of_populations(); ++pop_idx) {
                REQUIRE(bd.get_allele_count(pattern_idx, pop_idx) ==
                        expected_allele_counts.at(pattern_idx).at(pop_idx));
                REQUIRE(bd.get_red_allele_count(pattern_idx, pop_idx) ==
                        expected_red_counts.at(pattern_idx).at(pop_idx));
            }
        }

        expected_max_cts = {4,6,4};
        REQUIRE(bd.get_max_allele_counts() == expected_max_cts);

        REQUIRE_THROWS_AS(bd.get_pattern_weight(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_allele_counts(5), std::out_of_range);
        REQUIRE_THROWS_AS(bd.get_red_allele_counts(5), std::out_of_range);

        REQUIRE(bd.get_population_index("pop1") == 0);
        REQUIRE(bd.get_population_index("pop2") == 1);
        REQUIRE(bd.get_population_index("pop3") == 2);
        REQUIRE_THROWS_AS(bd.get_population_index("bogus_label"), std::out_of_range);
        REQUIRE(bd.get_population_label(0) == "pop1");
        REQUIRE(bd.get_population_label(1) == "pop2");
        REQUIRE(bd.get_population_label(2) == "pop3");
        REQUIRE_THROWS_AS(bd.get_population_label(3), std::out_of_range);

        expected_labels.clear();
        expected_labels = {"pop1 a", "pop1 b", "pop1 c"};
        REQUIRE(bd.get_sequence_labels(0) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop2 d", "pop2 e", "pop2 f"};
        REQUIRE(bd.get_sequence_labels(1) == expected_labels);
        expected_labels.clear();
        expected_labels = {"pop3 g", "pop3 h"};
        REQUIRE(bd.get_sequence_labels(2) == expected_labels);
        REQUIRE_THROWS_AS(bd.get_sequence_labels(3), std::out_of_range);
    }
}
