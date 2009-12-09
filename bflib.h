
#ifndef BFLIB
#define BFLIB


/*** RULES ***/
// The data pointer always points to the first free cell of the stack (0 at start, then)
// Stack starts at zero and goes up into the positives. '>' goes up the stack 1 byte ('>' would be 'push 0')
// Operations assume the unused stack is clean (zeroed)
// Operations must clean the unused stack (zero it)
// Operations on data types more than 1 byte are suffixed with _X, X being the type size
// Operations take the operands, destroy them (pop them) and push the result
/*** END OF RULES ***/


/*** NON stack-compliant functions ***/
#define TORIGHT   "[>+<-]"
#define TOLEFT    "[<+>-]"
#define TO2RIGHT  "[>>+<<-]"
#define TO2LEFT   "[<<+>>-]"
#define TO4RIGHT  "[>>>>+<<<<-]"
#define TO4LEFT   "[<<<<+>>>>-]"
#define TO8RIGHT  "[>>>>>>>>+<<<<<<<<-]"
#define TO8LEFT   "[<<<<<<<<+>>>>>>>>-]"
#define TO12RIGHT "[>>>>>>>>>>>>+<<<<<<<<<<<<-]"
#define TO12LEFT  "[<<<<<<<<<<<<+>>>>>>>>>>>>-]"
#define Z "[-]"
/*** End of NON stack-compliant functions ***/


/*** Stack-compliant functions ***/

#define PUSH(a) a ">"
#define POP "<" Z

#define ADD111 "add <[<+>-] end add "
#define SUB "sub <[<->-] end sub "
#define DUP "dup <[>+>+<<-]>>[<<+>>-] end dup "
#define DUP_2 "dup_2 <<[>>+>>+<<<<-]>[>>+>>+<<<<-]>>>[<<<<+>>>>-]>[<<<<+>>>>-]< end dup_2 "
#define SWAP "swap <<[>>+<<-]>" TOLEFT ">" TOLEFT " end swap "

// If stack top is zero, remove it and push one
#define ISZERO "iszero +<[>[-]<[-]]> " TOLEFT " end iszero "
// If stack top is non-zero, remove it and push one
#define ISNOTZERO "isnotzero <[>+<[-]]>" TOLEFT " end isnotzero "

#define ISEQUAL    "isequal " SUB ISZERO " end isequal "
#define ISNOTEQUAL "isnotequal " SUB ISNOTZERO "end isnotequal "

#define ISLOWERTHAN   "islowerthan <<[->[-" TORIGHT "]>" TOLEFT "<<]>" TOLEFT ISNOTZERO "end islowerthan "
#define ISGREATERTHAN "isgreaterthan " SWAP ISLOWERTHAN "end isgreaterthan "

#define ISGREATEROREQ "isgreateroreq " ISLOWERTHAN ISZERO "end isgreateroreq "
#define ISLOWEROREQ   "isloweroreq " SWAP ISGREATEROREQ "end isloweroreq "

#define IF(cond,then) "if " cond "<[[-]" then "] end if "
#define IFTHENELSE(cond,then,doelse) "ifthenelse "cond DUP ISZERO SWAP \
                                    "<[[-]<then@" then ">] <[[-]else@" doelse "] end ifthenelse "

#define AND "and <[" Z "<[" Z ">>+<<]>]" Z "<" Z ">>[-<<+>>]< end and "
#define OR  "or  <[" Z "<" Z "+>]" ISNOTZERO " end or "

#define MUL111   "mul111   <<[->[>+>+<<-]>[<+>-]<<]>>>[<<<+>>>-]<<[-] end mul111 "
#define MUL112 "mul112 <<[->[>+>>+" \
	">" DUP IF(ISZERO,"<<+>>") "<" \
	"<<<-]>[<+>-]<<]" \
	">>>[<<<+>>>-]<<[-]>>>[<<<+>>>-]<< end mul112 "

#define DIV "div "DUP IF(ISZERO,"<<" Z ">>") \
	"<" TORIGHT "<" TORIGHT ">"  \
	"[>>" DUP_2 IF(ISGREATEROREQ,"<<<+>>>") \
	DUP_2 SUB DUP "<<<<[->>>>+<<<<]>>>>>" DUP "<[-<<<<<+>>>>>]" \
	IF(ISGREATERTHAN,"<" Z ">") \
	"<<<" Z ">>><[-<<+>>]<<" \
	"]>" Z "< end div "

#define REMAINDER "remainder "DUP IF(ISZERO,"<<" Z ">>") \
	"<" TORIGHT "<" TORIGHT ">"  \
	"[>>" DUP_2 SUB DUP "<<<<[->>>>+<<<<]>>>>>" DUP "<[-<<<<<+>>>>>]" \
	IF(ISGREATERTHAN,"<<<[-<+>]>>" Z ">") \
	"<<<" Z ">>><[-<<+>>]<<" \
	"]>" Z "< end remainder "

