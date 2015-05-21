This is cpp.info, produced by makeinfo version 4.0 from
/home/andris/build/gnu/gcc-2.953/gcc/cpp.texi.

INFO-DIR-SECTION Programming
START-INFO-DIR-ENTRY
* Cpp: (cpp).		       The GNU C preprocessor.
END-INFO-DIR-ENTRY

   This file documents the GNU C Preprocessor.

   Copyright 1987, 1989, 1991, 1992, 1993, 1994, 1995, 1997, 1998 Free
Software Foundation, Inc.

   Permission is granted to make and distribute verbatim copies of this
manual provided the copyright notice and this permission notice are
preserved on all copies.

   Permission is granted to copy and distribute modified versions of
this manual under the conditions for verbatim copying, provided also
that the entire resulting derived work is distributed under the terms
of a permission notice identical to this one.

   Permission is granted to copy and distribute translations of this
manual into another language, under the above conditions for modified
versions.


File: cpp.info,  Node: Invocation,  Next: Concept Index,  Prev: Output,  Up: Top

Invoking the C Preprocessor
===========================

   Most often when you use the C preprocessor you will not have to
invoke it explicitly: the C compiler will do so automatically.
However, the preprocessor is sometimes useful on its own.

   The C preprocessor expects two file names as arguments, INFILE and
OUTFILE.  The preprocessor reads INFILE together with any other files
it specifies with `#include'.  All the output generated by the combined
input files is written in OUTFILE.

   Either INFILE or OUTFILE may be `-', which as INFILE means to read
from standard input and as OUTFILE means to write to standard output.
Also, if OUTFILE or both file names are omitted, the standard output
and standard input are used for the omitted file names.

   Here is a table of command options accepted by the C preprocessor.
These options can also be given when compiling a C program; they are
passed along automatically to the preprocessor when it is invoked by the
compiler.

`-P'
     Inhibit generation of `#'-lines with line-number information in
     the output from the preprocessor (*note Output::).  This might be
     useful when running the preprocessor on something that is not C
     code and will be sent to a program which might be confused by the
     `#'-lines.

`-C'
     Do not discard comments: pass them through to the output file.
     Comments appearing in arguments of a macro call will be copied to
     the output before the expansion of the macro call.

