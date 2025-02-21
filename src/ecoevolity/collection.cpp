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

#include "collection.hpp"
#include "operator.hpp"

void BaseComparisonPopulationTreeCollection::store_state() {
    this->log_likelihood_.store();
    this->log_prior_density_.store();
    this->store_state_of_trees();
    for (unsigned int h = 0; h < this->node_heights_.size(); ++h) {
        this->node_heights_.at(h)->store();
    }
    if (! this->concentration_is_fixed()) {
        this->concentration_->store();
    }
    if (! this->discount_is_fixed()) {
        this->discount_->store();
    }
}
void BaseComparisonPopulationTreeCollection::restore_state() {
    this->log_likelihood_.restore();
    this->log_prior_density_.restore();
    this->restore_state_of_trees();
    for (unsigned int h = 0; h < this->node_heights_.size(); ++h) {
        this->node_heights_.at(h)->restore();
    }
    if (! this->concentration_is_fixed()) {
        this->concentration_->restore();
    }
    if (! this->discount_is_fixed()) {
        this->discount_->restore();
    }
}
void BaseComparisonPopulationTreeCollection::store_state_of_trees() {
    for (unsigned int t = 0; t < this->trees_.size(); ++t) {
        this->trees_.at(t)->store_state();
    }
}
void BaseComparisonPopulationTreeCollection::restore_state_of_trees() {
    for (unsigned int t = 0; t < this->trees_.size(); ++t) {
        this->trees_.at(t)->restore_state();
    }
}

void BaseComparisonPopulationTreeCollection::store_model_state() {
    this->log_likelihood_.store();
    this->log_prior_density_.store();
    this->store_state_of_trees();
    if (! this->concentration_is_fixed()) {
        this->concentration_->store();
    }
    if (! this->discount_is_fixed()) {
        this->discount_->store();
    }

    this->stored_node_heights_.resize(this->get_number_of_events());
    for (unsigned int h_idx = 0;
            h_idx < this->get_number_of_events();
            ++h_idx) {
        this->stored_node_heights_.at(h_idx) = this->get_height(h_idx);
    }
    this->stored_node_height_indices_ = this->node_height_indices_;
}

void BaseComparisonPopulationTreeCollection::restore_model_state() {
    ECOEVOLITY_ASSERT(
            (this->node_heights_.size() <= this->stored_node_heights_.size() + 1)
            &&
            (this->node_heights_.size() >= this->stored_node_heights_.size() - 1)
            );

    this->log_likelihood_.restore();
    this->log_prior_density_.restore();
    this->restore_state_of_trees();
    if (! this->concentration_is_fixed()) {
        this->concentration_->restore();
    }
    if (! this->discount_is_fixed()) {
        this->discount_->restore();
    }

    if (this->node_heights_.size() < this->stored_node_heights_.size()) {
        this->node_heights_.push_back(
                std::make_shared<PositiveRealParameter>(
                    this->node_height_prior_));
    }
    else if (this->node_heights_.size() > this->stored_node_heights_.size()){
        this->node_heights_.pop_back();
    }
    for (unsigned int h_idx = 0;
            h_idx < this->stored_node_heights_.size();
            ++h_idx) {
        this->node_heights_.at(h_idx)->set_value(
                this->stored_node_heights_.at(h_idx));
        this->node_heights_.at(h_idx)->store();
    }
    for (unsigned int t_idx = 0;
            t_idx < this->get_number_of_trees();
            ++t_idx) {
        unsigned int orig_index = this->stored_node_height_indices_.at(t_idx);
        this->node_height_indices_.at(t_idx) = orig_index;
        this->trees_.at(t_idx)->set_root_height_parameter(this->node_heights_.at(orig_index));
    }
}

double BaseComparisonPopulationTreeCollection::get_log_prior_density_of_node_heights() const {
    double lnp = 0.0;
    for (unsigned int h = 0; h < this->node_heights_.size(); ++h) {
        lnp += this->node_heights_.at(h)->relative_prior_ln_pdf();
    }
    return lnp;
}

void BaseComparisonPopulationTreeCollection::compute_log_likelihood_and_prior(
        bool compute_partials) {
    double lnl = 0.0;
    double lnp = 0.0;
    if (compute_partials) {
        this->compute_tree_partials();
    }
    for (unsigned int i = 0; i < this->trees_.size(); ++i) {
        lnl += this->trees_.at(i)->get_log_likelihood_value();
        lnp += this->trees_.at(i)->get_log_prior_density_value();
    }
    for (unsigned int h = 0; h < this->node_heights_.size(); ++h) {
        lnp += this->node_heights_.at(h)->relative_prior_ln_pdf();
    }
    if (! this->concentration_is_fixed()) {
        lnp += this->concentration_->relative_prior_ln_pdf();

    }
    if (! this->discount_is_fixed()) {
        lnp += this->discount_->relative_prior_ln_pdf();

    }
    // Compute the prior prob of model even when the concentration is fixed.
    // Previously, below was skipped if the concentration was fixed, which
    // worked because none of the moves that update the model (e.g.,
    // DirichletProcessGibbsSampler) needed the prior ratio to determine
    // acceptance. As a result, skipping this when not estimating the
    // concentration parameter saved a bit of computation. However, any new
    // moves that update the model that assume the prior is being taken care of
    // here would not work correcty, but would still sample from the joint
    // prior as if they were working! Thus, to make the code more future-proof,
    // we will always compute the model prior here.
    if (this->model_prior_ == EcoevolityOptions::ModelPrior::pyp) {
        lnp += get_pyp_log_prior_probability<unsigned int>(
                this->node_height_indices_,
                this->get_concentration(),
                this->get_discount());
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::dpp) {
        lnp += get_dpp_log_prior_probability<unsigned int>(
                this->node_height_indices_,
                this->get_concentration());
    }
    // TODO: the uniform model repurposes the concentration parameter for its
    // 'split_weight' parameter. Now that there's also discount parameter, it
    // might be worth explicitly adding a 'split_weight' attribute to avoid
    // confusion.
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::uniform) {
        lnp += get_uniform_model_log_prior_probability(
                this->get_number_of_trees(),
                this->get_number_of_events(),
                this->get_concentration());
    }
    // TODO: If we wanted to estimate the concentration, discount, or
    // split_weight while fixing the model, we would need to include the model
    // prior density in this calculation. However, this would not be possible
    // given given current YAML config settings. We would need to allow a
    // starting model to be specified, but not estimated.
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::fixed) {}
    else {
        std::ostringstream message;
        message << "ERROR: Unexpected EcoevolityOptions::ModelPrior \'"
                << (int)this->model_prior_
                << "\'\n";
        throw EcoevolityError(message.str());
    }

    this->log_likelihood_.set_value(lnl);
    this->log_prior_density_.set_value(lnp);
}

