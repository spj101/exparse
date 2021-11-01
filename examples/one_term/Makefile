include ./Makefile.conf

.DEFAULT_GOAL=runtests

.PHONY : runtests
runtests: one_term
	time ./one_term

.PHONY : clean very-clean
clean:
	rm -rf *.o *.out one_term

very-clean: clean
