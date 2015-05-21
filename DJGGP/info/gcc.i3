This is gcc.info, produced by makeinfo version 4.0 from
/home/andris/build/gnu/gcc-2.953/gcc/gcc.texi.

INFO-DIR-SECTION Programming
START-INFO-DIR-ENTRY
* gcc: (gcc).                  The GNU Compiler Collection.
END-INFO-DIR-ENTRY
   This file documents the use and the internals of the GNU compiler.

   Published by the Free Software Foundation 59 Temple Place - Suite 330
Boston, MA 02111-1307 USA

   Copyright (C) 1988, 1989, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
1999, 2000 Free Software Foundation, Inc.

   Permission is granted to make and distribute verbatim copies of this
manual provided the copyright notice and this permission notice are
preserved on all copies.

   Permission is granted to copy and distribute modified versions of
this manual under the conditions for verbatim copying, provided also
that the sections entitled "GNU General Public License" and "Funding
for Free Software" are included exactly as in the original, and
provided that the entire resulting derived work is distributed under
the terms of a permission notice identical to this one.

   Permission is granted to copy and distribute translations of this
manual into another language, under the above conditions for modified
versions, except that the sections entitled "GNU General Public
License" and "Funding for Free Software", and this permission notice,
may be included in translations approved by the Free Software Foundation
instead of in the original English.


File: gcc.info,  Node: Debugging Options,  Next: Optimize Options,  Prev: Warning Options,  Up: Invoking GCC

Options for Debugging Your Program or GCC
=========================================

   GCC has various special options that are used for debugging either
your program or GCC:

`-g'
     Produce debugging information in the operating system's native
     format (stabs, COFF, XCOFF, or DWARF).  GDB can work with this
     debugging information.

     On most systems that use stabs format, `-g' enables use of extra
     debugging information that only GDB can use; this extra information
     makes debugging work better in GDB but will probably make other
     debuggers crash or refuse to read the program.  If you want to
     control for certain whether to generate the extra information, use
     `-gstabs+', `-gstabs', `-gxcoff+', `-gxcoff', `-gdwarf-1+', or
     `-gdwarf-1' (see below).

     Unlike most other C compilers, GCC allows you to use `-g' with
     `-O'.  The shortcuts taken by optimized code may occasionally
     produce surprising results: some variables you declared may not
     exist at all; flow of control may briefly move where you did not
     expect it; some statements may not be executed because they
     compute constant results or their values were already at hand;
     some statements may execute in different places because they were
     moved out of loops.

     Nevertheless it proves possible to debug optimized output.  This
     makes it reasonable to use the optimizer for programs that might
     have bugs.

     The following options are useful when GCC is generated with the
     capability for more than one debugging format.

`-ggdb'
     Produce debugging information for use by GDB.  This means to use
     the most expressive format available (DWARF 2, stabs, or the
     native format if neither of those are supported), including GDB
     extensions if at all possible.

`-gstabs'
     Produce debugging information in stabs format (if that is
     supported), without GDB extensions.  This is the format used by
     DBX on most BSD systems.  On MIPS, Alpha and System V Release 4
     systems this option produces stabs debugging output which is not
     understood by DBX or SDB.  On System V Release 4 systems this
     option requires the GNU assembler.

`-gstabs+'
     Produce debugging information in stabs format (if that is
     supported), using GNU extensions understood only by the GNU
     debugger (GDB).  The use of these extensions is likely to make
     other debuggers crash or refuse to read the program.

`-gcoff'
     Produce debugging information in COFF format (if that is
     supported).  This is the format used by SDB on most System V
     systems prior to System V Release 4.

`-gxcoff'
     Produce debugging information in XCOFF format (if that is
     supported).  This is the format used by the DBX debugger on IBM
     RS/6000 systems.

`-gxcoff+'
     Produce debugging information in XCOFF format (if that is
     supported), using GNU extensions understood only by the GNU
     debugger (GDB).  The use of these extensions is likely to make
     other debuggers crash or refuse to read the program, and may cause
     assemblers other than the GNU assembler (GAS) to fail with an
     error.

`-gdwarf'
     Produce debugging information in DWARF version 1 format (if that is
     supported).  This is the format used by SDB on most System V
     Release 4 systems.

`-gdwarf+'
     Produce debugging information in DWARF version 1 format (if that is
     supported), using GNU extensions understood only by the GNU
     debugger (GDB).  The use of these extensions is likely to make
     other debuggers crash or refuse to read the program.

`-gdwarf-2'
     Produce debugging information in DWARF version 2 format (if that is
     supported).  This is the format used by DBX on IRIX 6.

`-gLEVEL'
`-ggdbLEVEL'
`-gstabsLEVEL'
`-gcoffLEVEL'
`-gxcoffLEVEL'
`-gdwarfLEVEL'
`-gdwarf-2LEVEL'
     Request debugging information and also use LEVEL to specify how
     much information.  The default level is 2.

     Level 1 produces minimal information, enough for making backtraces
     in parts of the program that you don't plan to debug.  This
     includes descriptions of functions and external variables, but no
     information about local variables and no line numbers.

     Level 3 includes extra information, such as all the macro
     definitions present in the program.  Some debuggers support macro
     expansion when you use `-g3'.

