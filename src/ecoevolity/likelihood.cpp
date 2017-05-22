/******************************************************************************
 * Copyright (C) 2015-2016 Jamie R. Oaks.
 *
 * This file is part of Ecoevolity.
 *
 * Ecoevolity is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Ecoevolity is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Ecoevolity.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "likelihood.hpp"

const static MatrixExponentiator matrix_exponentiator;

void compute_leaf_partials(
        PopulationNode& node,
        const unsigned int red_allele_count,
        const unsigned int allele_count,
        const bool markers_are_dominant
        ) {
    if (markers_are_dominant) {
        unsigned int n = allele_count;
        unsigned int n_reds = red_allele_count;
        const unsigned int a_count = allele_count * 2;
        if (red_allele_count > 0) {
            BiallelicPatternProbabilityMatrix m(a_count);
            double p_r_k_n = 1.0;
            for (unsigned int r = 1; r <= n_reds; ++r) {
                p_r_k_n = (p_r_k_n * 2.0 * (n - r + 1.0)) / ((2.0 * n) - r + 1.0);
            }
            for (unsigned int k = n_reds; k <= (2 * n_reds); ++k) {
                if (k > n_reds) {
                    p_r_k_n = (p_r_k_n * ((2.0 * n_reds) - k + 1) * k) /
                              (2.0 * ( k - n_reds) * ((2.0 * n) - k + 1.0));
                }
                m.set_pattern_probability(a_count, k, p_r_k_n);
            }
            node.copy_bottom_pattern_probs(m);
            return;
        }
        BiallelicPatternProbabilityMatrix m(a_count, n_reds);
        node.copy_bottom_pattern_probs(m);
        return;
    }
    BiallelicPatternProbabilityMatrix m(allele_count, red_allele_count);
    node.copy_bottom_pattern_probs(m);
    return;
}

void compute_top_of_branch_partials(
        PopulationNode& node,
        const double u,
        const double v,
        const double mutation_rate, 
        const double ploidy
        ) {
    if (node.get_allele_count() == 0) {
        node.copy_top_pattern_probs(node.get_bottom_pattern_probs());
        return;
    }

    double theta = 2 * ploidy * node.get_population_size() * mutation_rate;
    double length = node.get_length() * mutation_rate;
    BiallelicPatternProbabilityMatrix m = matrix_exponentiator.expQTtx(
            node.get_allele_count(),
            u,
            v,
            theta,
            length,
            node.get_bottom_pattern_probs());
    node.copy_top_pattern_probs(m);
}

void compute_internal_partials(
        PopulationNode& node) {
    if (node.get_number_of_children() == 1) {
        node.copy_bottom_pattern_probs(node.get_child(0)->get_top_pattern_probs());
        return;
    }
    if (node.get_child(0)->get_allele_count() == 0) {
        node.copy_bottom_pattern_probs(node.get_child(1)->get_top_pattern_probs());
        return;
    }
    if (node.get_child(1)->get_allele_count() == 0) {
        node.copy_bottom_pattern_probs(node.get_child(0)->get_top_pattern_probs());
        return;
    }
    unsigned int allele_count_child1 = node.get_child(0)->get_allele_count();
    unsigned int allele_count_child2 = node.get_child(1)->get_allele_count();

    std::vector<double> pattern_probs_child1 = node.get_child(0)->get_top_pattern_probs().get_pattern_prob_matrix();
    std::vector<double> pattern_probs_child2 = node.get_child(1)->get_top_pattern_probs().get_pattern_prob_matrix();

    for (unsigned int n = 1; n <= allele_count_child1; ++n) {
        double b_nr = 1.0;
        for (unsigned int r = 0; r <= n; ++r) {
            pattern_probs_child1.at(((n*(n+1))/2)-1+r) *= b_nr;
            b_nr *= ((double)n - r)/(r+1);
        }
    }

    for (unsigned int n = 1; n<= allele_count_child2; ++n) {
        double b_nr = 1.0;
        for (unsigned int r = 0; r <= n; ++r) {
            pattern_probs_child2.at(((n*(n+1))/2)-1+r) *= b_nr;
            b_nr *= ((double)n - r)/(r+1);
        }
    }

    unsigned int allele_count = allele_count_child1 + allele_count_child2;
    std::vector<double> pattern_probs; 
    pattern_probs.assign(
            ((((allele_count + 1) * (allele_count + 2))/2) - 1),
            0);
    for (unsigned int n1 = 1; n1 <= allele_count_child1; ++n1) {
        for (unsigned int r1 = 0; r1 <= n1; ++r1) {
            double f11 = pattern_probs_child1.at(n1*(n1+1)/2-1+r1);
            for (unsigned int n2 = 1; n2 <= allele_count_child2; ++n2) {
                for (unsigned int r2 = 0; r2 <= n2; ++r2) {
                    pattern_probs.at((n1+n2)*(n1+n2+1)/2-1+(r1+r2)) += f11 * pattern_probs_child2.at(n2*(n2+1)/2-1+r2);
                }
            }
        }
    }

    for (unsigned int n = 1; n <= allele_count; ++n) {
        double b_nr = 1.0;
        for (unsigned int r = 0; r <= n; ++r) {
            double f_nr = pattern_probs.at(n*(n+1)/2-1+r);
            f_nr /= b_nr;
            // TODO: better way to fix this?
            f_nr = std::max(f_nr, 0.0);
            pattern_probs.at(n*(n+1)/2-1+r) = f_nr;
            b_nr *= ((double)n - r)/(r+1);

        }
    }
    BiallelicPatternProbabilityMatrix m(allele_count, pattern_probs);
    node.copy_bottom_pattern_probs(m);
}

void compute_pattern_partials(
        PopulationNode& node,
        const std::vector<unsigned int>& red_allele_counts,
        const std::vector<unsigned int>& allele_counts,
        const double u,
        const double v,
        const double mutation_rate,
        const double ploidy,
        const bool markers_are_dominant
        ) {
    ECOEVOLITY_ASSERT(red_allele_counts.size() == allele_counts.size());
    if (node.is_leaf()) {
        compute_leaf_partials(
                node,
                red_allele_counts.at(node.get_population_index()),
                allele_counts.at(node.get_population_index()),
                markers_are_dominant);
    }
    else if (node.get_number_of_children() == 1) {
        compute_pattern_partials(*node.get_child(0),
                red_allele_counts,
                allele_counts,
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant);
        compute_top_of_branch_partials(*node.get_child(0), u, v, mutation_rate, ploidy);
        compute_internal_partials(node);
    }
    else if (node.get_number_of_children() == 2) {
        compute_pattern_partials(*node.get_child(0),
                red_allele_counts,
                allele_counts,
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant);
        compute_pattern_partials(*node.get_child(1),
                red_allele_counts,
                allele_counts,
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant);
        compute_top_of_branch_partials(*node.get_child(0), u, v, mutation_rate, ploidy);
        compute_top_of_branch_partials(*node.get_child(1), u, v, mutation_rate, ploidy);
        compute_internal_partials(node);
    }
    else {
        std::ostringstream message;
        message << "compute_pattern_probability(); "
                << "unexpected number of children: "
                << node.get_number_of_children();
        throw EcoevolityError(message.str());
    }
}

std::vector< std::vector<double> > compute_root_probabilities(
        const PopulationNode& root,
        const double u,
        const double v,
        const double mutation_rate,
        const double ploidy
        ) {
    unsigned int N = root.get_allele_count();
    std::vector< std::vector<double> > x (N + 1); 
    QMatrix q = QMatrix(
            N,
            u,
            v,
            2 * ploidy * root.get_population_size() * mutation_rate);
    std::vector<double> xcol = q.find_orthogonal_vector();

    // ECOEVOLITY_DEBUG(
    //     std::cerr << "xcol = [";
    //     for (unsigned int i = 0; i < xcol.size(); ++i) {
    //         std::cerr << xcol.at(i) << " ";
    //     }
    //     std::cerr << "]" << std::endl;
    // )

    unsigned int index = 1;
    for (unsigned int n = 1; n <= N; ++n) {
        x.at(n).resize(n + 1, 0.0);
        double row_sum = 0.0;
        for (unsigned int r = 0; r <= n; ++r) {
            double xcol_index = std::max(xcol.at(index), 0.0);
            row_sum += xcol_index;
            x.at(n).at(r) = xcol_index;
            index++;
        }
        for (unsigned int r = 0; r <= n; ++r) {
            x.at(n).at(r) = x.at(n).at(r) / row_sum;
        }
    }
    return x;
}

double compute_root_likelihood(
        const PopulationNode& root,
        const double u,
        const double v,
        const double mutation_rate,
        const double ploidy
        ) {
    unsigned int N = root.get_allele_count();
    std::vector< std::vector<double> > conditionals = compute_root_probabilities(root, u, v, mutation_rate, ploidy);

    // ECOEVOLITY_DEBUG(
    //     for (unsigned int n = 1; n <= N; ++n) {
    //         for (unsigned int r = 0; r <= n; ++r) {
    //             std::cerr << "root height: " << root.get_height() << std::endl;
    //             std::cerr << "conditional[" << n << ", " << r << "] = " << conditionals.at(n).at(r) << std::endl;
    //             std::cerr << "bottom_pattern_probs[" << n << ", " << r << "] = " << root.get_bottom_pattern_probability(n, r) << std::endl;
    //         }
    //     }
    // )

    double sum = 0.0;
    for (unsigned int n = 1; n <= N; ++n) {
        for (unsigned int r = 0; r <= n; ++r) {
            double term = conditionals.at(n).at(r) * root.get_bottom_pattern_probability(n, r);
            sum += term;
            if (sum < 0.0) {
                throw EcoevolityError("compute_root_likelihood(): Numerical error");
            }
        }
    }

    // ECOEVOLITY_DEBUG(
    //     std::cerr << "root likelihood: " << sum << std::endl;
    // )
    return sum;
}

double compute_pattern_likelihood(
        PopulationNode& root,
        const std::vector<unsigned int>& red_allele_counts,
        const std::vector<unsigned int>& allele_counts,
        const double u,
        const double v,
        const double mutation_rate,
        const double ploidy,
        const bool markers_are_dominant
        ) {
    compute_pattern_partials(root,
            red_allele_counts,
            allele_counts,
            u,
            v,
            mutation_rate,
            ploidy,
            markers_are_dominant);
    return compute_root_likelihood(root, u, v, mutation_rate, ploidy);
}

void compute_constant_pattern_likelihoods(
        PopulationNode& root,
        const std::vector<unsigned int>& max_allele_counts,
        const double u,
        const double v,
        const double mutation_rate,
        const double ploidy,
        const bool markers_are_dominant,
        const bool state_frequencies_are_constrained,
        double& all_red_pattern_likelihood,
        double& all_green_pattern_likelihood
        ) {
    std::vector<unsigned int> red_allele_counts (max_allele_counts.size(), 0); 
    all_green_pattern_likelihood = compute_pattern_likelihood(root,
            red_allele_counts,
            max_allele_counts,
            u,
            v,
            mutation_rate,
            ploidy,
            markers_are_dominant);
    all_red_pattern_likelihood = all_green_pattern_likelihood;
    if (! state_frequencies_are_constrained) {
        all_red_pattern_likelihood = compute_pattern_likelihood(root,
                max_allele_counts,
                max_allele_counts,
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant);
    }
}

double get_log_likelihood_for_pattern_range(
        PopulationNode& root,
        const std::vector< std::vector<unsigned int> >& red_allele_count_matrix,
        const std::vector< std::vector<unsigned int> >& allele_count_matrix,
        const std::vector<unsigned int>& pattern_weights,
        const unsigned int start_index,
        const unsigned int stop_index,
        const double u,
        const double v,
        const double mutation_rate,
        const double ploidy,
        const bool markers_are_dominant
        ) {
    ECOEVOLITY_ASSERT((red_allele_count_matrix.size() == allele_count_matrix.size()) &&
                      (red_allele_count_matrix.size() == pattern_weights.size()));
    double log_likelihood = 0.0;
    for (unsigned int pattern_idx = start_index;
            pattern_idx < stop_index;
            ++pattern_idx) {
        double pattern_likelihood = compute_pattern_likelihood(root,
                red_allele_count_matrix.at(pattern_idx),
                allele_count_matrix.at(pattern_idx),
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant);
        if (pattern_likelihood == 0.0) {
            return -std::numeric_limits<double>::infinity();
        }
        double weight = (double) pattern_weights.at(pattern_idx);
        log_likelihood += weight * std::log(pattern_likelihood);
    }
    return log_likelihood;
}

double get_log_likelihood(
        PopulationNode& root,
        const std::vector< std::vector<unsigned int> >& red_allele_count_matrix,
        const std::vector< std::vector<unsigned int> >& allele_count_matrix,
        const std::vector<unsigned int>& pattern_weights,
        const std::vector<unsigned int>& max_allele_counts,
        const double u,
        const double v,
        const double mutation_rate,
        const double ploidy,
        const bool markers_are_dominant,
        const bool state_frequencies_are_constrained,
        const bool constant_sites_removed,
        double& all_red_pattern_likelihood,
        double& all_green_pattern_likelihood,
        unsigned int nthreads
        ) {
    if (nthreads < 2) {
        if (constant_sites_removed) {
            compute_constant_pattern_likelihoods(
                    root,
                    max_allele_counts,
                    u,
                    v,
                    mutation_rate,
                    ploidy,
                    markers_are_dominant,
                    state_frequencies_are_constrained,
                    all_red_pattern_likelihood,
                    all_green_pattern_likelihood);
        }
        return get_log_likelihood_for_pattern_range(
                root,
                red_allele_count_matrix,
                allele_count_matrix,
                pattern_weights,
                0,
                pattern_weights.size(),
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant);
    }
    double log_likelihood = 0.0;
    const unsigned int npatterns = pattern_weights.size();
    if (npatterns < nthreads) {
        nthreads = npatterns;
    }
    const unsigned int batch_size = npatterns / nthreads;
    unsigned int start_idx = 0;
    std::vector< std::future<double> > threads(nthreads - 1);
    std::vector< std::shared_ptr<PopulationNode> > root_clones(nthreads - 1);

    // Launch nthreads - 1 threads
    for (unsigned int i = 0; i < (nthreads - 1); ++i) {
        root_clones.at(i) = root.get_clade_clone();
        threads.at(i) = std::async(
                std::launch::async,
                get_log_likelihood_for_pattern_range,
                std::ref(*root_clones.at(i)),
                std::cref(red_allele_count_matrix),
                std::cref(allele_count_matrix),
                std::cref(pattern_weights),
                start_idx,
                start_idx + batch_size,
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant
                );
        start_idx += batch_size;
    }

    // Use the main thread as the last thread
    if (constant_sites_removed) {
        compute_constant_pattern_likelihoods(
                root,
                max_allele_counts,
                u,
                v,
                mutation_rate,
                ploidy,
                markers_are_dominant,
                state_frequencies_are_constrained,
                all_red_pattern_likelihood,
                all_green_pattern_likelihood);
    }
    log_likelihood += get_log_likelihood_for_pattern_range(
            root,
            red_allele_count_matrix,
            allele_count_matrix,
            pattern_weights,
            start_idx,
            pattern_weights.size(),
            u,
            v,
            mutation_rate,
            ploidy,
            markers_are_dominant);

    // Join the launched threads
    for (auto &t : threads) {
        log_likelihood += t.get();
    }
    return log_likelihood;
}
