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

#include "tree.hpp"

PopulationTree::PopulationTree(
        std::string path, 
        char population_name_delimiter,
        bool population_name_is_prefix,
        bool genotypes_are_diploid,
        bool markers_are_dominant,
        bool constant_sites_removed,
        bool validate,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        double ploidy) {
    this->init(path,
               population_name_delimiter,
               population_name_is_prefix,
               genotypes_are_diploid,
               markers_are_dominant,
               constant_sites_removed,
               validate,
               strict_on_constant_sites,
               strict_on_missing_sites,
               ploidy);
}

void PopulationTree::init(
        std::string path, 
        char population_name_delimiter,
        bool population_name_is_prefix,
        bool genotypes_are_diploid,
        bool markers_are_dominant,
        bool constant_sites_removed,
        bool validate,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        double ploidy) {
    if (genotypes_are_diploid && (ploidy != 2.0)) {
        throw EcoevolityBiallelicDataError(
                "Genotypes cannot be diploid if ploidy is not 2",
                path);
    }
    this->set_ploidy(ploidy);
    this->data_.init(
            path,
            population_name_delimiter,
            population_name_is_prefix,
            genotypes_are_diploid,
            markers_are_dominant,
            validate);
    if (this->data_.get_number_of_populations() < 1) {
        throw EcoevolityError("PopulationTree(); no populations were found");
    }
    unsigned int number_of_missing_patterns_removed = this->data_.remove_missing_population_patterns();
    if (number_of_missing_patterns_removed > 0) {
        if (strict_on_missing_sites) {
            std::ostringstream message;
            message << "\n#######################################################################\n"
                    <<   "###############################  ERROR  ###############################\n"
                    << this->data_.get_number_of_missing_sites_removed()
                    << " sites from the alignment in:\n    \'"
                    << path << "\'\n"
                    << "have no data for at least one population.\n"
                    << "#######################################################################\n";
            throw EcoevolityMissingDataError(message.str(), path);
        }
        else {
            std::ostringstream message;
            message << "\n#######################################################################\n"
                    <<   "##############################  WARNING  ##############################\n"
                    << this->data_.get_number_of_missing_sites_removed()
                    << " sites will be ignored from the alignment in:\n    \'"
                    << path << "\'\n"
                    << "due to at least one population with no data.\n"
                    << "#######################################################################\n";
            std::cerr << message.str() << std::endl;
        }
    }
    this->constant_sites_removed_ = constant_sites_removed;
    if (this->constant_sites_removed_) {
        // Have to make sure there are no missing sites
        unsigned int number_of_constant_patterns_removed = this->data_.remove_constant_patterns();
        if (number_of_constant_patterns_removed > 0) {
            if (strict_on_constant_sites) {
                std::ostringstream message;
                message << "\n#######################################################################\n"
                        <<   "###############################  ERROR  ###############################\n"
                        << this->data_.get_number_of_constant_sites_removed()
                        << " constant sites were found in the alignment in:\n"
                        << "    \'" << path << "\'\n"
                        << "but you indicated that such sites were already removed with option:\n"
                        << "    constant_sites_removed = true\n"
                        << "If you intended to remove them, please do so and re-run the analysis.\n"
                        << "If you intended for constant sites to be used in the likelihood\n"
                        << "calculations, you should set \'constant_sites_removed\' to false for\n"
                        << "this alignment and re-run the analysis.\n"
                        << "#######################################################################\n";
                throw EcoevolityConstantSitesError(message.str(), path);
            }
            else {
                std::ostringstream message;
                message << "\n#######################################################################\n"
                        <<   "##############################  WARNING  ##############################\n"
                        << this->data_.get_number_of_constant_sites_removed()
                        << " constant sites were found in the alignment in:\n"
                        << "    \'" << path << "\'\n"
                        << "but you indicated that such sites were already removed with option:\n"
                        << "    constant_sites_removed = true\n"
                        << "These sites have been removed, so if you intended to remove them, but\n"
                        << "missed them, all is well. However, if you intended for the constant\n"
                        << "sites to be used in the likelihood calculations, you should set\n"
                        << "\'constant_sites_removed\' to false for this alignment and re-run this\n"
                        << "analysis.\n"
                        << "#######################################################################\n";
                std::cerr << message.str() << std::endl;
            }
        }
    }

    this->init_tree();

    this->root_->resize_all();
}