`-p'
     Generate extra code to write profile information suitable for the
     analysis program `prof'.  You must use this option when compiling
     the source files you want data about, and you must also use it when
     linking.

`-pg'
     Generate extra code to write profile information suitable for the
     analysis program `gprof'.  You must use this option when compiling
     the source files you want data about, and you must also use it when
     linking.

`-a'
     Generate extra code to write profile information for basic blocks,
     which will record the number of times each basic block is
     executed, the basic block start address, and the function name
     containing the basic block.  If `-g' is used, the line number and
     filename of the start of the basic block will also be recorded.
     If not overridden by the machine description, the default action is
     to append to the text file `bb.out'.

     This data could be analyzed by a program like `tcov'.  Note,
     however, that the format of the data is not what `tcov' expects.
     Eventually GNU `gprof' should be extended to process this data.

`-Q'
     Makes the compiler print out each function name as it is compiled,
     and print some statistics about each pass when it finishes.

`-ax'
     Generate extra code to profile basic blocks.  Your executable will
     produce output that is a superset of that produced when `-a' is
     used.  Additional output is the source and target address of the
     basic blocks where a jump takes place, the number of times a jump
     is executed, and (optionally) the complete sequence of basic
     blocks being executed.  The output is appended to file `bb.out'.

     You can examine different profiling aspects without recompilation.
     Your executable will read a list of function names from file
     `bb.in'.  Profiling starts when a function on the list is entered
     and stops when that invocation is exited.  To exclude a function
     from profiling, prefix its name with `-'.  If a function name is
     not unique, you can disambiguate it by writing it in the form
     `/path/filename.d:functionname'.  Your executable will write the
     available paths and filenames in file `bb.out'.

     Several function names have a special meaning:
    `__bb_jumps__'
          Write source, target and frequency of jumps to file `bb.out'.

    `__bb_hidecall__'
          Exclude function calls from frequency count.

    `__bb_showret__'
          Include function returns in frequency count.

    `__bb_trace__'
          Write the sequence of basic blocks executed to file
          `bbtrace.gz'.  The file will be compressed using the program
          `gzip', which must exist in your `PATH'.  On systems without
          the `popen' function, the file will be named `bbtrace' and
          will not be compressed.  *Profiling for even a few seconds on
          these systems will produce a very large file.*  Note:
          `__bb_hidecall__' and `__bb_showret__' will not affect the
          sequence written to `bbtrace.gz'.

     Here's a short example using different profiling parameters in
     file `bb.in'.  Assume function `foo' consists of basic blocks 1
     and 2 and is called twice from block 3 of function `main'.  After
     the calls, block 3 transfers control to block 4 of `main'.

     With `__bb_trace__' and `main' contained in file `bb.in', the
     following sequence of blocks is written to file `bbtrace.gz': 0 3
     1 2 1 2 4.  The return from block 2 to block 3 is not shown,
     because the return is to a point inside the block and not to the
     top.  The block address 0 always indicates, that control is
     transferred to the trace from somewhere outside the observed
     functions.  With `-foo' added to `bb.in', the blocks of function
     `foo' are removed from the trace, so only 0 3 4 remains.

     With `__bb_jumps__' and `main' contained in file `bb.in', jump
     frequencies will be written to file `bb.out'.  The frequencies are
     obtained by constructing a trace of blocks and incrementing a
     counter for every neighbouring pair of blocks in the trace.  The
     trace 0 3 1 2 1 2 4 displays the following frequencies:

          Jump from block 0x0 to block 0x3 executed 1 time(s)
          Jump from block 0x3 to block 0x1 executed 1 time(s)
          Jump from block 0x1 to block 0x2 executed 2 time(s)
          Jump from block 0x2 to block 0x1 executed 1 time(s)
          Jump from block 0x2 to block 0x4 executed 1 time(s)

     With `__bb_hidecall__', control transfer due to call instructions
     is removed from the trace, that is the trace is cut into three
     parts: 0 3 4, 0 1 2 and 0 1 2.  With `__bb_showret__', control
     transfer due to return instructions is added to the trace.  The
     trace becomes: 0 3 1 2 3 1 2 3 4.  Note, that this trace is not
     the same, as the sequence written to `bbtrace.gz'.  It is solely
     used for counting jump frequencies.

`-fprofile-arcs'
     Instrument "arcs" during compilation.  For each function of your
     program, GCC creates a program flow graph, then finds a spanning
     tree for the graph.  Only arcs that are not on the spanning tree
     have to be instrumented: the compiler adds code to count the
     number of times that these arcs are executed.  When an arc is the
     only exit or only entrance to a block, the instrumentation code
     can be added to the block; otherwise, a new basic block must be
     created to hold the instrumentation code.

     Since not every arc in the program must be instrumented, programs
     compiled with this option run faster than programs compiled with
     `-a', which adds instrumentation code to every basic block in the
     program.  The tradeoff: since `gcov' does not have execution
     counts for all branches, it must start with the execution counts
     for the instrumented branches, and then iterate over the program
     flow graph until the entire graph has been solved.  Hence, `gcov'
     runs a little more slowly than a program which uses information
     from `-a'.

     `-fprofile-arcs' also makes it possible to estimate branch
     probabilities, and to calculate basic block execution counts.  In
     general, basic block execution counts do not give enough
     information to estimate all branch probabilities.  When the
     compiled program exits, it saves the arc execution counts to a
     file called `SOURCENAME.da'.  Use the compiler option
     `-fbranch-probabilities' (*note Options that Control Optimization:
     Optimize Options.) when recompiling, to optimize using estimated
     branch probabilities.

`-ftest-coverage'
     Create data files for the `gcov' code-coverage utility (*note
     `gcov': a GCC Test Coverage Program: Gcov.).  The data file names
     begin with the name of your source file:

    `SOURCENAME.bb'
          A mapping from basic blocks to line numbers, which `gcov'
          uses to associate basic block execution counts with line
          numbers.

    `SOURCENAME.bbg'
          A list of all arcs in the program flow graph.  This allows
          `gcov' to reconstruct the program flow graph, so that it can
          compute all basic block and arc execution counts from the
          information in the `SOURCENAME.da' file (this last file is
          the output from `-fprofile-arcs').

