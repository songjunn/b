#pragma once
#include <string>
#include "utlsymbol.h"
#include "utlmap.h"

namespace attrs {

	struct CAttr {
		int attr_type;
		std::string attr_name;
	};

	bool init_all_attrs();

	// for player
	std::string get_player_field_name(int i);
	int	get_player_field_type(std::string name);

	// for item
	std::string get_item_field_name(int i);
	int	get_item_field_type(std::string name);

	// interface
	bool init_attrs(const CAttr _attr_array[], uint32 _array_len, CUtlMap<int, int>& _attr_type_table, CUtlMap<CUtlSymbol, int>& _attr_name_table);
	int get_attr_type(std::string name, CUtlMap<CUtlSymbol, int>& _attr_name_table);
	std::string get_attr_name(int type, const CAttr _attr_array[], CUtlMap<int, int>& _attr_type_table);

}