void PopulationTree::init_tree() {
    if (this->data_.get_number_of_populations() < 3) {
        this->root_ = std::make_shared<PopulationNode>(0.0);
        for (unsigned int pop_idx = 0;
                pop_idx < this->data_.get_number_of_populations();
                ++pop_idx) {
            std::shared_ptr<PopulationNode> tip = std::make_shared<PopulationNode>(
                    pop_idx,
                    this->data_.get_population_label(pop_idx),
                    0.0,
                    this->data_.get_max_allele_count(pop_idx));
            tip->fix_node_height();
            this->root_->add_child(tip);
        }
        return;
    }
    std::shared_ptr<PopulationNode> ancestor = std::make_shared<PopulationNode>(0.0);
    ancestor->add_child(std::make_shared<PopulationNode>(this->data_.get_population_label(0)));
    ancestor->add_child(std::make_shared<PopulationNode>(this->data_.get_population_label(1)));
    for (unsigned int pop_idx = 2;
            pop_idx < this->data_.get_number_of_populations();
            ++pop_idx) {
        std::shared_ptr<PopulationNode> next_ancestor = std::make_shared<PopulationNode>(0.0);
        next_ancestor->add_child(ancestor);
        std::shared_ptr<PopulationNode> tip = std::make_shared<PopulationNode>(
                pop_idx,
                this->data_.get_population_label(pop_idx),
                0.0,
                this->data_.get_max_allele_count(pop_idx));
        tip->fix_node_height();
        next_ancestor->add_child(tip);
        ancestor = next_ancestor;
    }
    this->root_ = ancestor;
}

void PopulationTree::calculate_likelihood_correction() {
    double log_likelihood_correction = 0.0;
    for (unsigned int pattern_idx = 0;
            pattern_idx < this->data_.get_number_of_patterns();
            ++pattern_idx) {
        for (unsigned int pop_idx = 0;
                pop_idx < this->data_.get_number_of_populations();
                ++pop_idx) {
            log_likelihood_correction -= (
                    this->calculate_log_binomial(
                        this->data_.get_red_allele_count(pattern_idx, pop_idx),
                        this->data_.get_allele_count(pattern_idx, pop_idx)) *
                    this->data_.get_pattern_weight(pattern_idx)
                    );
        }
    }
    this->log_likelihood_correction_.set_value(log_likelihood_correction);
    this->likelihood_correction_was_calculated_ = true;
    // ECOEVOLITY_DEBUG(
    //     std::cerr << "Log likelihood correction: " << this->log_likelihood_correction_ << std::endl;
    // )
}

double PopulationTree::get_likelihood_correction(bool force) {
    if ((! this->likelihood_correction_was_calculated_) || (force)) {
        this->calculate_likelihood_correction();
    }
    return this->log_likelihood_correction_.get_value();
}

double PopulationTree::calculate_log_binomial(
        unsigned int red_allele_count,
        unsigned int allele_count) const {
    if ((red_allele_count == 0) || (red_allele_count == allele_count)) {
        return 0.0;
    }
    double f = 0.0;
    for (unsigned int i = red_allele_count + 1; i <= allele_count; ++i) {
        f += std::log(i) - std::log(allele_count - i + 1);
    }
    return f;
}

bool PopulationTree::constant_site_counts_were_provided() {
    if ((this->provided_number_of_constant_green_sites_ > -1) && (this->provided_number_of_constant_red_sites_ > -1)) {
        return true;
    }
    return false;
}

double PopulationTree::get_log_likelihood_value() const {
    return this->log_likelihood_.get_value();
}
double PopulationTree::get_stored_log_likelihood_value() const {
    return this->log_likelihood_.get_stored_value();
}

void PopulationTree::fold_patterns() {
    if (! this->state_frequencies_are_constrained()) {
        std::cerr << "WARNING: Site patterns are being folded when u/v rates are not constrained." << std::endl;
    }
    this->data_.fold_patterns();
    this->make_dirty();
}

void PopulationTree::set_root_height(double height) {
    this->root_->set_height(height);
}
void PopulationTree::update_root_height(double height) {
    this->root_->update_height(height);
}
double PopulationTree::get_root_height() const {
    return this->root_->get_height();
}
void PopulationTree::store_root_height() {
    this->root_->store_height();
}
void PopulationTree::restore_root_height() {
    this->root_->restore_height();
}
void PopulationTree::set_root_height_parameter(std::shared_ptr<PositiveRealParameter> h) {
    ECOEVOLITY_ASSERT(h->prior == this->node_height_prior_);
    this->root_->set_height_parameter(h);
}
std::shared_ptr<PositiveRealParameter> PopulationTree::get_root_height_parameter() const {
    return this->root_->get_height_parameter();
}

