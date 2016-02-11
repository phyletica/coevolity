#include "data.hpp"

BiallelicData::BiallelicData(
        const std::string path, 
        const char population_name_delimiter,
        const bool population_name_is_prefix,
        const bool markers_are_dominant,
        const bool validate) {
    char pop_name_delimiter = population_name_delimiter;
    if (population_name_delimiter == '_') {
        pop_name_delimiter = ' ';
    }
    this->markers_are_dominant_ = markers_are_dominant;

    MultiFormatReader nexus_reader(-1, NxsReader::WARNINGS_TO_STDERR);
    nexus_reader.ReadFilepath(path.c_str(), MultiFormatReader::NEXUS_FORMAT);
    
    unsigned int num_taxa_blocks = nexus_reader.GetNumTaxaBlocks();
    if (num_taxa_blocks < 1) {
        throw EcoevolityParsingError("No taxa block found", path, 0);
    }
    if (num_taxa_blocks > 1) {
        throw EcoevolityParsingError("More than one taxa block found", path, 0);
    }

    NxsTaxaBlock * taxa_block = nexus_reader.GetTaxaBlock(0);
    std::string taxa_block_title = taxa_block->GetTitle();
    const unsigned int num_char_blocks = nexus_reader.GetNumCharactersBlocks(taxa_block);
    if (num_char_blocks < 1) {
        throw EcoevolityParsingError("No character block found", path, 0);
    }
    if (num_char_blocks > 1) {
        throw EcoevolityParsingError("More than one character block found", path, 0);
    }

    NxsCharactersBlock * char_block = nexus_reader.GetCharactersBlock(taxa_block, 0);
    std::string char_block_title = char_block->GetTitle();

    unsigned int num_chars = char_block->GetNCharTotal();
    unsigned int num_taxa = char_block->GetNTaxTotal();
    NxsCharactersBlock::DataTypesEnum data_type;
    data_type = char_block->GetDataType();

    ECOEVOLITY_DEBUG(
    std::cerr << "Char block " << char_block_title << " has data type: " <<
            (int)data_type << std::endl;
    )

    for (unsigned int taxon_idx = 0; taxon_idx < num_taxa; ++taxon_idx) {
        NxsString seq_label = char_block->GetTaxonLabel(taxon_idx);
        std::vector<std::string> seq_label_elements = split(seq_label, pop_name_delimiter);
        ECOEVOLITY_ASSERT(! seq_label_elements.empty());
        std::string pop_label = seq_label_elements.front();
        if (! population_name_is_prefix) {
            pop_label = seq_label_elements.back();
        }
        bool pop_label_found = false;
        for (unsigned int pop_label_idx = 0; pop_label_idx < this->population_labels_.size(); pop_label_idx++) {
            if (this->population_labels_[pop_label_idx] == pop_label) {
                assert(! this->sequence_labels_[pop_label_idx].empty());
                this->sequence_labels_[pop_label_idx].push_back(seq_label);
                pop_label_found = true;
            }
        }
        if (! pop_label_found) {
            this->population_labels_.push_back(pop_label);
            this->pop_label_to_index_map_[pop_label] = this->population_labels_.size() - 1;
            std::vector<std::string> tmp_label_vector = {seq_label};
            this->sequence_labels_.push_back(tmp_label_vector);
        }
        this->seq_label_to_pop_label_map_[seq_label] = pop_label;
    }

    ECOEVOLITY_DEBUG(
    std::cerr << "this->populations_labels_:" << std::endl;
    unsigned int pop_idx = 0;
    for (auto p_label: this->population_labels_) {
        std::cerr << p_label << std::endl;
        for (auto s_label: this->sequence_labels_[pop_idx]) {
            std::cerr << "\t" << s_label << std::endl;
        }
        pop_idx += 1;
    }
    )

    std::vector<const NxsDiscreteDatatypeMapper *> data_type_mappers = char_block->GetAllDatatypeMappers();
    if (data_type_mappers.size() < 1) {
        throw EcoevolityParsingError("No character encoding found", path, 0);
    }
    if (data_type_mappers.size() > 1) {
        throw EcoevolityParsingError("More than one character encoding (i.e., mixed data types) found", path, 0);
    }
    const NxsDiscreteDatatypeMapper * data_type_mapper = data_type_mappers[0];

    if (data_type == NxsCharactersBlock::DataTypesEnum::standard) {
        const NxsDiscreteStateCell highest_state_code = data_type_mapper->GetHighestStateCode();
        if (highest_state_code == 1) {
            this->genotypes_are_diploid_ = false;
        }
        else if (highest_state_code == 2) {
            this->genotypes_are_diploid_ = true;
        }
        else {
            throw EcoevolityParsingError("More than 3 character state codes found", path, 0);
        }

        for (unsigned int site_idx = 0; site_idx < num_chars; ++site_idx) {
            std::vector<unsigned int> allele_cts (this->get_number_of_populations(), 0);
            std::vector<unsigned int> red_allele_cts (this->get_number_of_populations(), 0);
            for (unsigned int taxon_idx = 0; taxon_idx < num_taxa; ++taxon_idx) {
                const NxsDiscreteStateCell state_code = char_block->GetInternalRepresentation(taxon_idx, site_idx);
                if (state_code >= 0) {
                    const unsigned int num_states = char_block->GetNumStates(taxon_idx, site_idx);
                    if (num_states > 1) {
                        throw EcoevolityInvalidCharacterError(
                                "Invalid polymorphic character",
                                path,
                                char_block->GetTaxonLabel(taxon_idx),
                                site_idx);
                    }
                    const unsigned int& population_idx = this->get_population_index_from_seq_label(char_block->GetTaxonLabel(taxon_idx));
                    if (this->markers_are_dominant_) {
                        if (this->genotypes_are_diploid_) {
                            if (state_code == 1) {
                                throw EcoevolityInvalidCharacterError(
                                        "Invalid het character for dominant data",
                                        path,
                                        char_block->GetTaxonLabel(taxon_idx),
                                        site_idx);
                            }
                            red_allele_cts[population_idx] += state_code;
                            allele_cts[population_idx] += 2;
                        }
                        else {
                            red_allele_cts[population_idx] += 2*state_code;
                            allele_cts[population_idx] += 2;
                        }

                    }
                    else { // markers are not dominant
                        red_allele_cts[population_idx] += state_code;
                        if (this->genotypes_are_diploid_) {
                            allele_cts[population_idx] += 2;
                        }
                        else {
                            allele_cts[population_idx] += 1;
                        }
                    }
                }
            }
            int pattern_index = this->get_pattern_index(red_allele_cts, allele_cts);
            if (pattern_index < 0) {
                this->red_allele_counts_.push_back(red_allele_cts);
                this->allele_counts_.push_back(allele_cts);
                this->pattern_weights_.push_back(1);
            }
            else {
                this->pattern_weights_[pattern_index] += 1;
            }
        }
    }
    nexus_reader.DeleteBlocksFromFactories();
    this->update_pattern_booleans();
    if (validate) {
        this->validate();
    }
}