`-Q'
     Makes the compiler print out each function name as it is compiled,
     and print some statistics about each pass when it finishes.

`-dLETTERS'
     Says to make debugging dumps during compilation at times specified
     by LETTERS.  This is used for debugging the compiler.  The file
     names for most of the dumps are made by appending a word to the
     source file name (e.g.  `foo.c.rtl' or `foo.c.jump').  Here are the
     possible letters for use in LETTERS, and their meanings:

    `b'
          Dump after computing branch probabilities, to `FILE.bp'.

    `c'
          Dump after instruction combination, to the file
          `FILE.combine'.

    `d'
          Dump after delayed branch scheduling, to `FILE.dbr'.

    `D'
          Dump all macro definitions, at the end of preprocessing, in
          addition to normal output.

    `r'
          Dump after RTL generation, to `FILE.rtl'.

    `j'
          Dump after first jump optimization, to `FILE.jump'.

    `F'
          Dump after purging ADDRESSOF, to `FILE.addressof'.

    `f'
          Dump after flow analysis, to `FILE.flow'.

    `g'
          Dump after global register allocation, to `FILE.greg'.

    `G'
          Dump after GCSE, to `FILE.gcse'.

    `j'
          Dump after first jump optimization, to `FILE.jump'.

    `J'
          Dump after last jump optimization, to `FILE.jump2'.

    `k'
          Dump after conversion from registers to stack, to
          `FILE.stack'.

    `l'
          Dump after local register allocation, to `FILE.lreg'.

    `L'
          Dump after loop optimization, to `FILE.loop'.

    `M'
          Dump after performing the machine dependent reorganisation
          pass, to `FILE.mach'.

    `N'
          Dump after the register move pass, to `FILE.regmove'.

    `r'
          Dump after RTL generation, to `FILE.rtl'.

    `R'
          Dump after the second instruction scheduling pass, to
          `FILE.sched2'.

    `s'
          Dump after CSE (including the jump optimization that
          sometimes follows CSE), to `FILE.cse'.

    `S'
          Dump after the first instruction scheduling pass, to
          `FILE.sched'.

    `t'
          Dump after the second CSE pass (including the jump
          optimization that sometimes follows CSE), to `FILE.cse2'.

    `a'
          Produce all the dumps listed above.

    `m'
          Print statistics on memory usage, at the end of the run, to
          standard error.

    `p'
          Annotate the assembler output with a comment indicating which
          pattern and alternative was used.  The length of each
          instruction is also printed.

    `x'
          Just generate RTL for a function instead of compiling it.
          Usually used with `r'.

    `y'
          Dump debugging information during parsing, to standard error.

    `A'
          Annotate the assembler output with miscellaneous debugging
          information.

`-fdump-unnumbered'
     When doing debugging dumps (see -d option above), suppress
     instruction numbers and line number note output.  This makes it
     more feasible to use diff on debugging dumps for compiler
     invokations with different options, in particular with and without
     -g.

`-fpretend-float'
     When running a cross-compiler, pretend that the target machine
     uses the same floating point format as the host machine.  This
     causes incorrect output of the actual floating constants, but the
     actual instruction sequence will probably be the same as GCC would
     make when running on the target machine.

`-save-temps'
     Store the usual "temporary" intermediate files permanently; place
     them in the current directory and name them based on the source
     file.  Thus, compiling `foo.c' with `-c -save-temps' would produce
     files `foo.i' and `foo.s', as well as `foo.o'.

`-print-file-name=LIBRARY'
     Print the full absolute name of the library file LIBRARY that
     would be used when linking--and don't do anything else.  With this
     option, GCC does not compile or link anything; it just prints the
     file name.

`-print-prog-name=PROGRAM'
     Like `-print-file-name', but searches for a program such as `cpp'.

`-print-libgcc-file-name'
     Same as `-print-file-name=libgcc.a'.

     This is useful when you use `-nostdlib' or `-nodefaultlibs' but
     you do want to link with `libgcc.a'.  You can do

          gcc -nostdlib FILES... `gcc -print-libgcc-file-name`

`-print-search-dirs'
     Print the name of the configured installation directory and a list
     of program and library directories gcc will search--and don't do
     anything else.

     This is useful when gcc prints the error message `installation
     problem, cannot exec cpp: No such file or directory'.  To resolve
     this you either need to put `cpp' and the other compiler
     components where gcc expects to find them, or you can set the
     environment variable `GCC_EXEC_PREFIX' to the directory where you
     installed them.  Don't forget the trailing '/'.  *Note Environment
     Variables::.


File: gcc.info,  Node: Optimize Options,  Next: Preprocessor Options,  Prev: Debugging Options,  Up: Invoking GCC

Options That Control Optimization
=================================

   These options control various sorts of optimizations:

`-O'
`-O1'
     Optimize.  Optimizing compilation takes somewhat more time, and a
     lot more memory for a large function.

     Without `-O', the compiler's goal is to reduce the cost of
     compilation and to make debugging produce the expected results.
     Statements are independent: if you stop the program with a
     breakpoint between statements, you can then assign a new value to
     any variable or change the program counter to any other statement
     in the function and get exactly the results you would expect from
     the source code.

     Without `-O', the compiler only allocates variables declared
     `register' in registers.  The resulting compiled code is a little
     worse than produced by PCC without `-O'.

     With `-O', the compiler tries to reduce code size and execution
     time.

     When you specify `-O', the compiler turns on `-fthread-jumps' and
     `-fdefer-pop' on all machines.  The compiler turns on
     `-fdelayed-branch' on machines that have delay slots, and
     `-fomit-frame-pointer' on machines that can support debugging even
     without a frame pointer.  On some machines the compiler also turns
     on other flags.

`-O2'
     Optimize even more.  GCC performs nearly all supported
     optimizations that do not involve a space-speed tradeoff.  The
     compiler does not perform loop unrolling or function inlining when
     you specify `-O2'.  As compared to `-O', this option increases
     both compilation time and the performance of the generated code.

     `-O2' turns on all optional optimizations except for loop
     unrolling, function inlining, and strict aliasing optimizations.
     It also turns on the `-fforce-mem' option on all machines and
     frame pointer elimination on machines where doing so does not
     interfere with debugging.

`-O3'
     Optimize yet more.  `-O3' turns on all optimizations specified by
     `-O2' and also turns on the `inline-functions' option.

`-O0'
     Do not optimize.

`-Os'
     Optimize for size.  `-Os' enables all `-O2' optimizations that do
     not typically increase code size.  It also performs further
     optimizations designed to reduce code size.

     If you use multiple `-O' options, with or without level numbers,
     the last such option is the one that is effective.

   Options of the form `-fFLAG' specify machine-independent flags.
Most flags have both positive and negative forms; the negative form of
`-ffoo' would be `-fno-foo'.  In the table below, only one of the forms
is listed--the one which is not the default.  You can figure out the
other form by either removing `no-' or adding it.

`-ffloat-store'
     Do not store floating point variables in registers, and inhibit
     other options that might change whether a floating point value is
     taken from a register or memory.

     This option prevents undesirable excess precision on machines such
     as the 68000 where the floating registers (of the 68881) keep more
     precision than a `double' is supposed to have.  Similarly for the
     x86 architecture.  For most programs, the excess precision does
     only good, but a few programs rely on the precise definition of
     IEEE floating point.  Use `-ffloat-store' for such programs, after
     modifying them to store all pertinent intermediate computations
     into variables.

`-fno-default-inline'
     Do not make member functions inline by default merely because they
     are defined inside the class scope (C++ only).  Otherwise, when
     you specify `-O', member functions defined inside class scope are
     compiled inline by default; i.e., you don't need to add `inline'
     in front of the member function name.

`-fno-defer-pop'
     Always pop the arguments to each function call as soon as that
     function returns.  For machines which must pop arguments after a
     function call, the compiler normally lets arguments accumulate on
     the stack for several function calls and pops them all at once.

`-fforce-mem'
     Force memory operands to be copied into registers before doing
     arithmetic on them.  This produces better code by making all memory
     references potential common subexpressions.  When they are not
     common subexpressions, instruction combination should eliminate
     the separate register-load.  The `-O2' option turns on this option.

`-fforce-addr'
     Force memory address constants to be copied into registers before
     doing arithmetic on them.  This may produce better code just as
     `-fforce-mem' may.

`-fomit-frame-pointer'
     Don't keep the frame pointer in a register for functions that
     don't need one.  This avoids the instructions to save, set up and
     restore frame pointers; it also makes an extra register available
     in many functions.  *It also makes debugging impossible on some
     machines.*

     On some machines, such as the Vax, this flag has no effect, because
     the standard calling sequence automatically handles the frame
     pointer and nothing is saved by pretending it doesn't exist.  The
     machine-description macro `FRAME_POINTER_REQUIRED' controls
     whether a target machine supports this flag.  *Note Registers::.

`-fno-inline'
     Don't pay attention to the `inline' keyword.  Normally this option
     is used to keep the compiler from expanding any functions inline.
     Note that if you are not optimizing, no functions can be expanded
     inline.

`-finline-functions'
     Integrate all simple functions into their callers.  The compiler
     heuristically decides which functions are simple enough to be worth
     integrating in this way.

     If all calls to a given function are integrated, and the function
     is declared `static', then the function is normally not output as
     assembler code in its own right.

`-finline-limit-N'
     By default, gcc limits the size of functions that can be inlined.
     This flag allows the control of this limit for functions that are
     explicitly marked as inline (ie marked with the inline keyword or
     defined within the class definition in c++).  N is the size of
     functions that can be inlined in number of pseudo instructions
     (not counting parameter handling).  The default value of n is
     10000.  Increasing this value can result in more inlined code at
     the cost of compilation time and memory consumption.  Decreasing
     usually makes the compilation faster and less code will be inlined
     (which presumably means slower programs).  This option is
     particularly useful for programs that use inlining heavily such as
     those based on recursive templates with c++.

     _Note:_ pseudo instruction represents, in this particular context,
     an abstract measurement of function's size.  In no way, it
     represents a count of assembly instructions and as such its exact
     meaning might change from one release to an another.

`-fkeep-inline-functions'
     Even if all calls to a given function are integrated, and the
     function is declared `static', nevertheless output a separate
     run-time callable version of the function.  This switch does not
     affect `extern inline' functions.

`-fkeep-static-consts'
     Emit variables declared `static const' when optimization isn't
     turned on, even if the variables aren't referenced.

     GCC enables this option by default.  If you want to force the
     compiler to check if the variable was referenced, regardless of
     whether or not optimization is turned on, use the
     `-fno-keep-static-consts' option.

`-fno-function-cse'
     Do not put function addresses in registers; make each instruction
     that calls a constant function contain the function's address
     explicitly.

     This option results in less efficient code, but some strange hacks
     that alter the assembler output may be confused by the
     optimizations performed when this option is not used.

`-ffast-math'
     This option allows GCC to violate some ANSI or IEEE rules and/or
     specifications in the interest of optimizing code for speed.  For
     example, it allows the compiler to assume arguments to the `sqrt'
     function are non-negative numbers and that no floating-point values
     are NaNs.

     This option should never be turned on by any `-O' option since it
     can result in incorrect output for programs which depend on an
     exact implementation of IEEE or ANSI rules/specifications for math
     functions.

   The following options control specific optimizations.  The `-O2'
option turns on all of these optimizations except `-funroll-loops'
`-funroll-all-loops', and `-fstrict-aliasing'.  On most machines, the
`-O' option turns on the `-fthread-jumps' and `-fdelayed-branch'
options, but specific machines may handle it differently.

   You can use the following flags in the rare cases when "fine-tuning"
of optimizations to be performed is desired.

`-fstrength-reduce'
     Perform the optimizations of loop strength reduction and
     elimination of iteration variables.

`-fthread-jumps'
     Perform optimizations where we check to see if a jump branches to a
     location where another comparison subsumed by the first is found.
     If so, the first branch is redirected to either the destination of
     the second branch or a point immediately following it, depending
     on whether the condition is known to be true or false.

`-fcse-follow-jumps'
     In common subexpression elimination, scan through jump instructions
     when the target of the jump is not reached by any other path.  For
     example, when CSE encounters an `if' statement with an `else'
     clause, CSE will follow the jump when the condition tested is
     false.

`-fcse-skip-blocks'
     This is similar to `-fcse-follow-jumps', but causes CSE to follow
     jumps which conditionally skip over blocks.  When CSE encounters a
     simple `if' statement with no else clause, `-fcse-skip-blocks'
     causes CSE to follow the jump around the body of the `if'.

`-frerun-cse-after-loop'
     Re-run common subexpression elimination after loop optimizations
     has been performed.

`-frerun-loop-opt'
     Run the loop optimizer twice.

`-fgcse'
     Perform a global common subexpression elimination pass.  This pass
     also performs global constant and copy propagation.

`-fexpensive-optimizations'
     Perform a number of minor optimizations that are relatively
     expensive.

`-foptimize-register-moves'
`-fregmove'
     Attempt to reassign register numbers in move instructions and as
     operands of other simple instructions in order to maximize the
     amount of register tying.  This is especially helpful on machines
     with two-operand instructions.  GCC enables this optimization by
     default with `-O2' or higher.

     Note `-fregmove' and `-foptimize-register-moves' are the same
     optimization.

`-fdelayed-branch'
     If supported for the target machine, attempt to reorder
     instructions to exploit instruction slots available after delayed
     branch instructions.

`-fschedule-insns'
     If supported for the target machine, attempt to reorder
     instructions to eliminate execution stalls due to required data
     being unavailable.  This helps machines that have slow floating
     point or memory load instructions by allowing other instructions
     to be issued until the result of the load or floating point
     instruction is required.

`-fschedule-insns2'
     Similar to `-fschedule-insns', but requests an additional pass of
     instruction scheduling after register allocation has been done.
     This is especially useful on machines with a relatively small
     number of registers and where memory load instructions take more
     than one cycle.

`-ffunction-sections'
`-fdata-sections'
     Place each function or data item into its own section in the output
     file if the target supports arbitrary sections.  The name of the
     function or the name of the data item determines the section's name
     in the output file.

     Use these options on systems where the linker can perform
     optimizations to improve locality of reference in the instruction
     space.  HPPA processors running HP-UX and Sparc processors running
     Solaris 2 have linkers with such optimizations.  Other systems
     using the ELF object format as well as AIX may have these
     optimizations in the future.

     Only use these options when there are significant benefits from
     doing so.  When you specify these options, the assembler and
     linker will create larger object and executable files and will
     also be slower.  You will not be able to use `gprof' on all
     systems if you specify this option and you may have problems with
     debugging if you specify both this option and `-g'.

`-fcaller-saves'
     Enable values to be allocated in registers that will be clobbered
     by function calls, by emitting extra instructions to save and
     restore the registers around such calls.  Such allocation is done
     only when it seems to result in better code than would otherwise
     be produced.

     This option is always enabled by default on certain machines,
     usually those which have no call-preserved registers to use
     instead.

     For all machines, optimization level 2 and higher enables this
     flag by default.

`-funroll-loops'
     Perform the optimization of loop unrolling.  This is only done for
     loops whose number of iterations can be determined at compile time
     or run time.  `-funroll-loops' implies both `-fstrength-reduce' and
     `-frerun-cse-after-loop'.

`-funroll-all-loops'
     Perform the optimization of loop unrolling.  This is done for all
     loops and usually makes programs run more slowly.
     `-funroll-all-loops' implies `-fstrength-reduce' as well as
     `-frerun-cse-after-loop'.

`-fmove-all-movables'
     Forces all invariant computations in loops to be moved outside the
     loop.

`-freduce-all-givs'
     Forces all general-induction variables in loops to be
     strength-reduced.

     _Note:_ When compiling programs written in Fortran,
     `-fmove-all-movables' and `-freduce-all-givs' are enabled by
     default when you use the optimizer.

     These options may generate better or worse code; results are highly
     dependent on the structure of loops within the source code.

     These two options are intended to be removed someday, once they
     have helped determine the efficacy of various approaches to
     improving loop optimizations.

     Please let us (`gcc@gcc.gnu.org' and `fortran@gnu.org') know how
     use of these options affects the performance of your production
     code.  We're very interested in code that runs _slower_ when these
     options are _enabled_.

`-fno-peephole'
     Disable any machine-specific peephole optimizations.

`-fbranch-probabilities'
     After running a program compiled with `-fprofile-arcs' (*note
     Options for Debugging Your Program or `gcc': Debugging Options.),
     you can compile it a second time using `-fbranch-probabilities',
     to improve optimizations based on guessing the path a branch might
     take.

     With `-fbranch-probabilities', GCC puts a `REG_EXEC_COUNT' note on
     the first instruction of each basic block, and a `REG_BR_PROB'
     note on each `JUMP_INSN' and `CALL_INSN'.  These can be used to
     improve optimization.  Currently, they are only used in one place:
     in `reorg.c', instead of guessing which path a branch is mostly to
     take, the `REG_BR_PROB' values are used to exactly determine which
     path is taken more often.

`-fstrict-aliasing'
     Allows the compiler to assume the strictest aliasing rules
     applicable to the language being compiled.  For C (and C++), this
     activates optimizations based on the type of expressions.  In
     particular, an object of one type is assumed never to reside at
     the same address as an object of a different type, unless the
     types are almost the same.  For example, an `unsigned int' can
     alias an `int', but not a `void*' or a `double'.  A character type
     may alias any other type.

     Pay special attention to code like this:
          union a_union {
            int i;
            double d;
          };
          
          int f() {
            a_union t;
            t.d = 3.0;
            return t.i;
          }
     The practice of reading from a different union member than the one
     most recently written to (called "type-punning") is common.  Even
     with `-fstrict-aliasing', type-punning is allowed, provided the
     memory is accessed through the union type.  So, the code above
     will work as expected.  However, this code might not:
          int f() {
            a_union t;
            int* ip;
            t.d = 3.0;
            ip = &t.i;
            return *ip;
          }

     Every language that wishes to perform language-specific alias
     analysis should define a function that computes, given an `tree'
     node, an alias set for the node.  Nodes in different alias sets
     are not allowed to alias.  For an example, see the C front-end
     function `c_get_alias_set'.


File: gcc.info,  Node: Preprocessor Options,  Next: Assembler Options,  Prev: Optimize Options,  Up: Invoking GCC

Options Controlling the Preprocessor
====================================

   These options control the C preprocessor, which is run on each C
source file before actual compilation.

   If you use the `-E' option, nothing is done except preprocessing.
Some of these options make sense only together with `-E' because they
cause the preprocessor output to be unsuitable for actual compilation.

`-include FILE'
     Process FILE as input before processing the regular input file.
     In effect, the contents of FILE are compiled first.  Any `-D' and
     `-U' options on the command line are always processed before
     `-include FILE', regardless of the order in which they are
     written.  All the `-include' and `-imacros' options are processed
     in the order in which they are written.

`-imacros FILE'
     Process FILE as input, discarding the resulting output, before
     processing the regular input file.  Because the output generated
     from FILE is discarded, the only effect of `-imacros FILE' is to
     make the macros defined in FILE available for use in the main
     input.

     Any `-D' and `-U' options on the command line are always processed
     before `-imacros FILE', regardless of the order in which they are
     written.  All the `-include' and `-imacros' options are processed
     in the order in which they are written.

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
     specified previously with `-iprefix'.  If you have not specified a
     prefix yet, the directory containing the installed passes of the
     compiler is used as the default.

`-iwithprefixbefore DIR'
     Add a directory to the main include path.  The directory's name is
     made by concatenating PREFIX and DIR, as in the case of
     `-iwithprefix'.

`-isystem DIR'
     Add a directory to the beginning of the second include path,
     marking it as a system directory, so that it gets the same special
     treatment as is applied to the standard system directories.

`-nostdinc'
     Do not search the standard system directories for header files.
     Only the directories you have specified with `-I' options (and the
     current directory, if appropriate) are searched.  *Note Directory
     Options::, for information on `-I'.

     By using both `-nostdinc' and `-I-', you can limit the include-file
     search path to only those directories you specify explicitly.

`-undef'
     Do not predefine any nonstandard macros.  (Including architecture
     flags).

`-E'
     Run only the C preprocessor.  Preprocess all the C source files
     specified and output the results to standard output or to the
     specified output file.

`-C'
     Tell the preprocessor not to discard comments.  Used with the `-E'
     option.

`-P'
     Tell the preprocessor not to generate `#line' directives.  Used
     with the `-E' option.

`-M'
     Tell the preprocessor to output a rule suitable for `make'
     describing the dependencies of each object file.  For each source
     file, the preprocessor outputs one `make'-rule whose target is the
     object file name for that source file and whose dependencies are
     all the `#include' header files it uses.  This rule may be a
     single line or may be continued with `\'-newline if it is long.
     The list of rules is printed on standard output instead of the
     preprocessed C program.

     `-M' implies `-E'.

     Another way to specify output of a `make' rule is by setting the
     environment variable `DEPENDENCIES_OUTPUT' (*note Environment
     Variables::).

