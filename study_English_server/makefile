Vo_src = $(wildcard ./vo_src/*.cpp)
Bo_src = $(wildcard ./bo_src/*.cpp)
Dao_src = $(wildcard ./dao_src/*.cpp)
Socket_src = $(wildcard ./socket_src/*.cpp)
Util_src = $(wildcard ./util_src/*.cpp)
Database_src = $(wildcard ./database_src/*.cpp)
Main_src = $(wildcard *.cpp)
SrcFiles = $(Main_src) $(Socket_src) $(Util_src) $(Database_src) $(Vo_src) $(Bo_src) $(Dao_src)
O_Files = $(patsubst %.cpp, %.o, $(SrcFiles))

Vo_include = ./vo_include
Bo_include = ./bo_include
Dao_include = ./dao_include
Socket_include = ./socket_include
Util_include = ./util_include
Database_include = ./database_include
project_include_path = -I$(Socket_include) -I$(Util_include) -I$(Database_include) -I$(Vo_include) -I$(Bo_include) -I$(Dao_include)

CPP = g++
CPPFLAGS = -Wall -g -std=c++11
Include_Path = /usr/include/mysql/
Lib_Path = /usr/lib64/mysql/
Lib_Files = mysqlclient
Public_Flags = -I$(Include_Path) -L$(Lib_Path) -l$(Lib_Files)
Target = study_English_server

$(Target):$(O_Files)
		$(CPP) $(O_Files) -o $@ $(CPPFLAGS) $(Public_Flags) $(project_include_path)

%.o:%.cpp
	$(CPP) $< -c -o $@ $(CPPFLAGS) $(Public_Flags) $(project_include_path)

.PHONY:clean target
clean:
	rm -rf $(Target)

.PHONY:clean binary file
clean:
	rm -rf $(O_Files)