const std::vector<unsigned int>& BiallelicData::get_red_allele_counts(unsigned int pattern_index) const {
    return this->red_allele_counts_.at(pattern_index);
}

const std::vector<unsigned int>& BiallelicData::get_allele_counts(unsigned int pattern_index) const {
    return this->allele_counts_.at(pattern_index);
}

const unsigned int& BiallelicData::get_pattern_weight(unsigned int pattern_index) const {
    return this->pattern_weights_.at(pattern_index);
}

const unsigned int& BiallelicData::get_population_index(std::string population_label) const {
    return this->pop_label_to_index_map_.at(population_label);
}

const unsigned int& BiallelicData::get_population_index_from_seq_label(std::string seq_label) const {
    const std::string pop_label = this->seq_label_to_pop_label_map_.at(seq_label);
    return this->get_population_index(pop_label);
}

const std::string& BiallelicData::get_population_label(unsigned int population_index) const {
    return this->population_labels_.at(population_index);
}

const std::vector<std::string>& BiallelicData::get_sequence_labels(unsigned int population_index) const {
    return this->sequence_labels_.at(population_index);
}

unsigned int BiallelicData::get_number_of_patterns() const {
    return this->pattern_weights_.size();
}

unsigned int BiallelicData::get_number_of_populations() const {
    return this->population_labels_.size();
}

const bool& BiallelicData::markers_are_dominant() const {
    return this->markers_are_dominant_;
}

const bool& BiallelicData::genotypes_are_diploid() const {
    return this->genotypes_are_diploid_;
}

const bool& BiallelicData::has_constant_patterns() const {
    return this->has_constant_patterns_;
}

const bool& BiallelicData::has_missing_population_patterns() const {
    return this->has_missing_population_patterns_;
}

void BiallelicData::update_has_constant_patterns() {
    this->has_constant_patterns_ = false;
    std::vector<unsigned int> no_red_pattern (this->get_number_of_populations(), 0);
    for (unsigned int pattern_idx = 0; pattern_idx < this->get_number_of_patterns(); ++pattern_idx) {
        if ((this->get_red_allele_counts(pattern_idx) == no_red_pattern) ||
            (this->get_red_allele_counts(pattern_idx) == this->get_allele_counts(pattern_idx))) {
            this->has_constant_patterns_ = true;
            return;
        }
    }
    return;
}

void BiallelicData::update_has_missing_population_patterns() {
    this->has_missing_population_patterns_ = false;
    for (unsigned int pattern_idx = 0; pattern_idx < this->get_number_of_patterns(); ++pattern_idx) {
        for (auto count_iter: this->get_allele_counts(pattern_idx)) {
            if (count_iter == 0) {
                this->has_missing_population_patterns_ = true;
                return;
            }
        }
    }
    return;
}

