#include <doctest/doctest.h>
#include <factify/factify.h>
#include <factify/version.h>

#include <string>

TEST_CASE("Factify") {
  using namespace factify;

  Factify factify("Tests");

  CHECK(factify.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(factify.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(factify.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(factify.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("Factify version") {
  static_assert(std::string_view(FACTIFY_VERSION) == std::string_view("1.0"));
  CHECK(std::string(FACTIFY_VERSION) == std::string("1.0"));
}