`-MM'
     Like `-M' but the output mentions only the user header files
     included with `#include "FILE"'.  System header files included
     with `#include <FILE>' are omitted.

`-MD'
     Like `-M' but the dependency information is written to a file made
     by replacing ".c" with ".d" at the end of the input file names.
     This is in addition to compiling the file as specified--`-MD' does
     not inhibit ordinary compilation the way `-M' does.

     In Mach, you can use the utility `md' to merge multiple dependency
     files into a single dependency file suitable for using with the
     `make' command.

`-MMD'
     Like `-MD' except mention only user header files, not system
     header files.

`-MG'
     Treat missing header files as generated files and assume they live
     in the same directory as the source file.  If you specify `-MG',
     you must also specify either `-M' or `-MM'.  `-MG' is not
     supported with `-MD' or `-MMD'.

`-H'
     Print the name of each header file used, in addition to other
     normal activities.

`-AQUESTION(ANSWER)'
     Assert the answer ANSWER for QUESTION, in case it is tested with a
     preprocessing conditional such as `#if #QUESTION(ANSWER)'.  `-A-'
     disables the standard assertions that normally describe the target
     machine.

`-DMACRO'
     Define macro MACRO with the string `1' as its definition.

`-DMACRO=DEFN'
     Define macro MACRO as DEFN.  All instances of `-D' on the command
     line are processed before any `-U' options.

`-UMACRO'
     Undefine macro MACRO.  `-U' options are evaluated after all `-D'
     options, but before any `-include' and `-imacros' options.

