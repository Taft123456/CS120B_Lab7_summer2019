# Test sequence from START: A0 && A1 => PORTC: 0x00
test "PINA: 0x03 => PORTC: 0x00, state: RESET"
set state = START
setPINA 0x03
continue 2
expectPORTC 0x00
checkResult

# Test sequence from START: A0 && A1, no input => PORTC: 0x00
test "PINA: 0x03, 0x00 => PORTC: 0x00, state: RESET"
set state = START
setPINA 0x03
continue 2
setPINA 0x00
continue 1
expectPORTC 0x00
checkResult

# Test sequence from START: A0 => PORTC: 0x00
test "PINA: 0x01 => PORTC: 0x00, state: WAIT"
set state = START
setPINA 0x01
continue 2
expectPORTC 0x00
checkResult

# Test sequence from START: A1 => PORTC: 0x00
test "PINA: 0x02 => PORTC: 0x00, state: WAIT"
set state = START
setPINA 0x02
continue 2
expectPORTC 0x00
checkResult