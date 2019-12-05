@echo --------------------------------Build------------------------------------
win_flex -olexer.cpp icpp.l 
g++ -static-libgcc -static-libstdc++ -oicpp main.cpp lexer.cpp
@echo --------------------------------Done-------------------------------------

 
