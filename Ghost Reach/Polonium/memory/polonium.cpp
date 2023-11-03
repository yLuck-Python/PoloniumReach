#include "../ext/main.h"


void memory::polonium_thread() {

	while (true) {

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		utils::write_memory(polonium::address_break, polonium::break_default, polonium::new_break);
		utils::write_memory(polonium::address_float, polonium::polonium_default, polonium::new_polonium);
		utils::write_memory(polonium::address_double, (double) polonium::polonium_default, (double) polonium::new_polonium);
	}
}

template<class val>
void utils::write_memory(std::vector<size_t> address_list, val default_value, val new_value) {

	for (size_t address : address_list) {
		val buffer;

		if (!ReadProcessMemory(ext::mc_handle, (LPCVOID) address, &buffer, sizeof(val), NULL)) continue;

		if (buffer != default_value && buffer != new_value) continue;

		WriteProcessMemory(ext::mc_handle, (LPVOID)address, &new_value, sizeof(val), NULL);
	}

}

