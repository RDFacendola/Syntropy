Coding style - Version 0.0.1
=============

This document describes the coding conventions and style for the Syntropy codebase. The style described in this document is based upon [Google coding conventions](https://google.github.io/styleguide/cppguide.html):
exeptions and tldr are described below.

General naming guidelines
--------
Names should be short and descriptive. Names should not hint to containers or other informations about the type (e.g: `names_` rather than `name_list_`).

Acronyms should be avoided except when they refer to a common abbreviation (such as URL, GUID, USB). Acronyms are either all lowercase or
all uppercase, case mixing is not allowed (e.g: `GetURL()`, `my_url)`.

Tentative words such as _"should"_, _"must"_, _"will"_, _"wants"_ should be avoided as they don't add any useful information and are generally unsettling 
(e.g: `should_clear` are there conditions for which this value will be ignored? `will_send` cool, when? `wants_begin_play` meaning that someone may prevent that from ever happen?).

Avoid redundant and overly descriptive names. As a rule of thumb, two words are usually more than enough to describe everything: additional info should be part of the documentation.

File Names
--------
`syntropy.h, hashed_string.h, my_url.h`

File names should be all lowercase with words separated by underscores.

Namespaces
--------
`syntropy, synergy, synchrony, synergy::reflection`

Namespaces should be all lowercase and composed of a single word.

Classes and methods must always be contained inside the namespace of the module they belong to.
Nested namespaces are encouraged and should be used to organize classes by feature. Each nested namespace should be associated to a folder with the same name.

'using namespace ...' is forbidden as it pollutes the namespace. Anonymous namespaces are allowed but shall not be abused (consider exposing functionalities first).

Classes and Struct
--------
`MemoryManager, JSONDeserializable, CompositeURL, Property`

Classes and structs should start with a capital letter and have a capital letter for each new word, with no underscores.

Structs should be used for POD data (which can expose some minimal interface). Everything else should be a class.

Immutable classes are encouraged where possible.

Access specifiers should not be mixed and must be declared in the following order: `Public, Protected and Private`.
Declaration order should be: `typenames, enumerations, constants\constexprs, struct\classes, method and variables`.
This order guarantees that each name is declared before its usage (reader can read the class from the start without having to wander around).

Variable Names
--------
`my_variable, my_url, my_member_variable_, your_member_guid_`

Non-member variables should be all lowercase with words separated by underscores.
Member variables should end with a trailing underscore.

Global variables are strictly forbidden except when static initialization is required _(e.g: registering a test suite to the unit test system or classes to the reflection system)._

Function Names
--------
`DoSomethingCool(), GetID(), GetMoney(), SetFree(), IsAwesome(), HasCheezburger(), IsEnabled()`

Functions should start with a capital letter and have a capital letter for each new word, with no undercores.

Getters and setters for should start with 'Get' or 'Set' respectively. Getters of boolean values, instead, should start with 'Is' or 'Has'.

Enumerations
--------
`enum class DayOfWeek: uint8_t { Monday, Tuesday, ...};, enum class Direction: uint8_t{ North, South, East, West };`

Only scoped enumeration (i.e. enumeration classes) are allowed. Unscoped enumerations are forbidden. 
Enumerations and enumeration values should start with a capital letter and have a capital letter for each new word, with no underscores.

Constants
--------
`kPi, kEpsilon, kBufferSize`

Constants should start with a lowercase 'k' and have a capital letter for each new word, with no undercores.

Templates
---------
```
template <typename TType> 
class PropertyT{...};

template <size_t kSize> 
struct FixedBuffer{...};
```

Class templates should start with a capital letter and have a capital letter for each new word, with no underscores. 
The name may end with a capital T to avoid name clashing which may arise for derived classes templates, or type-erased classes.

Non-type template parameters are constant and should follow the constant naming style.

Type Traits
---------
```
template <typename TType>
struct is_class_name{...}
```

Type traits should be all lowercase with words should be separated by underscores.
This style, identical to the standard type traits one, makes more pleasant to write down SFINAE conditions.

Macros
---------
`SYNTROPY_LOG, SYNTROPY_ERROR`

Macros should be all uppercase with words separated by underscores. 

Avoid using macros at _all costs_ except when they wrap a preprocessor directive (such as `_LINE_`, `_FILE_`, etc.)
which cannot be used otherwise.

Documentation
=============

Files, classes, methods and variables must always be documented using the following **doxygen** syntax:

`/// Doxygen comments begin with a triple-backslash. Don't use these for any other reason!`

`a = a * b;      // Non-doxygen comments should use the common double-slash or slash-star syntax.`

`++b;            /* Don't forget to end each comment with a full stop. */`

```
/// \brief This is a brief description of the entity *below*. This is part of the index.
/// This is the rest of the documentation.
/// It may span as many lines as needed.
/// Other directives, see below.
class Nice{ ... };
```

```
float my_variable_    ///< \brief This is a brief description of the entity on the *left*.
                      ///         This is the rest of the documentation.
```

`/// \author Jhon Doe - February 2034`

`/// \remarks Watch out undefined behaviors and corner cases!`

`/// \see See Buddy::OtherMethod.`

`/// \param foo_parameter Describes a function argument.`

`/// \return Describes the return value of the function.`

`/// \tparam TArgument Describes the template parameter.`