import re
import sys
for l in sys.stdin:
    replacements = re.findall("'.'", l)
    for r in replacements:
        l = l.replace(r, f"0x{ord(r[1]):x}")
    print(l, end="")