void PopulationTree::set_freq_1(double p) {
    if (this->state_frequencies_are_fixed()) {
        return;
    }
    this->freq_1_->set_value(p);
    this->make_dirty();
}
void PopulationTree::update_freq_1(double p) {
    if (this->state_frequencies_are_fixed()) {
        return;
    }
    this->freq_1_->update_value(p);
    this->make_dirty();
}
double PopulationTree::get_freq_1() const {
    return this->freq_1_->get_value();
}
double PopulationTree::get_freq_0() const {
    return 1.0 - this->get_freq_1();
}
double PopulationTree::get_u() const {
    ECOEVOLITY_ASSERT(this->get_freq_1() > 0.0);
    return 1.0 / (2.0 * this->get_freq_1());
}
double PopulationTree::get_v() const {
    // double u = this->get_u();
    // return u / ((2.0 * u) - 1.0);
    ECOEVOLITY_ASSERT(this->get_freq_0() > 0.0);
    return 1.0 / (2.0 * this->get_freq_0());
}
void PopulationTree::store_freq_1() {
    this->freq_1_->store();
}
void PopulationTree::restore_freq_1() {
    this->freq_1_->restore();
    this->make_dirty();
}

void PopulationTree::set_mutation_rate(double m) {
    if (this->mutation_rate_is_fixed()) {
        return;
    }
    this->mutation_rate_->set_value(m);
    this->make_dirty();
}
void PopulationTree::update_mutation_rate(double m) {
    if (this->mutation_rate_is_fixed()) {
        return;
    }
    this->mutation_rate_->update_value(m);
    this->make_dirty();
}
double PopulationTree::get_mutation_rate() const {
    return this->mutation_rate_->get_value();
}
void PopulationTree::store_mutation_rate() {
    this->mutation_rate_->store();
}
void PopulationTree::restore_mutation_rate() {
    this->mutation_rate_->restore();
    this->make_dirty();
}

std::shared_ptr<PositiveRealParameter> PopulationTree::get_freq_1_parameter() const {
    return this->freq_1_;
}

void PopulationTree::set_mutation_rate_parameter(std::shared_ptr<PositiveRealParameter> h) {
    this->mutation_rate_ = h;
    this->make_dirty();
}
std::shared_ptr<PositiveRealParameter> PopulationTree::get_mutation_rate_parameter() const {
    return this->mutation_rate_;
}

void PopulationTree::set_root_population_size(double size) {
    if (this->population_sizes_are_fixed()) {
        return;
    }
    this->root_->set_population_size(size);
}
void PopulationTree::set_population_size(double size) {
    if (this->population_sizes_are_fixed()) {
        return;
    }
    this->root_->set_all_population_sizes(size);
}

double PopulationTree::get_root_population_size() const {
    return this->root_->get_population_size();
}
std::shared_ptr<PositiveRealParameter> PopulationTree::get_root_population_size_parameter() const {
    return this->root_->get_population_size_parameter();
}

void PopulationTree::store_state() {
    this->store_likelihood();
    this->store_prior_density();
    this->store_parameters();
}
void PopulationTree::store_likelihood() {
    this->log_likelihood_.store();
    this->all_green_pattern_likelihood_.store();
    this->all_red_pattern_likelihood_.store();
}
void PopulationTree::store_prior_density() {
    this->log_prior_density_.store();
}
void PopulationTree::store_parameters() {
    this->store_freq_1();
    this->store_mutation_rate();
    this->store_all_population_sizes();
    this->store_all_heights();
}
void PopulationTree::store_all_population_sizes() {
    this->root_->store_all_population_sizes();
}
void PopulationTree::store_all_heights() {
    this->root_->store_all_heights();
}

void PopulationTree::restore_state() {
    this->restore_likelihood();
    this->restore_prior_density();
    this->restore_parameters();
}
void PopulationTree::restore_likelihood() {
    this->log_likelihood_.restore();
    this->all_green_pattern_likelihood_.restore();
    this->all_red_pattern_likelihood_.restore();
}
void PopulationTree::restore_prior_density() {
    this->log_prior_density_.restore();
}
void PopulationTree::restore_parameters() {
    this->restore_freq_1();
    this->restore_mutation_rate();
    this->restore_all_population_sizes();
    this->restore_all_heights();
}
void PopulationTree::restore_all_population_sizes() {
    this->root_->restore_all_population_sizes();
}
void PopulationTree::restore_all_heights() {
    this->root_->restore_all_heights();
}

