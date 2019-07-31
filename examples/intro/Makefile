include ./Makefile.conf

.DEFAULT_GOAL=runtests

.PHONY : runtests
runtests: intro
	time ./intro

.PHONY : clean very-clean
clean:
	rm -rf *.o *.out intro

very-clean: clean
