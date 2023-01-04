# Shortcut for Layout Next
define ln
    layout next
end

# Shortcut for Focus Next
define fn
    focus next
end

# Kill current program and exit debugger
define killq
    kill
    q
end

# Set AArch options
set debug aarch64
show debug aarch64

# Set Debug target
target remote localhost:1234
symbol-file out/myos.elf

# Setup terminal UI.
tui enable
layout regs
tui reg general

# Set breakpoints
break kernel_main
break after_stack_change
break kernel_handoff
break exc_handler

# Start the program up until the first breakpoint ( _start )
continue