void PopulationTree::set_node_height_prior(std::shared_ptr<ContinuousProbabilityDistribution> prior) {
    this->node_height_prior_ = prior;
    this->root_->set_all_node_height_priors(prior);
}

void PopulationTree::set_population_size_prior(std::shared_ptr<ContinuousProbabilityDistribution> prior) {
    this->population_size_prior_ = prior;
    this->root_->set_all_population_size_priors(prior);
}

void PopulationTree::set_freq_1_prior(std::shared_ptr<ContinuousProbabilityDistribution> prior) {
    this->freq_1_->set_prior(prior);
    this->make_dirty();
}
void PopulationTree::set_mutation_rate_prior(std::shared_ptr<ContinuousProbabilityDistribution> prior) {
    this->mutation_rate_->set_prior(prior);
    this->make_dirty();
}

double PopulationTree::compute_log_prior_density() {
    double d = 0.0;
    d += this->compute_log_prior_density_of_state_frequencies();
    d += this->compute_log_prior_density_of_mutation_rate();
    d += this->compute_log_prior_density_of_node_heights();
    d += this->compute_log_prior_density_of_population_sizes();
    this->log_prior_density_.set_value(d);
    return d;
}
double PopulationTree::compute_log_prior_density_of_state_frequencies() const {
    return this->freq_1_->relative_prior_ln_pdf();
}
double PopulationTree::compute_log_prior_density_of_mutation_rate() const {
    return this->mutation_rate_->relative_prior_ln_pdf();
}
double PopulationTree::compute_log_prior_density_of_node_heights() const {
    return this->root_->calculate_ln_relative_node_height_prior_density();
}
double PopulationTree::compute_log_prior_density_of_population_sizes() const {
    return this->root_->calculate_ln_relative_population_size_prior_density();
}

double PopulationTree::get_log_prior_density_value() const {
    return this->log_prior_density_.get_value();
}
double PopulationTree::get_stored_log_prior_density_value() const {
    return this->log_prior_density_.get_stored_value();
}

bool PopulationTree::is_dirty() const {
    if (this->is_dirty_) {
        return true;
    }
    return this->root_->clade_has_dirt();
}

void PopulationTree::make_dirty() {
    this->is_dirty_ = true;
}
void PopulationTree::make_clean() {
    this->is_dirty_ = false;
    this->root_->make_all_clean();
}

void PopulationTree::provide_number_of_constant_sites(
                unsigned int number_all_red,
                unsigned int number_all_green) {
    if (! this->constant_sites_removed_) {
        throw EcoevolityError(
                "Trying to provide number of constant sites, but they haven't been removed");
    }
    this->provided_number_of_constant_red_sites_ = number_all_red;
    this->provided_number_of_constant_green_sites_ = number_all_green;
}


