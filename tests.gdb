echo ======================================================\n
echo Running all tests..."\n\n

# Example test:
test "PINA: 0x00 => PORTB: 0x00, state: Init"
set state = Start
setPINA 0x00
continue 1
expectPORTB 0x00
expect state Init
checkResult

test "PINA: 0x03 => PORTB: 0x00, state: Init"
set state = Start
setPINA 0x03
continue 1
expectPORTB 0x00
expect state Init
checkResult

test "PINA: 0x01 => PORTB: 0x01, state: WaterCheck"
set state = Start
setPINA 0x01
continue 2
expectPORTB 0x01
expect state WaterCheck
checkResult

test "PINA: 0x01 => PORTB: 0x01, state: WaterCheck"
set state = Start
setPINA 0x01
continue 2
expectPORTB 0x01
expect state WaterCheck
checkResult

test "PINA: 0x02 => PORTB: 0x01, state: MakeCoffee"
set state = WaterCheck
setPINA 0x02
continue 2
expectPORTB 0x01
expect state MakeCoffee
checkResult

test "PINA: 0x01 => PORTB: 0x01, state: WaterCheck"
set state = WaterCheck
setPINA 0x01
continue 2
expectPORTB 0x01
expect state WaterCheck
checkResult

test "PINA: 0x00 => PORTB: 0x01, state: Init"
set state = WaterCheck
setPINA 0x00
continue 11
expectPORTB 0x00
expect state Init
checkResult

test "PINA: 0x00 => PORTB: 0x01, state: Done"
set state = MakeCoffee
setPINA 0x00
continue 180
expectPORTB 0x02
expect state Done
checkResult

test "PINA: 0x04 => PORTB: 0x01, state: MakeCoffee"
set state = MakeCoffee
setPINA 0x04
continue 180
expectPORTB 0x01
expect state MakeCoffee
checkResult

test "PINA: 0x08 => PORTB: 0x01, state: Done"
set state = MakeCoffee
setPINA 0x08
continue 170
expectPORTB 0x02
expect state Done
checkResult

test "PINA: 0x04, 0x04, 0x04, 0x04 => PORTB: 0x01, state: MakeCoffee"
set state = MakeCoffee
setPINA 0x04
continue 1
setPINA 0x04
continue 1
setPINA 0x04
continue 1
setPINA 0x04
continue 200
expectPORTB 0x01
expect state MakeCoffee
checkResult

test "PINA: 0x00 => PORTB: 0x01, state: Init"
set state = Done
setPINA 0x00
continue 11
expectPORTB 0x00
expect state Init
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
