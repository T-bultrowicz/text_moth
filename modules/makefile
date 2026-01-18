CXX = clang++
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra \
           -Wno-experimental-header-units \
           -Wno-pragma-system-header-outside-header \
           -Wno-inline-namespace-reopened-noninline

# Katalog na pliki pośrednie i wynikowe
BUILD_DIR = build

# Ścieżka do szukania gotowych modułów
MOD_PATH = -fprebuilt-module-path=$(BUILD_DIR)

# Lista plików PCH (System Headers)
PCH_NAMES = cstddef string vector sstream iostream map memory
SYSTEM_PCH = $(addprefix $(BUILD_DIR)/, $(addsuffix .pch, $(PCH_NAMES)))

# Lista modułów projektu
MODULE_NAMES = lib parser moth engine
MODULE_PCM = $(addprefix $(BUILD_DIR)/, $(addsuffix .pcm, $(MODULE_NAMES)))

.PHONY: all clean

all: $(BUILD_DIR)/text_moths

# --- Reguła tworzenia katalogu build ---
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --- Reguła ogólna dla System Headers (.pch) ---
# Tworzy np. build/vector.pch z vector
$(BUILD_DIR)/%.pch: | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -xc++-system-header --precompile $* -o $@

# --- Linkowanie końcowe ---
$(BUILD_DIR)/text_moths: main.cpp $(MODULE_PCM) $(SYSTEM_PCH) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MOD_PATH) \
		-fmodule-file=$(BUILD_DIR)/iostream.pch \
		-fmodule-file=$(BUILD_DIR)/string.pch \
		-fmodule-file=$(BUILD_DIR)/vector.pch \
		-fmodule-file=$(BUILD_DIR)/sstream.pch \
		-fmodule-file=$(BUILD_DIR)/map.pch \
		-fmodule-file=$(BUILD_DIR)/memory.pch \
		-fmodule-file=$(BUILD_DIR)/cstddef.pch \
		main.cpp \
		$(BUILD_DIR)/lib.pcm \
		$(BUILD_DIR)/parser.pcm \
		$(BUILD_DIR)/moth.pcm \
		$(BUILD_DIR)/engine.pcm \
		-o text_moths

# --- Moduły Projektu ---

# lib.pcm
$(BUILD_DIR)/lib.pcm: lib.cppm $(BUILD_DIR)/cstddef.pch | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MOD_PATH) \
		-fmodule-file=$(BUILD_DIR)/cstddef.pch \
		--precompile lib.cppm -o $@

# parser.pcm
$(BUILD_DIR)/parser.pcm: parser.cppm $(BUILD_DIR)/lib.pcm \
                        $(BUILD_DIR)/string.pch $(BUILD_DIR)/vector.pch $(BUILD_DIR)/sstream.pch | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MOD_PATH) \
		-fmodule-file=$(BUILD_DIR)/string.pch \
		-fmodule-file=$(BUILD_DIR)/sstream.pch \
		-fmodule-file=$(BUILD_DIR)/vector.pch \
		-fprebuilt-module-path=. \
		--precompile parser.cppm -o $@

# moth.pcm
$(BUILD_DIR)/moth.pcm: moth.cppm $(BUILD_DIR)/lib.pcm \
                       $(BUILD_DIR)/string.pch $(BUILD_DIR)/iostream.pch | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MOD_PATH) \
		-fmodule-file=$(BUILD_DIR)/string.pch \
		-fmodule-file=$(BUILD_DIR)/iostream.pch \
		-fprebuilt-module-path=. \
		--precompile moth.cppm -o $@

# engine.pcm
$(BUILD_DIR)/engine.pcm: engine.cppm $(BUILD_DIR)/lib.pcm $(BUILD_DIR)/parser.pcm $(BUILD_DIR)/moth.pcm \
                         $(BUILD_DIR)/map.pch $(BUILD_DIR)/memory.pch $(BUILD_DIR)/vector.pch \
                         $(BUILD_DIR)/string.pch $(BUILD_DIR)/iostream.pch | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MOD_PATH) \
		-fmodule-file=$(BUILD_DIR)/map.pch \
		-fmodule-file=$(BUILD_DIR)/memory.pch \
		-fmodule-file=$(BUILD_DIR)/vector.pch \
		-fmodule-file=$(BUILD_DIR)/string.pch \
		-fmodule-file=$(BUILD_DIR)/iostream.pch \
		-fprebuilt-module-path=. \
		--precompile engine.cppm -o $@

clean:
	rm -rf $(BUILD_DIR)