void BaseComparisonPopulationTreeCollection::compute_tree_partials() {
    for (unsigned int i = 0; i < this->trees_.size(); ++i) {
        this->trees_.at(i)->compute_log_likelihood_and_prior(this->number_of_threads_);
    }
}

// void BaseComparisonPopulationTreeCollection::compute_tree_partials_threaded() {
//     // TODO: get multithreading working
//     unsigned int tree_index = 0;
//     unsigned int max_tree_index = this->trees_.size() - 1;
//     unsigned int thread_count = 0;
//     while (tree_index <= max_tree_index) {
//         std::vector<std::thread> threads;
//         threads.reserve(this->number_of_threads_ - 1);
//         // Launch number_of_threads - 1 threads
//         for (unsigned int i = 0; i < this->number_of_threads_ - 1; ++i) {
//             if (tree_index > max_tree_index) {
//                 break;
//             }
//             threads.push_back(std::thread(
//                         &ComparisonPopulationTree::compute_log_likelihood_and_prior,
//                         this->trees_.at(tree_index)));
//             ++tree_index;
//         }
//         
//         // Use the main thread as the last thread
//         if (tree_index <= max_tree_index) {
//             this->trees_.at(tree_index).compute_log_likelihood_and_prior();
//             ++tree_index;
//             ++thread_count;
//         }
// 
//         // Join the launched threads
//         for (auto &t : threads) {
//             t.join();
//             ++thread_count;
//         }
//     }
//     // Make sure we calculated the likelihood of all the trees 
//     ECOEVOLITY_ASSERT(thread_count == this->trees_.size());
// }

void BaseComparisonPopulationTreeCollection::make_trees_clean() {
    for (unsigned int i = 0; i < this->trees_.size(); ++i) {
        this->trees_.at(i)->make_clean();
    }
}
void BaseComparisonPopulationTreeCollection::make_trees_dirty() {
    for (unsigned int i = 0; i < this->trees_.size(); ++i) {
        this->trees_.at(i)->make_dirty();
    }
}

std::vector<unsigned int> BaseComparisonPopulationTreeCollection::get_standardized_height_indices() const {
    std::vector<unsigned int> standardized_indices;
    standardized_indices.reserve(this->node_height_indices_.size());
    std::map<unsigned int, unsigned int> standardizing_map;
    unsigned int next_idx = 0;
    for (auto const raw_idx: this->node_height_indices_) {
        if (standardizing_map.count(raw_idx) == 0) {
            standardizing_map[raw_idx] = next_idx;
            ++next_idx;
        }
        standardized_indices.push_back(standardizing_map.at(raw_idx));
    }
    return standardized_indices;
}

unsigned int BaseComparisonPopulationTreeCollection::get_largest_height_index() const {
    unsigned int current_ht_idx = 0;
    for (unsigned int h_idx = 1;
            h_idx < this->get_number_of_events();
            ++h_idx) {
        if (this->get_height(h_idx) > this->get_height(current_ht_idx)) {
            current_ht_idx = h_idx;
        }
    }
    return current_ht_idx;
}

std::vector<unsigned int> BaseComparisonPopulationTreeCollection::get_height_indices_sans_largest() const {
    std::vector<unsigned int> indices;
    indices.reserve(this->get_number_of_events() - 1);
    unsigned int largest_height_index = this->get_largest_height_index();
    for (unsigned int i = 0; i < this->get_number_of_events(); ++i) {
        if (i != largest_height_index) {
            indices.push_back(i);
        }
    }
    return indices;
}

double BaseComparisonPopulationTreeCollection::get_nearest_smaller_height(
        unsigned int height_index) const {
    double nearest_smaller_height = 0.0;
    double ref_height = this->get_height(height_index);
    for (unsigned int h_idx = 0; h_idx < this->node_heights_.size(); ++h_idx) {
        if (h_idx == height_index) {
            continue;
        }
        double candidate_height = this->get_height(h_idx);
        if (candidate_height > ref_height) {
            continue;
        }
        if ((ref_height - candidate_height) < (ref_height - nearest_smaller_height)) {
            nearest_smaller_height = candidate_height;
        }
    }
    return nearest_smaller_height;
}

double BaseComparisonPopulationTreeCollection::get_nearest_larger_height(
        unsigned int height_index) const {
    double nearest_larger_height = std::numeric_limits<double>::infinity();
    double ref_height = this->get_height(height_index);
    bool found = false;
    for (unsigned int h_idx = 0; h_idx < this->node_heights_.size(); ++h_idx) {
        if (h_idx == height_index) {
            continue;
        }
        double candidate_height = this->get_height(h_idx);
        if (candidate_height < ref_height) {
            continue;
        }
        if ((candidate_height - ref_height) < (nearest_larger_height - ref_height)) {
            nearest_larger_height = candidate_height;
            found = true;
        }
    }
    if (! found) {
        return ref_height;
    }
    return nearest_larger_height;
}

unsigned int BaseComparisonPopulationTreeCollection::get_nearest_smaller_height_index(
        unsigned int height_index,
        bool allow_smallest_index) const {
    double ref_height = this->get_height(height_index);
    double nearest_smaller_height = 0.0;
    int current_nearest_idx = -1;
    for (unsigned int h_idx = 0;
            h_idx < this->get_number_of_events();
            ++h_idx) {
        if (h_idx == height_index) {
            continue;
        }
        double candidate_height = this->get_height(h_idx);
        if (candidate_height > ref_height) {
            continue;
        }
        if ((ref_height - candidate_height) < (ref_height - nearest_smaller_height)) {
            nearest_smaller_height = candidate_height;
            current_nearest_idx = h_idx;
        }
    }
    if (current_nearest_idx < 0) {
        if (allow_smallest_index) {
            return height_index;
        }
        else {
            throw EcoevolityError(
                    "BaseComparisonPopulationTreeCollection::get_nearest_smaller_height_index "
                    "was called with smallest height index");
        }
    }
    return current_nearest_idx;
}

