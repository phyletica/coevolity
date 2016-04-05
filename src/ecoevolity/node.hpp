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

#ifndef ECOEVOLITY_NODE_HPP
#define ECOEVOLITY_NODE_HPP

#include <memory>

#include "basenode.hpp"
#include "parameter.hpp"

/**
 * Base class for a node of a phylogenetic tree.
 *
 * @note    Many of the class' methods modified from:
 *              BasicTNode class of Bio++ Library
 *              <http://biopp.univ-montp2.fr/wiki/index.php/Main_Page>
 *              License:    CeCILL <http://www.cecill.info>
 *              Author:     Sylvain Gaillard
 *              Copyright:  CNRS, (January 12, 2011)
 */
class Node: public BaseNode<Node>{
    private:
        typedef BaseNode<Node> BaseClass;

    public:
        Node() { }
        Node(const Node& node) : BaseClass(node) { }
        Node(std::string label) : BaseClass(label) { }
        Node(double height) : BaseClass(height) { }
        Node(std::string label, double height) : BaseClass(label, height) { }
};

class PopulationNode: public BaseNode<PopulationNode>{
    private:
        typedef BaseNode<PopulationNode> BaseClass;
        BiallelicPatternProbabilityMatrix bottom_pattern_probs_;
        BiallelicPatternProbabilityMatrix top_pattern_probs_;
        std::shared_ptr<CoalescenceRateParameter> coalescence_rate_ = std::make_shared<CoalescenceRateParameter>(10.0);

        void add_ln_relative_coalescence_rate_prior_density(
                double& density,
                std::vector< std::shared_ptr<CoalescenceRateParameter> >& parameters) const {
            bool parameter_found = false;
            for (auto parameter_iter : parameters) {
                if (parameter_iter == this->coalescence_rate_) {
                    parameter_found = true;
                    break;
                }
            }
            if (! parameter_found) {
                density += this->coalescence_rate_->relative_prior_ln_pdf();
                parameters.push_back(this->coalescence_rate_);
            }
            for (auto child_iter: this->children_) {
                child_iter->add_ln_relative_coalescence_rate_prior_density(density, parameters);
            }
        }

    public:
        PopulationNode() { }
        PopulationNode(const Node& node) : BaseClass(node) { }
        PopulationNode(const PopulationNode& node) : BaseClass(node) {
            this->bottom_pattern_probs_ = node.bottom_pattern_probs_;
            this->top_pattern_probs_ = node.top_pattern_probs_;
        }
        PopulationNode(std::string label) : BaseClass(label) { }
        PopulationNode(double height) : BaseClass(height) { }
        PopulationNode(std::string label, double height) :
            BaseClass(label, height)
            { }
        PopulationNode(unsigned int allele_count) : BaseClass() {
            this->bottom_pattern_probs_.resize(allele_count);
            this->top_pattern_probs_.resize(allele_count);
        }
        PopulationNode(std::string label, unsigned int allele_count) :
            BaseClass(label)
        {
            this->bottom_pattern_probs_.resize(allele_count);
            this->top_pattern_probs_.resize(allele_count);
        }
        PopulationNode(double height, unsigned int allele_count) :
            BaseClass(height)
        {
            this->bottom_pattern_probs_.resize(allele_count);
            this->top_pattern_probs_.resize(allele_count);
        }
        PopulationNode(std::string label,
                double height,
                unsigned int allele_count) :
            BaseClass(label, height)
        {
            this->bottom_pattern_probs_.resize(allele_count);
            this->top_pattern_probs_.resize(allele_count);
        }

        // overload copy operator
        PopulationNode& operator=(const PopulationNode& node) {
            this->children_ = node.children_;
            this->parent_ = node.parent_;
            this->height_->set_value(node.height_->get_value());
            this->label_ = node.label_;
            this->is_dirty_ = node.is_dirty_;
            this->bottom_pattern_probs_ = node.bottom_pattern_probs_;
            this->top_pattern_probs_ = node.top_pattern_probs_;
            return * this;
        }