`-traditional'
     Try to imitate the behavior of old-fashioned C, as opposed to ANSI
     C.

        * Traditional macro expansion pays no attention to singlequote
          or doublequote characters; macro argument symbols are
          replaced by the argument values even when they appear within
          apparent string or character constants.

        * Traditionally, it is permissible for a macro expansion to end
          in the middle of a string or character constant.  The
          constant continues into the text surrounding the macro call.

        * However, traditionally the end of the line terminates a
          string or character constant, with no error.

        * In traditional C, a comment is equivalent to no text at all.
          (In ANSI C, a comment counts as whitespace.)

        * Traditional C does not have the concept of a "preprocessing
          number".  It considers `1.0e+4' to be three tokens: `1.0e',
          `+', and `4'.

        * A macro is not suppressed within its own definition, in
          traditional C.  Thus, any macro that is used recursively
          inevitably causes an error.

        * The character `#' has no special meaning within a macro
          definition in traditional C.

        * In traditional C, the text at the end of a macro expansion
          can run together with the text after the macro call, to
          produce a single token.  (This is impossible in ANSI C.)

        * Traditionally, `\' inside a macro argument suppresses the
          syntactic significance of the following character.

     Use the `-traditional' option when preprocessing Fortran code, so
     that singlequotes and doublequotes within Fortran comment lines
     (which are generally not recognized as such by the preprocessor)
     do not cause diagnostics about unterminated character or string
     constants.

     However, this option does not prevent diagnostics about
     unterminated comments when a C-style comment appears to start, but
     not end, within Fortran-style commentary.

     So, the following Fortran comment lines are accepted with
     `-traditional':

          C This isn't an unterminated character constant
          C Neither is "20000000000, an octal constant
          C in some dialects of Fortran

     However, this type of comment line will likely produce a
     diagnostic, or at least unexpected output from the preprocessor,
     due to the unterminated comment:

          C Some Fortran compilers accept /* as starting
          C an inline comment.

     Note that `g77' automatically supplies the `-traditional' option
     when it invokes the preprocessor.  However, a future version of
     `g77' might use a different, more-Fortran-aware preprocessor in
     place of `cpp'.

`-trigraphs'
     Process ANSI standard trigraph sequences.  These are
     three-character sequences, all starting with `??', that are
     defined by ANSI C to stand for single characters.  For example,
     `??/' stands for `\', so `'??/n'' is a character constant for a
     newline.  Strictly speaking, the GNU C preprocessor does not
     support all programs in ANSI Standard C unless `-trigraphs' is
     used, but if you ever notice the difference it will be with relief.

     You don't want to know any more about trigraphs.

`-pedantic'
     Issue warnings required by the ANSI C standard in certain cases
     such as when text other than a comment follows `#else' or `#endif'.

`-pedantic-errors'
     Like `-pedantic', except that errors are produced rather than
     warnings.

`-Wtrigraphs'
     Warn if any trigraphs are encountered.  Currently this only works
     if you have turned trigraphs on with `-trigraphs' or `-ansi'; in
     the future this restriction will be removed.

`-Wcomment'
     Warn whenever a comment-start sequence `/*' appears in a `/*'
     comment, or whenever a Backslash-Newline appears in a `//' comment.

`-Wall'
     Requests both `-Wtrigraphs' and `-Wcomment' (but not
     `-Wtraditional' or `-Wundef').

`-Wtraditional'
     Warn about certain constructs that behave differently in
     traditional and ANSI C.

`-Wundef'
     Warn if an undefined identifier is evaluated in an `#if' directive.

`-I DIRECTORY'
     Add the directory DIRECTORY to the head of the list of directories
     to be searched for header files (*note Include Syntax::).  This
     can be used to override a system header file, substituting your
     own version, since these directories are searched before the system
     header file directories.  If you use more than one `-I' option,
     the directories are scanned in left-to-right order; the standard
     system directories come after.

`-I-'
     Any directories specified with `-I' options before the `-I-'
     option are searched only for the case of `#include "FILE"'; they
     are not searched for `#include <FILE>'.

     If additional directories are specified with `-I' options after
     the `-I-', these directories are searched for all `#include'
     directives.

     In addition, the `-I-' option inhibits the use of the current
     directory as the first search directory for `#include "FILE"'.
     Therefore, the current directory is searched only if it is
     requested explicitly with `-I.'.  Specifying both `-I-' and `-I.'
     allows you to control precisely which directories are searched
     before the current one and which are searched after.

`-nostdinc'
     Do not search the standard system directories for header files.
     Only the directories you have specified with `-I' options (and the
     current directory, if appropriate) are searched.

`-nostdinc++'
     Do not search for header files in the C++-specific standard
     directories, but do still search the other standard directories.
     (This option is used when building the C++ library.)

`-remap'
     When searching for a header file in a directory, remap file names
     if a file named `header.gcc' exists in that directory.  This can
     be used to work around limitations of file systems with file name
     restrictions.  The `header.gcc' file should contain a series of
     lines with two tokens on each line: the first token is the name to
     map, and the second token is the actual name to use.

`-D NAME'
     Predefine NAME as a macro, with definition `1'.

`-D NAME=DEFINITION'
     Predefine NAME as a macro, with definition DEFINITION.  There are
     no restrictions on the contents of DEFINITION, but if you are
     invoking the preprocessor from a shell or shell-like program you
     may need to use the shell's quoting syntax to protect characters
     such as spaces that have a meaning in the shell syntax.  If you
     use more than one `-D' for the same NAME, the rightmost definition
     takes effect.

`-U NAME'
     Do not predefine NAME.  If both `-U' and `-D' are specified for
     one name, the `-U' beats the `-D' and the name is not predefined.

`-undef'
     Do not predefine any nonstandard macros.

`-gcc'
     Define the macros __GNUC__ and __GNUC_MINOR__.  These are defined
     automatically when you use `gcc -E'; you can turn them off in that
     case with `-no-gcc'.

`-A PREDICATE(ANSWER)'
     Make an assertion with the predicate PREDICATE and answer ANSWER.
     *Note Assertions::.

     You can use `-A-' to disable all predefined assertions; it also
     undefines all predefined macros and all macros that preceded it on
     the command line.

`-dM'
     Instead of outputting the result of preprocessing, output a list of
     `#define' directives for all the macros defined during the
     execution of the preprocessor, including predefined macros.  This
     gives you a way of finding out what is predefined in your version
     of the preprocessor; assuming you have no file `foo.h', the command

          touch foo.h; cpp -dM foo.h

     will show the values of any predefined macros.

`-dD'
     Like `-dM' except in two respects: it does _not_ include the
     predefined macros, and it outputs _both_ the `#define' directives
     and the result of preprocessing.  Both kinds of output go to the
     standard output file.

`-dI'
     Output `#include' directives in addition to the result of
     preprocessing.

`-M [-MG]'
     Instead of outputting the result of preprocessing, output a rule
     suitable for `make' describing the dependencies of the main source
     file.  The preprocessor outputs one `make' rule containing the
     object file name for that source file, a colon, and the names of
     all the included files.  If there are many included files then the
     rule is split into several lines using `\'-newline.

     `-MG' says to treat missing header files as generated files and
     assume they live in the same directory as the source file.  It
     must be specified in addition to `-M'.

     This feature is used in automatic updating of makefiles.

`-MM [-MG]'
     Like `-M' but mention only the files included with `#include
     "FILE"'.  System header files included with `#include <FILE>' are
     omitted.

`-MD FILE'
     Like `-M' but the dependency information is written to FILE.  This
     is in addition to compiling the file as specified--`-MD' does not
     inhibit ordinary compilation the way `-M' does.

     When invoking `gcc', do not specify the FILE argument.  `gcc' will
     create file names made by replacing ".c" with ".d" at the end of
     the input file names.

     In Mach, you can use the utility `md' to merge multiple dependency
     files into a single dependency file suitable for using with the
     `make' command.

`-MMD FILE'
     Like `-MD' except mention only user header files, not system
     header files.

`-H'
     Print the name of each header file used, in addition to other
     normal activities.

`-imacros FILE'
     Process FILE as input, discarding the resulting output, before
     processing the regular input file.  Because the output generated
     from FILE is discarded, the only effect of `-imacros FILE' is to
     make the macros defined in FILE available for use in the main
     input.

`-include FILE'
     Process FILE as input, and include all the resulting output,
     before processing the regular input file.

`-idirafter DIR'
     Add the directory DIR to the second include path.  The directories
     on the second include path are searched when a header file is not
     found in any of the directories in the main include path (the one
     that `-I' adds to).

`-iprefix PREFIX'
     Specify PREFIX as the prefix for subsequent `-iwithprefix' options.

`-iwithprefix DIR'
     Add a directory to the second include path.  The directory's name
     is made by concatenating PREFIX and DIR, where PREFIX was
     specified previously with `-iprefix'.

`-isystem DIR'
     Add a directory to the beginning of the second include path,
     marking it as a system directory, so that it gets the same special
     treatment as is applied to the standard system directories.

`-x c'
`-x c++'
`-x objective-c'
`-x assembler-with-cpp'
     Specify the source language: C, C++, Objective-C, or assembly.
     This has nothing to do with standards conformance or extensions;
     it merely selects which base syntax to expect.  If you give none
     of these options, cpp will deduce the language from the extension
     of the source file: `.c', `.cc', `.m', or `.S'.  Some other common
     extensions for C++ and assembly are also recognized.  If cpp does
     not recognize the extension, it will treat the file as C; this is
     the most generic mode.

     *Note:* Previous versions of cpp accepted a `-lang' option which
     selected both the language and the standards conformance level.
     This option has been removed, because it conflicts with the `-l'
     option.

`-std=STANDARD'
`-ansi'
     Specify the standard to which the code should conform.  Currently
     cpp only knows about the standards for C; other language standards
     will be added in the future.

     STANDARD may be one of:
    `iso9899:1990'
          The ISO C standard from 1990.

    `iso9899:199409'
    `c89'
          The 1990 C standard, as amended in 1994.  `c89' is the
          customary shorthand for this version of the standard.

          The `-ansi' option is equivalent to `-std=c89'.

    `iso9899:199x'
    `c9x'
          The revised ISO C standard, which is expected to be
          promulgated some time in 1999.  It has not been approved yet,
          hence the `x'.

    `gnu89'
          The 1990 C standard plus GNU extensions.  This is the default.

    `gnu9x'
          The 199x C standard plus GNU extensions.

`-Wp,-lint'
     Look for commands to the program checker `lint' embedded in
     comments, and emit them preceded by `#pragma lint'.  For example,
     the comment `/* NOTREACHED */' becomes `#pragma lint NOTREACHED'.

     Because of the clash with `-l', you must use the awkward syntax
     above.  In a future release, this option will be replaced by
     `-flint' or `-Wlint'; we are not sure which yet.

