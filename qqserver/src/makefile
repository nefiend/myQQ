.SUFFIXES:.cpp .o        #.c和.o文件有关系

CC=g++
SRCS1=qqserver.cpp\
	pub.cpp\
	work.cpp
OBJS1=$(SRCS1:.cpp=.o)
EXEC1=qqserverd


start:$(OBJS1) 
	$(CC) -o $(EXEC1) $(OBJS1) 
	@echo "----------------------ok------------"
.c.o:
	$(CC) -Wall -g -o $@ -c $<
clean:
	rm -f $(OBJS1)
	rm -f core*
