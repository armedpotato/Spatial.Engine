#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/Search.h>

namespace spatial::ui
{

bool Search::searchText(std::string& search)
{
	return ui::inputText("Search: ", search);
}

} // namespace spatial::ui