unsigned int BaseComparisonPopulationTreeCollection::get_nearest_larger_height_index(
        unsigned int height_index,
        bool allow_largest_index) const {
    double ref_height = this->get_height(height_index);
    double nearest_larger_height = std::numeric_limits<double>::max();
    int current_nearest_idx = -1;
    for (unsigned int h_idx = 0;
            h_idx < this->get_number_of_events();
            ++h_idx) {
        if (h_idx == height_index) {
            continue;
        }
        double candidate_height = this->get_height(h_idx);
        if (candidate_height < ref_height) {
            continue;
        }
        if ((candidate_height - ref_height) < (nearest_larger_height - ref_height)) {
            nearest_larger_height = candidate_height;
            current_nearest_idx = h_idx;
        }
    }
    if (current_nearest_idx < 0) {
        if (allow_largest_index) {
            return height_index;
        }
        else {
            throw EcoevolityError(
                    "BaseComparisonPopulationTreeCollection::get_nearest_larger_height_index "
                    "was called with largest height index");
        }
    }
    return current_nearest_idx;
}

unsigned int BaseComparisonPopulationTreeCollection::get_distal_height_index_within_move(
                unsigned int starting_height_index,
                double delta_height) const {

    double starting_height = this->get_height(starting_height_index);
    double lower_limit = 0.0;
    double upper_limit = 0.0;
    if (delta_height > 0.0) {
        lower_limit = starting_height;
        upper_limit = starting_height + delta_height;
    }
    else {
        lower_limit = starting_height + delta_height;
        upper_limit = starting_height;
    }
    int current_ht_idx = -1;
    double current_largest_distance = -1.0;
    double distance = -1.0;
    double height = -1.0;
    for (unsigned int h_idx = 0;
            h_idx < this->get_number_of_events();
            ++h_idx) {
        if (h_idx == starting_height_index) {
            continue;
        }
        height = this->get_height(h_idx);
        if ((height > lower_limit) && (height < upper_limit)) {
            distance = std::abs(height - starting_height);
            if (distance > current_largest_distance) {
                current_ht_idx = h_idx;
                current_largest_distance = distance; 
            }
        }
    }
    if (current_ht_idx < 0) {
        return starting_height_index;
    }
    return current_ht_idx;
}

unsigned int BaseComparisonPopulationTreeCollection::get_number_of_trees_mapped_to_height(
        unsigned int height_index) const {
    unsigned int count = 0;
    for (auto h_index_iter : this->node_height_indices_) {
        if (h_index_iter == height_index) {
            ++count;
        }
    }
    return count;
}

std::vector<unsigned int> BaseComparisonPopulationTreeCollection::get_other_height_indices(
        unsigned int tree_index) const {
    std::vector<unsigned int> others;
    others.reserve(this->node_heights_.size());
    if (this->get_number_of_partners(tree_index) > 0) {
        for (unsigned int i = 0; i < this->node_heights_.size(); ++i) {
            others.push_back(i);
        }
    }
    else {
        unsigned int removed_height_index = this->get_height_index(tree_index);
        for (unsigned int i = 0; i < this->node_heights_.size(); ++i) {
            if (i != removed_height_index) {
                others.push_back(i);
            }
        }
    }
    return others;
}

std::vector<unsigned int> BaseComparisonPopulationTreeCollection::get_indices_of_mapped_trees(
        unsigned int height_index) const {
    std::vector<unsigned int> indices;
    for (unsigned int tree_idx = 0;
            tree_idx < this->get_number_of_trees();
            ++tree_idx) {
        if (this->get_height_index(tree_idx) == height_index) {
            indices.push_back(tree_idx);
        }
    }
    return indices;
}

unsigned int BaseComparisonPopulationTreeCollection::get_number_of_partners(
        unsigned int tree_index) const {
    unsigned int h_index = this->node_height_indices_.at(tree_index);
    unsigned int count = this->get_number_of_trees_mapped_to_height(h_index);
    // should always find at least itself
    ECOEVOLITY_ASSERT(count > 0);
    return count - 1;
}

std::vector<unsigned int> BaseComparisonPopulationTreeCollection::get_shared_event_indices() const {
    std::vector<unsigned int> shared_indices;
    for (unsigned int event_idx = 0;
            event_idx < this->get_number_of_events();
            ++event_idx) {
        if (this->get_number_of_trees_mapped_to_height(event_idx) > 1) {
            shared_indices.push_back(event_idx);
        }
    }
    return shared_indices;
}

void BaseComparisonPopulationTreeCollection::remap_tree(
        unsigned int tree_index,
        unsigned int height_index) {
    if (this->get_height_index(tree_index) != height_index) {
        unsigned int num_partners = this->get_number_of_partners(tree_index);
        unsigned int old_height_index = this->get_height_index(tree_index);
        this->node_height_indices_.at(tree_index) = height_index;
        this->trees_.at(tree_index)->set_root_height_parameter(
                this->get_height_parameter(height_index));
        if (num_partners == 0) {
            this->remove_height(old_height_index);
        }
    }
}

void BaseComparisonPopulationTreeCollection::remap_tree(
        unsigned int tree_index,
        unsigned int height_index,
        double log_likelihood) {
    this->remap_tree(tree_index, height_index);
    this->trees_.at(tree_index)->set_log_likelihood_value(log_likelihood);
    this->trees_.at(tree_index)->make_clean();
}

unsigned int BaseComparisonPopulationTreeCollection::remap_trees(
        const std::vector<unsigned int>& tree_indices,
        unsigned int height_index) {
    ECOEVOLITY_ASSERT(tree_indices.size() > 0);
    unsigned int current_target_index = height_index;
    for (auto const tree_idx: tree_indices) {
        this->remap_tree(tree_idx, current_target_index);
        // unsigned int current_target_index = this->get_height_index(tree_indices.at(0));
        current_target_index = this->get_height_index(tree_idx);
    }
    return current_target_index;
}