ComparisonPopulationTree::ComparisonPopulationTree(
        std::string path, 
        char population_name_delimiter,
        bool population_name_is_prefix,
        bool genotypes_are_diploid,
        bool markers_are_dominant,
        bool constant_sites_removed,
        bool validate,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        double ploidy) {
    this->comparison_init(path,
               population_name_delimiter,
               population_name_is_prefix,
               genotypes_are_diploid,
               markers_are_dominant,
               constant_sites_removed,
               validate,
               strict_on_constant_sites,
               strict_on_missing_sites,
               ploidy);
}
void ComparisonPopulationTree::comparison_init(
        std::string path, 
        char population_name_delimiter,
        bool population_name_is_prefix,
        bool genotypes_are_diploid,
        bool markers_are_dominant,
        bool constant_sites_removed,
        bool validate,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        double ploidy) {
    this->init(path,
               population_name_delimiter,
               population_name_is_prefix,
               genotypes_are_diploid,
               markers_are_dominant,
               constant_sites_removed,
               validate,
               strict_on_constant_sites,
               strict_on_missing_sites,
               ploidy);
    if (this->data_.get_number_of_populations() > 2) {
        throw EcoevolityComparisonSettingError(
                "ComparisonPopulationTree() does not support more than 2 populations",
                path);
    }
    this->root_->set_label("root-" + this->root_->get_child(0)->get_label());
}
ComparisonPopulationTree::ComparisonPopulationTree(
        const ComparisonSettings& settings,
        RandomNumberGenerator& rng,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites) {
    this->comparison_init(settings.get_path(),
               settings.get_population_name_delimiter(),
               settings.population_name_is_prefix(),
               settings.genotypes_are_diploid(),
               settings.markers_are_dominant(),
               settings.constant_sites_removed(),
               true, // validate
               strict_on_constant_sites,
               strict_on_missing_sites,
               settings.get_ploidy());
    if (settings.constrain_state_frequencies()) {
        this->constrain_state_frequencies();
        this->fold_patterns();
    }
    this->set_population_size_prior(
            settings.get_population_size_settings().get_prior_settings().get_instance());
    if (settings.constrain_population_sizes()) {
        this->constrain_population_sizes();
    }
    PositiveRealParameter p = PositiveRealParameter(
            settings.get_population_size_settings(),
            rng);
    this->set_population_size(p.get_value());
    if (settings.get_population_size_settings().is_fixed()) {
        this->fix_population_sizes();
    }
    
    this->set_freq_1_prior(settings.get_freq_1_settings().get_prior_settings().get_instance());
    if (settings.constrain_state_frequencies()) {
        this->constrain_state_frequencies();
    }
    else {
        PositiveRealParameter freq_1 = PositiveRealParameter(
                settings.get_freq_1_settings(),
                rng);
        this->set_freq_1(freq_1.get_value());
        if (freq_1.is_fixed()) {
            this->fix_state_frequencies();
        }
    }
    this->set_mutation_rate_parameter(
            std::make_shared<PositiveRealParameter>(
                    settings.get_mutation_rate_settings(),
                    rng));
    if (
        (this->data_.get_number_of_populations() == 1) &&
        (
            (this->population_sizes_are_fixed()) ||
            (this->population_sizes_are_constrained())
        )
    ) {
        std::ostringstream message;
        message << "\n#######################################################################\n"
                <<   "###############################  ERROR  ###############################\n"
                << "The alignment in:\n    \'"
                << this->data_.get_path() << "\'\n"
                << "contains only a single population, but you have fixed and/or "
                << "constrained the population sizes for this comparison. The timing of "
                << "population expansion/contraction cannot be estimated if the ancestral "
                << "and descendant population sizes for this comparison are either "
                << "fixed or constrained to be equal. Please update your configuration "
                << "file to estimate the unconstrained population sizes for this "
                << "comparison and re-run the analysis.\n"
                << "#######################################################################\n";
        throw EcoevolityComparisonSettingError(message.str(), this->data_.get_path());
    }
}

void ComparisonPopulationTree::set_child_population_size(
        unsigned int child_index,
        double size) {
    if (this->population_sizes_are_fixed()) {
        return;
    }
    this->root_->get_child(child_index)->set_population_size(size);
}
void ComparisonPopulationTree::update_child_population_size(
        unsigned int child_index,
        double size) {
    if (this->population_sizes_are_fixed()) {
        return;
    }
    this->root_->get_child(child_index)->update_population_size(size);
}
double ComparisonPopulationTree::get_child_population_size(
        unsigned int child_index) const {
    return this->root_->get_child(child_index)->get_population_size();
}
void ComparisonPopulationTree::store_child_population_size(
        unsigned int child_index) {
    this->root_->get_child(child_index)->store_population_size();
}
void ComparisonPopulationTree::restore_child_population_size(
        unsigned int child_index) {
    this->root_->get_child(child_index)->restore_population_size();
}
std::shared_ptr<PositiveRealParameter> ComparisonPopulationTree::get_child_population_size_parameter(
        unsigned int child_index) const {
    return this->root_->get_child(child_index)->get_population_size_parameter();
}

// Node height sharing needs to be dealt with in next level up in
// class hierarchy (ComparisonPopulationTreeCollection)
double ComparisonPopulationTree::compute_log_prior_density() {
    double d = 0.0;
    d += this->compute_log_prior_density_of_state_frequencies();
    d += this->compute_log_prior_density_of_mutation_rate();
    // d += this->compute_log_prior_density_of_node_heights();
    d += this->compute_log_prior_density_of_population_sizes();
    this->log_prior_density_.set_value(d);
    return d;
}

