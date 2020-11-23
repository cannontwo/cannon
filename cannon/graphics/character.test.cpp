#include <catch2/catch.hpp>

#include <cannon/graphics/character.hpp>

using namespace cannon::graphics;

TEST_CASE("Character", "[graphics]"){
  // Character only contains data, and so cannot touch OpenGL state.
}