void BaseComparisonPopulationTreeCollection::map_tree_to_new_height(
        unsigned int tree_index,
        double height) {
    unsigned int num_partners = this->get_number_of_partners(tree_index);
    unsigned int old_height_index = this->get_height_index(tree_index);
    std::vector<unsigned int> mapped_tree_indices = {tree_index};
    this->add_height(height, mapped_tree_indices);
    if (num_partners == 0) {
        this->remove_height(old_height_index);
    }
}

void BaseComparisonPopulationTreeCollection::map_tree_to_new_height(
        unsigned int tree_index,
        double height,
        double log_likelihood) {
    this->map_tree_to_new_height(tree_index, height);
    this->trees_.at(tree_index)->set_log_likelihood_value(log_likelihood);
    this->trees_.at(tree_index)->make_clean();
}

unsigned int BaseComparisonPopulationTreeCollection::map_trees_to_new_height(
        const std::vector<unsigned int>& tree_indices,
        double height) {
    ECOEVOLITY_ASSERT(tree_indices.size() > 0);
    this->map_tree_to_new_height(tree_indices.at(0), height);
    unsigned int new_height_index = this->get_height_index(tree_indices.at(0));
    for (unsigned int i = 1; i < tree_indices.size(); ++i) {
        this->remap_tree(tree_indices.at(i), new_height_index);
    }
    return new_height_index;
}

unsigned int BaseComparisonPopulationTreeCollection::merge_height(
        unsigned int height_index,
        unsigned int target_height_index) {
    std::vector<unsigned int> mapped_trees = this->get_indices_of_mapped_trees(height_index);
    unsigned int final_index = this->remap_trees(mapped_trees, target_height_index);
    return final_index;
}

void BaseComparisonPopulationTreeCollection::remove_height(
        unsigned int height_index) {
    ECOEVOLITY_ASSERT(this->get_number_of_trees_mapped_to_height(height_index) == 0);
    // ECOEVOLITY_ASSERT(this->node_heights_.at(height_index).use_count() < 2);
    this->node_heights_.erase(this->node_heights_.begin() + height_index);
    for (unsigned int i = 0; i < this->node_height_indices_.size(); ++i) {
        ECOEVOLITY_ASSERT(this->node_height_indices_.at(i) != height_index);
        if (this->node_height_indices_.at(i) > height_index) {
            --this->node_height_indices_.at(i);
        }
    }
}

void BaseComparisonPopulationTreeCollection::add_height(
        double height,
        const std::vector<unsigned int>& mapped_tree_indices) {
    this->node_heights_.push_back(std::make_shared<PositiveRealParameter>(this->node_height_prior_, height));
    for (auto tree_idx : mapped_tree_indices) {
        this->node_height_indices_.at(tree_idx) = this->node_heights_.size() - 1;
        this->trees_.at(tree_idx)->set_root_height_parameter(this->node_heights_.back());
    }
}

void BaseComparisonPopulationTreeCollection::write_state_log_header(
        std::ostream& out,
        bool short_summary) const {
    out << "generation" << this->logging_delimiter_
        << "ln_likelihood" << this->logging_delimiter_
        << "ln_prior" << this->logging_delimiter_
        << "number_of_events";
    if (this->model_prior_ == EcoevolityOptions::ModelPrior::pyp) {
        out << this->logging_delimiter_ << "concentration"
            << this->logging_delimiter_ << "discount";
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::dpp) {
        out << this->logging_delimiter_ << "concentration";
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::uniform) {
        out << this->logging_delimiter_ << "split_weight";
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::fixed) {
        // Nothing to report
    }
    if (short_summary) {
        out << std::endl;
        return;
    }
    for (unsigned int tree_idx = 0;
            tree_idx < this->trees_.size();
            ++tree_idx) {
        out << this->logging_delimiter_;
        this->trees_.at(tree_idx)->write_comparison_state_log_header(out,
                true,
                this->logging_delimiter_);
    }
    out << std::endl;
}

void BaseComparisonPopulationTreeCollection::log_state(std::ostream& out,
        unsigned int generation_index,
        bool short_summary) const {
    out << generation_index << this->logging_delimiter_
        << this->log_likelihood_.get_value() << this->logging_delimiter_
        << this->log_prior_density_.get_value() << this->logging_delimiter_
        << this->get_number_of_events();
    if (this->model_prior_ == EcoevolityOptions::ModelPrior::pyp) {
        out << this->logging_delimiter_ << this->get_concentration()
            << this->logging_delimiter_ << this->get_discount();
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::dpp) {
        out << this->logging_delimiter_ << this->get_concentration();
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::uniform) {
        out << this->logging_delimiter_ << this->get_concentration();
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::fixed) {
        // Nothing to report
    }
    if (short_summary) {
        out << std::endl;
        return;
    }
    std::vector<unsigned int> standardized_height_indices =
            this->get_standardized_height_indices();
    for (unsigned int tree_idx = 0;
            tree_idx < this->trees_.size();
            ++tree_idx) {
        out << this->logging_delimiter_;
        this->trees_.at(tree_idx)->log_comparison_state(out,
                standardized_height_indices.at(tree_idx),
                this->logging_delimiter_);
    }
    out << std::endl;
}

void BaseComparisonPopulationTreeCollection::update_log_paths(
        unsigned int max_number_of_attempts) {
    if (! path::exists(this->get_state_log_path())) {
        return;
    }
    unsigned int ntries = 0;
    while (true) {
        this->increment_log_paths();
        ++ntries;
        if (! path::exists(this->get_state_log_path())) {
            break;
        }
        if (ntries > max_number_of_attempts) {
            throw EcoevolityError("Could not generate unique output files");
        }
    }
}