#define P "<." Z
#define PRINTDIGIT "printdigit " PUSH("++++++") PUSH("++++++++") MUL111 ADD111 P " end printdigit "
#define PRINTDEC   "printdec " DUP PUSH("++++++++++") DIV DUP \
	PUSH("++++++++++") DIV PRINTDIGIT \
	PUSH("++++++++++") REMAINDER PRINTDIGIT \
	PUSH("++++++++++") REMAINDER PRINTDIGIT "end printdec "

#define GETCH ","

#define ADD222 "add222 " DUP DUP "<<<<<[->>>>>+<<<<<]>>>>>>" ADD111 \
	DUP "<[-<<<<<+>>>>>]" IF(ISGREATERTHAN,"<<<<+>>>>") \
	POP "<[-<<+>>] end add222 "

#define ADD223 "add223 " DUP DUP "<<<<<[->>>>>+<<<<<]>>>>>>" ADD111 \
	DUP "<[-<<<<<+>>>>>]" IF(ISGREATERTHAN,"<<<<+>>>>") \
	POP "<" TORIGHT "<" TORIGHT "<" TORIGHT ">>>>" \
	DUP "<[-<<<+>>>] <<<[->>>+>+<<<<]>>>>[-<<<<+>>>>] "\
	IF(ISGREATERTHAN,"<<<+>>>") \
	"end add223 "

#define SUB_2 "sub_2 " "<<<[->>>+<<<]>>>>" DUP \
	"<<<[->>>+<<<]>>>>" DUP "<[-<<<<+>>>>]" \
	SUB DUP "<[-<<<<<+>>>>>]" IF(ISLOWERTHAN"","<<<<->>>>") \
	POP "<[-<<->>] end sub_2 "

#define SWAP_2 "swap_2 <[->>+<<]<[->>+<<]<[->>+<<]<[->>+<<]" \
	">>>>[-<<<<+>>>>]>[-<<<<+>>>>]< end swap_2 "

#define ISZERO_2 "iszero_2 +<[>[-]<[-]]<[>>[-]<<[-]]>>[-<<+>>]< end iszero_2 "
#define ISNOTZERO_2 "isnotzero_2 <[[-]>+<]<[[-]>>[-]+<<]>>[-<<+>>]< end isnotzero_2 "

#define ISEQUAL_2    "isequal_2 " SUB_2 ISZERO_2 " end isequal_2 "
#define ISNOTEQUAL_2 "isnotequal_2 " SUB_2 ISNOTZERO_2 "end isnotequal_2 "

#define DUPFROM3 "dupfrom3 <<<[->>>+>+<<<<]>>>>[-<<<<+>>>>] end dupfrom3 "
#define DUPFROM4 "dupfrom4 <<<<[->>>>+>+<<<<<]>>>>>[-<<<<<+>>>>>] end dupfrom4 "

#define ISLOWERTHAN_2 "islowerthan_2 <<<<[->>>>+<<<<]> " TOLEFT \
	">[->>>+<<<]> " TOLEFT "< " TOLEFT ">> " TOLEFT \
	"< " TOLEFT ">> " TOLEFT "< " TOLEFT "@" \
	IFTHENELSE(DUP_2 "@" ISLOWERTHAN,"@" POP POP "@<<[-]>[-]+>@","@" IF(ISGREATERTHAN,"@<<[-]+>[-]>@") ) \
	ISLOWERTHAN

#define ISGREATERTHAN_2 "isgreaterthan_2 " SWAP_2 ISLOWERTHAN_2 "end isgreaterthan_2 "

#define ISGREATEROREQ_2 "isgreateroreq_2 " ISLOWERTHAN_2 ISZERO_2 "end isgreateroreq_2 "
#define ISLOWEROREQ_2   "isloweroreq_2 " SWAP_2 ISGREATEROREQ_2 "end isloweroreq_2 "

#define CAST24 "cast24 <" TO2RIGHT "< " TO2RIGHT ">>>> end cast24 "

#define MUL222   "mul222 < " TO2RIGHT \
	"< " TO2RIGHT \
	"< " TO2RIGHT \
	"< " TO2RIGHT ">>>>>> " \
	DUP "<<<<[->>>>+>+<<<<<]>>>>>[-<<<<<+>>>>>] " MUL112 \
	"<[-<<<<<<+>>>>>>]<[-<<<<<<+>>>>>>] " \
	DUP "<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>] " MUL111 \
	"<[-<<<<<<+>>>>>>]" \
	POP MUL111 "<[-<<<+>>>] " POP "end mul222 "

#define MUL224 "mul224 < " TO4RIGHT \
	"< " TO4RIGHT \
	"< " TO4RIGHT \
	"< " TO4RIGHT ">>>>>>>> " \
	DUP "<<<<[->>>>+>+<<<<<]>>>>>[-<<<<<+>>>>>] " MUL112 \
	"<[-<<<<<<+>>>>>>]<[-<<<<<<+>>>>>>] " \
	DUP "<<<<<[->>>>>+>+<<<<<<]>>>>>>[-<<<<<<+>>>>>>] " MUL112 \
	"end mul224 "

#define PRINTDEC_2

#endif // BFLIB