`-$'
     Forbid the use of `$' in identifiers.  The C standard does not
     permit this, but it is a common extension.


File: cpp.info,  Node: Concept Index,  Next: Index,  Prev: Invocation,  Up: Top

Concept Index
*************

* Menu:

* ##:                                    Concatenation.
* arguments in macro definitions:        Argument Macros.
* assertions:                            Assertions.
* assertions, undoing:                   Assertions.
* blank macro arguments:                 Argument Macros.
* cascaded macros:                       Cascaded Macros.
* commenting out code:                   Deleted Code.
* computed #include:                     Include Syntax.
* concatenation:                         Concatenation.
* conditionals:                          Conditionals.
* directives:                            Directives.
* expansion of arguments:                Argument Prescan.
* Fortran:                               Invocation.
* function-like macro:                   Argument Macros.
* g77:                                   Invocation.
* header file:                           Header Files.
* including just once:                   Once-Only.
* inheritance:                           Inheritance.
* invocation of the preprocessor:        Invocation.
* line control:                          Combining Sources.
* macro argument expansion:              Argument Prescan.
* macro body uses macro:                 Cascaded Macros.
* macros with argument:                  Argument Macros.
* manifest constant:                     Simple Macros.
* newlines in macro arguments:           Newlines in Args.
* null directive:                        Other Directives.
* options:                               Invocation.
* output format:                         Output.
* overriding a header file:              Inheritance.
* parentheses in macro bodies:           Macro Parentheses.
* pitfalls of macros:                    Macro Pitfalls.
* predefined macros:                     Predefined.
* predicates:                            Assertions.
* preprocessing directives:              Directives.
* prescan of macro arguments:            Argument Prescan.
* problems with macros:                  Macro Pitfalls.
* redefining macros:                     Redefining.
* repeated inclusion:                    Once-Only.
* retracting assertions:                 Assertions.
* second include path:                   Invocation.
* self-reference:                        Self-Reference.
* semicolons (after macro calls):        Swallow Semicolon.
* side effects (in macro arguments):     Side Effects.
* simple macro:                          Simple Macros.
* space as macro argument:               Argument Macros.
* standard predefined macros:            Standard Predefined.
* stringification:                       Stringification.
* testing predicates:                    Assertions.
* unassert:                              Assertions.
* undefining macros:                     Undefining.
* unsafe macros:                         Side Effects.
* unterminated:                          Invocation.