void BaseComparisonPopulationTreeCollection::increment_log_paths() {
    std::vector<std::string> path_elements;
    std::pair<std::string, std::string> prefix_ext;
    std::string new_suffix;
    int run_number;

    prefix_ext = path::splitext(this->get_state_log_path());
    path_elements = string_util::split(prefix_ext.first, '-');
    run_number = std::stoi(path_elements.back());
    path_elements.pop_back();
    ++run_number;
    
    new_suffix = "-" + std::to_string(run_number) + prefix_ext.second;

    this->set_state_log_path(
            string_util::join(path_elements, "-") + new_suffix);

    prefix_ext = path::splitext(this->get_operator_log_path());
    path_elements = string_util::split(prefix_ext.first, '-');
    path_elements.pop_back();
    this->set_operator_log_path(
            string_util::join(path_elements, "-") + new_suffix);
}

void BaseComparisonPopulationTreeCollection::mcmc(
        RandomNumberGenerator& rng,
        unsigned int chain_length,
        unsigned int sample_frequency) {

    std::ofstream state_log_stream;
    std::ofstream operator_log_stream;
    this->update_log_paths();
    if (path::exists(this->get_state_log_path())) {
        std::ostringstream message;
        message << "ERROR: The parameter log file \'"
                << this->get_state_log_path()
                << "\' already exists!\n";
        throw EcoevolityError(message.str());
    }
    if (path::exists(this->get_operator_log_path())) {
        std::ostringstream message;
        message << "ERROR: The operator log file \'"
                << this->get_operator_log_path()
                << "\' already exists!\n";
        throw EcoevolityError(message.str());
    }
    state_log_stream.open(this->get_state_log_path());
    operator_log_stream.open(this->get_operator_log_path());
    
    if (! state_log_stream.is_open()) {
        std::ostringstream message;
        message << "ERROR: Could not open parameter log file \'"
                << this->get_state_log_path()
                << "\'\n";
        throw EcoevolityError(message.str());
    }
    if (! operator_log_stream.is_open()) {
        std::ostringstream message;
        message << "ERROR: Could not open operator log file \'"
                << this->get_operator_log_path()
                << "\'\n";
        throw EcoevolityError(message.str());
    }

    std::cout << "State log path: " << this->get_state_log_path() << std::endl;
    std::cout << "Operator log path: " << this->get_operator_log_path() << std::endl;
    
    state_log_stream.precision(this->get_logging_precision());
    operator_log_stream.precision(this->get_logging_precision());

    this->write_state_log_header(state_log_stream);
    this->write_state_log_header(std::cout, true);

    this->make_trees_dirty();
    this->compute_log_likelihood_and_prior(true);
    if (this->get_log_likelihood() == -std::numeric_limits<double>::infinity()) {
        std::ostringstream message;
        message << "\n"
                << "\n#######################################################################\n"
                <<   "###############################  ERROR  ###############################\n"
                <<   "The initial model state has a probability density of zero.\n"
                <<   "#######################################################################\n";
        throw EcoevolityError(message.str());
    }
    if (std::isnan(this->get_log_likelihood())) {
        std::ostringstream message;
        message << "\n"
                << "\n#######################################################################\n"
                <<   "###############################  ERROR  ###############################\n"
                <<   "The initial model state has a NAN log likelihood.\n"
                <<   "#######################################################################\n";
        throw EcoevolityError(message.str());
    }
    this->log_state(state_log_stream, 0);
    this->log_state(std::cout, 0, true);

    unsigned int gen;
    unsigned int gen_of_last_state_log = 0;
    unsigned int gen_of_last_operator_log = 0;
    for (gen = 0; gen < chain_length; ++gen) {
        OperatorInterface& op = this->operator_schedule_.draw_operator(rng);
        op.operate(rng, this, this->get_number_of_threads());

        if ((gen + 1) % sample_frequency == 0) {
            this->log_state(state_log_stream, gen + 1);
            gen_of_last_state_log = gen;
            // Log every 10th sample to std out
            if ((gen + 1) % (sample_frequency * 10) == 0) {
                this->log_state(std::cout, gen + 1, true);
                // Log operator performance every 100 samples
                if ((gen + 1) % (sample_frequency * 100) == 0) {
                    operator_log_stream << "generation " << gen + 1 << ":\n";
                    this->operator_schedule_.write_operator_rates(operator_log_stream);
                    gen_of_last_operator_log = gen;
                }
            }
        }

        // Check if the chain has gone astray
        ECOEVOLITY_DEBUG(
            if ((gen + 1) % (sample_frequency * 100) == 0) {
                double chain_ln_likelihood = this->log_likelihood_.get_value();
                this->make_trees_dirty();
                this->compute_log_likelihood_and_prior(true);
                double expected_ln_likelihood = this->log_likelihood_.get_value();
                if (fabs(chain_ln_likelihood - expected_ln_likelihood) > 1e-6) {
                    std::ostringstream message;
                    message << "ERROR: The likelihood at generation "
                            << gen
                            << " is " << chain_ln_likelihood
                            << "; expected " << expected_ln_likelihood
                            << "; last operator " << op.get_name();
                    state_log_stream.close();
                    operator_log_stream.close();
                    throw EcoevolityError(message.str());
                }
            }
        )
    }
    // Make sure last generation is reported
    if (gen > (gen_of_last_state_log + 1)) {
        this->log_state(state_log_stream, gen + 1);
        this->log_state(std::cout, gen + 1, true);
    }
    if (gen > (gen_of_last_operator_log + 1)) {
        operator_log_stream << "generation " << gen + 1 << ":\n";
        this->operator_schedule_.write_operator_rates(operator_log_stream);
    }
    std::cout << "\nOperator stats:\n";
    this->operator_schedule_.write_operator_rates(std::cout);
    std::cout << "\n";
    state_log_stream.close();
    operator_log_stream.close();
}

void BaseComparisonPopulationTreeCollection::write_summary(
        std::ostream& out,
        unsigned int indent_level) const {
    std::string margin = string_util::get_indent(indent_level);
    std::string indent = string_util::get_indent(1);
    out << "Summary of data from " << this->get_number_of_trees() << " comparisons:\n";
    for (auto tree: this->trees_) {
        tree->write_data_summary(out, indent_level + 1);
    }
}

