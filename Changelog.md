
# Version 3.1

Can now use dashes and underscores `- and _` for long flags.
Registering flags may now throw exceptions if the flag is invalid.
More testing added to ParserTest.

# Version 3.0

parse function now returns True if parsed without invalid input.  
parse function has changed 3rd parameter, which sets if invalid input
immediately halts parsing.  
parse function no longer checks for repeated flags. All repeated flags will be
called.

printHelp output improved.

# Version 2.2

Fixed building in another project as submodule.

# Version 2.1

Added install rule for static libs.  
Changed install for libs to just "lib" and for headers to just "include".

# Version 2.0

"First" release.