File: cpp.info,  Node: Index,  Prev: Concept Index,  Up: Top

Index of Directives, Macros and Options
***************************************

* Menu:

* #assert:                               Assertions.
* #cpu:                                  Assertions.
* #define:                               Argument Macros.
* #elif:                                 #elif Directive.
* #else:                                 #else Directive.
* #error:                                #error Directive.
* #ident:                                Other Directives.
* #if:                                   Conditional Syntax.
* #ifdef:                                Conditionals-Macros.
* #ifndef:                               Conditionals-Macros.
* #import:                               Once-Only.
* #include:                              Include Syntax.
* #include_next:                         Inheritance.
* #line:                                 Combining Sources.
* #machine:                              Assertions.
* #pragma:                               Other Directives.
* #pragma once:                          Once-Only.
* #system:                               Assertions.
* #unassert:                             Assertions.
* #warning:                              #error Directive.
* -$:                                    Invocation.
* -A:                                    Invocation.
* -ansi:                                 Invocation.
* -C:                                    Invocation.
* -D:                                    Invocation.
* -dD:                                   Invocation.
* -dI:                                   Invocation.
* -dM:                                   Invocation.
* -gcc:                                  Invocation.
* -H:                                    Invocation.
* -I:                                    Invocation.
* -idirafter:                            Invocation.
* -imacros:                              Invocation.
* -include:                              Invocation.
* -iprefix:                              Invocation.
* -isystem:                              Invocation.
* -iwithprefix:                          Invocation.
* -lint:                                 Invocation.
* -M:                                    Invocation.
* -MD:                                   Invocation.
* -MM:                                   Invocation.
* -MMD:                                  Invocation.
* -nostdinc:                             Invocation.
* -nostdinc++:                           Invocation.
* -P:                                    Invocation.
* -pedantic:                             Invocation.
* -pedantic-errors:                      Invocation.
* -remap:                                Invocation.
* -std:                                  Invocation.
* -traditional:                          Invocation.
* -trigraphs:                            Invocation.
* -U:                                    Invocation.
* -undef:                                Invocation.
* -Wall:                                 Invocation.
* -Wcomment:                             Invocation.
* -Wtraditional:                         Invocation.
* -Wtrigraphs:                           Invocation.
* -Wundef:                               Invocation.
* -x assembler-with-cpp:                 Invocation.
* -x c:                                  Invocation.
* -x objective-c:                        Invocation.
* __BASE_FILE__:                         Standard Predefined.
* __CHAR_UNSIGNED__:                     Standard Predefined.
* __cplusplus:                           Standard Predefined.
* __DATE__:                              Standard Predefined.
* __FILE__:                              Standard Predefined.
* __GNUC__:                              Standard Predefined.
* __GNUC_MINOR__:                        Standard Predefined.
* __GNUG__:                              Standard Predefined.
* __INCLUDE_LEVEL_:                      Standard Predefined.
* __LINE__:                              Standard Predefined.
* __OPTIMIZE__:                          Standard Predefined.
* __REGISTER_PREFIX__:                   Standard Predefined.
* __STDC__:                              Standard Predefined.
* __STDC_VERSION__:                      Standard Predefined.
* __STRICT_ANSI__:                       Standard Predefined.
* __TIME__:                              Standard Predefined.
* __USER_LABEL_PREFIX__:                 Standard Predefined.
* __VERSION__:                           Standard Predefined.
* _AM29000:                              Nonstandard Predefined.
* _AM29K:                                Nonstandard Predefined.
* BSD:                                   Nonstandard Predefined.
* defined:                               Conditionals-Macros.
* M68020:                                Nonstandard Predefined.
* m68k:                                  Nonstandard Predefined.
* mc68000:                               Nonstandard Predefined.
* ns32000:                               Nonstandard Predefined.
* pyr:                                   Nonstandard Predefined.
* sequent:                               Nonstandard Predefined.
* sun:                                   Nonstandard Predefined.
* system header files:                   Header Uses.
* unix:                                  Nonstandard Predefined.
* vax:                                   Nonstandard Predefined.


