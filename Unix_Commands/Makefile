dir: cmd_parser.o
		 gcc 4_File_IO/dir.c cmd_parser.o -o dir.o

stat: cmd_parser.o
			gcc 4_File_IO/stat.c cmd_parser.o -o stat.o

touch: cmd_parser.o
			gcc 4_File_IO/touch.c cmd_parser.o -o touch.o

cat:	cmd_parser.o
			gcc 4_File_IO/cat.c cmd_parser.o -o cat.o

cmd_parser.o: 
						gcc -c util/cmd_parser.c

clean:
			rm -rf *.o*  
