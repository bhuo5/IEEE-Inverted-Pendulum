# REMARKS: Pico starts registering wrong pulse count when encoder is spun even mildly fast
# If anyone sees any inefficiencies then plz post in chat
# My theory is MicroPython is too slow for this application
# Getting the angle in degrees barely worked unless you spun encoder at under 5 rpm
# I don't think noise is the issue since encoder works perfectly fine at slow speeds
# SUGGESTION: we might need to try C
# We probably can't pull off PID calculations if getting the angle is this slow

from machine import Pin

out_a = Pin(0, Pin.IN, Pin.PULL_DOWN)
out_b = Pin(1, Pin.IN, Pin.PULL_DOWN)
pulses = 0

def get_angle(dummy):
    global pulses

    if out_a.value() == out_b.value():  # maybe add/remove "not" depending on which direction we define as +
        pulses += 1
    else:
        pulses -= 1
    
    print(pulses)
        
out_a.irq(trigger=Pin.IRQ_RISING, handler=get_angle)