        // methods for accessing/changing pattern probabilities
        unsigned int get_allele_count() const {
            return this->bottom_pattern_probs_.get_allele_count();
        }

        unsigned int get_leaf_allele_count() const {
            if (this->is_leaf()) {
                return this->get_allele_count();
            }
            unsigned int n = 0;
            for (auto child_iter: this->children_) {
                n += child_iter->get_leaf_allele_count();
            }
            return n;
        }

        void resize(unsigned int allele_count) {
            this->bottom_pattern_probs_.resize(allele_count);
            this->top_pattern_probs_.resize(allele_count);
        }
        void reset(unsigned int allele_count) {
            this->bottom_pattern_probs_.reset(allele_count);
            this->top_pattern_probs_.reset(allele_count);
        }
        void resize_all() {
            this->resize(this->get_leaf_allele_count());
            for (auto child_iter: this->children_) {
                child_iter->resize_all();
            }
        }

        const BiallelicPatternProbabilityMatrix& get_bottom_pattern_probs() const{
            return this->bottom_pattern_probs_;
        }
        const BiallelicPatternProbabilityMatrix& get_top_pattern_probs() const{
            return this->top_pattern_probs_;
        }
        BiallelicPatternProbabilityMatrix* clone_bottom_pattern_probs() const{
            return this->bottom_pattern_probs_.clone();
        }
        BiallelicPatternProbabilityMatrix* clone_top_pattern_probs() const{
            return this->top_pattern_probs_.clone();
        }

        void copy_bottom_pattern_probs(const BiallelicPatternProbabilityMatrix& m) {
            // No check here; bottom probs will be updated first and can differ
            // in size until the top is also updated.
            // if (m.get_allele_count() != this->top_pattern_probs_.get_allele_count()) {
            //     throw EcoevolityError(
            //             "PopulationNode:copy_bottom_pattern_probs(); allele counts must be the same between top and bottom of branch");
            // }
            this->bottom_pattern_probs_.copy(m);
        }
        void copy_top_pattern_probs(const BiallelicPatternProbabilityMatrix& m) {
            if (m.get_allele_count() != this->bottom_pattern_probs_.get_allele_count()) {
                throw EcoevolityError(
                        "PopulationNode:copy_top_pattern_probs(); allele counts must be the same between top and bottom of branch");
            }
            this->top_pattern_probs_.copy(m);
        }
        void copy_pattern_probs(
                const BiallelicPatternProbabilityMatrix& bottom_probs,
                const BiallelicPatternProbabilityMatrix& top_probs) {
            if (bottom_probs.get_allele_count() != top_probs.get_allele_count()) {
                throw EcoevolityError(
                        "PopulationNode:copy_pattern_probs(); allele counts must be the same between top and bottom of branch");
            }
            this->bottom_pattern_probs_.copy(bottom_probs);
            this->top_pattern_probs_.copy(top_probs);
        }

        const double& get_bottom_pattern_probability(
                unsigned int allele_count,
                unsigned int red_allele_count) const {
            return this->bottom_pattern_probs_.get_pattern_probability(
                    allele_count,
                    red_allele_count);
        }
        const double& get_top_pattern_probability(
                unsigned int allele_count,
                unsigned int red_allele_count) const {
            return this->top_pattern_probs_.get_pattern_probability(
                    allele_count,
                    red_allele_count);
        }
        void set_bottom_pattern_probability(
                unsigned int allele_count,
                unsigned int red_allele_count,
                double probability) {
            this->bottom_pattern_probs_.set_pattern_probability(
                    allele_count,
                    red_allele_count,
                    probability);
        }
        void set_top_pattern_probability(
                unsigned int allele_count,
                unsigned int red_allele_count,
                double probability) {
            this->top_pattern_probs_.set_pattern_probability(
                    allele_count,
                    red_allele_count,
                    probability);
        }

