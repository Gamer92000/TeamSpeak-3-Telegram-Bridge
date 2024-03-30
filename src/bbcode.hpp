#pragma once

#include <string>
#include <regex>

static void findAndReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

static std::string telegramEscape(const char *message) {
	std::string msg = std::string(message);
	// removed from escape: [, ], `, _, *
	auto escapeChars = { '(', ')', '~', '>', '#', '+', '-', '=', '|', '{', '}', '.', '!', '_' };
	for (auto c : escapeChars) {
		findAndReplaceAll(msg, std::string(1, c), std::string(1, '\\') + c);
	}
	return msg;
}

static std::string parseBbcode(const char *message)
{
	// first escape all characters that have a special meaning in markdown
	std::string msg = telegramEscape(message);

	// then replace all representable BBCodes with their markdown equivalent
	const std::vector<std::tuple<std::string, std::string>> replacementMap = {
			{"[b]", "*"},
			{"[B]", "*"},
			{"[/b]", "*"},
			{"[/B]", "*"},
			{"[i]", "_"},
			{"[I]", "_"},
			{"[/i]", "_"},
			{"[/I]", "_"},
			{"[u]", "__"},
			{"[U]", "__"},
			{"[/u]", "__"},
			{"[/U]", "__"},
			{"[s]", "~~"},
			{"[S]", "~~"},
			{"[/s]", "~~"},
			{"[/S]", "~~"},
			{"[code]", "```"},
			{"[CODE]", "```"},
			{"[/code]", "```"},
			{"[/CODE]", "```"},
			{"[pre]", "```"},
			{"[PRE]", "```"},
			{"[/pre]", "```"},
			{"[/PRE]", "```"},
			{"[quote]", "```"},
			{"[QUOTE]", "```"},
			{"[/quote]", "```"},
			{"[/QUOTE]", "```"},
			{"[spoiler]", "||"},
			{"[SPOILER]", "||"},
			{"[/spoiler]", "||"},
			{"[/SPOILER]", "||"},
	};

	for (auto replacement : replacementMap) {
		findAndReplaceAll(msg, std::get<0>(replacement), std::get<1>(replacement));
	}

	// next remove all other BBCodes
	auto bbcodeRegex = std::regex("\\[/?[a-zA-Z]+\\]");
	msg = std::regex_replace(msg, bbcodeRegex, "");

	// finally replace all newlines with %0A
	findAndReplaceAll(msg, "\n", "%0A");

	// replace any leftover [ or ] with their escaped version
	findAndReplaceAll(msg, "[", "\\[");
	findAndReplaceAll(msg, "]", "\\]");

	return msg;
}