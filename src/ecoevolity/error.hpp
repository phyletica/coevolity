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

#ifndef ECOEVOLITY_ERROR_HPP
#define ECOEVOLITY_ERROR_HPP

#include <iostream>
#include <sstream>


/**
 * Base class for errors.
 */
class EcoevolityBaseError: public std::exception {
    public:
        mutable std::string error_report_;

        virtual ~EcoevolityBaseError() throw() { }

        EcoevolityBaseError(
                const std::string & name,
                const std::string & message);
        EcoevolityBaseError(
                const std::string & name,
                const std::string & message,
                const std::string & file_name);
        EcoevolityBaseError(
                const std::string & name,
                const std::string & message,
                unsigned int line_number);
        EcoevolityBaseError(
                const std::string & name,
                const std::string & message,
                const std::string & file_name,
                unsigned int line_number);
        EcoevolityBaseError(
                const std::string & name,
                const std::string & message,
                const std::string & file_name,
                const std::string & taxon_label,
                unsigned int character_index);

        const char * what() const throw() {
            return this->error_report_.empty() ? "EcoevolityError: no message" : this->error_report_.c_str();
        }
};

class EcoevolityError: public EcoevolityBaseError {
    public:
        EcoevolityError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityError", message) { }
};

class EcoevolityNumericLimitError: public EcoevolityBaseError {
    public:
        EcoevolityNumericLimitError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityNumericLimitError", message) { }
};

class EcoevolityNullPointerError: public EcoevolityBaseError {
    public:
        EcoevolityNullPointerError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityNullPointerError", message) { }
};

class EcoevolityBiallelicDataError: public EcoevolityBaseError {
    public:
        EcoevolityBiallelicDataError(
                const std::string & message,
                const std::string & file_name) :
            EcoevolityBaseError("EcoevolityBiallelicDataError", message,
                    file_name) { }
};

class EcoevolityConstantSitesError: public EcoevolityBaseError {
    public:
        EcoevolityConstantSitesError(
                const std::string & message,
                const std::string & file_name) :
            EcoevolityBaseError("EcoevolityConstantSitesError", message,
                    file_name) { }
};

class EcoevolityMissingDataError: public EcoevolityBaseError {
    public:
        EcoevolityMissingDataError(
                const std::string & message,
                const std::string & file_name) :
            EcoevolityBaseError("EcoevolityMissingDataError", message,
                    file_name) { }
};

class EcoevolityTriallelicDataError: public EcoevolityBaseError {
    public:
        EcoevolityTriallelicDataError(
                const std::string & message,
                const std::string & file_name) :
            EcoevolityBaseError("EcoevolityTriallelicDataError", message,
                    file_name) { }
};

class EcoevolityParsingError: public EcoevolityBaseError {
    public:
        EcoevolityParsingError(
                const std::string & message,
                const std::string & file_name,
                unsigned int line_number) :
            EcoevolityBaseError("EcoevolityParsingError", message, file_name,
                    line_number) { }

        EcoevolityParsingError(
                const std::string & message,
                unsigned int line_number) :
            EcoevolityBaseError("EcoevolityParsingError", message,
                    line_number) { }

        EcoevolityParsingError(
                const std::string & message,
                const std::string & file_name) :
            EcoevolityBaseError("EcoevolityParsingError", message,
                    file_name) { }
};

class EcoevolityInvalidCharacterError: public EcoevolityBaseError {
    public:
        EcoevolityInvalidCharacterError(
                const std::string & message,
                const std::string & file_name,
                const std::string & taxon_label,
                unsigned int character_index) :
            EcoevolityBaseError("EcoevolityInvalidCharacterError", message,
                    file_name, taxon_label, character_index) { }
};

class EcoevolityParameterValueError: public EcoevolityBaseError {
    public:
        EcoevolityParameterValueError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityParameterValueError", message) { }
};

class EcoevolityProbabilityDistributionError: public EcoevolityBaseError {
    public:
        EcoevolityProbabilityDistributionError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityProbabilityDistributionError", message) { }
};

class EcoevolityContinuousDistributionSettingError: public EcoevolityBaseError {
    public:
        EcoevolityContinuousDistributionSettingError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityContinuousDistributionSettingError", message) { }
};

class EcoevolityPositiveRealParameterSettingError: public EcoevolityBaseError {
    public:
        EcoevolityPositiveRealParameterSettingError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityPositiveRealParameterSettingError", message) { }
};

class EcoevolityComparisonSettingError: public EcoevolityBaseError {
    public:
        EcoevolityComparisonSettingError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityComparisonSettingError", message) { }

        EcoevolityComparisonSettingError(
                const std::string & message,
                const std::string & file_name) :
            EcoevolityBaseError("EcoevolityComparisonSettingError", message, file_name) { }
};

class EcoevolityCollectionSettingError: public EcoevolityBaseError {
    public:
        EcoevolityCollectionSettingError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityCollectionSettingError", message) { }
};

class EcoevolityYamlConfigError: public EcoevolityBaseError {
    public:
        EcoevolityYamlConfigError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityYamlConfigError", message) { }
        EcoevolityYamlConfigError(
                const std::string & message,
                const std::string & file_path) :
            EcoevolityBaseError("EcoevolityYamlConfigError", message, file_path) { }
};

class EcoevolityYamlDataError: public EcoevolityBaseError {
    public:
        EcoevolityYamlDataError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityYamlDataError", message) { }
        EcoevolityYamlDataError(
                const std::string & message,
                const std::string & file_path) :
            EcoevolityBaseError("EcoevolityYamlDataError", message, file_path) { }
};

class EcoevolitySpreadsheetError: public EcoevolityBaseError {
    public:
        EcoevolitySpreadsheetError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolitySpreadsheetError", message) { }
};

class EcoevolityNotImplementedError: public EcoevolityBaseError {
    public:
        EcoevolityNotImplementedError(
                const std::string & message) :
            EcoevolityBaseError("EcoevolityNotImplementedError", message) { }
};

#endif
