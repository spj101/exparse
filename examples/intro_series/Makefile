include ./Makefile.conf

.DEFAULT_GOAL=runtests

.PHONY : runtests
runtests: intro_series
	time ./intro_series

.PHONY : clean very-clean
clean:
	rm -rf *.o *.out intro_series

very-clean: clean
