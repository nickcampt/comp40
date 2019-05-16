#ifndef EXECUTION_H
#define EXECUTION_H

struct data {
        uint32_t registers[9];
}

/* returns false when there is a failure */
bool run();

void set_registers(uint32_t word);

void interpret();

void run_machine();

void halt();


#endif
