#include "../ext/main.h"

void address_calculator();

template<class val>
std::vector<int> locations(std::vector<val> values_list, val value);

void memory::scanner_thread() {

    while (utils::minecraft_running()) {

        address_calculator();

        for (size_t address = memory::min_address; address < memory::max_address; address += 100000) {

            std::vector<double> memory(100000);

            if (!ReadProcessMemory(ext::mc_handle, (LPCVOID) address, &memory[0], 100000, NULL)) continue;

            if (!std::count(memory.begin(), memory.end(), polonium::break_default)) continue;
            
            for (int location : locations(memory, polonium::break_default)) {
                    
                size_t addr = address + location * sizeof(double);

                std::vector<float> poloniumf(200);
                std::vector<double> poloniumd(200);

                if (!ReadProcessMemory(ext::mc_handle, (LPCVOID)(addr - 100), &poloniumf[0], 200, NULL) && 
                    !ReadProcessMemory(ext::mc_handle, (LPCVOID)(addr - 100), &poloniumd[0], 200, NULL)) 
                    continue;
                    
                if (std::count(poloniumf.begin(), poloniumf.end(), memory::polonium::polonium_default)) {

                    for (int location : locations(poloniumf, memory::polonium::polonium_default)) {

                        memory::polonium::address_break.push_back(addr);
                        memory::polonium::address_float.push_back((addr - 100) + location * sizeof(float));

                        memory::polonium::address_float.erase(
                            unique(memory::polonium::address_float.begin(), memory::polonium::address_float.end()), memory::polonium::address_float.end());
                    }

                }

                if (std::count(poloniumd.begin(), poloniumd.end(), (double) memory::polonium::polonium_default)) {

                    for (int location : locations(poloniumd, (double) memory::polonium::polonium_default)) {

                        memory::polonium::address_break.push_back(addr);
                        memory::polonium::address_double.push_back((addr - 100) + location * sizeof(double));

                        memory::polonium::address_double.erase(
                            unique(memory::polonium::address_double.begin(), memory::polonium::address_double.end()), memory::polonium::address_double.end());
                    }
                }
            }
            memory::polonium::address_break.erase(
                unique(memory::polonium::address_break.begin(), memory::polonium::address_break.end()), memory::polonium::address_break.end());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    exit(0);
}

template<class val>
std::vector<int> locations(std::vector<val> values_list, val value) {
    std::vector<int> locations;

    for (int i = 0; i < values_list.size(); i++) {

        if (values_list[i] != value) continue;

        locations.push_back(i);
    }

    return locations;
}

void address_calculator() {
	
    size_t address = memory::calc_min_address;

    MEMORY_BASIC_INFORMATION mbi;

    for (
        size_t address = memory::calc_min_address;
        VirtualQueryEx(ext::mc_handle, (LPVOID)address, &mbi, sizeof(mbi)) && address < memory::calc_max_address;
        address += mbi.RegionSize
        ) {

        if (mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE && 
            mbi.Protect == PAGE_EXECUTE_READWRITE && mbi.AllocationProtect == PAGE_READWRITE) {

            memory::min_address = address;

            if (ext::other_client) memory::max_address = address + 251658240; //Pretty stupid but works :P
            
            else memory::max_address = address + mbi.RegionSize;
	    
	    break;
        }

    }
}