void BaseComparisonPopulationTreeCollection::set_node_height_indices(
        const std::vector<unsigned int>& indices,
        RandomNumberGenerator & rng) {
    std::unordered_set<unsigned int> index_set;
    for (auto i: indices) {
        index_set.insert(i);
    }
    if (index_set.size() > this->get_number_of_trees()) {
        throw EcoevolityError("event/node height indices are not valid.");
    }
    for (unsigned int i = 0; i < index_set.size(); ++i) {
        if (index_set.count(i) < 1) {
            throw EcoevolityError("event/node height indices are not valid.");
        }
    }

    this->node_height_indices_ = indices;

    unsigned int num_heights = this->node_heights_.size();
    unsigned int new_num_heights = index_set.size();
    if (new_num_heights > num_heights) {
        for (unsigned int i = 0; i < (new_num_heights - num_heights); ++i) {
            this->node_heights_.push_back(
                    std::make_shared<PositiveRealParameter>(
                        this->node_height_prior_));
        }
    }
    else if (new_num_heights < num_heights) {
        for (unsigned int i = 0; i < (num_heights - new_num_heights); ++i) {
            this->node_heights_.pop_back();
        }
    }
    for (unsigned int i = 0; i < new_num_heights; ++i) {
        this->node_heights_.at(i)->set_value(this->node_height_prior_->draw(rng));
    }
    for (unsigned int tree_idx = 0;
            tree_idx < this->trees_.size();
            ++tree_idx) {
        unsigned int height_index = this->node_height_indices_.at(tree_idx);
        this->trees_.at(tree_idx)->set_root_height_parameter(
                this->get_height_parameter(height_index));
    }
}

void BaseComparisonPopulationTreeCollection::draw_heights_from_prior(RandomNumberGenerator& rng) {
    if (this->model_prior_ == EcoevolityOptions::ModelPrior::fixed) {
        for (unsigned int height_idx = 0;
                height_idx < this->node_heights_.size();
                ++height_idx) {
            this->node_heights_.at(height_idx)->set_value(
                    this->node_height_prior_->draw(rng));
        }
        return;
    }
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // No longer need to use rjMCMC to sample from the uniform prior on event
    // models
    //////////////////////////////////////////////////////////////////////////
    // else if (this->model_prior_ == EcoevolityOptions::ModelPrior::uniform) {
    //     // Not aware of an "easy" way of uniformly sampling set partitions, so
    //     // (hackily) using reversible jump MCMC to do so.
    //     bool was_ignoring_data = this->ignoring_data();
    //     this->ignore_data();
    //     OperatorInterface& op = this->operator_schedule_.get_reversible_jump_operator();
    //     for (unsigned int i = 0; i < (this->get_number_of_trees() * 2); ++i) {
    //         op.operate(rng, this);
    //     }
    //     if (! was_ignoring_data) {
    //         this->use_data();
    //     }
    //     for (unsigned int height_idx = 0;
    //             height_idx < this->node_heights_.size();
    //             ++height_idx) {
    //         this->node_heights_.at(height_idx)->set_value(
    //                 this->node_height_prior_->draw(rng));
    //     }
    //     unsigned int num_heights = this->node_heights_.size();
    //     unsigned int new_num_heights;
    //     new_num_heights = rng.random_set_partition(
    //             this->node_height_indices_,
    //             this->get_concentration());
    //     return;
    // }
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    unsigned int num_heights = this->node_heights_.size();
    unsigned int new_num_heights;
    if (this->model_prior_ == EcoevolityOptions::ModelPrior::uniform) {
        new_num_heights = rng.random_set_partition(
                this->node_height_indices_,
                this->get_concentration() // concentration is co-opted for
                );                        // split_weight
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::pyp) {
        new_num_heights = rng.pitman_yor_process(
                this->node_height_indices_,
                this->get_concentration(),
                this->get_discount());
    }
    else if (this->model_prior_ == EcoevolityOptions::ModelPrior::dpp) {
        new_num_heights = rng.dirichlet_process(
                this->node_height_indices_,
                this->get_concentration());
    }
    else {
        std::ostringstream message;
        message << "ERROR: Unexpected EcoevolityOptions::ModelPrior \'"
                << (int)this->model_prior_
                << "\'\n";
        throw EcoevolityError(message.str());
    }
    if (new_num_heights > num_heights) {
        for (unsigned int i = 0; i < (new_num_heights - num_heights); ++i) {
            this->node_heights_.push_back(
                    std::make_shared<PositiveRealParameter>(
                        this->node_height_prior_));
        }
    }
    else if (new_num_heights < num_heights) {
        for (unsigned int i = 0; i < (num_heights - new_num_heights); ++i) {
            this->node_heights_.pop_back();
        }
    }
    for (unsigned int i = 0; i < new_num_heights; ++i) {
        this->node_heights_.at(i)->set_value(this->node_height_prior_->draw(rng));
    }
    for (unsigned int tree_idx = 0;
            tree_idx < this->trees_.size();
            ++tree_idx) {
        unsigned int height_index = this->node_height_indices_.at(tree_idx);
        this->trees_.at(tree_idx)->set_root_height_parameter(
                this->get_height_parameter(height_index));
    }
}

void BaseComparisonPopulationTreeCollection::draw_from_prior(RandomNumberGenerator& rng) {
    this->concentration_->set_value_from_prior(rng);
    this->discount_->set_value_from_prior(rng);
    this->draw_heights_from_prior(rng);
    for (unsigned int i = 0; i < this->trees_.size(); ++i) {
        this->trees_.at(i)->draw_from_prior(rng);
    }
}

std::map<std::string, BiallelicData> BaseComparisonPopulationTreeCollection::simulate_biallelic_data_sets(
        RandomNumberGenerator& rng,
        float singleton_sample_probability,
        bool validate) const {
    std::map<std::string, BiallelicData> alignments;
    for (auto tree: this->trees_) {
        alignments[tree->get_data().get_path()] = tree->simulate_biallelic_data_set(
                rng, singleton_sample_probability, validate);
    }
    return alignments;
}