`-dM'
     Tell the preprocessor to output only a list of the macro
     definitions that are in effect at the end of preprocessing.  Used
     with the `-E' option.

`-dD'
     Tell the preprocessing to pass all macro definitions into the
     output, in their proper sequence in the rest of the output.

`-dN'
     Like `-dD' except that the macro arguments and contents are
     omitted.  Only `#define NAME' is included in the output.

`-trigraphs'
     Support ANSI C trigraphs.  The `-ansi' option also has this effect.

`-Wp,OPTION'
     Pass OPTION as an option to the preprocessor.  If OPTION contains
     commas, it is split into multiple options at the commas.


File: gcc.info,  Node: Assembler Options,  Next: Link Options,  Prev: Preprocessor Options,  Up: Invoking GCC

Passing Options to the Assembler
================================

   You can pass options to the assembler.

`-Wa,OPTION'
     Pass OPTION as an option to the assembler.  If OPTION contains
     commas, it is split into multiple options at the commas.


File: gcc.info,  Node: Link Options,  Next: Directory Options,  Prev: Assembler Options,  Up: Invoking GCC

Options for Linking
===================

   These options come into play when the compiler links object files
into an executable output file.  They are meaningless if the compiler is
not doing a link step.

`OBJECT-FILE-NAME'
     A file name that does not end in a special recognized suffix is
     considered to name an object file or library.  (Object files are
     distinguished from libraries by the linker according to the file
     contents.)  If linking is done, these object files are used as
     input to the linker.

`-c'
`-S'
`-E'
     If any of these options is used, then the linker is not run, and
     object file names should not be used as arguments.  *Note Overall
     Options::.