// Node height (re)storing is managed by ComparisonPopulationTree.
void ComparisonPopulationTree::store_parameters() {
    this->store_freq_1();
    this->store_mutation_rate();
    this->store_all_population_sizes();
    // this->store_all_heights();
}
void ComparisonPopulationTree::restore_parameters() {
    this->restore_freq_1();
    this->restore_mutation_rate();
    this->restore_all_population_sizes();
    // this->restore_all_heights();
}

void ComparisonPopulationTree::write_state_log_header(
        std::ostream& out,
        bool include_event_index,
        const std::string& delimiter) const {
    std::string suffix = "_" + this->root_->get_child(0)->get_label();
    if (include_event_index) {
        out << "root_height_index" << suffix << delimiter;
    }
    out << "ln_likelihood" << suffix << delimiter
        << "ln_prior" << suffix << delimiter
        << "root_height" << suffix << delimiter
        << "mutation_rate" << suffix << delimiter
        << "freq_1" << suffix << delimiter
        << "pop_size" << suffix << delimiter;
    if (this->root_->get_number_of_children() > 1) {
        out << "pop_size" << "_" << this->root_->get_child(1)->get_label() << delimiter;
    }
    out << "pop_size_root" << suffix;
}

void ComparisonPopulationTree::log_state(
        std::ostream& out,
        const std::string& delimiter) const {
    out << this->log_likelihood_.get_value() << delimiter
        << this->log_prior_density_.get_value() << delimiter
        << this->get_height() << delimiter
        << this->get_mutation_rate() << delimiter
        << this->get_freq_1() << delimiter
        << this->get_child_population_size(0) << delimiter;
    if (this->root_->get_number_of_children() > 1) {
        out << this->get_child_population_size(1) << delimiter;
    }
    out << this->get_root_population_size();
}
void ComparisonPopulationTree::log_state(
        std::ostream& out,
        unsigned int event_index,
        const std::string& delimiter) const {
    out << event_index << delimiter;
    this->log_state(out, delimiter);
}

std::string ComparisonPopulationTree::get_state_header_string(
        const std::string& delimiter) const {
    std::ostringstream ss;
    this->write_state_log_header(ss, false, delimiter);
    return ss.str();
}

std::string ComparisonPopulationTree::get_state_string(
        const std::string& delimiter,
        unsigned int precision) const {
    std::ostringstream ss;
    ss.precision(precision);
    this->log_state(ss, delimiter);
    return ss.str();
}

// TODO: This is a hack. The more general solution would be a recursive method
// of PopulationTree
std::shared_ptr<GeneTreeSimNode> ComparisonPopulationTree::simulate_gene_tree(
        const unsigned int pattern_index,
        RandomNumberGenerator& rng) const {

    std::vector< std::shared_ptr<GeneTreeSimNode> > left_lineages;
    std::vector< std::shared_ptr<GeneTreeSimNode> > right_lineages;
    std::vector< std::shared_ptr<GeneTreeSimNode> > root_lineages;
    unsigned int allele_count;
    allele_count = this->data_.get_allele_count(
            pattern_index,
            this->data_.get_population_index(
                    this->root_->get_child(0)->get_label()));
    if (this->data_.markers_are_dominant()) {
        allele_count *= 2;
    }
    left_lineages.reserve(allele_count);
    for (unsigned int tip_idx = 0; tip_idx < allele_count; ++tip_idx) {
        std::shared_ptr<GeneTreeSimNode> tip = std::make_shared<GeneTreeSimNode>(
                    0,
                    0.0);
            tip->fix_node_height();
            left_lineages.push_back(tip);
    }

    double top_of_branch_height = this->get_node_length_in_subs_per_site(
            *this->root_->get_child(0));
    double current_height = 0.0;
    double last_left_coal_height = this->coalesce_in_branch(
            left_lineages,
            this->get_node_theta(*this->root_->get_child(0)),
            rng,
            current_height,
            top_of_branch_height
            );

    if (this->root_->get_number_of_children() > 1) {
        allele_count = this->data_.get_allele_count(
                pattern_index,
                this->data_.get_population_index(
                        this->root_->get_child(1)->get_label()));
        if (this->data_.markers_are_dominant()) {
            allele_count *= 2;
        }
        right_lineages.reserve(allele_count);
        for (unsigned int tip_idx = 0; tip_idx < allele_count; ++tip_idx) {
            std::shared_ptr<GeneTreeSimNode> tip = std::make_shared<GeneTreeSimNode>(
                        1,
                        0.0);
                tip->fix_node_height();
                right_lineages.push_back(tip);
        }

        double last_right_coal_height = this->coalesce_in_branch(
                right_lineages,
                this->get_node_theta(*this->root_->get_child(1)),
                rng,
                current_height,
                top_of_branch_height
                );
    }

    for (unsigned int i = 0; i < left_lineages.size(); ++i) {
        root_lineages.push_back(left_lineages.at(i));
    }
    left_lineages.clear();
    for (unsigned int i = 0; i < right_lineages.size(); ++i) {
        root_lineages.push_back(right_lineages.at(i));
    }
    right_lineages.clear();
    ECOEVOLITY_ASSERT(root_lineages.size() > 0);
    if (root_lineages.size() == 1) {
        return root_lineages.at(0);
    }
    double last_root_coal_height = this->coalesce_in_branch(
            root_lineages,
            this->get_node_theta(*this->root_),
            rng,
            top_of_branch_height,
            std::numeric_limits<double>::infinity()
            );
    ECOEVOLITY_ASSERT(root_lineages.size() == 1);
    return root_lineages.at(0);
}

