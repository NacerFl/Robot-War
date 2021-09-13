all:
	flex -v BURP.flex
	bison --debug --verbose -d BURP.y
	gcc Arena.c -c -o prog1
	gcc Window.c -c -o prog2
	gcc main.c -c -o prog3
	gcc BURP.tab.c -c -o prog4
	gcc lex.yy.c -c -o prog5
	gcc Missile.c -c -o prog6
	gcc Robot.c -c -o prog7
	gcc prog1 prog2 prog3 prog4 prog5 prog6 prog7 -o burp -lm -lncurses