`-lLIBRARY'
     Search the library named LIBRARY when linking.

     It makes a difference where in the command you write this option;
     the linker searches processes libraries and object files in the
     order they are specified.  Thus, `foo.o -lz bar.o' searches
     library `z' after file `foo.o' but before `bar.o'.  If `bar.o'
     refers to functions in `z', those functions may not be loaded.

     The linker searches a standard list of directories for the library,
     which is actually a file named `libLIBRARY.a'.  The linker then
     uses this file as if it had been specified precisely by name.

     The directories searched include several standard system
     directories plus any that you specify with `-L'.

     Normally the files found this way are library files--archive files
     whose members are object files.  The linker handles an archive
     file by scanning through it for members which define symbols that
     have so far been referenced but not defined.  But if the file that
     is found is an ordinary object file, it is linked in the usual
     fashion.  The only difference between using an `-l' option and
     specifying a file name is that `-l' surrounds LIBRARY with `lib'
     and `.a' and searches several directories.

`-lobjc'
     You need this special case of the `-l' option in order to link an
     Objective C program.

`-nostartfiles'
     Do not use the standard system startup files when linking.  The
     standard system libraries are used normally, unless `-nostdlib' or
     `-nodefaultlibs' is used.

`-nodefaultlibs'
     Do not use the standard system libraries when linking.  Only the
     libraries you specify will be passed to the linker.  The standard
     startup files are used normally, unless `-nostartfiles' is used.
     The compiler may generate calls to memcmp, memset, and memcpy for
     System V (and ANSI C) environments or to bcopy and bzero for BSD
     environments.  These entries are usually resolved by entries in
     libc.  These entry points should be supplied through some other
     mechanism when this option is specified.