void BiallelicData::update_pattern_booleans() {
    this->update_has_constant_patterns();
    this->update_has_missing_population_patterns();
}

int BiallelicData::get_pattern_index(
        std::vector<unsigned int> red_allele_counts,
        std::vector<unsigned int> allele_counts) const {
    ECOEVOLITY_ASSERT(this->allele_counts_.size() == this->red_allele_counts_.size());
    for (unsigned int pattern_idx = 0; pattern_idx < this->allele_counts_.size(); ++pattern_idx) {
        if ((this->red_allele_counts_[pattern_idx] == red_allele_counts) &&
            (this->allele_counts_[pattern_idx] == allele_counts)) {
            return pattern_idx;
        }
    }
    return -1;
}

void BiallelicData::remove_pattern(unsigned int pattern_index) {
    this->pattern_weights_.erase(this->pattern_weights_.begin() + pattern_index);
    this->allele_counts_.erase(this->allele_counts_.begin() + pattern_index);
    this->red_allele_counts_.erase(this->red_allele_counts_.begin() + pattern_index);
    return;
}

unsigned int BiallelicData::remove_constant_patterns(const bool validate) {
    unsigned int number_removed = 0;
    std::vector<unsigned int> no_red_pattern (this->get_number_of_populations(), 0);
    for (unsigned int pattern_idx = 0; pattern_idx < this->get_number_of_patterns(); ++pattern_idx) {
        if ((this->get_red_allele_counts(pattern_idx) == no_red_pattern) ||
            (this->get_red_allele_counts(pattern_idx) == this->get_allele_counts(pattern_idx))) {
            this->remove_pattern(pattern_idx);
            number_removed += 1;
        }
    }
    this->update_pattern_booleans();
    if (validate) {
        this->validate();
    }
    return number_removed;
}

unsigned int BiallelicData::remove_patterns_with_missing_taxa(const bool validate) {
    unsigned int number_removed = 0;
    for (unsigned int pattern_idx = 0; pattern_idx < this->get_number_of_patterns(); ++pattern_idx) {
        for (auto count_iter: this->get_allele_counts(pattern_idx)) {
            if (count_iter == 0) {
                this->remove_pattern(pattern_idx);
                number_removed += 1;
            }
        }
    }
    this->update_pattern_booleans();
    if (validate) {
        this->validate();
    }
    return number_removed;
}

void BiallelicData::validate() const {
    if (this->allele_counts_.size() != this->pattern_weights_.size()) {
        throw EcoevolityBiallelicDataError(
                "Different number of allele counts and weights");
    }
    if (this->allele_counts_.size() != this->red_allele_counts_.size()) {
        throw EcoevolityBiallelicDataError(
                "Different number of allele and red allele counts");
    }
    if (this->allele_counts_.size() < 1) {
        throw EcoevolityBiallelicDataError(
                "No data found");
    }
    std::vector<unsigned int> no_red_pattern (this->get_number_of_populations(), 0);
    unsigned int number_of_pops = this->population_labels_.size();
    bool has_constant = false;
    bool has_missing = false;
    for (unsigned int pattern_idx = 0; pattern_idx < this->get_number_of_patterns(); ++pattern_idx) {
        if (this->allele_counts_.at(pattern_idx).size() != number_of_pops) {
            throw EcoevolityBiallelicDataError(
                    "Different number of populations and allele counts per pattern");
        }
        if (this->red_allele_counts_.at(pattern_idx).size() != number_of_pops) {
            throw EcoevolityBiallelicDataError(
                    "Different number of populations and red allele counts per pattern");
        }
        if ((this->red_allele_counts_.at(pattern_idx) == no_red_pattern) ||
            (this->red_allele_counts_.at(pattern_idx) == this->allele_counts_.at(pattern_idx))) {
            has_constant = true;
        }
        for (unsigned int pop_idx = 0; pop_idx < number_of_pops; ++pop_idx) {
            if (this->red_allele_counts_.at(pattern_idx).at(pop_idx) > this->allele_counts_.at(pattern_idx).at(pop_idx)) {
                throw EcoevolityBiallelicDataError(
                    "Some patterns have more red alleles than total allele counts");
            }
            if (this->allele_counts_.at(pattern_idx).at(pop_idx) == 0) {
                has_missing = true;
            }
        }
    }
    if (has_constant != this->has_constant_patterns()) {
        throw EcoevolityBiallelicDataError(
                "constant pattern boolean is incorrect");
    }
    if (has_missing != this->has_missing_population_patterns()) {
        throw EcoevolityBiallelicDataError(
                "missing data pattern boolean is incorrect");
    }
    return;
}