double ComparisonPopulationTree::coalesce_in_branch(
        std::vector< std::shared_ptr<GeneTreeSimNode> >& lineages,
        double population_size,
        RandomNumberGenerator& rng,
        double bottom_of_branch_height,
        double top_of_branch_height
        ) {
    ECOEVOLITY_ASSERT(lineages.size() > 0);
    ECOEVOLITY_ASSERT(bottom_of_branch_height < top_of_branch_height);
    double current_height = bottom_of_branch_height;
    unsigned int k = lineages.size();
    while (true) {
        if (k == 1) {
            ECOEVOLITY_ASSERT(lineages.size() == k);
            break;
        }
        double scale = population_size / (((double)k) * (k - 1.0));
        double wait = rng.gamma(1.0, scale);
        
        if ((current_height + wait) >= top_of_branch_height) {
            break;
        }
        current_height += wait;
        std::shared_ptr<GeneTreeSimNode> mrca = std::make_shared<GeneTreeSimNode>(
                current_height);
        for (int i = 0; i < 2; ++i) {
            int idx = rng.uniform_int(0, lineages.size() - 1);
            mrca->add_child(lineages.at(idx));
            lineages.erase(lineages.begin() + idx);
        }
        lineages.push_back(mrca);
        --k;
        ECOEVOLITY_ASSERT(lineages.size() == k);
    }
    return current_height;
}

BiallelicData ComparisonPopulationTree::simulate_biallelic_data_set(
        RandomNumberGenerator& rng,
        bool validate) const {
    BiallelicData sim_data = this->data_.get_empty_copy();
    const bool filtering_constant_sites = this->constant_sites_removed_;
    // Looping over patterns to make sure simulated dataset has exact same
    // sample configuration (i.e., the same pattern of missing data) as the
    // member dataset.
    for (unsigned int pattern_idx = 0;
            pattern_idx < this->data_.get_number_of_patterns();
            ++pattern_idx) {
        for (unsigned int i = 0;
                i < this->data_.get_pattern_weight(pattern_idx);
                ++i) {
            bool site_added = false;
            while (! site_added) {
                auto pattern_tree = this->simulate_biallelic_site(
                        pattern_idx,
                        rng);
                auto pattern = pattern_tree.first;
                std::vector<unsigned int> red_allele_counts = pattern.first;
                std::vector<unsigned int> allele_counts = pattern.second;
                std::shared_ptr<GeneTreeSimNode> gtree = pattern_tree.second;
                site_added = sim_data.add_site(red_allele_counts,
                        allele_counts,
                        filtering_constant_sites);
            }
        }
    }
    sim_data.update_max_allele_counts();
    sim_data.update_pattern_booleans();
    if (validate) {
        sim_data.validate();
    }
    // What about gene trees? Write to stream/path (don't want to store/return
    // giant vector)
    return sim_data;
}

std::pair<
        std::pair<std::vector<unsigned int>, std::vector<unsigned int> >,
        std::shared_ptr<GeneTreeSimNode> >