`-nostdlib'
     Do not use the standard system startup files or libraries when
     linking.  No startup files and only the libraries you specify will
     be passed to the linker. The compiler may generate calls to
     memcmp, memset, and memcpy for System V (and ANSI C) environments
     or to bcopy and bzero for BSD environments.  These entries are
     usually resolved by entries in libc.  These entry points should be
     supplied through some other mechanism when this option is
     specified.

     One of the standard libraries bypassed by `-nostdlib' and
     `-nodefaultlibs' is `libgcc.a', a library of internal subroutines
     that GCC uses to overcome shortcomings of particular machines, or
     special needs for some languages.  (*Note Interfacing to GCC
     Output: Interface, for more discussion of `libgcc.a'.)  In most
     cases, you need `libgcc.a' even when you want to avoid other
     standard libraries.  In other words, when you specify `-nostdlib'
     or `-nodefaultlibs' you should usually specify `-lgcc' as well.
     This ensures that you have no unresolved references to internal GCC
     library subroutines.  (For example, `__main', used to ensure C++
     constructors will be called; *note `collect2': Collect2..)

`-s'
     Remove all symbol table and relocation information from the
     executable.

`-static'
     On systems that support dynamic linking, this prevents linking
     with the shared libraries.  On other systems, this option has no
     effect.

`-shared'
     Produce a shared object which can then be linked with other
     objects to form an executable.  Not all systems support this
     option.  You must also specify `-fpic' or `-fPIC' on some systems
     when you specify this option.

`-symbolic'
     Bind references to global symbols when building a shared object.
     Warn about any unresolved references (unless overridden by the
     link editor option `-Xlinker -z -Xlinker defs').  Only a few
     systems support this option.

`-Xlinker OPTION'
     Pass OPTION as an option to the linker.  You can use this to
     supply system-specific linker options which GCC does not know how
     to recognize.

     If you want to pass an option that takes an argument, you must use
     `-Xlinker' twice, once for the option and once for the argument.
     For example, to pass `-assert definitions', you must write
     `-Xlinker -assert -Xlinker definitions'.  It does not work to write
     `-Xlinker "-assert definitions"', because this passes the entire
     string as a single argument, which is not what the linker expects.

`-Wl,OPTION'
     Pass OPTION as an option to the linker.  If OPTION contains
     commas, it is split into multiple options at the commas.

`-u SYMBOL'
     Pretend the symbol SYMBOL is undefined, to force linking of
     library modules to define it.  You can use `-u' multiple times with
     different symbols to force loading of additional library modules.

