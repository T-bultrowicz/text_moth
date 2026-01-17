CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -Wno-experimental-header-units \
			-Wno-pragma-system-header-outside-header -Wno-inline-namespace-reopened-noninline

# Lista wszystkich potrzebnych header-units
SYSTEM_PCH = cstddef.pch string.pch vector.pch sstream.pch iostream.pch

.PHONY: all clean

all: text_moths

# Linkowanie końcowe
# Zależności: main.cpp oraz wszystkie moduły (.pcm) i headery (.pch)
text_moths: main.cpp lib.pcm parser.pcm moth.pcm engine.pcm $(SYSTEM_PCH)
	clang++ $(CXXFLAGS) \
		-fmodule-file=lib=lib.pcm \
		-fmodule-file=parser=parser.pcm \
		-fmodule-file=moth=moth.pcm \
		-fmodule-file=engine=engine.pcm \
		-fmodule-file=iostream.pch \
		-fmodule-file=string.pch \
		-fmodule-file=vector.pch \
		-fmodule-file=sstream.pch \
		main.cpp lib.pcm parser.pcm moth.pcm engine.pcm -o text_moths

# --- Reguły dla Header Units (budowane tylko raz!) ---

cstddef.pch:
	clang++ $(CXXFLAGS) -xc++-system-header --precompile cstddef -o cstddef.pch

string.pch:
	clang++ $(CXXFLAGS) -xc++-system-header --precompile string -o string.pch

vector.pch:
	clang++ $(CXXFLAGS) -xc++-system-header --precompile vector -o vector.pch

sstream.pch:
	clang++ $(CXXFLAGS) -xc++-system-header --precompile sstream -o sstream.pch

iostream.pch:
	clang++ $(CXXFLAGS) -xc++-system-header --precompile iostream -o iostream.pch

# --- Reguły dla Modułów ---

lib.pcm: lib.cppm cstddef.pch
	clang++ $(CXXFLAGS) -fmodule-file=cstddef.pch --precompile lib.cppm -o lib.pcm

parser.pcm: parser.cppm lib.pcm string.pch vector.pch sstream.pch
	clang++ $(CXXFLAGS) \
		-fmodule-file=string.pch \
		-fmodule-file=sstream.pch \
		-fmodule-file=vector.pch \
		-fmodule-file=lib=lib.pcm \
		--precompile parser.cppm -o parser.pcm

moth.pcm: moth.cppm lib.pcm string.pch iostream.pch
	clang++ $(CXXFLAGS) \
		-fmodule-file=string.pch \
		-fmodule-file=iostream.pch \
		-fmodule-file=lib=lib.pcm \
		--precompile moth.cppm -o moth.pcm

# # Engine (dodaj tutaj zależności engine'a)
# engine.pcm: engine.cppm lib.pcm parser.pcm moth.pcm iostream.pch string.pch
# 	clang++ $(CXXFLAGS) \
# 		-fmodule-file=lib.pcm \
# 		-fmodule-file=parser=parser.pcm \
# 		-fmodule-file=moth=moth.pcm \
# 		-fmodule-file=iostream.pch \
# 		-fmodule-file=string.pch \
# 		--precompile engine.cppm -o engine.pcm

clean:
	rm -f text_moths *.pcm *.pch