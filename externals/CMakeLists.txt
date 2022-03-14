file(GLOB json11_src
     "*.h"
     "*.hpp"
     "*.c"
     "*.cpp"
)

target_sources(taraxa PRIVATE ${json11_src})
