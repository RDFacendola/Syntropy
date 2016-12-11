
/// \file base_io.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "algorithm.h"

namespace syntropy
{

    template <typename TElement, typename TTranslator>
    class Formatter
    {
    public:

        Formatter(const std::string& format, const TTranslator& translator, char token_open, char token_closed);

        void operator()(std::ostream& out, const TElement& element) const;

    private:

        std::vector<typename TTranslator::TThunk> steps_;                    ///< \brief Steps to apply to the element to get the formatted output

    };

    template <typename TValue, typename TElement>
    struct ConstValueInserter
    {
        ConstValueInserter(TValue&& value);

        ConstValueInserter(const TValue& value);

        void operator()(std::ostream& stream, const TElement&) const;

        TValue value_;
    };

}


namespace syntropy
{
    // Implementation

    //////////////// FORMATTER ////////////////

    template <typename TElement, typename TTranslator>
    Formatter<TElement, TTranslator>::Formatter(const std::string& format, const TTranslator& translator, char token_open, char token_closed)
    {
        auto it = format.begin();

        while (it != format.end())
        {
            auto token = GetToken(it, format.end(), token_open, token_closed);

            // Add a constant string
            if (it != token.first)
            {
                steps_.emplace_back(ConstValueInserter<std::string, TElement>(std::string(it, token.first)));
            }

            // Add a new token manipulator
            if (token.first != token.second)
            {
                steps_.emplace_back(translator(std::string(token.first, token.second)));
            }

            // Next token
            it = token.second;
        }
    }

    template <typename TElement, typename TTokenSolver>
    void Formatter<TElement, TTokenSolver>::operator()(std::ostream& stream, const TElement& element) const
    {
        for (auto&& step : steps_)
        {
            step(stream, element);
        }
    }

    //////////////// CONST VALUE INSERTER ////////////////

    template <typename TValue, typename TElement>
    ConstValueInserter<TValue, TElement>::ConstValueInserter(TValue&& value)
        : value_(std::move(value))
    {

    }

    template <typename TValue, typename TElement>
    ConstValueInserter<TValue, TElement>::ConstValueInserter(const TValue& value)
        : value_(value)
    {

    }

    template <typename TValue, typename TElement>
    void ConstValueInserter<TValue, TElement>::operator()(std::ostream& out, const TElement&) const
    {
        out << value_;
    }

}