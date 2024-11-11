#pragma once

#include <string>

namespace factify {

  /**  Language codes to be used with the Factify class */
  enum class LanguageCode { EN, DE, ES, FR };

  /**
   * @brief A class for saying hello in multiple languages
   */
  class Factify {
    std::string name;

  public:
    /**
     * @brief Creates a new factify
     * @param name the name to greet
     */
    Factify(std::string name);

    /**
     * @brief Creates a localized string containing the greeting
     * @param lang the language to greet in
     * @return a string containing the greeting
     */
    std::string greet(LanguageCode lang = LanguageCode::EN) const;
  };

}  // namespace factify
