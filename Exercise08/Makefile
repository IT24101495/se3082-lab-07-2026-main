CC = mpicc
CFLAGS = -O2 -Wall -Wextra
PROGRAMS = sum_bcast sum_scatter sum_gather sum_reduce sum_allreduce sum_scan

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

run: all
	@for program in $(PROGRAMS); do \
		echo "===== $$program ====="; \
		mpirun -np 4 ./$$program || exit 1; \
	done

clean:
	rm -f $(PROGRAMS)

.PHONY: all run clean