std::map<std::string, BiallelicData> BaseComparisonPopulationTreeCollection::simulate_complete_biallelic_data_sets(
        RandomNumberGenerator& rng,
        unsigned int locus_size,
        float singleton_sample_probability,
        bool max_one_variable_site_per_locus,
        bool validate) const {
    std::map<std::string, BiallelicData> alignments;
    if (max_one_variable_site_per_locus) {
        for (auto tree: this->trees_) {
            auto alignment_nloci = tree->simulate_data_set_max_one_variable_site_per_locus(
                    rng, locus_size, singleton_sample_probability, validate);
            alignments[tree->get_data().get_path()] = alignment_nloci.first;
        }
    }
    else {
        for (auto tree: this->trees_) {
            auto alignment_nloci = tree->simulate_complete_biallelic_data_set(
                    rng, locus_size, singleton_sample_probability, validate);
            alignments[tree->get_data().get_path()] = alignment_nloci.first;
        }
    }
    return alignments;
}

std::map<std::string, BiallelicData> BaseComparisonPopulationTreeCollection::simulate_linked_biallelic_data_sets(
        RandomNumberGenerator& rng,
        float singleton_sample_probability,
        bool max_one_variable_site_per_locus,
        bool validate) const {
    std::map<std::string, BiallelicData> alignments;
    for (auto tree: this->trees_) {
        alignments[tree->get_data().get_path()] = tree->simulate_linked_biallelic_data_set(
                rng,
                singleton_sample_probability,
                max_one_variable_site_per_locus,
                validate);
    }
    return alignments;
}



ComparisonPopulationTreeCollection::ComparisonPopulationTreeCollection(
        const CollectionSettings & settings,
        RandomNumberGenerator & rng,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        bool strict_on_triallelic_sites,
        bool store_seq_loci_info
        ) : BaseComparisonPopulationTreeCollection() {
    this->state_log_path_ = settings.get_state_log_path();
    this->operator_log_path_ = settings.get_operator_log_path();
    this->node_height_prior_ = settings.get_time_prior_settings().get_instance();
    this->concentration_ = std::make_shared<PositiveRealParameter>(
            settings.get_concentration_settings(),
            rng);
    this->discount_ = std::make_shared<DiscountParameter>(
            settings.get_discount_settings(),
            rng);
    this->operator_schedule_ = OperatorSchedule(
            settings);
    if (this->operator_schedule_.get_total_weight() <= 0.0) {
        throw EcoevolityError("No operators have weight");
    }
    this->init_trees(settings.get_comparison_settings(),
            rng,
            strict_on_constant_sites,
            strict_on_missing_sites,
            strict_on_triallelic_sites,
            store_seq_loci_info);
    this->stored_node_heights_.reserve(this->trees_.size());
    this->stored_node_height_indices_.reserve(this->trees_.size());
    if (settings.event_model_is_fixed()) {
        this->set_node_height_indices(settings.get_fixed_event_model_indices(), rng);
    }
    this->model_prior_ = settings.get_model_prior();
}

void ComparisonPopulationTreeCollection::init_trees(
        const std::vector<ComparisonSettings> & comparison_settings,
        RandomNumberGenerator & rng,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        bool strict_on_triallelic_sites,
        bool store_seq_loci_info
        ) {
    std::unordered_set<std::string> population_labels;
    double fresh_height;
    for (unsigned int tree_idx = 0;
            tree_idx < comparison_settings.size();
            ++tree_idx) {
        fresh_height = this->node_height_prior_->draw(rng);
        std::shared_ptr<PositiveRealParameter> new_height_parameter = std::make_shared<PositiveRealParameter>(this->node_height_prior_, fresh_height);
        std::shared_ptr<PopulationTree> new_tree = std::make_shared<ComparisonPopulationTree>(
                comparison_settings.at(tree_idx),
                rng,
                strict_on_constant_sites,
                strict_on_missing_sites,
                strict_on_triallelic_sites,
                store_seq_loci_info
                );
        for (auto const& pop_label: new_tree->get_population_labels()) {
            auto p = population_labels.insert(pop_label);
            if (! p.second) {
                std::ostringstream message;
                message << "\n#######################################################################\n"
                        <<   "###############################  ERROR  ###############################\n"
                        << "Population label conflict. The population label:\n"
                        << "\'" << pop_label << "\'\n"
                        << "is used in multiple alignments.\n"
                        << "#######################################################################\n";
                throw EcoevolityCollectionSettingError(message.str());
            }
        }
        new_tree->set_node_height_prior(this->node_height_prior_);
        new_tree->set_root_height_parameter(new_height_parameter);
        this->node_heights_.push_back(new_height_parameter);
        this->trees_.push_back(new_tree);
        this->node_height_indices_.push_back(tree_idx);
        ECOEVOLITY_ASSERT(
                this->trees_.at(tree_idx)->get_root_height_parameter() ==
                this->node_heights_.at(this->node_height_indices_.at(tree_idx))
                );
    }
    ECOEVOLITY_ASSERT(this->trees_.size() == this->node_heights_.size());
    ECOEVOLITY_ASSERT(this->trees_.size() == this->node_height_indices_.size());
}




ComparisonRelativeRootPopulationTreeCollection::ComparisonRelativeRootPopulationTreeCollection(
        const RelativeRootCollectionSettings & settings,
        RandomNumberGenerator & rng,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        bool strict_on_triallelic_sites,
        bool store_seq_loci_info
        ) : BaseComparisonPopulationTreeCollection() {
    this->state_log_path_ = settings.get_state_log_path();
    this->operator_log_path_ = settings.get_operator_log_path();
    this->node_height_prior_ = settings.get_time_prior_settings().get_instance();
    this->concentration_ = std::make_shared<PositiveRealParameter>(
            settings.get_concentration_settings(),
            rng);
    this->discount_ = std::make_shared<DiscountParameter>(
            settings.get_discount_settings(),
            rng);
    this->operator_schedule_ = OperatorSchedule(
            settings);
    if (this->operator_schedule_.get_total_weight() <= 0.0) {
        throw EcoevolityError("No operators have weight");
    }
    this->init_trees(settings.get_comparison_settings(),
            rng,
            strict_on_constant_sites,
            strict_on_missing_sites,
            strict_on_triallelic_sites,
            store_seq_loci_info);
    this->stored_node_heights_.reserve(this->trees_.size());
    this->stored_node_height_indices_.reserve(this->trees_.size());
    if (settings.event_model_is_fixed()) {
        this->set_node_height_indices(settings.get_fixed_event_model_indices(), rng);
    }
    this->model_prior_ = settings.get_model_prior();
}

