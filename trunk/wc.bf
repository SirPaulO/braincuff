
#define INC_CHAR_COUNT "<<<[>>>>+<<<<-]<[>>>>+<<<<-]>>>>>+>" DUP \
	IF(ISZERO,"<<+>>") \
	"<[<<<<+>>>>-]<[<<<<+>>>>-]"


#define MAIN2 \
	">>" /*two bytes for lines*/ \
	">>" /*two bytes for words*/ \
	">>" /*two bytes for chars*/ \
	"+>" /*one flag for 'line empty so far'*/ \
	"" GETCH "----[++++> main loop" \
	"" DUP PUSH("++++++++++") IF(ISEQUAL,"<<" Z "+>>") /*check for new line*/ \
	"" DUP PUSH("++++")PUSH("++++++++")MUL "<->" IF(ISGREATERTHAN,INC_CHAR_COUNT) /*check for char*/ \
	"" POP /*discard char*/ \
	"" GETCH "----]" POP " end main loop " \
	"@" PRINTDEC

#define MAIN "->->>->@" MUL224 "@"
