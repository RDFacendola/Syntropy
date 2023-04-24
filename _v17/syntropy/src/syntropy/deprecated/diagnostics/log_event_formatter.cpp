#include "syntropy/diagnostics/log_event_formatter.h"

namespace Syntropy
{
    /************************************************************************/
    /* LOG EVENT FORMATTER                                                  */
    /************************************************************************/

    void LogEventFormatter::Parse()
    {
        tokens_.clear();

        auto translated_token = StringView{};

        for (auto format_it = format_.begin(); format_it != format_.end(); format_it += translated_token.size())
        {
            auto format_view = Strings::MakeView(format_it, format_.end());

            // Find if a translation rule can be applied to the prefix of the current string.

            auto translation_rule = &kPercentageToken;

            for (; translation_rule; translation_rule = translation_rule->next_)
            {
                if (Strings::IsPrefix(translation_rule->token_, format_view))
                {
                    translated_token = StringView{ &(*format_it), translation_rule->token_.size() };

                    break;
                }
            }

            // If no translation rule could be applied, output an unformatted string up to the next potential token.

            if (!translation_rule)
            {
                translation_rule = &kUnformattedToken;

                if (auto length = format_view.find_first_of('%', 1); length != StringView::npos)
                {
                    translated_token = StringView{ &(*format_it), length };
                }
                else
                {
                    translated_token = format_view;
                }
            }

            tokens_.emplace_back(Token{ translated_token, translation_rule });
        }
    }
}