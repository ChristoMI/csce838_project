from subprocess import call

DISABLE_WDT_TIMER = 4

wdt_timer_disable_result = call("/home/car/ryan_brown_ws/src/bluetooth_com/src/wdt " + str(DISABLE_WDT_TIMER), shell = True)