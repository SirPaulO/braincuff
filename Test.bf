

#define MAIN \
	PRINTABLE(PUSH(+++++)) \
	PRINTABLE(PUSH(++)) \
	PRINTABLE(DIV) \
	PRINTABLE(PUSH(++)) \
	PRINTABLE(IFTHENELSE(ISEQUAL,PUSH(++++++++++)PUSH(++++++++++), \
	                             PUSH(+++)PUSH(+++)) \
	         ) \
	PRINTABLE(MUL PRINTDEC)