        const double& get_coalescence_rate() const {
            return this->coalescence_rate_->get_value();
        }
        std::shared_ptr<CoalescenceRateParameter> get_coalescence_rate_parameter() const {
            return this->coalescence_rate_;
        }
        void set_coalescence_rate_parameter(std::shared_ptr<CoalescenceRateParameter> rate) {
            this->coalescence_rate_ = rate;
            this->make_all_dirty();
        }
        void set_all_coalescence_rate_parameters(std::shared_ptr<CoalescenceRateParameter> rate) {
            this->coalescence_rate_ = rate;
            this->make_dirty();
            for (auto child_iter: this->children_) {
                child_iter->set_all_coalescence_rate_parameters(rate);
            }
        }
        void set_all_coalescence_rate_parameters() {
            std::shared_ptr<CoalescenceRateParameter> rate = this->coalescence_rate_;
            this->make_dirty();
            for (auto child_iter: this->children_) {
                child_iter->set_all_coalescence_rate_parameters(rate);
            }
        }

        void set_coalescence_rate(double rate) {
            this->coalescence_rate_->set_value(rate);
            this->make_all_dirty();
        }
        void set_all_coalescence_rates(double rate) {
            this->coalescence_rate_->set_value(rate);
            this->make_dirty();
            for (auto child_iter: this->children_) {
                child_iter->set_all_coalescence_rates(rate);
            }
        }
        void update_coalescence_rate(double rate) {
            this->coalescence_rate_->update_value(rate);
            this->make_all_dirty();
        }
        void update_all_coalescence_rates(double rate) {
            this->coalescence_rate_->update_value(rate);
            this->make_dirty();
            for (auto child_iter: this->children_) {
                child_iter->update_all_coalescence_rates(rate);
            }
        }

        void store_coalescence_rate() {
            this->coalescence_rate_->store();
        }
        void restore_coalescence_rate() {
            this->coalescence_rate_->restore();
            this->make_all_dirty();
        }
        void store_all_coalescence_rates() {
            this->coalescence_rate_->store();
            for (auto child_iter: this->children_) {
                child_iter->store_all_coalescence_rates();
            }
        }
        void restore_all_coalescence_rates() {
            this->coalescence_rate_->restore();
            this->make_dirty();
            for (auto child_iter: this->children_) {
                child_iter->restore_all_coalescence_rates();
            }
        }

        void set_population_size_prior(std::shared_ptr<ContinuousProbabilityDistribution> prior) {
            this->coalescence_rate_->set_prior(prior);
            this->make_all_dirty();
        }
        void set_all_population_size_priors(std::shared_ptr<ContinuousProbabilityDistribution> prior) {
            this->coalescence_rate_->set_prior(prior);
            this->make_dirty();
            for (auto child_iter: this->children_) {
                child_iter->set_all_population_size_priors(prior);
            }
        }

        void fix_coalescence_rate() {
            this->coalescence_rate_->fix();
        }
        void fix_all_coalescence_rates() {
            this->coalescence_rate_->fix();
            for (auto child_iter: this->children_) {
                child_iter->fix_all_coalescence_rates();
            }
        }
        void estimate_coalescence_rate() {
            this->coalescence_rate_->estimate();
        }
        void estimate_all_coalescence_rates() {
            this->coalescence_rate_->estimate();
            for (auto child_iter: this->children_) {
                child_iter->estimate_all_coalescence_rates();
            }
        }

        double calculate_ln_relative_coalescence_rate_prior_density() const {
            double d = 0.0;
            std::vector< std::shared_ptr<CoalescenceRateParameter> > parameters;
            parameters.reserve(this->get_node_count());
            this->add_ln_relative_coalescence_rate_prior_density(d, parameters);
            return d;
        }

        bool coalescence_rate_is_fixed() const {
            return this->coalescence_rate_->is_fixed();
        }

        bool all_coalescence_rates_are_fixed() const {
            if (! this->coalescence_rate_is_fixed()) {
                return false;
            }
            for (auto child_iter: this->children_) {
                if (! child_iter->all_coalescence_rates_are_fixed()) {
                    return false;
                }
            }
            return true;
        }

};

#endif