ComparisonPopulationTree::simulate_biallelic_site(
        const unsigned int pattern_idx,
        RandomNumberGenerator& rng) const {

    std::shared_ptr<GeneTreeSimNode> gene_tree = this->simulate_gene_tree(pattern_idx, rng);
    gene_tree->compute_binary_transition_probabilities(this->get_u(), this->get_v());
    gene_tree->simulate_binary_character(this->get_freq_0(), rng);

    const std::vector<unsigned int>& expected_allele_counts = this->data_.get_allele_counts(pattern_idx);
    std::vector<unsigned int> allele_counts(expected_allele_counts.size(), 0);
    std::vector<unsigned int> red_allele_counts(expected_allele_counts.size(), 0);
    if (this->data_.markers_are_dominant()) {
        std::vector<int> last_allele(expected_allele_counts.size(), -1);
        gene_tree->get_allele_counts(
                allele_counts,
                red_allele_counts,
                last_allele);
    }
    else {
        gene_tree->get_allele_counts(
                allele_counts,
                red_allele_counts);
    }
    ECOEVOLITY_ASSERT(allele_counts == expected_allele_counts);
    std::pair<std::vector<unsigned int>, std::vector<unsigned int> > pattern = 
            std::make_pair(red_allele_counts, allele_counts);
    return std::make_pair(pattern, gene_tree);
}

void ComparisonPopulationTree::draw_from_prior(RandomNumberGenerator& rng) {
    if ((! this->state_frequencies_are_fixed()) && (! this->state_frequencies_are_constrained())) {
        this->freq_1_->set_value_from_prior(rng);
    }
    if (! this->mutation_rate_is_fixed()) {
        this->mutation_rate_->set_value_from_prior(rng);
    }
    if (! this->population_sizes_are_fixed()) {
        this->set_root_population_size(this->population_size_prior_->draw(rng));
        if (! this->population_sizes_are_constrained()) {
            this->set_child_population_size(0, this->population_size_prior_->draw(rng));
            if (this->root_->get_number_of_children() > 1) {
                this->set_child_population_size(1, this->population_size_prior_->draw(rng));
            }
        }
    }
}

double PopulationTree::compute_log_likelihood(
        unsigned int nthreads) {
    if (this->ignoring_data()) {
        this->log_likelihood_.set_value(0.0);
        return 0.0;
    }
    double all_red_pattern_prob = 0.0;
    double all_green_pattern_prob = 0.0;
    double log_likelihood = get_log_likelihood(
            this->get_mutable_root(),
            this->data_.get_red_allele_count_matrix(),
            this->data_.get_allele_count_matrix(),
            this->data_.get_pattern_weights(),
            this->data_.get_max_allele_counts(),
            this->get_u(),
            this->get_v(),
            this->get_mutation_rate(),
            this->get_ploidy(),
            this->data_.markers_are_dominant(),
            this->state_frequencies_are_constrained(),
            this->constant_sites_removed(),
            all_red_pattern_prob,
            all_green_pattern_prob,
            nthreads);

    this->all_red_pattern_likelihood_.set_value(all_red_pattern_prob);
    this->all_green_pattern_likelihood_.set_value(all_green_pattern_prob);

    if (this->constant_sites_removed()) {
        if (this->constant_site_counts_were_provided()) {
            double constant_log_likelihood =
                    ((double)this->get_provided_number_of_constant_green_sites() * std::log(all_green_pattern_prob)) +
                    ((double)this->get_provided_number_of_constant_red_sites() * std::log(all_red_pattern_prob));
            log_likelihood += constant_log_likelihood;
        }
        //////////////////////////////////////////////////////////////////////
        // No reason to use removed site counts. Simply leave constant sites in
        // and calc likelihood without correction. This is better, because it
        // doesn't treat all constant site patterns equally (i.e., it accounts
        // for constant patterns with missing data).
        // else if (this->use_removed_constant_site_counts_){
        //     double constant_log_likelihood =
        //             ((double)this->data_.get_number_of_constant_green_sites_removed() *
        //             std::log(this->all_green_pattern_likelihood_.get_value())) +
        //             ((double)this->data_.get_number_of_constant_red_sites_removed() *
        //             std::log(this->all_red_pattern_likelihood_.get_value()));
        //     log_likelihood += constant_log_likelihood;
        // }
        //////////////////////////////////////////////////////////////////////
        else {
            log_likelihood -= ((double)this->data_.get_number_of_sites() * 
                    std::log(1.0 - all_green_pattern_prob -
                            all_red_pattern_prob));
        }
    }


    log_likelihood += this->get_likelihood_correction();

    // ECOEVOLITY_DEBUG(
    //     std::cerr << "compute_log_likelihood(): " << log_likelihood << std::endl;
    // )

    this->log_likelihood_.set_value(log_likelihood);
    return log_likelihood;
}