void ComparisonRelativeRootPopulationTreeCollection::init_trees(
        const std::vector<RelativeRootComparisonSettings> & comparison_settings,
        RandomNumberGenerator & rng,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        bool strict_on_triallelic_sites,
        bool store_seq_loci_info
        ) {
    std::unordered_set<std::string> population_labels;
    double fresh_height;
    for (unsigned int tree_idx = 0;
            tree_idx < comparison_settings.size();
            ++tree_idx) {
        fresh_height = this->node_height_prior_->draw(rng);
        std::shared_ptr<PositiveRealParameter> new_height_parameter = std::make_shared<PositiveRealParameter>(this->node_height_prior_, fresh_height);
        std::shared_ptr<PopulationTree> new_tree = std::make_shared<ComparisonRelativeRootPopulationTree>(
                comparison_settings.at(tree_idx),
                rng,
                strict_on_constant_sites,
                strict_on_missing_sites,
                strict_on_triallelic_sites,
                store_seq_loci_info
                );
        for (auto const& pop_label: new_tree->get_population_labels()) {
            auto p = population_labels.insert(pop_label);
            if (! p.second) {
                std::ostringstream message;
                message << "\n#######################################################################\n"
                        <<   "###############################  ERROR  ###############################\n"
                        << "Population label conflict. The population label:\n"
                        << "\'" << pop_label << "\'\n"
                        << "is used in multiple alignments.\n"
                        << "#######################################################################\n";
                throw EcoevolityCollectionSettingError(message.str());
            }
        }
        new_tree->set_node_height_prior(this->node_height_prior_);
        new_tree->set_root_height_parameter(new_height_parameter);
        this->node_heights_.push_back(new_height_parameter);
        this->trees_.push_back(new_tree);
        this->node_height_indices_.push_back(tree_idx);
        ECOEVOLITY_ASSERT(
                this->trees_.at(tree_idx)->get_root_height_parameter() ==
                this->node_heights_.at(this->node_height_indices_.at(tree_idx))
                );
    }
    ECOEVOLITY_ASSERT(this->trees_.size() == this->node_heights_.size());
    ECOEVOLITY_ASSERT(this->trees_.size() == this->node_height_indices_.size());
}




ComparisonDirichletPopulationTreeCollection::ComparisonDirichletPopulationTreeCollection(
        const DirichletCollectionSettings & settings,
        RandomNumberGenerator & rng,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        bool strict_on_triallelic_sites,
        bool store_seq_loci_info
        ) : BaseComparisonPopulationTreeCollection() {
    this->state_log_path_ = settings.get_state_log_path();
    this->operator_log_path_ = settings.get_operator_log_path();
    this->node_height_prior_ = settings.get_time_prior_settings().get_instance();
    this->concentration_ = std::make_shared<PositiveRealParameter>(
            settings.get_concentration_settings(),
            rng);
    this->discount_ = std::make_shared<DiscountParameter>(
            settings.get_discount_settings(),
            rng);
    this->operator_schedule_ = OperatorSchedule(
            settings);
    if (this->operator_schedule_.get_total_weight() <= 0.0) {
        throw EcoevolityError("No operators have weight");
    }
    this->init_trees(settings.get_comparison_settings(),
            rng,
            strict_on_constant_sites,
            strict_on_missing_sites,
            strict_on_triallelic_sites,
            store_seq_loci_info);
    this->stored_node_heights_.reserve(this->trees_.size());
    this->stored_node_height_indices_.reserve(this->trees_.size());
    if (settings.event_model_is_fixed()) {
        this->set_node_height_indices(settings.get_fixed_event_model_indices(), rng);
    }
    this->model_prior_ = settings.get_model_prior();
}

void ComparisonDirichletPopulationTreeCollection::init_trees(
        const std::vector<DirichletComparisonSettings> & comparison_settings,
        RandomNumberGenerator & rng,
        bool strict_on_constant_sites,
        bool strict_on_missing_sites,
        bool strict_on_triallelic_sites,
        bool store_seq_loci_info
        ) {
    std::unordered_set<std::string> population_labels;
    double fresh_height;
    for (unsigned int tree_idx = 0;
            tree_idx < comparison_settings.size();
            ++tree_idx) {
        fresh_height = this->node_height_prior_->draw(rng);
        std::shared_ptr<PositiveRealParameter> new_height_parameter = std::make_shared<PositiveRealParameter>(this->node_height_prior_, fresh_height);
        std::shared_ptr<PopulationTree> new_tree = std::make_shared<ComparisonDirichletPopulationTree>(
                comparison_settings.at(tree_idx),
                rng,
                strict_on_constant_sites,
                strict_on_missing_sites,
                strict_on_triallelic_sites,
                store_seq_loci_info
                );
        for (auto const& pop_label: new_tree->get_population_labels()) {
            auto p = population_labels.insert(pop_label);
            if (! p.second) {
                std::ostringstream message;
                message << "\n#######################################################################\n"
                        <<   "###############################  ERROR  ###############################\n"
                        << "Population label conflict. The population label:\n"
                        << "\'" << pop_label << "\'\n"
                        << "is used in multiple alignments.\n"
                        << "#######################################################################\n";
                throw EcoevolityCollectionSettingError(message.str());
            }
        }
        new_tree->set_node_height_prior(this->node_height_prior_);
        new_tree->set_root_height_parameter(new_height_parameter);
        this->node_heights_.push_back(new_height_parameter);
        this->trees_.push_back(new_tree);
        this->node_height_indices_.push_back(tree_idx);
        ECOEVOLITY_ASSERT(
                this->trees_.at(tree_idx)->get_root_height_parameter() ==
                this->node_heights_.at(this->node_height_indices_.at(tree_idx))
                );
    }
    ECOEVOLITY_ASSERT(this->trees_.size() == this->node_heights_.size());
    ECOEVOLITY_ASSERT(this->trees_.size() == this->node_height_indices_.size());
}
