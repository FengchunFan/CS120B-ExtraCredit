echo ======================================================\n
echo Running all tests..."\n\n

# Example test:
test "PINA: 0x00 => PORTC: 0x07, state: Press"
set state = Start
setPINA 0x00
continue 2
expectPORTC 0x07
expect state Press
checkResult

# Example test:
test "PINA: 0x00, 0x01 => PORTC: 0x08, state: Increment"
set state = Start
setPINA 0x00
continue 2
setPINA 0x01
continue 2
expectPORTC 0x08
expect state Increment
checkResult

test "PINA: 0x00, 0x01, 0x02 => PORTC: 0x07, state: Decrement"
set state = Start
setPINA 0x00
continue 2
setPINA 0x01
continue 2
setPINA 0x02
continue 2
expectPORTC 0x07
expect state Decrement
checkResult

test "PINA: 0x00, 0x01, 0x02, 0x03 => PORTC: 0x00, state: Reset"
set state = Start
setPINA 0x00
continue 2
setPINA 0x01
continue 2
setPINA 0x02
continue 2
setPINA 0x03
continue2
expectPORTC 0x00
expect state Reset
checkResult

test "PINA: 0x00, 0x01, 0x01, 0x01 => PORTC: 0x09, state: Increment"
set state = Start
setPINA 0x00
continue 2
setPINA 0x01
continue 2
setPINA 0x01
continue 2
setPINA 0x01
continue 2
expectPORTC 0x09
expect state Increment
checkResult

# Add tests below

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
